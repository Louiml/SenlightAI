Write a C++ function named `maxNumberOfPairs` that takes a vector of integers `nums` and an integer `k`, and returns the maximum number of pairs `(nums[i], nums[j])` such that `i < j` and `nums[i] + nums[j] == k`. Each element can be used in at most one pair. The vector may contain duplicates, negative numbers, zero, and its size can be zero. The order of elements matters only for pairing uniqueness; you may process elements in any order. Return the count of non-overlapping valid pairs.
The solution uses a hash map (`unordered_map`) to track how many times each value has been seen but not yet paired. Iterate through the array once. For each number `num`, compute the complement `complement = k - num`. If there is a stored count of that complement (i.e., `map[complement] > 0`), then a pair can be formed: increment the answer, and decrement the count of the complement (because one instance of it is now used). Otherwise, increment the count of the current `num` in the map. This greedy pairing works because each element is considered exactly once, and when a match is found, it pairs the current element with an earlier unmatched element, which is optimal (no need to look ahead). Edge cases: (1) empty vector returns 0; (2) if `k` is even and `num == complement` (i.e., `k == 2*num`), the same logic works because the map count handles multiple occurrences—when the first `num` is seen, it is stored; when the second `num` is seen, its complement matches the stored value, so they pair; (3) negative numbers and zeros are handled naturally by integer arithmetic; (4) duplicates are fine because each occurrence is stored and consumed exactly once. Time complexity is O(n) where n is the size of the vector, because each element is processed once and hash map operations average O(1). Space complexity is O(n) in the worst case (all elements distinct and unpaired). The function uses `const` reference for the input vector to avoid copying.
#include <vector>
#include <unordered_map>

// Returns the maximum number of non-overlapping pairs whose sum equals k.
int maxNumberOfPairs(const std::vector<int>& nums, int k) {
    int pairCount = 0;
    std::unordered_map<int, int> frequency; // value -> count of unpaired occurrences

    for (int num : nums) {
        int complement = k - num;
        if (frequency[complement] > 0) {
            // Found a match: pair current num with an existing complement.
            ++pairCount;
            --frequency[complement];
        } else {
            // No match yet, store this num for potential future pairing.
            ++frequency[num];
        }
    }

    return pairCount;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic cases
    assert(maxNumberOfPairs({1, 2, 3, 4}, 5) == 2); // (1,4) and (2,3)
    assert(maxNumberOfPairs({3, 1, 3, 4, 3}, 6) == 1); // only (3,3)
    assert(maxNumberOfPairs({1, 2, 3, 4, 5}, 7) == 2); // (2,5), (3,4)
    
    // Duplicates and self-pairing
    assert(maxNumberOfPairs({2, 2, 2, 2}, 4) == 2); // two pairs of (2,2)
    assert(maxNumberOfPairs({1, 1, 1}, 2) == 1); // only one pair from three ones
    assert(maxNumberOfPairs({5, 5, 5, 5}, 10) == 2);
    
    // Negative numbers and zeros
    assert(maxNumberOfPairs({-1, -2, 3, 4}, 2) == 2); // (-1,3), (-2,4)
    assert(maxNumberOfPairs({0, 0, 0}, 0) == 1); // pair of zeros
    assert(maxNumberOfPairs({-3, 3, -3, 3}, 0) == 2);
    
    // Edge cases
    assert(maxNumberOfPairs({}, 5) == 0);
    assert(maxNumberOfPairs({1}, 1) == 0); // single element
    assert(maxNumberOfPairs({1, 1}, 3) == 0); // no valid pair
    assert(maxNumberOfPairs({10, 20, 30}, 50) == 1); // (20,30), 10 left

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
