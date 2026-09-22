Write a C++ function that takes a `std::vector<int>` representing the weights of a flock of ducks and returns a `std::vector<std::string>` containing the original names of the ducks (provided in a parallel `std::vector<std::string>` of the same size) in ascending order of their weights. If two or more ducks have the same weight, their names must appear in the same relative order as they appeared in the input (stable sort). The function must not modify the input vectors, must work with empty vectors (returning an empty result), and must not rely on the `Duck` class or any external data structures other than standard headers.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above in the solution section.
// For testing, we include it here directly.

int main() {
    // Basic sorting with distinct weights
    {
        std::vector<std::string> names = {"Daffy", "Dewey", "Howard", "Donald"};
        std::vector<int> weights = {8, 2, 7, 10};
        std::vector<std::string> expected = {"Dewey", "Howard", "Daffy", "Donald"};
        assert(sortDucksByName(names, weights) == expected);
    }

    // Stable sorting with equal weights: preserve input order
    {
        std::vector<std::string> names = {"Dewey", "Louis", "Huey", "Daffy"};
        std::vector<int> weights = {2, 2, 2, 8};
        std::vector<std::string> expected = {"Dewey", "Louis", "Huey", "Daffy"};
        assert(sortDucksByName(names, weights) == expected);
    }

    // Mixed equal weights and different order
    {
        std::vector<std::string> names = {"B", "A", "C", "D"};
        std::vector<int> weights = {3, 3, 1, 2};
        std::vector<std::string> expected = {"C", "D", "B", "A"};
        assert(sortDucksByName(names, weights) == expected);
    }

    // Single element
    {
        std::vector<std::string> names = {"Only"};
        std::vector<int> weights = {42};
        std::vector<std::string> expected = {"Only"};
        assert(sortDucksByName(names, weights) == expected);
    }

    // Empty vectors
    {
        std::vector<std::string> names;
        std::vector<int> weights;
        assert(sortDucksByName(names, weights).empty());
    }

    // Mismatched sizes -> return empty
    {
        std::vector<std::string> names = {"A", "B"};
        std::vector<int> weights = {1};
        assert(sortDucksByName(names, weights).empty());
    }

    // Negative weights are handled correctly
    {
        std::vector<std::string> names = {"Min", "Mid", "Max"};
        std::vector<int> weights = {-5, 0, -3};
        std::vector<std::string> expected = {"Min", "Max", "Mid"};
        assert(sortDucksByName(names, weights) == expected);
    }

    // Large weights and repeating all weights
    {
        std::vector<std::string> names = {"a", "b", "c", "d"};
        std::vector<int> weights = {7, 7, 7, 7};
        std::vector<std::string> expected = {"a", "b", "c", "d"};
        assert(sortDucksByName(names, weights) == expected);
    }
}
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>

// Returns names sorted by corresponding weights; stable for equal weights.
std::vector<std::string> sortDucksByName(const std::vector<std::string>& names,
                                         const std::vector<int>& weights) {
    std::vector<std::string> result;
    std::size_t n = names.size();
    if (n == 0 || weights.size() != n) {
        return result;
    }

    std::vector<std::size_t> indices(n);
    for (std::size_t i = 0; i < n; ++i) {
        indices[i] = i;
    }

    std::sort(indices.begin(), indices.end(),
        [&](std::size_t i, std::size_t j) {
            if (weights[i] != weights[j]) {
                return weights[i] < weights[j];
            }
            return i < j;  // stable tie-breaker by original index
        });

    result.reserve(n);
    for (std::size_t idx : indices) {
        result.push_back(names[idx]);
    }
    return result;
}
// The solution needs to sort names by corresponding weights, but the sort must be stable (preserve the original order for equal weights). A simple approach is to create a vector of index numbers `0...n-1`, then sort these indices using a custom comparator that compares `weights[i]` and `weights[j]`; if equal, the comparator returns `i < j` to enforce the original relative order. Finally, build the result by iterating over the sorted indices and pushing back `names[index]`. This avoids copying or modifying the input vectors and works for any size including zero. Time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the index vector and the result vector. Edge cases include empty input (return empty vector), all weights equal (the index-based tie-breaker preserves the exact input order), and negative or zero weights (no special handling needed since they just compare as integers).
