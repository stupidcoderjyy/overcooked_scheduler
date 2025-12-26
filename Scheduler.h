//
// Created by PC on 2025/12/26.
//

#ifndef OVERCOOKED_SCHEDULER_SCHEDULER_H
#define OVERCOOKED_SCHEDULER_SCHEDULER_H

#include <vector>
#include <tuple>

struct Task {
    int cost;
    bool isAuto;
};

std::tuple<std::vector<Task>, int> Solve(const std::vector<Task>& tasks, const std::vector<std::vector<int>>& map, int n);

#endif //OVERCOOKED_SCHEDULER_SCHEDULER_H