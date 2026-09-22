Write a C++ function named `findMinMaxIndices` that takes a non-empty array of integers (`int a[]`), its size (`int n`), and two integer reference parameters (`indexOfMin` and `indexOfMax`). The function should locate the indices of the minimum and maximum elements in the array. If the array has fewer than one element (i.e., `n < 1`), the function should return `false` and leave the reference parameters unchanged. Otherwise, it should update the reference parameters with the correct indices and return `true`. In the case of duplicate values, the function should return the first occurrence of the minimum and the first occurrence of the maximum. The function must be `const`-correct where appropriate (the input array should be treated as read-only). Provide a clear, self-contained implementation.

// The algorithm is a straightforward single-pass linear scan. Initialize both `indexOfMin` and `indexOfMax` to `0` as the initial guess. Then iterate from index `1` to `n-1`. For each element `a[i]`, if `a[i]` is strictly less than `a[indexOfMin]`, update `indexOfMin` to `i`. Similarly, if `a[i]` is strictly greater than `a[indexOfMax]`, update `indexOfMax` to `i`. Using strict comparisons ensures that the first occurrence of duplicate values is kept, which matches the specification. Edge cases: if `n == 0` (empty array), return `false` immediately without modifying the reference parameters. If `n == 1`, both indices remain `0` and the function returns `true`. The time complexity is \(O(n)\) because each element is visited exactly once, and the space complexity is \(O(1)\) as only a constant number of auxiliary variables are used.

#include <cstddef> // for std::size_t

// Locate the indices of the minimum and maximum elements in a[0:n-1].
// Returns false if n < 1; otherwise returns true and sets indexOfMin and indexOfMax.
template<class T>
bool findMinMaxIndices(const T a[], int n, int& indexOfMin, int& indexOfMax) {
    if (n < 1) return false;

    indexOfMin = 0;
    indexOfMax = 0;

    for (int i = 1; i < n; ++i) {
        if (a[indexOfMin] > a[i]) {
            indexOfMin = i;
        }
        if (a[indexOfMax] < a[i]) {
            indexOfMax = i;
        }
    }

    return true;
}

#include <cassert>

int main() {
    int a1[6] = {2, 6, 4, 3, 1, 5};
    int minIdx, maxIdx;
    assert(findMinMaxIndices(a1, 6, minIdx, maxIdx) == true);
    assert(minIdx == 4 && maxIdx == 1);

    int a2[1] = {42};
    assert(findMinMaxIndices(a2, 1, minIdx, maxIdx) == true);
    assert(minIdx == 0 && maxIdx == 0);

    int a3[4] = {5, 5, 5, 5};
    assert(findMinMaxIndices(a3, 4, minIdx, maxIdx) == true);
    assert(minIdx == 0 && maxIdx == 0); // first occurrences of duplicates

    int a4[] = {-10, -3, -8, -1};
    assert(findMinMaxIndices(a4, 4, minIdx, maxIdx) == true);
    assert(minIdx == 0 && maxIdx == 3);

    // Edge case: empty array
    int a5[0]; // size 0, but we pass 0 as n
    assert(findMinMaxIndices(a5, 0, minIdx, maxIdx) == false);

    // Mixed values with duplicates
    int a6[7] = {7, 2, 9, 2, 1, 9, 3};
    assert(findMinMaxIndices(a6, 7, minIdx, maxIdx) == true);
    assert(minIdx == 4 && maxIdx == 2);
}
