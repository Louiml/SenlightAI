// Write a C++ function `simulate_odometer` that takes an integer `num_pulses_for_incr` (the number of wheel pulses needed for one distance increment), a double `dist_incr` (the distance in km added per increment), and an integer `max_total_increments` (the total number of increments before stopping, i.e., when total distance reaches `max_total_increments * dist_incr`). The function should simulate a simple odometer that processes pulses: it waits for exactly `num_pulses_for_incr` pulses to accumulate one distance increment, then increments both total and partial distances by `dist_incr`. The simulation should continue until `max_total_increments` increments have been made (i.e., total distance equals `max_total_increments * dist_incr`), at which point the process stops. The function must return a `std::vector<std::pair<double, double>>` where each pair contains `(total_distance, partial_distance)` at each increment (i.e., after each increment, before stopping). You may assume all inputs are positive integers/doubles and `num_pulses_for_incr > 0` and `max_total_increments >= 1`. The function should not print anything; it just collects the distances in order.
// The simulation follows a straightforward state-update loop. Maintain two accumulators `total` and `partial`, both initially `0.0`. For each of the `max_total_increments` increments, we conceptually "wait" for `num_pulses_for_incr` pulses (since pulses are not individually tracked, this is just a loop counter; no event-driven logic is needed). After waiting, we add `dist_incr` to both `total` and `partial`, then record the pair into the result vector. The loop runs exactly `max_total_increments` times. Edge cases: when `max_total_increments == 1`, the output contains exactly one pair with both values equal to `dist_incr`. When `dist_incr` is a fractional value (e.g., 0.5), the accumulated values may have floating-point rounding, so tests should use exact comparisons only if the increments are exactly representable (e.g., integer or simple fractions like 0.5). The algorithm runs in O(max_total_increments) time and uses O(max_total_increments) space for the result vector, though the process itself only uses constant extra space. There are no other edge cases because inputs are guaranteed positive and the loop count is fixed.
#include <vector>
#include <utility>

// Simulate an odometer that increments distances after every num_pulses_for_incr pulses.
// Returns a vector of (total, partial) distances after each increment, in order.
std::vector<std::pair<double, double>> simulate_odometer(
    int num_pulses_for_incr,
    double dist_incr,
    int max_total_increments
) {
    std::vector<std::pair<double, double>> result;
    result.reserve(max_total_increments);

    double total = 0.0;
    double partial = 0.0;

    for (int i = 0; i < max_total_increments; ++i) {
        // Wait for the required number of pulses (conceptually).
        // Since pulses are not individually tracked, we just loop.
        for (int pulse = 0; pulse < num_pulses_for_incr; ++pulse) {
            // Each pulse is simulated; no action needed per pulse in this simplified model.
        }

        total += dist_incr;
        partial += dist_incr;
        result.emplace_back(total, partial);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// Declaration of solution function (include header or copy above)
std::vector<std::pair<double, double>> simulate_odometer(int, double, int);

int main() {
    // Basic case: 1 pulse per increment, increment 2.0, 3 increments.
    auto r = simulate_odometer(1, 2.0, 3);
    assert(r.size() == 3);
    assert(r[0] == std::make_pair(2.0, 2.0));
    assert(r[1] == std::make_pair(4.0, 4.0));
    assert(r[2] == std::make_pair(6.0, 6.0));

    // More pulses per increment.
    r = simulate_odometer(5, 1.0, 2);
    assert(r.size() == 2);
    assert(r[0] == std::make_pair(1.0, 1.0));
    assert(r[1] == std::make_pair(2.0, 2.0));

    // Single increment.
    r = simulate_odometer(3, 0.5, 1);
    assert(r.size() == 1);
    assert(r[0].first == 0.5);
    assert(r[0].second == 0.5);

    // Larger number of increments.
    r = simulate_odometer(2, 1.5, 4);
    assert(r.size() == 4);
    assert(r[3] == std::make_pair(6.0, 6.0));

    // Verify all pairs have equal total and partial.
    r = simulate_odometer(10, 1.0, 5);
    for (const auto& p : r) {
        assert(p.first == p.second);
    }

    // Check the final total equals max increments * dist_incr.
    const double DIST = 0.25;
    const int MAX_INC = 8;
    r = simulate_odometer(7, DIST, MAX_INC);
    assert(r.back().first == MAX_INC * DIST);
    assert(r.back().second == MAX_INC * DIST);

    return 0;
}
