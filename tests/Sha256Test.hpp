//
// Created by phoehne on 8/28/22.
//

#include <cppunit/extensions/HelperMacros.h>

#ifndef SLAUGHTER_SHA256TEST_HPP
#define SLAUGHTER_SHA256TEST_HPP


class Sha256Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Sha256Test);
        CPPUNIT_TEST(testSimpleStreamHash);
        CPPUNIT_TEST(testByteLimitedStreamHash);
        CPPUNIT_TEST(testName);
        CPPUNIT_TEST(testHashSize);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp() override;
    void tearDown() override;

    void testSimpleStreamHash();
    void testByteLimitedStreamHash();
    void testName();
    void testHashSize();
};


#endif //SLAUGHTER_SHA256TEST_HPP
