Write a C++ function `int countFallenDominoes(const std::vector<std::pair<int, int>>& dominoes)` that takes a list of dominoes, each represented by a pair `(position, length)`, where `position` is the x-coordinate of its base and `length` is its height. When a domino is pushed to the right, it knocks over any subsequent domino whose base is within the range `[position, position + length]` (inclusive), and those knocked dominoes can then knock over further dominoes to their right. However, a new set of dominoes can only start falling if the previous chain stopped (i.e., no overlap). Count how many separate groups of dominoes will be toppled if you push every first domino in each disconnected group. Dominoes are given unsorted and may have arbitrary positions and lengths (positive integers), and the count must be based on sorting them by position. Edge cases include duplicate positions, zero-length dominoes, and overlapping chains that merge into one group. The function should return the number of distinct falling chains.

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.

int main() {
    // Basic case: two separate chains
    assert(countFallenDominoes({{1, 2}, {4, 1}}) == 2);
    // Chain that spans multiple dominoes
    assert(countFallenDominoes({{1, 2}, {3, 1}, {4, 1}}) == 1);
    // Single domino
    assert(countFallenDominoes({{0, 5}}) == 1);
    // Empty input
    assert(countFallenDominoes({}) == 0);
    // Overlapping but not continuous? Here 0->2 covers 2, then 2->1 covers 3, but 3->2 stops before 6
    assert(countFallenDominoes({{0, 2}, {2, 1}, {3, 2}, {6, 1}}) == 2);
    // Unsorted input
    assert(countFallenDominoes({{5, 1}, {0, 3}, {2, 2}}) == 1);
    // Duplicate positions with different lengths
    assert(countFallenDominoes({{1, 1}, {1, 3}, {4, 1}}) == 1);
    // Zero-length dominoes
    assert(countFallenDominoes({{0, 0}, {1, 0}, {2, 1}, {3, 0}}) == 3);
    // All separate
    assert(countFallenDominoes({{0, 0}, {1, 0}, {2, 0}}) == 3);
    // Long chain that jumps over a small gap
    assert(countFallenDominoes({{0, 10}, {10, 1}, {11, 1}, {20, 1}}) == 2);

    return 0;
}

#include <vector>
#include <algorithm>
#include <utility>

// Count the number of distinct falling chains of dominoes.
// Each domino is a pair (position, length). Dominoes are sorted by position,
// and a chain continues as long as the next domino's position is within the
// reach of the current chain's maximum reach.
int countFallenDominoes(const std::vector<std::pair<int, int>>& dominoes) {
    if (dominoes.empty()) {
        return 0;
    }

    // Sort by position (and length as a tie-breaker for determinism)
    std::vector<std::pair<int, int>> sorted = dominoes;
    std::sort(sorted.begin(), sorted.end());

    int groups = 1;
    int currentReach = sorted[0].first + sorted[0].second;

    for (size_t i = 1; i < sorted.size(); ++i) {
        int pos = sorted[i].first;
        int len = sorted[i].second;
        if (pos <= currentReach) {
            // Same chain; extend reach if this domino goes further
            currentReach = std::max(currentReach, pos + len);
        } else {
            // New chain starts
            ++groups;
            currentReach = pos + len;
        }
    }

    return groups;
}

// The problem reduces to sorting the dominoes by position and then scanning left to right to identify how many connected "chains" exist. A chain is a maximal sequence of dominoes where each consecutive pair satisfies `next.position <= current.position + current.length`; if this condition holds, the current domino will knock over the next one, so they belong to the same falling group. If the condition fails, a new group starts with the next domino. The algorithm: copy the input pairs into a vector of a struct with `pos` and `length`, sort by `pos` (if positions are equal, any order works since length affects reach), then iterate through the sorted vector. Maintain a `currentReach` initialized to the position+length of the first domino in the current group, and a group counter incremented each time we start a new group (initially 1 for the first). For each subsequent domino, if its `pos <= currentReach`, it is part of the same group and we update `currentReach = max(currentReach, pos + length)`. Otherwise, we increment the group counter and reset `currentReach` to this domino's `pos + length`. Edge cases: N=0 should return 0; a single domino returns 1; duplicate positions are handled because we just check `pos <= currentReach` (always true if `currentReach` is at least the previous position), but a duplicate with a longer length may extend the reach; zero-length dominoes (length=0) only affect themselves unless they are at the same position as another. Time complexity is O(N log N) due to sorting, and space O(N) for the copied vector (or O(1) extra if we sort in place, but we'll copy to avoid modifying input).
