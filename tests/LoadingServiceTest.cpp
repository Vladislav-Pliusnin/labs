#include "application/loadingservice.h"
#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(LoadingServiceTest, LoadFromShelf) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {3, 3}, Charge(100.0), Mass(50.0)));
    
    service.load(wh, 1, 1, 10, 0);
    
    EXPECT_EQ(wh.shelf(1).quantity(), 40);
    EXPECT_DOUBLE_EQ(wh.robot(1).currentLoad().value(), 10.0);
}

TEST(LoadingServiceTest, LoadLogsToJournal) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {3, 3}, Charge(100.0), Mass(50.0)));
    
    service.load(wh, 1, 1, 10, 5);
    
    EXPECT_EQ(journal.entries().size(), 1);
    EXPECT_EQ(journal.entries()[0].tick, 5);
    EXPECT_EQ(journal.entries()[0].type, EventType::RobotLoaded);
}

TEST(LoadingServiceTest, UnloadAtPickup) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addPickupPoint(PickUpPoint(1, {5, 5}));
    Robot r(1, {5, 5}, Charge(100.0), Mass(50.0));
    r.load(Mass(15.0));
    wh.addRobot(r);
    
    service.unload(wh, 1, 1, 0);
    
    EXPECT_DOUBLE_EQ(wh.robot(1).currentLoad().value(), 0.0);
    EXPECT_EQ(wh.pickup(1).totalDelivered(), 15);
}

TEST(LoadingServiceTest, UnloadLogsToJournal) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addPickupPoint(PickUpPoint(1, {5, 5}));
    Robot r(1, {5, 5}, Charge(100.0), Mass(50.0));
    r.load(Mass(15.0));
    wh.addRobot(r);
    
    service.unload(wh, 1, 1, 3);
    
    EXPECT_EQ(journal.entries()[0].tick, 3);
    EXPECT_EQ(journal.entries()[0].type, EventType::RobotUnloaded);
}

TEST(LoadingServiceTest, LoadNotAtShelfThrows) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));
    
    EXPECT_THROW(service.load(wh, 1, 1, 10, 0), std::runtime_error);
}

TEST(LoadingServiceTest, UnloadNotAtPickupThrows) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addPickupPoint(PickUpPoint(1, {5, 5}));
    wh.addRobot(Robot(1, {3, 3}, Charge(100.0), Mass(50.0)));
    
    EXPECT_THROW(service.unload(wh, 1, 1, 0), std::runtime_error);
}

TEST(LoadingServiceTest, LoadExceedsCapacityThrows) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addShelf(Shelf(1, {3, 3}, 100, 50));
    wh.addRobot(Robot(1, {3, 3}, Charge(100.0), Mass(10.0)));
    
    EXPECT_THROW(service.load(wh, 1, 1, 20, 0), std::runtime_error);
}

TEST(LoadingServiceTest, LoadMoreThanAvailableThrows) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    LoadingService service(journal);
    
    wh.addShelf(Shelf(1, {3, 3}, 100, 5));
    wh.addRobot(Robot(1, {3, 3}, Charge(100.0), Mass(50.0)));
    
    EXPECT_THROW(service.load(wh, 1, 1, 10, 0), std::runtime_error);
}