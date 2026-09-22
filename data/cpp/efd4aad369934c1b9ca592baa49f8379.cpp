Write a C++ function `populateAlternating` that takes an integer array `arr` and its size `n` as parameters. The function must fill the array with the integers from 1 to `n` (both inclusive) arranged in the order: all odd numbers in increasing sequence first (1, 3, 5, ...), then all even numbers in decreasing sequence (..., 6, 4, 2). For example, if `n = 7`, the array should contain {1, 3, 5, 7, 6, 4, 2}. The function should handle any `n >= 0`; if `n = 0`, the array should remain unchanged (no elements to fill). Do not print the array—only populate it. The function must work for any valid size up to 10^4, and you should ensure the algorithm is efficient in both time and memory.

The solution splits the array into two halves. For even `n`, the first `n/2` elements are the odd numbers: position `i` (0-indexed) gets `2*i + 1`. The remaining `n/2` elements are even numbers in decreasing order: position `i` (starting from `n/2`) gets `n - 2*(i - n/2)`. For odd `n`, the first `(n/2)+1` elements are the odd numbers: position `i` gets `2*i + 1` (this makes the last odd number `n` itself). The remaining `n - (n/2) - 1` elements are even numbers decreasing: position `i` (starting from `n/2 + 1`) gets `n - 2*(i - n/2 - 1) - 1`, which simplifies to an even sequence starting from `n-1` down to 2. The edge case `n = 0` requires no writes. The solution uses a single pass over the array with constant-time assignments, so time complexity is `O(n)` and auxiliary space is `O(1)` (excluding the input array itself, which is modified in place). The approach handles both parity cases without any branching inside the loop, making it robust and efficient.

#include <cstddef>

// Fills arr with integers 1..n in order: odd ascending, then even descending.
// If n == 0, arr is left unchanged.
void populateAlternating(int* arr, int n) {
    if (n <= 0) return;  // Nothing to fill for non-positive sizes

    int half = n / 2;  // Number of even elements to place at the end

    // Fill odd numbers: 1, 3, 5, ... for the first half (or half+1 if n odd)
    int oddCount = (n % 2 == 0) ? half : half + 1;
    for (int i = 0; i < oddCount; ++i) {
        arr[i] = 2 * i + 1;
    }

    // Fill even numbers in decreasing order: n or n-1, n-3, ... down to 2
    int nextEven = (n % 2 == 0) ? n : n - 1;  // Start from largest even number ≤ n
    for (int i = oddCount; i < n; ++i) {
        arr[i] = nextEven;
        nextEven -= 2;
    }
}

#include <cassert>
#include <vector>

// Free function declaration (should match the solution above)
void populateAlternating(int* arr, int n);

int main() {
    // Test n = 6 (even)
    int arr1[6];
    populateAlternating(arr1, 6);
    assert(arr1[0] == 1);
    assert(arr1[1] == 3);
    assert(arr1[2] == 5);
    assert(arr1[3] == 6);
    assert(arr1[4] == 4);
    assert(arr1[5] == 2);

    // Test n = 7 (odd)
    int arr2[7];
    populateAlternating(arr2, 7);
    assert(arr2[0] == 1);
    assert(arr2[1] == 3);
    assert(arr2[2] == 5);
    assert(arr2[3] == 7);
    assert(arr2[4] == 6);
    assert(arr2[5] == 4);
    assert(arr2[6] == 2);

    // Test n = 1
    int arr3[1];
    populateAlternating(arr3, 1);
    assert(arr3[0] == 1);

    // Test n = 2
    int arr4[2];
    populateAlternating(arr4, 2);
    assert(arr4[0] == 1);
    assert(arr4[1] == 2);

    // Test n = 0 (should do nothing)
    int arr5[1] = {42};
    populateAlternating(arr5, 0);
    assert(arr5[0] == 42);  // unchanged

    // Test n = 10 (larger even)
    int arr6[10];
    populateAlternating(arr6, 10);
    int expected6[10] = {1,3,5,7,9,10,8,6,4,2};
    for (int i = 0; i < 10; ++i) assert(arr6[i] == expected6[i]);

    // Test n = 9 (larger odd)
    int arr7[9];
    populateAlternating(arr7, 9);
    int expected7[9] = {1,3,5,7,9,8,6,4,2};
    for (int i = 0; i < 9; ++i) assert(arr7[i] == expected7[i]);

    return 0;
}
