#include "domain/robot.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(RobotTest, Creation) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    EXPECT_EQ(robot.id(), 1);
    EXPECT_EQ(robot.position().x, 0);
    EXPECT_EQ(robot.position().y, 0);
    EXPECT_DOUBLE_EQ(robot.charge().value(), 100.0);
    EXPECT_DOUBLE_EQ(robot.currentLoad().value(), 0.0);
}

TEST(RobotTest, CanMoveWhenHasCharge) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    EXPECT_TRUE(robot.canMove());
}

TEST(RobotTest, CannotMoveWhenNoCharge) {
    Robot robot(1, Position{0, 0}, Charge(0.0), Mass(50.0));
    EXPECT_FALSE(robot.canMove());
}

TEST(RobotTest, CanLoadWithinCapacity) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    EXPECT_TRUE(robot.canLoad(Mass(30.0)));
}

TEST(RobotTest, CannotLoadExceedingCapacity) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.load(Mass(40.0));
    EXPECT_FALSE(robot.canLoad(Mass(20.0)));
}

TEST(RobotTest, LoadThrowsWhenExceedingCapacity) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.load(Mass(40.0));
    EXPECT_THROW(robot.load(Mass(20.0)), std::runtime_error);
}

TEST(RobotTest, MoveConsumesCharge) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.load(Mass(10.0));
    robot.move(Direction::Right, 5.0, 0.5);
    EXPECT_EQ(robot.position().x, 1);
    EXPECT_EQ(robot.position().y, 0);
    EXPECT_DOUBLE_EQ(robot.charge().value(), 90.0);
}

TEST(RobotTest, MoveThrowsWhenNoCharge) {
    Robot robot(1, Position{0, 0}, Charge(0.0), Mass(50.0));
    EXPECT_THROW(robot.move(Direction::Up, 5.0, 0.5), std::runtime_error);
}

TEST(RobotTest, MoveThrowsWhenInsufficientCharge) {
    Robot robot(1, Position{0, 0}, Charge(3.0), Mass(50.0));
    EXPECT_THROW(robot.move(Direction::Down, 5.0, 0.0), std::runtime_error);
}

TEST(RobotTest, UnloadReturnsLoadedMass) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.load(Mass(25.0));
    Mass unloaded = robot.unload();
    EXPECT_DOUBLE_EQ(unloaded.value(), 25.0);
    EXPECT_DOUBLE_EQ(robot.currentLoad().value(), 0.0);
}

TEST(RobotTest, ChargeIncreasesCharge) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.move(Direction::Up, 40.0, 0.0);
    robot.charge(Charge(30.0));
    EXPECT_DOUBLE_EQ(robot.charge().value(), 90.0);
}

TEST(RobotTest, ChargeDoesNotExceedMax) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.move(Direction::Up, 10.0, 0.0);
    robot.charge(Charge(50.0));
    EXPECT_DOUBLE_EQ(robot.charge().value(), 100.0);
}

TEST(RobotTest, ChargeFullyRestoresMaxCharge) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.move(Direction::Up, 80.0, 0.0);
    robot.chargeFully();
    EXPECT_DOUBLE_EQ(robot.charge().value(), 100.0);
}

TEST(RobotTest, SetPositionChangesPosition) {
    Robot robot(1, Position{0, 0}, Charge(100.0), Mass(50.0));
    robot.setPosition(Position{5, 5});
    EXPECT_EQ(robot.position().x, 5);
    EXPECT_EQ(robot.position().y, 5);
}

TEST(RobotTest, IsAtChargingStation) {
    Robot robot(1, Position{5, 5}, Charge(100.0), Mass(50.0));
    EXPECT_TRUE(robot.isAtChargingStation(Position{5, 5}));
    EXPECT_FALSE(robot.isAtChargingStation(Position{0, 0}));
}

TEST(RobotTest, IsAtShelf) {
    Robot robot(1, Position{3, 3}, Charge(100.0), Mass(50.0));
    EXPECT_TRUE(robot.isAtShelf(Position{3, 3}));
    EXPECT_FALSE(robot.isAtShelf(Position{1, 1}));
}

TEST(RobotTest, IsAtPickupPoint) {
    Robot robot(1, Position{7, 7}, Charge(100.0), Mass(50.0));
    EXPECT_TRUE(robot.isAtPickupPoint(Position{7, 7}));
    EXPECT_FALSE(robot.isAtPickupPoint(Position{0, 0}));
}