Write a C++ function named `moveAllZeroesToEnd` that takes a `std::vector<int>&` and modifies it in-place so that all non-zero elements appear in their original relative order, followed by all zero elements. The function must not use any additional array or vector storage; it should operate with only constant extra space. The order of the non-zero elements must be preserved, and all zeroes must be moved to the end without changing the relative order of non-zero elements. The function should handle vectors of any size, including empty vectors, vectors with all zeros, all non-zeros, and mixed values (including negative numbers). The function must return `void` and must be `const`-correct in the sense that it does not modify the vector if it is passed as a `const` reference, but since it modifies in-place, it should take a non-const reference. Provide a self-contained implementation with appropriate headers.
#include <cassert>
#include <vector>

// Declaration of the function under test (assumed to be defined elsewhere)
void moveAllZeroesToEnd(std::vector<int>& nums);

int main() {
    // Test 1: Mixed zeros and non-zeros
    std::vector<int> v1 = {0, 1, 0, 3, 12};
    moveAllZeroesToEnd(v1);
    assert(v1 == std::vector<int>({1, 3, 12, 0, 0}));

    // Test 2: All zeros
    std::vector<int> v2 = {0, 0, 0};
    moveAllZeroesToEnd(v2);
    assert(v2 == std::vector<int>({0, 0, 0}));

    // Test 3: No zeros
    std::vector<int> v3 = {5, -2, 4};
    moveAllZeroesToEnd(v3);
    assert(v3 == std::vector<int>({5, -2, 4}));

    // Test 4: Empty vector
    std::vector<int> v4;
    moveAllZeroesToEnd(v4);
    assert(v4.empty());

    // Test 5: Single zero
    std::vector<int> v5 = {0};
    moveAllZeroesToEnd(v5);
    assert(v5 == std::vector<int>({0}));

    // Test 6: Single non-zero
    std::vector<int> v6 = {42};
    moveAllZeroesToEnd(v6);
    assert(v6 == std::vector<int>({42}));

    // Test 7: Zeros at the beginning and end
    std::vector<int> v7 = {0, 0, 1, 2, 0, 3};
    moveAllZeroesToEnd(v7);
    assert(v7 == std::vector<int>({1, 2, 3, 0, 0, 0}));

    // Test 8: Negative numbers with zeros
    std::vector<int> v8 = {-1, 0, -2, 0, 0, -3};
    moveAllZeroesToEnd(v8);
    assert(v8 == std::vector<int>({-1, -2, -3, 0, 0, 0}));

    // Test 9: Large vector with alternating zeros (simulate briefly)
    std::vector<int> v9(1000, 0);
    for (int i = 0; i < 1000; i += 2) v9[i] = i + 1;  // 1,0,3,0,5,...
    moveAllZeroesToEnd(v9);
    for (int i = 0; i < 500; ++i) {
        assert(v9[i] == 2 * i + 1);
    }
    for (int i = 500; i < 1000; ++i) {
        assert(v9[i] == 0);
    }

    return 0;
}
#include <vector>

// Moves all zeroes to the end of the vector in-place, preserving the relative order of non-zero elements.
// The function uses O(1) extra space and runs in O(n) time.
void moveAllZeroesToEnd(std::vector<int>& nums) {
    int i = 0;  // position to place the next non-zero element
    for (int j = 0; j < static_cast<int>(nums.size()); ++j) {
        if (nums[j] != 0) {
            std::swap(nums[i], nums[j]);
            ++i;
        }
    }
}
// The optimal solution uses a two-pointer (or two-index) technique. Maintain an index `i` that points to the position where the next non-zero element should be placed. Initialize `i = 0`. Iterate through the vector with another index `j` from `0` to `n-1`. For each element `nums[j]`, if it is non-zero, swap `nums[i]` and `nums[j]`, then increment `i`. This effectively moves all non-zero elements to the front in their original relative order, while zeros naturally get pushed to the end. The swap is safe even when `i == j` (swapping an element with itself is a no-op). After the loop, all positions from `i` to `n-1` will contain zeros because every non-zero encountered has been placed at or before position `i-1`, and zeros were swapped to the right. Edge cases: an empty vector — the loop does nothing and the function returns. A vector with all zeros — no swaps occur because `nums[j]` is always zero, and `i` stays `0`. A vector with no zeros — the loop swaps each element with itself (when `i == j`), and the vector remains unchanged in order. Negative numbers are treated as non-zero and handled correctly. Time complexity is O(n) because each element is visited once. Space complexity is O(1) extra space, aside from the input vector itself, because we only use a few integer variables.
