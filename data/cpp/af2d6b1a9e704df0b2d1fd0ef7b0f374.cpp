// Write a C++ function `intervalIntersection` that takes two vectors of closed intervals, `A` and `B`, where each interval is represented as a vector of two integers `[start, end]` (with `start <= end`), and returns a vector of all intervals that are the intersection of any interval from `A` and any interval from `B`. The input vectors are each assumed to be sorted by interval start time and contain no overlapping intervals within themselves. The output should be sorted by start time. If no intersections exist, return an empty vector. The function must be `const`-correct, taking the inputs by `const` reference and returning the result by value. Handle all edge cases including empty inputs, intervals that just touch (e.g., `[1,2]` and `[2,3]` intersect at `[2,2]`), and intervals where one completely contains the other.

// The optimal algorithm uses a two-pointer technique since both interval lists are sorted. Maintain indices `i` and `j` for lists `A` and `B`. At each step, compute the intersection of `A[i]` and `B[j]` as the range `[max(A[i][0], B[j][0]), min(A[i][1], B[j][1])]`. If this lower bound is less than or equal to the upper bound, push it to the result. Then advance the pointer whose interval ends earlier, because that interval cannot intersect any later interval from the other list (since both lists are sorted and non-overlapping). If one interval ends exactly when the other starts, the intersection is a single point, which is still valid. Continue while both indices are within bounds. Time complexity is O(|A| + |B|) because each pointer moves at most the length of its own list. Space complexity is O(1) auxiliary, not counting the output vector.

#include <vector>
#include <algorithm>

// Return the intersection of two sorted lists of closed intervals.
std::vector<std::vector<int>> intervalIntersection(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {
    std::vector<std::vector<int>> result;
    size_t i = 0, j = 0;
    while (i < A.size() && j < B.size()) {
        int low = std::max(A[i][0], B[j][0]);
        int high = std::min(A[i][1], B[j][1]);
        if (low <= high) {
            result.push_back({low, high});
        }
        // Advance the interval that ends first.
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

// Free function from solution (declared here for compile test).
std::vector<std::vector<int>> intervalIntersection(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B);

int main() {
    // Basic overlap
    {
        std::vector<std::vector<int>> A = {{0,2},{5,10},{13,23},{24,25}};
        std::vector<std::vector<int>> B = {{1,5},{8,12},{15,24},{25,26}};
        std::vector<std::vector<int>> expected = {{1,2},{5,5},{8,10},{15,23},{24,24},{25,25}};
        assert(intervalIntersection(A, B) == expected);
    }
    // One list empty
    {
        std::vector<std::vector<int>> A = {{1,3}};
        std::vector<std::vector<int>> B = {};
        assert(intervalIntersection(A, B).empty());
    }
    // Both empty
    {
        std::vector<std::vector<int>> A = {};
        std::vector<std::vector<int>> B = {};
        assert(intervalIntersection(A, B).empty());
    }
    // Touching intervals (end == start)
    {
        std::vector<std::vector<int>> A = {{0,2}};
        std::vector<std::vector<int>> B = {{2,3}};
        std::vector<std::vector<int>> expected = {{2,2}};
        assert(intervalIntersection(A, B) == expected);
    }
    // One interval fully contains another
    {
        std::vector<std::vector<int>> A = {{0,10}};
        std::vector<std::vector<int>> B = {{3,7}};
        std::vector<std::vector<int>> expected = {{3,7}};
        assert(intervalIntersection(A, B) == expected);
    }
    // No overlap
    {
        std::vector<std::vector<int>> A = {{0,1},{5,6}};
        std::vector<std::vector<int>> B = {{2,3},{7,8}};
        assert(intervalIntersection(A, B).empty());
    }
    // Multiple equal intervals
    {
        std::vector<std::vector<int>> A = {{1,1},{1,1}};
        std::vector<std::vector<int>> B = {{1,1}};
        std::vector<std::vector<int>> expected = {{1,1},{1,1}};
        assert(intervalIntersection(A, B) == expected);
    }
    // Larger random-like case
    {
        std::vector<std::vector<int>> A = {{0,4},{6,10},{12,14}};
        std::vector<std::vector<int>> B = {{3,5},{7,13}};
        std::vector<std::vector<int>> expected = {{3,4},{7,10},{12,13}};
        assert(intervalIntersection(A, B) == expected);
    }
    return 0;
}
