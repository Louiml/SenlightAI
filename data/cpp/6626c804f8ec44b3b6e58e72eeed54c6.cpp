Write a C++ function named `printOddNumbersUpTo` that takes a single integer parameter `limit` (assumed to be non-negative) and returns a `std::string` containing all odd numbers from 1 up to and including `limit`, separated by a single space, with no trailing space. If there are no odd numbers in the range (i.e., `limit` is 0), the function must return an empty string. The function must not produce any output to the console; it must only build and return the formatted string. For example, `printOddNumbersUpTo(10)` should return `"1 3 5 7 9"`, and `printOddNumbersUpTo(1)` should return `"1"`.
// The solution iterates from 1 to `limit` inclusive, checking each integer for oddness using the modulo operator (`i % 2 != 0`). For each odd number, we append it to a `std::string` using `std::to_string`, adding a leading space before each number except the first one to avoid trailing or leading extra spaces. Edge cases: `limit = 0` should yield an empty string (the loop doesn't execute, and we return it); `limit = 1` yields just `"1"`; large values are handled by `std::string`'s dynamic growth. Time complexity is \(O(n)\) where \(n = \text{limit}\), and space complexity is \(O(n)\) for the result string (since it holds roughly \(n/2\) numbers and separators). The function is `const`-correct and uses no global state.
#include <string>

// Return a string of all odd numbers from 1 to limit (inclusive), separated by spaces.
// If limit < 1, returns an empty string. No trailing space is added.
std::string printOddNumbersUpTo(int limit) {
    std::string result;
    bool first = true;  // To avoid leading space before the first number.

    for (int i = 1; i <= limit; ++i) {
        if (i % 2 != 0) {
            if (!first) {
                result += ' ';
            }
            result += std::to_string(i);
            first = false;
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the solution function (defined elsewhere in the full program).
std::string printOddNumbersUpTo(int limit);

int main() {
    // Basic cases
    assert(printOddNumbersUpTo(10) == "1 3 5 7 9");
    assert(printOddNumbersUpTo(1) == "1");
    assert(printOddNumbersUpTo(0) == "");
    
    // Larger limit
    assert(printOddNumbersUpTo(5) == "1 3 5");
    assert(printOddNumbersUpTo(20) == "1 3 5 7 9 11 13 15 17 19");
    
    // Odd limit includes itself
    assert(printOddNumbersUpTo(7) == "1 3 5 7");
    
    // Even limit excludes itself
    assert(printOddNumbersUpTo(8) == "1 3 5 7");
    
    // Limit = 2 (only 1 is odd)
    assert(printOddNumbersUpTo(2) == "1");
    
    // Limit = 3 (1 and 3)
    assert(printOddNumbersUpTo(3) == "1 3");
    
    // No trailing space check (length comparison)
    assert(printOddNumbersUpTo(4).back() == '3');
    return 0;
}
