// Write a C++ function named `greatestCandies` that takes a non-empty vector of integers `candies` (representing the number of candies each child has) and an integer `extraCandies`, and returns a `vector<bool>` where each element at index `i` is `true` if the child at index `i` would have the greatest number of candies among all children after receiving `extraCandies`, and `false` otherwise. A child is considered to have the greatest number if, after adding `extraCandies` to their current candies, their total is greater than or equal to the maximum number of candies any child currently has. The input vector may contain any non-negative integers, and the function must not modify the input vector. Example: for `candies = {2, 3, 5, 1, 3}` and `extraCandies = 3`, the output should be `{true, true, true, false, true}`.
#include <cassert>
#include <vector>

// The function is defined here in the test section for completeness.
// In a real project, it would be in a separate header.
std::vector<bool> greatestCandies(const std::vector<int>& candies, int extraCandies) {
    if (candies.empty()) {
        return {};
    }

    int maxCandies = candies[0];
    for (size_t i = 1; i < candies.size(); ++i) {
        if (candies[i] > maxCandies) {
            maxCandies = candies[i];
        }
    }

    std::vector<bool> result;
    result.reserve(candies.size());
    for (int candy : candies) {
        result.push_back(candy + extraCandies >= maxCandies);
    }
    return result;
}

int main() {
    // Example from the prompt.
    std::vector<int> candies1 = {2, 3, 5, 1, 3};
    std::vector<bool> expected1 = {true, true, true, false, true};
    assert(greatestCandies(candies1, 3) == expected1);

    // Single child.
    assert(greatestCandies({7}, 0) == std::vector<bool>{true});
    assert(greatestCandies({7}, 5) == std::vector<bool>{true});

    // All already have the maximum.
    std::vector<int> candies2 = {4, 4, 4};
    assert(greatestCandies(candies2, 0) == std::vector<bool>{true, true, true});

    // Extra candies make everyone the greatest.
    std::vector<int> candies3 = {1, 2, 3};
    assert(greatestCandies(candies3, 5) == std::vector<bool>{true, true, true});

    // No one can reach the maximum.
    std::vector<int> candies4 = {1, 5, 3};
    assert(greatestCandies(candies4, 1) == std::vector<bool>{false, true, false});

    // Ties with the maximum already exist.
    std::vector<int> candies5 = {5, 5, 1};
    assert(greatestCandies(candies5, 0) == std::vector<bool>{true, true, false});

    // Zero candies and extra candies.
    std::vector<int> candies6 = {0, 0};
    assert(greatestCandies(candies6, 0) == std::vector<bool>{true, true});

    // Large values.
    std::vector<int> candies7 = {1000000, 1, 500000};
    assert(greatestCandies(candies7, 500000) == std::vector<bool>{true, false, true});

    return 0;
}
#include <vector>

// Given a vector of non-negative integers representing candies and an extra amount,
// return a vector<bool> where each element is true if that child would have the
// greatest number of candies after receiving extraCandies.
std::vector<bool> greatestCandies(const std::vector<int>& candies, int extraCandies) {
    if (candies.empty()) {
        return {};
    }

    // Find the current maximum number of candies among all children.
    int maxCandies = candies[0];
    for (size_t i = 1; i < candies.size(); ++i) {
        if (candies[i] > maxCandies) {
            maxCandies = candies[i];
        }
    }

    // Build the result by checking each child with the extra candies.
    std::vector<bool> result;
    result.reserve(candies.size());
    for (int candy : candies) {
        result.push_back(candy + extraCandies >= maxCandies);
    }

    return result;
}
// The solution has two main steps. First, find the maximum value in the `candies` vector by iterating through all elements and tracking the largest one. Second, for each candy count, check whether `candy + extraCandies` is greater than or equal to that maximum; if so, push `true` to the result vector, otherwise push `false`. Edge cases include a vector with a single element (the maximum is that element, so the result is always `true`), duplicates (the comparison uses `>=` so all children who tie with the current max and have enough extra candies will still be `true`), and large values (use `int`, but ensure no overflow in practice by noting `extraCandies` and candy counts are non‑negative and within typical `int` range). The time complexity is O(n) for finding the max plus O(n) for building the result, so O(n) overall, where n is the number of children. The space complexity is O(n) for the result vector (which is required to return), and O(1) auxiliary space aside from that.
