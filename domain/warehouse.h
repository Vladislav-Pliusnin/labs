#pragma once

#include "Types.h"
#include "Robot.h"
#include "Shelf.h"
#include "Order.h"
#include "PickupPoint.h"
#include "ChargingStation.h"

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <optional>

namespace Warehouse {
    class WarehouseRepo {
        private:
        void validate(const Position& p) const {
            if (!inBounds(p)) {
                throw std::out_of_range("Position out of bounds.\n");
            }
        }

        template <typename Map, typename Key>
        static auto& at(Map& m, Key k, const char* name) {
            auto it = m.find(k);
            if (it == m.end()) {
                throw std::runtime_error(std::string(name) + " not found");
            }
            return it->second;
        }

        template <typename Map, typename Key>
        static const auto& at(const Map& m, Key k, const char* name) {
            auto it = m.find(k);
            if (it == m.end()) {
                throw std::runtime_error(std::string(name) + " not found");
            }
            return it->second;
        }

        int width_, height_;
        std::vector<std::vector<CellType>> grid_;
        std::unordered_map<RobotId, Robot> robots_;
        std::unordered_map<ShelfId, Shelf> shelves_;
        std::unordered_map<uint32_t, PickUpPoint> pickups_;
        std::unordered_map<uint32_t, ChargingStation> stations_;
        std::unordered_map<OrderId, Order> orders_;
    
        public:
        WarehouseRepo(int width, int height) 
        : width_(width), height_(height), grid_(height, std::vector<CellType>(width, CellType::Empty)) {}
    
        int width() const  { return width_; }
        int height() const { return height_; }
    
        CellType cellAt(const Position& pos) const {
            validate(pos);
            return grid_[pos.y][pos.x];
        }

        void setCell(const Position& pos, CellType type) {
            validate(pos);
            grid_[pos.y][pos.x] = type;
        }

        bool inBounds(const Position& pos) const {
            return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
        }

        bool canMoveTo(const Position& pos, RobotId exclude = UINT32_MAX) const {
            if (!inBounds(pos)) {
                return false;
            }
            if (grid_[pos.y][pos.x] == CellType::Shelf) {
                return false;
            }
            return !isRobotAt(pos, exclude);
        }

        void addRobot(Robot robot) {
            validate(robot.position());
            if (robots_.count(robot.id())) {
                throw std::runtime_error("Duplicate robot id.\n");
            }
            if (isRobotAt(robot.position())) {
                throw std::runtime_error("Robot position occupied.\n");
            }
            robots_[robot.id()] = std::move(robot);
        }

        Robot& robot(RobotId id) {
            auto it = robots_.find(id);
            if (it == robots_.end()) {
                throw std::runtime_error("Robot not found.\n");
            }
            return it->second;
        }

        const Robot& robot(RobotId id) const {
            auto it = robots_.find(id);
            if (it == robots_.end()) {
                throw std::runtime_error("Robot not found.\n");
            }
            return it->second;
        }

        auto& robots() { return robots_; }
        const auto& robots() const { return robots_; }

        bool isRobotAt(const Position& pos, RobotId exclude = UINT32_MAX) const {
            for (const auto& [id, r] : robots_) {
                if (id != exclude && r.position() == pos) {
                    return true;
                }
            }
            return false;
        }

        void addShelf(Shelf shelf) {
            validate(shelf.position());
            if (grid_[shelf.position().y][shelf.position().x] != CellType::Empty) {
                throw std::runtime_error("Shelf cell not empty.\n");
            }
            grid_[shelf.position().y][shelf.position().x] = CellType::Shelf;
            shelves_[shelf.id()] = std::move(shelf);
        }

        Shelf& shelf(ShelfId id) { return at(shelves_, id, "Shelf"); }
        const Shelf& shelf(ShelfId id) const { return at(shelves_, id, "Shelf"); }

        std::optional<ShelfId> shelfAt(const Position& pos) const {
            for (const auto& [id, s] : shelves_) {
                if (s.position() == pos) {
                    return id;
                }
            }
            return std::nullopt;
        }

        auto& shelves() { return shelves_; }
        const auto& shelves() const { return shelves_; }

        void addPickupPoint(PickUpPoint p) {
            validate(p.position());
            if (grid_[p.position().y][p.position().x] != CellType::Empty) {
                throw std::runtime_error("Pickup cell not empty.\n");
            }
            grid_[p.position().y][p.position().x] = CellType::PickupPoint;
            pickups_[p.id()] = std::move(p);
        }

        PickUpPoint& pickup(uint32_t id) { return at(pickups_, id, "Pickup"); }
        const PickUpPoint& pickup(uint32_t id) const { return at(pickups_, id, "Pickup"); }
        auto& pickups() { return pickups_; }

        void addChargingStation(ChargingStation s) {
            validate(s.position());
            if (grid_[s.position().y][s.position().x] != CellType::Empty) {
                throw std::runtime_error("Charging cell not empty.\n");
            }
            grid_[s.position().y][s.position().x] = CellType::ChargingStation;
            stations_[s.id()] = std::move(s);
        }

        ChargingStation& station(uint32_t id) { return at(stations_, id, "Station"); }
        const ChargingStation& station(uint32_t id) const { return at(stations_, id, "Station"); }
        auto& stations() { return stations_; }

        void addOrder(Order order) {
            if (!shelves_.count(order.shelfId())) {
                throw std::runtime_error("Order references unknown shelf.\n");
            }
            orders_[order.id()] = std::move(order);
        }

        Order& order(OrderId id) { return at(orders_, id, "Order"); }
        const Order& order(OrderId id) const { return at(orders_, id, "Order"); }
        auto& orders() { return orders_; }

        void moveRobot(RobotId id, Direction dir, double baseCost, double massCost) {
            auto& r = robot(id);
            Position next = moveInDirection(r.position(), dir);
            if (!canMoveTo(next, id)) {
                throw std::runtime_error("Invalid move.\n");
            }
            r.move(dir, baseCost, massCost);
        }

        void loadRobot(RobotId robotId, ShelfId shelfId, uint32_t amount) {
            auto& r = robot(robotId);
            auto& s = shelf(shelfId);
            if (r.position() != s.position()) {
                throw std::runtime_error("Robot not at shelf.\n");
            }
            s.take(amount);
            r.load(Mass(static_cast<double>(amount)));
        }

        void unloadRobotAtPickup(RobotId robotId, uint32_t pickupId) {
            auto& r = robot(robotId);
            auto& p = pickup(pickupId);
            if (r.position() != p.position()) {
                throw std::runtime_error("Robot not at pickup point.\n");
            }
            auto mass = r.unload();
            p.receive(static_cast<uint32_t>(mass.value()));
        }

        void chargeRobotAtStation(RobotId robotId, uint32_t stationId) {
            auto& r = robot(robotId);
            const auto& s = station(stationId);
            if (r.position() != s.position()) {
                throw std::runtime_error("Robot not at charging station");
            }
            auto amount = s.calculateChargeAmount(r.charge(), r.maxCharge());
            r.charge(amount);
        }
    };
}