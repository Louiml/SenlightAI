/*
Write a C++ function that accepts a fixed-size vector of exactly 4 integers (represented as a `std::array<int, 4>` or a custom 4-element vector type) and an integer indicating the number of trailing elements (1 to 3) to modify. The function must set the specified trailing elements to zero and return the original order of those elements (before modification) as a new fixed-size vector of the same length (with non-trailing positions containing dummy zeros or preserving original values—choose a clear specification). Handle edge cases where the count is 0 or equals the vector size by returning an empty or appropriately sized result. The function should not modify the input vector; it should work on a copy.
*/
#include <array>
#include <cstddef>
#include <algorithm>

// Given a 4-element array and a count k (0 to 4), returns an array of the same
// size where the first (4 - k) elements are -1 (sentinel) and the last k elements
// hold the original values of the input's last k elements. The input is copied
// and its last k elements are set to zero (this copy is not returned).
// The function does not modify the input.
std::array<int, 4> zeroTrailingAndCapture(const std::array<int, 4>& input, int k) {
    // Clamp k to valid range [0, 4]
    k = std::clamp(k, 0, 4);
    
    // Result array: fill non-trailing positions with -1 sentinel, trailing with original values
    std::array<int, 4> result;
    for (std::size_t i = 0; i < 4; ++i) {
        if (static_cast<int>(i) < 4 - k) {
            result[i] = -1;
        } else {
            result[i] = input[i];
        }
    }
    
    // This function's main purpose is to demonstrate the trailing-zero modification,
    // so we create a copy and zero the trailing elements (though not returned).
    // For a more useful variant, we could return both, but per specification we
    // only return the captured originals.
    std::array<int, 4> copy = input;
    for (int i = 4 - k; i < 4; ++i) {
        copy[i] = 0;
    }
    // (copy is intentionally unused; the task focuses on the captured order.)
    
    return result;
}
#include <cassert>
#include <array>

int main() {
    std::array<int, 4> v = {10, 20, 30, 40};
    
    // k=2: return last two originals, first two sentinels
    auto r1 = zeroTrailingAndCapture(v, 2);
    assert(r1 == std::array<int, 4>({-1, -1, 30, 40}));
    
    // k=0: nothing captured, all sentinels
    auto r2 = zeroTrailingAndCapture(v, 0);
    assert(r2 == std::array<int, 4>({-1, -1, -1, -1}));
    
    // k=4: all captured
    auto r3 = zeroTrailingAndCapture(v, 4);
    assert(r3 == v);
    
    // k=1: only last element
    auto r4 = zeroTrailingAndCapture(v, 1);
    assert(r4 == std::array<int, 4>({-1, -1, -1, 40}));
    
    // k=3: last three captured
    auto r5 = zeroTrailingAndCapture(v, 3);
    assert(r5 == std::array<int, 4>({-1, 20, 30, 40}));
    
    // Negative k clamps to 0
    auto r6 = zeroTrailingAndCapture(v, -5);
    assert(r6 == std::array<int, 4>({-1, -1, -1, -1}));
    
    // k > 4 clamps to 4
    auto r7 = zeroTrailingAndCapture(v, 10);
    assert(r7 == v);
    
    // Input unchanged
    assert(v == std::array<int, 4>({10, 20, 30, 40}));
    
    return 0;
}
// The solution should copy the input vector, then iterate over the last `k` positions (indices `size - k` to `size - 1`) and store their original values into a result array of the same size (for simplicity, fill the non-trailing indices with -1 as sentinel). Then set those positions in the copied vector to zero. Key edge cases: `k` must be clamped between 0 and the vector size (4). If `k` is 0, no elements are zeroed, and the result array should be all sentinels. If `k` equals size, all 4 elements are zeroed, and the result contains all original values. The time complexity is O(4) = O(1) since the size is fixed, and space complexity is O(1) for the temporary arrays. The function must be `const`-correct on the input (taking by const reference) and return the result by value.
