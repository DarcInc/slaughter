//
// Created by phoehne on 8/28/22.
//

#include "AesDecryptorTest.hpp"

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <sstream>

#include <AesDecryptor.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(AesDecryptorTest);

// Output from openssl when the test string was encrypted.  Useful for debuggin
// in memory values.
//
// salt=807E00B0CF291321
// key=44606F38F8A3DD84E515B7111419056251AC1EACDF178A992F2D498B4EBB1410
// iv =01057D6475975CBF1140723A4D75987A

const static std::string sampleTextHex = std::string("53616c7465645f5f807e00b0cf291321") +
                                                     "873515fd18c688f92fe2e849c3eb6f0d" +
                                                     "5565a21a5bac898cd9fda1861d0aa28a" +
                                                     "6949451973aad5995eeaf293d3df1725";
const static std::string sampleTextB64 = std::string("U2FsdGVkX1+AfgCwzykTIYc1Ff0Yxoj5L") +
        "+LoScPrbw1VZaIaW6yJjNn9oYYdCqKKaUlFGXOq1Zle6vKT098XJQ==";

const static std::string passphrase = "foobar";

struct membuf: std::streambuf {
    membuf(char const* base, size_t size) {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }
};
struct imemstream: virtual membuf, std::istream {
    imemstream(char const* base, size_t size)
            : membuf(base, size)
            , std::istream(static_cast<std::streambuf*>(this)) {
    }
};


int AesDecryptorTest::nibble(char data) {
    if (::isdigit(data)) {
        return data - '0';
    }

    return data - 'a' + 10;
}

void AesDecryptorTest::setUp() {
    TestFixture::setUp();

    m_sampleData = (unsigned char *) malloc(sampleTextHex.size() / 2);
    for (int i = 0; i < sampleTextHex.size() / 2; i++) {
        unsigned char someByte = 0;
        char upper = sampleTextHex[i * 2];
        char lower = sampleTextHex[i * 2 + 1];

        someByte += nibble(upper) << 4;
        someByte += nibble(lower);

        m_sampleData[i] = someByte;
    }
}

void AesDecryptorTest::tearDown() {
    TestFixture::tearDown();
    free(m_sampleData);
}

void AesDecryptorTest::testSimpleDecrypt() {
    imemstream buffer((const char*)m_sampleData, sampleTextHex.size()/2);

    Stashing::AesDecryptor decryptor;
    std::ostringstream oss;
    decryptor.descrypt(buffer, oss, passphrase);

    CPPUNIT_ASSERT_EQUAL(std::string("The quick brown fox jumped over the lazy dog"), oss.str());
}
