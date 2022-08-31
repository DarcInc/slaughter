//
// Created by Paul Hoehne on 8/30/22.
//

#include "BlockTest.hpp"

#include <Block.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(BlockTest);

void BlockTest::setUp() {
    TestFixture::setUp();
}

void BlockTest::tearDown() {
    TestFixture::tearDown();
}

void BlockTest::testDefaultConstructor() {
    Stashing::Block b;

    CPPUNIT_ASSERT_EQUAL(256ul, b.capcity());
    CPPUNIT_ASSERT_EQUAL(0ul, b.pos());
    CPPUNIT_ASSERT_EQUAL(0ul, b.used());
}

void BlockTest::testSizeConstructor() {
    Stashing::Block b(1024);

    CPPUNIT_ASSERT_EQUAL(1024ul, b.capcity());
    CPPUNIT_ASSERT_EQUAL(0ul, b.pos());
    CPPUNIT_ASSERT_EQUAL(0ul, b.used());
}

void BlockTest::testMoveConstructor() {
    Stashing::Block b(1024);
    Stashing::Block c(std::move(b));

    CPPUNIT_ASSERT_EQUAL(0ul, b.capcity());
    CPPUNIT_ASSERT_EQUAL(1024ul, c.capcity());
}

void BlockTest::testInitializeWithData() {
    std::string message("Hello World");
    Stashing::Block b((void*)message.c_str(), message.length());

    CPPUNIT_ASSERT_EQUAL(message.length(), b.capcity());
    CPPUNIT_ASSERT_EQUAL(0ul, b.pos());
    CPPUNIT_ASSERT_EQUAL(message.length(), b.used());

    auto data = b.raw();
    CPPUNIT_ASSERT(::memcmp(message.c_str(), data.get(), message.length()) == 0);
}

void BlockTest::testMoveAssignment() {
    auto someFunction = []{
        return Stashing::Block(4096);
    };

    Stashing::Block b(8192);
    b = someFunction();

    CPPUNIT_ASSERT_EQUAL(4096ul, b.capcity());
}

void BlockTest::testSimpleWrite() {
    Stashing::Block b;
    std::string message("This is a message");
    auto bytesWritten = b.write((void*)message.c_str(), message.length());

    CPPUNIT_ASSERT_EQUAL(256ul, b.capcity());
    CPPUNIT_ASSERT_EQUAL(message.length(), b.pos());
    CPPUNIT_ASSERT_EQUAL(message.length(), b.used());
    CPPUNIT_ASSERT_EQUAL(message.length(), bytesWritten);

    auto data = b.raw();
    CPPUNIT_ASSERT(::memcmp(message.c_str(), data.get(), message.length()) == 0);
}

void BlockTest::testExpandingWrite() {
    Stashing::Block b(16);
    CPPUNIT_ASSERT_EQUAL(16ul, b.capcity());

    std::string message("The quick brown fox jumped over the lazy dog");
    auto bytesWritten = b.write((void*)message.c_str(), message.length());
    CPPUNIT_ASSERT_EQUAL(8192ul, b.capcity());
    CPPUNIT_ASSERT_EQUAL(message.length(), b.pos());
    CPPUNIT_ASSERT_EQUAL(message.length(), b.used());
    CPPUNIT_ASSERT_EQUAL(message.length(), bytesWritten);

    auto data = b.raw();
    CPPUNIT_ASSERT(::memcmp(message.c_str(), data.get(), message.length()) == 0);
}

void BlockTest::testExpandingLargeWrite() {
    Stashing::Block b;
    void* testData = malloc(65*1024);
    memset(testData, 0xef, 65*1024);

    auto bytesWritten = b.write(testData, 65*1024);
    CPPUNIT_ASSERT_EQUAL(128ul*1024, b.capcity());
    CPPUNIT_ASSERT_EQUAL(65ul*1024, b.pos());
    CPPUNIT_ASSERT_EQUAL(65ul*1024, b.used());
    CPPUNIT_ASSERT_EQUAL(65ul*1024, bytesWritten);

    auto data = b.raw();
    CPPUNIT_ASSERT(::memcmp(testData, data.get(), 64*1024) == 0);
}

void BlockTest::testSimpleRead() {
    Stashing::Block b;
    std::string message("Hello World");

    b.write((void*)message.c_str(), message.length());

    unsigned char shortBuff[5];
    unsigned char fullBuff[11];
    unsigned char largeBuff[20];

    b.rewind();
    auto bytesRead = b.read(shortBuff, 5);
    CPPUNIT_ASSERT(::memcmp(message.c_str(), shortBuff, 5) == 0);
    CPPUNIT_ASSERT_EQUAL(5ul, bytesRead);
    CPPUNIT_ASSERT_EQUAL(11ul, b.used());
    CPPUNIT_ASSERT_EQUAL(5ul, b.pos());

    b.rewind();
    bytesRead = b.read(fullBuff, 11);
    CPPUNIT_ASSERT(::memcmp(message.c_str(), fullBuff, 11) == 0);
    CPPUNIT_ASSERT_EQUAL(11ul, bytesRead);
    CPPUNIT_ASSERT_EQUAL(11ul, b.used());
    CPPUNIT_ASSERT_EQUAL(11ul, b.pos());

    b.rewind();
    ::memset(largeBuff, 0xef, sizeof(largeBuff));
    bytesRead = b.read(largeBuff, 20);
    CPPUNIT_ASSERT(::memcmp(message.c_str(), largeBuff, 11) == 0);
    CPPUNIT_ASSERT_EQUAL((unsigned char)0xef, largeBuff[11]);
    CPPUNIT_ASSERT_EQUAL(11ul, bytesRead);
    CPPUNIT_ASSERT_EQUAL(11ul, b.used());
    CPPUNIT_ASSERT_EQUAL(11ul, b.pos());
}

void BlockTest::testSeek() {
    Stashing::Block b;
    std::string message("Hello World");

    b.write((void*)message.c_str(), message.length());

    b.seek(0);
    CPPUNIT_ASSERT_EQUAL(0ul, b.pos());

    b.seek(5);
    CPPUNIT_ASSERT_EQUAL(5ul, b.pos());

    b.seek(20);
    CPPUNIT_ASSERT_EQUAL(11ul, b.pos());
}

void BlockTest::testReadFromOffset() {
    std::string message("Hello Cruel World");
    Stashing::Block b((void*)message.c_str(), message.length());

    b.seek(6);
    unsigned char testBuffer[5];
    b.read(testBuffer, 5);

    CPPUNIT_ASSERT(memcmp("Cruel", testBuffer, 5) == 0);
    CPPUNIT_ASSERT_EQUAL(11ul, b.pos());
}

void BlockTest::testWriteAtOffset() {
    std::string message("Hello Cruel World");
    Stashing::Block b((void*)message.c_str(), message.length());

    b.seek(6);
    b.write((void*)"Happy", 5);

    CPPUNIT_ASSERT_EQUAL(11ul, b.pos());

    auto data = b.raw();
    CPPUNIT_ASSERT(memcmp("Hello Happy World", data.get(), message.length()) == 0);
}

void BlockTest::testWriteAtOffsetAndExpand() {
    Stashing::Block b(32);
    std::string message("0123456789");

    b.write((void*)message.c_str(), message.length());
    b.write((void*)message.c_str(), message.length());
    b.write((void*)message.c_str(), message.length());
    b.seek(b.pos() - 1);
    auto pos = b.pos();

    b.write((void*)message.c_str(), message.length());
    CPPUNIT_ASSERT_EQUAL(39ul, b.used());
    CPPUNIT_ASSERT_EQUAL(39ul, b.pos());
    CPPUNIT_ASSERT_EQUAL(8192ul, b.capcity());

    auto data = b.raw();
    CPPUNIT_ASSERT_EQUAL((unsigned char)'0', data.get()[pos]);
}




