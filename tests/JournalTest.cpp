#include "domain/journal.h"
#include <gtest/gtest.h>
#include <fstream>
using namespace Warehouse;

TEST(JournalTest, EmptyJournal) {
    Journal journal;
    EXPECT_TRUE(journal.entries().empty());
}

TEST(JournalTest, LogSingleEntry) {
    Journal journal;
    journal.log(0, EventType::TickStarted, "Tick 0 started");
    
    EXPECT_EQ(journal.entries().size(), 1);
    EXPECT_EQ(journal.entries()[0].tick, 0);
    EXPECT_EQ(journal.entries()[0].type, EventType::TickStarted);
    EXPECT_EQ(journal.entries()[0].details, "Tick 0 started");
}

TEST(JournalTest, LogMultipleEntries) {
    Journal journal;
    journal.log(0, EventType::TickStarted, "start");
    journal.log(1, EventType::RobotMoved, "moved");
    journal.log(1, EventType::RobotCharged, "charged");
    
    EXPECT_EQ(journal.entries().size(), 3);
    EXPECT_EQ(journal.entries()[1].tick, 1);
}

TEST(JournalTest, ClearJournal) {
    Journal journal;
    journal.log(0, EventType::TickStarted, "start");
    journal.log(0, EventType::TickFinished, "finish");
    EXPECT_EQ(journal.entries().size(), 2);
    
    journal.clear();
    EXPECT_TRUE(journal.entries().empty());
}

TEST(JournalTest, SaveToFile) {
    Journal journal;
    journal.log(0, EventType::TickStarted, "tick 0");
    journal.log(1, EventType::RobotMoved, "robot 1 moved");
    
    const std::string filename = "test_journal.csv";
    journal.saveToFile(filename);
    
    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open());
    
    std::string line1, line2;
    std::getline(file, line1);
    std::getline(file, line2);
    
    EXPECT_EQ(line1, "0,TICK_STARTED,tick 0");
    EXPECT_EQ(line2, "1,ROBOT_MOVED,robot 1 moved");
    
    file.close();
    std::remove(filename.c_str());
}

TEST(JournalTest, LoadFromFile) {
    const std::string filename = "test_journal_load.csv";
    {
        std::ofstream file(filename);
        file << "0,TICK_STARTED,tick 0\n";
        file << "5,ROBOT_CHARGED,robot 1 charged to 100\n";
    }
    
    Journal journal;
    journal.loadFromFile(filename);
    
    EXPECT_EQ(journal.entries().size(), 2);
    EXPECT_EQ(journal.entries()[0].tick, 0);
    EXPECT_EQ(journal.entries()[0].type, EventType::TickStarted);
    EXPECT_EQ(journal.entries()[1].tick, 5);
    EXPECT_EQ(journal.entries()[1].type, EventType::RobotCharged);
    
    std::remove(filename.c_str());
}

TEST(JournalTest, SaveAndLoadRoundTrip) {
    Journal original;
    original.log(0, EventType::OrderCreated, "order 1");
    original.log(1, EventType::OrderAssigned, "assigned to robot 1");
    original.log(2, EventType::OrderCompleted, "completed");
    
    const std::string filename = "test_roundtrip.csv";
    original.saveToFile(filename);
    
    Journal loaded;
    loaded.loadFromFile(filename);
    
    EXPECT_EQ(original.entries().size(), loaded.entries().size());
    for (size_t i = 0; i < original.entries().size(); ++i) {
        EXPECT_EQ(original.entries()[i].tick, loaded.entries()[i].tick);
        EXPECT_EQ(original.entries()[i].type, loaded.entries()[i].type);
        EXPECT_EQ(original.entries()[i].details, loaded.entries()[i].details);
    }
    
    std::remove(filename.c_str());
}

TEST(JournalTest, LoadFromNonexistentFileThrows) {
    Journal journal;
    EXPECT_THROW(journal.loadFromFile("nonexistent.csv"), std::runtime_error);
}

TEST(JournalTest, EventTypeToString) {
    EXPECT_EQ(EventTypeToString(EventType::OrderCreated), "ORDER_CREATED");
    EXPECT_EQ(EventTypeToString(EventType::RobotMoved), "ROBOT_MOVED");
    EXPECT_EQ(EventTypeToString(EventType::TickStarted), "TICK_STARTED");
}