// You are given a sorted list of points on a number line (which may contain duplicates) and a set of queries. For each query, you are given two integers A and B, and you must count how many points lie in the inclusive interval [A, B]. Write a C++ function `countPointsInRange(const std::vector<int>& points, int A, int B)` that returns the number of points in that range. The input is guaranteed to be sorted in non‑decreasing order. Handle edge cases where A > B (return 0), where the range is entirely outside the points, and where points contain duplicates (each duplicate counts separately). Do not modify the input vector. The function should work for large inputs efficiently.

// The solution relies on binary search on the sorted array. The lower bound for A gives the first index >= A, and the upper bound for B gives the first index > B. The difference between these two iterators (or indices) gives the count of elements in [A, B]. This works because the array is sorted, and the standard library implementations of `lower_bound` and `upper_bound` run in O(log n) time. Edge cases: if A > B, the interval is empty, so return 0 naturally because lower_bound(A) would be > upper_bound(B). If all points are less than A, lower_bound returns end, and upper_bound returns end, difference is 0. If all points are greater than B, lower_bound returns begin, upper_bound returns begin, difference is 0. Duplicates are handled correctly because both bounds work on values, not unique elements. Time complexity per query is O(log n), space complexity O(1) auxiliary (excluding the input vector). For q queries, total O(q log n).

#include <vector>
#include <algorithm>

// Counts how many points in the sorted vector fall within [A, B] inclusive.
// The vector must be sorted in non-decreasing order.
int countPointsInRange(const std::vector<int>& points, int A, int B) {
    // If A > B, the interval is empty, return 0 immediately.
    if (A > B) {
        return 0;
    }
    // lower_bound gives first iterator with value >= A.
    auto left = std::lower_bound(points.begin(), points.end(), A);
    // upper_bound gives first iterator with value > B.
    auto right = std::upper_bound(points.begin(), points.end(), B);
    // Distance between them is the number of elements in [A, B].
    return static_cast<int>(right - left);
}

#include <cassert>
#include <vector>

// The function declaration is provided by including the solution code above.
int main() {
    // Basic case.
    std::vector<int> p1 = {1, 2, 3, 4, 5};
    assert(countPointsInRange(p1, 2, 4) == 3);
    // Range covers all.
    assert(countPointsInRange(p1, 1, 5) == 5);
    // Range outside left.
    assert(countPointsInRange(p1, -10, 0) == 0);
    // Range outside right.
    assert(countPointsInRange(p1, 6, 10) == 0);
    // Empty interval.
    assert(countPointsInRange(p1, 4, 2) == 0);
    // Single point exactly at boundary.
    assert(countPointsInRange(p1, 3, 3) == 1);
    
    // Duplicates.
    std::vector<int> p2 = {1, 1, 2, 2, 2, 3, 4, 4};
    assert(countPointsInRange(p2, 2, 3) == 4);
    assert(countPointsInRange(p2, 1, 1) == 2);
    assert(countPointsInRange(p2, 4, 4) == 2);
    
    // All same values.
    std::vector<int> p3 = {7, 7, 7};
    assert(countPointsInRange(p3, 7, 7) == 3);
    assert(countPointsInRange(p3, 6, 8) == 3);
    
    // Empty vector.
    std::vector<int> p4;
    assert(countPointsInRange(p4, 0, 10) == 0);
    
    // Negative values.
    std::vector<int> p5 = {-5, -3, -1, 0, 2};
    assert(countPointsInRange(p5, -3, -1) == 2);
    assert(countPointsInRange(p5, -4, 0) == 2);
    
    return 0;
}
