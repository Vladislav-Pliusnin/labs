#include "domain/shelf.h"
#include <gtest/gtest.h>

using namespace Warehouse;

TEST(ShelfTest, Creation) {
    Shelf shelf(1, Position{10, 20}, 100, 50);
    EXPECT_EQ(shelf.id(), 1);
    EXPECT_EQ(shelf.position().x, 10);
    EXPECT_EQ(shelf.position().y, 20);
    EXPECT_EQ(shelf.productId(), 100);
    EXPECT_EQ(shelf.quantity(), 50);
}

TEST(ShelfTest, HasProduct) {
    Shelf shelfWithProduct(1, Position{0, 0}, 100, 10);
    Shelf shelfWithoutProduct(2, Position{1, 1}, 100, 0);
    
    EXPECT_TRUE(shelfWithProduct.hasProduct());
    EXPECT_FALSE(shelfWithoutProduct.hasProduct());
}

TEST(ShelfTest, HasEnough) {
    Shelf shelf(1, Position{0, 0}, 100, 25);
    
    EXPECT_TRUE(shelf.hasEnough(20));
    EXPECT_TRUE(shelf.hasEnough(25));
    EXPECT_FALSE(shelf.hasEnough(30));
}

TEST(ShelfTest, Take) {
    Shelf shelf(1, Position{0, 0}, 100, 50);
    
    shelf.take(20);
    EXPECT_EQ(shelf.quantity(), 30);
    
    shelf.take(30);
    EXPECT_EQ(shelf.quantity(), 0);
    EXPECT_FALSE(shelf.hasProduct());
}

TEST(ShelfTest, TakeZeroProducts) {
    Shelf shelf(1, Position{0, 0}, 100, 50);
    EXPECT_THROW(shelf.take(0), std::invalid_argument);
}

TEST(ShelfTest, TakeInsufficientProducts) {
    Shelf shelf(1, Position{0, 0}, 100, 10);
    EXPECT_THROW(shelf.take(15), std::runtime_error);
}

TEST(ShelfTest, Recovery) {
    Shelf shelf(1, Position{0, 0}, 100, 20);
    
    shelf.recovery(30);
    EXPECT_EQ(shelf.quantity(), 50);
    
    shelf.recovery(0);
    EXPECT_EQ(shelf.quantity(), 50);
}

TEST(ShelfTest, SetQuantity) {
    Shelf shelf(1, Position{0, 0}, 100, 10);
    
    shelf.setQuantity(100);
    EXPECT_EQ(shelf.quantity(), 100);
    
    shelf.setQuantity(0);
    EXPECT_EQ(shelf.quantity(), 0);
}

TEST(ShelfTest, IsAt) {
    Shelf shelf(1, Position{5, 10}, 100, 25);
    
    EXPECT_TRUE(shelf.isAt(Position{5, 10}));
    EXPECT_FALSE(shelf.isAt(Position{5, 11}));
}

TEST(ShelfTest, MultipleShelvesDifferentIds) {
    Shelf shelf1(1, Position{0, 0}, 100, 50);
    Shelf shelf2(2, Position{0, 0}, 100, 50);
    
    EXPECT_EQ(shelf1.id(), 1);
    EXPECT_EQ(shelf2.id(), 2);
    EXPECT_NE(shelf1.id(), shelf2.id());
}

TEST(ShelfTest, DifferentProducts) {
    Shelf shelf1(1, Position{0, 0}, 100, 50);
    Shelf shelf2(2, Position{1, 1}, 200, 50);
    
    EXPECT_EQ(shelf1.productId(), 100);
    EXPECT_EQ(shelf2.productId(), 200);
    EXPECT_NE(shelf1.productId(), shelf2.productId());
}

TEST(ShelfTest, TakeAllProduct) {
    Shelf shelf(1, Position{0, 0}, 100, 15);
    
    shelf.take(15);
    EXPECT_EQ(shelf.quantity(), 0);
    EXPECT_FALSE(shelf.hasProduct());
    EXPECT_FALSE(shelf.hasEnough(1));
}

TEST(ShelfTest, RecoveryAfterEmpty) {
    Shelf shelf(1, Position{0, 0}, 100, 10);
    
    shelf.take(10);
    EXPECT_EQ(shelf.quantity(), 0);
    
    shelf.recovery(25);
    EXPECT_EQ(shelf.quantity(), 25);
    EXPECT_TRUE(shelf.hasProduct());
    EXPECT_TRUE(shelf.hasEnough(20));
}