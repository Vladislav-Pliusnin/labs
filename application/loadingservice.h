#pragma once

#include "domain/warehouserepo.h"
#include "domain/journal.h"
#include <string>

namespace Warehouse {
    class LoadingService {
        private:
        Journal& journal_;
        public:
        LoadingService(Journal& journal) : journal_(journal) {}

        void load(WarehouseRepo& wh, RobotId robotId, ShelfId shelfId, uint32_t amount, uint32_t tick) {
            wh.loadRobot(robotId, shelfId, amount);
        
            journal_.log(tick, EventType::RobotLoaded,
                         "Robot " + std::to_string(robotId) +
                         " loaded " + std::to_string(amount) +
                         " units from shelf " + std::to_string(shelfId));
        }

        void unload(WarehouseRepo& wh, RobotId robotId, uint32_t pickupId, uint32_t tick) {
            double loadBefore = wh.robot(robotId).currentLoad().value();
            wh.unloadRobotAtPickup(robotId, pickupId);
        
            journal_.log(tick, EventType::RobotUnloaded,
                         "Robot " + std::to_string(robotId) +
                         " unloaded " + std::to_string(static_cast<uint32_t>(loadBefore)) +
                         " units at pickup " + std::to_string(pickupId));
        }
    };
};