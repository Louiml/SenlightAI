Write a C++ function named `maxMinUntilSentinel` that repeatedly reads integer values from standard input until the sentinel value `-99` is entered. The sentinel itself must **not** be included when determining the maximum or minimum. The function should return a `std::pair<int, int>` containing the maximum value first and the minimum value second. The input is guaranteed to contain at least one non-sentinel integer before the sentinel appears. You may assume all inputs are valid integers, and the user may enter positive, negative, or zero values. The function must not print anything; it only reads from `std::cin` and returns the result.
The algorithm initializes maximum and minimum to a sentinel-like flag (e.g., `bool first = true` or initialize them after the first input). For each integer read, if it equals `-99`, break the loop. Otherwise, if it is the first non-sentinel value, set both max and min to that value; for subsequent values, update max and min using comparison. Edge cases include: the first value being the sentinel (which is disallowed by the guarantee), values being all equal, and negative numbers. Because we read until a sentinel, the number of iterations is unknown until runtime, but the time complexity is \(O(n)\) where \(n\) is the number of inputs before `-99`. Space complexity is \(O(1)\) because we only store a few variables, ignoring the input stream buffer.
#include <iostream>
#include <utility>

// Reads integers from std::cin until -99 is entered.
// Returns {maximum, minimum} of all non-sentinel values.
// Requires at least one non-sentinel value before the sentinel.
std::pair<int, int> maxMinUntilSentinel() {
    int value;
    int maximum = 0;
    int minimum = 0;
    bool first = true;

    while (std::cin >> value) {
        if (value == -99) {
            break;
        }
        if (first) {
            maximum = value;
            minimum = value;
            first = false;
        } else {
            if (value > maximum) {
                maximum = value;
            }
            if (value < minimum) {
                minimum = value;
            }
        }
    }
    return {maximum, minimum};
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function (normally in a header; here for completeness)
std::pair<int, int> maxMinUntilSentinel();

// Test harness that redirects cin
int main() {
    // Test 1: Provided sample sequence
    {
        std::istringstream input("12 31 4 22 34 50 129 33 45 32 50 -99\n");
        std::cin.rdbuf(input.rdbuf());
        auto result = maxMinUntilSentinel();
        assert(result.first == 129);
        assert(result.second == 4);
    }

    // Test 2: All negative numbers
    {
        std::istringstream input("-5 -1 -10 -99\n");
        std::cin.rdbuf(input.rdbuf());
        auto result = maxMinUntilSentinel();
        assert(result.first == -1);
        assert(result.second == -10);
    }

    // Test 3: Single non-sentinel value
    {
        std::istringstream input("42 -99\n");
        std::cin.rdbuf(input.rdbuf());
        auto result = maxMinUntilSentinel();
        assert(result.first == 42);
        assert(result.second == 42);
    }

    // Test 4: All equal values
    {
        std::istringstream input("7 7 7 7 -99\n");
        std::cin.rdbuf(input.rdbuf());
        auto result = maxMinUntilSentinel();
        assert(result.first == 7);
        assert(result.second == 7);
    }

    // Test 5: Zero and mixed values
    {
        std::istringstream input("0 -3 9 0 -99\n");
        std::cin.rdbuf(input.rdbuf());
        auto result = maxMinUntilSentinel();
        assert(result.first == 9);
        assert(result.second == -3);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
