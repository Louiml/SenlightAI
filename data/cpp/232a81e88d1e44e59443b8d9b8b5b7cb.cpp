/*
Write a C++ function named `countNonDipPositions` that takes a vector of integers representing a sequence of mountain heights and returns the number of positions that are not a "dip". A position is considered a dip if it is strictly lower than at least one of its immediate neighbors (left or right). Positions at the boundaries are only compared to their single existing neighbor. The input vector may contain 1 to 1000 elements, and heights can be any integer values (including negatives). The function should count and return how many positions are NOT dips, i.e., positions where the height is greater than or equal to all existing immediate neighbors. The function must be const-correct and handle edge cases such as a single-element vector, all-equal heights, and strictly increasing or decreasing sequences.
*/

#include <vector>

// Count positions that are not strictly lower than any immediate neighbor.
// A position is a dip if it is lower than at least one neighbor.
// Returns the number of non-dip positions.
int countNonDipPositions(const std::vector<int>& heights) {
    int count = 0;
    const int n = static_cast<int>(heights.size());
    
    for (int i = 0; i < n; ++i) {
        bool isDip = false;
        if (i > 0 && heights[i] < heights[i - 1]) {
            isDip = true;
        }
        if (i < n - 1 && heights[i] < heights[i + 1]) {
            isDip = true;
        }
        if (!isDip) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Assume countNonDipPositions is defined above.

int main() {
    std::vector<int> v1 = {5};
    assert(countNonDipPositions(v1) == 1);

    std::vector<int> v2 = {1, 2, 3};
    assert(countNonDipPositions(v2) == 1); // only position 3 (index 2) is non-dip

    std::vector<int> v3 = {3, 2, 1};
    assert(countNonDipPositions(v3) == 1); // only position 3 (index 2) is non-dip

    std::vector<int> v4 = {1, 2, 1};
    assert(countNonDipPositions(v4) == 2); // positions 0 and 2

    std::vector<int> v5 = {5, 5, 5, 5};
    assert(countNonDipPositions(v5) == 4);

    std::vector<int> v6 = {10, 1, 1, 10};
    assert(countNonDipPositions(v6) == 2); // positions 0 and 3

    std::vector<int> v7 = {-3, -5, -4, -2};
    assert(countNonDipPositions(v7) == 1); // only position 3

    std::vector<int> v8 = {2, 2, 1};
    assert(countNonDipPositions(v8) == 2); // positions 0 and 1

    return 0;
}

// The solution iterates through each index of the vector once. For each position, we check if a "dip" condition exists: if `i > 0` and `heights[i] < heights[i-1]`, that means the current is lower than the left neighbor, so it's a dip. Similarly, if `i < n-1` and `heights[i] < heights[i+1]`, it's a dip. If neither condition is true, the position is not a dip, so we increment the counter. This directly mirrors the logic in the provided snippet but generalizes it to a free function. Edge cases: for `n=1`, both neighbor checks are skipped, and the single position is never a dip, so the count is 1. For a sequence like `[1,2,1]`, positions 0 and 2 are not dips (each has only one neighbor, and they are not lower than it), but position 1 is a dip because it is lower than both neighbors. All-equal heights like `[5,5,5]` yield no dips, so the count is 3. Time complexity is O(n) where n is the number of elements, and space complexity is O(1) since we only use a counter and loop variable.
