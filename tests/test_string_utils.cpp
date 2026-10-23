#include <gtest/gtest.h>
#include <string>

#include "string_utils.h"


TEST (StringUtilsTest, SplitStandardLine) 
{
    const std::string text("NET VCC U1.8");
    auto tokens = split(text);

    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "NET");
    EXPECT_EQ(tokens[2], "U1.8");
}


TEST (StringUtilsTest, SplitHandlesExtraWhitespace) 
{
    const std::string text("  NET \t VCC  ");
    auto tokens = split(text);

    EXPECT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[1], "VCC");
}


TEST (StringUtilsTest, SplitEmptyLineReturnsEmpty) 
{
    const std::string text("");
    auto tokens = split(text);

    EXPECT_EQ(tokens.size(), 0u);
}


TEST (StringUtilsTest, SplitStandardPinRef) 
{
    const std::string text("U1.8");
    auto pairs = splitPinRef(text);

    EXPECT_EQ(pairs.first, "U1");
    EXPECT_EQ(pairs.second, "8");
}


TEST (StringUtilsTest, SplitPinRefUsesLastDot)
{
    const std::string text("U.1.A1");
    auto pairs = splitPinRef(text);

    EXPECT_EQ(pairs.first, "U.1");
    EXPECT_EQ(pairs.second, "A1");
}


TEST (StringUtilsTest, SplitPinRefRejectsMissingDot)
{
    const std::string text1("U18");
    const std::string text2(".8");
    const std::string text3("U1.");

    EXPECT_THROW(splitPinRef(text1), std::invalid_argument);
    EXPECT_THROW(splitPinRef(text2), std::invalid_argument);
    EXPECT_THROW(splitPinRef(text3), std::invalid_argument);
}
