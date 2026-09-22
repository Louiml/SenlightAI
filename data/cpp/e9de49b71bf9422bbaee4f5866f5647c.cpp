// Write a standalone C++ function `bool shiftAndCheck(const std::vector<int>& original, const std::vector<int>& shifted)` that determines whether the second vector is a circular (cyclic) rotation of the first vector. The function must return `true` if `shifted` can be obtained by rotating `original` by any non-negative number of positions (including zero, meaning they are identical), and `false` otherwise. Both vectors are non-empty and have the same length. The function should handle duplicate elements correctly — for example, `{1, 2, 1, 2}` is a rotation of `{2, 1, 2, 1}` but also of itself, and both should return `true`. The solution must be self-contained, use only standard C++ headers, and be const-correct.

// The core observation is that a vector `A` is a rotation of vector `B` if and only if `A` appears as a contiguous subarray within the concatenation `B + B` (with a length restriction). Concretely, for vectors of size `n`, if we concatenate `original` with itself to form a doubled vector, then `shifted` must appear as a subarray of length `n` starting at some index `0 <= start < n`. This works because every rotation of `original` appears exactly as a length-`n` window in that doubled sequence, and conversely any length-`n` window in the doubled sequence is a valid rotation. Duplicate elements are handled naturally since the comparison is element-wise over the full window, not a set or multiset comparison. Edge cases: vectors of size 1 (any single-element vector is trivially a rotation of itself) and equal vectors (start index 0). If the sizes differ, return `false` immediately. Time complexity is O(n^2) in the worst case if implemented naively (checking all n starting positions, each requiring O(n) comparison), which is acceptable for this simple task. Space complexity is O(n) for the doubled vector if constructed explicitly; alternatively, one can avoid extra space by using modular indexing, but constructing the doubled vector is clearer and still O(n) auxiliary space. This approach is robust and avoids pitfalls such as sorting or hashing, which would not preserve rotation order.

#include <vector>

// Determines if `shifted` is a circular rotation of `original`.
// Both vectors must be non-empty and of equal length for a meaningful check.
bool shiftAndCheck(const std::vector<int>& original, const std::vector<int>& shifted) {
    const size_t n = original.size();
    if (n == 0 || shifted.size() != n) {
        return false;
    }

    // Concatenate original with itself to obtain all possible rotations as subarrays.
    std::vector<int> doubled;
    doubled.reserve(2 * n);
    doubled.insert(doubled.end(), original.begin(), original.end());
    doubled.insert(doubled.end(), original.begin(), original.end());

    // Check each possible starting point for a length-n match.
    for (size_t start = 0; start < n; ++start) {
        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (doubled[start + i] != shifted[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// Declare the function being tested (assumes it is defined above or in a header).
bool shiftAndCheck(const std::vector<int>& original, const std::vector<int>& shifted);

int main() {
    // Identical vectors (rotation by 0 positions).
    assert(shiftAndCheck({1, 2, 3}, {1, 2, 3}));
    // Simple one-step left rotation.
    assert(shiftAndCheck({1, 2, 3}, {2, 3, 1}));
    // Left rotation by 2 positions.
    assert(shiftAndCheck({1, 2, 3}, {3, 1, 2}));
    // Not a rotation (different order).
    assert(!shiftAndCheck({1, 2, 3}, {1, 3, 2}));
    // Single element.
    assert(shiftAndCheck({5}, {5}));
    // Duplicate elements: {2,1,2,1} is a rotation of {1,2,1,2}.
    assert(shiftAndCheck({1, 2, 1, 2}, {2, 1, 2, 1}));
    // Also, {1,2,1,2} is a rotation of itself.
    assert(shiftAndCheck({1, 2, 1, 2}, {1, 2, 1, 2}));
    // Negative numbers.
    assert(shiftAndCheck({-1, 0, 2}, {0, 2, -1}));
    // Different lengths => false.
    assert(!shiftAndCheck({1, 2}, {1, 2, 3}));
    // Rotation by n-1 positions (same as right shift by 1).
    assert(shiftAndCheck({1, 5, 9, 4}, {4, 1, 5, 9}));
    // Empty vectors are invalid per spec, but function handles gracefully.
    assert(!shiftAndCheck({}, {}));
    return 0;
}
