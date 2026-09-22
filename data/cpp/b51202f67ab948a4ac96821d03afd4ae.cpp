Write a C++ function that takes a non-empty 2D vector of integers `grid` and a positive integer `x`, and returns the minimum number of operations required to make all elements in the grid equal. In one operation, you can add or subtract `x` from any single element. If it is impossible to make all elements equal, return -1. The grid dimensions and all values fit within `int` range.

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minOperations({{2, 4}, {6, 8}}, 2) == 4);
    assert(minOperations({{1, 5}, {9, 13}}, 4) == 6);

    // Already equal
    assert(minOperations({{7, 7}, {7, 7}}, 2) == 0);

    // 1x1 grid
    assert(minOperations({{10}}, 3) == 0);

    // Impossible due to mismatched remainders
    assert(minOperations({{1, 2}, {3, 4}}, 2) == -1);
    assert(minOperations({{3, 6}, {9, 10}}, 3) == -1);

    // Single row, odd length
    assert(minOperations({{1, 3, 5}}, 2) == 2); // target=3

    // Single row, even length with two valid medians
    assert(minOperations({{0, 4, 8, 12}}, 4) == 4); // target=4 or 8

    // Negative values with same remainders
    assert(minOperations({{-5, 1, 7}}, 6) == 2); // target=1

    // Larger grid, all same remainder
    assert(minOperations({{3, 9, 15}, {21, 27, 33}}, 6) == 12);

    // Duplicate values
    assert(minOperations({{5, 5}, {5, 5}}, 3) == 0);

    // All equal remainder but different values
    assert(minOperations({{2, 8, 14, 20}}, 6) == 6); // target=8 or 14

    return 0;
}

#include <vector>
#include <algorithm>
#include <cmath>

// Returns the minimum number of +/- x operations to make all grid elements equal,
// or -1 if impossible.
int minOperations(const std::vector<std::vector<int>>& grid, int x) {
    int rows = grid.size();
    int cols = grid[0].size();
    int total = rows * cols;

    // Check that all remainders modulo x are identical.
    int remainder = grid[0][0] % x;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] % x != remainder) {
                return -1;
            }
        }
    }

    // Flatten and sort.
    std::vector<int> values;
    values.reserve(total);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            values.push_back(grid[i][j]);
        }
    }
    std::sort(values.begin(), values.end());

    // Helper lambda to compute cost for a given target.
    auto costToTarget = [&](int target) {
        long long cost = 0;
        for (int v : values) {
            cost += std::abs(target - v) / x;
        }
        return cost;
    };

    // For odd length, median is the middle element.
    if (total % 2 == 1) {
        return static_cast<int>(costToTarget(values[total / 2]));
    }

    // For even length, try both middle elements.
    long long cost1 = costToTarget(values[total / 2]);
    long long cost2 = costToTarget(values[total / 2 - 1]);
    return static_cast<int>(std::min(cost1, cost2));
}

// The core insight is that all numbers must be congruent modulo `x` to be transformable into each other by repeatedly adding/subtracting `x`. If any remainder differs, return -1. After validating that, flatten the grid into a 1D vector and sort it. The optimal target value is a median of the sorted array because the sum of absolute deviations is minimized at a median. For an even-length array, both the two middle values are valid medians, so compute the cost for both and take the minimum. The cost for each element is `abs(target - element) / x` since the difference is divisible by `x`. Edge cases: a 1x1 grid returns 0; unsolvable grids return -1 immediately; duplicate values are naturally handled. Time complexity is O(N log N) where N = rows × cols due to sorting; space complexity is O(N) for the flattened vector.
