/*
Write a C++ function that takes a non-empty vector of positive integers, where each integer is in the range 1 to the size of the vector (inclusive), and returns a vector containing all numbers from 1 to the vector size that are missing from the input. The input may contain duplicates, and the order of the returned missing numbers must be ascending. For example, given `{2, 3, 1, 8, 2, 3, 5, 1}` (size 8), the function should return `{4, 6, 7}`. The function must not modify the input vector (i.e., it should work on a `const` reference), and must handle edge cases such as a vector already containing every number exactly once (returning an empty vector) and a vector where all numbers are the same.
*/
#include <vector>
#include <algorithm>

// Returns a vector of all numbers from 1 to nums.size() that are missing from the input.
std::vector<int> findMissingNumbers(const std::vector<int>& nums) {
    // Make a mutable copy to perform cyclic sort without altering the input.
    std::vector<int> arr = nums;
    int n = arr.size();
    int i = 0;
    while (i < n) {
        // Correct position for arr[i] is index arr[i]-1.
        // If the current element is not in its correct place and not a duplicate,
        // swap it to its target position.
        if (arr[i] >= 1 && arr[i] <= n && arr[i] != arr[arr[i] - 1]) {
            std::swap(arr[i], arr[arr[i] - 1]);
        } else {
            ++i;
        }
    }

    std::vector<int> missing;
    for (int idx = 0; idx < n; ++idx) {
        if (arr[idx] != idx + 1) {
            missing.push_back(idx + 1);
        }
    }
    return missing;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Example 1 from the statement
    std::vector<int> input1 = {2, 3, 1, 8, 2, 3, 5, 1};
    std::vector<int> expected1 = {4, 6, 7};
    assert(findMissingNumbers(input1) == expected1);

    // Example 2
    std::vector<int> input2 = {2, 4, 1, 2};
    std::vector<int> expected2 = {3};
    assert(findMissingNumbers(input2) == expected2);

    // Example 3
    std::vector<int> input3 = {2, 3, 2, 1};
    std::vector<int> expected3 = {4};
    assert(findMissingNumbers(input3) == expected3);

    // All numbers present exactly once
    std::vector<int> input4 = {1, 2, 3, 4};
    std::vector<int> expected4 = {};
    assert(findMissingNumbers(input4) == expected4);

    // All numbers are the same (size 5, all 1s)
    std::vector<int> input5 = {1, 1, 1, 1, 1};
    std::vector<int> expected5 = {2, 3, 4, 5};
    assert(findMissingNumbers(input5) == expected5);

    // Single element vector with the correct number
    std::vector<int> input6 = {1};
    std::vector<int> expected6 = {};
    assert(findMissingNumbers(input6) == expected6);

    // Single element vector with a different number (but valid range 1..1, so only 1 is possible)
    std::vector<int> input7 = {1}; // already tested

    // Larger random case: size 10, with duplicates and some missing
    std::vector<int> input8 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}; // all present
    std::vector<int> expected8 = {};
    assert(findMissingNumbers(input8) == expected8);

    // Reverse with duplicates
    std::vector<int> input9 = {3, 3, 2, 1}; // size 4, missing 4
    std::vector<int> expected9 = {4};
    assert(findMissingNumbers(input9) == expected9);

    // Input with all values same but different size
    std::vector<int> input10 = {4, 4, 4, 4}; // size 4, missing {1,2,3}
    std::vector<int> expected10 = {1, 2, 3};
    assert(findMissingNumbers(input10) == expected10);

    std::cout << "All tests passed!\n";
    return 0;
}
// The solution uses the cyclic sort pattern, which places each number at its correct index (i.e., value `v` should be at index `v-1`) by swapping elements until every position either holds its correct value or encounters a duplicate. The algorithm iterates with an index `i`: if the current element `nums[i]` is not at its correct position (i.e., `nums[i] != nums[nums[i]-1]`), swap it with the element at its target index; otherwise increment `i`. This ensures that after the loop, every index either contains the correct number or a duplicate. Then, a second pass collects all indices `i` where `nums[i] != i+1`, adding `i+1` to the result. Edge cases include: empty input (though specified non-empty, the code can handle it), duplicates of any number, and a vector already sorted correctly. Time complexity is O(n) because each swap places at least one element in its final position, and the second pass is linear. Space complexity is O(1) auxiliary (excluding the output vector), as swapping is in-place on a copy if the input is const (we must copy the vector internally, but the auxiliary space beyond the copy is constant). For a `const` reference, we make a local copy, which takes O(n) space, but the problem statement typically ignores the output and the copy; if we are strict, the space is O(n) due to the copy. However, the core algorithm's auxiliary space (beyond input/output) is O(1). The method is robust for duplicates and missing numbers.
