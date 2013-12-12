/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) 
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/network/protocol.hpp>

#include <bitcoin/network/hosts.hpp>
#include <bitcoin/network/handshake.hpp>
#include <bitcoin/utility/logger.hpp>

namespace libbitcoin {

using std::placeholders::_1;
using std::placeholders::_2;

static std::string pretty(const ip_address_type& ip)
{
    std::ostringstream oss;
    oss << (int)ip[12] << '.' << (int)ip[13] << '.'
        << (int)ip[14] << '.' << (int)ip[15];
    return oss.str();
}

protocol::protocol(threadpool& pool, hosts& hsts,
    handshake& shake, network& net)
  : strand_(pool.service()), hosts_(hsts), handshake_(shake), network_(net)
{
    channel_subscribe_ = std::make_shared<channel_subscriber_type>(pool);
}

void protocol::set_max_outbound(size_t max_outbound)
{
    max_outbound_ = max_outbound;
}
void protocol::set_hosts_filename(const std::string& hosts_filename)
{
    hosts_filename_ = hosts_filename;
}
void protocol::disable_listener()
{
    listen_is_enabled_ = false;
}

void protocol::start(completion_handler handle_complete)
{
    atomic_counter_ptr count_paths = std::make_shared<atomic_counter>(0);
    bootstrap(strand_.wrap(
        std::bind(&protocol::handle_bootstrap,
            this, _1, count_paths, handle_complete)));
    handshake_.start(strand_.wrap(
        std::bind(&protocol::handle_start_handshake_service,
            this, _1, count_paths, handle_complete)));
}
void protocol::handle_bootstrap(const std::error_code& ec,
    atomic_counter_ptr count_paths, completion_handler handle_complete)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL) << "Failed to bootstrap: " << ec.message();
        handle_complete(ec);
        return;
    }
    if (++(*count_paths) == 2)
    {
        handle_complete(std::error_code());
        run();
    }
}
void protocol::handle_start_handshake_service(const std::error_code& ec,
    atomic_counter_ptr count_paths, completion_handler handle_complete)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Failed to start handshake service: " << ec.message();
        handle_complete(ec);
        return;
    }
    ++(*count_paths);
    if (*count_paths == 2)
    {
        handle_complete(std::error_code());
        run();
    }
}

void protocol::stop(completion_handler handle_complete)
{
    hosts_.save(hosts_filename_,
        strand_.wrap(std::bind(&protocol::handle_save,
            this, _1, handle_complete)));
}
void protocol::handle_save(const std::error_code& ec,
    completion_handler handle_complete)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL) << "Failed to save hosts '"
            << hosts_filename_ << "': " << ec.message();
        handle_complete(ec);
        return;
    }
    channel_subscribe_->relay(error::service_stopped, nullptr);
    handle_complete(std::error_code());
}

void protocol::bootstrap(completion_handler handle_complete)
{
    hosts_.load(hosts_filename_,
        strand_.wrap(std::bind(&protocol::load_hosts,
            this, _1, handle_complete)));
}
void protocol::load_hosts(const std::error_code& ec,
    completion_handler handle_complete)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Could not load hosts file: " << ec.message();
        handle_complete(ec);
        return;
    }
    hosts_.fetch_count(
        strand_.wrap(std::bind(&protocol::if_0_seed,
            this, _1, _2, handle_complete)));
}

void protocol::if_0_seed(const std::error_code& ec, size_t hosts_count,
    completion_handler handle_complete)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Unable to check hosts empty: " << ec.message();
        handle_complete(ec);
        return;
    }
    if (hosts_count == 0)
    {
        load_seeds_ = std::make_shared<seeds>(this);
        load_seeds_->start(handle_complete);
    }
    else
        handle_complete(std::error_code());
}

#ifdef ENABLE_TESTNET
const std::vector<std::string> dns_seeds
{
    "testnet.mojocoin.com",
    // tpfaucet.appspot.com only provides an
    // IP adddress for their node, no DNS record.
    "54.243.211.176"
};
#else
const std::vector<std::string> dns_seeds
{
    "bitseed.xf2.org",
    "dnsseed.bluematt.me",
    "seed.bitcoin.sipa.be",
    "dnsseed.bitcoin.dashjr.org",
    "archivum.info",
    "progressbar.sk",
    "faucet.bitcoin.st",
    "bitcoin.securepayment.cc"
};
#endif

protocol::seeds::seeds(protocol* parent)
  : strand_(parent->strand_), hosts_(parent->hosts_),
    handshake_(parent->handshake_), network_(parent->network_)
{
}
void protocol::seeds::start(completion_handler handle_complete)
{
    handle_complete_ = handle_complete;
    ended_paths_ = 0;
    finished_ = false;
    for (const std::string& hostname: dns_seeds)
        connect_dns_seed(hostname);
}

void protocol::seeds::error_case(const std::error_code& ec)
{
    if (finished_)
        return;
    ++ended_paths_;
    if (ended_paths_ == dns_seeds.size())
    {
        finished_ = true;
        handle_complete_(ec);
    }
}

void protocol::seeds::connect_dns_seed(const std::string& hostname)
{
    connect(handshake_, network_, hostname, protocol_port,
        strand_.wrap(std::bind(&protocol::seeds::request_addresses,
            shared_from_this(), _1, _2)));
}
void protocol::seeds::request_addresses(
    const std::error_code& ec, channel_ptr dns_seed_node)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Failed to connect to seed node: " << ec.message();
        error_case(ec);
    }
    else
    {
        dns_seed_node->send(get_address_type(),
            strand_.wrap(std::bind(&protocol::seeds::handle_send_get_address,
                shared_from_this(), _1)));
        dns_seed_node->subscribe_address(
            strand_.wrap(std::bind(&protocol::seeds::save_addresses,
                shared_from_this(), _1, _2, dns_seed_node)));
    }

}
void protocol::seeds::handle_send_get_address(const std::error_code& ec)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Sending get_address message failed: " << ec.message();
        error_case(ec);
    }
}

void protocol::seeds::save_addresses(const std::error_code& ec,
    const address_type& packet, channel_ptr)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Problem receiving addresses from seed nodes: "
            << ec.message();
        error_case(ec);
    }
    else
    {
        log_debug(LOG_PROTOCOL) << "Storing seeded addresses.";
        for (const network_address_type& net_address: packet.addresses)
            hosts_.store(net_address,
                strand_.wrap(std::bind(&protocol::seeds::handle_store,
                    shared_from_this(), _1)));

        if (!finished_)
        {
            ++ended_paths_;
            finished_ = true;
            handle_complete_(std::error_code());
        }
    }
}
void protocol::seeds::handle_store(const std::error_code& ec)
{
    if (ec)
        log_error(LOG_PROTOCOL)
            << "Failed to store addresses from seed nodes: "
            << ec.message();
}

void protocol::run()
{
    strand_.dispatch(std::bind(&protocol::try_outbound_connects, this));
    if (listen_is_enabled_)
        network_.listen(protocol_port,
            strand_.wrap(std::bind(&protocol::handle_listen, this, _1, _2)));
}
void protocol::try_outbound_connects()
{
    BITCOIN_ASSERT(connections_.empty());
    BITCOIN_ASSERT(connect_states_.empty());
    connect_states_.resize(max_outbound_);
    for (size_t i = 0; i < max_outbound_; ++i)
        try_connect_once(i);
}
void protocol::try_connect_once(slot_index slot)
{
    BITCOIN_ASSERT(connections_.size() <= max_outbound_);
    // Any state can call this method.
    connect_states_[slot] = connect_state::finding_peer;
    hosts_.fetch_address(
        strand_.wrap(std::bind(&protocol::attempt_connect,
            this, _1, _2, slot)));
}

template <typename ConnectionList>
bool already_connected(
    const network_address_type& address,
    const ConnectionList& connections)
{
    // Are we already connected to this address?
    for (const auto& connection: connections)
    {
        if (connection.address.ip == address.ip &&
            connection.address.port == address.port)
        {
            return true;
        }
    }
    return false;
}

void protocol::attempt_connect(const std::error_code& ec,
    const network_address_type& address, slot_index slot)
{
    BITCOIN_ASSERT(connect_states_[slot] == connect_state::finding_peer);
    connect_states_[slot] = connect_state::connecting;
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Problem fetching random address: " << ec.message();
        return;
    }
    if (already_connected(address, connections_))
    {
        log_debug(LOG_PROTOCOL)
            << "Already connected to " << encode_hex(address.ip);
        // Retry another connection
        // Still in same strand.
        try_connect_once(slot);
        return;
    }
    log_debug(LOG_PROTOCOL) << "Trying "
        << pretty(address.ip) << ":" << address.port;
    connect(handshake_, network_, pretty(address.ip), address.port,
        strand_.wrap(std::bind(&protocol::handle_connect,
            this, _1, _2, address, slot)));
}
void protocol::handle_connect(
    const std::error_code& ec, channel_ptr node,
    const network_address_type& address, slot_index slot)
{
    BITCOIN_ASSERT(connect_states_[slot] == connect_state::connecting);
    connect_states_[slot] = connect_state::established;
    BITCOIN_ASSERT(connections_.size() <= max_outbound_);
    if (ec)
    {
        log_warning(LOG_PROTOCOL) << "Unable to connect to "
            << pretty(address.ip) << ":" << address.port
            << " - " << ec.message();
        // Retry another connection
        // Still in same strand.
        try_connect_once(slot);
        return;
    }
    connections_.push_back({address, node});
    log_info(LOG_PROTOCOL) << "Connected to "
        << pretty(address.ip) << ":" << address.port
        << " (" << connections_.size() << " connections)";
    // Remove channel from list of connections
    node->subscribe_stop(
        strand_.wrap(std::bind(&protocol::outbound_channel_stopped,
            this, _1, node, slot)));
    setup_new_channel(node);
}

void protocol::maintain_connection(const std::string& hostname, uint16_t port)
{
    connect(handshake_, network_, hostname, port,
        strand_.wrap(std::bind(&protocol::handle_manual_connect,
            this, _1, _2, hostname, port)));
}
void protocol::handle_manual_connect(
    const std::error_code& ec, channel_ptr node,
    const std::string& hostname, uint16_t port)
{
    if (ec)
    {
        log_warning(LOG_PROTOCOL) << "Manual connection to "
            << hostname << ":" << port << " failed with " << ec.message();
        maintain_connection(hostname, port);
        return;
    }
    manual_connections_.push_back(node);
    // Connected!
    log_info(LOG_PROTOCOL) << "Manual connection established: "
        << hostname << ":" << port;
    // Remove channel from list of connections
    node->subscribe_stop(
        strand_.wrap(std::bind(&protocol::manual_channel_stopped,
            this, _1, node, hostname, port)));
    setup_new_channel(node);
}

void protocol::handle_listen(const std::error_code& ec, acceptor_ptr accept)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Error while listening: " << ec.message();
    }
    else
    {
        accept->accept(
            strand_.wrap(std::bind(&protocol::handle_accept,
                this, _1, _2, accept)));
    }
}
void protocol::handle_accept(const std::error_code& ec, channel_ptr node,
    acceptor_ptr accept)
{
    accept->accept(
        strand_.wrap(std::bind(&protocol::handle_accept,
            this, _1, _2, accept)));
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Problem accepting connection: " << ec.message();
        return;
    }
    accepted_channels_.push_back(node);
    log_info(LOG_PROTOCOL) << "Accepted connection: "
        << accepted_channels_.size();
    auto handshake_complete = [this, node](const std::error_code& ec)
    {
        if (ec)
        {
            log_error(LOG_PROTOCOL) << "Problem with handshake: "
                << ec.message();
            return;
        }
        // Remove channel from list of connections
        node->subscribe_stop(
            strand_.wrap(std::bind(&protocol::inbound_channel_stopped,
                this, _1, node)));
        setup_new_channel(node);
    };
    handshake_.ready(node, handshake_complete);
}

void handle_send(const std::error_code& ec)
{
    if (ec)
        log_error(LOG_PROTOCOL)
            << "Sending error: " << ec.message();
}
void protocol::setup_new_channel(channel_ptr node)
{
    subscribe_address(node);
    node->send(get_address_type(), handle_send);
    // Notify subscribers
    channel_subscribe_->relay(std::error_code(), node);
}

template <typename ConnectionsList>
void remove_connection(ConnectionsList& connections, channel_ptr which_node)
{
    auto it = connections.begin();
    for (; it != connections.end(); ++it)
        if (it->node == which_node)
            break;
    BITCOIN_ASSERT(it != connections.end());
    connections.erase(it);
}
void protocol::outbound_channel_stopped(
    const std::error_code& ec, channel_ptr which_node, slot_index slot)
{
    // We must always attempt a reconnection if this was an
    // outbound connection.
    if (ec) 
        log_error(LOG_PROTOCOL)
            << "Channel stopped (outbound): " << ec.message();
    // Erase this channel from our connections list.
    // And then attempt a reconnection.
    remove_connection(connections_, which_node);
    BITCOIN_ASSERT(connect_states_[slot] == connect_state::established);
    connect_states_[slot] = connect_state::stopped;
    // Attempt a reconnection.
    // Recreate 1 new connection always.
    // Still in same strand.
    try_connect_once(slot);
}

template <typename ChannelsList>
void remove_channel(ChannelsList& channels, channel_ptr which_node)
{
    auto it = std::find(channels.begin(), channels.end(), which_node);
    BITCOIN_ASSERT(it != channels.end());
    channels.erase(it);
}

void protocol::manual_channel_stopped(
    const std::error_code& ec, channel_ptr which_node,
    const std::string& hostname, uint16_t port)
{
    // We must always attempt a reconnection if this was an
    // outbound connection.
    if (ec) 
        log_error(LOG_PROTOCOL)
            << "Channel stopped (manual): " << ec.message();
    // Remove from list and try to reconnect.
    // Timeout logic would go here if we ever need it.
    remove_channel(manual_connections_, which_node);
    // Reconnect again.
    maintain_connection(hostname, port);
}
void protocol::inbound_channel_stopped(
    const std::error_code& ec, channel_ptr which_node)
{
    // We must always attempt a reconnection if this was an
    // outbound connection.
    if (ec) 
    {
        log_error(LOG_PROTOCOL)
            << "Channel stopped (inbound): " << ec.message();
    }
    // Remove from accepted connections.
    remove_channel(accepted_channels_, which_node);
}

void protocol::subscribe_address(channel_ptr node)
{
    node->subscribe_address(
        strand_.wrap(std::bind(&protocol::receive_address_message,
            this, _1, _2, node)));
}
void protocol::receive_address_message(const std::error_code& ec,
    const address_type& packet, channel_ptr node)
{
    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Problem receiving addresses: " << ec.message();
        return;
    }
    log_debug(LOG_PROTOCOL) << "Storing addresses.";
    for (const network_address_type& net_address: packet.addresses)
        hosts_.store(net_address,
            strand_.wrap(std::bind(&protocol::handle_store_address,
                this, _1)));
    node->subscribe_address(
        strand_.wrap(std::bind(&protocol::receive_address_message,
            this, _1, _2, node)));
}
void protocol::handle_store_address(const std::error_code& ec)
{
    if (ec)
        log_error(LOG_PROTOCOL) << "Failed to store address: " << ec.message();
}

void protocol::fetch_connection_count(
    fetch_connection_count_handler handle_fetch)
{
    strand_.post(
        std::bind(&protocol::do_fetch_connection_count,
            this, handle_fetch));
}
void protocol::do_fetch_connection_count(
    fetch_connection_count_handler handle_fetch)
{
    handle_fetch(std::error_code(), connections_.size());
}

void protocol::subscribe_channel(channel_handler handle_channel)
{
    channel_subscribe_->subscribe(handle_channel);
}

} // namespace libbitcoin

