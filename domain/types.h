#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Warehouse {
    struct Position {
        int x, y;
        
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Position& other) const {
            return !(*this == other);
        }

        int Distance(const Position& other) const {
            return std::abs(x - other.x) + std::abs(y - other.y);
        }
    };

    class Charge {
        private:
        double amount_;
        
        public:
        explicit Charge(double amount) : amount_(amount) {
            if (amount < 0.0) {
                throw std::invalid_argument("Charge cannot be negative.\n");
            }
        }
        
        double value() const {
            return amount_;
        }

        Charge operator+(const Charge& other) const {
            return Charge(amount_ + other.amount_);
        }

        Charge operator-(const Charge& other) const {
            if(amount_ < other.amount_) {
                throw std::runtime_error("Insufficient charge.");
            }
            return Charge(amount_ - other.amount_);
        }

        bool operator>=(const Charge& other) const {
            return amount_ >= other.amount_;
        }
        
        bool operator<=(const Charge& other) const {
            return amount_ <= other.amount_;
        }
    };

    class Mass {
        private:
        double mass_;

        public:
        explicit Mass(double mass) : mass_(mass) {
            if (mass < 0.0) {
                throw std::invalid_argument("Mass cannot be negative.\n");
            }
        }
        
        double value() const {
            return mass_;
        }

        Mass operator+(const Mass& other) const {
            return Mass(mass_ + other.mass_);
        }

        Mass operator-(const Mass& other) const {
            if(mass_ < other.mass_) {
                throw std::runtime_error("Insufficient mass capacity.");
            }
            return Mass(mass_ - other.mass_);
        }

        bool operator>=(const Mass& other) const {
            return mass_ >= other.mass_;
        }
        
        bool operator<=(const Mass& other) const {
            return mass_ <= other.mass_;
        }
    };

    using RobotId = uint32_t;
    using OrderId = uint32_t;
    using ProductId = uint32_t;
    using ShelfId = uint32_t;

    enum class SellType {
        Empty,
        ShelfwithGoods,
        PickupPoint,
        ChargingStation
    };

    enum class OrderStatus {
        New,
        InProgress,
        Completed,
        Impossible,
        Cancelled
    };

    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    constexpr Position MoveinDirection(const Position& pos, Direction dir) {
        switch(dir) {
            case Direction::Up: return {pos.x, pos.y - 1};
            case Direction::Down: return {pos.x, pos.y + 1};
            case Direction::Left: return {pos.x - 1, pos.y};
            case Direction::Right: return {pos.x + 1, pos.y};
        }
        return pos;
    }

    constexpr std::string OrderStatustoString(OrderStatus status) {
        switch (status) {
            case OrderStatus::New:        return "NEW";
            case OrderStatus::InProgress: return "IN_PROGRESS";
            case OrderStatus::Completed:  return "COMPLETED";
            case OrderStatus::Impossible: return "IMPOSSIBLE";
            case OrderStatus::Cancelled:  return "CANCELLED";
        }
        return "UNKNOWN";
    }
};