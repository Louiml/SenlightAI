// Write a C++ function `void nextPermutation(vector<int>& nums)` that rearranges a vector of integers into the lexicographically next greater permutation of its elements. If no such greater permutation exists (i.e., the vector is in strictly descending order), rearrange it into the ascending order (the smallest permutation). The function must modify the vector in place. For example, given `[1,2,3]`, the output should be `[1,3,2]`; given `[3,2,1]`, the output should be `[1,2,3]`; given `[1,1,5]`, the output should be `[1,5,1]`. Handle duplicate values correctly, and ensure the function works for vectors of any size (including empty and single-element vectors, which remain unchanged). The function must be efficient and avoid unnecessary copying.

The standard algorithm for lexicographic next permutation works in three steps:  
1. **Find the pivot**: Scan from right to left to find the first index `i` such that `nums[i] < nums[i+1]`. This `i` is the rightmost position where a change can produce a larger permutation. If no such `i` exists (the entire sequence is non-increasing), the current permutation is the last one; reverse the whole vector to get the first permutation.  
2. **Find the successor**: Scan from the right end to find the first index `j > i` such that `nums[j] > nums[i]`. Because the suffix after `i` is non-increasing, this `j` is the smallest element greater than `nums[i]` in that suffix. Swap `nums[i]` and `nums[j]`.  
3. **Reverse the suffix**: After swapping, the suffix starting at `i+1` remains non-increasing; reversing it makes it the smallest possible suffix, which yields the next permutation.  
**Edge cases**: Empty vector, single element, all equal elements, or already maximal permutation—all handled by the pivot search and reversal.  
**Complexities**: Time \(O(n)\) (one backward scan, one backward scan for successor, and one reverse), Space \(O(1)\) auxiliary.

#include <vector>
#include <algorithm>

// Rearrange nums into the lexicographically next greater permutation in place.
// If not possible, rearrange into the smallest permutation (ascending order).
void nextPermutation(std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    int pivot = -1;

    // Step 1: Find the rightmost index where nums[i] < nums[i+1].
    for (int i = n - 2; i >= 0; --i) {
        if (nums[i] < nums[i + 1]) {
            pivot = i;
            break;
        }
    }

    // Step 2: If a pivot exists, find the smallest element in the suffix
    // that is greater than nums[pivot] and swap it.
    if (pivot != -1) {
        for (int i = n - 1; i > pivot; --i) {
            if (nums[i] > nums[pivot]) {
                std::swap(nums[i], nums[pivot]);
                break;
            }
        }
    }

    // Step 3: Reverse the suffix starting just after the pivot.
    std::reverse(nums.begin() + pivot + 1, nums.end());
}

#include <vector>
#include <cassert>

int main() {
    std::vector<int> v1 = {1, 2, 3};
    nextPermutation(v1);
    assert(v1 == std::vector<int>({1, 3, 2}));

    std::vector<int> v2 = {3, 2, 1};
    nextPermutation(v2);
    assert(v2 == std::vector<int>({1, 2, 3}));

    std::vector<int> v3 = {1, 1, 5};
    nextPermutation(v3);
    assert(v3 == std::vector<int>({1, 5, 1}));

    std::vector<int> v4 = {1};
    nextPermutation(v4);
    assert(v4 == std::vector<int>({1}));

    std::vector<int> v5 = {};
    nextPermutation(v5);
    assert(v5.empty());

    std::vector<int> v6 = {2, 3, 1};
    nextPermutation(v6);
    assert(v6 == std::vector<int>({3, 1, 2}));

    std::vector<int> v7 = {1, 3, 2};
    nextPermutation(v7);
    assert(v7 == std::vector<int>({2, 1, 3}));

    std::vector<int> v8 = {1, 2, 3, 4};
    nextPermutation(v8);
    assert(v8 == std::vector<int>({1, 2, 4, 3}));

    std::vector<int> v9 = {4, 3, 2, 1};
    nextPermutation(v9);
    assert(v9 == std::vector<int>({1, 2, 3, 4}));

    std::vector<int> v10 = {1, 2, 2, 3};
    nextPermutation(v10);
    assert(v10 == std::vector<int>({1, 2, 3, 2}));

    return 0;
}
