// Write a C++ function named `sumUntilNegative` that reads a sequence of integers from standard input until a negative integer is encountered (the negative integer is not included in the sum), and returns the accumulated sum as an `int`. The input will contain at least one non-negative integer before any negative value. If the very first integer is negative, the function should return `0`. The function should not require any parameters and should handle an arbitrary number of integers until the terminator is found or end-of-input is reached (in which case it returns the sum of all non-negative integers read). Your implementation must not use global variables and must process input via standard input stream.
The solution uses a simple loop that repeatedly reads integers from `std::cin`. For each integer read, if it is negative, break the loop immediately without adding it. Otherwise, add it to an accumulator `sum`. The key edge case: if the first integer is negative, the loop breaks right away and returns `0`. Another edge case: if end-of-input occurs before any negative number (e.g., only positive integers are provided), the loop ends naturally and returns the sum of all those values. Time complexity is `O(k)` where `k` is the number of integers read until the terminator or EOF; space complexity is `O(1)` as only a single integer accumulator and a temporary variable are used.
#include <iostream>

// Reads integers from standard input until a negative integer is encountered.
// Returns the sum of all non-negative integers read; the negative terminator is excluded.
int sumUntilNegative() {
    int sum = 0;
    int value;
    while (std::cin >> value) {
        if (value < 0) {
            break;
        }
        sum += value;
    }
    return sum;
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declaration of the function under test
int sumUntilNegative();

// Test harness: manually redirect std::cin to test specific input sequences
int main() {
    // Test 1: normal case with terminator
    {
        std::istringstream input("1 2 3 -1");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 6);
    }
    // Test 2: first value is negative -> returns 0
    {
        std::istringstream input("-5 10 20");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 0);
    }
    // Test 3: no negative terminator, only positives
    {
        std::istringstream input("4 5 6");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 15);
    }
    // Test 4: single zero before negative
    {
        std::istringstream input("0 -1");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 0);
    }
    // Test 5: large numbers with negative at end
    {
        std::istringstream input("1000 2000 -999");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 3000);
    }
    // Test 6: empty input (should return 0)
    {
        std::istringstream input("");
        std::cin.rdbuf(input.rdbuf());
        assert(sumUntilNegative() == 0);
    }
    return 0;
}
