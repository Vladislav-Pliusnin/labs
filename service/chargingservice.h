#pragma once

#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <string>

namespace Warehouse {
    class ChargingService {
        private:
        Journal& journal_;
        public:
        ChargingService(Journal& journal) : journal_(journal) {}

        void charge(WarehouseRepo& wh, RobotId robotId, uint32_t stationId, uint32_t tick) {
            double before = wh.robot(robotId).charge().value();
            wh.chargeRobotAtStation(robotId, stationId);
            double after = wh.robot(robotId).charge().value();
        
            journal_.log(tick, EventType::RobotCharged,
                         "Robot " + std::to_string(robotId) +
                         " charged from " + std::to_string(before) +
                         " to " + std::to_string(after) +
                         " at station " + std::to_string(stationId));
        }
    };
};