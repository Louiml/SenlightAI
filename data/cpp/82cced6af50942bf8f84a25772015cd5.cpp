/*
Write a C++ function that accepts a vector of integers and an integer pivot value, and returns a new vector where all elements less than the pivot appear first, followed by all elements equal to the pivot, and finally all elements greater than the pivot. The relative order of the elements within each of the three groups must be preserved from the original input. Assume the input vector is non-empty. The function should not modify the original vector and should be `const`-correct where applicable.
*/
#include <vector>

// Return a vector with elements arranged as:
// values < pivot, then values == pivot, then values > pivot.
// The relative ordering within each group is preserved.
std::vector<int> pivotArray(const std::vector<int>& nums, int pivot) {
    std::vector<int> less;
    std::vector<int> equal;
    std::vector<int> greater;

    // First pass: distribute elements into three groups.
    for (const int& num : nums) {
        if (num < pivot) {
            less.push_back(num);
        } else if (num == pivot) {
            equal.push_back(num);
        } else {
            greater.push_back(num);
        }
    }

    // Concatenate: less + equal + greater.
    less.insert(less.end(), equal.begin(), equal.end());
    less.insert(less.end(), greater.begin(), greater.end());

    return less;
}
#include <cassert>
#include <vector>

// The function declaration is assumed to be available from the solution above.
// The following main function tests the pivotArray function.

int main() {
    // Basic case with all three groups.
    std::vector<int> input1 = {9, 12, 3, 5, 14, 5, 10, 10};
    std::vector<int> result1 = pivotArray(input1, 10);
    std::vector<int> expected1 = {9, 3, 5, 5, 10, 10, 12, 14};
    assert(result1 == expected1);

    // All elements less than pivot.
    std::vector<int> input2 = {1, 2, 3};
    std::vector<int> result2 = pivotArray(input2, 5);
    std::vector<int> expected2 = {1, 2, 3};
    assert(result2 == expected2);

    // All elements greater than pivot.
    std::vector<int> input3 = {6, 7, 8};
    std::vector<int> result3 = pivotArray(input3, 5);
    std::vector<int> expected3 = {6, 7, 8};
    assert(result3 == expected3);

    // All elements equal to pivot.
    std::vector<int> input4 = {5, 5, 5};
    std::vector<int> result4 = pivotArray(input4, 5);
    std::vector<int> expected4 = {5, 5, 5};
    assert(result4 == expected4);

    // Single element less than pivot.
    std::vector<int> input5 = {1};
    std::vector<int> result5 = pivotArray(input5, 2);
    std::vector<int> expected5 = {1};
    assert(result5 == expected5);

    // Single element greater than pivot.
    std::vector<int> input6 = {3};
    std::vector<int> result6 = pivotArray(input6, 2);
    std::vector<int> expected6 = {3};
    assert(result6 == expected6);

    // Mixed with duplicates and negative numbers.
    std::vector<int> input7 = {-3, 4, -3, 0, 4, -3, 0, 4};
    std::vector<int> result7 = pivotArray(input7, 0);
    std::vector<int> expected7 = {-3, -3, -3, 0, 0, 4, 4, 4};
    assert(result7 == expected7);

    return 0;
}
// The solution uses a three-pass separation approach. First, we iterate through the entire input vector once, distributing each element into one of three temporary vectors: `less`, `equal`, and `greater`, based on its comparison with the pivot. The relative order within each group is automatically preserved because we push elements in the order they appear. After the distribution pass, we concatenate the three vectors in the required order: `less`, then `equal`, then `greater`. An edge case is when the pivot appears multiple times; all occurrences are kept together in the middle group. Another edge case is when all elements are on one side of the pivot, which still works because the other groups remain empty. The time complexity is O(n) due to a single traversal plus concatenation (which is also O(n) total), and the auxiliary space complexity is O(n) for the three temporary vectors plus the returned vector. We could optimize by pre-allocating the result but this is unnecessary for clarity.
