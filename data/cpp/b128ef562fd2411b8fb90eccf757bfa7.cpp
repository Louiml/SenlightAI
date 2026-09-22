Write a C++ function `int lastIceCreamBowl(int n, int m, const std::vector<double>& quantities)` that solves the following problem: There are `n` ice cream bowls, each with a certain quantity of ice cream (in grams, given as a positive integer when read but stored as double). You have a scoop that can hold exactly `m` grams. For each bowl, compute the number of scoops needed to completely serve that bowl, which is the ceiling of `quantity / m`. The function must return the 1-based index of the bowl that requires the maximum number of scoops. If multiple bowls require the same maximum number of scoops, return the index of the last one encountered (i.e., the largest index among those with the maximum value). The input `quantities` will have exactly `n` elements, all positive, and `n` and `m` are positive integers. No two bowls have the same quantity? Actually they can, but that does not affect the logic. The function should handle `n` from 1 to 10^5 and values up to 10^9, so use 64-bit for intermediate calculations if needed, though the output is an int.

The core algorithm is a single pass over the input array. For each element `q` at index `i` (0-based), we compute `ceil(q / m)`. We track the maximum value seen so far and the index (1-based) associated with the latest occurrence of that maximum. The update condition uses `>=` so that later indices are preferred when the ceiling value is equal to the current maximum. Initialization: set `max_scoops = 0` (or negative) and `last_index = -1`. Since all quantities are positive, the first element will always set the max. Edge cases: if `n = 1`, the function returns 1. The use of `double` for quantity and `ceil` is fine, but to avoid floating-point precision issues with large numbers, it is safer to compute ceiling using integer arithmetic: `(q + m - 1) / m` if `q` and `m` are integers. Since the problem statement says quantities are integers read as doubles, we can cast to `long long` for division. The time complexity is O(n), space complexity O(1) (excluding the input vector). The function is pure and uses `const` reference for the vector.

#include <vector>
#include <cstdint>

// Given n bowls with quantities q_i (stored in vector quantities of length n),
// and a scoop of size m, return the 1-based index of the bowl requiring the most scoops.
// In case of a tie, return the largest index.
int lastIceCreamBowl(int n, int m, const std::vector<double>& quantities) {
    // Use integer arithmetic to avoid floating-point precision issues.
    long long max_scoops = -1; // will always be replaced
    int best_index = -1;

    for (int i = 0; i < n; ++i) {
        long long q = static_cast<long long>(quantities[i]);
        // Ceiling division for positive integers: (q + m - 1) / m
        long long scoops = (q + m - 1) / m;
        // Use >= to prefer the last index in case of a tie.
        if (scoops >= max_scoops) {
            max_scoops = scoops;
            best_index = i + 1; // convert to 1-based
        }
    }
    return best_index;
}

#include <cassert>
#include <vector>

// Include the solution function here or via a header.
// For brevity, the function is assumed to be defined above.

int main() {
    // Basic single bowl
    assert(lastIceCreamBowl(1, 5, {3.0}) == 1);
    // Exact multiple
    assert(lastIceCreamBowl(2, 10, {10.0, 20.0}) == 2); // both need 1 and 2 scoops
    // Tie: both need 2 scoops, return last index
    assert(lastIceCreamBowl(2, 5, {6.0, 9.0}) == 2); // ceil(6/5)=2, ceil(9/5)=2
    // Tie with multiple, ensure last (largest) index
    assert(lastIceCreamBowl(3, 3, {3.0, 3.0, 3.0}) == 3); // all need 1
    // Larger values to avoid floating error
    assert(lastIceCreamBowl(3, 2, {1000000000.0, 1.0, 999999999.0}) == 1); // first needs 500M, others 1 and ~500M
    // Mixed
    assert(lastIceCreamBowl(4, 7, {1.0, 7.0, 8.0, 14.0}) == 4); // scoops:1,1,2,2 => last max is index 4
    // All same quantities
    assert(lastIceCreamBowl(5, 4, {4.5, 4.5, 4.5, 4.5, 4.5}) == 5); // each ceil(4.5/4)=2, tie -> last
    // Check that exact divisibility works
    assert(lastIceCreamBowl(2, 6, {12.0, 18.0}) == 2); // 2 and 3 scoops
    // Switching maxima
    assert(lastIceCreamBowl(3, 5, {10.0, 6.0, 7.0}) == 3); // 2,2,2 tie -> last
    // Single large value
    assert(lastIceCreamBowl(2, 3, {2.0, 100.0}) == 2); // 1 and 34
    return 0;
}
