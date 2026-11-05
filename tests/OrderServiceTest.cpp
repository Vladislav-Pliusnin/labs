#include "application/orderservice.h"
#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(OrderServiceTest, CreateAndAssignOrder) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    OrderService service(journal);
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));

    OrderId id = service.createOrder(wh, 1, 10, 0);
    service.assignOrder(wh, id, 1, 1);

    EXPECT_EQ(id, 1);
    EXPECT_EQ(wh.order(id).status(), OrderStatus::InProgress);
    EXPECT_EQ(wh.order(id).assignedRobotId(), 1);
    EXPECT_EQ(journal.entries().size(), 2);
}

TEST(OrderServiceTest, CompleteOrderLifecycle) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    OrderService service(journal);
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));

    OrderId id = service.createOrder(wh, 1, 10, 0);
    service.assignOrder(wh, id, 1, 1);
    service.completeOrder(wh, id, 2);

    EXPECT_EQ(wh.order(id).status(), OrderStatus::Completed);
}

TEST(OrderServiceTest, MarkImpossibleAndCancel) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    OrderService service(journal);
    wh.addShelf(Shelf(1, {3, 3}, 100, 5));

    OrderId id1 = service.createOrder(wh, 1, 10, 0);
    service.markImpossible(wh, id1, 1);
    EXPECT_EQ(wh.order(id1).status(), OrderStatus::Impossible);

    OrderId id2 = service.createOrder(wh, 1, 5, 2);
    service.cancelOrder(wh, id2, 3);
    EXPECT_EQ(wh.order(id2).status(), OrderStatus::Cancelled);
}

TEST(OrderServiceTest, IsFeasibleAndActiveOrders) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    OrderService service(journal);
    wh.addShelf(Shelf(1, {3, 3}, 100, 10));
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));

    EXPECT_TRUE(service.isFeasible(wh, 1, 5));
    EXPECT_FALSE(service.isFeasible(wh, 1, 15));

    OrderId id1 = service.createOrder(wh, 1, 5, 0);
    OrderId id2 = service.createOrder(wh, 1, 5, 0);
    service.assignOrder(wh, id1, 1, 1);
    service.completeOrder(wh, id1, 2);

    auto active = service.getActiveOrders(wh);
    EXPECT_EQ(active.size(), 1);
    EXPECT_EQ(active[0], id2);
}

TEST(OrderServiceTest, InvalidOperationsThrow) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    OrderService service(journal);
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));

    OrderId id = service.createOrder(wh, 1, 10, 0);
    EXPECT_THROW(service.completeOrder(wh, id, 1), std::runtime_error);

    service.assignOrder(wh, id, 1, 1);
    service.completeOrder(wh, id, 2);
    EXPECT_THROW(service.cancelOrder(wh, id, 3), std::runtime_error);

    EXPECT_THROW(service.assignOrder(wh, 999, 1, 0), std::runtime_error);
}