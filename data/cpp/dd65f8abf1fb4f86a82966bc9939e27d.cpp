// Write a C++ function named `mergeSortedArraysInPlace` that takes two `std::vector<int>` references, `arr1` and `arr2`, along with two integers `m` and `n`, where `m` is the number of meaningful elements initially stored in the first `m` slots of `arr1`, and `n` is the size of `arr2`. The vector `arr1` has been preallocated to hold `m + n` elements (the extra `n` slots may contain any garbage values). Both meaningful portions are sorted in non-decreasing order. The function must merge the two sorted arrays into `arr1` so that the entire `arr1` becomes sorted in non-decreasing order and then return `arr1`. The function should not use any extra dynamic memory beyond a constant number of variables, and it must operate in-place. Handle edge cases where `m` or `n` is zero.
The optimal strategy is to traverse the two sorted segments from the back to the front, because the back of `arr1` (where the garbage values are located) is empty in terms of meaningful data. Maintain three indices: `i = m - 1` for the last meaningful element of the original `arr1`, `j = n - 1` for the last element of `arr2`, and `k = m + n - 1` for the last position of the final merged array. While both `i` and `j` are non‑negative, compare `arr1[i]` and `arr2[j]`, place the larger one at `arr1[k]`, then decrement the corresponding index (`i` or `j`) and `k`. This guarantees that the largest remaining element goes to the correct final position without overwriting any meaningful data. After this loop, if any elements remain in the original `arr1` (i.e., `i >= 0`), they are already in their correct relative order and can be left as is because the front part of `arr1` is already sorted and smaller than everything already placed. If any elements remain in `arr2` (i.e., `j >= 0`), copy them one by one into the remaining front slots of `arr1`. This works because those remaining elements are all smaller than or equal to the smallest element already placed, so they belong at the very beginning. Edge cases: if `m == 0`, the function simply copies all of `arr2` into `arr1`; if `n == 0`, it does nothing. The algorithm runs in `O(m + n)` time and uses `O(1)` auxiliary space beyond the input/output, making it efficient for large arrays. The key insight is that starting from the back avoids the need for a temporary array and prevents overwriting.
#include <vector>

// Merge two sorted arrays in-place into arr1 (which already has space for m+n).
// arr1 contains m meaningful elements at the front; arr2 has n elements.
// Both input portions are sorted in non-decreasing order.
std::vector<int>& mergeSortedArraysInPlace(std::vector<int>& arr1,
                                           const std::vector<int>& arr2,
                                           int m, int n) {
    int i = m - 1;           // last meaningful element in arr1
    int j = n - 1;           // last element in arr2
    int k = m + n - 1;       // last position in the merged array

    // Merge from the back, placing the larger element at arr1[k]
    while (i >= 0 && j >= 0) {
        if (arr1[i] > arr2[j]) {
            arr1[k--] = arr1[i--];
        } else {
            arr1[k--] = arr2[j--];
        }
    }

    // If any elements remain in arr2, copy them to the front.
    // Elements left in arr1 are already in correct position.
    while (j >= 0) {
        arr1[k--] = arr2[j--];
    }

    return arr1;
}
#include <cassert>
#include <vector>

// Function prototype (as defined in the solution)
std::vector<int>& mergeSortedArraysInPlace(std::vector<int>& arr1,
                                           const std::vector<int>& arr2,
                                           int m, int n);

int main() {
    // Test 1: Standard merge with both arrays non-empty
    std::vector<int> a1 = {1, 3, 5, 0, 0, 0};
    std::vector<int> a2 = {2, 4, 6};
    mergeSortedArraysInPlace(a1, a2, 3, 3);
    assert((a1 == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // Test 2: All elements in arr2 are smaller than arr1
    std::vector<int> b1 = {4, 5, 6, 0, 0, 0};
    std::vector<int> b2 = {1, 2, 3};
    mergeSortedArraysInPlace(b1, b2, 3, 3);
    assert((b1 == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // Test 3: All elements in arr1 are smaller than arr2
    std::vector<int> c1 = {1, 2, 3, 0, 0, 0};
    std::vector<int> c2 = {4, 5, 6};
    mergeSortedArraysInPlace(c1, c2, 3, 3);
    assert((c1 == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // Test 4: arr2 is empty (n = 0)
    std::vector<int> d1 = {1, 4, 7};
    std::vector<int> d2 = {};
    mergeSortedArraysInPlace(d1, d2, 3, 0);
    assert((d1 == std::vector<int>{1, 4, 7}));

    // Test 5: arr1 has no meaningful elements (m = 0)
    std::vector<int> e1 = {0, 0, 0};
    std::vector<int> e2 = {2, 5, 9};
    mergeSortedArraysInPlace(e1, e2, 0, 3);
    assert((e1 == std::vector<int>{2, 5, 9}));

    // Test 6: Duplicate elements
    std::vector<int> f1 = {1, 2, 3, 0, 0, 0};
    std::vector<int> f2 = {1, 2, 3};
    mergeSortedArraysInPlace(f1, f2, 3, 3);
    assert((f1 == std::vector<int>{1, 1, 2, 2, 3, 3}));

    // Test 7: Single element each
    std::vector<int> g1 = {7, 0};
    std::vector<int> g2 = {3};
    mergeSortedArraysInPlace(g1, g2, 1, 1);
    assert((g1 == std::vector<int>{3, 7}));

    // Test 8: Many elements, including negative numbers
    std::vector<int> h1 = {-10, -5, 0, 4, 9, 0, 0, 0};
    std::vector<int> h2 = {-8, 2, 12};
    mergeSortedArraysInPlace(h1, h2, 5, 3);
    assert((h1 == std::vector<int>{-10, -8, -5, 0, 2, 4, 9, 12}));

    return 0;
}
