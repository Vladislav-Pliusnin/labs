#include "domain/chargingstation.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(ChargingStationTest, Creation) {
    ChargingStation station(1, Position{0, 0}, Charge(5.0));
    EXPECT_EQ(station.id(), 1);
    EXPECT_DOUBLE_EQ(station.chargePerTick().value(), 5.0);
}

TEST(ChargingStationTest, ZeroChargePerTick) {
    EXPECT_THROW(ChargingStation(1, Position{0, 0}, Charge(0.0)), std::invalid_argument);
}

TEST(ChargingStationTest, NegativeChargePerTick) {
    EXPECT_THROW(ChargingStation(1, Position{0, 0}, Charge(-1.0)), std::invalid_argument);
}

TEST(ChargingStationTest, CalculateChargeAmount) {
    ChargingStation station(1, Position{0, 0}, Charge(10.0));
    Charge amount = station.calculateCharge(Charge(90.0), Charge(100.0));
    EXPECT_DOUBLE_EQ(amount.value(), 10.0);
}

TEST(ChargingStationTest, CalculateChargeAmountLimitedByMax) {
    ChargingStation station(1, Position{0, 0}, Charge(10.0));
    Charge amount = station.calculateCharge(Charge(95.0), Charge(100.0));
    EXPECT_DOUBLE_EQ(amount.value(), 5.0);
}

TEST(ChargingStationTest, CalculateChargeAmountAlreadyFull) {
    ChargingStation station(1, Position{0, 0}, Charge(10.0));
    Charge amount = station.calculateCharge(Charge(100.0), Charge(100.0));
    EXPECT_DOUBLE_EQ(amount.value(), 0.0);
}

TEST(ChargingStationTest, IsAt) {
    ChargingStation station(1, Position{3, 3}, Charge(5.0));
    EXPECT_TRUE(station.isAt(Position{3, 3}));
    EXPECT_FALSE(station.isAt(Position{0, 0}));
}