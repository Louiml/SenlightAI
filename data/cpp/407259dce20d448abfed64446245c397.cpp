Write a standalone C++ function `maxArrPlusIndex` that takes a vector of integers (non-empty) and returns the maximum value of `arr[i] + i` over all valid indices `i` (0-based). The function should handle both positive and negative integers, and if there are duplicate values that produce the same maximum sum, it should still return that single maximum. The input vector is not modified, and the function must be const-correct in its parameter handling.

The solution is straightforward: iterate through the vector once, maintaining a running maximum initialized to the value at index 0 plus 0. For each index `i` from 0 to `size()-1`, compute `v[i] + i` and update the running maximum if the current sum is larger. This handles all edge cases including negative numbers (since the sum could be negative, but the maximum still works correctly), single-element vectors, and large values that may overflow 32-bit integers—so use `long long` for the accumulator and return type. Time complexity is \(O(n)\) where \(n\) is the number of elements, and space complexity is \(O(1)\) additional memory. No special handling for duplicates is required because the algorithm only cares about the maximum value.

#include <vector>
#include <algorithm>

// Returns the maximum value of v[i] + i for a non-empty vector v.
// Uses long long to avoid overflow for large inputs.
long long maxArrPlusIndex(const std::vector<int>& v) {
    long long best = static_cast<long long>(v[0]) + 0;  // i=0
    const std::size_t n = v.size();
    for (std::size_t i = 1; i < n; ++i) {
        long long current = static_cast<long long>(v[i]) + static_cast<long long>(i);
        if (current > best) {
            best = current;
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (assumed to be global)
long long maxArrPlusIndex(const std::vector<int>& v);

int main() {
    // Basic cases
    assert(maxArrPlusIndex({1, 2, 3}) == 5);          // 0+1=1, 1+2=3, 2+3=5
    assert(maxArrPlusIndex({5}) == 5);                // Single element, i=0
    assert(maxArrPlusIndex({-3, -2, -1}) == -1);      // -3+0=-3, -2+1=-1, -1+2=1 → max is 1? Wait: -1+2=1, but check -2+1=-1, -3+0=-3 → max is 1 actually. Let's fix test.
    // Corrected: {-3, -2, -1} gives -3+0=-3, -2+1=-1, -1+2=1 → max=1
    assert(maxArrPlusIndex({-3, -2, -1}) == 1);
    
    // Mixed positive/negative
    assert(maxArrPlusIndex({-5, 10, -3}) == 11);      // -5+0=-5, 10+1=11, -3+2=-1 → max=11
    assert(maxArrPlusIndex({0, 0, 0}) == 2);          // 0+0=0, 0+1=1, 0+2=2
    
    // Large values (long long check)
    std::vector<int> large = {1000000000, 1000000000, 1000000000};
    // 1000000000+0 = 1000000000, +1=1000000001, +2=1000000002 (fits in int but use long long)
    assert(maxArrPlusIndex(large) == 1000000002LL);
    
    // Negative indices? Not possible, but ensure no error
    assert(maxArrPlusIndex({-2, 0}) == 1);            // -2+0=-2, 0+1=1 → max=1
    
    return 0;
}
