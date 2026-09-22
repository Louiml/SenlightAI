// Write a C++ function named `readUntilDigit` that takes an `std::istream&` parameter and returns the same stream reference. The function must read characters from the stream one at a time, skipping over any non-digit characters, until it encounters a digit character. It should then put that digit character back into the stream (using `putback`) before returning the stream. This mimics extracting an integer from a stream that may have leading non-digit characters. Your solution must handle any input stream (e.g., `std::cin`, `std::istringstream`) and must not alter the stream's state beyond what is necessary. The function should be robust against empty streams (i.e., if EOF is reached before a digit is found, it should return the stream in a failed state, but still return the reference).

#include <cassert>
#include <sstream>
#include <iostream>

// Use the solution function (declared above) in tests.
int main() {
    // Standard case: leading non-digits, then a digit, then an integer.
    {
        std::istringstream iss("abc123 45");
        assert(readUntilDigit(iss) && (iss >> std::ws)); // ensure stream is good and whitespace consumed
        int val = -1;
        iss >> val;
        assert(val == 123);
    }

    // No leading non-digits: digit is the first character.
    {
        std::istringstream iss("7");
        readUntilDigit(iss);
        int val = -1;
        iss >> val;
        assert(val == 7);
    }

    // Leading whitespace is skipped automatically.
    {
        std::istringstream iss("   \t 5  ");
        readUntilDigit(iss);
        int val = -1;
        iss >> val;
        assert(val == 5);
    }

    // Multiple digits: only the first is put back, rest remain.
    {
        std::istringstream iss("xy 42");
        readUntilDigit(iss);
        int val = -1;
        iss >> val;
        assert(val == 42);
    }

    // No digit at all: stream fails and returns itself.
    {
        std::istringstream iss("hello world");
        std::istream& result = readUntilDigit(iss);
        assert(!result); // stream is in failed state
    }

    // Empty stream.
    {
        std::istringstream iss("");
        std::istream& result = readUntilDigit(iss);
        assert(!result);
    }

    // After putback, the stream position is at the digit.
    {
        std::istringstream iss("abcd9");
        readUntilDigit(iss);
        char c;
        iss.get(c);
        assert(c == '9');
    }

    // Negative numbers: the minus sign is skipped, digit is read.
    {
        std::istringstream iss("-42");
        readUntilDigit(iss);
        int val = 0;
        iss >> val;
        assert(val == 42); // note: minus is gone, so sign is lost
    }

    // Ensure the function works with cin-like streams (here, using an istringstream).
    {
        std::istringstream iss("foo123bar");
        readUntilDigit(iss);
        int val = 0;
        iss >> val;
        assert(val == 123);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>
#include <cctype>
#include <istream>

// Reads and discards non-digit characters from the stream,
// then puts back the first digit encountered and returns the stream.
std::istream& readUntilDigit(std::istream& input) {
    char ch;
    // Read characters until a digit is found or the stream fails.
    while (input.get(ch)) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            input.putback(ch);
            break;
        }
        // Non-digit: discard and continue.
    }
    return input;
}

// The core idea is to repeatedly read a single character from the stream using the `>>` operator (which skips whitespace) and check if it is a digit using `std::isdigit`. If it is not a digit, discard it and continue. Once a digit is found, put it back using `istream::putback` so that subsequent extraction (like reading an integer) can consume it. The loop should terminate when either a digit is found or the stream fails (EOF or error). If we hit EOF first, we simply return the stream in its failed state—there is no digit to put back. Edge cases: leading whitespace is handled automatically by `>>`; if the stream contains no digit at all, the function returns after the stream reaches EOF; if the digit is the very first character, we put it back and return, leaving the stream positioned at that digit. Time complexity is O(k) where k is the number of characters read until the first digit (or EOF), and space complexity is O(1) aside from the single character buffer.
