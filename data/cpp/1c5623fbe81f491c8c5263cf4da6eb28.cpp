// Write a C++ function named `findDuplicateElements` that takes a non-empty vector of integers where each element appears either once or exactly twice, and returns a vector containing all the elements that appear exactly twice. The function must not modify the input vector (i.e., it must be `const`-correct), and the returned duplicates can be in any order. Your implementation should have a time complexity of O(n) and an auxiliary space complexity of O(1) (excluding the space for the output). You may assume the input integers are in the range [1, n], where n is the size of the vector. The function signature should be `std::vector<int> findDuplicateElements(const std::vector<int>& nums)`.

// This problem is a classic "find all duplicates in an array" puzzle, and the key insight is that the values are guaranteed to be in the range [1, n] (where n is the size of the array), and each value appears either once or twice. Since we cannot modify the input vector, we cannot use the common negative-indexing trick directly on the input. However, we can copy the input into a new local vector and then apply the negative-indexing method on that copy. The algorithm works as follows: iterate through the copied array (which contains the same integers as the input). For each element, take its absolute value (to handle previously negated entries), compute the corresponding index as `abs(value) - 1` (since values start at 1). Check the value at that index: if it is already negative, it means this value has been seen before, so it is a duplicate. Otherwise, negate the value at that index to mark it as seen. This method works because we are effectively using the array's own slots as a visited set. Edge cases include: an empty input (though the problem states non-empty, it's good to handle gracefully; but we can assume non-empty per spec), a vector of size 1 (then no duplicates), and the case where all elements are unique (then the result is empty). The time complexity is O(n) because we make a single pass over the copied array plus a single copy operation (O(n)). The auxiliary space is O(n) for the copy (but this is not counted as "additional" in the strict sense? Actually, the problem says "auxiliary space O(1)" – but since we copy the input, that is O(n) extra space. The original approach in the snippet modifies the input directly to achieve O(1) auxiliary space, but since we cannot modify the input, we must copy it. We should note that in a contest setting, this copy is acceptable if the problem allows. However, to keep the auxiliary space truly O(1), one could try to use the input directly only if the problem permits modification. Since the task explicitly says "must not modify the input", we have to accept O(n) auxiliary space. Alternatively, we could argue that the auxiliary space is O(1) if we ignore the copy because the copy is just a local copy? No, that's not fair. The standard solution for this problem does modify the array. The task is artificially constrained, so we will just be honest: our solution uses O(n) extra space for the copy, and O(n) time.

#include <vector>
#include <cstdlib> // for std::abs

/**
 * Finds all elements that appear exactly twice in a vector where each element
 * appears either once or twice and values are in [1, n] for n = nums.size().
 * The input vector is not modified.
 *
 * @param nums Const reference to the input vector.
 * @return A vector containing all duplicate values (each exactly once, any order).
 */
std::vector<int> findDuplicateElements(const std::vector<int>& nums) {
    // Make a local copy so we can mark visited using negative indexing without
    // modifying the caller's data.
    std::vector<int> copy = nums;
    std::vector<int> duplicates;

    for (int value : copy) {
        int current = std::abs(value);
        int index = current - 1; // values are in [1, n]
        if (copy[index] < 0) {
            // Already visited -> this is a duplicate
            duplicates.push_back(current);
        } else {
            // Mark visited by negating
            copy[index] = -copy[index];
        }
    }

    return duplicates;
}

#include <cassert>
#include <vector>

// The solution function is declared above (omitted here for brevity).
int main() {
    std::vector<int> test1 = {4, 3, 2, 7, 8, 2, 3, 1};
    std::vector<int> result1 = findDuplicateElements(test1);
    std::sort(result1.begin(), result1.end());
    assert(result1 == std::vector<int>({2, 3}));

    std::vector<int> test2 = {1, 1, 2};
    std::vector<int> result2 = findDuplicateElements(test2);
    assert(result2 == std::vector<int>({1}));

    std::vector<int> test3 = {1, 2, 3, 4};
    std::vector<int> result3 = findDuplicateElements(test3);
    assert(result3.empty());

    std::vector<int> test4 = {5, 5};
    std::vector<int> result4 = findDuplicateElements(test4);
    assert(result4 == std::vector<int>({5}));

    std::vector<int> test5 = {1};
    std::vector<int> result5 = findDuplicateElements(test5);
    assert(result5.empty());

    std::vector<int> test6 = {2, 1, 2, 1};
    std::vector<int> result6 = findDuplicateElements(test6);
    std::sort(result6.begin(), result6.end());
    assert(result6 == std::vector<int>({1, 2}));

    // Also verify input is not modified
    std::vector<int> original = {4, 3, 2, 7, 8, 2, 3, 1};
    findDuplicateElements(original);
    assert(original == std::vector<int>({4, 3, 2, 7, 8, 2, 3, 1}));
}
