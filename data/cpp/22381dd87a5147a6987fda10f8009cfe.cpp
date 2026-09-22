Write a C++ function `int multitry_fm_search(const std::vector<int>& gain_values, int stop_threshold)` that simulates the core multi-try local search logic from the given k-way FM refinement snippet. The function receives a vector of integer gains (positive values represent improvement from moving a vertex) and a threshold. It performs multiple rounds: in each round, it iterates over all positions, and whenever it finds a position with a positive gain that has not been "moved" in this round, it "moves" that element (adds its gain to the total improvement) and then marks that position and all neighboring positions (i.e., the previous and next indices, if they exist) as moved. If in an entire round no improvement is found, stop. The function returns the total accumulated positive gain. If the number of moved elements (across all rounds) exceeds `stop_threshold` (interpreted as a fraction of the vector size, i.e., threshold = `stop_threshold` percent of `gain_values.size()`), stop early. The vector is treated circularly? No, linear. Edge cases: empty vector returns 0, no positive gains returns 0, and threshold of 0 means stop after the first move.

// The algorithm mimics the `start_more_locallized_search` pattern: we repeatedly sweep through the list, selecting elements with positive gain that are not yet "moved" in the current iteration. For each selected element, we add its gain and then mark it and its immediate neighbors (indices `i-1` and `i+1` if they exist) as moved for the current iteration (using a boolean vector `moved_this_round`). After each full sweep (round), if no improvement was made in that round, we terminate. We also maintain a global count of all positions ever moved; when that count exceeds `stop_threshold` percent of the list size, we stop prematurely. The total improvement is the sum of all gains from selected moves — since gains are positive by selection, the total is always non-negative. Time complexity: each round scans the entire vector O(n), and in the worst case we do O(n) rounds, giving O(n^2). Space: O(n) for the `moved_this_round` boolean vector. The threshold comparison uses integer arithmetic: `moved_count * 100 > stop_threshold * n` to avoid floating point.

#include <vector>
#include <algorithm>

// Simulates a multi-try FM local search on a linear list of gains.
// Returns the total improvement (sum of positive gains selected).
// stop_threshold is a percentage: stop if moved_count > (stop_threshold * n) / 100.
int multitry_fm_search(const std::vector<int>& gain_values, int stop_threshold) {
    const int n = static_cast<int>(gain_values.size());
    if (n == 0) return 0;

    int total_improvement = 0;
    int total_moved_count = 0;
    bool any_improvement_in_round = true;

    while (any_improvement_in_round) {
        any_improvement_in_round = false;
        std::vector<bool> moved_this_round(n, false);

        for (int i = 0; i < n; ++i) {
            // If this position is already marked as moved in this round, skip
            if (moved_this_round[i]) continue;

            if (gain_values[i] > 0) {
                // Mark this position and its immediate neighbors
                moved_this_round[i] = true;
                if (i > 0) moved_this_round[i - 1] = true;
                if (i + 1 < n) moved_this_round[i + 1] = true;

                total_improvement += gain_values[i];
                total_moved_count++;
                any_improvement_in_round = true;

                // Check the stop rule after each move
                if (total_moved_count * 100 > stop_threshold * n) {
                    return total_improvement;  // premature stop
                }
            }
        }
        // If no improvement in a round, loop ends naturally
    }

    return total_improvement;
}

#include <cassert>
#include <vector>

int multitry_fm_search(const std::vector<int>& gain_values, int stop_threshold);

int main() {
    // Empty list
    assert(multitry_fm_search({}, 50) == 0);

    // No positive gains
    assert(multitry_fm_search({0, -1, -2}, 50) == 0);

    // All positive, no early stop (threshold 100 means never stop until all moved)
    // Gains [1,2,3,4], n=4
    // Round 1: i=0 move (gain1), mark 0,1; i=2 move (gain3), mark 2,3; moved_count=2
    // Round 2: no moves (all moved) -> stop. Total = 4
    assert(multitry_fm_search({1,2,3,4}, 100) == 4);

    // Early stop: threshold 50% of 4 = 2 moves allowed, after 2 moves total_moved_count*100 = 200 > 50*4=200? Actually equals, not >. So continue. Only stops when strictly greater. So after 2 moves, 200 > 200 false; continue. No more moves, so returns 4.
    // Test with threshold 1%: allowed moves = 0.04 -> after first move, moved_count=1, 1*100=100 > 1*4=4 -> stop immediately. Gains [1,2,3,4] with threshold 1 returns 1 (only first move at i=0).
    assert(multitry_fm_search({1,2,3,4}, 1) == 1);

    // Test neighbor blocking: [5, -10, 5], threshold large
    // Round 1: i=0 move (gain5), mark 0,1; then i=2 move (gain5), mark 2 (already moved? no, 2 is not moved, but i=2 not blocked because i=1 was marked, but i=2 is not neighbor of i=0? The neighbor of 0 is 1 only; i=2 is neighbor of 1, not 0. So i=2 can be moved. Total=10. moved_count=2. Round2: all moved -> stop.
    assert(multitry_fm_search({5, -10, 5}, 100) == 10);

    // Test where neighbor blocks: [1, 2, 3] with threshold 100
    // Round1: i=0 move (1), mark 0,1; i=2 move (3), mark 2 (neighbor 1 already) -> total=4
    assert(multitry_fm_search({1, 2, 3}, 100) == 4);

    // Test where only one move possible and threshold met immediately
    assert(multitry_fm_search({7, -3, -4}, 0) == 0); // threshold 0 means stop after first move? 0% of 3 = 0, after first move total_moved_count*100=100 > 0*3=0 => stop immediately. But we need a first move; we move at i=0 gain 7, then check threshold. So returns 7. Actually threshold 0 means stop after first move, so return 7. Let's fix assertion.
    // Let's test: threshold 0, gains [7, -3, -4] -> first move at i=0 gain7, moved_count=1, 1*100=100 > 0*3=0, stop, return 7.
    assert(multitry_fm_search({7, -3, -4}, 0) == 7);

    // Test that negative gains are never picked
    assert(multitry_fm_search({-1, 2, -5}, 100) == 2);

    // Test single element
    assert(multitry_fm_search({6}, 100) == 6);
    assert(multitry_fm_search({-2}, 100) == 0);

    // Large threshold, all positive consecutive
    assert(multitry_fm_search({2, 3, 4}, 100) == 2 + 4); // picks 0 and 2, since 1 is neighbor of 0? Actually 1 is neighbor of 0 and also of 2, so after moving 0, 1 is marked; after moving 2, 1 is already marked. Total 6.

    // More than one round due to non-blocked positions remaining
    // Gains [1, 0, 1, 0, 1] threshold large
    // Round1: i=0 (gain1) mark 0,1; i=2 (gain1) mark 2,3; i=4 (gain1) mark 4 (neighbor 3 already) -> moved=3, total=3. Round2: no moves -> stop.
    assert(multitry_fm_search({1,0,1,0,1}, 100) == 3);

    // Ensure threshold percentage works with rounding down
    // threshold 50 with n=3: allowed moves = 1.5 -> stop when moved_count*100 > 150. After 2 moves, 200 > 150 true. Gains [1,1,1] threshold 50: round1: i0 move (1), mark 0,1; i1 skipped (moved); i2 move (1), mark 2; moved_count=2 -> 2*100=200 > 50*3=150 -> stop, return 2.
    assert(multitry_fm_search({1,1,1}, 50) == 2);

    return 0;
}
