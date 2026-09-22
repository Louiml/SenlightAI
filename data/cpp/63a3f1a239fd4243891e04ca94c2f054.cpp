You are given two integer arrays `dist` and `speed`, each of length `n`, representing the initial distance (in miles) and speed (in miles per minute) of `n` monsters approaching a city. At each minute (starting at minute 0), exactly one monster can be eliminated before it reaches the city, and this elimination must happen at the start of that minute. However, if any monster reaches the city at or before that minute (i.e., its distance divided by its speed gives arrival time less than or equal to the current minute), the game ends immediately. Write a C++ function `int maxMonstersEliminated(const std::vector<int>& dist, const std::vector<int>& speed)` that returns the maximum number of monsters you can eliminate before the game ends. You may assume all distances and speeds are positive integers, and `n >= 1`.
#include <cassert>
#include <vector>

int main() {
    // Example from original snippet
    std::vector<int> dist1 = {1, 3, 4};
    std::vector<int> speed1 = {1, 1, 1};
    assert(maxMonstersEliminated(dist1, speed1) == 3);

    // Monsters arrive very quickly
    std::vector<int> dist2 = {1, 2, 3};
    std::vector<int> speed2 = {10, 10, 10};
    // times: (0, 0, 0) -> can eliminate only first at minute 0, then second arrives at 0.2 -> fail at minute 1
    assert(maxMonstersEliminated(dist2, speed2) == 1);

    // One monster
    std::vector<int> dist3 = {5};
    std::vector<int> speed3 = {2};
    assert(maxMonstersEliminated(dist3, speed3) == 1);

    // All same deadline 0
    std::vector<int> dist4 = {2, 3, 4};
    std::vector<int> speed4 = {2, 3, 4};
    // times: (0, 0, 0) -> only 1 can be eliminated
    assert(maxMonstersEliminated(dist4, speed4) == 1);

    // Larger distances give more time
    std::vector<int> dist5 = {10, 20, 30};
    std::vector<int> speed5 = {5, 1, 3};
    // times: (1, 19, 9) sorted -> (1,9,19) -> can eliminate all 3
    assert(maxMonstersEliminated(dist5, speed5) == 3);

    // Mixed case
    std::vector<int> dist6 = {4, 2, 10};
    std::vector<int> speed6 = {2, 1, 5};
    // times: (1, 1, 1) -> only 1
    assert(maxMonstersEliminated(dist6, speed6) == 1);

    // Edge: exactly on time
    std::vector<int> dist7 = {3, 3, 3};
    std::vector<int> speed7 = {3, 3, 3};
    // times: (0,0,0) -> only 1
    assert(maxMonstersEliminated(dist7, speed7) == 1);

    // Edge: one monster with huge distance
    std::vector<int> dist8 = {100};
    std::vector<int> speed8 = {1};
    assert(maxMonstersEliminated(dist8, speed8) == 1);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum number of monsters that can be eliminated before any
// reaches the city, given initial distances and speeds.
int maxMonstersEliminated(const std::vector<int>& dist, const std::vector<int>& speed) {
    const int n = static_cast<int>(dist.size());
    std::vector<int> times;
    times.reserve(n);
    for (int i = 0; i < n; ++i) {
        // Latest minute at which this monster can be eliminated
        times.push_back((dist[i] - 1) / speed[i]);
    }
    std::sort(times.begin(), times.end());
    for (int i = 0; i < n; ++i) {
        if (times[i] < i) {
            // At minute i, this monster has already arrived
            return i;
        }
    }
    return n;
}
// The key insight is that you can eliminate monsters in order of how soon they arrive. For each monster, compute the exact minute by which it must be destroyed (the latest minute it can be eliminated without reaching the city). Since a monster with distance `d` and speed `s` arrives at time `d / s` (as a real number), the last minute you can eliminate it without it being there at the start of that minute is `ceil(d/s) - 1`. Since all times are integer minutes and you eliminate at the very beginning of a minute, the condition is that you can eliminate a monster at minute `t` if `t < d/s`, equivalently `t * s < d`, so the maximum `t` is `(d-1)/s` using integer division.  
//
// Algorithm:  
// 1. Compute `times[i] = (dist[i] - 1) / speed[i]` for each monster — the last minute at which it can be eliminated.  
// 2. Sort these `times` in non-decreasing order, because the optimal strategy is to always eliminate the monster with the earliest deadline next.  
// 3. Iterate over the sorted times with minute index `i` starting from 0. If at minute `i` the current monster's deadline is less than `i`, i.e., `times[i] < i`, then that monster cannot be eliminated (it would have arrived by minute `i`), so the game ends and we return `i` (the number of monsters successfully eliminated before this point).  
// 4. If all monsters can be eliminated (the loop completes without returning), return `n`.  
//
// Edge cases:  
// - If a monster has distance exactly equal to speed, then `(d-1)/s = 0`, so it can only be eliminated at minute 0.  
// - If all monsters have very large times, the loop completes and returns `n`.  
// - The input arrays are non-empty and positive integers, so no division by zero.  
// Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the `times` vector.
