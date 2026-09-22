// Write a C++ function `int removeDuplicatesAtMostTwice(std::vector<int>& nums)` that, given a sorted vector of integers, removes duplicate elements in-place so that each distinct value appears at most twice, and returns the new length of the modified vector. The function must modify the input vector directly, using only O(1) extra memory (no additional vector, array, or dynamic allocation). After the function returns, the first part of `nums` up to the returned length must contain the valid elements (preserving the original relative order), and the content beyond that length is irrelevant. If the vector has 2 or fewer elements, return its original length unchanged.

// The key observation is that because the vector is sorted, any duplicate value appears consecutively. We allow at most two copies of each value. A common efficient approach uses a "write pointer" `index` that indicates where the next valid element should be placed, and a "read pointer" `i` that scans the input. Since we allow two duplicates, we can initialize `index = 2` (because the first two elements, whatever they are, are always kept — even if they are equal, they form a valid pair). Then for each `i` starting from 2, we compare `nums[i]` with `nums[index - 2]`. If they are different, it means `nums[i]` is not a third (or greater) copy of the value at `index-2`, so we can safely place it at position `index` and increment `index`. If they are equal, we skip `nums[i]` because it would create a third consecutive duplicate. This works because `index` always lags behind or equals `i`, and the prefix up to `index-1` already contains at most two copies of each value. Edge cases: empty vector (length 0) and single element (length 1) are handled by the initial check `if (nums.size() < 2) return nums.size();`. Also, vectors with all identical values (e.g., [5,5,5,5]) will keep only the first two, yielding length 2. Complexity: time is O(n) where n is the original size because each element is processed once. Space is O(1) extra memory, ignoring the input vector itself and the integer variables.

#include <vector>

// Removes duplicates in a sorted vector so that each value appears at most twice.
// Modifies the vector in-place and returns the new length.
int removeDuplicatesAtMostTwice(std::vector<int>& nums) {
    int n = nums.size();
    if (n < 2) {
        return n;
    }
    // The first two elements are always kept.
    int write_index = 2;
    // Start scanning from the third element (index 2).
    for (int read_index = 2; read_index < n; ++read_index) {
        // Compare with the element two positions before the current write position.
        if (nums[read_index] != nums[write_index - 2]) {
            nums[write_index] = nums[read_index];
            ++write_index;
        }
    }
    return write_index;
}

#include <cassert>
#include <vector>

// The function is declared above; here we include it by having it in the same translation unit.
int removeDuplicatesAtMostTwice(std::vector<int>& nums);

int main() {
    // Example 1 from the problem statement.
    std::vector<int> v1 = {1,1,1,2,2,3};
    int len1 = removeDuplicatesAtMostTwice(v1);
    assert(len1 == 5);
    assert((v1[0] == 1 && v1[1] == 1 && v1[2] == 2 && v1[3] == 2 && v1[4] == 3));

    // Example 2 from the problem statement.
    std::vector<int> v2 = {0,0,1,1,1,1,2,3,3};
    int len2 = removeDuplicatesAtMostTwice(v2);
    assert(len2 == 7);
    assert((v2[0] == 0 && v2[1] == 0 && v2[2] == 1 && v2[3] == 1 && v2[4] == 2 && v2[5] == 3 && v2[6] == 3));

    // Empty vector: length remains 0.
    std::vector<int> v3 = {};
    assert(removeDuplicatesAtMostTwice(v3) == 0);

    // Single element: length unchanged.
    std::vector<int> v4 = {42};
    assert(removeDuplicatesAtMostTwice(v4) == 1);
    assert(v4[0] == 42);

    // Two identical elements: both kept.
    std::vector<int> v5 = {7,7};
    assert(removeDuplicatesAtMostTwice(v5) == 2);
    assert((v5[0] == 7 && v5[1] == 7));

    // All same value, many copies: only two kept.
    std::vector<int> v6 = {3,3,3,3,3};
    int len6 = removeDuplicatesAtMostTwice(v6);
    assert(len6 == 2);
    assert((v6[0] == 3 && v6[1] == 3));

    // Already has at most two of each: unchanged length.
    std::vector<int> v7 = {1,1,2,2,3,3};
    assert(removeDuplicatesAtMostTwice(v7) == 6);

    // Negative numbers and mixing.
    std::vector<int> v8 = {-5,-5,-4,-4,-4,-3,-3,-2};
    int len8 = removeDuplicatesAtMostTwice(v8);
    assert(len8 == 7);
    assert((v8[0] == -5 && v8[1] == -5 && v8[2] == -4 && v8[3] == -4 && v8[4] == -3 && v8[5] == -3 && v8[6] == -2));

    // Sorted but with exactly two consecutive groups of same value.
    std::vector<int> v9 = {1,1,1,2,2,2,3,3,3};
    int len9 = removeDuplicatesAtMostTwice(v9);
    assert(len9 == 6);
    assert((v9[0] == 1 && v9[1] == 1 && v9[2] == 2 && v9[3] == 2 && v9[4] == 3 && v9[5] == 3));

    // Large vector with many duplicates.
    std::vector<int> v10(100, 0); // 100 zeros
    assert(removeDuplicatesAtMostTwice(v10) == 2);
    assert((v10[0] == 0 && v10[1] == 0));

    return 0;
}
