// Write a C++ function `maximumProduct` that takes a non-empty vector of integers and returns the maximum product that can be obtained by multiplying exactly three distinct elements from the vector. The input may contain negative numbers, zeros, and duplicates, and the vector length is at least 3. The function must handle all possible sign combinations (e.g., two large negatives with a positive, three negatives, three positives, and cases involving zeros) correctly. The return value must fit within the range of a 32-bit signed integer.

#include <cassert>
#include <vector>

// The solution function is declared above; assume it is included.

int main() {
    std::vector<int> v1 = {1, 2, 3};
    assert(maximumProduct(v1) == 6);

    std::vector<int> v2 = {1, 2, 3, 4};
    assert(maximumProduct(v2) == 24);

    std::vector<int> v3 = {-1, -2, -3};
    assert(maximumProduct(v3) == -6);

    std::vector<int> v4 = {-100, -98, -1, 2, 3, 4};
    assert(maximumProduct(v4) == 39200); // (-100)*(-98)*4 = 39200

    std::vector<int> v5 = {-10, -10, 5, 2};
    assert(maximumProduct(v5) == 500); // (-10)*(-10)*5 = 500

    std::vector<int> v6 = {-5, -1, 0, 3};
    assert(maximumProduct(v6) == 15); // (-5)*(-1)*3 = 15, or 0*... but max is 15

    std::vector<int> v7 = {0, 0, 0};
    assert(maximumProduct(v7) == 0);

    std::vector<int> v8 = {1, 1, 1};
    assert(maximumProduct(v8) == 1);

    std::vector<int> v9 = {-2, 0, 1};
    assert(maximumProduct(v9) == 0); // (-2)*0*1 = 0, (-2)*1*0 = 0, etc.

    std::vector<int> v10 = {9, 8, 7, -1, -2};
    assert(maximumProduct(v10) == 504); // 9*8*7 = 504, (-2)*(-1)*9 = 18
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum product of any three distinct elements in nums.
// nums must contain at least 3 elements.
int maximumProduct(std::vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
    int n = static_cast<int>(nums.size());
    // Product of the three largest numbers
    int candidate1 = nums[n-1] * nums[n-2] * nums[n-3];
    // Product of the two smallest (possibly negative) and the largest
    int candidate2 = nums[0] * nums[1] * nums[n-1];
    return std::max(candidate1, candidate2);
}

// The key insight is that the maximum product of three numbers is either:
// 1. The product of the three largest numbers in the vector (which covers the case where all numbers are positive or where the largest three include a mix that yields the highest product), or
// 2. The product of the two smallest numbers (which, if negative, are the two with the largest absolute values among negatives) and the largest number. This handles scenarios where two negatives multiply to a positive and then multiply by the largest positive, potentially exceeding the product of the three largest (for example, `[-100, -98, 1, 2, 3]` yields `(-100)*(-98)*3 = 29400` vs `3*2*1 = 6`).
//
// Edge cases include:
// - All numbers are negative: the largest product is the product of the three smallest in magnitude (i.e., the three largest after sorting), which yields a negative result; the two-negative-plus-largest formula would also produce the same because the largest number is the least negative.
// - Presence of zeros: if zeros are present, products involving zeros become zero, which may be better than a negative product. But the algorithm still works because both candidates will naturally include zero at least in one of the products, and we take the maximum.
// - Duplicates: sorting handles them fine; just pick indices accordingly.
//
// Sort the array in ascending order. Let `n` be its size. Candidate A = `arr[n-1]*arr[n-2]*arr[n-3]` (product of three largest). Candidate B = `arr[0]*arr[1]*arr[n-1]` (product of two smallest and the largest). The answer is the maximum of these two. This works because any other combination of three elements cannot exceed both candidates (due to the monotonic order of sorted values). Time complexity is O(n log n) due to sorting, space O(1) excluding input storage.
