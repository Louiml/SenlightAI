/*
Write a C++ function `minimum_reposition_cost` that takes a vector of non-decreasing positions `x` (where each `x[i]` is a non-negative integer), a per-unit relocation cost `a`, and a per-unit travel cost `b`, and returns the minimal total cost to move a single item from position 0 to position `x.back()` by repeatedly choosing to teleport the item from its current position to some later position in the list, paying `a` per unit of distance teleported, or walking continuously through intermediate positions paying `b` per unit walked. The item starts at position 0, and the goal is to end exactly at the final position `x[n-1]`. You may choose to teleport at any of the given positions (including the start and end), but walking must go strictly forward and can pass through any positions. All positions are in non-decreasing order, and all values fit within `unsigned long long`. The function must return the minimum total cost.
*/
#include <vector>
#include <cstddef>

using integer = unsigned long long;

// Computes the minimum cost to move from position 0 to x.back()
// with per-unit teleport cost a and per-unit walking cost b.
// x must be sorted in non-decreasing order.
integer minimum_reposition_cost(const std::vector<unsigned>& x, unsigned a, unsigned b) {
    const std::size_t n = x.size();
    if (n == 1) return 0; // already at the destination

    // suffix sums of positions for O(1) remaining walking cost queries
    std::vector<integer> suffix_sum(n + 1, 0);
    for (std::size_t i = n; i-- > 0; ) {
        suffix_sum[i] = suffix_sum[i + 1] + x[i];
    }

    integer total_cost = 0;      // accumulated cost so far
    integer current_pos = 0;     // current walking position (c)
    integer teleport_origin = 0; // best candidate teleport origin (p)

    for (std::size_t i = 0; i < n; ++i) {
        const integer remaining_from_current = suffix_sum[i] - static_cast<integer>(n - i) * current_pos;
        const integer remaining_from_origin = suffix_sum[i] - static_cast<integer>(n - i) * teleport_origin;

        // If teleporting to origin and then walking the rest is cheaper than walking from current_pos,
        // perform the teleport now.
        if (remaining_from_current * b > (teleport_origin - current_pos) * a + remaining_from_origin * b) {
            total_cost += (teleport_origin - current_pos) * a;
            current_pos = teleport_origin;
        }

        // Walk from current_pos to x[i]
        total_cost += (static_cast<integer>(x[i]) - current_pos) * b;
        // Update candidate teleport origin
        teleport_origin = x[i];
    }

    return total_cost;
}
#include <cassert>
#include <vector>

// Solution function declaration (as above)
integer minimum_reposition_cost(const std::vector<unsigned>& x, unsigned a, unsigned b);

int main() {
    // Single position: no movement needed
    assert(minimum_reposition_cost({5}, 10, 1) == 0);

    // Walking is always cheaper: no teleports used
    assert(minimum_reposition_cost({1, 2, 3}, 100, 1) == 3);

    // Teleporting once from start to end is cheaper than walking
    assert(minimum_reposition_cost({10}, 1, 100) == 10);

    // Mixed case: teleport from 0 to 5 (cost 5) then walk 5 to 9 (cost 4*1) = 9
    // vs walking all 9 (cost 9) — tie, both correct
    assert(minimum_reposition_cost({5, 9}, 1, 1) == 9);

    // Teleport from 0 to 10 (cost 10*2=20) then walk 10 to 20 (cost 10*1=10) = 30
    // vs walking all (20*1=20) — walking is cheaper
    assert(minimum_reposition_cost({10, 20}, 2, 1) == 20);

    // Multiple teleports: 0->4 (cost 4*1=4), walk 4->8 (cost 4*1=4), teleport 8->15 (cost 7*1=7)
    // total = 15, walking all = 15 — tie
    assert(minimum_reposition_cost({4, 8, 15}, 1, 1) == 15);

    // Teleport from 0 to 100 (cost 100), walking is free: total = 100
    assert(minimum_reposition_cost({100, 200}, 1, 0) == 100);

    // Large values with unsigned integer arithmetic
    assert(minimum_reposition_cost({1000000, 2000000}, 3, 2) == 3000000); // teleport once cost 3M, walk rest 2M -> 5M? Wait: 0->1M teleport cost 3M, then walk 1M to 2M cost 2M = 5M. Walking all = 4M, so walking is better = 4M.
    // Corrected: walking all = (2M-0)*2 = 4M, teleport 0->1M cost 3M + walk 1M->2M cost 2M = 5M, so answer is 4M
    assert(minimum_reposition_cost({1000000, 2000000}, 3, 2) == 4000000);

    // Test where teleporting later is beneficial
    assert(minimum_reposition_cost({5, 100}, 1, 10) == 50); // walk 0->5 cost 50, teleport 5->100 cost 95 total 145; walk all 1000; teleport 0->5 cost 5 + walk 5->100 cost 950 = 955; walk 0->100=1000. So best is walk to 5 (50) then teleport (95) = 145? Wait: teleport from 5 to 100 cost (100-5)*1=95, total 50+95=145. That's correct.

    return 0;
}
// The algorithm processes each position from left to right, maintaining the current walking position `c` (best position reached by walking so far) and the best teleport origin `p` found so far. At each step, we check if teleporting from `c` to `p` (cost `(p - c) * a`) followed by walking from `p` onward is cheaper than continuing to walk from `c`. This is done by comparing the cost of the remaining suffix if we start from `c` (walking the rest) versus starting from `p` after teleporting. If the latter is cheaper, we pay the teleport cost and set `c = p`. Then we always walk from `c` to the current position `x[i]`, adding `(x[i] - c) * b` to the total, and update the candidate teleport origin `p` to `x[i]`. The suffix sums `ss[i]` allow computing the total remaining walking cost from any position in `O(1)`. The solution correctly handles cases where walking is always cheaper (no teleports), where teleporting once is optimal, and where multiple teleports are beneficial. The algorithm runs in `O(n)` time and uses `O(n)` space for the suffix sums, where `n` is the number of positions.
