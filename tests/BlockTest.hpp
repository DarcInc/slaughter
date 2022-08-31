//
// Created by Paul Hoehne on 8/30/22.
//

#include <cppunit/extensions/HelperMacros.h>

#ifndef SLAUGHTER_BLOCKTEST_HPP
#define SLAUGHTER_BLOCKTEST_HPP


class BlockTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BlockTest);
        CPPUNIT_TEST(testDefaultConstructor);
        CPPUNIT_TEST(testSizeConstructor);
        CPPUNIT_TEST(testMoveConstructor);
        CPPUNIT_TEST(testInitializeWithData);
        CPPUNIT_TEST(testMoveAssignment);
        CPPUNIT_TEST(testSimpleWrite);
        CPPUNIT_TEST(testExpandingWrite);
        CPPUNIT_TEST(testExpandingLargeWrite);
        CPPUNIT_TEST(testSimpleRead);
        CPPUNIT_TEST(testSeek);
        CPPUNIT_TEST(testReadFromOffset);
        CPPUNIT_TEST(testWriteAtOffset);
        CPPUNIT_TEST(testWriteAtOffsetAndExpand);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp() override;
    void tearDown() override;

    void testDefaultConstructor();
    void testSizeConstructor();
    void testMoveConstructor();
    void testInitializeWithData();
    void testMoveAssignment();
    void testSimpleWrite();
    void testExpandingWrite();
    void testExpandingLargeWrite();
    void testSimpleRead();
    void testSeek();
    void testReadFromOffset();
    void testWriteAtOffset();
    void testWriteAtOffsetAndExpand();
};


#endif //SLAUGHTER_BLOCKTEST_HPP
