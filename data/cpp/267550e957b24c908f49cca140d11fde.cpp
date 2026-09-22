Write a standalone C++ function `maximumSubarrayValue(const std::vector<long long>& nums)` that, given a non-empty vector of integers (which may contain negative numbers, zero, and duplicates), returns the **maximum sum of any contiguous subarray** (Kadane’s algorithm). The function must handle inputs where all numbers are negative, where the best subarray is a single element, and where the array length is large (up to 2e5). The function should be `long long` because sums may exceed 32-bit integer limits. You are not allowed to use any external libraries beyond the standard C++ headers. The solution must be efficient enough for large vectors and should not modify the input vector.
#include <cassert>
#include <vector>
using namespace std;

int main() {
    // Mixed positive and negative
    assert(maximumSubarrayValue({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // All negative
    assert(maximumSubarrayValue({-1, -2, -3}) == -1);
    // Single element
    assert(maximumSubarrayValue({5}) == 5);
    // All positive
    assert(maximumSubarrayValue({1, 2, 3, 4}) == 10);
    // Contains zero
    assert(maximumSubarrayValue({-1, 0, -2}) == 0);
    // Large positive sum (uses long long)
    assert(maximumSubarrayValue({1000000000LL, 1000000000LL, 1000000000LL}) == 3000000000LL);
    // Duplicate negatives
    assert(maximumSubarrayValue({-5, -5, -5}) == -5);
    // Alternating signs with best subarray in middle
    assert(maximumSubarrayValue({2, -1, 2, -1, 2}) == 4);
    // Single negative
    assert(maximumSubarrayValue({-7}) == -7);
    // Mixed with zero at start
    assert(maximumSubarrayValue({0, -3, 1, 1}) == 2);
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum sum of any contiguous subarray.
long long maximumSubarrayValue(const std::vector<long long>& nums) {
    long long current_sum = nums[0];
    long long best_sum = nums[0];
    
    for (size_t i = 1; i < nums.size(); ++i) {
        current_sum = std::max(nums[i], current_sum + nums[i]);
        best_sum = std::max(best_sum, current_sum);
    }
    
    return best_sum;
}
// The main algorithm is Kadane’s algorithm, which uses dynamic programming to keep track of the maximum subarray sum ending at each position. We maintain two variables: `current_sum` (the maximum sum of a subarray ending at the current index) and `best_sum` (the maximum sum found so far). For each element, we update `current_sum = max(element, current_sum + element)`. This handles the case where adding the current element to the previous subarray is beneficial versus starting a new subarray at the current element. Then we update `best_sum = max(best_sum, current_sum)`. Edge cases: (1) If all numbers are negative, the algorithm will correctly pick the least negative (closest to zero) as the best subarray, since starting fresh at each element yields the element itself. (2) If the array has one element, return that element. (3) Large sums may require `long long`. Time complexity is \(O(n)\) because we iterate through the array once. Space complexity is \(O(1)\) because we only use two scalar variables, and we do not copy the vector.
