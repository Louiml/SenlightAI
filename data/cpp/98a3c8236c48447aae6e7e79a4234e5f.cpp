Write a C++ function `randomizedShuffle(int* arr, int size)` that randomly permutes the given integer array using the Fisher–Yates shuffle algorithm. The function must modify the array in-place and support arrays of any positive size, including size 1 (which remains unchanged). The shuffle must be unbiased, meaning every possible permutation of the array must be equally likely. Use `rand()` as the source of randomness, and ensure the function is declared with appropriate `const` correctness (the array pointer itself is not const, but the size parameter is `const int`). The function should not return anything and must not allocate any dynamic memory. Your implementation must avoid using `swap` on out-of-range indices and must handle the loop correctly so that the current element can also be selected (i.e., it may stay in place). Provide a standalone solution without a `main` function, and later test it with assertions verifying that the multiset of elements is preserved and that repeated calls produce different orderings on a sufficiently large array with a fresh seed.

// The solution uses the modern Fisher–Yates shuffle, which iterates from the last index down to 0. At each step with index `i`, a random index `j` is chosen uniformly from the range `[0, i]` (inclusive) using `rand() % (i+1)`. Then the elements at `i` and `j` are swapped. This guarantees that after processing index `i`, the element at position `i` is fixed and will not be moved again, producing an unbiased random permutation. The key edge cases are: (1) when `size` is 0 or 1, the loop either doesn't run or swaps an index with itself, leaving the array unchanged; (2) using `rand() % (i+1)` ensures `j` is never greater than `i`, so no out-of-bounds access occurs; (3) the shuffle is in-place with O(1) extra space. Time complexity is O(n) because each element is swapped exactly once. Space complexity is O(1). For unbiasedness, we assume `rand()` produces a uniform distribution over its range, which is standard for this exercise.

#include <cstdlib>

// Randomly shuffle the first `size` elements of `arr` using the Fisher–Yates algorithm.
// `size` is the number of elements to shuffle; the array must have at least `size` elements.
// Uses rand() as the pseudo-random source.
void randomizedShuffle(int* arr, const int size) {
    // Iterate from the last index down to the first.
    for (int i = size - 1; i >= 0; --i) {
        // Pick a random index in the range [0, i] inclusive.
        int j = rand() % (i + 1);
        // Swap the current element with the randomly selected one.
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>

// Declare the function to be tested (as it is defined in the solution, but here we redeclare it for the test)
void randomizedShuffle(int* arr, const int size);

int main() {
    // Seed randomness for reproducible tests in this file (optional but useful)
    std::srand(42);

    // Test 1: Size 1 array remains unchanged (only one permutation possible)
    int a1[] = {7};
    randomizedShuffle(a1, 1);
    assert(a1[0] == 7);

    // Test 2: Size 0 array does nothing (no crash, no effect)
    int a2[] = {1, 2, 3}; // we pass size 0, so nothing should happen
    int before2[] = {1, 2, 3};
    randomizedShuffle(a2, 0);
    assert(a2[0] == 1 && a2[1] == 2 && a2[2] == 3);

    // Test 3: Elements are preserved (multiset unchanged) for a larger array
    int a3[] = {10, 20, 30, 40, 50};
    int original3[] = {10, 20, 30, 40, 50};
    randomizedShuffle(a3, 5);
    std::multiset<int> original_set(original3, original3 + 5);
    std::multiset<int> shuffled_set(a3, a3 + 5);
    assert(original_set == shuffled_set);

    // Test 4: Different seeds produce different orderings (likely, with size 5 it's almost certain)
    // Reset to a known array and shuffle with a different seed.
    int a4[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int first_shuffle[10];
    std::copy(a4, a4 + 10, first_shuffle);
    std::srand(123);
    randomizedShuffle(a4, 10);
    std::copy(a4, a4 + 10, first_shuffle); // Save the first shuffle

    // Now reset and shuffle again with a different seed, compare
    int a5[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::srand(456);
    randomizedShuffle(a5, 10);
    // At least one position should differ (with extremely high probability)
    bool differs = false;
    for (int i = 0; i < 10; ++i) {
        if (first_shuffle[i] != a5[i]) {
            differs = true;
            break;
        }
    }
    assert(differs);

    // Test 5: All elements still exactly the same set after shuffle on a small array
    int a6[] = {5, 5, 3, 3, 1};
    int original6[] = {5, 5, 3, 3, 1};
    randomizedShuffle(a6, 5);
    std::multiset<int> ms_orig(original6, original6 + 5);
    std::multiset<int> ms_shuf(a6, a6 + 5);
    assert(ms_orig == ms_shuf);

    // Test 6: After shuffling, the sum of indices * values is invariant (simple checksum)
    int a7[] = {1, 2, 3, 4};
    int sum_before = 0;
    for (int i = 0; i < 4; ++i) sum_before += a7[i] * (i+1);
    randomizedShuffle(a7, 4);
    int sum_after = 0;
    for (int i = 0; i < 4; ++i) sum_after += a7[i] * (i+1);
    // The sum may change; we don't assert this. But we do assert values are preserved
    std::multiset<int> ms7_orig({1,2,3,4});
    std::multiset<int> ms7_shuf(a7, a7+4);
    assert(ms7_orig == ms7_shuf);

    // Test 7: Array with negative numbers works fine
    int a8[] = {-1, -2, -3, 0};
    int original8[] = {-1, -2, -3, 0};
    randomizedShuffle(a8, 4);
    std::multiset<int> ms8_orig(original8, original8+4);
    std::multiset<int> ms8_shuf(a8, a8+4);
    assert(ms8_orig == ms8_shuf);

    return 0;
}
