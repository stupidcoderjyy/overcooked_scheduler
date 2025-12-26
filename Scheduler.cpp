//
// Created by PC on 2025/12/26.
//

#include "Scheduler.h"

#include <queue>
#include <ranges>

using namespace std;

tuple<vector<Task>, int> Solve(const vector<Task> &tasks, const vector<vector<int>> &map, int n) {
    int m = tasks.size();
    vector<vector<int>> adj(m);
    vector in_degree(m, 0);
    vector out_degree(m, 0);

    // 构建邻接表、入度和出度数组
    for (int i = 0; i < m; ++i) {
        in_degree[i] = map[i].size();
        for (int p : map[i]) {
            adj[p].push_back(i);
            out_degree[p]++;
        }
    }

    // 拓扑排序（Kahn算法）
    vector<int> topo_order;
    queue<int> q;
    for (int i = 0; i < m; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo_order.push_back(u);
        for (int v : adj[u]) {
            if (--in_degree[v] == 0) q.push(v);
        }
    }

    // 计算每个任务的完成时间C和工人可用时间
    vector<long long> C(m, 0);
    long long worker_time = 0;
    for (int u : topo_order) {
        long long dep_time = 0;
        for (int p : map[u]) dep_time = max(dep_time, C[p]);

        if (!tasks[u].isAuto) {
            long long start = max(dep_time, worker_time);
            C[u] = start + tasks[u].cost;
            worker_time = C[u];
        } else {
            C[u] = dep_time + tasks[u].cost;
        }
    }

    // 找单个产品的完成时间（终点任务）
    long long single_time = 0;
    int end_task = -1;
    for (int i = 0; i < m; ++i) {
        if (out_degree[i] == 0) {
            end_task = i;
            single_time = C[i];
            break;
        }
    }

    // 计算关键路径（最晚完成时间L）
    vector<long long> L(m, 0);
    L[end_task] = single_time;
    for (int u : std::ranges::reverse_view(topo_order)) {
        if (u == end_task) {
            continue;
        }
        long long min_l = LLONG_MAX;
        for (int v : adj[u]) min_l = min(min_l, L[v] - tasks[u].cost);
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
        bottleneck_cycle = max(bottleneck_cycle, tasks[u].cost);
    }

    // 计算总时长
    long long total_time = single_time + (n - 1) * 1LL * bottleneck_cycle;

    // 构建最优任务序列
    vector<Task> optimal_seq;
    for (int id : topo_order) {
        optimal_seq.push_back(tasks[id]);
    }
    return make_tuple(optimal_seq, static_cast<int>(total_time));
}
