// Write a C++ function named `majorityInEveryColumn` that takes a 2D integer vector `grid` with dimensions N rows and M columns, and returns a `std::vector<int>` containing the majority element of each column if every column has a unique strict majority element (appearing more than half the rows). If any column lacks a strict majority element, the function should return an empty vector. A strict majority in a column means an integer that appears strictly more than `N/2` times in that column. The function must handle N and M at least 1, and all grid entries are within `int` range. If all columns have a majority, return a vector of length M where the i-th element is the majority of column i. The result must be in column order from left to right.
// The core idea is to process each column independently. For column `j`, we gather the frequency of each distinct integer using an `unordered_map<int, int>`. The threshold for strict majority is `N/2` (integer division works because we need strictly more than half, so if N=5, threshold=2; a value appears at least 3 times to be majority). We iterate through the column, increment frequencies. After counting, we scan the map for any key with count > N/2. If exactly one such key exists, that's the majority; if zero or more than one (which is impossible because two values can't both exceed half the total), we return an empty vector. If found, we push it to the result. Edge cases: N=1, every element is trivially its own majority (threshold 0, any appears once > 0). N=2, threshold=1, so a column is valid only if both entries are the same. Also, we must ensure that the function returns empty immediately upon finding a column without a majority, not partially filled results. Time complexity: O(N*M) for scanning all cells, plus map operations per column. Space complexity: O(max distinct values in any column) for the map, plus O(M) for the result.
#include <vector>
#include <unordered_map>

// Return the majority element per column if every column has one, else an empty vector.
// A strict majority appears more than N/2 times in its column.
std::vector<int> majorityInEveryColumn(const std::vector<std::vector<int>>& grid) {
    const int N = grid.size();
    if (N == 0) return {};
    const int M = grid[0].size();
    if (M == 0) return {};

    std::vector<int> result;
    result.reserve(M);

    const int threshold = N / 2; // strict majority requires count > threshold

    for (int col = 0; col < M; ++col) {
        std::unordered_map<int, int> freq;
        for (int row = 0; row < N; ++row) {
            ++freq[grid[row][col]];
        }

        int majorityValue = 0;
        bool found = false;
        for (const auto& pair : freq) {
            if (pair.second > threshold) {
                if (found) {
                    // Two distinct values both exceed half, impossible, but defensive.
                    return {};
                }
                majorityValue = pair.first;
                found = true;
            }
        }
        if (!found) return {};
        result.push_back(majorityValue);
    }

    return result;
}
#include <cassert>
#include <vector>

// forward declaration of the function being tested
std::vector<int> majorityInEveryColumn(const std::vector<std::vector<int>>& grid);

int main() {
    // 3x3 grid, each column has a clear majority
    std::vector<std::vector<int>> grid1 = {{1, 2, 3}, {1, 2, 3}, {1, 4, 3}};
    std::vector<int> expected1 = {1, 2, 3};
    assert(majorityInEveryColumn(grid1) == expected1);

    // Single row, each element is its own majority
    std::vector<std::vector<int>> grid2 = {{7, -1, 9}};
    std::vector<int> expected2 = {7, -1, 9};
    assert(majorityInEveryColumn(grid2) == expected2);

    // 2 rows, column 0 lacks majority (different values), column 1 has majority
    std::vector<std::vector<int>> grid3 = {{5, 8}, {4, 8}};
    assert(majorityInEveryColumn(grid3).empty());

    // All same values
    std::vector<std::vector<int>> grid4 = {{2, 2}, {2, 2}, {2, 2}};
    std::vector<int> expected4 = {2, 2};
    assert(majorityInEveryColumn(grid4) == expected4);

    // Column with a value appearing exactly half (not majority)
    std::vector<std::vector<int>> grid5 = {{1, 3}, {2, 3}, {1, 4}};
    // col0: counts 1:2,2:1 => 1 has 2 > 1 (threshold=1) so majority=1
    // col1: counts 3:2,4:1 => 3 has 2 > 1 so majority=3
    std::vector<int> expected5 = {1, 3};
    assert(majorityInEveryColumn(grid5) == expected5);

    // 2 rows with equal values in one column, diff in another
    std::vector<std::vector<int>> grid6 = {{9, 1}, {9, 2}};
    // col0: 9 appears 2 > 1 => valid, col1: 1 and 2 each 1, not > 1 => empty
    assert(majorityInEveryColumn(grid6).empty());

    // Large grid, negative numbers
    std::vector<std::vector<int>> grid7 = {{-4, 0}, {-4, 0}, {-4, 5}, {-4, 5}};
    // N=4 threshold=2
    // col0: -4 appears 4 > 2 => majority -4
    // col1: 0 appears 2, 5 appears 2, neither >2 => empty
    assert(majorityInEveryColumn(grid7).empty());

    // Edge: empty grid
    std::vector<std::vector<int>> grid8 = {};
    assert(majorityInEveryColumn(grid8).empty());

    // Edge: grid with zero columns (but rows of zero length)
    std::vector<std::vector<int>> grid9 = {{}, {}, {}};
    assert(majorityInEveryColumn(grid9).empty());

    return 0;
}
