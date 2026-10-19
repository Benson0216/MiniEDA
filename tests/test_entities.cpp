#include <gtest/gtest.h>

#include "pin.h"
#include "component.h"
#include "net.h"


TEST(MiniEDATest, ChangeOfPinId) {
    
    std::vector<Pin> pins;
    Pin p;
    
    p.setId(1);
    pins.push_back(p);

    p.setId(2);
    pins.push_back(p);

    auto& begin = pins[0];
    auto& end = pins[1];    
    
    EXPECT_TRUE(begin.getId() == 1);
    EXPECT_TRUE(end.getId() == 2);
    EXPECT_TRUE(begin.getId() != end.getId());
}

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
