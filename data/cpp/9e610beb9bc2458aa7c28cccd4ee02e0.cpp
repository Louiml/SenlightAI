// Write a C++ function named `sumZeroArray` that takes a positive integer `n` and returns a `std::vector<int>` of length `n` such that all numbers in the array are unique and their sum equals 0. The array must contain no duplicate values (with the exception that if `n` is odd, one element can be 0, and all other values must be distinct nonzero integers). For the solution, the simplest approach is to pair `i` and `-i` for `i` from 1 to `n/2`. If `n` is odd, the last element remains 0. The function should return a vector that satisfies the properties; the order of elements does not matter except that the returned vector must have the exact length `n` and contain `n` unique integers summing to zero.
The main algorithm is straightforward: for each `i` from 1 to `n/2`, place `i` at an even index and `-i` at an odd index. Since each positive number is paired with its negation, the sum of every pair is zero. If `n` is odd, the remaining uninitialized element (which is at index `n-1`, the last index) stays 0, which keeps the total sum zero. Edge cases: `n=1` returns `{0}`; `n=2` returns `{1, -1}` or `{-1, 1}` depending on order; large `n` works fine because the values are bounded by `n/2`. The values produced are distinct because no two positive integers equal each other, and the only possible duplicate would be 0, which appears only when `n` is odd and only once. Time complexity is O(n) because we fill the vector in a single loop over `n/2` iterations. Space complexity is O(n) for the output vector; no extra auxiliary space is used beyond loop variables.
#include <vector>

// Returns a vector of length n containing distinct integers whose sum is 0.
// For even n, pairs (i, -i) for i=1..n/2 are used.
// For odd n, the last element remains 0, and pairs are used for the first n-1 elements.
std::vector<int> sumZeroArray(int n) {
    std::vector<int> result(n, 0);
    for (int i = 0; i < n / 2; ++i) {
        result[2 * i] = i + 1;
        result[2 * i + 1] = -(i + 1);
    }
    return result;
}
#include <cassert>
#include <numeric>
#include <vector>
#include <algorithm>

// The solution function is declared above; the test includes it via the same translation unit.
int main() {
    // Helper to verify sum is zero and all elements are unique.
    auto check = [](const std::vector<int>& v) {
        assert(std::accumulate(v.begin(), v.end(), 0) == 0);
        std::vector<int> copy = v;
        std::sort(copy.begin(), copy.end());
        assert(std::adjacent_find(copy.begin(), copy.end()) == copy.end());
    };

    // Test basic cases
    check(sumZeroArray(1));   // {0}
    check(sumZeroArray(2));   // {1, -1} or {-1, 1}
    check(sumZeroArray(3));   // {1, -1, 0}
    check(sumZeroArray(4));   // {1, -1, 2, -2}
    check(sumZeroArray(5));   // {1, -1, 2, -2, 0}

    // Test larger even and odd sizes
    check(sumZeroArray(10));
    check(sumZeroArray(11));
    check(sumZeroArray(100));

    // Verify the length is correct
    assert(sumZeroArray(6).size() == 6);
    assert(sumZeroArray(7).size() == 7);

    // For known cases, verify specific content (order not enforced)
    std::vector<int> v2 = sumZeroArray(2);
    assert((v2[0] == 1 && v2[1] == -1) || (v2[0] == -1 && v2[1] == 1));

    std::vector<int> v3 = sumZeroArray(3);
    assert((v3[0] == 1 && v3[1] == -1 && v3[2] == 0) ||
           (v3[0] == -1 && v3[1] == 1 && v3[2] == 0));
}
