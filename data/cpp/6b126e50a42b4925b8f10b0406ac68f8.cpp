Write a C++ function `generateOddNumbersSequence` that takes a positive integer `n` and returns a `std::vector<int>` containing the first `n` positive odd numbers in increasing order (i.e., 1, 3, 5, ..., 2n-1). The function must be self-contained, using only standard library components, and must not read from standard input or write to standard output. The task should handle edge cases such as `n = 1` and `n = 0` (if `n == 0`, return an empty vector). The function must be `const`-correct and use descriptive variable names.

#include <cassert>
#include <vector>

// Function declaration (same as in solution)
std::vector<int> generateOddNumbersSequence(int n);

int main() {
    // n = 0 -> empty vector
    assert(generateOddNumbersSequence(0).empty());

    // n = 1 -> single element {1}
    assert(generateOddNumbersSequence(1) == std::vector<int>({1}));

    // n = 5 -> first five odd numbers
    assert(generateOddNumbersSequence(5) == std::vector<int>({1, 3, 5, 7, 9}));

    // n = 2 -> {1, 3}
    assert(generateOddNumbersSequence(2) == std::vector<int>({1, 3}));

    // n = 10 -> check last element is 19
    auto result = generateOddNumbersSequence(10);
    assert(result.size() == 10);
    assert(result.front() == 1);
    assert(result.back() == 19);

    // n = 3 -> {1, 3, 5}
    assert(generateOddNumbersSequence(3) == std::vector<int>({1, 3, 5}));

    // n = 4 -> {1, 3, 5, 7}
    assert(generateOddNumbersSequence(4) == std::vector<int>({1, 3, 5, 7}));

    // n = 6 -> check no even numbers
    auto evenCheck = generateOddNumbersSequence(6);
    for (const int& val : evenCheck) {
        assert(val % 2 == 1);
    }

    // n = 100 -> verify size and first element
    auto large = generateOddNumbersSequence(100);
    assert(large.size() == 100);
    assert(large[0] == 1);
    assert(large[99] == 199);

    return 0;
}

#include <vector>

// Returns the first n positive odd integers: 1, 3, 5, ..., 2n-1.
// For n == 0, returns an empty vector.
std::vector<int> generateOddNumbersSequence(int n) {
    std::vector<int> result;
    result.reserve(n); // avoid reallocations

    for (int i = 1; i <= n; ++i) {
        result.push_back(2 * i - 1);
    }

    return result;
}

// The solution is straightforward: iterate from 1 to `n` and for each index `i` (1-based), compute the i-th odd number as `2*i - 1`. Append each computed value to a vector and return it. Since the sequence is deterministic and each element depends only on its position, there are no special algorithmic challenges. Edge cases: when `n == 0`, the loop does not execute and an empty vector is returned; when `n == 1`, the output is `{1}`. Time complexity is O(n) because we fill a vector of size `n`. Space complexity is O(n) for the output vector, which is required to store the result. No auxiliary data structures are needed beyond the output vector itself.
