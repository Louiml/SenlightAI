Write a C++ function named `reverseHalfArray` that takes a reference to a `std::array<int, 5>` and reverses only the first half of the array (i.e., swaps elements at index `0` with index `4`, and index `1` with index `3`), leaving the middle element (index `2`) unchanged. The function must modify the array in-place and return `void`. The array always has exactly 5 elements. Handle the loop condition carefully to avoid an infinite loop or out-of-bounds access. Your solution should use a single loop, a temporary variable for swapping, and a classic two-pointer approach (start index and end index). Do not use any standard library algorithms like `std::reverse`.
#include <cassert>
#include <array>

// The solution function is declared above (reverseHalfArray).
void reverseHalfArray(std::array<int, 5>& arr); // forward declaration for test

int main() {
    // Test 1: Basic case from the snippet
    std::array<int, 5> a1 = {1, 2, 3, 4, 5};
    reverseHalfArray(a1);
    assert(a1 == std::array<int, 5>({5, 4, 3, 2, 1}));

    // Test 2: All identical values, no visible change
    std::array<int, 5> a2 = {7, 7, 7, 7, 7};
    reverseHalfArray(a2);
    assert(a2 == std::array<int, 5>({7, 7, 7, 7, 7}));

    // Test 3: Negative numbers
    std::array<int, 5> a3 = {-1, -2, 0, 2, 1};
    reverseHalfArray(a3);
    assert(a3 == std::array<int, 5>({1, 2, 0, -2, -1}));

    // Test 4: Zeroes and large numbers
    std::array<int, 5> a4 = {100, 0, -5, 0, 200};
    reverseHalfArray(a4);
    assert(a4 == std::array<int, 5>({200, 0, -5, 0, 100}));

    // Test 5: Verify middle element remains unchanged when others differ
    std::array<int, 5> a5 = {10, 20, 30, 40, 50};
    reverseHalfArray(a5);
    assert(a5 == std::array<int, 5>({50, 40, 30, 20, 10}));
    assert(a5[2] == 30); // middle unchanged

    return 0;
}
#include <array>

// Reverse the outer elements of a fixed-size 5-element array in-place.
// Swaps index 0 with 4, and 1 with 3. The middle element (index 2) stays.
void reverseHalfArray(std::array<int, 5>& arr) {
    int left = 0;
    int right = 4;
    while (left < right) {
        // Swap the elements at left and right
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
    // For size 5, this performs exactly two swaps: (0,4) and (1,3).
}
// The task is to reverse the "outer" elements of a fixed-size 5-element array, which is equivalent to swapping index `i` with index `4-i` for `i` from 0 up to (but not including) the middle index. Since the array size is 5, the middle index is 2. So we need `i` to take values 0 and 1. A safe loop condition is `while (i < j)` with `i` starting at 0 and `j` starting at 4, incrementing `i` and decrementing `j` each iteration. This ensures exactly two swaps: (0↔4) and (1↔3), leaving index 2 untouched. Edge cases: if the array somehow had an even length, the loop would naturally stop when `i >= j`, but here it is fixed at 5. The loop must not use a faulty condition like `i < 5/2` with an odd combined condition that could cause infinite loops (as in the original snippet). Time complexity is O(1) since the array size is fixed at 5, and space complexity is O(1) (only one temporary integer). The function modifies the array in-place, so no copying is needed. The solution uses `std::array<int, 5>` to enforce the fixed size and allow range-based loops in tests for verification.
