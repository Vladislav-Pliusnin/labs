#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include "Types.h"

namespace Warehouse {
    class Robot {
        private:
        RobotId id_;
        Position position_;
        Charge maxCharge_;
        Charge charge_;
        Mass maxCapacity_;
        Mass currentLoad_;

        public:
        Robot(RobotId id, const Position& position, const Charge& maxCharge, const Mass& maxCapacity)
        : id_(id), position_(position), maxCharge_(maxCharge), charge_(maxCharge), maxCapacity_(maxCapacity), currentLoad_(Mass(0.0)) {}

        RobotId id() const { return id_; }
        const Position& position() const { return position_; }
        const Charge& maxCharge() const { return maxCharge_; }
        const Charge&  charge() const { return charge_; }
        const Mass& maxCapacity() const { return maxCapacity_; }
        const Mass& currentLoad() const { return currentLoad_; }

        bool canMove() const {
            return charge_.value() > 0.0;
        }

        bool canLoad(const Mass& mass) const {
            Mass totalLoad = currentLoad_ + mass;
            return totalLoad <= maxCapacity_;
        }

        void move(Direction direction, double ChargePerStep, double ChargePerMass) {
            if (!canMove()) {
                throw std::runtime_error("Robot has no charge for move.\n");
            }

            double TotalChargeCost = ChargePerStep + (ChargePerMass * currentLoad().value());
            Charge cost(TotalChargeCost);
            
            if (!(charge_ >= cost)) {
                throw std::runtime_error("Insufficient charge for movement.\n");
            }

            position_ = MoveinDirection(position_, direction);
            charge_ = charge_ - cost;
        }

        void setPosition(const Position& position) {
            position_ = position;
        }

        void charge(const Charge& amount) {
            Charge newCharge = charge_ + amount;
            if (newCharge >= maxCharge_) {
                charge_ = maxCharge_;
            }
            else {
                charge_ = newCharge;
            }
        }

        void chargeFully() {
            charge_ = maxCharge_;
        }

        void load(const Mass& mass) {
            if(!canLoad(mass)) {
                throw std::runtime_error("Robot capacity exceeded.\n");
            }
            currentLoad_ = currentLoad_ + mass;
        }

        Mass unload() {
            Mass unloaded = currentLoad_;
            currentLoad_ = Mass(0.0);
            return unloaded;
        }

    bool isAtChargingStation(const Position& stationPosition) const {
        return position_ == stationPosition;
    }

    bool isAtShelf(const Position& shelfPosition) const {
        return position_ == shelfPosition;
    }

    bool isAtPickupPoint(const Position& pickupPosition) const {
        return position_ == pickupPosition;
    }
    };
};