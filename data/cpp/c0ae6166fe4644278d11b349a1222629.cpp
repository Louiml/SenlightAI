/*
Given an array of `4n` positive integers, write a C++ function `bool canFormRectanglePairs(const std::vector<int>& nums)` that determines whether the integers can be paired such that each pair has the same product, and every pair consists of two distinct integers (i.e., the product of each pair is equal across all pairs). The function should return `true` if such a pairing is possible, and `false` otherwise. The input size `n` is implicitly `nums.size() / 4`, and `n` is a positive integer. All integers are between 1 and 10000 inclusive. The function must handle duplicate values correctly: each value can be used at most as many times as it appears in the input, and each integer must be used exactly once across all pairs.
*/
#include <vector>
#include <algorithm>
#include <numeric>

// Given a vector of 4n positive integers (1..10000), determine if they can be
// paired into n pairs such that each pair has the same product.
// The function returns true if possible, false otherwise.
bool canFormRectanglePairs(const std::vector<int>& nums) {
    // Count frequencies of each value (values are in 1..10000)
    std::vector<int> freq(10001, 0);
    for (int x : nums) {
        ++freq[x];
    }
    
    // Check parity: every frequency must be even
    for (int i = 1; i <= 10000; ++i) {
        if (freq[i] % 2 != 0) {
            return false;
        }
        freq[i] /= 2; // halve each frequency
    }
    
    // Build a sorted list of all values, each appearing freq[i] times
    std::vector<int> values;
    for (int i = 1; i <= 10000; ++i) {
        for (int j = 0; j < freq[i]; ++j) {
            values.push_back(i);
        }
    }
    
    // If after halving the list is empty (should not happen for valid n), return false
    if (values.empty()) {
        return false;
    }
    
    // The product of the smallest and largest must be constant
    long long target = static_cast<long long>(values.front()) * values.back();
    int left = 0;
    int right = static_cast<int>(values.size()) - 1;
    
    while (left < right) {
        long long prod = static_cast<long long>(values[left]) * values[right];
        if (prod != target) {
            return false;
        }
        ++left;
        --right;
    }
    
    return true;
}
#include <cassert>
#include <vector>

// Function declaration (provided by the solution)
bool canFormRectanglePairs(const std::vector<int>& nums);

int main() {
    // Basic valid case: 4 numbers, pairs (1,12) and (2,6) both product 12? Actually 1*12=12, 2*6=12, but we need 4n=4 numbers -> n=1, so only one pair? Wait 4n means n=1 gives 4 numbers, but we need pairs = n, so n=1 means 2 pairs? Actually 4n = 4, so n=1 meaning 1 pair? That's inconsistent. Let's reconsider: The problem says "given 4n positive integers" and we need to pair them into n pairs? Actually the intended is: we have 4n integers, we want to form n rectangles? Wait the snippet implies we have 4n sticks, and we need to form n rectangles? But the code pairs them into 2n pairs, then checks if the product of each pair is equal. Actually the code takes 4n numbers, checks even frequencies, then forms a list of half the size (2n numbers), and pairs them using two pointers into n pairs. So we need 2n numbers after halving, and we pair them into n pairs. So the input size is 4n, and we pair into n pairs. For n=1, input size is 4, we have 2 numbers after halving, we make 1 pair. For n=2, input size 8, we have 4 numbers after halving, 2 pairs.
    
    // Valid case: n=1, input [1,1,12,12] -> after halving [1,12], product 12, true
    assert(canFormRectanglePairs({1,1,12,12}) == true);
    
    // Valid case: n=2, input [1,1,2,2,3,3,6,6] -> frequencies: 1:2,2:2,3:2,6:2 -> halved [1,2,3,6] -> pairs (1,6)=6, (2,3)=6, true
    assert(canFormRectanglePairs({1,1,2,2,3,3,6,6}) == true);
    
    // Invalid: odd frequency -> n=1, input [1,1,2,3] -> freq of 2 odd -> false
    assert(canFormRectanglePairs({1,1,2,3}) == false);
    
    // Invalid: even frequencies but product mismatch -> input [1,1,2,2,4,4,8,8] -> halved [1,2,4,8] -> target=8, (1,8)=8, (2,4)=8 ok? Wait 1*8=8, 2*4=8, that's true. Need a failure example: [1,1,2,2,3,3,4,4] -> halved [1,2,3,4] -> target=4, (1,4)=4, (2,3)=6 mismatch -> false
    assert(canFormRectanglePairs({1,1,2,2,3,3,4,4}) == false);
    
    // Valid with repetition: [2,2,2,2,9,9,9,9] -> frequencies 2:4,9:4 -> halved [2,2,9,9] -> target=18, (2,9)=18, (2,9)=18 true
    assert(canFormRectanglePairs({2,2,2,2,9,9,9,9}) == true);
    
    // Edge: all same number, e.g., [5,5,5,5] -> halved [5,5] -> target=25, true
    assert(canFormRectanglePairs({5,5,5,5}) == true);
    
    // Invalid: all same but odd count? [5,5,5] not a valid input size (must be multiple of 4), we can test with size 4 anyway, but that's fine.
    // Another invalid: [7,7,8,8] -> halved [7,8] -> product 56, only one pair, true? Actually that's valid because n=1, we have 2 numbers after halving, pair (7,8) product 56, true. Wait but we need 4n numbers, here n=1, we have 4 numbers, ok.
    assert(canFormRectanglePairs({7,7,8,8}) == true);
    
    // Invalid: [1,1,2,2,3,3] has only 6 numbers, not a multiple of 4, but function assumes valid input size; we won't test that.
    
    // More complex valid: [1,1,2,2,4,4,8,8,16,16,32,32] (n=3, 12 numbers) -> halved [1,2,4,8,16,32] -> target=32, pairs (1,32)=32, (2,16)=32, (4,8)=32 true
    assert(canFormRectanglePairs({1,1,2,2,4,4,8,8,16,16,32,32}) == true);
    
    return 0;
}
// The key observation is that if the integers can be paired with equal product, then the total number of integers must be even, and each integer’s frequency must be even because each occurrence is paired with another integer. So first count the frequency of each value. If any frequency is odd, return `false`. If all frequencies are even, then each value can be split into half as many copies for the sorted list of values. After halving frequencies, we obtain a multiset of `2n` integers where each integer appears an even number of times in the original, but now we need to pair them with equal product. The product of the smallest and largest remaining values must equal the product of the next smallest and next largest, and so on. So sort the values (with halved frequencies) into a vector `S`. Then set `target = S[0] * S[S.size()-1]`. Iterate from both ends inward, comparing `S[i] * S[j]` to `target`; if any mismatch, return `false`. If all match, return `true`. Edge cases: `n` must be positive, and the vector size must be exactly `4n`; the function can assume that validity. Time complexity is O(V + K log K) where V is the value range (10000) and K is the number of unique values or the total size after halving, but in practice O(10000 + M log M) where M ≤ 20000. Space complexity O(10000 + M).
