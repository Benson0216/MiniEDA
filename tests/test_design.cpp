#include <gtest/gtest.h>

#include "design.h"
#include "net.h"


TEST(MiniEDATest, CreateDesign) {
    
    Design design;

    auto& comp = design.addComponent();
    auto& pin1 = comp.addPin();
    auto& pin2 = comp.addPin();

    auto& net = design.addNet();
    net.append(pin1);
    
    EXPECT_TRUE(&net.pins()[0].get() == &pin1);
}
