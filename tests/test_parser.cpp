#include <gtest/gtest.h>
#include <stdexcept>

#include "parser.h"
#include "design.h"


namespace {
    std::string errorOf(const std::string& text) {
        std::istringstream in(text);
        Design design;
        
        try {
            parseNetlist(in, design);

        } catch (const std::runtime_error& e) {
            return e.what();
        }
        
        return "";
    }
}


TEST (ParserTest, ParseStandardNetList)
{
    std::istringstream in(
        "COMP U1 1=IN 2=GND\n"
        "COMP R1 1=A 2=B\n"
        "NET GND U1.2 R1.2\n"
    );

    Design design;

    parseNetlist(
        in,
        design
    );

    EXPECT_TRUE(
        design.findComponent("U1").getName() == "U1"
    );
    EXPECT_EQ(
        &design.findNet("GND").pins()[0].get(), 
        &design.findComponent("U1").findPin("2")
    );
}


TEST (ParserTest, EmptyLinesAndCommentsAreSkipped)
{
    std::istringstream in(
        "\n"
        "# a comment\n"
        "#NoSpaceComment\n"
        "   \n"
    );
    Design design;
    parseNetlist(in, design);

    EXPECT_EQ(design.componentCount(), 0u);
    EXPECT_EQ(design.netCount(), 0u);
}


TEST (ParserTest, ComponentOnlyThrows)
{
    EXPECT_EQ(
        errorOf("COMP\n"), 
        "line 1: COMP requires a name"
    );
}


TEST (ParserTest, NetReferencingMissingComponentThrows)
{
    EXPECT_EQ(
        errorOf("NET GND U9.1\n"), 
        "line 1: component not found: U9"
    );
}


TEST (ParserTest, UnknownKeywordThrows)
{
    EXPECT_EQ(
        errorOf("FOO bar\n"), 
        "line 1: unknown keyword: FOO"
    );
}


TEST (ParserTest, DuplicateComponentThrows)
{
    EXPECT_EQ(
        errorOf(
            "COMP R1 1=A\n"
            "COMP R1 1=A\n"
        ), 
        "line 2: duplicate component name: R1"
    );
}


TEST (ParserTest, MissingNetNameThrows)
{
    EXPECT_EQ(
        errorOf(
            "NET\n"
        ), 
        "line 1: NET requires a name"
    );
}


TEST (ParserTest, InvalidReferenceThrows)
{
    EXPECT_EQ(
        errorOf(
            "NET GND U1\n"
        ), 
        "line 1: invalid reference: U1"
    );
}


TEST (ParserTest, InvalidDefinitionThrows)
{
    EXPECT_EQ(
        errorOf(
            "COMP U1 1=IN 2=\n"
        ), 
        "line 1: Invalid definition: 2="
    );
}


TEST (ParserTest, DuplicatePinNumberThrows)
{
    EXPECT_EQ(
        errorOf(
            "COMP U1 1=A 1=B\n"
        ), 
        "line 1: duplicate pin number: U1.1"
    );
}


TEST (ParserTest, PinNumberNotExistsThrows)
{
    EXPECT_EQ(
        errorOf(
            "COMP R1 1=A\nNET GND R1.9\n"
        ), 
        "line 2: pin not found: R1.9"
    );
}


TEST (ParserTest, NetBeforeComponentThrows)
{
    EXPECT_EQ(
        errorOf(
            "NET GND R1.1\nCOMP R1 1=A\n"
        ), 
        "line 1: component not found: R1"
    );
}


TEST (ParserTest, DuplicateNetThrows)
{
    EXPECT_EQ(
        errorOf(
            "NET GND\nNET GND\n"
        ), 
        "line 2: duplicate net name: GND"
    );
}


TEST (ParserTest, LineBreakNotThrows)
{
    EXPECT_EQ(
        errorOf(
            "COMP R1 1=A\r\n"
        ), 
        ""
    );
}
