#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>

#include "design.h"
#include "parser.h"
#include "analysis.h"


namespace
{
    Design makeDesign(
        const std::string& text
    )
    {    
        std::istringstream in(text);
        Design design;
        parseNetlist(in, design);

        return design;
    }


    std::vector<std::string> names(
        const std::vector<PinRef>& refs
    )
    {
        std::vector<std::string> results;

        for (const PinRef& ref: refs)
        {
            results.push_back(
                ref.component->getName() + 
                "." +
                ref.pin->getNumber()
            );
        }
        
        return results;
    }
}


TEST (AnalysisTest, FindsFloatingPinsAndDanglingNets)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
        "NET N1 U1.1 R1.1\n"
        "NET LONELY U1.2\n"
    );

    EXPECT_EQ(names(findFloatingPins(design)),
              (std::vector<std::string>{"R1.2"}));

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 1u);
    EXPECT_EQ(dangling[0]->getName(), "LONELY");
}


TEST (AnalysisTest, EmptyFloatingPinsAndDanglingNets)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
        "NET N1 U1.1 R1.1\n"
        "NET N2 U1.2 R1.2\n"
    );

    ASSERT_EQ(names(findFloatingPins(design)), 
              std::vector<std::string>{});

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 0u);
}


TEST (AnalysisTest, EmptyDesignWithEmptyFloatingPinsAndDanglingNets)
{
    Design design = makeDesign("");

    ASSERT_EQ(names(findFloatingPins(design)), 
              std::vector<std::string>{});

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 0u);
}


TEST (AnalysisTest, PinsWithNoConnectionToAnyNets)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
    );

    auto floating_pin_names = names(findFloatingPins(design));

    ASSERT_EQ(floating_pin_names.size(), 4u);
    EXPECT_EQ(floating_pin_names[0], "U1.1");
    EXPECT_EQ(floating_pin_names[1], "U1.2");
    EXPECT_EQ(floating_pin_names[2], "R1.1");
    EXPECT_EQ(floating_pin_names[3], "R1.2");

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 0u);
}


TEST (AnalysisTest, NetWithoutAnyConnectionToPins)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
        "NET EMPTY\n"
    );

    ASSERT_EQ(names(findFloatingPins(design)), 
              (std::vector<std::string>{"U1.1", "U1.2", "R1.1", "R1.2"}));

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 1u);
    EXPECT_EQ(dangling[0]->getName(), "EMPTY");
}


TEST (AnalysisTest, NetWithDuplicatePins)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
        "NET A U1.1 U1.1\n"
    );

    ASSERT_EQ(names(findFloatingPins(design)), 
              (std::vector<std::string>{"U1.2", "R1.1", "R1.2"}));

    auto dangling = findDanglingNets(design);
    ASSERT_EQ(dangling.size(), 1u);
    EXPECT_EQ(dangling[0]->getName(), "A");
}


TEST (AnalysisTest, SamePinConnectWithMultipleNets)
{
    Design design = makeDesign(
        "COMP U1 1=A 2=B\n"
        "COMP R1 1=A 2=B\n"
        "NET A U1.1 R1.1\n"
        "NET B U1.1 R1.2\n"
    );

    EXPECT_EQ(
        names(findFloatingPins(design)),
        (std::vector<std::string>{"U1.2"})
    );
    EXPECT_TRUE(findDanglingNets(design).empty());
}
