// Write a C++ function that takes a reference to a `std::vector<int>` and returns a boolean indicating whether there exist three distinct indices whose values sum to zero. The function must handle vectors of any size, including empty vectors, and must not modify the original vector unless a local copy is explicitly made. The input vector may contain negative numbers, zeros, and duplicates; duplicates are allowed to be used only once per triplet (i.e., you cannot reuse the same element more than once, but if the same value appears at multiple indices, each such index is treated as distinct). The solution should be efficient for large vectors.
#include <cassert>
#include <vector>

// The solution function is declared above; we just need main.
int main() {
    // Basic positive case
    std::vector<int> a = {0, -1, 2, -3, 1};
    assert(hasZeroSumTriplet(a) == true); // (-1, 0, 1) exists

    // No triplet sums to zero
    std::vector<int> b = {1, 2, 3, 4};
    assert(hasZeroSumTriplet(b) == false);

    // Fewer than 3 elements
    std::vector<int> c = {};
    assert(hasZeroSumTriplet(c) == false);
    std::vector<int> d = {5};
    assert(hasZeroSumTriplet(d) == false);
    std::vector<int> e = {1, -1};
    assert(hasZeroSumTriplet(e) == false);

    // All zeros
    std::vector<int> f = {0, 0, 0};
    assert(hasZeroSumTriplet(f) == true);

    // Duplicates that must use distinct indices
    std::vector<int> g = {2, 2, -4};
    assert(hasZeroSumTriplet(g) == true); // 2 + 2 + (-4) = 0

    // Large negative numbers
    std::vector<int> h = {-5, -4, 0, 1, 2};
    assert(hasZeroSumTriplet(h) == false);

    // Test with a triplet where target is zero
    std::vector<int> i = {-1, 0, 1, 2};
    assert(hasZeroSumTriplet(i) == true);

    // Input remains unmodified
    std::vector<int> original = {3, -1, 2, -2};
    std::vector<int> copy = original;
    hasZeroSumTriplet(original);
    assert(original == copy);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns true if there exist three distinct indices i, j, k such that
// arr[i] + arr[j] + arr[k] == 0. The input vector is not modified.
bool hasZeroSumTriplet(const std::vector<int>& arr) {
    const size_t n = arr.size();
    if (n < 3) return false;

    // Work on a copy because sorting is required.
    std::vector<int> sorted = arr; // non-const copy
    std::sort(sorted.begin(), sorted.end());

    for (size_t i = 0; i + 2 < n; ++i) {
        size_t left = i + 1;
        size_t right = n - 1;
        const int target = -sorted[i];

        while (left < right) {
            const int sum = sorted[left] + sorted[right];
            if (sum == target) {
                return true;
            } else if (sum < target) {
                ++left;
            } else {
                --right;
            }
        }
    }
    return false;
}
// The classic approach is to sort the array first, then fix one element and use a two-pointer technique to find the other two. Sorting takes \(O(n \log n)\). After sorting, iterate `i` from 0 to `n-3`, setting `target = -arr[i]`. For each `i`, place `left = i+1` and `right = n-1`. While `left < right`, compute `sum = arr[left] + arr[right]`. If `sum == target`, return true immediately. If `sum < target`, increment `left`; otherwise decrement `right`. Because the array is sorted, this two-pointer scan correctly explores all pairs for that fixed `i` in \(O(n)\) time, leading to an overall \(O(n^2)\) time after sorting. Space complexity is \(O(1)\) auxiliary (ignoring the sort’s internal memory). Edge cases: if `n < 3`, return false immediately. Duplicate values are naturally handled because each index is distinct; you don’t need to skip duplicates. Empty or single-element vectors return false. The function should accept a `const std::vector<int>&` to avoid copying and must sort a local copy because `sort` requires non-const access.
