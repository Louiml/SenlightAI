/*
Write a C++ function that takes two integer 2D vectors: `flowers`, where each element is a pair `[start, end]` representing the blooming period of a flower (inclusive of both endpoints), and `people`, a list of query times. For each time `t` in `people`, the function must return the number of flowers that are in full bloom at exactly time `t` (i.e., `start <= t <= end`). The function should return a vector of integers of the same size as `people`, with each entry being the bloom count for the corresponding query. You may assume the input vectors are non-empty, but individual periods may have `start == end`, and times may be any integers (including negative). The function must be efficient for large inputs (up to 10^5 flowers and 10^5 people).
*/
#include <vector>
#include <algorithm>

// Count flowers in bloom at each person's query time.
std::vector<int> bloomCountAtTimes(const std::vector<std::vector<int>>& flowers,
                                   const std::vector<int>& people) {
    std::vector<int> starts, ends;
    starts.reserve(flowers.size());
    ends.reserve(flowers.size());
    for (const auto& interval : flowers) {
        starts.push_back(interval[0]);
        ends.push_back(interval[1]);
    }
    std::sort(starts.begin(), starts.end());
    std::sort(ends.begin(), ends.end());

    std::vector<int> result;
    result.reserve(people.size());
    for (int t : people) {
        // Flowers that have started by time t (inclusive)
        int started = std::upper_bound(starts.begin(), starts.end(), t) - starts.begin();
        // Flowers that have ended before time t (exclusive, so end < t)
        int ended = std::lower_bound(ends.begin(), ends.end(), t) - ends.begin();
        result.push_back(started - ended);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<std::vector<int>> f1 = {{1, 3}, {2, 4}, {3, 5}};
    std::vector<int> p1 = {3};
    assert(bloomCountAtTimes(f1, p1) == std::vector<int>({3}));

    // Multiple people, no overlap
    std::vector<std::vector<int>> f2 = {{1, 2}, {3, 4}};
    std::vector<int> p2 = {1, 2, 3, 4};
    assert(bloomCountAtTimes(f2, p2) == std::vector<int>({1, 1, 1, 1}));

    // Point bloom (start == end)
    std::vector<std::vector<int>> f3 = {{5, 5}, {5, 5}, {1, 10}};
    std::vector<int> p3 = {5};
    assert(bloomCountAtTimes(f3, p3) == std::vector<int>({3}));

    // Negative and zero times
    std::vector<std::vector<int>> f4 = {{-5, -1}, {-2, 0}, {0, 0}};
    std::vector<int> p4 = {-3, -1, 0};
    std::vector<int> r4 = bloomCountAtTimes(f4, p4);
    assert(r4 == std::vector<int>({1, 1, 2})); // -3: only first, -1: first+second, 0: second+third

    // No flowers at a time
    std::vector<std::vector<int>> f5 = {{1, 2}, {4, 5}};
    std::vector<int> p5 = {3};
    assert(bloomCountAtTimes(f5, p5) == std::vector<int>({0}));

    // Large duplicate periods
    std::vector<std::vector<int>> f6 = {{10, 20}, {10, 20}, {10, 20}};
    std::vector<int> p6 = {10, 15, 20, 21};
    assert(bloomCountAtTimes(f6, p6) == std::vector<int>({3, 3, 3, 0}));

    // Empty people vector
    std::vector<std::vector<int>> f7 = {{1, 2}};
    std::vector<int> p7 = {};
    assert(bloomCountAtTimes(f7, p7) == std::vector<int>());

    return 0;
}
// The naive O(n*m) approach would iterate over all flowers for each person, which is too slow. The key observation is that the number of flowers blooming at time `t` equals the number of flowers that have started blooming by time `t` minus the number of flowers that have already ended before time `t` (since a flower is NOT blooming if its `end < t`). We can precompute two sorted arrays: one of all `start` times, and one of all `end` times. For each query time `t`, we count how many start times are <= `t` using `upper_bound` (because a flower with start == t is included), and how many end times are < `t` using `lower_bound` (because a flower with end == t is still blooming at t). The difference gives the answer. Edge cases: negative times, periods where start == end (correctly counted when t equals that value), duplicate times in flowers or people, and multiple flowers with the same start/end. Time complexity: O((F + P) log F) for sorting and each binary search, plus O(P) to build the result. Space complexity: O(F) for the two sorted arrays, plus O(P) for the result.
