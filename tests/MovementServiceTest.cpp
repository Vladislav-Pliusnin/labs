#include "application/movementservice.h"
#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <gtest/gtest.h>
using namespace Warehouse;

TEST(MovementServiceTest, MoveAllDirections) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    MovementService service(journal);
    wh.addRobot(Robot(1, {5, 5}, Charge(100.0), Mass(50.0)));

    service.move(wh, 1, Direction::Right, 0, 1.0, 0.0);
    EXPECT_EQ(wh.robot(1).position(), (Position{6, 5}));

    service.move(wh, 1, Direction::Down, 1, 1.0, 0.0);
    EXPECT_EQ(wh.robot(1).position(), (Position{6, 6}));

    service.move(wh, 1, Direction::Left, 2, 1.0, 0.0);
    EXPECT_EQ(wh.robot(1).position(), (Position{5, 6}));

    service.move(wh, 1, Direction::Up, 3, 1.0, 0.0);
    EXPECT_EQ(wh.robot(1).position(), (Position{5, 5}));
}

TEST(MovementServiceTest, MoveLogsToJournal) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    MovementService service(journal);
    wh.addRobot(Robot(1, {2, 2}, Charge(100.0), Mass(50.0)));

    service.move(wh, 1, Direction::Right, 5, 1.0, 0.0);

    EXPECT_EQ(journal.entries().size(), 1);
    EXPECT_EQ(journal.entries()[0].tick, 5);
    EXPECT_EQ(journal.entries()[0].type, EventType::RobotMoved);
    EXPECT_NE(journal.entries()[0].details.find("2,2"), std::string::npos);
    EXPECT_NE(journal.entries()[0].details.find("3,2"), std::string::npos);
}

TEST(MovementServiceTest, MoveWithMassCost) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    MovementService service(journal);
    Robot r(1, {5, 5}, Charge(100.0), Mass(50.0));
    r.load(Mass(10.0));
    wh.addRobot(r);

    double before = wh.robot(1).charge().value();
    service.move(wh, 1, Direction::Right, 0, 1.0, 0.5);
    double after = wh.robot(1).charge().value();

    EXPECT_DOUBLE_EQ(before - after, 6.0);
}

TEST(MovementServiceTest, InvalidMovesThrow) {
    WarehouseRepo wh(10, 10);
    Journal journal;
    MovementService service(journal);

    wh.addRobot(Robot(1, {0, 0}, Charge(100.0), Mass(50.0)));
    EXPECT_THROW(service.move(wh, 1, Direction::Left, 0, 1.0, 0.0), std::runtime_error);

    wh.addShelf(Shelf(1, {3, 2}, 100, 10));
    wh.addRobot(Robot(2, {2, 2}, Charge(100.0), Mass(50.0)));
    EXPECT_THROW(service.move(wh, 2, Direction::Right, 0, 1.0, 0.0), std::runtime_error);

    wh.addRobot(Robot(3, {5, 5}, Charge(100.0), Mass(50.0)));
    wh.addRobot(Robot(4, {6, 5}, Charge(100.0), Mass(50.0)));
    EXPECT_THROW(service.move(wh, 3, Direction::Right, 0, 1.0, 0.0), std::runtime_error);

    EXPECT_THROW(service.move(wh, 999, Direction::Right, 0, 1.0, 0.0), std::runtime_error);
}