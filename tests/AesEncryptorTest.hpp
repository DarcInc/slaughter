//
// Created by phoehne on 8/29/22.
//

#include <cppunit/extensions/HelperMacros.h>

#include <fstream>
#include <iostream>

#ifndef SLAUGHTER_AESENCRYPTORTEST_HPP
#define SLAUGHTER_AESENCRYPTORTEST_HPP


class AesEncryptorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AesEncryptorTest);
        CPPUNIT_TEST(testShortString);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp() override;

    void tearDown() override;

    void testShortString();

};


#endif //SLAUGHTER_AESENCRYPTORTEST_HPP
