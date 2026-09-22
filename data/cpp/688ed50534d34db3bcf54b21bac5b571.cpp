// Given a square grid of integers where each value is between 1 and n*n (inclusive), and where exactly one number appears twice and exactly one number is missing, write a C++ function that returns a vector of two integers: the repeated value first, and the missing value second. The input grid is of size n x n with n ≥ 1, and the grid contains all numbers from 1 to n*n except one, with one extra duplicate replacing the missing number. The function should handle edge cases like n=1 (where the only grid cell contains 1, so the repeated and missing must be identified as per the constraints—though for n=1 the problem is degenerate; assume exactly one repeated and one missing exist, which is impossible for n=1, so for n=1 just return {1,1} or handle gracefully). For n≥2, the grid always satisfies the condition that exactly one number occurs twice and exactly one number does not appear.

// The solution uses a counting array of size n*n+1, initialized to zeros. We iterate through the entire grid and increment the count for each value encountered. After counting, we scan indices from 1 to n*n and identify which value has count equal to 2 (the repeated) and which has count equal to 0 (the missing). We then return these in order {repeated, missing}. Edge cases: if n=1, the grid contains a single cell with value 1, so counts[1]=1, and we would not find a repeated or missing; to satisfy the problem's guarantee, we can handle it by returning {-1,-1} or by assuming that for n=1 the input is invalid; but since the problem statement says exactly one repeated and one missing exist, for n=1 that is impossible, so we can either return {1,1} or handle it gracefully. In practice, the problem will have n≥2. Time complexity is O(n^2) for counting and O(n^2) for scanning the count array, so overall O(n^2). Space complexity is O(n^2) for the count array.

#include <vector>

/**
 * Finds the repeated and missing values in a square grid.
 * The grid contains numbers from 1 to n*n, with exactly one duplicate and one missing.
 * 
 * @param grid An n x n matrix of integers satisfying the constraints.
 * @return A vector of two integers: {repeated, missing}.
 */
std::vector<int> findRepeatedAndMissing(const std::vector<std::vector<int>>& grid) {
    int n = static_cast<int>(grid.size());
    int total = n * n;
    std::vector<int> count(total + 1, 0);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ++count[grid[i][j]];
        }
    }
    
    int repeated = -1;
    int missing = -1;
    for (int value = 1; value <= total; ++value) {
        if (count[value] == 2) {
            repeated = value;
        } else if (count[value] == 0) {
            missing = value;
        }
    }
    
    // For safety, if n==1 and the grid is [1], we return {1,1} as fallback.
    if (repeated == -1 || missing == -1) {
        repeated = 1;
        missing = 1;
    }
    
    return {repeated, missing};
}

#include <cassert>
#include <vector>

// Declaration of the function under test (assumes the Solution code is included above).
std::vector<int> findRepeatedAndMissing(const std::vector<std::vector<int>>& grid);

int main() {
    // Basic 2x2 case: numbers 1..4, missing 3, repeated 2
    std::vector<std::vector<int>> grid1 = {{1, 2}, {2, 4}};
    assert(findRepeatedAndMissing(grid1) == std::vector<int>({2, 3}));
    
    // 3x3 case: numbers 1..9, missing 5, repeated 9
    std::vector<std::vector<int>> grid2 = {{1, 2, 3}, {4, 9, 6}, {7, 8, 9}};
    assert(findRepeatedAndMissing(grid2) == std::vector<int>({9, 5}));
    
    // 2x2 case with repeated at boundary: missing 1, repeated 4
    std::vector<std::vector<int>> grid3 = {{4, 2}, {3, 4}};
    assert(findRepeatedAndMissing(grid3) == std::vector<int>({4, 1}));
    
    // 1x1 case might be invalid, but we test fallback
    std::vector<std::vector<int>> grid4 = {{1}};
    assert(findRepeatedAndMissing(grid4) == std::vector<int>({1, 1}));
    
    // 4x4 case: numbers 1..16, missing 12, repeated 7
    std::vector<std::vector<int>> grid5 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 7},
        {13, 14, 15, 16}
    };
    assert(findRepeatedAndMissing(grid5) == std::vector<int>({7, 12}));
    
    // Another 2x2 case: missing 4, repeated 1
    std::vector<std::vector<int>> grid6 = {{1, 2}, {3, 1}};
    assert(findRepeatedAndMissing(grid6) == std::vector<int>({1, 4}));
    
    return 0;
}
