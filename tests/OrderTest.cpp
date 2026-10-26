#include "domain/order.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(OrderTest, Creation) {
    Order order(1, 100, 5);
    EXPECT_EQ(order.id(), 1);
    EXPECT_EQ(order.shelfId(), 100);
    EXPECT_EQ(order.quantity(), 5);
    EXPECT_EQ(order.status(), OrderStatus::New);
    EXPECT_FALSE(order.isAssigned());
}

TEST(OrderTest, ZeroQuantity) {
    EXPECT_THROW(Order(1, 100, 0), std::invalid_argument);
}

TEST(OrderTest, AssignRobot) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    
    EXPECT_EQ(order.status(), OrderStatus::InProgress);
    EXPECT_TRUE(order.isAssigned());
    EXPECT_EQ(order.assignedRobotId().value(), 42);
}

TEST(OrderTest, AssignRobotToNewOrderThrows) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    
    EXPECT_THROW(order.assignRobot(43), std::runtime_error);
}

TEST(OrderTest, UnassignRobot) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.unassignRobot();
    
    EXPECT_EQ(order.status(), OrderStatus::New);
    EXPECT_FALSE(order.isAssigned());
}

TEST(OrderTest, UnassignRobotFromNonInProgressThrows) {
    Order order(1, 100, 5);
    EXPECT_THROW(order.unassignRobot(), std::runtime_error);
}

TEST(OrderTest, CompleteOrder) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.complete();
    
    EXPECT_EQ(order.status(), OrderStatus::Completed);
    EXPECT_TRUE(order.isFinished());
    EXPECT_FALSE(order.isActive());
}

TEST(OrderTest, CompleteNonInProgressOrderThrows) {
    Order order(1, 100, 5);
    EXPECT_THROW(order.complete(), std::runtime_error);
}

TEST(OrderTest, Impossible) {
    Order order(1, 100, 5);
    order.impossible();
    
    EXPECT_EQ(order.status(), OrderStatus::Impossible);
    EXPECT_TRUE(order.isFinished());
    EXPECT_FALSE(order.isActive());
    EXPECT_FALSE(order.isAssigned());
}

TEST(OrderTest, ImpossibleFromInProgress) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.impossible();
    
    EXPECT_EQ(order.status(), OrderStatus::Impossible);
    EXPECT_FALSE(order.isAssigned());
}

TEST(OrderTest, ImpossibleFromCompletedThrows) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.complete();
    
    EXPECT_THROW(order.impossible(), std::runtime_error);
}

TEST(OrderTest, CancelOrder) {
    Order order(1, 100, 5);
    order.cancel();
    
    EXPECT_EQ(order.status(), OrderStatus::Cancelled);
    EXPECT_TRUE(order.isFinished());
    EXPECT_FALSE(order.isActive());
}

TEST(OrderTest, CancelInProgressOrder) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.cancel();
    
    EXPECT_EQ(order.status(), OrderStatus::Cancelled);
    EXPECT_FALSE(order.isAssigned());
}

TEST(OrderTest, CancelCompletedOrderThrows) {
    Order order(1, 100, 5);
    order.assignRobot(42);
    order.complete();
    
    EXPECT_THROW(order.cancel(), std::runtime_error);
}

TEST(OrderTest, IsActive) {
    Order order(1, 100, 5);
    EXPECT_TRUE(order.isActive());
    
    order.assignRobot(42);
    EXPECT_TRUE(order.isActive());
    
    order.complete();
    EXPECT_FALSE(order.isActive());
}

TEST(OrderTest, IsFinished) {
    Order order(1, 100, 5);
    EXPECT_FALSE(order.isFinished());
    
    order.impossible();
    EXPECT_TRUE(order.isFinished());
}