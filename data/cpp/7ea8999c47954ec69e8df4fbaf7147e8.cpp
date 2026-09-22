Write a C++ function `findMajorityElement` that takes a pointer to an array of integers, the array length `n`, and a sentinel value `fail`, and returns the majority element if one exists (an element that appears more than `n/2` times), otherwise returns `fail`. The function must work correctly for arrays with negative numbers, duplicate values, and cases where no majority element exists. Do not modify the input array, and handle the edge case where `n` is 0 by returning `fail`.

The Boyer-Moore majority vote algorithm is optimal for this problem. It works in two passes. In the first pass, we maintain a candidate `m` and a counter `c`. For each element, if the counter is zero, we set `m` to the current element and reset the counter to 1 (or increment). Otherwise, we increment the counter if the current element equals `m`, or decrement it otherwise. After this pass, `m` is the only possible majority candidate, but it might not be a true majority—so a second pass counts the actual occurrences of `m` in the array. If that count is more than `n/2`, we return `m`; otherwise, we return `fail`. Edge cases: (1) Empty array (`n == 0`) returns `fail`; (2) When the majority element appears exactly `n/2` times (for even `n`), it is not a majority and `fail` is returned; (3) The candidate selection is robust to arbitrary ordering, including arrays with multiple elements that tie in frequency. Time complexity is `O(n)` because each pass scans the array once. Space complexity is `O(1)` as we only use a few local variables.

#include <cstddef> // for std::size_t

// Returns the majority element (appearing > n/2 times) in the array xs of length n.
// If no majority element exists, returns the sentinel value 'fail'.
// Behavior is undefined if xs is null and n > 0; caller must ensure valid input.
int findMajorityElement(const int* xs, std::size_t n, int fail) {
    if (n == 0) {
        return fail;
    }

    // First pass: find a candidate for majority.
    int candidate = xs[0];
    int count = 1;

    for (std::size_t i = 1; i < n; ++i) {
        if (count == 0) {
            candidate = xs[i];
            count = 1;
        } else if (xs[i] == candidate) {
            ++count;
        } else {
            --count;
        }
    }

    // Second pass: verify if the candidate is a true majority.
    int occurrences = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (xs[i] == candidate) {
            ++occurrences;
        }
    }

    return (occurrences * 2 > n) ? candidate : fail;
}

#include <cassert>
#include <vector>

int main() {
    // Test with a clear majority.
    std::vector<int> arr1 = {3, 3, 4, 2, 4, 4, 2, 4, 4};
    assert(findMajorityElement(arr1.data(), arr1.size(), -1) == 4);

    // Test with no majority (even length, no element > n/2).
    std::vector<int> arr2 = {1, 2, 3, 4};
    assert(findMajorityElement(arr2.data(), arr2.size(), -1) == -1);

    // Test with no majority (odd length, but no element > n/2).
    std::vector<int> arr3 = {1, 1, 2, 2, 3};
    assert(findMajorityElement(arr3.data(), arr3.size(), -1) == -1);

    // Test with single element (trivially a majority).
    std::vector<int> arr4 = {42};
    assert(findMajorityElement(arr4.data(), arr4.size(), -999) == 42);

    // Test with all identical elements.
    std::vector<int> arr5 = {7, 7, 7, 7, 7};
    assert(findMajorityElement(arr5.data(), arr5.size(), 0) == 7);

    // Test with negative numbers.
    std::vector<int> arr6 = {-1, -1, -1, -2, -3};
    assert(findMajorityElement(arr6.data(), arr6.size(), 0) == -1);

    // Test empty array.
    std::vector<int> empty;
    assert(findMajorityElement(empty.data(), empty.size(), -100) == -100);

    // Test with exactly n/2 occurrences (not a majority for even n).
    std::vector<int> arr7 = {5, 5, 6, 6};
    assert(findMajorityElement(arr7.data(), arr7.size(), -1) == -1);

    // Test with majority at the end.
    std::vector<int> arr8 = {1, 2, 3, 4, 5, 5, 5, 5, 5};
    assert(findMajorityElement(arr8.data(), arr8.size(), -1) == 5);

    // Test with majority at the beginning and duplicates.
    std::vector<int> arr9 = {2, 2, 2, 1, 3, 2, 2, 0};
    assert(findMajorityElement(arr9.data(), arr9.size(), -1) == 2);
}
