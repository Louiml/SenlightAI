// Given a vector of integers (possibly unsorted and containing duplicates), write a C++ function `int minOperations(const std::vector<int>& nums)` that returns the minimum number of operations needed to make all elements in the vector equal, where in each operation you can either remove three elements that have the same value or remove two elements that have the same value. If it is impossible to make all elements equal (i.e., some value appears only once and cannot be paired or tripled), return `-1`. For example, `[2,2,3,3,3,4,4,4,4]` requires 3 operations (one triplet of 3s and one triplet + one pair of 4s), while `[1,1,1,2,2]` requires 2 operations (one triplet of 1s and one pair of 2s). The function must not modify the input vector.
#include <cassert>
#include <vector>

// Function declaration (defined elsewhere in the solution section)
int minOperations(const std::vector<int>& nums);

int main() {
    // Empty vector: no operations needed (special case handled)
    assert(minOperations({}) == 0);
    
    // All same, count = 3: one triplet
    assert(minOperations({5,5,5}) == 1);
    
    // All same, count = 4: one triplet + one pair = 2 operations
    assert(minOperations({7,7,7,7}) == 2);
    
    // Count = 2: one pair
    assert(minOperations({1,1}) == 1);
    
    // Count = 6: two triplets
    assert(minOperations({2,2,2,2,2,2}) == 2);
    
    // Count = 8: two triplets + one pair = 3 operations
    assert(minOperations({3,3,3,3,3,3,3,3}) == 3);
    
    // Count = 1: impossible -> -1
    assert(minOperations({1}) == -1);
    
    // Mixed frequencies: {1:3, 2:2, 3:4} -> 1 + 1 + 2 = 4
    assert(minOperations({1,1,1,2,2,3,3,3,3}) == 4);
    
    // Mixed with a single occurrence -> -1
    assert(minOperations({4,4,4,5}) == -1);
    
    // Large frequency: count = 100 -> ceil(100/3) = 34
    std::vector<int> big(100, 9);
    assert(minOperations(big) == 34);
    
    // Duplicates spread out, order doesn't matter
    assert(minOperations({1,2,1,3,2,3,3,1}) == 3); // counts: 1:3, 2:2, 3:3 -> 1+1+1=3
    
    return 0;
}
#include <vector>
#include <unordered_map>
#include <cmath>

// Returns the minimum number of operations to remove all elements by removing
// groups of 2 or 3 identical values, or -1 if impossible.
int minOperations(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    
    int total_ops = 0;
    for (const auto& [value, count] : freq) {
        if (count == 1) {
            return -1; // cannot split a single occurrence into 2 or 3
        }
        // Minimum operations for count >= 2 is ceil(count / 3)
        total_ops += (count + 2) / 3; // integer ceiling division
    }
    return total_ops;
}
// The key insight is that the order of elements does not matter, only the frequency of each distinct value. For each distinct value with frequency `f`, we need to split `f` into a sum of 2's and 3's to minimize the number of parts (operations), because each operation removes either 2 or 3 identical elements. If a frequency is 1, it is impossible to split it into 2's and 3's, so we return -1. For `f >= 2`, the optimal strategy is to use as many 3's as possible because 3-element operations are more efficient. Mathematically, the minimum number of operations for frequency `f` is `ceil(f / 3)` when `f != 1` (e.g., `f=2` -> 1 operation, `f=3` -> 1 operation, `f=4` -> 2 operations, `f=5` -> 2 operations, `f=6` -> 2 operations). This is because any integer `f >= 2` can be expressed as `3*k + r` where `r` is 0, 1, or 2; if `r=0` we use `k` triplets; if `r=1` we replace one triplet (3) with two pairs (2+2) giving `k-1+2 = k+1` operations, which equals `ceil(f/3)`; if `r=2` we add one pair giving `k+1` operations, also `ceil(f/3)`. Edge case: `f=1` returns -1. The algorithm first counts frequencies using an unordered_map (O(n) time, O(n) space) or by sorting (O(n log n) time, O(1) extra space but modifies a copy). Then for each frequency, we check if it's 1 and sum `ceil(f/3)` for all others. Time complexity is O(n) with a hash map or O(n log n) with sorting; space complexity is O(n) for the hash map or O(1) extra if we copy and sort (but copying itself is O(n)). We'll use the hash map approach for clarity.
