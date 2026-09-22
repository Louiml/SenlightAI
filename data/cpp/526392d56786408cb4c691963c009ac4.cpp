// Given an integer array `nums` of length `2 * n` that represents pairs `(x1, y1), (x2, y2), ..., (xn, yn)`, write a C++ function that returns a new array in the shuffled order `[x1, y1, x2, y2, ..., xn, yn]`. The original array contains the first `n` elements as the `x` values and the next `n` elements as the corresponding `y` values. The function should take the vector and the integer `n` as parameters and return a new vector with the shuffled elements. Assume `n >= 1` and the input vector size is exactly `2 * n`. Do not modify the input vector.

The solution constructs the result by iterating over the output positions from index `0` to `2*n - 1`. For each even index in the result, we take the next unused element from the first half of the input (indices `0` to `n-1`); for each odd index, we take the next unused element from the second half (indices `n` to `2n-1`). This can be done with two pointers: one for the first half starting at 0, and one for the second half starting at `n`. When the output index is even, we push the element at the first pointer and increment it; when odd, we push the element at the second pointer and increment it. Edge cases: if `n = 1`, the input has exactly two elements, and the output is simply `[nums[0], nums[1]]`, which the algorithm handles naturally. The algorithm runs in `O(n)` time because it processes each element exactly once, and it uses `O(n)` auxiliary space for the result vector (plus a constant amount for pointers). The input is not modified, so `const` correctness is respected by taking a `const` reference.

#include <vector>

// Shuffle an array of pairs [x1, y1, x2, y2, ...] into [x1, y1, x2, y2, ...].
// The input has length 2*n where first n are x's and last n are y's.
std::vector<int> shufflePairs(const std::vector<int>& nums, int n) {
    std::vector<int> result;
    result.reserve(2 * n);
    
    int firstHalfIndex = 0;       // index into the x-part (0..n-1)
    int secondHalfIndex = n;      // index into the y-part (n..2n-1)
    
    for (int i = 0; i < 2 * n; ++i) {
        if (i % 2 == 0) {
            result.push_back(nums[firstHalfIndex]);
            ++firstHalfIndex;
        } else {
            result.push_back(nums[secondHalfIndex]);
            ++secondHalfIndex;
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example: [1,2,3,4] with n=2 -> [1,3,2,4]
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(shufflePairs(v1, 2) == std::vector<int>({1, 3, 2, 4}));
    
    // n=1 case: [a,b] -> [a,b]
    std::vector<int> v2 = {5, 6};
    assert(shufflePairs(v2, 1) == std::vector<int>({5, 6}));
    
    // Larger example: [1,2,3,4,5,6] n=3 -> [1,4,2,5,3,6]
    std::vector<int> v3 = {1, 2, 3, 4, 5, 6};
    assert(shufflePairs(v3, 3) == std::vector<int>({1, 4, 2, 5, 3, 6}));
    
    // Values with negatives: [-1, -2, -3, -4] n=2 -> [-1, -3, -2, -4]
    std::vector<int> v4 = {-1, -2, -3, -4};
    assert(shufflePairs(v4, 2) == std::vector<int>({-1, -3, -2, -4}));
    
    // Zeros and repeated values: [0, 0, 1, 1, 2, 2] n=3 -> [0,1,0,2,1,2]
    std::vector<int> v5 = {0, 0, 1, 1, 2, 2};
    assert(shufflePairs(v5, 3) == std::vector<int>({0, 1, 0, 2, 1, 2}));
    
    return 0;
}
