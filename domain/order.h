#pragma once

#include "types.h"
#include <stdexcept>
#include <optional>

namespace Warehouse {
    class Order {
        private:
        OrderId id_;
        ShelfId shelfId_;
        uint32_t quantity_;
        OrderStatus status_;
        std::optional<RobotId> assignedRobotId_;
        
        public:
        Order(OrderId id, ShelfId shelfId, uint32_t quantity)
        : id_(id), shelfId_(shelfId), quantity_(quantity), status_(OrderStatus::New), assignedRobotId_(std::nullopt) {
            if(quantity_ == 0) {
                throw std::invalid_argument("Order quantity must be greater than zero.\n");
            }
        }

        OrderId id() const { return id_; }
        ShelfId shelfId() const { return shelfId_; }
        uint32_t quantity() const { return quantity_; }
        OrderStatus status() const { return status_; }
        std::optional<RobotId> assignedRobotId() const { return assignedRobotId_; }

        bool isAssigned() const {
            return assignedRobotId_.has_value();
        }

        void assignRobot(RobotId robotId) {
            if(status_ != OrderStatus::New) {
                throw std::runtime_error("Can only assign robot to new orders.\n");
            }

            assignedRobotId_ = robotId;
            status_ = OrderStatus::InProgress;
        }

        void unassignRobot() {
            if(status_ != OrderStatus::InProgress) {
                throw std::runtime_error("Can only unassign robot from in-progress orders.\n");
            }
            
            assignedRobotId_ = std::nullopt;
            status_ = OrderStatus::New;
        }
        
        void complete() {
            if(status_ != OrderStatus::InProgress) {
                throw std::runtime_error("Can only complete in-progress.\n");
            }

            status_ = OrderStatus::Completed;
        }

        void impossible() {
            if(status_ != OrderStatus::New && status_ != OrderStatus::InProgress) {
                throw std::runtime_error("Can only mark new or in-progress orders as impossible.\n");
            }

            status_ = OrderStatus::Impossible;
            assignedRobotId_ = std::nullopt;
        }

        void cancel() {
            if(status_ == OrderStatus::Completed) {
                throw std::runtime_error("Cannot cancel completed orders.\n");
            }

            status_ = OrderStatus::Cancelled;
            assignedRobotId_ = std::nullopt;
        }

        bool isActive() const {
            return status_ == OrderStatus::New || status_ == OrderStatus::InProgress;
        }

        bool isFinished() const {
            return status_ == OrderStatus::Completed || status_ == OrderStatus::Cancelled || status_ == OrderStatus::Impossible;
        }
    };
};