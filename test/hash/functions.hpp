/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_SYSTEM_TEST_HASH_HPP
#define LIBBITCOIN_SYSTEM_TEST_HASH_HPP

// ripemd_tests
// ----------------------------------------------------------------------------

struct ripemd_test
{
    data_chunk data;
    const short_hash expected;
};

static const std_array<ripemd_test, 21> ripemd_tests
{
    ripemd_test
    {base16_chunk(""), base16_array("9c1185a5c5e9fc54612808977ee8f548b2258d31")},
    {base16_chunk("d5c2b51c22c73a03f316c980880a4ce564e80ed716d54d54ddc43181c8f0512617492f37894640940b1184e14d"), base16_array("306e7d59121745f5d570c5aef029f521276455ce")},
    {base16_chunk("327b8d8ec5d615581e334abdc90dc86f647e0e6e0c136015ef7307d83a2d72"), base16_array("cc8e4f0af6057cff56cca797216cf312bb47da73")},
    {base16_chunk("d3d9a60e3dc1f5ced2a961786f"), base16_array("9b7378b66961a4103fa851ee5533acda4bf27b81")},
    {base16_chunk("6416828b23c1327ff88f63"), base16_array("5ec8f40f77037a5f1c83368111e31921542b485e")},
    {base16_chunk("246f84cfc13b53d4cce6229e6b684e16e2450a411d93"), base16_array("7da76c22ee062bd6fd59fade9b723a0051fa15ac")},
    {base16_chunk("b772474743eca0d41f94ae40cd57b0f9e64a3d51f2c99e473d78de54bfb398057a"), base16_array("6acdbcf1ac21be3c6472a83293404ccda2eb06f7")},
    {base16_chunk("a33705b8833a535ada"), base16_array("cddf70a264a374513a8ad363a55f79d393de75a2")},
    {base16_chunk("5e"), base16_array("53b92648727c2ec7f961a961786c1cf83540927d")},
    {base16_chunk("ab4c1cc2a9ab7815241a29d95dba37b7f3263befaee248c1614832917c18deff9794"), base16_array("ff3c8e53be8fa0c02e08abd0e2e370932859c6ba")},
    {base16_chunk("138700cef9f6e79c726e905746d0eb3eab4272380086e35c7772a429f2e99de0e4350eaeab56a8ce8596c286fbf572"), base16_array("50416b50b8b581b44624efdde7e7ec50680e8718")},
    {base16_chunk("0370b2576696fdf206c9d6ae1581a6217d38"), base16_array("5b2984f2cec13c2c4626c78f5612884c73df629c")},
    {base16_chunk("822ca89d7ad99c51"), base16_array("a34b70f1acaabc02ffeea2f88967a5ee1a1b40a4")},
    {base16_chunk("9dbc66f727723493d7d462e5697ead73e0b386d8cbb187b7490e87e53525a3399b604c29"), base16_array("d969a802332e3be8c8254c5f24cf80e9e000678a")},
    {base16_chunk(""), base16_array("9c1185a5c5e9fc54612808977ee8f548b2258d31")},
    {base16_chunk("2062a5b9648ee5a70b2523311d6188666671ebb28090ef72061ded0cfe006099cd5546dbf7"), base16_array("8d699ff496fb01870c7c877c5f883e98329dba8e")},
    {base16_chunk("05a33c8c098c03fb14532eaa9538530b938d3b69782c1a89b2c9166e89cf7772228ea2822f3ef06f0f92bd4adaee109f"), base16_array("3c467471ee1c19c93d0bb184dd89c86d9b13174d")},
    {base16_chunk("b44a1a6a2a9c9b3e82711225372b0cd60e88bf7be9ced35459ea525da0bb6dfc57117fde2566b2606682"), base16_array("6ffd7e4188245ba250677b9c0d876e39ebb04c2b")},
    {base16_chunk("4fef1cade9f13670c461b32d70e0549bb966b12680fd0ee40ea33fb4b2948a2bd35e195dfb1f69b5c54fe1f5bb1f0d21d0"), base16_array("b55f71c9194073973df18691ebec78a7a2c2c6e1")},
    {base16_chunk("b96c28c68798081dafff4f274a78f2cc9e3ba4e332bb"), base16_array("22f40e4e4921f6d7eccec5cdc269031f3623e912")},
    {base16_chunk("59ce28d8016e7ebb1dedbfef16686735ed65"), base16_array("0a71887812c3d5c241bcd651a736a0fe9baa86fd")}
};

// sha1_tests
// ----------------------------------------------------------------------------

struct sha1_test
{
    const data_chunk data;
    const short_hash expected;
};

static const std_array<sha1_test, 21> sha1_tests
{
    sha1_test
    {base16_chunk(""), base16_array("da39a3ee5e6b4b0d3255bfef95601890afd80709")},
    {base16_chunk("27ed20cb2fa1c9093ebf82427b2abff42cbd"), base16_array("6a56c8684a4a5f37fee6d91526696aded7c0aa0a")},
    {base16_chunk("ddba0a9bc3e53d6ef1c34c11031b54fdce18d54d0c2fb59fc80f0af4314e6916d0b0c5ae9ad8"), base16_array("68e277e7fa9bc9d5ccfbceb08f57cf83fde883c0")},
    {base16_chunk("aa9224a8e046e5b2"), base16_array("ff409fff25b6ae55ac96994427eab6eaafd67559")},
    {base16_chunk("14b7ae683c371ccd6436f4742075578dc599dfc3a437ec8b"), base16_array("36759eabb931b80fbbca8e6e533fca72388bd8f2")},
    {base16_chunk("f9c8eda189222a4e46f65c36a64cae11b26aa531121aa6697fd93504f9aecac893a47dd9"), base16_array("7e619f364df8159ed0e55edeb5008d77cbfb7574")},
    {base16_chunk("e35af7be6135f20cebee722c6cdbafe8620a7cd796d3fbf6ac1cf81eae17faeebe80"), base16_array("4c8770cb9f6ed23b4603990de3b12f1ec437f1bf")},
    {base16_chunk("fdac4bc3951ee0ebad89ce3806496468ce495c91b1f2e206822cf18ebe3c8bd1319e1ef87d"), base16_array("abc658273513f036231c3ac93d693e520ba289cf")},
    {base16_chunk("a04117c5defe8bdc4be999f3c8889f74af9429ef2a6808c462b48c8400a8ac4bf1397ccfcff0e03ebd"), base16_array("f12c13bcc078f864baf9eda3a0f551828006cab9")},
    {base16_chunk("46bf37cf595b18e6553ee97254734a7a704c6db89833cd5ebe7fdf8659"), base16_array("4840799d2cd8ede95c43e701530c83f84cdbd31c")},
    {base16_chunk("c158cd2ed7c0dced347c1f9a619d1c8dc382b4e539400cc9aefb97d6083a141c"), base16_array("b3bedc3d40dbd0991d31af668f2bc946fe2cc2b5")},
    {base16_chunk("1848614e29ff5920e8a7e1ba31fcd2c2c3c7d07b7c"), base16_array("2a5a51e2f2e1c90b303054fe41f57bd25258a13c")},
    {base16_chunk("b34d4bef16f613861b7ad624ebdebd99517e"), base16_array("9f0cefbc13cd853394c7b02de97f5f436ea3918d")},
    {base16_chunk("4ee01f8feee0a6ac275f58b85ceab49c603b78ff606703cab57c52fc7617930366a56c5daed2f3e17e0ee2da4ce8b8"), base16_array("17ecd1fb6df59261017650bc1fd8dffb11594d3f")},
    {base16_chunk("a7afbeb9491d91d679f843c7"), base16_array("d6ad4a2a306137c7851dc6ef9b2fd0af20ecbfd1")},
    {base16_chunk("821161f3c04b6b11c4d1a4"), base16_array("128a1700ae4590fe7b982a53b16334190f21c4b7")},
    {base16_chunk("d9cdc2bb692281ea033489ab4cf08a0ea7e934edb4f9b135bcdae7da484c399e463490"), base16_array("7848f419a5da361142fc46646866272bfa7c6552")},
    {base16_chunk("99a74cb1645b7ed583a5c077a0545586f55cb2629109b7f10dce77a91ef3285c"), base16_array("c5ddea4614c2655844f6597fabb64230e2e8232b")},
    {base16_chunk("cb9779"), base16_array("33274f991e78de247db476c70d88f889c23cd0eb")},
    {base16_chunk("0b681b18aa5a44b9a1b245ed10"), base16_array("61e8608b603391ac90a543d3b531662ccef29f16")},
    {base16_chunk("70206feaa65059d63e2d5807f92b49e3ce183ba3c4185c03944e6a366119409873cf"), base16_array("643ca9c7b4774128514be660c0304d4dec5cc7ca")}
};

// sha256_tests
// ----------------------------------------------------------------------------

struct sha256_test
{
    const data_chunk data;
    const hash_digest expected;
};

static const std_array<sha256_test, 21> sha256_tests
{
    sha256_test
    {base16_chunk(""), base16_array("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")},
    {base16_chunk("f0d5e1ba902e341a58320a08078af4311e1291dcffd97f00c06988e0e26a071832"), base16_array("1f23e6ce087e16c4f3585fbe89a4be22961b413076eea1621a81fefaf0c33ecc")},
    {base16_chunk("a5c082e4489c6aa415a961a1e2d0cbf12ed671e5d10d97fb6e63b852b056396670c336d21c75eca4ab0527023b59656a2136"), base16_array("ad794747682bf286993167ba8b46a0c2b3ad888b91f2917e29832bee1261fcf0")},
    {base16_chunk("c7cbf76a7f875446a32bc8c2a7ce295e1acf85eb10ad66c682b154f08acaa10262ef"), base16_array("997b848f26a6de7efd214c47f0d56dc95b656ec519ebded493f51c2f5166dbb6")},
    {base16_chunk("2a3333448873620c921c47ae387b1768c7b5227fdb4af5512756a6ba0eab6e84be95da98"), base16_array("15fbe855e1343185950e6d8b6d907c24e3d8f0d769be537d8b8f239ae33c8926")},
    {base16_chunk("650fb10d5d392f14456bf2f301e8af92bb8a51164b28"), base16_array("e7abad46a85893bf44e464626473b432ca4e87c94b1e462270ba313d16441bf5")},
    {base16_chunk("0c8fd399"), base16_array("e2d710db0ccb875f2069683862f743312a5ed223d1e209f5ec27becb9821226a")},
    {base16_chunk("9aa42e8eb0bd"), base16_array("13ed61ce40d00e1c50573392f16170afd77c43c34211e3f601f044059c3377e3")},
    {base16_chunk("4512b0693593a91b02b06bf0c137"), base16_array("f0b0c8dfdcc4e31e756a820066e61e383c4159da3278baee23aeb4cfb5add765")},
    {base16_chunk(""), base16_array("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")},
    {base16_chunk("98d44f8c45db013d5e892af5db3314122bd4ba22a004bd64003de55c8a561e3e98e4de2fc6c90cd6ff63"), base16_array("ad36ae65b0d60df776bd84cb976437ebbe66b1a5c8ebc12958ec67b62113166a")},
    {base16_chunk("661857e3dbd1903488575f89ad46bc914145b55bf62b3278fc9ce9a691f42fb83011"), base16_array("00c4fe1f581d8c48138c246178b14899bcac98640347a13d28fad5ae9a234a88")},
    {base16_chunk("4942d8d8625f50d3f6fe973e3456ee67f6439be16b3f98e2bd37730141562f0fb28ac3476bc2e5ff17a1fee447d38058"), base16_array("8a3262616425b9fc15f730fbd74d39abb04bac89812ca41a8c299a703e5e2e61")},
    {base16_chunk("d12efc20aa76955c10f6fca2e52d80558f"), base16_array("0cddc4a3f09b14c374f7f12ee9b9169e8bde407f46ec85c789c556bd4fbc09a7")},
    {base16_chunk("a9b2a310ea57fca020d5759d"), base16_array("9daf4a1a21bf3efb018186300e23943d21803707596490578ee7dc69d0ac3b20")},
    {base16_chunk("baa23f3228e6d0c087f19e31baf04d907f5d"), base16_array("d955f9374fff4141174695791bded97ca9710c2496beee019207d93bc83dd0e0")},
    {base16_chunk("1b8f20c61ef4b5c82aaac6748e5073c2bd8a2e42bc6079746be3c91804ccad713ffaf2daf940ad63bd"), base16_array("66dbc28bec9a41e79483c96e396c586575c7f6f83c91f50a650c4aa696611646")},
    {base16_chunk("fb89f0f61023de0f133ad0b18deef86337a8861f2dc50cfb76d2f0f4a4ad3e3edda87198c19452f3c8b1dda58d"), base16_array("044ed06e0272fe15a8f0bb8bf5c817c14880bdd293597d47f1039a4815424f4d")},
    {base16_chunk("279b6543be0a72dda676c5ff99da02f227637b"), base16_array("f8f7394e8665d30c90870c742363baa529a5053aae1b9438d31c22f5184141a9")},
    {base16_chunk("26d34b5f6d0e23bc1e4fdf11a00d8c"), base16_array("73b12a2fde6b7d790f7da3eb60f990208f28fabf43380a66cf26b615c7f6f545")},
    {base16_chunk("a475bc116efb92cde208e19af68dd00e28f62e27836d28cc41ff4571391ee21379069e4632599d75"), base16_array("cc7ef1dd07f26065caeab1a9dbd820db31448812d3cf5d1a592b8b263e493a47")}
};

// hmac_sha256_tests
// ----------------------------------------------------------------------------

struct hmac_sha256_test
{
    const std::string text;
    const std::string key;
    const hash_digest expected;
};

static const std_array<hmac_sha256_test, 1> hmac_sha256_tests
{
    hmac_sha256_test
    {
        "data",
        "key",
        base16_array("5031fe3d989c6d1537a013fa6e739da23463fdaec3b70137d828e36ace221bd0")
    }
};

// pbkd_sha256_tests
// ----------------------------------------------------------------------------

struct pbkd_sha256_test
{
    const std::string passphrase;
    const std::string salt;
    const size_t count;
    const long_hash expected;
};

static const std_array<pbkd_sha256_test, 2> pbkd_sha256_tests
{
    pbkd_sha256_test
    {
        "passwd",
        "salt",
        1,
        base16_array("55ac046e56e3089fec1691c22544b605f94185216dde0465e68b9d57c20dacbc49ca9cccf179b645991664b39d77ef317c71b845b1e30bd509112041d3a19783")
    },
    {
        "Password",
        "NaCl",
        80000,
        base16_array("4ddcd8f60b98be21830cee5ef22701f9641a4418d04c0414aeff08876b34ab56a1d425a1225833549adb841b51c9b3176a272bdebba1d078478f62b397f33c8d")
    }
};

// sha512_tests
// ----------------------------------------------------------------------------

struct sha512_test
{
    const std::string data;
    const long_hash expected;
};

static const std_array<sha512_test, 1> sha512_tests
{
    sha512_test
    {
        "data",
        base16_array("77c7ce9a5d86bb386d443bb96390faa120633158699c8844c30b13ab0bf92760b7e4416aea397db91b4ac0e5dd56b8ef7e4b066162ab1fdc088319ce6defc876")
    }
};

// hmac_sha512_tests
// ----------------------------------------------------------------------------

struct hmac_sha512_test
{
    const std::string text;
    const std::string key;
    const long_hash expected;
};

static const std_array<hmac_sha512_test, 1> hmac_sha512_tests
{
    hmac_sha512_test
    {
        "data",
        "key",
        base16_array("3c5953a18f7303ec653ba170ae334fafa08e3846f2efe317b87efce82376253cb52a8c31ddcde5a3a2eee183c2b34cb91f85e64ddbc325f7692b199473579c58")
    }
};

// pbkd_sha512_tests
// ----------------------------------------------------------------------------

struct pbkd_sha512_test
{
    const std::string passphrase;
    const std::string salt;
    const size_t count;
    const long_hash expected;
};

static const std_array<pbkd_sha512_test, 5> pbkd_sha512_tests
{
    pbkd_sha512_test
    {
        "password",
        "salt",
        1,
        base16_array("867f70cf1ade02cff3752599a3a53dc4af34c7a669815ae5d513554e1c8cf252c02d470a285a0501bad999bfe943c08f050235d7d68b1da55e63f73b60a57fce")
    },
    {
        "password",
        "salt",
        2,
        base16_array("e1d9c16aa681708a45f5c7c4e215ceb66e011a2e9f0040713f18aefdb866d53cf76cab2868a39b9f7840edce4fef5a82be67335c77a6068e04112754f27ccf4e")
    },
    {
        "password",
        "salt",
        4096,
        base16_array("d197b1b33db0143e018b12f3d1d1479e6cdebdcc97c5c0f87f6902e072f457b5143f30602641b3d55cd335988cb36b84376060ecd532e039b742a239434af2d5")
    },
    {
        "passwordPASSWORDpassword",
        "saltSALTsaltSALTsaltSALTsaltSALTsalt",
        4096,
        base16_array("8c0511f4c6e597c6ac6315d8f0362e225f3c501495ba23b868c005174dc4ee71115b59f9e60cd9532fa33e0f75aefe30225c583a186cd82bd4daea9724a3d3b8")
    },
    {
        "password",
        "NaCL",
        1,
        base16_array("73decfa58aa2e84f94771a75736bb88bd3c7b38270cfb50cb390ed78b305656af8148e52452b2216b2b8098b761fc6336060a09f76415e9f71ea47f9e9064306")
    }
};

// scrypt_tests
// ----------------------------------------------------------------------------

struct scrypt_test
{
    const data_array<80> data;
    const hash_digest expected;
};

static const std_array<scrypt_test, 5> scrypt_tests
{
    scrypt_test
    {
        base16_array("020000004c1271c211717198227392b029a64a7971931d351b387bb80db027f270411e398a07046f7d4a08dd815412a8712f874a7ebf0507e3878bd24e20a3b73fd750a667d2f451eac7471b00de6659"),
        base16_array("065898d7ab2daa8235cdda9511d248f3010b5e11f682f80741ef2b0000000000")
    },
    {
        base16_array("0200000011503ee6a855e900c00cfdd98f5f55fffeaee9b6bf55bea9b852d9de2ce35828e204eef76acfd36949ae56d1fbe81c1ac9c0209e6331ad56414f9072506a77f8c6faf551eac7471b00389d01"),
        base16_array("94fc881c9ff1da50d235ed28f2bbcfddfeb7084e63ebd5bd110d3a0000000000")
    },
    {
        base16_array("02000000a72c8a177f523946f42f22c3e86b8023221b4105e8007e59e81f6beb013e29aaf635295cb9ac966213fb56e046dc71df5b3f7f67ceaeab24038e743f883aff1aaafaf551eac7471b0166249b"),
        base16_array("81caa81451ddf8659cf2afd8599d2d6c8a724432e188f295f8400b0000000000")
    },
    {
        base16_array("010000007824bc3a8a1b4628485eee3024abd8626721f7f870f8ad4d2f33a27155167f6a4009d1285049603888fe85a84b6c803a53305a8d497965a5e896e1a00568359589faf551eac7471b0065434e"),
        base16_array("fe05e1971818866adc7e8e6e2fd7e8d8991e032349cd91580007300000000000")
    },
    {
        base16_array("0200000050bfd4e4a307a8cb6ef4aef69abc5c0f2d579648bd80d7733e1ccc3fbc90ed664a7f74006cb11bde87785f229ecd366c2d4e44432832580e0608c579e4cb76f383f7f551eac7471b00c36982"),
        base16_array("8cec00384d72c7e74f5b340d73af02fa47cb0c13c7afa426b4f0180000000000")
    }
};

#endif
