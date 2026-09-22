Write a C++ function `std::vector<int> restorePermutation(const std::vector<int>& merged)` that takes a vector `merged` of length `2n` (where `n` is the length of the original permutation) and returns the original permutation `p` of length `n`. The input is guaranteed to be a valid merge of a permutation `p` with itself: two copies of `p` are interleaved while preserving each copy's relative order. The function should return the unique permutation `p` that could have produced the given merge. The values in the permutation are integers from `1` to `n`, and each appears exactly twice in `merged`. For example, for input `[3,1,3,1,2,2]`, the output should be `[3,1,2]`.

// The key insight is that in a valid merge of a permutation with itself, the first occurrence of each number in the merged sequence exactly reflects the order of the original permutation. To see why, consider that each element of the second copy of `p` is inserted after the corresponding element of the first copy, preserving the first copy's order. When we scan the merged array from left to right, the very first time we encounter a particular value, it must be from the first copy of the permutation (because the second copy's occurrence cannot appear before the first copy's occurrence for that same value). Therefore, the order in which distinct numbers first appear in the merged sequence is exactly the order of the original permutation `p`. The algorithm simply iterates through the merged vector, tracking which numbers have already been seen, and appends each newly seen number to the result. Since every number appears exactly twice, we stop after collecting `n` distinct numbers. Edge cases: `n=1` where the merged array is `[1,1]` yields `[1]`; duplicate values in the merged array are handled by the seen-set. Time complexity is O(2n) = O(n), and space complexity is O(n) for the result and a boolean/seen array of size `n+1` (indices from 1 to n).

#include <vector>
#include <unordered_set>

// Restore the original permutation from a merged sequence.
// The input 'merged' has length 2n and is a valid merge of a permutation with itself.
// Returns the original permutation of length n.
std::vector<int> restorePermutation(const std::vector<int>& merged) {
    std::vector<int> result;
    std::unordered_set<int> seen;
    
    for (int value : merged) {
        if (seen.find(value) == seen.end()) {
            // First occurrence of this value: it belongs to the first copy.
            result.push_back(value);
            seen.insert(value);
            // Once we have found all n distinct elements, we can stop.
            if (result.size() * 2 == merged.size()) {
                break;
            }
        }
    }
    return result;
}

#include <vector>
#include <cassert>
#include <iostream>

// Include the solution function here (or link appropriately).
// For testing, we assume the function is defined above.

int main() {
    // Test case 1: Example from problem statement.
    std::vector<int> merged1 = {3, 1, 3, 1, 2, 2};
    std::vector<int> expected1 = {3, 1, 2};
    assert(restorePermutation(merged1) == expected1);

    // Test case 2: Example with n=2.
    std::vector<int> merged2 = {2, 2, 1, 1};
    std::vector<int> expected2 = {2, 1};
    assert(restorePermutation(merged2) == expected2);

    // Test case 3: Another n=2 merge.
    std::vector<int> merged3 = {2, 1, 2, 1};
    std::vector<int> expected3 = {2, 1};
    assert(restorePermutation(merged3) == expected3);

    // Test case 4: n=1.
    std::vector<int> merged4 = {1, 1};
    std::vector<int> expected4 = {1};
    assert(restorePermutation(merged4) == expected4);

    // Test case 5: Larger permutation n=5.
    std::vector<int> merged5 = {4, 5, 4, 1, 3, 5, 2, 1, 2, 3};
    std::vector<int> expected5 = {4, 5, 1, 2, 3};
    assert(restorePermutation(merged5) == expected5);

    // Test case 6: Already in order.
    std::vector<int> merged6 = {1, 2, 3, 1, 2, 3};
    std::vector<int> expected6 = {1, 2, 3};
    assert(restorePermutation(merged6) == expected6);

    // Test case 7: Reverse order input.
    std::vector<int> merged7 = {3, 3, 2, 2, 1, 1};
    std::vector<int> expected7 = {3, 2, 1};
    assert(restorePermutation(merged7) == expected7);

    // Test case 8: Random interleaving.
    std::vector<int> merged8 = {2, 1, 2, 3, 1, 3};
    std::vector<int> expected8 = {2, 1, 3};
    assert(restorePermutation(merged8) == expected8);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
