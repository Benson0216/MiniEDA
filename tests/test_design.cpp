#include <gtest/gtest.h>
#include <stdexcept>

#include "design.h"
#include "net.h"


TEST (DesignTest, NetHoldsSamePinAsComponent) 
{
    Design design;

    auto& comp = design.addComponent("A1");
    auto& pin1 = comp.addPin(
        "2", "VSS"
    );

    auto& net = design.addNet("VSS");
    net.append(pin1);

    EXPECT_TRUE(&net.pins()[0].get() == &pin1);
}


TEST (ComponentTest, DuplicatePinNumberThrows) 
{
    Component comp("test");
    comp.addPin("1", "pin1");

    EXPECT_THROW(comp.addPin("1", "again"), std::invalid_argument);
}


TEST (ComponentTest, FindMissingPinThrows) 
{
    Component comp("test");

    EXPECT_THROW(comp.findPin("99"), std::invalid_argument);
}


TEST (ComponentTest, FindPinReturnsSameObject) 
{
    Component comp("test");
    auto& pin2 = comp.addPin(
        "2", "pin2"
    );

    EXPECT_TRUE(&comp.findPin("2") == &pin2);
}


TEST (DesignTest, FindComponent)
{
    Design design;
    auto& comp = design.addComponent("C1");

    EXPECT_TRUE(&design.findComponent("C1") == &comp);
}


TEST (DesignTest, FindNet)
{
    Design design;
    auto& net = design.addNet("GND");

    EXPECT_TRUE(&design.findNet("GND") == &net);
}


TEST (DesignTest, DuplicateComponentThrows)
{
    Design design;
    design.addComponent("C1");

    EXPECT_THROW(design.addComponent("C1"), std::invalid_argument);
}


TEST (DesignTest, DuplicateNetThrows)
{
    Design design;
    design.addNet("GPIO");

    EXPECT_THROW(design.addNet("GPIO"), std::invalid_argument);
}


TEST (DesignTest, FindMissingComponentThrows) 
{
    Design design;

    EXPECT_THROW(design.findComponent("C1"), std::invalid_argument);
}


TEST (DesignTest, FindMissingNetThrows) 
{
    Design design;

    EXPECT_THROW(design.findNet("GND"), std::invalid_argument);
}


TEST (DesignTest, ComponentsReturnedInInsertionOrder)
{
    Design design;
    auto& r1 = design.addComponent("R1");
    auto& r2 = design.addComponent("R2");

    auto comps = design.components();

    ASSERT_EQ(comps.size(), 2u);
    EXPECT_EQ(comps[0], &r1);
    EXPECT_EQ(comps[1], &r2);
}


TEST (DesignTest, NetsReturnedInInsertionOrder)
{
    Design design;
    auto& n1 = design.addNet("VSS_1");
    auto& n2 = design.addNet("VSS_2");

    auto nets = design.nets();

    ASSERT_EQ(nets.size(), 2u);
    EXPECT_EQ(nets[0], &n1);
    EXPECT_EQ(nets[1], &n2);
}


TEST (ComponentTest, PinsReturnedInInsertionOrder)
{
    Component comp("R1");
    auto& p1 = comp.addPin(
        "1", "A1"
    );
    auto& p2 = comp.addPin(
        "2", "A2"
    );
    auto pins = comp.pins();

    ASSERT_EQ(pins.size(), 2u);
    EXPECT_EQ(pins[0], &p1);
    EXPECT_EQ(pins[1], &p2);
    EXPECT_EQ(pins[0]->getNumber(), "1");
    EXPECT_EQ(pins[1]->getNumber(), "2");
}


TEST (DesignTest, EmptyDesignHasNoComponentsOrNets)
{
    Design design;

    EXPECT_EQ(design.components().size(), 0u);
    EXPECT_EQ(design.nets().size(), 0u);
}


TEST (DesignTest, ConstDesignFindsSameObjects)
{
    Design design;
    auto& r1 = design.addComponent("R1");
    auto& n1 = design.addNet("VSS");
    const Design& cdesign = design;

    EXPECT_EQ(&cdesign.findComponent("R1"), &r1);
    EXPECT_EQ(&cdesign.findNet("VSS"), &n1);
}
