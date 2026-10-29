#include "domain/pickuppoint.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(PickupPointTest, Creation) {
    PickUpPoint pickuppoint(1, Position{5, 5});
    EXPECT_EQ(pickuppoint.id(), 1);
    EXPECT_EQ(pickuppoint.position().x, 5);
    EXPECT_EQ(pickuppoint.position().y, 5);
    EXPECT_EQ(pickuppoint.totalDelivered(), 0);
}

TEST(PickupPointTest, Receive) {
    PickUpPoint pickuppoint(1, Position{0, 0});
    pickuppoint.receive(10);
    EXPECT_EQ(pickuppoint.totalDelivered(), 10);
    pickuppoint.receive(5);
    EXPECT_EQ(pickuppoint.totalDelivered(), 15);
}

TEST(PickupPointTest, ResetCounter) {
    PickUpPoint pickuppoint(1, Position{0, 0});
    pickuppoint.receive(10);
    pickuppoint.resetCounter();
    EXPECT_EQ(pickuppoint.totalDelivered(), 0);
}

TEST(PickupPointTest, IsAt) {
    PickUpPoint pickuppoint(1, Position{2, 2});
    EXPECT_TRUE(pickuppoint.isAt(Position{2, 2}));
    EXPECT_FALSE(pickuppoint.isAt(Position{0, 0}));
}