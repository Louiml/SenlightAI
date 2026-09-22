// Write a C++ function `int carFleetCount(int target, const std::vector<int>& positions, const std::vector<int>& speeds)` that simulates the classic "car fleet" problem. Given a one-lane road with a common destination at `target` (a positive integer), and for each car its starting `position` (non-negative integer less than `target`) and constant `speed` (positive integer), all cars move at their constant speed toward the destination. A car will never pass another car; if a faster car catches up to a slower car ahead, they merge into a single fleet moving at the slower car's speed. Additionally, any car that reaches the destination "disappears" and is considered separately from any fleet that has already arrived. The function must return the number of distinct fleets that will arrive at the destination, counting a single car as a fleet of size one. If two or more cars form a fleet, they count as one fleet. The input arrays have the same length `n` (1 ≤ n ≤ 10^5), and positions are distinct. You may assume that the total distance to travel from each position is positive. Implement the function with appropriate `const` correctness and no global state.
#include <cassert>
#include <vector>

// Forward declaration (the solution function is defined elsewhere)
int carFleetCount(int target, const std::vector<int>& positions, const std::vector<int>& speeds);

int main() {
    // Example 1: Same speed, all merge into one fleet
    assert(carFleetCount(10, {0, 4, 2}, {2, 2, 2}) == 1);

    // Example 2: Different speeds, one behind catches up
    // Positions: 0 (speed 1), 3 (speed 5), 5 (speed 2), target 10
    // Times: 10, 1.4, 2.5 -> sorted by position descending: 5(time2.5), 3(time1.4), 0(time10)
    // Traverse: 2.5 -> push, 1.4 <= top? no (1.4 < 2.5) merge, 10 > top? push -> 2 fleets
    assert(carFleetCount(10, {0, 3, 5}, {1, 5, 2}) == 2);

    // Example 3: Single car
    assert(carFleetCount(100, {50}, {10}) == 1);

    // Example 4: All separate fleets (each car slower than the one ahead)
    // Positions descending: 8(speed1, time=2), 5(speed2, time=2.5), 2(speed3, time=2.666)
    // All increasing times, so each forms a new fleet -> 3
    assert(carFleetCount(10, {2, 5, 8}, {3, 2, 1}) == 3);

    // Example 5: Edge case where a fast car behind merges multiple fleets
    // target=100, positions: 90(speed 10, time=1), 80(speed 5, time=4), 0(speed 1, time=100)
    // Sorted descending: 90(1), 80(4), 0(100)
    // 1 push, 4 > 1 push, 100 > 4 push -> 3 fleets? Actually 0 speed1 takes 100, but can it catch 80? 80 has time 4, far ahead, so no. All separate.
    // But if we want merging: target=10, positions: 5(speed 1, time=5), 3(speed 3, time=2.33), 0(speed 10, time=1)
    // Sorted: 5(5), 3(2.33), 0(1). 5 push, 2.33<5 merge (no push), 1<5 merge (no push) -> 1 fleet? Actually 3 car speed3 catches 5? time 2.33<5 yes merges; 0 speed10 time1 catches the merged fleet (still time5) yes merges. So 1 fleet. Test: target=10, positions {0,3,5}, speeds {10,3,1} -> 1.
    assert(carFleetCount(10, {0, 3, 5}, {10, 3, 1}) == 1);

    // Example 6: Two fleets separated by a gap
    // target=20, positions: 15(speed5, time=1), 10(speed2, time=5), 5(speed10, time=1.5)
    // Sorted: 15(1), 10(5), 5(1.5) -> 1 push, 5>1 push, 1.5<5 merge -> total 2
    assert(carFleetCount(20, {5, 10, 15}, {10, 2, 5}) == 2);

    // Example 7: Empty input (should return 0)
    assert(carFleetCount(20, {}, {}) == 0);

    // Example 8: Many cars, all same time (same speed and same? but positions distinct, all same speed gives different times)
    // target=10, positions {0,2,4}, speeds {2,2,2} -> times 5,4,3 -> sorted desc: 4(3),2(4),0(5) -> all increasing? 3 push, 4>3 push, 5>4 push -> 3 fleets? Actually each slower (larger time) behind, so no merging. 3 fleets.
    assert(carFleetCount(10, {0,2,4}, {2,2,2}) == 3);

    // Example 9: Cars at same position (not allowed per spec, but test distinct)
    // Already covered.

    // Example 10: Large values with precision
    assert(carFleetCount(1000000, {0, 1, 2}, {1000000, 1, 1}) == 2); // 0 time=1, 1 time=999999, 2 time=999998 -> sorted desc: 2(time999998),1(999999),0(1) -> 999998 push, 999999>top push, 1<top merge -> 2 fleets
    assert(carFleetCount(1000000, {0, 1, 2}, {1000000, 1, 1}) == 2);

    return 0;
}
#include <vector>
#include <algorithm>
#include <stack>
#include <cstddef>

// Counts the number of car fleets that will arrive at the target.
// Cars are sorted by distance from target (closest first), and we merge
// those that can catch up to the fleet ahead.
int carFleetCount(int target, const std::vector<int>& positions, const std::vector<int>& speeds) {
    const std::size_t n = positions.size();
    if (n == 0) return 0;

    // Create a vector of pairs: (distance remaining, speed)
    std::vector<std::pair<int, int>> carData;
    carData.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        carData.emplace_back(target - positions[i], speeds[i]);
    }

    // Sort by distance remaining ascending (which is same as position descending)
    std::sort(carData.begin(), carData.end());

    // Compute arrival times as double to avoid precision issues
    std::vector<double> arrivalTimes(n);
    for (std::size_t i = 0; i < n; ++i) {
        arrivalTimes[i] = static_cast<double>(carData[i].first) / carData[i].second;
    }

    // Stack to store arrival times of distinct fleets (in order from closest to farthest)
    std::stack<double> fleetTimes;
    for (std::size_t i = 0; i < n; ++i) {
        // If the current car (behind) takes at least as long as the fleet ahead,
        // it cannot catch up; it will be a separate fleet.
        // But we need to check against the fleet currently at the top of the stack.
        if (!fleetTimes.empty() && arrivalTimes[i] >= fleetTimes.top()) {
            // This car is not fast enough to catch the fleet ahead, so push it as a new fleet.
            // However, we must also consider that this car might be caught by later cars.
            // So simply push.
            fleetTimes.push(arrivalTimes[i]);
        } else {
            // The current car is faster than the fleet ahead, so it will catch up.
            // Remove all fleets that it can catch (these have smaller or equal time).
            while (!fleetTimes.empty() && arrivalTimes[i] < fleetTimes.top()) {
                fleetTimes.pop(); // merge with the fleet ahead
            }
            // After merging, push the current car's time as the new fleet's time.
            // Note: the merged fleet's arrival time is the minimum of the times? Actually,
            // since the front car is slower (larger time), but we popped it because the behind car
            // is faster (smaller time) and catches up, the combined fleet moves at the front car's
            // speed, so its arrival time is the popped time (the larger one). But we don't need to
            // restore it because the fleet ahead has been popped; we're representing the new fleet
            // that now is the same as the popped one, but we'll just push the current car's time?
            // Wait, careful: The fleet's arrival time is determined by the slowest car, which is
            // the one with the largest time. Since we popped the fleet ahead (which had a larger time),
            // we should push that larger time, not the current car's time. But the current car's time
            // is smaller. So the correct approach is to push the larger time. However, in the standard
            // solution, they simply push the current car's time, which is wrong because the merged fleet
            // arrives at the time of the slower car. Let's think again.

            // Standard solution: sort by position (distance) descending (closest first), and
            // for each car from farthest to closest? Actually the standard LeetCode solution:
            // sort by starting position ascending (closest to target first), then iterate from
            // closest to farthest, and if the time is greater than the last fleet's time, it's a new fleet;
            // else it merges into the last fleet (no new fleet). So the correct condition is:
            // if arrivalTimes[i] > top of stack, push (new fleet); else do nothing (merged).
            // Because the fleet ahead has the smallest time among the fleet. Let's adopt that correct logic.

            // Let me restructure: The correct algorithm is:
            // Sort by position descending (closest to target first). Compute times.
            // For each time in order: if stack empty or time > stack.top(), push it (new fleet).
            // Otherwise, it merges with the fleet ahead, so do nothing.
            // The stack size at the end is the answer.
            // That is much simpler and avoids the complex popping.
            // I'll use that correct version below.
        }
    }

    // The above logic is flawed. I'll rewrite the function properly.
    // (To keep the output clean, I'll provide the correct implementation below.)
    // Re-implement:
    // Sort cars by position descending (closest to target first).
    // Use a vector of indices or pairs.
    std::vector<std::pair<int, int>> cars; // (position, speed)
    for (std::size_t i = 0; i < n; ++i) {
        cars.emplace_back(positions[i], speeds[i]);
    }
    std::sort(cars.begin(), cars.end(), [](const auto& a, const auto& b) {
        return a.first > b.first; // descending by position
    });

    std::stack<double> fleet;
    for (const auto& car : cars) {
        double time = static_cast<double>(target - car.first) / car.second;
        if (fleet.empty() || time > fleet.top()) {
            fleet.push(time); // forms a new fleet
        }
        // else it merges into the fleet ahead, no push
    }
    return static_cast<int>(fleet.size());
}
// The core observation is that for each car, we can compute the time it would take to reach the target if it were alone: `time = (target - position) / speed`. Since cars cannot pass each other, a car with a longer arrival time behind a car with a shorter arrival time will catch up to the car ahead (or the fleet) and merge, because the ahead car is slower in terms of time to target (i.e., it takes longer). Actually, the correct condition is: sort cars by their starting position from closest to the target (largest position) to farthest (smallest position). Traverse from the closest to the destination towards the farthest. Maintain a stack of arrival times for the fleets that form. For each car in this order, if its arrival time is strictly less than the arrival time of the last fleet in the stack, it means this car is faster than the fleet ahead (since it has a smaller time to target) and will catch up, but because we go from closest to farthest, the faster car is behind the slower fleet? Let’s reason carefully: Sort by distance to target ascending (i.e., position descending). Process from closest to farthest. For a car behind (with larger distance to travel), if its time-to-target is greater than or equal to the time of the fleet directly ahead (which is the last in stack), then it will catch up to that fleet before reaching the target, because the fleet ahead arrives no later than this car? Actually, the fleet ahead is closer to target, so it has less distance. If the behind car's time >= ahead fleet's time, then the behind car is slower or equal in speed (since it has more distance but takes more or equal time), so it will never catch up; it forms a separate fleet. If behind car's time < ahead fleet's time, then behind car is faster and will catch up, so it merges; we should pop the ahead fleet and possibly continue. The algorithm: sort by distance to target ascending (position descending). Compute times. Initialize stack with first (closest) car's time. For each subsequent car i from second to last (i.e., moving away from target): while stack not empty and time[i] >= stack.top() (the behind car is not fast enough to catch the fleet ahead? Actually top is the fleet immediately ahead; if time[i] >= top.time, then the behind car cannot catch up because it takes at least as long to cover more distance, so it forms a new fleet; break. If time[i] < top.time, it will catch up, so pop the top (merge) and continue checking next ahead. After loop, push time[i] (representing the new fleet that either merged with some or is separate). Finally the answer is stack size. Edge cases: n=1, all same speed, multiple fleets merging into one, floating point precision. Use `double` instead of `float` to avoid precision issues. Time complexity O(n log n) due to sorting, space O(n) for the vectors and stack. The code snippet uses float and a stack but has an incorrect counting; the corrected standard approach returns stack size.
