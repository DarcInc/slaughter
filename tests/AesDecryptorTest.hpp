//
// Created by phoehne on 8/28/22.
//

#include <cppunit/extensions/HelperMacros.h>

#ifndef SLAUGHTER_AESDECRYPTORTEST_HPP
#define SLAUGHTER_AESDECRYPTORTEST_HPP


class AesDecryptorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AesDecryptorTest);
        CPPUNIT_TEST(testSimpleDecrypt);
        CPPUNIT_TEST(testSimpleMessageDecrypt);
    CPPUNIT_TEST_SUITE_END();

    unsigned char* m_sampleData;
    int nibble(char data);
public:
    void setUp() override;
    void tearDown() override;

    void testSimpleDecrypt();
    void testSimpleMessageDecrypt();
};


#endif //SLAUGHTER_AESDECRYPTORTEST_HPP
