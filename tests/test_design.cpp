#include <gtest/gtest.h>

#include "design.h"
#include "net.h"


TEST (DesignTest, CreateDesign) {
    Design design;

    auto& comp = design.addComponent("test");
    auto& pin1 = comp.addPin(
        "1", "pin1"
    );
    auto& pin2 = comp.addPin(
        "2", "pin2"
    );

    auto& net = design.addNet();
    net.append(pin1);
    
    EXPECT_EQ(comp.getName(), "test");
    EXPECT_EQ(pin2.getName(), "pin2");
}

TEST (DesignTest, NetHoldsSamePinAsComponent) {
    Design design;

    auto& comp = design.addComponent("A1");
    auto& pin1 = comp.addPin(
        "2", "VSS"
    );

    auto& net = design.addNet();
    net.append(pin1);

    EXPECT_TRUE(&net.pins()[0].get() == &pin1);
}

TEST (DesignTest, DuplicatePinNumberThrows) {
    Design design;

    auto& comp = design.addComponent("test");
    auto& pin1 = comp.addPin(
        "1", "pin1"
    );

    EXPECT_THROW(comp.addPin("1", "again"), std::invalid_argument);
}

TEST (DesignTest, FindMissingPinThrows) {
    Design design;

    auto& comp = design.addComponent("test");

    EXPECT_THROW(comp.findPin("99"), std::invalid_argument);
}

TEST (DesignTest, FindPinReturnsSameObject) {
    Design design;

    auto& comp = design.addComponent("test");

    auto& pin2 = comp.addPin(
        "2", "pin2"
    );

    EXPECT_TRUE(&comp.findPin("2") == &pin2);
}
