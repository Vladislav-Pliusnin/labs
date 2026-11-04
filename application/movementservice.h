#pragma once

#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <string>

namespace Warehouse {
    class MovementService {
        private:
        Journal& journal_;
        
        public:
        MovementService(Journal& journal) : journal_(journal) {}
        
        void move(WarehouseRepo& wh, RobotId robotId, Direction dir, uint32_t tick, double baseCost, double massCost) {
            const auto& robot = wh.robot(robotId);
            Position oldPos = robot.position();
            wh.moveRobot(robotId, dir, baseCost, massCost);
            const auto& newRobot = wh.robot(robotId);
            Position newPos = newRobot.position();
        
            std::string details = "Robot " + std::to_string(robotId) + 
                                  " moved from (" + std::to_string(oldPos.x) + "," + 
                                  std::to_string(oldPos.y) + ") to (" + 
                                  std::to_string(newPos.x) + "," + 
                                  std::to_string(newPos.y) + ")";
        
            journal_.log(tick, EventType::RobotMoved, details);
        }
    };
};