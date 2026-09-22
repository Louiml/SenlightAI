// Write a C++ function `int removeDuplicatesInPlace(std::vector<int>& nums)` that takes a sorted vector of integers (non-decreasing order) and removes all duplicate elements **in-place** so that each distinct value appears exactly once. The function must return the new length of the vector after removal. The relative order of the remaining elements must be preserved. The vector may be empty; in that case, return `0`. You may assume that the input is always sorted. The function must not use any extra container (like a set or another vector) for the main removal logic — only a constant amount of auxiliary memory.

// The main idea is to use the **two-pointer technique** because the list is sorted. We maintain one pointer `i` that represents the index of the last unique element we have placed. We iterate with a second pointer `j` through the array. Whenever `nums[j]` differs from `nums[i]`, we increment `i` and copy `nums[j]` to `nums[i]`. This way, the first `i+1` positions always contain the unique elements in order. After the loop, the new length is `i+1`. Edge cases: empty input (return `0`), single element (no changes, return `1`), all duplicates (reduce to one element), and no duplicates (all elements remain). The algorithm runs in **O(n)** time because each element is visited once. It uses **O(1)** auxiliary space because we only modify the vector in‑place and use integer variables.

#include <vector>

// Remove duplicates from a sorted vector in-place.
// Returns the new length of the vector with unique elements.
int removeDuplicatesInPlace(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    int writeIndex = 0;  // position of last unique element placed

    for (int readIndex = 1; readIndex < static_cast<int>(nums.size()); ++readIndex) {
        if (nums[readIndex] != nums[writeIndex]) {
            ++writeIndex;
            nums[writeIndex] = nums[readIndex];
        }
    }

    // Optionally resize to the new length (though not required by the task)
    // nums.resize(writeIndex + 1);

    return writeIndex + 1;
}

#include <cassert>
#include <vector>

// Function under test
int removeDuplicatesInPlace(std::vector<int>& nums);

int main() {
    // Case 1: empty
    std::vector<int> v1;
    assert(removeDuplicatesInPlace(v1) == 0);
    assert(v1.empty());

    // Case 2: single element
    std::vector<int> v2 = {5};
    assert(removeDuplicatesInPlace(v2) == 1);
    assert((v2 == std::vector<int>{5}));

    // Case 3: all duplicates
    std::vector<int> v3 = {1, 1, 1, 1};
    assert(removeDuplicatesInPlace(v3) == 1);
    assert(v3[0] == 1); // first element remains

    // Case 4: mixed duplicates
    std::vector<int> v4 = {1, 1, 2, 3, 3, 4, 5, 5};
    int len4 = removeDuplicatesInPlace(v4);
    assert(len4 == 5);
    for (int i = 0; i < len4; ++i) {
        assert(v4[i] == (i + 1)); // expects 1,2,3,4,5
    }

    // Case 5: no duplicates
    std::vector<int> v5 = {10, 20, 30};
    int len5 = removeDuplicatesInPlace(v5);
    assert(len5 == 3);
    for (int i = 0; i < len5; ++i) {
        assert(v5[i] == ((i + 1) * 10));
    }

    // Case 6: negative numbers and zeros
    std::vector<int> v6 = {-3, -3, 0, 0, 2, 4, 4};
    int len6 = removeDuplicatesInPlace(v6);
    assert(len6 == 4);
    assert(v6[0] == -3);
    assert(v6[1] == 0);
    assert(v6[2] == 2);
    assert(v6[3] == 4);

    return 0;
}
