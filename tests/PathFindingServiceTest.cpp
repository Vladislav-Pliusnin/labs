#include "domain/warehouserepo.h"
#include "application/pathfindingservice.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(PathfindingServiceTest, EmptyPath) {
    WarehouseRepo wh(10, 10);
    Position start{5, 5};
    Position goal{5, 5};

    auto path = PathFindingService::findPath(wh, start, goal);

    EXPECT_TRUE(path.has_value());
    EXPECT_TRUE(path.value().empty());
}

TEST(PathfindingServiceTest, SimpleStraightPath) {
    WarehouseRepo wh(10, 10);
    Position start{0, 0};
    Position goal{2, 0};

    auto path = PathFindingService::findPath(wh, start, goal);

    EXPECT_TRUE(path.has_value());
    EXPECT_EQ(path.value().size(), 2);
    EXPECT_EQ(path.value()[0], Direction::Right);
    EXPECT_EQ(path.value()[1], Direction::Right);
}

TEST(PathfindingServiceTest, PathAroundObstacle) {
    WarehouseRepo wh(10, 10);
    Position start{0, 0};
    Position goal{0, 2};
    
    Shelf shelf(1, {0, 1}, 100, 10);
    wh.addShelf(shelf);

    auto path = PathFindingService::findPath(wh, start, goal);

    EXPECT_TRUE(path.has_value());
    EXPECT_EQ(path.value().size(), 4);
}

TEST(PathfindingServiceTest, ExcludeRobot) {
    WarehouseRepo wh(10, 10);
    Position start{5, 5};
    Position goal{5, 6};
    
    Robot robot(1, start, Charge(100.0), Mass(50.0));
    wh.addRobot(robot);

    auto path = PathFindingService::findPath(wh, start, goal, 1);

    EXPECT_TRUE(path.has_value());
    EXPECT_EQ(path.value().size(), 1);
    EXPECT_EQ(path.value()[0], Direction::Down);
}

TEST(PathfindingServiceTest, PathAvoidsOtherRobots) {
    WarehouseRepo wh(10, 10);
    Position start{0, 0};
    Position goal{0, 2};
    
    wh.addRobot(Robot(1, start, Charge(100.0), Mass(50.0)));
    wh.addRobot(Robot(2, {0, 1}, Charge(100.0), Mass(50.0)));

    auto path = PathFindingService::findPath(wh, start, goal, 1);

    EXPECT_TRUE(path.has_value());
    EXPECT_EQ(path.value().size(), 4);
}