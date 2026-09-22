/*
Write a C++ function that takes a `const std::vector<int>&` as input and returns a `std::vector<int>` containing only the **unique elements** of the input in the order they first appear (i.e., preserve relative order, removing duplicates). The function must also handle an empty input vector by returning an empty vector. For example, given `{1, 2, 3, 9, 1, 9}`, the output should be `{1, 2, 3, 9}`. The solution should not modify the input and must be efficient even for large inputs.
*/

#include <vector>
#include <unordered_set>

// Returns a vector containing only the first occurrence of each distinct element in input,
// preserving the relative order of first appearances.
std::vector<int> uniqueInOrder(const std::vector<int>& input) {
    std::vector<int> result;
    std::unordered_set<int> seen;
    result.reserve(input.size()); // optional: avoid reallocations
    for (int value : input) {
        if (seen.insert(value).second) { // insert returns pair; .second is true if newly inserted
            result.push_back(value);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case from the snippet
    std::vector<int> v1 = {1, 2, 3, 9, 1, 9};
    assert((uniqueInOrder(v1) == std::vector<int>{1, 2, 3, 9}));

    // Empty input
    std::vector<int> v2;
    assert(uniqueInOrder(v2).empty());

    // All duplicates
    std::vector<int> v3 = {5, 5, 5, 5};
    assert((uniqueInOrder(v3) == std::vector<int>{5}));

    // Already unique
    std::vector<int> v4 = {10, 20, 30};
    assert((uniqueInOrder(v4) == std::vector<int>{10, 20, 30}));

    // Negative numbers and order preservation
    std::vector<int> v5 = {-1, -1, 0, 2, 0, -2, -2};
    assert((uniqueInOrder(v5) == std::vector<int>{-1, 0, 2, -2}));

    // Single element
    std::vector<int> v6 = {42};
    assert((uniqueInOrder(v6) == std::vector<int>{42}));
}

// Use an `unordered_set<int>` to track which values have already been seen, and iterate through the input vector once. For each element, if it is not already present in the set, add it to the result vector and insert it into the set. This ensures each value is only added once, while preserving the original first-seen order. Edge cases include an empty input (return an empty vector) and inputs with all duplicate values (result has one element). Time complexity is O(n) average-case (due to hash set insert/lookup), and O(n) worst-case if hash collisions are heavy; space complexity is O(n) for the set and result vector. The function must be `const`-correct, taking a const reference and returning a new vector without modifying the input.
