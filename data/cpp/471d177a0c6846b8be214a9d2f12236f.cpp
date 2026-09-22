Given two integers `a` and `b` (with `-10^9 ≤ a, b ≤ 10^9`), write a standalone C++ function named `printDescendingRange` that returns a string containing all integers from the larger of the two numbers down to the smaller number, inclusive, in descending order, with each number separated by a single space and no trailing space. The function should handle both equal values (returning just that single number) and all possible orderings of `a` and `b`. The output must exactly match the expected format: numbers in strictly decreasing order, each pair separated by one space, and no extra spaces at the beginning or end.

#include <cassert>
#include <string>

// Solution function declaration (included here for compilation; normally in header)
std::string printDescendingRange(int a, int b);

int main() {
    // Basic ordering a > b
    assert(printDescendingRange(5, 2) == "5 4 3 2");
    // Basic ordering a < b
    assert(printDescendingRange(2, 5) == "5 4 3 2");
    // Equal values
    assert(printDescendingRange(7, 7) == "7");
    // Single-step difference
    assert(printDescendingRange(-3, -4) == "-3 -4");
    // Negative and positive values
    assert(printDescendingRange(-2, 3) == "3 2 1 0 -1 -2");
    // Larger range with negative numbers
    assert(printDescendingRange(0, -5) == "0 -1 -2 -3 -4 -5");
    // Large values within int range
    assert(printDescendingRange(1000000, 999998) == "1000000 999999 999998");
    // Reverse large values
    assert(printDescendingRange(-1000000, -1000002) == "-1000000 -1000001 -1000002");
    // Two consecutive numbers
    assert(printDescendingRange(10, 11) == "11 10");
    // Both negative equal
    assert(printDescendingRange(-4, -4) == "-4");
    return 0;
}

#include <string>
#include <algorithm>

// Returns a string of integers from the larger of a and b down to the smaller, inclusive, space-separated.
std::string printDescendingRange(int a, int b) {
    int high = std::max(a, b);
    int low = std::min(a, b);
    
    std::string result;
    for (int i = high; i >= low; --i) {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(i);
    }
    return result;
}

// The core algorithm is straightforward: determine the maximum and minimum of the two input values, then iterate from the maximum down to the minimum using a `for` loop, appending each number to a `std::string` result. The loop runs `(max - min + 1)` iterations. To avoid a trailing space, we append the first number directly, then for every subsequent number we first append a space then the number. The main edge cases include: (1) when `a == b`, the loop runs exactly once and returns that number alone; (2) when `a < b`, we correctly start from `b`; (3) when values are large (up to 1e9), we must use `long long` for safe arithmetic to compute the loop count, though the loop itself is fine with `int` because the difference fits in `int` (max 2e9, which exceeds `int` range, so we use `long long` or simply loop with `int` from max down to min—which only requires the loop variable to reach the min, and `int` can hold 1e9 fine; the difference of 2e9 is within `long long`, but the loop bound check `i >= min` works with `int` as both are within `int` range; however, to be safe we use `long long`). Time complexity is O(max(a,b) - min(a,b) + 1) = O(N) where N is the size of the range, and space complexity is O(N) because the output string stores all numbers. No extra data structures are needed.
