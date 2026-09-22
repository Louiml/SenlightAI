Write a C++ function named `rearrangeBySign` that takes a non-empty `std::vector<int>` containing equal numbers of positive (or zero) and negative integers, and returns a new vector where elements are arranged so that positive and negative numbers alternate, starting with a positive number. The relative order of all positive numbers must be preserved, and the relative order of all negative numbers must be preserved. The original vector must not be modified. Assume the input always has an equal count of non-negative (`>=0`) and negative (`<0`) numbers, and that the first element in the rearranged result must always be non-negative.
// The solution separates the input into two temporary vectors: one for non-negative numbers (`>=0`) and one for negative numbers (`<0`), preserving their original relative order by iterating through the input once for each category. Then a result vector of the same total size is built by placing elements from the non-negative list at even indices (0, 2, 4, …) and from the negative list at odd indices (1, 3, 5, …). This works because the problem guarantees equal counts, so the positive list and negative list have identical lengths, and no index out-of-bounds occurs. Edge cases include all numbers being non-negative or all being negative—but per the constraint, those cannot happen; still, the code would produce a partially filled result if they did, so the precondition is essential. Time complexity is O(n) for two linear scans plus one linear merge, and space complexity is O(n) for the two auxiliary lists and the result vector (total O(n) auxiliary space beyond the returned vector).
#include <vector>

// Rearrange a vector with equal numbers of non-negative and negative values
// so that they alternate, starting with a non-negative number.
std::vector<int> rearrangeBySign(const std::vector<int>& nums) {
    std::vector<int> positives;
    std::vector<int> negatives;

    // Collect non-negative numbers in original order.
    for (int value : nums) {
        if (value >= 0) {
            positives.push_back(value);
        }
    }

    // Collect negative numbers in original order.
    for (int value : nums) {
        if (value < 0) {
            negatives.push_back(value);
        }
    }

    // Build the alternating result.
    std::vector<int> result(nums.size(), 0);
    for (std::size_t i = 0; i < result.size(); ++i) {
        if (i % 2 == 0) {
            result[i] = positives[i / 2];
        } else {
            result[i] = negatives[(i - 1) / 2];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Function under test (declared here for the test, but in practice it would be included)
std::vector<int> rearrangeBySign(const std::vector<int>& nums);

int main() {
    // Basic alternating case
    std::vector<int> input1 = {3, -1, 2, -5};
    std::vector<int> expected1 = {3, -1, 2, -5};
    assert(rearrangeBySign(input1) == expected1);

    // Mixed zeros and negatives
    std::vector<int> input2 = {0, -2, 4, -6};
    std::vector<int> expected2 = {0, -2, 4, -6};
    assert(rearrangeBySign(input2) == expected2);

    // Input already alternating from positive start
    std::vector<int> input3 = {1, -1, 2, -2, 3, -3};
    std::vector<int> expected3 = {1, -1, 2, -2, 3, -3};
    assert(rearrangeBySign(input3) == expected3);

    // Input with positives not naturally ordered relative to negatives
    std::vector<int> input4 = {5, 7, -1, -2};
    std::vector<int> expected4 = {5, -1, 7, -2};
    assert(rearrangeBySign(input4) == expected4);

    // Single pair
    std::vector<int> input5 = {10, -10};
    std::vector<int> expected5 = {10, -10};
    assert(rearrangeBySign(input5) == expected5);

    // Larger case with zeros
    std::vector<int> input6 = {9, 0, -1, -2, 8, -3, 7, -4};
    std::vector<int> expected6 = {9, -1, 0, -2, 8, -3, 7, -4};
    assert(rearrangeBySign(input6) == expected6);

    return 0;
}
