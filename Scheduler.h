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

class Solver {
public:
    Solver(const std::vector<Task>& tasks, const std::vector<std::vector<int>>& map);
    std::tuple<std::vector<Task>, int> Solve(int n);
private:
    const std::vector<Task>& tasks_;
    const std::vector<std::vector<int>>& map_;
};

std::tuple<std::vector<Task>, int> Solve(const std::vector<Task>& tasks, const std::vector<std::vector<int>>& map, int n);

#endif //OVERCOOKED_SCHEDULER_SCHEDULER_H