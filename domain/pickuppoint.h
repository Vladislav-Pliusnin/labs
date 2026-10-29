#pragma once

#include "types.h"
#include <cstdint>

namespace Warehouse {
    class PickUpPoint {
        private:
        uint32_t id_;
        Position position_;
        uint32_t totalDelivered_;

        public:
        PickUpPoint(uint32_t id, const Position& position) : id_(id), position_(position), totalDelivered_(0) {}

        uint32_t id() const { return id_; }
        const Position& position() const { return position_; }
        uint32_t totalDelivered() const { return totalDelivered_; }

        void receive(uint32_t amount) {
            totalDelivered_ = totalDelivered_ + amount;
        }

        void resetCounter() {
            totalDelivered_ = 0;
        }

        bool isAt (const Position& pos) {
            return position_ == pos;
        }
    };
};