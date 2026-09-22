Write a C++ function named `rearrangeAlternating` that accepts a non-empty vector of integers containing an equal number of positive and negative values (and no zeros). The function must return a new vector where the elements are rearranged such that every pair of consecutive elements alternates between a positive and a negative number, starting with a positive number. The relative order of all positive numbers must be preserved, and similarly the relative order of all negative numbers must be preserved. For example, given `[3, -1, 2, -5]`, the output must be `[3, -1, 2, -5]`; given `[-1, 1, -2, 2]`, the output is `[1, -1, 2, -2]`.
// The solution separates positives and negatives into two separate vectors while preserving their original relative order. After separation, because the input is guaranteed to have equal counts of positives and negatives, both vectors will have the same length. The main algorithm then iterates over the indices of the positive vector (or negative vector, same length), appending one positive element followed by one negative element to a result vector. As no zeros exist and counts are equal, this produces exactly the required alternating pattern starting with a positive. Edge cases are minimal: the vector must contain at least one positive and one negative (therefore size at least 2), but the logic works for any equal-count input, including the smallest case of two elements. Time complexity is \(O(n)\) where \(n\) is the total number of elements, because we traverse all elements exactly twice (one separation pass and one merge pass). Space complexity is \(O(n)\) for the three vectors: positives, negatives, and result (the latter two could be optimized by reusing one, but that would not affect asymptotic complexity).
#include <vector>

// Rearrange a vector with equal numbers of positive and negative integers
// so that it alternates positive and negative, starting with a positive.
std::vector<int> rearrangeAlternating(const std::vector<int>& nums) {
    std::vector<int> positives;
    std::vector<int> negatives;

    // Preserve relative order by scanning the original sequence once.
    for (int value : nums) {
        if (value > 0) {
            positives.push_back(value);
        } else {
            negatives.push_back(value);
        }
    }

    std::vector<int> result;
    result.reserve(nums.size());  // avoid reallocations

    // Equal counts are guaranteed, so both vectors have the same length.
    for (std::size_t i = 0; i < positives.size(); ++i) {
        result.push_back(positives[i]);
        result.push_back(negatives[i]);
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared here (for completeness in this test file).
std::vector<int> rearrangeAlternating(const std::vector<int>& nums);

int main() {
    std::vector<int> input1 = {3, -1, 2, -5};
    std::vector<int> expected1 = {3, -1, 2, -5};
    assert(rearrangeAlternating(input1) == expected1);

    std::vector<int> input2 = {-1, 1, -2, 2};
    std::vector<int> expected2 = {1, -1, 2, -2};
    assert(rearrangeAlternating(input2) == expected2);

    std::vector<int> input3 = {1, -1};
    std::vector<int> expected3 = {1, -1};
    assert(rearrangeAlternating(input3) == expected3);

    std::vector<int> input4 = {-10, 5, -3, 7, -1, 2};
    std::vector<int> expected4 = {5, -10, 7, -3, 2, -1};
    assert(rearrangeAlternating(input4) == expected4);

    std::vector<int> input5 = {4, -4, 6, -6, 8, -8};
    std::vector<int> expected5 = {4, -4, 6, -6, 8, -8};
    assert(rearrangeAlternating(input5) == expected5);

    return 0;
}
