Write a C++ function `interleaveFromEnds` that takes a non-empty vector of integers and returns a new vector containing the elements rearranged as follows: output the first element, then the last element, then the second element, then the second-to-last element, and so on, moving inward. For example, given `[1,2,3,4,5]`, the result should be `[1,5,2,4,3]`. If the vector has an odd length, the middle element appears once at the end of the output. The function must not modify the input vector and must handle all non-negative lengths, including vectors of size 1 and 2. The input vector elements can be any integers (positive, negative, or zero).
// The solution processes the input vector from both ends simultaneously using two indices: `left` starting at 0 and `right` starting at `n-1`. In each iteration, we push the element at `left` and the element at `right` into the result, then increment `left` and decrement `right`. This continues while `left < right`. If the array has an odd length, after the loop terminates, `left` will equal `right`, and we push that single middle element. For even lengths, the loop stops when `left > right`, leaving no middle element. The time complexity is O(n) because each element is visited exactly once. The space complexity is O(n) for the result vector; no additional data structures are used beyond a few integer variables. Edge cases: a vector of size 1 returns the same single element; a vector of size 2 returns the two elements in their original order (first, then last); negative numbers and zeros work fine because no comparison is performed—only indexing and copying.
#include <vector>

// Rearrange a vector by interleaving elements from the ends moving inward.
// Example: [1,2,3,4,5] -> [1,5,2,4,3]
std::vector<int> interleaveFromEnds(const std::vector<int>& input) {
    std::vector<int> result;
    result.reserve(input.size());
    size_t left = 0;
    size_t right = input.size() - 1;

    while (left < right) {
        result.push_back(input[left]);
        result.push_back(input[right]);
        ++left;
        --right;
    }

    // For odd-length vectors, add the middle element.
    if (left == right) {
        result.push_back(input[left]);
    }

    return result;
}
#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above or included via header.)

int main() {
    // Basic odd-length case
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(interleaveFromEnds(v1) == std::vector<int>({1, 5, 2, 4, 3}));

    // Basic even-length case
    std::vector<int> v2 = {10, 20, 30, 40};
    assert(interleaveFromEnds(v2) == std::vector<int>({10, 40, 20, 30}));

    // Single element
    std::vector<int> v3 = {7};
    assert(interleaveFromEnds(v3) == std::vector<int>({7}));

    // Two elements
    std::vector<int> v4 = {5, 6};
    assert(interleaveFromEnds(v4) == std::vector<int>({5, 6}));

    // Negative numbers and zeros
    std::vector<int> v5 = {-1, 0, 2, -3, 4};
    assert(interleaveFromEnds(v5) == std::vector<int>({-1, 4, 0, -3, 2}));

    // Larger vector with repeated values
    std::vector<int> v6 = {1, 1, 2, 2, 3, 3, 4};
    assert(interleaveFromEnds(v6) == std::vector<int>({1, 4, 1, 3, 2, 3, 2}));

    // Input remains unmodified
    std::vector<int> v7 = {9, 8, 7};
    std::vector<int> original = v7;
    interleaveFromEnds(v7);
    assert(v7 == original);
}
