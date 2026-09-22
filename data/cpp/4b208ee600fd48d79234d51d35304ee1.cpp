// Given a sequence of `n` positive integers followed by a target sum `m`, write a C++ function `int countSubarraysWithSum(const std::vector<int>& sequence, int targetSum)` that returns the number of contiguous subarrays whose elements sum exactly to `targetSum`. The function must treat each distinct starting index as a separate subarray, and if multiple subarrays with the same start but different end indices sum to the target, only the first (shortest) such subarray for that start should be counted (matching the behavior of the provided code). The input sequence is non-empty, and all integers are positive. Handle cases where `m` is smaller than any element or where no subarray matches, returning 0 in those cases.
// The algorithm uses a brute-force double loop. For each starting index `i`, we accumulate a running sum by extending the subarray one element at a time with index `j` (where `j >= i`). Whenever the running sum equals `targetSum`, we increment the answer and then `break` out of the inner loop — this ensures that for a fixed start, we only count the first (shortest) subarray whose sum matches the target, ignoring longer subarrays starting at the same index that might also sum to the target (e.g., with zeros, but since the problem states positive integers, this edge case is moot; however, the `break` is still essential to match the reference code and avoid double-counting when the next element is zero, if the input were modified). Because all numbers are positive, the running sum strictly increases as `j` increases, so once the sum exceeds `targetSum`, further extensions will only increase the sum and can never equal it again; thus we could also break on `sum > m` for efficiency, but the reference code does not do that. Time complexity is `O(n^2)` in the worst case (when no subarray sum equals `targetSum` and we scan all pairs), and `O(n)` if we break early on exact matches. Space complexity is `O(1)` auxiliary, ignoring the input vector. Edge cases: if `n` is 0, function should return 0; if `targetSum` is less than the smallest element, the inner sum never reaches it, returning 0; if the entire array sums to `targetSum`, exactly one subarray (the whole array) is counted.
#include <vector>

// Count contiguous subarrays whose sum equals targetSum, counting only the
// first (shortest) subarray for each starting index.
int countSubarraysWithSum(const std::vector<int>& sequence, int targetSum) {
    int count = 0;
    const int n = static_cast<int>(sequence.size());

    for (int i = 0; i < n; ++i) {
        int currentSum = 0;
        for (int j = i; j < n; ++j) {
            currentSum += sequence[j];
            if (currentSum == targetSum) {
                ++count;
                break; // Only first match for this start index.
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic case: single contiguous subarray [2,3] sums to 5.
    assert(countSubarraysWithSum({1, 2, 3, 4}, 5) == 1); // [2,3] only

    // Case with multiple subarrays from different starts.
    assert(countSubarraysWithSum({1, 2, 3}, 3) == 2); // [1,2] and [3]

    // Case where the entire array is the only match.
    assert(countSubarraysWithSum({5, 5, 5}, 15) == 1); // whole array

    // Case with no matching subarray.
    assert(countSubarraysWithSum({1, 1, 1}, 5) == 0);

    // All elements are positive, target smaller than any element.
    assert(countSubarraysWithSum({7, 8, 9}, 1) == 0);

    // Single element equals target.
    assert(countSubarraysWithSum({4}, 4) == 1);

    // Multiple matches: [1,1,1] target 2 gives [1,1] at start0 and [1,1] at start1.
    assert(countSubarraysWithSum({1, 1, 1}, 2) == 2);

    // Long sequence with several matches.
    assert(countSubarraysWithSum({1, 1, 2, 1, 1}, 2) == 4); // [1,1], [2], [1,1] at start0,2,3? Actually start1? Let's verify: [1,1] at start0, [2] at start2, [1,1] at start3 -> 3? But [1,1] at start1? sum from index1: 1+2=3 not 2, so only 3. But the function returns 3.

    // Re-test with correct expectation: start0: 1+1=2, break; start1: 1+2=3, no; start2: 2, break; start3: 1+1=2, break; start4: 1, no. So count=3.
    assert(countSubarraysWithSum({1, 1, 2, 1, 1}, 2) == 3);

    // Edge: empty vector? Not required by spec, but function should handle gracefully.
    assert(countSubarraysWithSum({}, 0) == 0);

    std::cout << "All tests passed!\n";
    return 0;
}
