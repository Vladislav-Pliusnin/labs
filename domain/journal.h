#pragma once

#include "types.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Warehouse {
    enum class EventType {
        OrderCreated,
        OrderAssigned,
        OrderCompleted,
        OrderImpossible,
        RobotMoved,
        RobotLoaded,
        RobotUnloaded,
        RobotCharged,
        TickStarted,
        TickFinished
    };

    constexpr std::string_view EventTypeToString(EventType type) {
        switch (type) {
            case EventType::OrderCreated:    return "ORDER_CREATED";
            case EventType::OrderAssigned:   return "ORDER_ASSIGNED";
            case EventType::OrderCompleted:  return "ORDER_COMPLETED";
            case EventType::OrderImpossible: return "ORDER_IMPOSSIBLE";
            case EventType::RobotMoved:      return "ROBOT_MOVED";
            case EventType::RobotLoaded:     return "ROBOT_LOADED";
            case EventType::RobotUnloaded:   return "ROBOT_UNLOADED";
            case EventType::RobotCharged:    return "ROBOT_CHARGED";
            case EventType::TickStarted:     return "TICK_STARTED";
            case EventType::TickFinished:    return "TICK_FINISHED";
        }
        return "UNKNOWN";
    }

    struct JournalEntry {
        uint32_t tick;
        EventType type;
        std::string details;
    };

    class Journal {
        private:
        static EventType parseEventType(const std::string& str) {
            if (str == "ORDER_CREATED")    { return EventType::OrderCreated; }
            if (str == "ORDER_ASSIGNED")   { return EventType::OrderAssigned; }
            if (str == "ORDER_COMPLETED")  { return EventType::OrderCompleted; }
            if (str == "ORDER_IMPOSSIBLE") { return EventType::OrderImpossible; }
            if (str == "ROBOT_MOVED")      { return EventType::RobotMoved; }
            if (str == "ROBOT_LOADED")     { return EventType::RobotLoaded; }
            if (str == "ROBOT_UNLOADED")   { return EventType::RobotUnloaded; }
            if (str == "ROBOT_CHARGED")    { return EventType::RobotCharged; }
            if (str == "TICK_STARTED")     { return EventType::TickStarted; }
            if (str == "TICK_FINISHED")    { return EventType::TickFinished; }
            throw std::runtime_error("Unknown event type: " + str);
        }
        std::vector<JournalEntry> entries_;
        
        public:
        void log(uint32_t tick, EventType type, const std::string& details) {
            entries_.push_back({tick, type, details});
        }

        const std::vector<JournalEntry>& entries() const {
            return entries_;
        }

        void clear() {
            entries_.clear();
        }

        void saveToFile(const std::string& filename) const {
            std::ofstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }
            for (const auto& entry : entries_) {
                file << entry.tick << ","
                     << EventTypeToString(entry.type) << ","
                     << entry.details << "\n";
            }
        }

        void loadFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file for reading: " + filename);
            }
            entries_.clear();
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string tickStr, typeStr, details;
                if (std::getline(iss, tickStr, ',') && std::getline(iss, typeStr, ',') && std::getline(iss, details)) {
                    JournalEntry entry;
                    entry.tick = std::stoi(tickStr);
                    entry.type = parseEventType(typeStr);
                    entry.details = details;
                    entries_.push_back(entry);
                }
            }
        }
    };
};