Write a C++ function `bool canReachFestival(int n, vector<pair<int,int>> stores, pair<int,int> start, pair<int,int> target)` that determines whether a person can travel from a starting coordinate to a target coordinate under the following rules: The person starts with full energy and can travel at most 1000 units of Manhattan distance before needing to stop. They may stop at any of the given convenience store coordinates (each store can be used at most once) to "refill" and regain full travel capacity. The person can also stop at the target directly if it is within 1000 distance from the current location. The function should return `true` if the target is reachable through a sequence of moves where each move's Manhattan distance ≤ 1000, and `false` otherwise. The coordinates are integers, and the number of stores `n` is between 0 and 100. The start and target are always distinct. The function must handle duplicate store coordinates (treat each occurrence independently) and must not modify the input vectors.

#include <cassert>
#include <vector>
#include <utility>
#include <iostream>

int main() {
    // Direct reach
    assert(canReachFestival(0, {}, {0,0}, {500,500}) == true);
    assert(canReachFestival(0, {}, {0,0}, {600,600}) == false); // 1200 > 1000

    // One store needed
    std::vector<std::pair<int,int>> stores1 = {{500,500}};
    assert(canReachFestival(1, stores1, {0,0}, {1000,1000}) == true); // via store
    assert(canReachFestival(1, {{900,900}}, {0,0}, {1000,1000}) == false); // store too far

    // Chain of stores
    std::vector<std::pair<int,int>> stores2 = {{300,0}, {600,0}, {900,0}};
    assert(canReachFestival(3, stores2, {0,0}, {1200,0}) == true); // 300,600,900,1200 each step ≤1000
    assert(canReachFestival(3, stores2, {0,0}, {1500,0}) == false); // gaps too large

    // Duplicate stores (each used once)
    std::vector<std::pair<int,int>> stores3 = {{500,0}, {500,0}};
    assert(canReachFestival(2, stores3, {0,0}, {1000,0}) == true); // use both to jump

    // No stores, unreachable
    assert(canReachFestival(0, {}, {0,0}, {2000,0}) == false);

    // Negative coordinates
    std::vector<std::pair<int,int>> stores4 = {{-500,0}, {0,500}};
    assert(canReachFestival(2, stores4, {-1000,0}, {1000,0}) == true); // -1000 -> -500 -> 0 -> 1000? Check: -1000 to -500=500, -500 to 0=500, 0 to 1000=1000, works

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <queue>
#include <utility>
#include <cmath>

// Returns true if the target is reachable from start within steps of max 1000 Manhattan distance,
// using each convenience store at most once as a refill point.
bool canReachFestival(int n, const std::vector<std::pair<int,int>>& stores,
                      std::pair<int,int> start, std::pair<int,int> target) {
    // Quick direct check
    auto manhattan = [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    };

    if (manhattan(start, target) <= 1000) return true;

    // visited[i] == true means store i has been used
    std::vector<bool> visited(n, false);

    // BFS queue of coordinates
    std::queue<std::pair<int,int>> q;
    q.push(start);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Try to go directly to target
        if (manhattan(current, target) <= 1000) return true;

        // Try each store
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            if (manhattan(current, stores[i]) <= 1000) {
                visited[i] = true;
                q.push(stores[i]);
            }
        }
    }

    return false;
}

// The problem is a reachability search on an implicit graph. Nodes are the start, the target, and all convenience stores. An edge exists between two nodes if their Manhattan distance ≤ 1000. We need to check if the target is reachable from the start. Use Breadth-First Search (BFS) starting from the start coordinate. Maintain a visited array for the stores (since start is always visited). At each current node, first check if the target is within 1000 distance; if so, return true. Otherwise, iterate over all stores: if a store is not yet visited and its Manhattan distance from the current node is ≤ 1000, mark it visited and enqueue it. This works because stores act as intermediate refill points and can be visited only once. Edge cases: n=0 (only check direct distance to target), target unreachable, multiple coordinates, and self-loops (start equals a store) — but since start is distinct from target and we only store coordinates, self-loops are naturally avoided by not visiting start as a store (start is not in the store list). Time complexity: O(n^2) in the worst case because each of up to n stores can be popped and we scan all n stores per pop. Space complexity: O(n) for the queue and visited array.
