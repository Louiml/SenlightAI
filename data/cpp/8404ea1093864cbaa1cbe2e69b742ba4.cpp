// Write a C++ function that takes a non-empty vector of integers and an index `k` (where `0 <= k < vec.size()`), and swaps the element at index 0 with the element at index `k`. The function should modify the vector in-place and return nothing. If `k == 0`, the vector should remain unchanged. You must use `std::iter_swap` to perform the swap, and the function must be `const`-correct by taking the vector by non-const reference. After the swap, the function should not print anything—it should only modify the vector.
// The solution approach is straightforward: since we must use `std::iter_swap`, we obtain iterators to the first element and the element at index `k` using `vec.begin()` and `vec.begin() + k`. Then we call `iter_swap` on those two iterators. The key edge case is when `k == 0`: in that case, `vec.begin()` and `vec.begin() + 0` refer to the same location, and `iter_swap` will perform a self-swap, which is a no-op and leaves the vector unchanged—this is safe. We do not need to check bounds because the problem guarantees `0 <= k < vec.size()`. The time complexity is O(1) since swapping two elements is constant time regardless of vector size; the auxiliary space complexity is O(1) because no additional storage is used. The function must be written without a main function, and the vector is passed by reference to allow in-place modification.
#include <vector>
#include <algorithm> // for std::iter_swap

// Swaps the first element of the vector with the element at index k.
// If k == 0, the vector remains unchanged (self-swap is a no-op).
void swapFirstWithKth(std::vector<int>& vec, int k) {
    std::iter_swap(vec.begin(), vec.begin() + k);
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Test 1: Basic swap with a middle element
    std::vector<int> v1 = {1, 2, 3, 4};
    swapFirstWithKth(v1, 2);
    assert(v1 == std::vector<int>({3, 2, 1, 4}));

    // Test 2: Swap with the last element
    std::vector<int> v2 = {10, 20, 30, 40};
    swapFirstWithKth(v2, 3);
    assert(v2 == std::vector<int>({40, 20, 30, 10}));

    // Test 3: k == 0, vector unchanged
    std::vector<int> v3 = {5, 6, 7};
    swapFirstWithKth(v3, 0);
    assert(v3 == std::vector<int>({5, 6, 7}));

    // Test 4: k == 1, swap first two
    std::vector<int> v4 = {-1, -2, -3};
    swapFirstWithKth(v4, 1);
    assert(v4 == std::vector<int>({-2, -1, -3}));

    // Test 5: Single-element vector, k == 0
    std::vector<int> v5 = {42};
    swapFirstWithKth(v5, 0);
    assert(v5 == std::vector<int>({42}));

    // Test 6: Large vector, swap with a middle element
    std::vector<int> v6 = {0, 1, 2, 3, 4, 5, 6};
    swapFirstWithKth(v6, 4);
    assert(v6 == std::vector<int>({4, 1, 2, 3, 0, 5, 6}));

    // Test 7: Negative numbers, swap with last
    std::vector<int> v7 = {-5, -4, -3, -2, -1};
    swapFirstWithKth(v7, 4);
    assert(v7 == std::vector<int>({-1, -4, -3, -2, -5}));

    // Test 8: Duplicate values, swap with k=2
    std::vector<int> v8 = {7, 7, 7, 8};
    swapFirstWithKth(v8, 2);
    assert(v8 == std::vector<int>({7, 7, 7, 8})); // all 7's, so unchanged

    return 0;
}
