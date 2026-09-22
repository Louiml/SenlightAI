Write a C++ function that takes a positive integer `limit` (greater than 1) and returns a `std::vector<int>` containing all odd integers from 1 up to (but not including) `limit`, in increasing order. For example, if `limit` is 10, the result should be `{1, 3, 5, 7, 9}`; if `limit` is 5, the result should be `{1, 3}`. If `limit` is 2, the result is `{1}` (since 1 is less than 2). The function must be pure (no console output), use proper `const` correctness, and handle any `limit` ≥ 2. Edge cases: `limit` must be at least 2 (you may assume valid input), and the function should work efficiently for large limits by avoiding unnecessary iterations.
#include <cassert>
#include <vector>

// Solution function (copied here for self-containment)
std::vector<int> oddNumbersUpTo(int limit) {
    std::vector<int> result;
    result.reserve(limit / 2);
    for (int value = 1; value < limit; value += 2) {
        result.push_back(value);
    }
    return result;
}

int main() {
    // Basic case: limit = 10 -> odds 1,3,5,7,9
    assert(oddNumbersUpTo(10) == std::vector<int>({1,3,5,7,9}));
    
    // Small limit: limit = 2 -> only 1
    assert(oddNumbersUpTo(2) == std::vector<int>({1}));
    
    // Odd limit: limit = 5 -> 1,3
    assert(oddNumbersUpTo(5) == std::vector<int>({1,3}));
    
    // limit = 3 -> only 1
    assert(oddNumbersUpTo(3) == std::vector<int>({1}));
    
    // Larger even limit
    assert(oddNumbersUpTo(20) == std::vector<int>({1,3,5,7,9,11,13,15,17,19}));
    
    // Consecutive limits: 4 and 6
    assert(oddNumbersUpTo(4) == std::vector<int>({1,3}));
    assert(oddNumbersUpTo(6) == std::vector<int>({1,3,5}));
    
    // Very small valid limit: 2 (already tested) and edge at 2
    assert(oddNumbersUpTo(100) == std::vector<int>({1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63,65,67,69,71,73,75,77,79,81,83,85,87,89,91,93,95,97,99}));
    
    return 0;
}
#include <vector>

// Return a vector containing all odd integers from 1 up to (but not including) limit.
std::vector<int> oddNumbersUpTo(int limit) {
    std::vector<int> result;
    result.reserve(limit / 2);  // Reserve upper bound for efficiency
    for (int value = 1; value < limit; value += 2) {
        result.push_back(value);
    }
    return result;
}
// The core algorithm is straightforward: iterate from 1 up to `limit-1` using a step of 2 to collect only odd numbers. This directly mirrors the pattern in the original `for` loop but generalizes the upper bound to a parameter. Start with an empty vector and push each odd value. Since we know the exact count in advance (ceil((limit-1)/2) or equivalently limit/2 when limit is even, (limit+1)/2 when odd), we could reserve capacity for efficiency, but it's not required. Edge cases: `limit` = 2 produces only `1`; `limit` = 3 produces `1` as well (since 3 is not included); `limit` = 4 produces `1,3`. No special handling needed for odd/even limits; the step of 2 naturally skips evens. Time complexity is O(limit) because we push about `limit/2` elements, each in O(1) amortized. Space complexity is O(limit/2) for the output vector, plus O(1) auxiliary.
