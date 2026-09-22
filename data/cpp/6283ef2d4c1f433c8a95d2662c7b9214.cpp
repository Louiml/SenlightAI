Write a C++ function that takes a vector of integers and returns a new vector containing all elements at even indices (0-based), starting from index 0, in their original relative order. The function must not modify the input vector. Additionally, make the function accept a vector with any number of elements (including zero) and handle it correctly. The returned vector should be sorted in ascending order before being returned.
The solution iterates through the input vector using standard iteration, collecting elements whose indices are even (0, 2, 4, ...). Since the task requires the output to be sorted, we first gather the even-indexed elements into a result vector, then sort the result in ascending order using `std::sort`. Edge cases include an empty input vector (returns an empty vector), a single-element vector (returns that same element), and vectors where the last even index is the final index (e.g., size 5 → indices 0,2,4). Time complexity is O(n log n) because of the sorting step, where n is the number of even-indexed elements. Space complexity is O(n) for the returned vector. The input vector is passed by const reference to guarantee it isn't modified.
#include <vector>
#include <algorithm>

// Returns a new vector containing all elements at even indices (0-based) from the input,
// sorted in ascending order. The original input is not modified.
std::vector<int> evenIndexSorted(const std::vector<int>& input) {
    std::vector<int> result;
    for (std::vector<int>::size_type i = 0; i < input.size(); i += 2) {
        result.push_back(input[i]);
    }
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared here (or included from the solution above).

int main() {
    // Basic case with multiple even indices
    std::vector<int> v1 = {5, 3, 8, 1, 9, 2};
    assert(evenIndexSorted(v1) == std::vector<int>({5, 8, 9})); // indices 0,2,4 → {5,8,9} sorted = {5,8,9}

    // Unsorted even-index elements
    std::vector<int> v2 = {10, 0, 3, 7, 1, 4};
    assert(evenIndexSorted(v2) == std::vector<int>({1, 3, 10})); // indices 0,2,4 → {10,3,1} sorted = {1,3,10}

    // Single element vector
    std::vector<int> v3 = {42};
    assert(evenIndexSorted(v3) == std::vector<int>({42}));

    // Empty vector
    std::vector<int> v4;
    assert(evenIndexSorted(v4) == std::vector<int>());

    // Vector with even number of elements
    std::vector<int> v5 = {4, 2, 6, 1};
    assert(evenIndexSorted(v5) == std::vector<int>({4, 6})); // indices 0,2 → {4,6} sorted = {4,6}

    // Negative numbers and duplicates
    std::vector<int> v6 = {-3, 5, -3, 2, 7, 0};
    assert(evenIndexSorted(v6) == std::vector<int>({-3, -3, 7})); // indices 0,2,4 → {-3,-3,7} sorted = {-3,-3,7}

    return 0;
}
