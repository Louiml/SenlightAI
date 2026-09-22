/*
Write a C++ function `int minimumSpanContainingAllTypes(const std::vector<int>& positions, const std::vector<int>& types)` that takes two parallel vectors representing the 1D coordinate (`positions[i]`) and type identifier (`types[i]`) of each of `n` items, and returns the minimum length of a contiguous interval on the number line that contains at least one item of every distinct type present in the input. The input may contain duplicate types, arbitrary non-negative coordinates (not necessarily sorted), and up to 50,000 items. If there is only one distinct type, the answer is `0` because the interval can be a single point. If the input vectors are empty, return `0`. You may assume both vectors have equal non-zero length unless both are empty. All coordinates are non-negative integers, and type identifiers are non-negative integers that may be sparse (e.g., 100, 1000, etc.). The function must handle unsorted input and duplicate types correctly. For example, given positions `{1, 2, 3, 4}` and types `{1, 2, 2, 3}`, the minimum interval covering types 1, 2, 3 is from position 1 to 3, length 2. Provide a standalone implementation without a `main` function; the test section will supply the `main` to validate.
*/

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstddef>

// Return the minimum length of a contiguous interval that contains at least one
// item of every distinct type present in the input.
// positions[i] and types[i] describe the coordinate and type of the i-th item.
// An empty input returns 0. A single distinct type returns 0.
int minimumSpanContainingAllTypes(const std::vector<int>& positions,
                                  const std::vector<int>& types) {
    const std::size_t n = positions.size();
    if (n == 0) {
        return 0;
    }

    // Compress type identifiers to 0..k-1
    std::vector<int> compressed_types(n);
    std::unordered_map<int, int> type_to_id;
    int distinct_count = 0;
    for (std::size_t i = 0; i < n; ++i) {
        auto it = type_to_id.find(types[i]);
        if (it == type_to_id.end()) {
            type_to_id[types[i]] = distinct_count;
            compressed_types[i] = distinct_count;
            ++distinct_count;
        } else {
            compressed_types[i] = it->second;
        }
    }

    // If only one distinct type, any single point works.
    if (distinct_count == 1) {
        return 0;
    }

    // Create vector of (position, compressed_type) pairs and sort by position.
    std::vector<std::pair<int, int>> items;
    items.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        items.emplace_back(positions[i], compressed_types[i]);
    }
    std::sort(items.begin(), items.end());

    // Sliding window over sorted items.
    std::vector<int> freq(distinct_count, 0);
    int covered_types = 0;
    int left = 0;
    int answer = -1; // will become positive after first valid window

    for (int right = 0; right < static_cast<int>(n); ++right) {
        int type = items[right].second;
        if (freq[type] == 0) {
            ++covered_types;
        }
        ++freq[type];

        // Shrink left while window still contains all distinct types.
        while (covered_types == distinct_count) {
            int current_len = items[right].first - items[left].first;
            if (answer == -1 || current_len < answer) {
                answer = current_len;
            }
            // Remove left element
            int left_type = items[left].second;
            --freq[left_type];
            if (freq[left_type] == 0) {
                --covered_types;
            }
            ++left;
        }
    }

    return answer;
}

#include <cassert>
#include <vector>

// The solution function is declared above; this is the test harness.
int main() {
    // Basic example from the description
    assert(minimumSpanContainingAllTypes({1, 2, 3, 4}, {1, 2, 2, 3}) == 2);

    // Single distinct type
    assert(minimumSpanContainingAllTypes({5, 7, 9}, {42, 42, 42}) == 0);

    // Empty input
    assert(minimumSpanContainingAllTypes({}, {}) == 0);

    // Unsorted input with duplicate coordinates
    // positions: 10 (type A), 0 (type B), 10 (type C) -> need A and C at pos 10, B at 0, interval [0,10] length 10
    assert(minimumSpanContainingAllTypes({10, 0, 10}, {1, 2, 3}) == 10);

    // Two items with same position but different types -> span 0
    assert(minimumSpanContainingAllTypes({4, 4}, {1, 2}) == 0);

    // Sparse types with gaps
    // positions: 0 (type 100), 5 (type 200), 9 (type 300) -> need all, interval [0,9] length 9
    assert(minimumSpanContainingAllTypes({0, 5, 9}, {100, 200, 300}) == 9);

    // Duplicate types and overlapping positions
    // positions: 1( A), 2(B), 3(A), 4(C) -> need A,B,C, best is [1,4] length 3? 
    // Actually A at 1 and 3, B at 2, C at 4 -> [1,4] length 3, but [2,4] lacks A at ? 
    // [1,4] covers all, length 3; also [2,4] has A at 3, B at 2, C at 4, length 2? 
    // positions sorted: (1,A),(2,B),(3,A),(4,C) -> window [2,4] covers B,A,C length 2. 
    // Wait positions of A at 1 and 3, B at 2, C at 4. Window [2,4] includes (2,B),(3,A),(4,C) -> yes length 2.
    assert(minimumSpanContainingAllTypes({1, 2, 3, 4}, {1, 2, 1, 3}) == 2);

    // All positions same
    assert(minimumSpanContainingAllTypes({7, 7, 7}, {1, 2, 3}) == 0);

    // Many items, expected result from a small random-like case
    // positions: 0(A), 1(B), 3(A), 5(C), 6(B) -> need A,B,C, best [1,5]? 
    // Sorted: (0,A),(1,B),(3,A),(5,C),(6,B). Valid windows: [0,5] len5, [1,5] len4, [3,6]? has A,B,C? (3,A),(5,C),(6,B) len3? Actually (3,A) yes, (5,C), (6,B) -> yes length 3. 
    assert(minimumSpanContainingAllTypes({0, 1, 3, 5, 6}, {1, 2, 1, 3, 2}) == 3);

    // Two distinct types far apart
    assert(minimumSpanContainingAllTypes({0, 100}, {1, 2}) == 100);

    // More than one item per type, minimal span includes a middle point
    // positions: 0(A), 10(B), 20(A) -> need A and B, best [0,10] length 10 or [10,20] length 10
    assert(minimumSpanContainingAllTypes({0, 10, 20}, {1, 2, 1}) == 10);
}

// The core idea is to sort all items by their coordinate and then use a sliding window (two-pointer) over the sorted list to track how many distinct types are covered by the current window. First, compress the type identifiers to a contiguous range `[0, k-1]` where `k` is the number of distinct types, to allow efficient counting via a frequency array. Sort pairs of `(position, compressed_type)`. Use two pointers `left` and `right` to maintain a window `[left, right]` that contains all `k` distinct types. Expand `right` until the window contains all types, then record the current window length as `positions[right] - positions[left]`, then shrink from the left by moving `left` forward and updating the frequency counts. Continue this process until the right pointer reaches the end and the window cannot cover all types. Edge cases: if there is only one distinct type, the answer is `0` because the window of one element has length `0`. If the input is empty, return `0`. Duplicate positions with different types must be handled correctly — note that the window length uses `positions[right] - positions[left]` and if multiple items share the same coordinate, the window still works because the positions are sorted and duplicates are adjacent. Time complexity is O(n log n) due to sorting and O(n) for the two-pointer scan, giving O(n log n) overall. Space complexity is O(n) for storing the sorted pairs and O(k) for the frequency array, where `k <= n`.
