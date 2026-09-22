// Write a C++ function that takes a vector of integers and returns a new vector containing only the distinct elements in ascending sorted order, preserving no duplicates. The function should be named `uniqueSorted` and should accept a `const std::vector<int>&` as input, returning `std::vector<int>`. The input vector may contain any number of elements, including zero, negative values, and many duplicates. The output must be sorted ascending and contain each distinct value exactly once.
#include <cassert>
#include <vector>

// forward declaration for testing
std::vector<int> uniqueSorted(const std::vector<int>&);

int main() {
    assert(uniqueSorted({3, 3, 3, 5, 6, 9}) == std::vector<int>({3, 5, 6, 9}));
    assert(uniqueSorted({}) == std::vector<int>({}));
    assert(uniqueSorted({1}) == std::vector<int>({1}));
    assert(uniqueSorted({5, 5, 5}) == std::vector<int>({5}));
    assert(uniqueSorted({-2, 0, -2, 3, 0, 1}) == std::vector<int>({-2, 0, 1, 3}));
    assert(uniqueSorted({10, 9, 8, 7}) == std::vector<int>({7, 8, 9, 10}));
    assert(uniqueSorted({2, 2, 1, 1, 3, 3}) == std::vector<int>({1, 2, 3}));
    assert(uniqueSorted({-1, -1, 0, 0, -1}) == std::vector<int>({-1, 0}));
    assert(uniqueSorted({100, -100, 100, -100}) == std::vector<int>({-100, 100}));
    assert(uniqueSorted({0, 0, 0}) == std::vector<int>({0}));
}
#include <vector>
#include <set>

// Return a vector of distinct elements from the input, sorted ascending.
std::vector<int> uniqueSorted(const std::vector<int>& input) {
    std::set<int> distinct(input.begin(), input.end());
    return std::vector<int>(distinct.begin(), distinct.end());
}
// The simplest and most efficient approach is to use a `std::set<int>`, which automatically stores elements in sorted order and removes duplicates. Insert all elements from the input vector into the set, then copy the set's contents into a new vector. This leverages the set's internal balanced binary search tree, ensuring `O(log n)` per insertion and `O(n log n)` total time for `n` elements. The space complexity is `O(n)` due to the set and output vector. If the input is empty, return an empty vector. An alternative would be to sort the vector and use `std::unique`, but the set approach is cleaner and directly matches the required behavior. Edge cases: empty input returns empty output; all elements identical returns a single-element vector; negative numbers sort naturally.
