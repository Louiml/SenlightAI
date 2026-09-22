// Given a list of `n` tasks, each with a type `t` and a position `p` on a 1D line (all positions are non‑negative integers), write a C++ function `long long minMovementCost(const std::vector<std::pair<long long, long long>>& tasks, long long typeCount, long long maxPosition)` that computes the minimum total movement cost to process all tasks in the given order. The cost to move from position `a` to position `b` is `abs(a - b)`. The agent starts at any position (cost 0 to choose the starting point). However, the movement rule depends on the task type: if two consecutive tasks have the same type, the agent must move directly from the previous position to the current task’s position, and the cost is simply the absolute difference. If the type changes between consecutive tasks, the agent is allowed an intermediate repositioning step: before moving to the current task’s position, the agent may first move to any position (paying the absolute difference from the previous position to that intermediate position) and then move to the target position (paying the absolute difference from the intermediate to the target). The goal is to minimize the total cost over all tasks, and you may choose the starting position freely. The tasks are given as a vector of pairs `{type, position}`, and the type values are positive integers from 1 to `typeCount` (inclusive). Positions are between 0 and `maxPosition` inclusive. Return the minimal total cost.

#include <cassert>
#include <vector>
#include <utility>

int main() {
    using ll = long long;
    // Single task, type change (first task): start anywhere, move to position 5 -> cost 0 (start at 5).
    assert(minMovementCost({{1,5}}, 1, 10) == 0);
    // Two tasks same type: must move from first ending to second directly.
    // First task: cost 0 (start at position 3). Second: position 7 -> cost |7-3|=4.
    assert(minMovementCost({{1,3},{1,7}}, 1, 10) == 4);
    // Two tasks different type: first task cost 0 (start at 3), second type change: can use intermediate.
    // Best: start at 3, first ends at 3. Then move to target 7: choose mid between 3 and 7, total |3-3|+|3-7|=4 (or direct without intermediate? Actually type change forces an intermediate but can be same position, so cost is still |3-7|? Wait the intermediate can be same as start or target. So minimal is 4). 
    assert(minMovementCost({{1,3},{2,7}}, 2, 10) == 4);
    // Three tasks: type1 at 0, type2 at 10, type1 at 0.
    // Task1: start at 0 cost0. Task2 (type change): from 0 to 10, via mid maybe 10 -> cost |0-10|+|10-10|=10. End at 10.
    // Task3 (type change from type2 to type1): from 10 to 0, via mid 0 -> cost |10-0|+|0-0|=10. Total 20.
    assert(minMovementCost({{1,0},{2,10},{1,0}}, 2, 10) == 20);
    // Same type but multiple: type1 at 1,2,3: cost |1-2|+|2-3|=1+1=2.
    assert(minMovementCost({{1,1},{1,2},{1,3}}, 1, 10) == 2);
    // Different types but optimal intermediate can reduce cost? For type change, the minimum is still just the absolute difference because you can choose mid = either endpoint.
    // So for (type1 at 0, type2 at 100) cost = 100.
    assert(minMovementCost({{1,0},{2,100}}, 2, 100) == 100);
    // Case: start position free, but multiple tasks with same type and far apart: cost is sum of distances.
    assert(minMovementCost({{1,0},{1,5},{1,2}}, 1, 5) == 5+3); // 0->5=5, 5->2=3 => 8
    // All at same position: cost 0 regardless of type changes.
    assert(minMovementCost({{1,3},{2,3},{1,3}}, 2, 10) == 0);
    // Large maxPosition but tasks near 0.
    assert(minMovementCost({{1,0},{2,1}}, 2, 1000000) == 1);
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>

// Compute minimum total movement cost to process tasks sequentially.
// tasks: vector of {type, position}; type is 1-based, position in [0, maxPosition].
// Returns minimal total cost. Starting position is free.
long long minMovementCost(const std::vector<std::pair<long long, long long>>& tasks,
                          long long typeCount, long long maxPosition) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    int n = static_cast<int>(tasks.size());
    int M = static_cast<int>(maxPosition);

    // f[pos] = min cost after processing some tasks, ending at pos.
    std::vector<long long> f(M + 1, 0);
    std::vector<long long> g(M + 1);
    std::vector<long long> new_f(M + 1);

    long long prevType = -1; // sentinel meaning no previous task

    for (int i = 0; i < n; ++i) {
        long long curType = tasks[i].first;
        long long target = tasks[i].second;

        if (curType == prevType) {
            // Same type: must move directly to target.
            // Compute new_f[target] = min_prev (f[prev] + abs(prev - target))
            // For all positions, we need the transformation g[pos] = min_prev(f[prev] + abs(prev - pos)).
            // Then answer for target is g[target].
            // But since we only need g at target, we can compute the full g via two sweeps.
            // Left-to-right: g[pos] = min(f[pos], g[pos-1] + 1) but careful: starting from f[pos].
            // Actually g[pos] = min over prev of (f[prev] + abs(prev - pos)).
            // Standard: compute left min: for pos from 0..M: if pos==0: g[0]=f[0]; else g[pos]=min(f[pos], g[pos-1]+1).
            // Then right pass: for pos from M-1 down to 0: g[pos]=min(g[pos], g[pos+1]+1).
            // After that g[pos] is min_prev(f[prev]+abs(prev-pos)).
            // Then new_f target = g[target], but we keep all new_f for simplicity.
            std::copy(f.begin(), f.end(), g.begin());
            // left pass
            for (int pos = 1; pos <= M; ++pos) {
                g[pos] = std::min(g[pos], g[pos-1] + 1);
            }
            // right pass
            for (int pos = M - 1; pos >= 0; --pos) {
                g[pos] = std::min(g[pos], g[pos+1] + 1);
            }
            // For this task, after moving, we must be at target.
            std::fill(new_f.begin(), new_f.end(), INF);
            new_f[target] = g[target];
        } else {
            // Different type (or first task): allow one intermediate repositioning.
            // Step 1: compute g[mid] = min_prev(f[prev] + abs(prev - mid)).
            std::copy(f.begin(), f.end(), g.begin());
            for (int pos = 1; pos <= M; ++pos) {
                g[pos] = std::min(g[pos], g[pos-1] + 1);
            }
            for (int pos = M - 1; pos >= 0; --pos) {
                g[pos] = std::min(g[pos], g[pos+1] + 1);
            }
            // Step 2: compute new_f[p] = min_mid(g[mid] + abs(mid - p)).
            // Use g as the new "f" for the second transformation.
            std::copy(g.begin(), g.end(), new_f.begin());
            for (int pos = 1; pos <= M; ++pos) {
                new_f[pos] = std::min(new_f[pos], new_f[pos-1] + 1);
            }
            for (int pos = M - 1; pos >= 0; --pos) {
                new_f[pos] = std::min(new_f[pos], new_f[pos+1] + 1);
            }
            // After both movements, we end exactly at target.
            // Wait: the second transformation gives min over mid of (g[mid] + abs(mid - p)) for any p.
            // But we must end at target, so the cost is new_f[target].
            // But we need to set f to new_f? Actually after this task, the ending position must be target.
            // So we should set f to a vector where only f[target] = new_f[target] and others INF.
            // However, the DP definition says f[pos] after processing task means ending at pos.
            // Since we must end at target, all other positions are invalid.
            // So we set f to be all INF except f[target] = new_f[target].
            std::fill(f.begin(), f.end(), INF);
            f[target] = new_f[target];
        }
        prevType = curType;
    }

    // Answer is min over positions of f[pos].
    long long ans = INF;
    for (long long val : f) {
        ans = std::min(ans, val);
    }
    return ans;
}

// The problem is a dynamic programming (DP) on positions. Let `f[pos]` be the minimum total cost after processing the current task, with the agent ending at position `pos` (immediately after the final movement of that task). Initially, before any task, we can start anywhere at cost 0, so before the first task `f[pos] = 0` for all `pos`. For a task `{t, p}`:
// - If the previous task had the same type `t`, then the agent must move directly from the previous ending position to `p`. So for each candidate previous ending position `prev`, the cost to end at `p` is `f[prev] + abs(prev - p)`. Since we want the minimal over `prev`, we compute `new_f[p] = min_{prev} (f[prev] + abs(prev - p))`. That is a classic 1D DP with absolute‑value costs, which can be computed in `O(maxPosition)` using two sweeps (left‑to‑right and right‑to‑left) to propagate minima, yielding `new_f[p] = min_{prev} (f[prev] + abs(prev - p))`. After computing this, we set `f = new_f`.
// - If the previous task had a different type (or this is the first task), the agent may use an intermediate repositioning step. That means before moving to `p`, the agent can move from the previous position `prev` to any intermediate `mid` (cost `abs(prev - mid)`), then to `p` (cost `abs(mid - p)`). The total cost for given `prev` is `f[prev] + abs(prev - mid) + abs(mid - p)`. To minimize over `prev` and `mid`, we can split: `min_mid ( min_prev(f[prev] + abs(prev - mid)) + abs(mid - p) )`. The inner term is exactly the same as the same‑type transition: let `g[mid] = min_prev(f[prev] + abs(prev - mid))`, computable in O(maxPosition) with sweeps. Then the final cost for ending at `p` is `min_mid (g[mid] + abs(mid - p))`, which again is the same form and can also be computed in O(maxPosition) with sweeps. So for a type change, we apply the sweep transformation twice. After processing all tasks, the answer is the minimum over `pos` of `f[pos]`. Edge cases: multiple consecutive tasks with the same type require only a single transformation per task (direct movement). Tasks with different types require double transformation. The first task always uses the double transformation (because there is no previous type), and the initial `f` is all zeros. Complexity: O(n * maxPosition) time (each task does O(maxPosition) sweeps) and O(maxPosition) auxiliary space. Note that `typeCount` is given but not actually needed except to validate input; the algorithm only cares about type equality between consecutive tasks.
