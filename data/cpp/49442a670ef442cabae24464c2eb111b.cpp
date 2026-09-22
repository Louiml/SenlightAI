Write a C++ function `void rearrangeAlternating(std::vector<int>& nums)` that takes a vector of integers containing an equal number of positive and negative values (all non-zero; zero is not present) and rearranges them in-place so that the resulting array alternates between positive and negative numbers, starting with a positive number. The relative order of the positive numbers among themselves must be preserved, and the relative order of the negative numbers among themselves must also be preserved. The function should modify the input vector directly and not return a new vector. For example, given `{3,1,-2,-5,2,-4}`, the result should be `{3,-2,1,-5,2,-4}`. The input is guaranteed to be valid (equal counts, no zeros).
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: basic example from the prompt
    std::vector<int> v1 = {3,1,-2,-5,2,-4};
    rearrangeAlternating(v1);
    assert(v1 == std::vector<int>({3,-2,1,-5,2,-4}));
    
    // Test 2: alternating already starting with positive
    std::vector<int> v2 = {1,-1,2,-2,3,-3};
    rearrangeAlternating(v2);
    assert(v2 == std::vector<int>({1,-1,2,-2,3,-3}));
    
    // Test 3: all positives first then all negatives
    std::vector<int> v3 = {5,6,7,-1,-2,-3};
    rearrangeAlternating(v3);
    assert(v3 == std::vector<int>({5,-1,6,-2,7,-3}));
    
    // Test 4: all negatives first then all positives
    std::vector<int> v4 = {-10,-20,-30,1,2,3};
    rearrangeAlternating(v4);
    assert(v4 == std::vector<int>({1,-10,2,-20,3,-30}));
    
    // Test 5: single pair
    std::vector<int> v5 = {9,-9};
    rearrangeAlternating(v5);
    assert(v5 == std::vector<int>({9,-9}));
    
    // Test 6: repeated values preserving order
    std::vector<int> v6 = {4,4,-5,-5,6,-6};
    rearrangeAlternating(v6);
    assert(v6 == std::vector<int>({4,-5,4,-5,6,-6}));
    
    // Test 7: larger random-looking but deterministic
    std::vector<int> v7 = {2, -3, 8, -1, 5, -7, 10, -11};
    rearrangeAlternating(v7);
    assert(v7 == std::vector<int>({2,-3,8,-1,5,-7,10,-11}));
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>

// Rearranges the input vector in-place so that it alternates between positive
// and negative numbers, starting with a positive. Preserves relative order of
// positives and negatives separately. Assumes equal counts and no zeros.
void rearrangeAlternating(std::vector<int>& nums) {
    int positiveIndex = 0;  // next even index for positive
    int negativeIndex = 1;  // next odd index for negative
    
    // Scan through original sequence; place each element in correct slot.
    for (size_t i = 0; i < nums.size(); ++i) {
        int value = nums[i];
        if (value > 0) {
            nums[positiveIndex] = value;
            positiveIndex += 2;
        } else { // value < 0 (guaranteed non-zero)
            nums[negativeIndex] = value;
            negativeIndex += 2;
        }
    }
}
// The algorithm uses two index pointers: one for the next positive position and one for the next negative position. Since the result must start with a positive number, the positive pointer starts at index 0 and the negative pointer starts at index 1. We iterate through the original vector once. For each element, if it is positive, we place it at the current positive pointer position (which is the next available even index) and increment the positive pointer by 2. If the element is negative, we place it at the current negative pointer position (the next available odd index) and increment the negative pointer by 2. Because the input has equal counts of positives and negatives, both pointers will finish exactly at the end of the vector without exceeding bounds. This preserves relative order because we process elements in their original sequence and append each to the correct alternating slot in the output. The main edge case is ensuring the input always has equal counts (guaranteed by the problem), and that no zero is present. If there were unequal counts, the pointers would go out of bounds, but that is not a concern here. The algorithm runs in O(n) time, as it makes a single pass over the input, and uses O(1) auxiliary space beyond the input vector itself (since we rearrange in-place).
