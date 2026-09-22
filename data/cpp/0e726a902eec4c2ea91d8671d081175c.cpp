/*
Write a C++ function named `sumPairs` that accepts a vector of pairs of integers and returns a vector of integers containing the sum of each pair, in the same order as the input pairs. The input vector will contain at least one pair, and each integer will fit within the standard `int` range. The function must be `const`-correct (i.e., take the input by `const` reference) and must not modify the input. The returned vector should have exactly the same size as the input vector. For example, if the input is `{{3, 5}, {10, -2}, {0, 0}}`, the output should be `{8, 8, 0}`. The function should work efficiently for large vectors, up to 10^6 pairs.
*/

#include <vector>
#include <utility>

// Compute the sum of each pair of integers and return them in a new vector.
std::vector<int> sumPairs(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<int> result;
    result.reserve(pairs.size());

    for (const auto& p : pairs) {
        result.push_back(p.first + p.second);
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// (Assume the solution function is already declared above this point.)

int main() {
    // Basic case
    std::vector<std::pair<int, int>> test1 = {{3, 5}, {10, -2}, {0, 0}};
    assert(sumPairs(test1) == std::vector<int>({8, 8, 0}));

    // Single pair
    std::vector<std::pair<int, int>> test2 = {{-7, 7}};
    assert(sumPairs(test2) == std::vector<int>({0}));

    // All negative pairs
    std::vector<std::pair<int, int>> test3 = {{-1, -2}, {-5, -5}};
    assert(sumPairs(test3) == std::vector<int>({-3, -10}));

    // Large values (within int range)
    std::vector<std::pair<int, int>> test4 = {{100000, 200000}, {-30000, -40000}};
    assert(sumPairs(test4) == std::vector<int>({300000, -70000}));

    // Pairs with zeros
    std::vector<std::pair<int, int>> test5 = {{0, 0}, {0, 1}, {1, 0}};
    assert(sumPairs(test5) == std::vector<int>({0, 1, 1}));

    // Many pairs (stress check with small loop)
    std::vector<std::pair<int, int>> test6;
    for (int i = 0; i < 100; ++i) {
        test6.push_back({i, i});
    }
    std::vector<int> expected6;
    for (int i = 0; i < 100; ++i) {
        expected6.push_back(2 * i);
    }
    assert(sumPairs(test6) == expected6);

    return 0;
}

// The solution is straightforward: iterate through each pair in the input vector, compute the sum of its two integer components, and push the result into a new vector that will be returned. Since the input is read-only, we take it by `const std::vector<std::pair<int, int>>&` to avoid copying. We reserve the output vector's capacity to the input's size to avoid reallocations. Edge cases include empty pairs (but the problem guarantees at least one pair) and negative numbers, which sum normally with C++’s built-in integer arithmetic. The time complexity is O(n), where n is the number of pairs, because we visit each pair exactly once. The space complexity is O(n) for the output vector, plus O(1) auxiliary space for the loop and temporary sum. No special handling is needed for overflow because the problem guarantees results fit within `int`.
