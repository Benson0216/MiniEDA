#include <gtest/gtest.h>

#include "pin.h"
#include "component.h"
#include "net.h"
#include "design.h"


TEST(MiniEDATest, ComponentTest) 
{
    Component comp("comp");
    auto& comp_pin = comp.addPin(
        "1", "pin1"
    );

    Net net("VDD");
    net.append(comp_pin);
    auto& pins = net.pins();
    const auto& first_net_pin = pins[0];

    auto& p = comp.findPin("1");

    EXPECT_EQ(&comp_pin, &p);
    EXPECT_EQ(&first_net_pin.get(), &comp_pin);
}


TEST (EntityTest, NetKeepsAppendedPinsInOrder)
{
    Component comp("R1");
    auto& p1 = comp.addPin("1", "A1");
    auto& p2 = comp.addPin("2", "A2");

    Net net("VDD");
    net.append(p1);
    net.append(p2);

    const auto& pins = net.pins();

    EXPECT_EQ(net.getName(), "VDD");
    ASSERT_EQ(pins.size(), 2u);
    EXPECT_EQ(&pins[0].get(), &p1);
    EXPECT_EQ(&pins[1].get(), &p2);
}
