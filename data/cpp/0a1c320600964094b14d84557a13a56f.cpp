Write a C++ function that takes a vector of integers and returns the sum of squares of the largest elements after grouping them into groups of exactly 4, where in each group of 4, only the smallest element’s square is added to the sum. The grouping is performed greedily from the largest elements downward: after sorting the entire array in ascending order, you repeatedly take the 4 largest remaining elements, discard the three largest among them, and add the square of the smallest of those 4 to the total. Continue this process until fewer than 4 elements remain, at which point you stop (ignore any leftover elements). The input may contain duplicate values and the vector size may be less than 4, in which case the result is 0. Also handle the case where the vector is empty. The function must be named `sumSmallestOfGroupsOfFour` and accept a `const std::vector<int>&` parameter, returning a `long long` to accommodate large squares.

#include <cassert>
#include <vector>

// Function declaration (for test compilation)
long long sumSmallestOfGroupsOfFour(const std::vector<int>& nums);

int main() {
    // Empty vector
    assert(sumSmallestOfGroupsOfFour({}) == 0);
    // Less than 4 elements
    assert(sumSmallestOfGroupsOfFour({1, 2, 3}) == 0);
    // Exactly 4: sorted [1,2,3,4] -> group {1,2,3,4} -> smallest=1 -> 1^2=1
    assert(sumSmallestOfGroupsOfFour({4, 3, 2, 1}) == 1);
    // 5 elements: sorted [1,2,3,4,5] -> take largest 4 {2,3,4,5} -> smallest=2 -> 4; leftover 1 ignored
    assert(sumSmallestOfGroupsOfFour({1, 2, 3, 4, 5}) == 4);
    // 8 elements: sorted [1,2,3,4,5,6,7,8] -> groups: {5,6,7,8} smallest=5 ->25, {1,2,3,4} smallest=1 ->1, total=26
    assert(sumSmallestOfGroupsOfFour({8, 7, 6, 5, 4, 3, 2, 1}) == 26);
    // Duplicates: [0,0,0,0] -> 0
    assert(sumSmallestOfGroupsOfFour({0, 0, 0, 0}) == 0);
    // Negative numbers: [-3,-2,-1,0] -> smallest=-3 ->9
    assert(sumSmallestOfGroupsOfFour({-3, -2, -1, 0}) == 9);
    // Large values: [10000, 10000, 10000, 10000] -> 100000000
    assert(sumSmallestOfGroupsOfFour({10000, 10000, 10000, 10000}) == 100000000);
    // 9 elements: sorted [1..9] -> groups: {6,7,8,9} smallest=6 ->36, {2,3,4,5} smallest=2 ->4, total=40, leftover 1
    assert(sumSmallestOfGroupsOfFour({1, 2, 3, 4, 5, 6, 7, 8, 9}) == 40);
    // Mixed with negative and positive: [-5, -1, 0, 2, 10] -> sorted [-5,-1,0,2,10] -> group {−1,0,2,10} smallest=−1 ->1, leftover −5
    assert(sumSmallestOfGroupsOfFour({-5, -1, 0, 2, 10}) == 1);
    return 0;
}

#include <vector>
#include <algorithm>

// Given a vector of integers, sort ascending, then repeatedly take the 4 largest remaining elements,
// add the square of the smallest among those 4 to the sum, and continue until fewer than 4 remain.
// Returns the sum as a long long.
long long sumSmallestOfGroupsOfFour(const std::vector<int>& nums) {
    if (nums.size() < 4) {
        return 0;
    }

    std::vector<int> sorted = nums; // copy to allow sorting
    std::sort(sorted.begin(), sorted.end());

    long long total = 0;
    int n = static_cast<int>(sorted.size());

    // Start at the largest element (index n-1) and step backward by 4.
    for (int i = n - 1; i >= 3; i -= 4) {
        if (i - 3 < 0) break; // not enough elements left
        int smallestInGroup = sorted[i - 3];
        total += static_cast<long long>(smallestInGroup) * smallestInGroup;
    }

    return total;
}

// The solution sorts the array in ascending order. Then, starting from the largest element (index `n-1`), we take groups of 4 by moving backward in steps of 4 (i.e., indices `n-1`, `n-5`, `n-9`, ...). For each such group, the smallest element is the one with the highest index in that group when considering ascending order — specifically, for a group starting at index `i` (the largest), the smallest is at index `i-3` (since the group consists of indices `i-3, i-2, i-1, i`). We add the square of that element to the sum. If the remaining elements count is fewer than 4 (i.e., `i-3 < 0`), we break and ignore leftovers. Edge cases: empty vector → 0; size less than 4 → 0; duplicates are fine because sorting handles them; large values may overflow int, so use `long long` for sum and cast the square to `long long`. Time complexity is O(n log n) due to sorting, and O(1) auxiliary space (excluding the input vector). The loop runs at most n/4 iterations.
