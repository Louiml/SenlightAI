Write a C++ function named `rotateRight` that takes a non-empty vector of integers (`std::vector<int>&`) and an integer `k` representing the number of positions to rotate the vector to the right. The function must modify the vector in place, such that each element is shifted `k` positions to the right, with elements that fall off the end wrapping around to the beginning. The value of `k` can be negative (meaning rotate left), zero, or larger than the vector size, and the function must handle all such cases correctly. Do not use any extra container (e.g., a second vector, deque, or array) for the rotation; only constant extra space (like temporary variables) is allowed. Also, do not use the standard library `std::rotate` algorithm. The rotation must be performed by a sequence of `std::reverse` operations, as inspired by the provided snippet. The function signature should be `void rotateRight(std::vector<int>& nums, int k)`.
The classic three-reversal rotation technique works as follows:  
1. Normalize `k` to be within `[0, n-1]` where `n = nums.size()`. Since rotation by `n` positions returns the array to its original order, we compute `k = k % n`. If `k` is negative, we convert it to an equivalent positive by adding `n` (e.g., `k = (k % n + n) % n`). After this, `k` is in `[0, n-1]`.  
2. Reverse the entire vector.  
3. Reverse the first `k` elements (indices `[0, k-1]`).  
4. Reverse the remaining `n - k` elements (indices `[k, n-1]`).  

This produces the desired right rotation because reversing the whole array moves the last `k` elements to the front, and then reversing those two segments restores the correct internal order.  

Edge cases:  
- `n == 0`? The problem states non-empty, but if we handle it defensively, we can return immediately to avoid modulo by zero.  
- `k == 0` or `k` multiple of `n` → no change after normalization is 0, so just return (reversing whole then two zero-length segments is fine but unnecessary).  
- Negative `k`: e.g., `k = -1` means rotate right by -1 = rotate left by 1. After normalization, `k` becomes `n-1`, which correctly shifts right by `n-1` (equivalent to left by 1).  
- Very large `k`: handled by modulo.  

Time complexity: O(n) because we reverse the whole array (n/2 swaps) and two subarrays (total n/2 swaps) → O(n). Space: O(1) auxiliary (only a few integer variables for indices, no extra containers).
#include <vector>
#include <algorithm>

// Rotate the vector to the right by k positions in O(n) time and O(1) space.
// Handles negative k, k > n, and k == 0.
void rotateRight(std::vector<int>& nums, int k) {
    const int n = static_cast<int>(nums.size());
    if (n <= 1) {
        return; // Nothing to rotate, also avoids modulo by zero
    }

    // Normalize k to be in [0, n-1] to handle negative and large k
    k = k % n;
    if (k < 0) {
        k += n; // Convert negative to positive
    }

    if (k == 0) {
        return; // No rotation needed
    }

    // Three-step reversal:
    std::reverse(nums.begin(), nums.end());             // 1. Reverse all
    std::reverse(nums.begin(), nums.begin() + k);       // 2. Reverse first k
    std::reverse(nums.begin() + k, nums.end());         // 3. Reverse remaining
}
#include <vector>
#include <cassert>

// Forward declaration of the solution function (or include header)
void rotateRight(std::vector<int>& nums, int k);

int main() {
    // Basic right rotation
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    rotateRight(v1, 2);
    assert((v1 == std::vector<int>{4, 5, 1, 2, 3}));

    // Rotate by full size -> no change
    std::vector<int> v2 = {1, 2, 3};
    rotateRight(v2, 3);
    assert((v2 == std::vector<int>{1, 2, 3}));

    // Negative k (rotate left by 1)
    std::vector<int> v3 = {10, 20, 30};
    rotateRight(v3, -1);
    assert((v3 == std::vector<int>{20, 30, 10}));

    // k larger than size
    std::vector<int> v4 = {1, 2, 3, 4};
    rotateRight(v4, 6); // 6 % 4 = 2
    assert((v4 == std::vector<int>{3, 4, 1, 2}));

    // k = 0
    std::vector<int> v5 = {7, 8, 9};
    rotateRight(v5, 0);
    assert((v5 == std::vector<int>{7, 8, 9}));

    // Single element
    std::vector<int> v6 = {42};
    rotateRight(v6, 100);
    assert((v6 == std::vector<int>{42}));

    // Negative large k
    std::vector<int> v7 = {1, 2, 3, 4, 5};
    rotateRight(v7, -7); // -7 % 5 = -2 -> +5 = 3
    assert((v7 == std::vector<int>{3, 4, 5, 1, 2}));

    // Two elements
    std::vector<int> v8 = {1, 2};
    rotateRight(v8, 1);
    assert((v8 == std::vector<int>{2, 1}));

    return 0;
}
