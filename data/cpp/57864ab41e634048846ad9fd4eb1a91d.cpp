Given a positive integer `n` followed by a sequence of `n` integers, each between `1` and `n` inclusive (duplicates allowed), write a standalone C++ function named `rearrangeElements` that receives the vector of integers and returns a new vector of length `n` constructed as follows: first, output every distinct element in the order of their first occurrence, skipping duplicates; then, output all integers from `1` to `n` that were not present in the original sequence, in increasing order. The function should not modify the input vector and must work for `n` up to at least 10^5.

The task requires removing consecutive duplicates from the original sequence while preserving the original order of first appearances, then appending the missing numbers in ascending order. The main algorithm uses a boolean visited array of size `n+1` to track which numbers from `1` to `n` have already been seen. Iterate through the input vector once: for each element `x`, if `visited[x]` is false, mark it as visited and append `x` to the result vector; duplicates are skipped. After this pass, iterate from `1` to `n` and append any index `i` where `visited[i]` is still false. Edge cases include: all numbers are distinct (then no missing numbers are appended), all numbers are duplicates (then the first part contains one element and the rest are all missing numbers), and when `n` is 1. Time complexity is O(n) for the two passes, and space complexity is O(n) for the visited array and the result vector (excluding the input storage).

#include <vector>
#include <cstddef>

// Given a vector of integers in [1, n] where n = vec.size(),
// return a vector with distinct values in first-occurrence order,
// followed by all missing values from 1..n in ascending order.
std::vector<int> rearrangeElements(const std::vector<int>& vec) {
    const std::size_t n = vec.size();
    std::vector<bool> visited(n + 1, false);
    std::vector<int> result;
    result.reserve(n);

    // First pass: collect distinct elements in order of first appearance.
    for (std::size_t i = 0; i < n; ++i) {
        const int value = vec[i];
        if (!visited[value]) {
            visited[value] = true;
            result.push_back(value);
        }
    }

    // Second pass: append missing numbers in increasing order.
    for (int num = 1; num <= static_cast<int>(n); ++num) {
        if (!visited[num]) {
            result.push_back(num);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (assumed from above).
std::vector<int> rearrangeElements(const std::vector<int>& vec);

int main() {
    // Case 1: No duplicates, all numbers present.
    std::vector<int> input1 = {1, 2, 3};
    std::vector<int> expected1 = {1, 2, 3};
    assert(rearrangeElements(input1) == expected1);

    // Case 2: Duplicates at the beginning.
    std::vector<int> input2 = {2, 2, 1, 3};
    std::vector<int> expected2 = {2, 1, 3};
    assert(rearrangeElements(input2) == expected2);

    // Case 3: All duplicates of one number.
    std::vector<int> input3 = {2, 2, 2};
    std::vector<int> expected3 = {2, 1, 3};
    assert(rearrangeElements(input3) == expected3);

    // Case 4: Single element.
    std::vector<int> input4 = {1};
    std::vector<int> expected4 = {1};
    assert(rearrangeElements(input4) == expected4);

    // Case 5: Missing numbers interspersed, duplicates later.
    std::vector<int> input5 = {3, 1, 3, 2, 4, 2};
    std::vector<int> expected5 = {3, 1, 2, 4};
    assert(rearrangeElements(input5) == expected5);

    // Case 6: No duplicates but missing numbers.
    std::vector<int> input6 = {4, 1, 3};
    std::vector<int> expected6 = {4, 1, 3, 2};
    assert(rearrangeElements(input6) == expected6);

    // Case 7: Larger n with all duplicates except one.
    std::vector<int> input7 = {5, 5, 5, 5, 5};
    std::vector<int> expected7 = {5, 1, 2, 3, 4};
    assert(rearrangeElements(input7) == expected7);

    // Case 8: n = 2 with both elements equal.
    std::vector<int> input8 = {2, 2};
    std::vector<int> expected8 = {2, 1};
    assert(rearrangeElements(input8) == expected8);

    // Case 9: Input with first occurrence order already ascending.
    std::vector<int> input9 = {1, 2, 3, 4};
    std::vector<int> expected9 = {1, 2, 3, 4};
    assert(rearrangeElements(input9) == expected9);

    // Case 10: All missing except the last number.
    std::vector<int> input10 = {3, 3, 3, 3};
    std::vector<int> expected10 = {3, 1, 2, 4};
    assert(rearrangeElements(input10) == expected10);

    return 0;
}
