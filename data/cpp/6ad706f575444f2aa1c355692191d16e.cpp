Given a sorted array of `2*n` distinct integers, write a C++ function that rearranges the array into a "zigzag alternating" sequence where elements from the sorted array are placed at positions `0, 2, 4, ...` in ascending order and at positions `1, 3, 5, ...` in descending order. Specifically, for an array `a` sorted in non‑decreasing order, the output array `b` of length `2*n` is defined as: `b[2*i] = a[i]` and `b[2*i+1] = a[2*n-1 - i]` for `i = 0, 1, ..., n-1`. That is, the smallest element goes to index 0, the next smallest to index 2, ..., the largest element to index 1, the second largest to index 3, etc. The function should take a `std::vector<int>` (or `std::vector<long long>`) containing `2*n` elements, assume it is already sorted, and return a new vector containing the rearranged sequence. Handle the general case where `n` can be any positive integer (test with various sizes). The input vector is not modified.

The task is straightforward once the pattern is understood. After sorting, the array `a` has indices `0, 1, ..., 2n-1`. For each `i` from 0 to `n-1`, the even index `2*i` receives `a[i]` (the i‑th smallest), and the odd index `2*i+1` receives `a[2n-1 - i]` (the i‑th largest). This creates an alternating sequence where even positions are increasing and odd positions are decreasing. Since the input is already sorted, no sorting is needed inside the function; we only allocate a result vector of size `2*n` and fill it in a single loop. The main edge case is ensuring that the index arithmetic correctly maps the largest element to position 1 and the smallest to position 0, which holds for all `n >= 1`. The time complexity is O(n) and the space complexity is O(n) for the output vector, excluding the input storage. The function must be `const`‑correct: it takes a `const std::vector<int>&` and returns a `std::vector<int>` by value.

#include <vector>

// Rearranges a sorted vector of 2*n elements into alternating ascending/descending order.
// Even indices (0, 2, 4, ...) get the smallest, next smallest, ...; odd indices get the largest, next largest, ...
// Precondition: input is sorted in non-decreasing order and has even length.
std::vector<int> zigzagAlternate(const std::vector<int>& sortedArr) {
    const int total = static_cast<int>(sortedArr.size());
    const int n = total / 2; // total is guaranteed even

    std::vector<int> result(total);

    for (int i = 0; i < n; ++i) {
        result[2 * i] = sortedArr[i];          // even positions: ascending
        result[2 * i + 1] = sortedArr[total - 1 - i]; // odd positions: descending
    }

    return result;
}

#include <cassert>
#include <vector>
#include <iostream>

// Declaration of the solution function (included here for testing)
std::vector<int> zigzagAlternate(const std::vector<int>& sortedArr);

int main() {
    // Example from the given snippet: sorted array of length 4 (n=2)
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> r1 = zigzagAlternate(a1);
    std::vector<int> expected1 = {1, 4, 2, 3};
    assert(r1 == expected1);

    // n=1
    std::vector<int> a2 = {10, 20};
    std::vector<int> r2 = zigzagAlternate(a2);
    std::vector<int> expected2 = {10, 20};
    assert(r2 == expected2);

    // n=3 (length 6)
    std::vector<int> a3 = {1, 3, 5, 7, 9, 11};
    std::vector<int> r3 = zigzagAlternate(a3);
    std::vector<int> expected3 = {1, 11, 3, 9, 5, 7};
    assert(r3 == expected3);

    // n=4 (length 8)
    std::vector<int> a4 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> r4 = zigzagAlternate(a4);
    std::vector<int> expected4 = {1, 8, 2, 7, 3, 6, 4, 5};
    assert(r4 == expected4);

    // Negative numbers
    std::vector<int> a5 = {-100, -50, 0, 50};
    std::vector<int> r5 = zigzagAlternate(a5);
    std::vector<int> expected5 = {-100, 50, -50, 0};
    assert(r5 == expected5);

    // Confirm that the input vector is not modified
    std::vector<int> original = {1, 2, 3, 4};
    zigzagAlternate(original);
    assert(original == std::vector<int>({1, 2, 3, 4}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
