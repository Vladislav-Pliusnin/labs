#pragma once

#include "types.h"
#include <stdexcept>

namespace Warehouse {
    class ChargingStation {
        private:
        uint32_t id_;
        Position position_;
        Charge chargePerTick_;

        public:
        ChargingStation(uint32_t id, const Position& position, const Charge& chargePerTick)
        : id_(id), position_(position), chargePerTick_(chargePerTick) {
            if(chargePerTick.value() <= 0.0) {
                throw std::invalid_argument("Charge per tick should be positive.\n");
            }
        }

        uint32_t id() const { return id_; }
        const Position& position() const { return position_; }
        const Charge& chargePerTick() const { return chargePerTick_; }
        
        Charge calculateCharge(const Charge& currentCharge, const Charge& maxCharge) const {
            double remaining = maxCharge.value() - currentCharge.value();
            
            if (remaining <= 0.0) {
                return Charge(0.0);
            }

            double chargeAmount;
            if (chargePerTick_.value() >= remaining) {
                chargeAmount = remaining;
            } else {
                chargeAmount = chargePerTick_.value();
            }
            return Charge(chargeAmount);
        };
        
        bool isAt(const Position& pos) const {
            return position_ == pos;
        }
    };
}