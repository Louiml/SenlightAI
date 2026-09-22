Write a C++ function named `removeAndReport` that takes a `const std::vector<int>&` input and returns a `std::pair<std::vector<int>, std::vector<int>>`. The first vector of the pair must contain the original elements in their original order except that all occurrences of the value `5` are removed. The second vector must contain the original elements in their original order except that all even numbers are removed (i.e., only odd numbers remain). Both output vectors must contain exactly the surviving elements, with no extra trailing elements or unspecified values—meaning you must physically shrink the vectors (via `erase` or constructing new vectors) to match the count of surviving elements. Additionally, the function must not modify the input vector, and it must handle edge cases such as an empty input, an input containing only `5`s, an input containing only even numbers, and an input containing only odd numbers.
// The key is to leverage the standard library algorithms `std::remove` and `std::remove_if` to shift the intended elements to the front, then use `std::vector::erase` to truncate the vector to the logical end returned by these algorithms. Because we must not modify the input, we first copy the input into two separate local vectors (or directly construct new vectors from the input and operate on them). This ensures the original data stays untouched. For the first vector, `std::remove` moves all elements not equal to `5` to the front and returns an iterator to the new logical end; calling `erase` from that iterator to `end()` removes the duplicates at the tail. For the second vector, `std::remove_if` with a lambda `[](int n){ return n % 2 == 0; }` does the same but removes even numbers; then we erase the tail. Edge cases naturally work: empty input yields empty vectors; all `5`s yields an empty first vector; all evens yields an empty second vector; all odds yields the full second vector. The time complexity is O(n) for each copy and each remove (since `std::remove` and `std::remove_if` run in linear time per element), so overall O(n) with O(n) auxiliary space for the two vector copies.
#include <vector>
#include <utility>
#include <algorithm>

// Returns a pair of vectors:
// first: original elements with every 5 removed
// second: original elements with every even number removed
std::pair<std::vector<int>, std::vector<int>> removeAndReport(const std::vector<int>& input) {
    // Copy input into two separate working vectors
    std::vector<int> withoutFives = input;
    std::vector<int> withoutEvens = input;

    // Remove all occurrences of 5
    auto endFives = std::remove(withoutFives.begin(), withoutFives.end(), 5);
    withoutFives.erase(endFives, withoutFives.end());

    // Remove all even numbers
    auto endEvens = std::remove_if(withoutEvens.begin(), withoutEvens.end(),
                                   [](int n) { return n % 2 == 0; });
    withoutEvens.erase(endEvens, withoutEvens.end());

    return {withoutFives, withoutEvens};
}
#include <cassert>
#include <vector>
#include <utility>

// Assume removeAndReport is defined as above
// (Include the solution code here in the same file for testing)

int main() {
    // Basic mixed input
    {
        std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto result = removeAndReport(input);
        assert((result.first == std::vector<int>{1, 2, 3, 4, 6, 7, 8, 9, 10}));
        assert((result.second == std::vector<int>{1, 3, 5, 7, 9}));
    }

    // Empty input
    {
        std::vector<int> input;
        auto result = removeAndReport(input);
        assert(result.first.empty());
        assert(result.second.empty());
    }

    // Only 5s – first vector empty, second keeps 5s (which are odd)
    {
        std::vector<int> input = {5, 5, 5};
        auto result = removeAndReport(input);
        assert(result.first.empty());
        assert((result.second == std::vector<int>{5, 5, 5}));
    }

    // Only even numbers – second vector empty, first keeps evens
    {
        std::vector<int> input = {2, 4, 6};
        auto result = removeAndReport(input);
        assert((result.first == std::vector<int>{2, 4, 6}));
        assert(result.second.empty());
    }

    // Only odd numbers – second vector unchanged, first keeps all (no 5s)
    {
        std::vector<int> input = {1, 3, 7, 9};
        auto result = removeAndReport(input);
        assert((result.first == std::vector<int>{1, 3, 7, 9}));
        assert((result.second == std::vector<int>{1, 3, 7, 9}));
    }

    // All 5s and evens – both vectors empty
    {
        std::vector<int> input = {5, 2, 5, 4};
        auto result = removeAndReport(input);
        assert(result.first.empty());
        assert(result.second.empty());
    }

    // Single element, not 5 and odd
    {
        std::vector<int> input = {7};
        auto result = removeAndReport(input);
        assert((result.first == std::vector<int>{7}));
        assert((result.second == std::vector<int>{7}));
    }

    // Duplicate 5s and evens mixed
    {
        std::vector<int> input = {5, 5, 6, 6, 5, 1};
        auto result = removeAndReport(input);
        assert((result.first == std::vector<int>{6, 6, 1}));
        assert((result.second == std::vector<int>{5, 5, 5, 1}));
    }

    // Original input must remain unchanged
    {
        std::vector<int> input = {1, 5, 2, 5, 3};
        std::vector<int> original = input;
        removeAndReport(input);
        assert(input == original);
    }

    return 0;
}
