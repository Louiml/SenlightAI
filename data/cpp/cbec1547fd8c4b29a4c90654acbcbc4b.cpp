// Write a C++ function named `repeatEachCharacter` that takes a non-negative integer `r` and a string `s` (which may be empty or contain spaces) and returns a new string where each character of `s` is repeated `r` times consecutively, preserving the original order of characters. The function must not print anything; it must return the result as a `std::string`. Handle edge cases such as `r == 0` (return an empty string), an empty input string (return an empty string), and strings with spaces or other non-alphanumeric characters (treat them like any other character).

The solution iterates over each character in the input string `s`. For each character, it appends that character `r` times to a result string. A simple loop from 0 to `r-1` for each character accomplishes this. The total number of appended characters is `r * s.length()`. If `r` is 0 or `s` is empty, the result is an empty string. The algorithm runs in O(r * n) time, where `n` is the length of the string, because we perform exactly `r` appends per character. Space complexity is O(r * n) for the returned string, since that is the size of the output. No special handling is needed for spaces or punctuation—they are just characters. Edge cases: (1) `r = 0` results in empty output regardless of `s`; (2) empty `s` results in empty output regardless of `r`; (3) large `r` and `n` could cause memory usage proportional to the product, so the function should ideally reserve capacity to avoid reallocations.

#include <string>

// Repeat each character in s exactly r times, preserving order.
// If r is 0 or s is empty, returns an empty string.
std::string repeatEachCharacter(int r, const std::string& s) {
    if (r <= 0 || s.empty()) {
        return "";
    }
    
    std::string result;
    result.reserve(s.size() * r);  // Pre-allocate for efficiency
    
    for (char c : s) {
        result.append(r, c);  // Append c r times
    }
    
    return result;
}

#include <cassert>
#include <string>
#include "repeat_hpp.h"  // Assume the function is declared in this header

int main() {
    // Basic cases
    assert(repeatEachCharacter(3, "ab") == "aaabbb");
    assert(repeatEachCharacter(1, "hello") == "hello");
    assert(repeatEachCharacter(2, "x") == "xx");
    
    // Edge cases
    assert(repeatEachCharacter(0, "anything") == "");
    assert(repeatEachCharacter(5, "") == "");
    assert(repeatEachCharacter(0, "") == "");
    
    // Spaces and punctuation
    assert(repeatEachCharacter(2, "a b") == "aa  bb");
    assert(repeatEachCharacter(3, "!?") == "!!!???");
    
    // Larger repetition counts
    assert(repeatEachCharacter(4, "12") == "11112222");
    
    // Verify with known output from original problem style
    assert(repeatEachCharacter(2, "abc") == "aabbcc");
    
    return 0;
}
