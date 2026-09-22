/*
Write a C++ function named `extractSubvector` that takes a `const std::vector<int>&` representing a list of numbers, along with two integer indices `start` and `end` (both inclusive), and returns a new `std::vector<int>` containing the elements of the input vector from index `start` through index `end` in their original order. You may assume that `start` and `end` are valid indices such that `0 ≤ start ≤ end < numbers.size()`. The function must work for vectors of any size, including a single-element vector where `start == end`, and must not modify the input vector. The returned vector should be a deep copy, not a view or reference.
*/
#include <vector>

// Return a new vector containing elements from numbers[start] to numbers[end] inclusive.
std::vector<int> extractSubvector(const std::vector<int>& numbers, int start, int end) {
    std::vector<int> result;
    // Guard against empty input; though valid indices are guaranteed, this is safe.
    if (numbers.empty()) {
        return result;
    }
    result.reserve(end - start + 1); // reserve space for efficiency
    for (int i = start; i <= end; ++i) {
        result.push_back(numbers[i]);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {10, 20, 30, 40, 50};
    assert(extractSubvector(v1, 1, 3) == std::vector<int>({20, 30, 40}));
    assert(extractSubvector(v1, 0, 4) == v1);
    assert(extractSubvector(v1, 2, 2) == std::vector<int>({30}));

    std::vector<int> v2 = {7};
    assert(extractSubvector(v2, 0, 0) == std::vector<int>({7}));

    std::vector<int> v3 = {-5, -1, 0, 3};
    assert(extractSubvector(v3, 0, 3) == v3);
    assert(extractSubvector(v3, 1, 2) == std::vector<int>({-1, 0}));

    std::vector<int> empty;
    assert(extractSubvector(empty, 0, 0).empty()); // safe fallback for empty input
}
// The solution simply iterates over the input vector from the given starting index to the ending index, copying each element into a new result vector. The main algorithm is a linear scan with a `for` loop, pushing each element into the answer. Edge cases include when `start == end` (returns a vector with exactly one element) and when the input vector is empty (though the problem guarantees valid indices, we can still handle it gracefully by returning an empty vector if the input is empty). Time complexity is \(O(n)\) where \(n = end - start + 1\) is the number of elements copied, and space complexity is \(O(n)\) because we allocate a new vector of exactly that size. Using `const` reference for the input ensures no accidental modification, and we avoid any unnecessary copying of the entire input.
