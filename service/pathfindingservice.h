#pragma once

#include "domain/types.h"
#include "domain/warehouserepo.h"
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <optional>
#include <algorithm>

namespace Warehouse {
    class PathFindingService {
        private:
        static std::vector<Direction> buildPath(
            const std::map<Position, std::pair<Position, Direction>>& cameFrom,
            const Position& start, const Position& goal
        ) {
            std::vector<Direction> path;
            Position cur = goal;
            while (cur != start) {
                path.push_back(cameFrom.at(cur).second);
                cur = cameFrom.at(cur).first;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        public:
        static std::optional<std::vector<Direction>> findPath(
            const WarehouseRepo& wh,
            Position start,
            Position goal,
            RobotId excludeId = UINT32_MAX
        ) {
            if (start == goal) {
                return std::vector<Direction>{};
            }
            if (!wh.inBounds(goal)) {
                return std::nullopt;
            }
            std::map<Position, std::pair<Position, Direction>> cameFrom;
            std::queue<Position> q;
            std::set<Position> visited;

            q.push(start);
            visited.insert(start);

            static const std::pair<Direction, Position> moves[] = {
                {Direction::Up,    {0, -1}},
                {Direction::Down,  {0,  1}},
                {Direction::Left,  {-1, 0}},
                {Direction::Right, {1,  0}}
            };

            while (!q.empty()) {
                Position cur = q.front(); q.pop();

                for (const auto& [dir, off] : moves) {
                    Position next = {cur.x + off.x, cur.y + off.y};
                    if (visited.count(next)) {
                        continue;
                    }
                    if (!wh.canMoveTo(next, excludeId)) {
                        continue;
                    }

                    cameFrom[next] = {cur, dir};
                    if (next == goal) {
                        return buildPath(cameFrom, start, goal);
                    }

                    visited.insert(next);
                    q.push(next);
                }
            }
            return std::nullopt;
        }
    };
};