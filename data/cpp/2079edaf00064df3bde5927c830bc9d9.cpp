// Write a C++ function `std::vector<int> findMissingAndRepeatedValues(const std::vector<std::vector<int>>& grid)` that, given a square grid of integers (size n×n) containing values from 1 to n², returns a vector where the first element is the number that appears twice (the repeated value) and the second element is the number that appears zero times (the missing value). The input is assumed to contain exactly one repeated and one missing number; all other numbers appear exactly once. The function must work for any positive n, handle grids with n=1 (where the single element is both repeated and missing? – but per problem guarantee, n≥2), and not modify the input grid. The returned vector should have exactly two integers: `{repeated, missing}`.
#include <cassert>
#include <vector>

// Function prototype for testing (must match the solution)
std::vector<int> findMissingAndRepeatedValues(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: 2x2 grid with repeated 2 and missing 4
    std::vector<std::vector<int>> grid1 = {{1, 3}, {2, 2}};
    assert(findMissingAndRepeatedValues(grid1) == std::vector<int>({2, 4}));

    // Test 2: 3x3 grid with repeated 9 and missing 5
    std::vector<std::vector<int>> grid2 = {{1, 2, 3}, {4, 9, 6}, {7, 8, 9}};
    assert(findMissingAndRepeatedValues(grid2) == std::vector<int>({9, 5}));

    // Test 3: 2x2 grid with repeated 1 and missing 3
    std::vector<std::vector<int>> grid3 = {{1, 4}, {1, 2}};
    assert(findMissingAndRepeatedValues(grid3) == std::vector<int>({1, 3}));

    // Test 4: 1x1 grid? Not valid per problem guarantee, but test behavior (if it had to handle, both would be same value)
    // Without guarantee, this would be ambiguous; we skip since n>=2 assumed.

    // Test 5: 3x3 grid with repeated 4 and missing 7
    std::vector<std::vector<int>> grid4 = {{7, 8, 9}, {1, 2, 3}, {4, 4, 6}};
    assert(findMissingAndRepeatedValues(grid4) == std::vector<int>({4, 5}));

    // Test 6: Larger grid 4x4, repeated 16, missing 1
    std::vector<std::vector<int>> grid5 = {{16, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    assert(findMissingAndRepeatedValues(grid5) == std::vector<int>({16, 1}));

    // Test 7: Repeated and missing at extremes: repeated 1, missing 16
    std::vector<std::vector<int>> grid6 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 1}};
    assert(findMissingAndRepeatedValues(grid6) == std::vector<int>({1, 16}));

    // Test 8: Duplicate in first cell, missing last
    std::vector<std::vector<int>> grid7 = {{2, 2}, {3, 4}};  // actually this is 2x2, but 2 repeated, 1 missing
    assert(findMissingAndRepeatedValues(grid7) == std::vector<int>({2, 1}));

    // Test 9: All normal except missing one at end
    std::vector<std::vector<int>> grid8 = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}, {16, 17, 18, 19, 20}, {21, 22, 23, 24, 24}};
    assert(findMissingAndRepeatedValues(grid8) == std::vector<int>({24, 25}));

    // Test 10: Very small grid with n=2 and duplicate at end
    std::vector<std::vector<int>> grid9 = {{1, 2}, {3, 3}};
    assert(findMissingAndRepeatedValues(grid9) == std::vector<int>({3, 4}));

    return 0;
}
#include <vector>
#include <unordered_map>

// Given a square grid of integers containing values 1..n^2 with exactly one repeated and one missing,
// return a vector {repeated, missing}.
std::vector<int> findMissingAndRepeatedValues(const std::vector<std::vector<int>>& grid) {
    int n = static_cast<int>(grid.size());
    int total = n * n;

    // Count frequencies for all numbers from 1 to total
    std::unordered_map<int, int> freq;
    for (int i = 1; i <= total; ++i) {
        freq[i] = 0;
    }

    // Count actual occurrences in the grid
    for (const auto& row : grid) {
        for (int value : row) {
            ++freq[value];
        }
    }

    // Find the repeated (count 2) and missing (count 0)
    int repeated = 0;
    int missing = 0;
    for (const auto& pair : freq) {
        if (pair.second == 2) {
            repeated = pair.first;
        } else if (pair.second == 0) {
            missing = pair.first;
        }
    }

    return {repeated, missing};
}
// The simplest approach is to count frequencies of all numbers from 1 to n² using an `unordered_map` (or a fixed-size vector, but the map makes it general). Initialize counts for all possible values to 0, then iterate over the grid incrementing the count for each element. After counting, scan through the map (or numbers 1..n²) to find the value with count 2 (repeated) and the value with count 0 (missing). Return them in that order. Edge cases: n=1 would give all numbers as 1..1, but the problem guarantee says exactly one repeated and one missing, so for n=1 that would be impossible; hence we assume n≥2. Time complexity is O(n²) because we process each cell once and then scan n² numbers. Space complexity is O(n²) for the frequency map. An alternative optimization would use a vector of size n²+1, but the map is more flexible and still meets constraints.
