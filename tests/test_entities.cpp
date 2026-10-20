#include <gtest/gtest.h>

#include "pin.h"
#include "component.h"
#include "net.h"


TEST(MiniEDATest, AppendPinInNetAndChangePinId) {

    Component comp;
    auto& comp_pin = comp.addPin();

    comp_pin.setId(1);

    Net net;
    net.append(comp_pin);
    auto& pins = net.pins();
    const auto& first_net_pin = pins[0];

    auto& p = comp.findPin(1);
    p.setId(2);

    EXPECT_EQ(comp_pin.getId(), 2);
    EXPECT_EQ(&comp_pin, &p);
    EXPECT_EQ(first_net_pin.get().getId(), 2);
    EXPECT_EQ(&first_net_pin.get(), &comp_pin);
}
