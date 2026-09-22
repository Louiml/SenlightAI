// Write a C++ function that takes two vectors of integers, `a` and `b`, and for each element in `a`, returns the count of elements in `b` that are less than or equal to that element. The result should be a vector of integers of the same size as `a`. The input vectors may contain duplicates, negative numbers, and unsorted elements. The function must not modify the input vector `a` (it may sort a copy of `b`), and should handle empty input vectors gracefully (returning an empty result if `a` is empty, and all zeros if `b` is empty). The solution must be efficient for large vectors.
#include <cassert>
#include <vector>

// The solution function is defined elsewhere; include it here for the test.

int main() {
    // Basic test
    std::vector<int> a1 = {3, 1, 5};
    std::vector<int> b1 = {1, 2, 3, 4, 5};
    assert(countLessOrEqual(a1, b1) == std::vector<int>({3, 1, 5}));

    // Duplicate elements in b
    std::vector<int> a2 = {2, 4};
    std::vector<int> b2 = {4, 4, 2, 2};
    // b sorted: [2,2,4,4] -> for 2: count = 2, for 4: count = 4
    assert(countLessOrEqual(a2, b2) == std::vector<int>({2, 4}));

    // Negative numbers
    std::vector<int> a3 = {-3, 0, 5};
    std::vector<int> b3 = {-5, -1, 2};
    assert(countLessOrEqual(a3, b3) == std::vector<int>({1, 2, 3}));

    // Empty a
    std::vector<int> a4 = {};
    std::vector<int> b4 = {1, 2};
    assert(countLessOrEqual(a4, b4).empty());

    // Empty b
    std::vector<int> a5 = {1, 2, 3};
    std::vector<int> b5 = {};
    assert(countLessOrEqual(a5, b5) == std::vector<int>({0, 0, 0}));

    // Single element in both
    std::vector<int> a6 = {7};
    std::vector<int> b6 = {3};
    assert(countLessOrEqual(a6, b6) == std::vector<int>({0}));

    // All b greater than all a
    std::vector<int> a7 = {1, 2};
    std::vector<int> b7 = {3, 4, 5};
    assert(countLessOrEqual(a7, b7) == std::vector<int>({0, 0}));

    // All b less than all a
    std::vector<int> a8 = {10, 20};
    std::vector<int> b8 = {1, 2};
    assert(countLessOrEqual(a8, b8) == std::vector<int>({2, 2}));

    // Ensure original b is not modified
    std::vector<int> a9 = {3};
    std::vector<int> b9 = {2, 1, 3};
    std::vector<int> b_original = b9;
    (void)countLessOrEqual(a9, b9);
    assert(b9 == b_original);
}
#include <vector>
#include <algorithm>

// For each element in 'a', return the number of elements in 'b' that are <= it.
std::vector<int> countLessOrEqual(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    result.reserve(a.size());

    if (b.empty()) {
        // No elements in b, so every count is 0.
        result.assign(a.size(), 0);
        return result;
    }

    // Sort a copy of b to enable binary search; leaving original b unchanged.
    std::vector<int> sortedB(b);
    std::sort(sortedB.begin(), sortedB.end());

    for (int value : a) {
        // Binary search for first position where sortedB[pos] > value.
        int low = 0;
        int high = static_cast<int>(sortedB.size()); // exclusive upper bound
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (sortedB[mid] > value) {
                high = mid;  // move left side
            } else {
                low = mid + 1; // move right side
            }
        }
        // 'low' is the count of elements <= value.
        result.push_back(low);
    }
    return result;
}
// The key idea is to sort vector `b` first so that for each element in `a`, we can perform a binary search to find the first position where `b` exceeds that element. The index of the first greater element equals the number of elements ≤ the query. Specifically, after sorting `b`, for a value `x`, find the lower bound (first index `i` such that `b[i] > x`). That index `i` is exactly the count of elements in `b` that are ≤ `x`. We use a standard binary search where we maintain an answer variable initialized to `b.size()` (in case all elements are ≤ `x`). Edge cases: (1) If `b` is empty, every query should return 0; we can handle this by checking `b.empty()` early. (2) If `a` is empty, return an empty result. (3) Duplicate values in `b` work naturally since the binary search finds the first strictly greater element. (4) Natural overflow is not a concern since indices fit in `int` for typical problem sizes, but we could use `size_t` for safety; however, the result size matches `a`'s size and `b`'s size, both fit in `int` for typical constraints. Time complexity: sorting `b` takes O(m log m) where m = `b.size()`, and each binary search takes O(log m), so total O((m + n) log m) for n = `a.size()`. Space complexity: O(n) for the result vector (excluding the copy of `b` if we sort in place, but since we must not modify `b`, we take a copy, so O(m) extra space for the copy, plus O(n) for result).
