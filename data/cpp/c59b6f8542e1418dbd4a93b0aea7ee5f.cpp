// Write a C++ function named `findMissingFromRange` that takes a non-empty vector of integers `nums` of length `n`, where each element is in the inclusive range `[1, n]`, and returns a vector containing all integers from `1` to `n` that do **not** appear in `nums`. The input vector may contain duplicates, and the result should be sorted in ascending order. The function must modify the input vector in place (if needed) but must restore it to its original values before returning, ensuring the caller's data remains unchanged. You may not use any additional data structures beyond a few local variables and the output vector.

The key insight is to use the input array itself as a marker system, since all values are within `[1, n]`. For each element, treat its absolute value as a 1-based index and mark the element at that index (0-based `index = abs(nums[i]) - 1`) as negative, but only if it is currently positive. After processing all elements, any position `i` that remains positive means the number `i+1` was never seen. This works because we only flip signs, and values are restored by taking absolute values when reading—so duplicate values are handled naturally (negative values are ignored in the comparisons). However, to keep the input vector unchanged, we either copy it first (saving O(n) extra space) or, better, note that the algorithm must restore the signs after detection. The cleaner approach: copy the input into a local vector, apply the sign-marking on the copy, then build the result from the marks. Since we cannot use extra data structures beyond the output, a copy is acceptable as auxiliary space (O(n)). Alternatively, we could mark then unmark, but the simplest correct method is to work on a copy. Time complexity is O(n) (two passes), space complexity is O(n) for the copy plus O(k) for the output (k = number of missing numbers). Edge cases: n=1, duplicates everywhere, and the case where all numbers appear (output empty).

#include <vector>
#include <cstdlib>

// Returns all numbers in [1, n] that are missing from nums.
// The caller's vector is not modified.
std::vector<int> findMissingFromRange(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    std::vector<int> marks = nums; // work on a copy to preserve input
    std::vector<int> result;

    // Mark each seen number by negating the element at its corresponding index.
    for (int i = 0; i < n; ++i) {
        int val = std::abs(marks[i]);
        if (val >= 1 && val <= n) {
            int idx = val - 1;
            if (marks[idx] > 0) {
                marks[idx] = -marks[idx];
            }
        }
    }

    // Any position still positive means that number was missing.
    for (int i = 0; i < n; ++i) {
        if (marks[i] > 0) {
            result.push_back(i + 1);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// function declaration from solution
std::vector<int> findMissingFromRange(const std::vector<int>& nums);

int main() {
    std::vector<int> v1 = {4,3,2,7,8,2,3,1};
    std::vector<int> expected1 = {5,6};
    assert(findMissingFromRange(v1) == expected1);

    std::vector<int> v2 = {1,1};
    std::vector<int> expected2 = {2};
    assert(findMissingFromRange(v2) == expected2);

    std::vector<int> v3 = {1,2,3,4,5};
    std::vector<int> expected3 = {};
    assert(findMissingFromRange(v3) == expected3);

    std::vector<int> v4 = {2,2,2,2,2};
    std::vector<int> expected4 = {1,3,4,5};
    assert(findMissingFromRange(v4) == expected4);

    std::vector<int> v5 = {1};
    std::vector<int> expected5 = {};
    assert(findMissingFromRange(v5) == expected5);

    std::vector<int> v6 = {1,2,2,4,4,6};
    std::vector<int> expected6 = {3,5};
    assert(findMissingFromRange(v6) == expected6);

    // Ensure input is not modified
    std::vector<int> original = {3,1,3,4};
    std::vector<int> copy = original;
    findMissingFromRange(original);
    assert(original == copy);

    return 0;
}
