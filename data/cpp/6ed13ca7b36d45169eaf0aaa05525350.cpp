/*
Write a C++ function that reads a sequence of integers from standard input until the sentinel value 42 is encountered. For every integer read before 42, the function must output that integer on its own line, preserving the original order. The function should stop reading and return immediately upon reading 42, without printing 42 itself. The input is guaranteed to contain at least one integer before the sentinel (so the function always outputs at least one line). The function must handle arbitrary numbers of inputs, including possible negative values and zero, but all inputs are valid integers. After the function returns, the program should terminate normally.
*/
#include <iostream>

// Reads integers from standard input and prints each one until 42 is encountered.
// The sentinel 42 is not printed, and reading stops immediately after it.
void processUntilSentinel() {
    int value;
    while (std::cin >> value) {
        if (value == 42) {
            break;
        }
        std::cout << value << '\n';
    }
}
#include <cassert>
#include <iostream>
#include <sstream>

// The function under test is defined elsewhere, but for testing we simulate input.
// Since the function directly uses std::cin, we need to redirect std::cin for tests.
// We'll provide a wrapper that takes an istringstream as input (not part of the solution).
void processUntilSentinelWithStream(std::istream& input, std::ostream& output) {
    int value;
    while (input >> value) {
        if (value == 42) {
            break;
        }
        output << value << '\n';
    }
}

int main() {
    // Test 1: Basic sequence
    {
        std::istringstream in("1 2 3 42 4 5");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "1\n2\n3\n");
    }
    // Test 2: Sentinel as first valid input? (Problem guarantees not, but test anyway)
    {
        std::istringstream in("42 7");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "");
    }
    // Test 3: Negative and zero values before sentinel
    {
        std::istringstream in("-5 0 7 42 100");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "-5\n0\n7\n");
    }
    // Test 4: Only one value before sentinel
    {
        std::istringstream in("10 42");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "10\n");
    }
    // Test 5: Large input, verify order and count
    {
        std::istringstream in("1 2 3 4 5 6 7 8 9 10 42");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");
    }
    // Test 6: Sentinel appears after many numbers, no trailing newline after last
    {
        std::istringstream in("100 200 300 42");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "100\n200\n300\n");
    }
    // Test 7: Input with extra whitespace and newlines
    {
        std::istringstream in("  7 \n 8 \t 9 \n 42 \n 10");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "7\n8\n9\n");
    }
    // Test 8: Sentinel not present (should print all, but problem says it will be there; test robustly)
    {
        std::istringstream in("5 6 7");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "5\n6\n7\n");
    }
    // Test 9: Zero as sentinel? No, sentinel is 42, so zeros print
    {
        std::istringstream in("0 42 0");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "0\n");
    }
    // Test 10: Single non‑42 integer only (no sentinel, but we break at EOF)
    {
        std::istringstream in("9");
        std::ostringstream out;
        processUntilSentinelWithStream(in, out);
        assert(out.str() == "9\n");
    }

    return 0;
}
// The solution is straightforward: repeatedly read an integer from standard input using `std::cin`. For each value read, check if it equals 42. If it does, break out of the loop and return. Otherwise, print the value followed by a newline. The main algorithm is a simple sentinel-controlled loop. Edge cases: the sentinel may be the very first input (but problem guarantees at least one non‑42 integer, so this won't happen); the input may contain whitespace, but `std::cin >> value` skips leading whitespace automatically; trailing newlines after 42 are ignored. Time complexity is O(N) where N is the number of integers read before the sentinel, and space complexity is O(1) auxiliary (only one integer variable). No special data structures needed.
