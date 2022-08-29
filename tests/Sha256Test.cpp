//
// Created by phoehne on 8/28/22.
//

#include "Sha256Test.hpp"

#include <sstream>

#include <Hash.hpp>
#include <Sha256.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(Sha256Test);

void Sha256Test::setUp() {
    TestFixture::setUp();
}

void Sha256Test::tearDown() {
    TestFixture::tearDown();
}

// To generate the test hash
// echo -n "Once upon a time there was a might horse" | shasum -a 256
void Sha256Test::testSimpleStreamHash() {
    std::istringstream iss("Once upon a time there was a might horse");
    std::string expected = "6a49c23b01d34c51f34b6e661b4f9bfa718fb3a91c3f741fd5ec4b7e9fa44ccf";

    Stashing::Sha256 algo;
    auto result = algo.hash(iss);

    CPPUNIT_ASSERT_EQUAL(expected, result->string());
}

// To generate the test hash
// echo -n "Once upon a time there was a might horse" | head -c 10 | shasum -a 256
void Sha256Test::testByteLimitedStreamHash() {
    std::string expected = "e105b89cc5b45dcf2712c8e970aee30045e01212a217150e786d2512b8d775df";
    std::istringstream iss("Once upon a time there was a might horse");

    Stashing::Sha256 algo;
    auto result = algo.hash(iss, 10);

    CPPUNIT_ASSERT_EQUAL(expected, result->string());
}

void Sha256Test::testName() {
    std::string expected = "Sha256";

    Stashing::Sha256 algo;

    CPPUNIT_ASSERT_EQUAL(expected, algo.name());
}

void Sha256Test::testHashSize() {
    Stashing::Sha256 algo;
    CPPUNIT_ASSERT_EQUAL(32ul, algo.hashSize());
}
