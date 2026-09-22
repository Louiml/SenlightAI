// Write a C++ function named `rotateRight` that accepts a non-empty `std::vector<int>` by non-const reference and a non-negative integer `k`, then rotates the vector to the right by `k` positions. Rotation to the right means each element moves to the next higher index, with the last elements wrapping around to the front. If `k` is greater than or equal to the vector's size, the rotation effectively uses `k % size` steps. The function must modify the original vector in place and must not allocate extra storage proportional to the vector size (i.e., use only O(1) auxiliary space). The function should work correctly for vectors of length 1, for `k = 0`, and for very large `k` values. The function must not return anything.
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5, 6, 7};
    rotateRight(v1, 3);
    assert((v1 == std::vector<int>{5, 6, 7, 1, 2, 3, 4}));

    std::vector<int> v2 = {-1, -100, 3, 99};
    rotateRight(v2, 2);
    assert((v2 == std::vector<int>{3, 99, -1, -100}));

    std::vector<int> v3 = {1, 2, 3};
    rotateRight(v3, 0);
    assert((v3 == std::vector<int>{1, 2, 3}));

    std::vector<int> v4 = {1, 2, 3, 4};
    rotateRight(v4, 4);
    assert((v4 == std::vector<int>{1, 2, 3, 4}));

    std::vector<int> v5 = {7};
    rotateRight(v5, 5);
    assert((v5 == std::vector<int>{7}));

    std::vector<int> v6 = {1, 2, 3, 4, 5};
    rotateRight(v6, 7);
    assert((v6 == std::vector<int>{4, 5, 1, 2, 3}));

    std::vector<int> v7 = {1, 2};
    rotateRight(v7, 1);
    assert((v7 == std::vector<int>{2, 1}));
}
#include <vector>
#include <algorithm>

// Rotate the vector to the right by k positions, in place, using O(1) auxiliary space.
void rotateRight(std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (n == 0 || k == 0) {
        return;
    }
    k = k % n;
    if (k == 0) {
        return;
    }
    // reverse the first n-k elements
    std::reverse(nums.begin(), nums.begin() + n - k);
    // reverse the last k elements
    std::reverse(nums.begin() + n - k, nums.end());
    // reverse the entire vector
    std::reverse(nums.begin(), nums.end());
}
// The optimal solution uses three reverse operations. First, compute `k = k % n` where `n` is the vector size—this reduces redundant full rotations (e.g., rotating by `n` leaves the vector unchanged). Edge cases: if `n == 0` (not expected per spec) or `k == 0`, return early to avoid invalid indexing. For `n > 0` and `k > 0`, we reverse the first `n-k` elements, then reverse the last `k` elements, and finally reverse the entire vector. This works because reversing a subarray twice restores order, but the intermediate split effectively moves the last `k` elements to the front. Example: `[1,2,3,4,5]` with `k=2`: reverse first 3 → `[3,2,1,4,5]`, reverse last 2 → `[3,2,1,5,4]`, reverse all → `[4,5,1,2,3]`, which is correct. Time complexity is O(n) because each reverse is O(n), and we do three passes. Auxiliary space is O(1) since we only use a few integer variables for indices and the swap. The algorithm handles `k=0` (no change), `k=n` (after modulo becomes 0), and `n=1` (any `k` becomes 0 after modulo).
