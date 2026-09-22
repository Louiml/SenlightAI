Given an odd-length sequence of integers, write a C++ function `findMiddleValue` that takes a `std::vector<long long>` (where the size is guaranteed to be odd and at least 1) and returns the element located at the exact middle position (0-based index `n/2`, where `n` is the size). The task is inspired by a simple array-indexing problem: for any odd-length vector, the middle index is simply `size() / 2` due to integer division truncation. You do not need to sort or modify the vector. The function must work for positive, negative, and zero values, and must handle sizes like 1, 3, 5, etc. Ensure your solution uses appropriate `const` correctness and returns a copy of the value (not a reference), since the original vector remains unchanged.

// The solution is straightforward. Given a vector `v` with `n` elements where `n` is odd, the middle element is at index `n / 2` (integer division). For example, if `n=5`, indices are 0,1,2,3,4, and `5/2=2`, which is the third element. If `n=1`, index `0` is the only element. Because the problem guarantees `n ≥ 1` and odd, there is always exactly one middle element, so no special case for empty input is needed. We simply access `v[v.size() / 2]` and return it. No sorting or extra data structures are required. The time complexity is O(1) (constant time, since we only access one element), and the space complexity is O(1) auxiliary, not counting the input vector itself. Edge cases: ensure the vector is non-empty and odd; if not, the behavior is undefined by contract, but for the given constraints it is always safe. Negative and zero values are handled generically as long long.

#include <vector>

// Returns the element at the exact middle index of an odd-length vector.
// Precondition: v.size() is odd and at least 1.
long long findMiddleValue(const std::vector<long long>& v) {
    // For odd length, integer division gives the middle index.
    size_t middleIndex = v.size() / 2;
    return v[middleIndex];
}

#include <vector>
#include <cassert>

// Function declaration (should match the solution).
long long findMiddleValue(const std::vector<long long>& v);

int main() {
    // Test single element
    std::vector<long long> v1 = {42};
    assert(findMiddleValue(v1) == 42);

    // Test three elements with negatives
    std::vector<long long> v2 = {-10, 0, 10};
    assert(findMiddleValue(v2) == 0);

    // Test five elements
    std::vector<long long> v3 = {1, 2, 3, 4, 5};
    assert(findMiddleValue(v3) == 3);

    // Test seven elements with mixed signs
    std::vector<long long> v4 = {-5, -3, -1, 0, 1, 3, 5};
    assert(findMiddleValue(v4) == 0);

    // Test large values
    std::vector<long long> v5 = {1000000000000LL, -2000000000000LL, 3000000000000LL};
    assert(findMiddleValue(v5) == -2000000000000LL);

    // Test all same values
    std::vector<long long> v6 = {7, 7, 7, 7, 7};
    assert(findMiddleValue(v6) == 7);

    // Test with duplicates and non-sorted order
    std::vector<long long> v7 = {5, 1, 9, 2, 8};
    assert(findMiddleValue(v7) == 9);  // index 2 (0-based) is 9

    // Test negative-only values
    std::vector<long long> v8 = {-1, -2, -3};
    assert(findMiddleValue(v8) == -2);

    return 0;
}
