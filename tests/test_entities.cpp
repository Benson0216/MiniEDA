#include <gtest/gtest.h>

#include "pin.h"
#include "component.h"
#include "net.h"


TEST(MiniEDATest, ComponentTest) {

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
