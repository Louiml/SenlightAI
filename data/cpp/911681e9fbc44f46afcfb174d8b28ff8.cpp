Write a C++ function named `swapAdjacentPairs` that takes an array of integers and its size as parameters, and modifies the array in place by swapping every adjacent pair of elements: positions (0,1), (2,3), (4,5), and so on. If the array has an odd number of elements, the last element remains unchanged. The function must return `void` and must not use any external libraries beyond the C++ standard library. The function should be `const`-correct where appropriate (though the array itself is mutable), and should handle edge cases such as an empty array, an array of size 1, and an array with duplicate values without any special-casing beyond the natural loop logic.

The solution uses a simple loop that iterates with a step of 2. For each index `i`, if `i+1` is less than the array size, swap `arr[i]` and `arr[i+1]`. This ensures that pairs are swapped correctly, and the last element in an odd-sized array is naturally left untouched because the condition `i+1 < size` fails. The algorithm runs in O(n) time because each element is visited at most once in the loop (step size 2) and each swap is O(1). It uses O(1) auxiliary space, as only a temporary variable for swapping is involved (managed by `std::swap`). Edge cases: an empty array (size 0) causes the loop not to execute; a size-1 array also skips the loop body because `i+1 < size` is false. Duplicate values require no handling because swapping is order-independent. The function modifies the array in place, so no return value is needed. It is good practice to pass the size as `int` and use `size_t` for indices, but since the original snippet uses `int`, we keep that for consistency, though using `int` for indexing is fine as long as we guard against negative sizes (which are not expected from the caller).

#include <utility> // for std::swap

// Swaps each adjacent pair of elements in the array.
// If the array size is odd, the last element stays in place.
void swapAdjacentPairs(int arr[], int size) {
    for (int i = 0; i + 1 < size; i += 2) {
        std::swap(arr[i], arr[i + 1]);
    }
}

#include <cassert>

// The solution function is declared above (in the same translation unit).
// Test cases verify correct behavior for various inputs.
int main() {
    // Test 1: Even-sized array
    int even[8] = {5, 2, 9, 6, 5, 6, 8, 4};
    int evenExpected[8] = {2, 5, 6, 9, 6, 5, 4, 8};
    swapAdjacentPairs(even, 8);
    for (int i = 0; i < 8; ++i) assert(even[i] == evenExpected[i]);

    // Test 2: Odd-sized array (last element unchanged)
    int odd[5] = {11, 33, 9, 76, 46};
    int oddExpected[5] = {33, 11, 76, 9, 46};
    swapAdjacentPairs(odd, 5);
    for (int i = 0; i < 5; ++i) assert(odd[i] == oddExpected[i]);

    // Test 3: Empty array (no crash)
    int empty[0] = {};
    swapAdjacentPairs(empty, 0);
    // No assertions needed; just ensure it runs without error.

    // Test 4: Single-element array (unchanged)
    int single[1] = {42};
    swapAdjacentPairs(single, 1);
    assert(single[0] == 42);

    // Test 5: Array with duplicate values
    int dup[4] = {7, 7, 3, 3};
    int dupExpected[4] = {7, 7, 3, 3}; // Swapping equal values yields same array
    swapAdjacentPairs(dup, 4);
    for (int i = 0; i < 4; ++i) assert(dup[i] == dupExpected[i]);

    // Test 6: Two-element array
    int two[2] = {-1, 5};
    swapAdjacentPairs(two, 2);
    assert(two[0] == 5 && two[1] == -1);

    return 0;
}
