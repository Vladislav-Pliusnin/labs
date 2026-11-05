#pragma once

#include "domain/warehouserepo.h"
#include "domain/Journal.h"
#include <string>
#include <vector>

namespace Warehouse {
    class OrderService {
        private:
        Journal& journal_;
        OrderId nextId_ = 1;
        
        public:
        OrderService(Journal& journal) : journal_(journal) {}

        OrderId createOrder(WarehouseRepo& wh, ShelfId shelfId, uint32_t quantity, uint32_t tick) {
            OrderId id = nextId_++;
            wh.addOrder(Order(id, shelfId, quantity));

            journal_.log(tick, EventType::OrderCreated,
                         "Order " + std::to_string(id) +
                         ": shelf " + std::to_string(shelfId) +
                         ", qty " + std::to_string(quantity));
            return id;
        }

        void assignOrder(WarehouseRepo& wh, OrderId orderId, RobotId robotId, uint32_t tick) {
            wh.order(orderId).assignRobot(robotId);

            journal_.log(tick, EventType::OrderAssigned,
                         "Order " + std::to_string(orderId) +
                         " assigned to robot " + std::to_string(robotId));
        }
        
        void completeOrder(WarehouseRepo& wh, OrderId orderId, uint32_t tick) {
            wh.order(orderId).complete();

            journal_.log(tick, EventType::OrderCompleted,
                         "Order " + std::to_string(orderId) + " completed");
        }

        void markImpossible(WarehouseRepo& wh, OrderId orderId, uint32_t tick) {
            wh.order(orderId).impossible();

            journal_.log(tick, EventType::OrderImpossible,
                         "Order " + std::to_string(orderId) + " marked impossible");
        }

        void cancelOrder(WarehouseRepo& wh, OrderId orderId, uint32_t tick) {
            wh.order(orderId).cancel();

            journal_.log(tick, EventType::OrderCancelled,
                         "Order " + std::to_string(orderId) + " cancelled");
        }

        bool isFeasible(const WarehouseRepo& wh, ShelfId shelfId, uint32_t quantity) const {
            return wh.shelf(shelfId).hasEnough(quantity);
        }

        std::vector<OrderId> getActiveOrders(WarehouseRepo& wh) const {
           std::vector<OrderId> result;
            for (const auto& [id, order] : wh.orders()) {
                if (order.isActive()) {
                    result.push_back(id);
                }
            }
            return result;
        }
    };
};