#include <gtest/gtest.h>

#include "design.h"
#include "net.h"


TEST (DesignTest, CreateDesign) 
{
    Design design;

    auto& comp = design.addComponent("R1");
    auto& pin1 = comp.addPin(
        "1", "pin1"
    );

    auto& net = design.addNet("GND");
    net.append(pin1);
    
    EXPECT_TRUE(&design.findComponent("R1") == &comp);
    EXPECT_TRUE(&design.findNet("GND") == &net);
}


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
    Design design;
    auto& comp = design.addComponent("test");
    comp.addPin("1", "pin1");

    EXPECT_THROW(comp.addPin("1", "again"), std::invalid_argument);
}


TEST (ComponentTest, FindMissingPinThrows) 
{
    Design design;

    auto& comp = design.addComponent("test");

    EXPECT_THROW(comp.findPin("99"), std::invalid_argument);
}


TEST (ComponentTest, FindPinReturnsSameObject) 
{
    Design design;

    auto& comp = design.addComponent("test");

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
