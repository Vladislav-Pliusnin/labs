#include "application/chargingservice.h"
#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(ChargingServiceTest, ChargeAtStation) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    ChargingService service(journal);
    
    wh.addChargingStation(ChargingStation(1, {0, 0}, Charge(20.0)));
    Robot r(1, {0, 0}, Charge(100.0), Mass(50.0));
    
    r.move(Direction::Right, 30.0, 0.0);
    r.move(Direction::Left, 0.0, 0.0);
    wh.addRobot(r);
    
    double before = wh.robot(1).charge().value();
    service.charge(wh, 1, 1, 0);
    double after = wh.robot(1).charge().value();
    
    EXPECT_GT(after, before);
    EXPECT_LE(after, 100.0);
}

TEST(ChargingServiceTest, ChargeLogsToJournal) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    ChargingService service(journal);
    
    wh.addChargingStation(ChargingStation(1, {0, 0}, Charge(20.0)));
    Robot r(1, {0, 0}, Charge(100.0), Mass(50.0));
    r.move(Direction::Right, 30.0, 0.0);
    r.move(Direction::Left, 0.0, 0.0);
    wh.addRobot(r);
    
    service.charge(wh, 1, 1, 7);
    
    EXPECT_EQ(journal.entries().size(), 1);
    EXPECT_EQ(journal.entries()[0].tick, 7);
    EXPECT_EQ(journal.entries()[0].type, EventType::RobotCharged);
}

TEST(ChargingServiceTest, ChargeFullyChargedRobot) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    ChargingService service(journal);
    
    wh.addChargingStation(ChargingStation(1, {0, 0}, Charge(20.0)));
    wh.addRobot(Robot(1, {0, 0}, Charge(100.0), Mass(50.0)));
    
    double before = wh.robot(1).charge().value();
    service.charge(wh, 1, 1, 0);
    double after = wh.robot(1).charge().value();
    
    EXPECT_DOUBLE_EQ(before, after);
}

TEST(ChargingServiceTest, ChargeNotAtStationThrows) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    ChargingService service(journal);
    
    wh.addChargingStation(ChargingStation(1, {0, 0}, Charge(20.0)));
    wh.addRobot(Robot(1, {5, 5}, Charge(50.0), Mass(50.0)));
    
    EXPECT_THROW(service.charge(wh, 1, 1, 0), std::runtime_error);
}

TEST(ChargingServiceTest, MultipleChargesAccumulate) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    ChargingService service(journal);
    
    wh.addChargingStation(ChargingStation(1, {0, 0}, Charge(20.0)));
    Robot r(1, {0, 0}, Charge(100.0), Mass(50.0));
    r.move(Direction::Right, 50.0, 0.0);
    r.move(Direction::Left, 0.0, 0.0);
    wh.addRobot(r);
    
    service.charge(wh, 1, 1, 0);
    double after1 = wh.robot(1).charge().value();
    
    service.charge(wh, 1, 1, 1);
    double after2 = wh.robot(1).charge().value();
    
    EXPECT_GT(after2, after1);
}