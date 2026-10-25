#pragma once

#include "types.h"
#include <cstdint>
#include <stdexcept>
#include <string>

namespace Warehouse {
    class Shelf {
        private:
        ShelfId id_;
        Position position_;
        ProductId productId_;
        uint32_t quantity_;

        public:
        Shelf(ShelfId id, const Position& position, ProductId productId, uint32_t quantity)
        : id_(id), position_(position), productId_(productId), quantity_(quantity) {}

        ShelfId id() const { return id_; }
        const Position& position() const { return position_; }
        ProductId productId() const { return productId_; }
        uint32_t quantity() const { return quantity_; }

        bool hasProduct() const {
            return quantity_ > 0;
        }

        bool hasEnough(uint32_t required) const {
            return quantity_ >= required;
        }

        void take(uint32_t amount) {
            if(amount == 0) {
                throw std::invalid_argument("Cannot take zero items.\n");
            }

            if(!hasEnough(amount)) {
                throw std::runtime_error("Insufficient product.\n");
            }

            quantity_ = quantity_ - amount;
        }

        void recovery(uint32_t amount) {
            quantity_ = quantity_ + amount;
        }

        void setQuantity(uint32_t quantity) {
            quantity_ = quantity;
        }

        bool isAt(const Position& pos) const {
            return position_ == pos;
        }
    };
};