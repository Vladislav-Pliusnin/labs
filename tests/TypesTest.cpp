#include "domain/types.h"
#include <gtest/gtest.h>

using namespace warehouse;

TEST(PositionTest, Creation) {
    Position pos{5, 10};
    EXPECT_EQ(pos.x, 5);
    EXPECT_EQ(pos.y, 10);
}

TEST(PositionTest, Equality) {
    Position pos1{3, 4};
    Position pos2{3, 4};
    Position pos3{5, 6};
    
    EXPECT_EQ(pos1, pos2);
    EXPECT_NE(pos1, pos3);
}

TEST(PositionTest, Distance) {
    Position pos1{0, 0};
    Position pos2{3, 4};
    EXPECT_EQ(pos1.Distance(pos2), 7);
}

TEST(ChargeTest, Creation) {
    Charge charge(100.0);
    EXPECT_DOUBLE_EQ(charge.value(), 100.0);
}

TEST(ChargeTest, CreationNegativeThrows) {
    EXPECT_THROW(Charge(-10.0), std::invalid_argument);
}

TEST(ChargeTest, Addition) {
    Charge charge1(50.0);
    Charge charge2(30.0);
    Charge result = charge1 + charge2;
    EXPECT_DOUBLE_EQ(result.value(), 80.0);
}

TEST(ChargeTest, Subtraction) {
    Charge charge1(100.0);
    Charge charge2(40.0);
    Charge result = charge1 - charge2;
    EXPECT_DOUBLE_EQ(result.value(), 60.0);
}

TEST(ChargeTest, SubtractionInsufficientThrows) {
    Charge charge1(30.0);
    Charge charge2(50.0);
    EXPECT_THROW(charge1 - charge2, std::invalid_argument);
}

TEST(MassTest, Creation) {
    Mass mass(25.5);
    EXPECT_DOUBLE_EQ(mass.value(), 25.5);
}

TEST(MassTest, CreationNegativeThrows) {
    EXPECT_THROW(Mass(-5.0), std::invalid_argument);
}

TEST(MoveInDirectionTest, MoveUp) {
    Position pos{5, 5};
    Position result = MoveinDirection(pos, Direction::Up);
    EXPECT_EQ(result.x, 5);
    EXPECT_EQ(result.y, 4);
}

TEST(MoveInDirectionTest, MoveDown) {
    Position pos{5, 5};
    Position result = MoveinDirection(pos, Direction::Down);
    EXPECT_EQ(result.x, 5);
    EXPECT_EQ(result.y, 6);
}

TEST(MoveInDirectionTest, MoveLeft) {
    Position pos{5, 5};
    Position result = MoveinDirection(pos, Direction::Left);
    EXPECT_EQ(result.x, 4);
    EXPECT_EQ(result.y, 5);
}

TEST(MoveInDirectionTest, MoveRight) {
    Position pos{5, 5};
    Position result = MoveinDirection(pos, Direction::Right);
    EXPECT_EQ(result.x, 6);
    EXPECT_EQ(result.y, 5);
}

TEST(OrderStatusToStringTest, NewStatus) {
    EXPECT_EQ(OrderStatustoString(OrderStatus::New), "NEW");
}

TEST(OrderStatusToStringTest, CompletedStatus) {
    EXPECT_EQ(OrderStatustoString(OrderStatus::Completed), "COMPLETED");
}