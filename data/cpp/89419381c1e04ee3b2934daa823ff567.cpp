Write a C++ function `rearrangeAlternating` that takes a vector of integers as input and returns a new vector where the integers originally at even indices (0, 2, 4, …) are sorted in non-decreasing order, and the integers originally at odd indices (1, 3, 5, …) are sorted in non-increasing order, while preserving the original parity positions in the output. For example, given input `{5, 2, 1, 9, 4, 7}`, the even-indexed elements are `{5, 1, 4}` (sorted ascending → `{1, 4, 5}`) and the odd-indexed elements are `{2, 9, 7}` (sorted descending → `{9, 7, 2}`), producing output `{1, 9, 4, 7, 5, 2}`. The input vector will contain at least one integer, and you may assume no integer overflow issues. The function must not modify the input vector.

#include <cassert>
#include <vector>

int main() {
    // Test 1: basic mixed case
    std::vector<int> input1 = {5, 2, 1, 9, 4, 7};
    std::vector<int> expected1 = {1, 9, 4, 7, 5, 2};
    assert(rearrangeAlternating(input1) == expected1);

    // Test 2: single element
    std::vector<int> input2 = {42};
    std::vector<int> expected2 = {42};
    assert(rearrangeAlternating(input2) == expected2);

    // Test 3: already sorted but parity requires different
    std::vector<int> input3 = {1, 3, 2, 4};
    std::vector<int> expected3 = {1, 4, 2, 3};  // even: {1,2}->{1,2}, odd:{3,4}->{4,3}
    assert(rearrangeAlternating(input3) == expected3);

    // Test 4: all equal
    std::vector<int> input4 = {7, 7, 7, 7};
    std::vector<int> expected4 = {7, 7, 7, 7};
    assert(rearrangeAlternating(input4) == expected4);

    // Test 5: negative numbers
    std::vector<int> input5 = {-1, -5, 3, 2, 0};
    std::vector<int> expected5 = {-1, 2, 0, -5, 3};  // even:{-1,3,0}->{-1,0,3}, odd:{-5,2}->{2,-5}
    assert(rearrangeAlternating(input5) == expected5);

    // Test 6: odd length with descending odd
    std::vector<int> input6 = {9, 1, 8, 2, 7};
    std::vector<int> expected6 = {7, 2, 8, 1, 9};  // even:{9,8,7}->{7,8,9}, odd:{1,2}->{2,1}
    assert(rearrangeAlternating(input6) == expected6);

    // Test 7: verify input not modified
    std::vector<int> original = {4, 3, 2, 1};
    std::vector<int> copy = original;
    rearrangeAlternating(original);
    assert(original == copy);
}

#include <vector>
#include <algorithm>
#include <cassert>

// Rearrange a vector so that even-indexed elements are sorted ascending
// and odd-indexed elements are sorted descending, preserving parity positions.
std::vector<int> rearrangeAlternating(const std::vector<int>& input) {
    std::vector<int> even;
    std::vector<int> odd;

    // Separate elements by index parity
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (i % 2 == 0)
            even.push_back(input[i]);
        else
            odd.push_back(input[i]);
    }

    // Sort even indices ascending, odd indices descending
    std::sort(even.begin(), even.end());
    std::sort(odd.begin(), odd.end(), std::greater<int>());

    // Merge back preserving parity
    std::vector<int> result;
    result.reserve(input.size());
    std::size_t even_idx = 0;
    std::size_t odd_idx = 0;
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (i % 2 == 0)
            result.push_back(even[even_idx++]);
        else
            result.push_back(odd[odd_idx++]);
    }
    return result;
}

// The solution separates the elements based on their original index parity: collect all even-indexed elements into one vector and all odd-indexed elements into another. Then sort the even-indexed vector in ascending order using `std::sort`, and sort the odd-indexed vector in descending order using `std::sort` with `std::greater<int>()`. After sorting, merge the two vectors back by iterating over the original positions: for each index `i`, if `i` is even, take the next element from the sorted even vector; if odd, take the next from the sorted odd vector. Edge cases: when the input size is 1, the odd vector is empty, so the output is simply the sorted even vector (which contains one element). When the size is even, both vectors have equal length; when odd, the even vector has one more element than the odd vector. The algorithm runs in O(n log n) time due to sorting (where n is the number of elements), and uses O(n) auxiliary space to store the two separated vectors and the output vector (the output vector counts as required result storage). The input is not modified because we copy elements into new vectors.
