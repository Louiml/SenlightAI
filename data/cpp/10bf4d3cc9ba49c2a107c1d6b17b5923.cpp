// Write a C++ function that takes an array of integers and its size, and returns the smallest non-negative integer that is not present in the array. The function must handle arrays containing negative numbers, duplicates, and values up to 1,000,000. If all non-negative integers from 0 up to some bound are present, the answer should be the next integer after the maximum non-negative present. The function should efficiently find this missing number without sorting the array.
The solution uses a boolean lookup table to mark which non-negative numbers are present. Since the problem guarantees array values up to 1,000,000, we allocate a static boolean array of size `1e6+2` (one extra for safety). We iterate through the input array; for each element that is non-negative, we set the corresponding index in the lookup table to `true`. After marking, we scan the lookup table from index 0 upward and return the first index where the value is `false`. If all indices from 0 to 1,000,000 are marked, the loop will naturally reach index `1,000,001` which is `false` (since we only mark up to `1e6`), giving the correct answer. Edge cases include arrays with only negative numbers (answer is 0), arrays with duplicates (no special handling needed), and arrays containing all numbers from 0 to 1,000,000 (answer is 1,000,001). Time complexity is O(n + M) where n is the array size and M is the constant 1,000,001, which simplifies to O(n) since M is fixed. Space complexity is O(M) = O(1) in practice because M is a constant.
#include <vector>
#include <cstddef>

// Returns the smallest non-negative integer not present in the input array.
// The input array may contain negative numbers, duplicates, and values up to 1e6.
int smallestMissingNumber(const std::vector<int>& arr) {
    constexpr int MAX_VAL = 1000000;
    constexpr int CHECK_SIZE = MAX_VAL + 2; // +1 to allow answer MAX_VAL+1
    
    bool present[CHECK_SIZE] = {false}; // all initialized to false
    
    // Mark all non-negative values that appear in the array.
    for (int value : arr) {
        if (value >= 0 && value <= MAX_VAL) {
            present[value] = true;
        }
    }
    
    // Find the first index not marked.
    for (int i = 0; i < CHECK_SIZE; ++i) {
        if (!present[i]) {
            return i;
        }
    }
    
    // This point is unreachable because CHECK_SIZE includes one extra slot.
    return CHECK_SIZE;
}
#include <cassert>
#include <vector>

// The solution function declaration (must match the provided implementation).
int smallestMissingNumber(const std::vector<int>& arr);

int main() {
    // Basic case with positive and negative numbers.
    assert(smallestMissingNumber({0, 1, 2, 3, 4}) == 5);
    assert(smallestMissingNumber({5, 1, 3, 0}) == 2);
    assert(smallestMissingNumber({-1, -2, -3}) == 0);
    
    // Duplicates and missing zero.
    assert(smallestMissingNumber({1, 2, 1, 2, 3}) == 0);
    
    // Missing a middle number.
    assert(smallestMissingNumber({0, 2, 3, 4}) == 1);
    
    // Large values but not all present.
    assert(smallestMissingNumber({1000000, 999999}) == 0);
    
    // All numbers from 0 to 1000000 present (edge case).
    std::vector<int> allNumbers;
    for (int i = 0; i <= 1000000; ++i) {
        allNumbers.push_back(i);
    }
    assert(smallestMissingNumber(allNumbers) == 1000001);
    
    // Mixed with negatives and duplicates.
    assert(smallestMissingNumber({0, 0, -1, 1, -2, 2, 3}) == 4);
    
    return 0;
}
