// Write a C++ function that takes a non-empty vector of integers where every element is either strictly positive or strictly negative (the problem guarantees that the numbers of positive and negative elements are equal), and returns a new vector of the same size such that all positive numbers appear at even indices (0, 2, 4, …) and all negative numbers appear at odd indices (1, 3, 5, …), preserving the relative order of positives and negatives as they appear in the input. The input vector must not be modified; your function should return the rearranged result. Assume the size is even and at least 2. For example, given `[3, -1, 2, -5]`, the output should be `[3, -1, 2, -5]`; given `[-1, 3, -5, 2]`, the output should be `[3, -1, 2, -5]`. Your function should be `const`‑correct: accept the input as a constant reference, and do not mutate any input data. Also, provide a solution that works for any vector meeting the described constraints.

The solution uses a two‑pointer technique with an output vector of the same size as the input, initialized to zeros. We maintain two indices: a positive index starting at 0 and a negative index starting at 1. We iterate over the input vector once. For each element, if it is positive, we place it at the current positive index and increment that index by 2; if negative, we place it at the current negative index and increment that index by 2. Because the counts of positives and negatives are equal, these indices will never go out of bounds. This preserves the relative order among positives and among negatives, as we process the input left‑to‑right. The main edge cases include: the vector may start with negatives, but the output always places positives first; inputs with only one positive and one negative; and vectors where positives and negatives alternate. Since every element is nonzero, we do not need to handle zero, but the condition checks for `> 0` and `< 0` exclude zero anyway. The algorithm runs in O(n) time and uses O(n) auxiliary space for the result (the input is not modified). If we consider the result as necessary output, the extra space is O(1) beyond the output, but conventionally we count the output vector as part of the space, so it is O(n) total auxiliary.

#include <vector>

// Rearranges a vector of equal counts of positive and negative integers
// so that positives occupy even indices and negatives occupy odd indices,
// preserving relative order among each sign. Returns the new vector.
std::vector<int> rearrangeArray(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> result(n, 0);
    int positiveIndex = 0;
    int negativeIndex = 1;

    for (int i = 0; i < n; ++i) {
        if (nums[i] > 0) {
            result[positiveIndex] = nums[i];
            positiveIndex += 2;
        } else { // nums[i] < 0, since zero is not present per constraints
            result[negativeIndex] = nums[i];
            negativeIndex += 2;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we provide test code.
int main() {
    // Basic alternating case
    std::vector<int> test1 = {3, -1, 2, -5};
    assert(rearrangeArray(test1) == std::vector<int>({3, -1, 2, -5}));

    // Negatives first in input
    std::vector<int> test2 = {-1, 3, -5, 2};
    assert(rearrangeArray(test2) == std::vector<int>({3, -1, 2, -5}));

    // Single pair
    std::vector<int> test3 = {-7, 4};
    assert(rearrangeArray(test3) == std::vector<int>({4, -7}));

    // Larger mixed input
    std::vector<int> test4 = {1, -2, 3, -4, 5, -6};
    assert(rearrangeArray(test4) == std::vector<int>({1, -2, 3, -4, 5, -6}));

    // Input with duplicates and shuffled order
    std::vector<int> test5 = {-10, 10, -10, 10, -10, 10};
    assert(rearrangeArray(test5) == std::vector<int>({10, -10, 10, -10, 10, -10}));

    // Larger shuffled input
    std::vector<int> test6 = {8, -3, 9, -1, 4, -7};
    assert(rearrangeArray(test6) == std::vector<int>({8, -3, 9, -1, 4, -7}));

    // Alternating starting with negative
    std::vector<int> test7 = {-2, 1, -4, 3, -6, 5};
    assert(rearrangeArray(test7) == std::vector<int>({1, -2, 3, -4, 5, -6}));

    // All positives first, then negatives
    std::vector<int> test8 = {2, 4, 6, -1, -3, -5};
    assert(rearrangeArray(test8) == std::vector<int>({2, -1, 4, -3, 6, -5}));

    // All negatives first, then positives
    std::vector<int> test9 = {-1, -3, -5, 2, 4, 6};
    assert(rearrangeArray(test9) == std::vector<int>({2, -1, 4, -3, 6, -5}));

    // Mixed with more repetitions
    std::vector<int> test10 = {7, -2, -8, 3, 5, -4, 9, -6};
    assert(rearrangeArray(test10) == std::vector<int>({7, -2, 3, -8, 5, -4, 9, -6}));

    return 0;
}
