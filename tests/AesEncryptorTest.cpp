//
// Created by phoehne on 8/29/22.
//

#include "AesEncryptorTest.hpp"
#include <AesEncryptor.hpp>
#include <AesDecryptor.hpp>


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

    const std::string message = "This is the time for all good men to come to the aid";
    auto cipherText = encryptor.encrypt((void*)message.c_str(), message.length() + 1, "foobar");

    int len = message.length() + 1;
    len = len % 16 ? ((len / 16) + 1) * 16 : len;
    len += 16;

    auto clearText = decryptor.decrypt(cipherText.get(), len, "foobar");
    std::string actual((char*)clearText.get());

    CPPUNIT_ASSERT_EQUAL(message, actual);
}
