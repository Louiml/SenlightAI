/*
Write a C++ function named `removeDuplicatesFromSorted` that accepts a reference to a `std::vector<int>` which is guaranteed to be sorted in non-decreasing order. The function must modify the vector in-place so that each distinct element appears only once, preserving the original relative order. It should return the new length of the array after duplicates are removed. The elements beyond the returned length are irrelevant and may be left unchanged. The input vector may be empty, contain a single element, or contain many duplicates, but it will always be sorted. The function must not use additional dynamic storage beyond a constant number of variables (modifying the vector in-place is required). The function should be efficient for large inputs, handling up to 10^5 elements. Do not include a `main` function in your solution; provide only the function definition with appropriate includes and a descriptive comment.
*/
#include <vector>

// Remove duplicates from a sorted vector in-place.
// Returns the new length of the array after duplicates are removed.
// The vector is modified so that the first 'new_length' elements are the unique values.
int removeDuplicatesFromSorted(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    int j = 0; // index of the last unique element found
    for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
        if (nums[j] != nums[i]) {
            ++j;
            nums[j] = nums[i];
        }
    }
    return j + 1; // number of unique elements
}
#include <cassert>
#include <vector>

int removeDuplicatesFromSorted(std::vector<int>& nums); // forward declaration

int main() {
    // Empty vector
    std::vector<int> empty;
    assert(removeDuplicatesFromSorted(empty) == 0);
    assert(empty.empty());

    // Single element
    std::vector<int> single = {42};
    assert(removeDuplicatesFromSorted(single) == 1);
    assert(single[0] == 42);

    // Already unique
    std::vector<int> unique = {1, 2, 3, 4, 5};
    int len1 = removeDuplicatesFromSorted(unique);
    assert(len1 == 5);
    assert((unique == std::vector<int>{1, 2, 3, 4, 5}));

    // All duplicates
    std::vector<int> allDup = {7, 7, 7, 7};
    int len2 = removeDuplicatesFromSorted(allDup);
    assert(len2 == 1);
    assert(allDup[0] == 7);

    // Mixed duplicates
    std::vector<int> mixed = {0, 0, 1, 1, 2, 2, 3, 3, 3, 4};
    int len3 = removeDuplicatesFromSorted(mixed);
    assert(len3 == 5);
    assert((mixed[0] == 0 && mixed[1] == 1 && mixed[2] == 2 && mixed[3] == 3 && mixed[4] == 4));

    // Negative numbers and duplicates
    std::vector<int> neg = {-5, -5, -1, 0, 0, 3};
    int len4 = removeDuplicatesFromSorted(neg);
    assert(len4 == 4);
    assert((neg[0] == -5 && neg[1] == -1 && neg[2] == 0 && neg[3] == 3));

    // Large value with many duplicates
    std::vector<int> large = {100, 100, 100, 200, 200, 300};
    int len5 = removeDuplicatesFromSorted(large);
    assert(len5 == 3);
    assert((large[0] == 100 && large[1] == 200 && large[2] == 300));

    // Already sorted with no duplicates but large size
    std::vector<int> almost = {1, 2, 2, 3, 4, 4, 5};
    int len6 = removeDuplicatesFromSorted(almost);
    assert(len6 == 5);
    assert((almost[0] == 1 && almost[1] == 2 && almost[2] == 3 && almost[3] == 4 && almost[4] == 5));

    return 0;
}
// The core idea is to scan the sorted vector with two pointers: one pointer `i` iterates through each element, and another pointer `j` tracks the position of the last kept unique element. Since the array is sorted, duplicates are adjacent, so when `nums[i]` differs from `nums[j]`, we know `nums[i]` is a new unique value and copy it to position `j+1`. This is done by incrementing `j` and assigning `nums[j] = nums[i]`. If the values are equal, we simply skip `i`. The algorithm handles edge cases naturally: an empty vector results in a loop that never executes, and the function returns 0 (since `j` starts at 0 and we return `j+1` only after the loop; but to be safe we initialize `j` to 0 and return `j+1` only if the vector is non-empty; better approach: if vector is empty, return 0, else the standard loop works). A more robust implementation checks for emptiness, but the reference uses `j=0` and returns `j+1`, which would incorrectly return 1 for an empty vector. So in the solution we add an explicit empty check for correctness. Time complexity is O(n) with a single pass, and space complexity is O(1) since we only use two indices. The in-place modification is valid because we only overwrite positions that are at or before the current read position.
