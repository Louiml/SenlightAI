// Write a C++ function named `printFromFourth` that reads exactly seven integers from standard input using `std::cin`, and returns a string containing only the last four integers (the 4th through 7th values) separated by a single space each, with no trailing space. The function must ignore the first three integers entirely. The integers can be any positive or negative values within the range of `int`. The output string should contain exactly the sequence of the 4th, 5th, 6th, and 7th input integers in the order they were read, separated by spaces. If fewer than seven integers are provided in the input, the function still processes whatever is available but should only include the 4th and later values that actually were read; however, the main test will always provide exactly seven integers. Assume input is well-formed (no non-integer tokens). The function must be callable repeatedly; each call reads new input from `std::cin`.

// The task is straightforward: read seven integers in a loop, but only start accumulating values into a string after reading the third integer (i.e., for indices 3 through 6 in zero-based indexing). A common approach is to use a loop from 0 to 6, and for each iteration, read an integer into a temporary variable. If the current index is greater than or equal to 3, append that integer to a result string, appending a space before each value except when it is the first appended value, to avoid a trailing space. Alternative approaches could use a queue or an array to store the last four values, but because the count is fixed and small, direct conditional appending is simpler and memory efficient. Edge cases: if fewer than four integers are read (unlikely per problem statement), the string would be empty or incomplete; this is handled gracefully but not required for the test. Time complexity is O(7) = O(1) with respect to input size (constant), and space complexity is O(1) for the temporary variable and the output string size, which is at most a few characters (though the string itself stores the numbers, its length is bounded by a constant since only four numbers are output).

#include <string>
#include <iostream>

/**
 * Reads exactly seven integers from stdin and returns a string
 * containing the last four integers (4th through 7th) separated by spaces.
 * The first three integers are ignored.
 */
std::string printFromFourth() {
    std::string result;
    int value;
    bool first = true;

    for (int i = 0; i < 7; ++i) {
        std::cin >> value;  // assume valid input
        if (i >= 3) {
            if (!first) {
                result += ' ';
            }
            result += std::to_string(value);
            first = false;
        }
    }
    return result;
}

#include <cassert>
#include <sstream>
#include <iostream>

// The solution function is assumed to be declared above.
// For testing, we manipulate std::cin using a stringstream.

int main() {
    // Test 1: Normal case
    {
        std::istringstream input("1 2 3 4 5 6 7");
        std::cin.rdbuf(input.rdbuf());
        assert(printFromFourth() == "4 5 6 7");
    }

    // Test 2: All zeros
    {
        std::istringstream input("0 0 0 0 0 0 0");
        std::cin.rdbuf(input.rdbuf());
        assert(printFromFourth() == "0 0 0 0");
    }

    // Test 3: Negative numbers
    {
        std::istringstream input("-1 -2 -3 -4 -5 -6 -7");
        std::cin.rdbuf(input.rdbuf());
        assert(printFromFourth() == "-4 -5 -6 -7");
    }

    // Test 4: Mixed positive and negative
    {
        std::istringstream input("10 -20 30 -40 50 -60 70");
        std::cin.rdbuf(input.rdbuf());
        assert(printFromFourth() == "-40 50 -60 70");
    }

    // Test 5: Large numbers
    {
        std::istringstream input("1000000 2000000 3000000 4000000 5000000 6000000 7000000");
        std::cin.rdbuf(input.rdbuf());
        assert(printFromFourth() == "4000000 5000000 6000000 7000000");
    }

    return 0;
}
