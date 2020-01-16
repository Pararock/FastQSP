#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/variant.h"

using ::testing::Return;
using ::testing::_; // Matcher for parameters

//void qspFreeVariants(QSPVariant*, int);
//QSPVariant qspGetEmptyVariant(QSP_BOOL);
//QSP_BOOL qspConvertVariantTo(QSPVariant*, QSP_BOOL);
//void qspCopyVariant(QSPVariant*, QSPVariant*);
//QSP_BOOL qspIsCanConvertToNum(QSPVariant*);
//int qspAutoConvertCompare(QSPVariant*, QSPVariant*);


TEST(Variant, getEmptStringVariantIsEmptyAndDoesntLeak)
{
    auto variant = qspGetEmptyVariant(true);
    EXPECT_TRUE(variant.IsStr) << L"Empty string variant should be set as a string";
    EXPECT_STREQ(L"", variant.Val.Str) << L"Empty string variant should an empty string";
    qspFreeVariants(&variant, 1);
    EXPECT_EQ(nullptr, variant.Val.Str);
}

TEST(Variant, getEmptyIntVariantIsEmpty)
{
    auto variant = qspGetEmptyVariant(false);
    EXPECT_FALSE(variant.IsStr) << L"Empty int variant should be set as a int";
    EXPECT_EQ(0, variant.Val.Num) << L"Empty int variant should be initalized to 0";
}