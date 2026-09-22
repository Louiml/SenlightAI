// Write a C++ function `int firstIndexOf(const std::vector<int>& values, int target)` that returns the index of the *first* occurrence of `target` in an unsorted vector (linear search). If `target` is not present, return `-1`. Additionally, write a second function `int binaryFirstIndexOf(const std::vector<int>& values, int target)` that returns the index of the *first* occurrence of `target` in a **sorted** vector using an iterative binary search that handles duplicate values correctly (i.e., it does not simply return any matching index, but specifically the leftmost one). The functions must be `const`‑correct (accept a `const` reference) and must not modify the input vector. The binary search must handle empty vectors and vectors of size 1 correctly, and must not use recursion (to avoid stack overflow on large inputs). Both functions must have clear time and space complexity documentation in comments.
// For the linear search, the approach is straightforward: iterate through the vector from index 0 to `size()-1`, and return the first index where `values[i] == target`. If the loop completes without a match, return `-1`. Time complexity is `O(n)` and space complexity is `O(1)`.
//
// For the binary search, the challenge is to find the leftmost occurrence in a sorted vector. A naive binary search may return any matching index, but we need the first one. The standard approach: use two indices `low = 0` and `high = values.size()` (exclusive upper bound). While `low < high`, compute `mid = low + (high - low) / 2` (this avoids integer overflow). If `values[mid] < target`, then the target cannot be in the left half, so set `low = mid + 1`. Otherwise (i.e., `values[mid] >= target`), we move `high = mid` because the leftmost occurrence is either at `mid` or to the left of `mid`. After the loop, `low` will be the first index where `values[low] == target` if the target exists; otherwise, `low` will be the insertion point (first index where `values[low] > target`). Finally, check if `low < values.size()` and `values[low] == target`; if so, return `low`, else return `-1`. This handles empty vectors (loop doesn't run, `low=0`, then `0 < 0` false, return `-1`). For size 1, it works correctly. Time complexity is `O(log n)` and space complexity `O(1)`. Edge cases: empty vector, target smaller than first element, target larger than last element, duplicates, single-element vector.
#include <vector>

/**
 * Linear search: returns the index of the first occurrence of target, or -1 if not found.
 * Time: O(n), Memory: O(1)
 */
int firstIndexOf(const std::vector<int>& values, int target) {
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * Binary search on a sorted vector: returns the index of the first occurrence of target, or -1 if not found.
 * Handles duplicates by finding the leftmost match. Assumes `values` is sorted in ascending order.
 * Time: O(log n), Memory: O(1)
 */
int binaryFirstIndexOf(const std::vector<int>& values, int target) {
    int low = 0;
    int high = static_cast<int>(values.size()); // exclusive upper bound

    while (low < high) {
        int mid = low + (high - low) / 2; // avoids overflow
        if (values[mid] < target) {
            low = mid + 1; // target is to the right
        } else {
            high = mid; // target is at mid or to the left
        }
    }

    // After loop, low is the first index where values[low] >= target.
    // Check if it's exactly the target and within bounds.
    if (low < static_cast<int>(values.size()) && values[low] == target) {
        return low;
    }
    return -1;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(firstIndexOf(v1, 3) == 2);
    assert(firstIndexOf(v1, 6) == -1);
    assert(firstIndexOf(v1, 1) == 0);

    std::vector<int> v2 = {5, 5, 5, 5};
    assert(firstIndexOf(v2, 5) == 0);
    assert(firstIndexOf(v2, 4) == -1);

    std::vector<int> v3 = {};
    assert(firstIndexOf(v3, 1) == -1);
    assert(binaryFirstIndexOf(v3, 1) == -1);

    std::vector<int> v4 = {5};
    assert(binaryFirstIndexOf(v4, 5) == 0);
    assert(binaryFirstIndexOf(v4, 4) == -1);

    std::vector<int> v5 = {1, 2, 2, 2, 3, 4};
    assert(binaryFirstIndexOf(v5, 2) == 1);
    assert(binaryFirstIndexOf(v5, 3) == 4);
    assert(binaryFirstIndexOf(v5, 0) == -1);
    assert(binaryFirstIndexOf(v5, 5) == -1);

    std::vector<int> v6 = {1, 1, 1, 2, 3, 3, 3};
    assert(binaryFirstIndexOf(v6, 1) == 0);
    assert(binaryFirstIndexOf(v6, 3) == 4);

    return 0;
}
