Write a C++ function named `stableSortByPollutionThenIndex` that takes a vector of structs, each containing an `id` (an integer identifier) and a `pollution` level (an integer), and returns a new vector sorted primarily by `pollution` in ascending order, and secondarily by `id` in ascending order. The sorting must be stable: if two elements have the same `pollution`, their relative order from the original input must be preserved. However, the input vector is not guaranteed to be in any particular order initially. The function should not modify the input vector; it should return a sorted copy. For example, given input `{{3, 5}, {1, 2}, {2, 5}}`, the result should be `{{1, 2}, {3, 5}, {2, 5}}` (note `3` comes before `2` because their pollution is equal and `3` appeared first in the original input). If pollution values are equal but ids differ, stability is the primary tie-breaking rule; if both pollution and original order are identical (impossible for distinct elements), no specific order is required.
The key is to first sort the input vector by `id` to establish a deterministic "initial order" that we then leverage with a stable sort by pollution. However, the problem statement emphasizes preserving the original relative order for equal pollution values. The provided code snippet uses `std::sort` by id, followed by `std::stable_sort` by pollution. That is the correct approach: first, sort a copy of the input by id to define a clear order (so that identical pollution values will appear in id order among the sorted-by-id sequence), then apply `std::stable_sort` by pollution to ensure that elements with equal pollution maintain the order they had after the id sort. This works because `std::stable_sort` preserves the relative order of equal elements from the input sequence it receives. The overall time complexity is O(n log n) for both sorts (the stable sort may use O(n log n) time but with extra memory allocation; `std::stable_sort` can use O(n) auxiliary space). The function takes a vector by const reference and returns a new vector, so space complexity is O(n) for the copy plus O(n) for the stable sort's auxiliary buffer (if used), totaling O(n). Edge cases: empty vector (return empty), single element (return same), duplicates with identical pollution and id (unlikely but still sorted correctly). Since ids are unique per the problem's nature, but not explicitly stated, we handle general cases.
#include <vector>
#include <algorithm>

struct PollutionRecord {
    int id;
    int pollution;
};

// Return a new vector sorted by pollution ascending, and for equal pollution,
// preserve the relative order of elements as they appear in the original input.
std::vector<PollutionRecord> stableSortByPollutionThenIndex(const std::vector<PollutionRecord>& input) {
    // Copy input to avoid modifying the original.
    std::vector<PollutionRecord> result = input;
    
    // First sort by id to establish a deterministic order.
    std::sort(result.begin(), result.end(),
              [](const PollutionRecord& a, const PollutionRecord& b) {
                  return a.id < b.id;
              });
    
    // Stable sort by pollution: preserves order from the id-sorted vector for equal pollution.
    std::stable_sort(result.begin(), result.end(),
                     [](const PollutionRecord& a, const PollutionRecord& b) {
                         return a.pollution < b.pollution;
                     });
    
    return result;
}
#include <cassert>
#include <vector>

// Assume PollutionRecord and the function are defined above.

int main() {
    // Test basic sorting and stability.
    std::vector<PollutionRecord> v1 = {{3, 5}, {1, 2}, {2, 5}};
    auto r1 = stableSortByPollutionThenIndex(v1);
    assert((r1 == std::vector<PollutionRecord>{{1, 2}, {3, 5}, {2, 5}}));
    
    // Test multiple equal pollution values from distribution of ids.
    std::vector<PollutionRecord> v2 = {{4, 10}, {2, 10}, {1, 10}, {3, 5}};
    auto r2 = stableSortByPollutionThenIndex(v2);
    assert((r2 == std::vector<PollutionRecord>{{3, 5}, {4, 10}, {2, 10}, {1, 10}}));
    
    // All equal pollution: must preserve original relative order.
    std::vector<PollutionRecord> v3 = {{5, 1}, {7, 1}, {6, 1}};
    auto r3 = stableSortByPollutionThenIndex(v3);
    assert((r3 == std::vector<PollutionRecord>{{5, 1}, {7, 1}, {6, 1}}));
    
    // Already sorted by pollution.
    std::vector<PollutionRecord> v4 = {{1, 0}, {2, 3}, {3, 8}};
    auto r4 = stableSortByPollutionThenIndex(v4);
    assert((r4 == std::vector<PollutionRecord>{{1, 0}, {2, 3}, {3, 8}}));
    
    // Empty input.
    std::vector<PollutionRecord> v5;
    auto r5 = stableSortByPollutionThenIndex(v5);
    assert(r5.empty());
    
    // Single element.
    std::vector<PollutionRecord> v6 = {{42, 99}};
    auto r6 = stableSortByPollutionThenIndex(v6);
    assert((r6 == std::vector<PollutionRecord>{{42, 99}}));
    
    // Negative pollution values.
    std::vector<PollutionRecord> v7 = {{2, -3}, {1, -1}, {3, -3}};
    auto r7 = stableSortByPollutionThenIndex(v7);
    assert((r7 == std::vector<PollutionRecord>{{2, -3}, {3, -3}, {1, -1}}));
    
    return 0;
}
