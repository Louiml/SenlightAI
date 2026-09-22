/*
Write a C++ function named `intersectionOfIntervals` that takes two vectors of closed intervals, `A` and `B`, where each interval is represented as a `vector<int>` of exactly two integers `[start, end]` with `start <= end`, and returns a vector of intervals representing the intersection of every pair of intervals from `A` and `B` that overlap. The input lists are each sorted by start time and contain no overlapping intervals within the same list (i.e., each list is non-overlapping internally). The output must be sorted by start time and contain only intervals where the intersection is non-empty. If either input list is empty, return an empty result. You may assume all interval endpoints are non-negative integers.
*/

#include <vector>
#include <algorithm>

// Compute the intersection of two sorted, disjoint interval lists.
// Each interval is a vector<int> of size 2: [start, end] with start <= end.
std::vector<std::vector<int>> intersectionOfIntervals(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {
    
    std::vector<std::vector<int>> result;
    if (A.empty() || B.empty()) {
        return result;
    }

    size_t i = 0, j = 0;
    const size_t n = A.size(), m = B.size();

    while (i < n && j < m) {
        // Compute the intersection of the current intervals.
        int left = std::max(A[i][0], B[j][0]);
        int right = std::min(A[i][1], B[j][1]);

        // If the intersection is non-empty, add it.
        if (left <= right) {
            result.push_back({left, right});
        }

        // Move the pointer whose interval ends earlier.
        if (A[i][1] < B[j][1]) {
            ++i;
        } else {
            ++j;
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is included above.

int main() {
    using std::vector;

    // Example from typical LeetCode problem
    vector<vector<int>> A1 = {{0,2},{5,10},{13,23},{24,25}};
    vector<vector<int>> B1 = {{1,5},{8,12},{15,24},{25,26}};
    vector<vector<int>> expected1 = {{1,2},{5,5},{8,10},{15,23},{24,24},{25,25}};
    assert(intersectionOfIntervals(A1, B1) == expected1);

    // Both empty
    assert(intersectionOfIntervals({}, {}).empty());

    // One empty
    vector<vector<int>> A2 = {{1,3}};
    assert(intersectionOfIntervals(A2, {}).empty());
    assert(intersectionOfIntervals({}, A2).empty());

    // One-to-one overlap
    vector<vector<int>> A3 = {{1,3}};
    vector<vector<int>> B3 = {{2,4}};
    vector<vector<int>> expected3 = {{2,3}};
    assert(intersectionOfIntervals(A3, B3) == expected3);

    // Exact same intervals
    vector<vector<int>> A4 = {{1,2}};
    vector<vector<int>> B4 = {{1,2}};
    vector<vector<int>> expected4 = {{1,2}};
    assert(intersectionOfIntervals(A4, B4) == expected4);

    // No intersection
    vector<vector<int>> A5 = {{1,2}};
    vector<vector<int>> B5 = {{3,4}};
    assert(intersectionOfIntervals(A5, B5).empty());

    // Multi-interval overlapping multiple times
    vector<vector<int>> A6 = {{0,10},{15,20}};
    vector<vector<int>> B6 = {{5,16},{18,25}};
    vector<vector<int>> expected6 = {{5,10},{15,16},{18,20}};
    assert(intersectionOfIntervals(A6, B6) == expected6);

    // Single point intersections
    vector<vector<int>> A7 = {{1,1},{3,3}};
    vector<vector<int>> B7 = {{1,2},{3,4}};
    vector<vector<int>> expected7 = {{1,1},{3,3}};
    assert(intersectionOfIntervals(A7, B7) == expected7);

    // Duplicate end boundaries
    vector<vector<int>> A8 = {{1,5}};
    vector<vector<int>> B8 = {{5,6}};
    vector<vector<int>> expected8 = {{5,5}};
    assert(intersectionOfIntervals(A8, B8) == expected8);

    return 0;
}

// The classic two-pointer technique is used because both lists are sorted by their start and are internally non-overlapping. Initialize two indices `i` and `j` to the first intervals of `A` and `B`. At each step, compute the candidate intersection by taking the maximum of the start points and the minimum of the end points: `left = max(A[i][0], B[j][0])`, `right = min(A[i][1], B[j][1])`. If `left <= right`, the intersection is valid and push `{left, right}` to the result. Then advance the pointer that has the smaller end point: if `A[i][1] < B[j][1]`, increment `i`; else increment `j`. This works because the interval that ends earlier cannot overlap with any later interval from the other list beyond the current candidate. Continue until one list is exhausted. Edge cases: when either input is empty, immediately return an empty vector. Also note that the original snippet incorrectly returned the input list when one is empty; our task must return an empty vector. The algorithm runs in `O(n + m)` time, where `n` and `m` are the sizes of the input lists, and uses `O(1)` extra space besides the output vector.
