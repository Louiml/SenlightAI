/*
Write a C++ function named `findMaxConsecutiveSum` that takes a constant reference to a `std::vector<int>` and an integer `k`. The function should return the maximum sum obtainable by selecting `k` consecutive elements from the vector. If `k` is greater than the size of the vector or if `k` is non-positive, return 0. The vector may contain negative numbers, zeros, and positive numbers. The function must not modify the input container and must be efficient for large inputs.
*/

#include <vector>
#include <algorithm>

// Returns the maximum sum of any k consecutive elements.
// Returns 0 if k is non-positive or larger than the vector size.
int findMaxConsecutiveSum(const std::vector<int>& vec, int k) {
    const std::size_t n = vec.size();
    if (k <= 0 || static_cast<std::size_t>(k) > n) {
        return 0;
    }

    // Compute initial window sum
    int current_sum = 0;
    for (int i = 0; i < k; ++i) {
        current_sum += vec[static_cast<std::size_t>(i)];
    }

    int max_sum = current_sum;

    // Slide the window
    for (std::size_t i = static_cast<std::size_t>(k); i < n; ++i) {
        current_sum += vec[i] - vec[i - static_cast<std::size_t>(k)];
        max_sum = std::max(max_sum, current_sum);
    }

    return max_sum;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (assume it is included from above)
int findMaxConsecutiveSum(const std::vector<int>& vec, int k);

int main() {
    // Basic positive numbers
    assert(findMaxConsecutiveSum({1, 2, 3, 4}, 2) == 7); // 3+4
    // Negative numbers
    assert(findMaxConsecutiveSum({-1, -2, -3, -4}, 2) == -3); // -1+-2
    // Mixed values
    assert(findMaxConsecutiveSum({5, -2, 9, 1, -7, 3}, 3) == 12); // 9+1+2? Actually 9+1+(-7)=3, 5+(-2)+9=12, so max=12
    // k = 1
    assert(findMaxConsecutiveSum({4, 0, 9, -1}, 1) == 9);
    // k equals vector size
    assert(findMaxConsecutiveSum({10, 20, 30}, 3) == 60);
    // k larger than size -> return 0
    assert(findMaxConsecutiveSum({1, 2}, 3) == 0);
    // k = 0 -> return 0
    assert(findMaxConsecutiveSum({1, 2, 3}, 0) == 0);
    // k negative -> return 0
    assert(findMaxConsecutiveSum({1, 2}, -1) == 0);
    // Single element with k=1
    assert(findMaxConsecutiveSum({-7}, 1) == -7);
    // All zeros
    assert(findMaxConsecutiveSum({0, 0, 0, 0}, 2) == 0);

    return 0;
}

// The optimal approach uses a sliding window technique. First, handle edge cases: if `k <= 0` or `k > vector.size()`, return 0. Then compute the sum of the first `k` elements and store it as the initial maximum. Slide the window from index `k` to the end of the vector: for each new index `i`, add `vec[i]` and subtract `vec[i - k]` to update the current window sum in constant time. After each slide, compare the current sum with the stored maximum and update if larger. This avoids repeatedly summing overlapping subsets. Correctness is immediate because every possible window of length `k` is visited exactly once. Time complexity is O(n), where n is the size of the vector, because each element is added once and subtracted once. Space complexity is O(1) besides the input itself.
