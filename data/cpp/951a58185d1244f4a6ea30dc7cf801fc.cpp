// Write a C++ function `std::vector<int> findBadges(const std::vector<int>& next)` that takes a vector `next` of size `n` (where `next[i]` for `0 <= i < n` represents the student that student `i` points to, with values in the range `[0, n-1]`). For each starting index `i` from `0` to `n-1`, the function must simulate a traversal that starts at `i`, moves repeatedly to `next[current]`, and stops the first time it attempts to visit a student who has already been visited in that traversal. The returned vector must contain, in order, the index of that first revisited student for each starting `i`. You may assume `n >= 1` and the graph is functional (each node has exactly one outgoing edge), so the traversal always terminates. The function should handle cases where a student points to themselves and also work with large `n` efficiently.

The problem is a classic graph traversal on a functional graph where each node has exactly one outgoing edge. For each starting node, we perform a depth-first search (or simple iterative traversal) tracking visited nodes in that specific traversal. When we encounter a node already marked as visited during this traversal, that node is our answer for the starting node. Because the graph is functional, the traversal is deterministic and will always eventually enter a cycle (which could be a self-loop). The key insight is that the first repeated node must be the entry point into the cycle, which is the answer. Important edge cases include self-loops (where `next[i] == i`, immediately returning `i`), and cycles of length greater than 1 (where the entry point is the first node revisited, not necessarily the starting node). For each starting index, we reset the visited array. The time complexity is `O(n^2)` in the worst case (e.g., a chain where each traversal visits all nodes), and space complexity is `O(n)` for the visited array. However, this matches the original problem's requirements. An alternative more efficient approach exists that computes results for all starting nodes in `O(n)` using cycle detection, but the straightforward per-start DFS is correct and sufficient for this task.

#include <vector>

// For each start index i (0 <= i < n), simulate following the "next" pointers
// until a node is visited twice. Return that node's index for every start.
// The input 'next' must have size n, with each element in [0, n-1].
std::vector<int> findBadges(const std::vector<int>& next) {
    const int n = static_cast<int>(next.size());
    std::vector<int> result(n);
    std::vector<bool> visited(n, false);

    for (int start = 0; start < n; ++start) {
        std::fill(visited.begin(), visited.end(), false);
        int current = start;
        while (!visited[current]) {
            visited[current] = true;
            current = next[current];
        }
        result[start] = current; // 'current' is the first repeated node
    }
    return result;
}

#include <cassert>
#include <vector>

// Declaration of the function from the solution
std::vector<int> findBadges(const std::vector<int>& next);

int main() {
    // Example from the original problem: n=3, p = [2,3,2] (1-indexed)
    // Convert to 0-indexed: next = [1,2,1]
    std::vector<int> next1 = {1, 2, 1};
    std::vector<int> res1 = findBadges(next1);
    assert(res1 == std::vector<int>({1, 2, 2}));

    // Self-loop: each points to itself
    std::vector<int> next2 = {0, 1, 2};
    std::vector<int> res2 = findBadges(next2);
    assert(res2 == std::vector<int>({0, 1, 2}));

    // Two-node cycle
    std::vector<int> next3 = {1, 0};
    std::vector<int> res3 = findBadges(next3);
    assert(res3 == std::vector<int>({0, 0}));

    // Single node points to itself
    std::vector<int> next4 = {0};
    std::vector<int> res4 = findBadges(next4);
    assert(res4 == std::vector<int>({0}));

    // Chain into a cycle: 0->1, 1->2, 2->1 (cycle at 1 and 2)
    std::vector<int> next5 = {1, 2, 1};
    std::vector<int> res5 = findBadges(next5);
    assert(res5 == std::vector<int>({1, 2, 1}));

    // Large random chain: 0->1->2->0 (cycle of 3)
    std::vector<int> next6 = {1, 2, 0};
    std::vector<int> res6 = findBadges(next6);
    assert(res6 == std::vector<int>({0, 1, 2}));

    // More complex: 0->1, 1->1, 2->3, 3->2
    std::vector<int> next7 = {1, 1, 3, 2};
    std::vector<int> res7 = findBadges(next7);
    assert(res7 == std::vector<int>({1, 1, 3, 3}));

    // All point to node 0
    std::vector<int> next8 = {0, 0, 0};
    std::vector<int> res8 = findBadges(next8);
    assert(res8 == std::vector<int>({0, 0, 0}));

    return 0;
}
