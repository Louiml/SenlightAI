// Write a C++ function named `running_median` that takes a single `std::vector<int>` argument representing a stream of integers in order of arrival, and returns a `std::vector<double>` where the i-th element is the median of the first (i+1) integers in the input vector. The median must be computed after each insertion, maintaining the same behavior as a classic online median finder: for an even count of elements, return the average of the two middle values; for an odd count, return the exact middle value. Your function must handle empty input (return an empty vector), duplicate values, negative numbers, and large counts efficiently. Do not modify the input vector.

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Basic sequence with odd and even counts.
    std::vector<int> stream1 = {5, 15, 1, 3};
    std::vector<double> expected1 = {5.0, 10.0, 5.0, 4.0};
    assert(running_median(stream1) == expected1);

    // Empty input.
    std::vector<int> stream2 = {};
    assert(running_median(stream2).empty());

    // Single element.
    std::vector<int> stream3 = {42};
    assert(running_median(stream3) == std::vector<double>{42.0});

    // Duplicates and negatives.
    std::vector<int> stream4 = {-5, -5, -5};
    assert(running_median(stream4) == (std::vector<double>{-5.0, -5.0, -5.0}));

    // Mixed large and small values.
    std::vector<int> stream5 = {10, -2, 8, 0, 7};
    std::vector<double> expected5 = {10.0, 4.0, 8.0, 4.0, 7.0};
    assert(running_median(stream5) == expected5);

    // Strictly increasing sequence.
    std::vector<int> stream6 = {1, 2, 3};
    std::vector<double> expected6 = {1.0, 1.5, 2.0};
    assert(running_median(stream6) == expected6);

    // Strictly decreasing sequence.
    std::vector<int> stream7 = {3, 2, 1};
    std::vector<double> expected7 = {3.0, 2.5, 2.0};
    assert(running_median(stream7) == expected7);

    // Large even count.
    std::vector<int> stream8 = {100, 1, 50, 25};
    // Medians: 100, (100+1)/2=50.5, 50, (50+25)/2=37.5
    std::vector<double> expected8 = {100.0, 50.5, 50.0, 37.5};
    assert(running_median(stream8) == expected8);

    // Values that cause even split after 2 and 4 elements.
    std::vector<int> stream9 = {2, 4, 6, 8, 10};
    // 2, 3, 4, 5, 6
    std::vector<double> expected9 = {2.0, 3.0, 4.0, 5.0, 6.0};
    assert(running_median(stream9) == expected9);

    // All identical large values.
    std::vector<int> stream10 = {7, 7, 7, 7};
    std::vector<double> expected10 = {7.0, 7.0, 7.0, 7.0};
    assert(running_median(stream10) == expected10);

    return 0;
}

#include <vector>
#include <queue>
#include <functional>

// Returns a vector where the i-th element is the median of the first (i+1) integers in input.
std::vector<double> running_median(const std::vector<int>& input) {
    std::vector<double> result;
    result.reserve(input.size());

    // Max-heap for the lower half (default priority_queue is max-heap).
    std::priority_queue<int> left;
    // Min-heap for the upper half.
    std::priority_queue<int, std::vector<int>, std::greater<int>> right;

    for (int num : input) {
        // Insert into appropriate heap.
        if (left.empty() || num <= left.top()) {
            left.push(num);
        } else {
            right.push(num);
        }

        // Rebalance heaps to maintain |left| == |right| or |left| == |right| + 1.
        if (left.size() > right.size() + 1) {
            right.push(left.top());
            left.pop();
        } else if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }

        // Compute current median.
        if (left.size() == right.size()) {
            result.push_back((left.top() + right.top()) / 2.0);
        } else {
            result.push_back(static_cast<double>(left.top()));
        }
    }

    return result;
}

// The core idea is to maintain two heaps: a max-heap for the smaller half of the numbers seen so far and a min-heap for the larger half. The max-heap (`left`) stores the lower half, and the min-heap (`right`) stores the upper half, with the invariant that the size of `left` is either equal to the size of `right` or exactly one greater. After inserting each number, if it is less than or equal to the top of `left` (or if `left` is empty), push it into `left`; otherwise push into `right`. Then rebalance: if `left` has more than one extra element than `right`, move the top of `left` to `right`; if `right` becomes larger than `left`, move the top of `right` to `left`. To compute the median, if sizes are equal, average the tops of both heaps; otherwise the median is the top of `left`. This guarantees O(log n) per insertion and O(1) for median retrieval, with O(n) total time for n insertions and O(n) auxiliary space for the two heaps. Edge cases handled include empty input, a single element, and duplicate values where the comparison `num <= left.top()` correctly places equal values in either heap without breaking size invariants. Note the function must avoid modifying the input and preserve the original stream order.
