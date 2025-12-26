//
// Created by PC on 2025/12/26.
//

#include "Scheduler.h"

#include <queue>
#include <ranges>

using namespace std;

Solver::Solver(const std::vector<Task>& tasks, const std::vector<std::vector<int>>& map):
        tasks_(tasks), map_(map) {
}

std::tuple<std::vector<Task>, int> Solver::Solve(int n) {
    int m = tasks_.size();
    vector<vector<int>> adj(m);
    vector task_depend_size(m, 0);
    vector task_usage_size(m, 0);

    // 构建邻接表、入度和出度数组
    for (int i = 0; i < m; ++i) {
        task_depend_size[i] = map_[i].size();
        for (int p : map_[i]) {
            adj[p].push_back(i);
            task_usage_size[p]++;
        }
    }

    // 拓扑排序（Kahn算法）
    // topo_order是一个序列，依赖的任务总是排在被依赖任务的后面
    vector<int> topo_order;
    queue<int> q;
    // 加入没有依赖的任务
    for (int i = 0; i < m; ++i) {
        if (task_depend_size[i] == 0) {
            q.push(i);
        }
    }

    // 遍历队列中的任务，不断消除依赖关系，把没有依赖的任务加入队列
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo_order.push_back(u);
        for (int v : adj[u]) {
            if (--task_depend_size[v] == 0) {
                q.push(v);
            }
        }
    }

    // 计算每个任务的完成时间C和工人可用时间
    vector<int> C(m, 0);
    int worker_time = 0;
    for (int u : topo_order) {
        int dep_time = 0;
        for (int p : map_[u]) {
            dep_time = max(dep_time, C[p]);
        }

        if (!tasks_[u].isAuto) {
            int start = max(dep_time, worker_time);
            C[u] = start + tasks_[u].cost;
            worker_time = C[u];
        } else {
            C[u] = dep_time + tasks_[u].cost;
        }
    }

    // 找单个产品的完成时间（终点任务）
    int single_time = 0;
    int end_task = -1;
    for (int i = 0; i < m; ++i) {
        if (task_usage_size[i] == 0) {
            end_task = i;
            single_time = C[i];
            break;
        }
    }

    // 计算关键路径（最晚完成时间L）
    vector<int> L(m, 0);
    L[end_task] = single_time;
    for (int u : std::ranges::reverse_view(topo_order)) {
        if (u == end_task) {
            continue;
        }
        int min_l = INT_MAX;
        for (int v : adj[u]) {
            min_l = min(min_l, L[v] - tasks_[u].cost);
        }
        L[u] = min_l;
    }

    // 识别关键任务和瓶颈周期
    vector<int> critical_tasks;
    for (int u = 0; u < m; ++u) {
        if (C[u] == L[u]) {
            critical_tasks.push_back(u);
        }
    }

    int bottleneck_cycle = 0;
    for (int u : critical_tasks) {
        bottleneck_cycle = max(bottleneck_cycle, tasks_[u].cost);
    }

    // 计算总时长
    int total_time = single_time + (n - 1) * bottleneck_cycle;

    // 构建最优任务序列
    vector<Task> optimal_seq;
    for (int id : topo_order) {
        optimal_seq.push_back(tasks_[id]);
    }
    return std::make_tuple(optimal_seq, total_time);
}