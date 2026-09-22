Write a C++ function named `inclusiveRange` that takes a `std::vector<int>` representing a sequence of numbers, and two integers `start` and `end`. The function must return a new `std::vector<int>` containing all elements from the original vector whose indices are in the closed interval `[start, end]` (inclusive on both ends). If `start` is less than 0, treat it as 0; if `end` is greater than or equal to the vector size, treat it as the last valid index. If the vector is empty or `start > end` after clamping, return an empty vector. The function must not modify the input vector and must preserve the relative order of elements.
// The solution involves first computing the effective bounds: `left = max(0, start)` and `right = min(end, (int)numbers.size() - 1)`. If the vector is empty (`numbers.empty()`) or after clamping `left > right`, return an empty vector. Otherwise, iterate from `left` to `right` inclusive, pushing each element into a result vector. This approach is straightforward and handles all edge cases: negative start values are clamped upward, end values beyond the array are clamped downward, and invalid ranges produce an empty result. The algorithm runs in O(k) time where k is the size of the requested subarray (at most n), and uses O(k) auxiliary space for the result. No extra data structures are needed beyond the result vector, and the input vector is read-only.
#include <vector>
#include <algorithm>

// Return a subvector of `numbers` with indices in [start, end] inclusive.
// Start and end are clamped to valid range; returns empty vector if range invalid.
std::vector<int> inclusiveRange(const std::vector<int>& numbers, int start, int end) {
    if (numbers.empty()) {
        return {};
    }
    int left = std::max(0, start);
    int right = std::min(end, static_cast<int>(numbers.size()) - 1);
    if (left > right) {
        return {};
    }
    std::vector<int> result;
    result.reserve(right - left + 1);
    for (int i = left; i <= right; ++i) {
        result.push_back(numbers[i]);
    }
    return result;
}
#include <cassert>
#include <vector>

std::vector<int> inclusiveRange(const std::vector<int>&, int, int); // declaration

int main() {
    std::vector<int> data = {10, 20, 30, 40, 50};
    assert(inclusiveRange(data, 1, 3) == std::vector<int>({20, 30, 40}));
    assert(inclusiveRange(data, -3, 2) == std::vector<int>({10, 20, 30}));
    assert(inclusiveRange(data, 2, 100) == std::vector<int>({30, 40, 50}));
    assert(inclusiveRange(data, 4, 4) == std::vector<int>({50}));
    assert(inclusiveRange(data, 3, 1) == std::vector<int>({}));
    std::vector<int> empty;
    assert(inclusiveRange(empty, 0, 0) == std::vector<int>({}));
    assert(inclusiveRange(data, 0, 0) == std::vector<int>({10}));
    assert(inclusiveRange(data, -5, -1) == std::vector<int>({}));
    assert(inclusiveRange(data, 2, 2) == std::vector<int>({30}));
    assert(inclusiveRange(data, 0, 4) == std::vector<int>({10, 20, 30, 40, 50}));
}
