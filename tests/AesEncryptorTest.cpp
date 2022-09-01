//
// Created by phoehne on 8/29/22.
//

#include "AesEncryptorTest.hpp"

#include <AesEncryptor.hpp>
#include <AesDecryptor.hpp>
#include <Block.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(AesEncryptorTest);

void AesEncryptorTest::setUp() {
    TestFixture::setUp();
}

void AesEncryptorTest::tearDown() {
    TestFixture::tearDown();
}


void AesEncryptorTest::testShortString() {
    Stashing::AesEncryptor encryptor;
    Stashing::AesDecryptor decryptor;

    std::string message = "The quick brown fox jumped over the lazy dog";

    auto cipher = encryptor.encrypt((void*)message.c_str(), message.length(), "foobar");
    auto clear = decryptor.decrypt(cipher.get(), "foobar");

    char buffer[clear->used() + 1];
    clear->read((void*)buffer, clear->used());
    buffer[clear->used()] = '\0';


    CPPUNIT_ASSERT_EQUAL(message, std::string(buffer));
}
