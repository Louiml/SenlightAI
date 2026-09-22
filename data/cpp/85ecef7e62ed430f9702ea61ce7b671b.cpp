// Write a C++ function `gcdOfStrings(const std::string& str1, const std::string& str2)` that takes two non-empty strings and returns the largest string `x` such that `x` can be concatenated with itself one or more times to form both `str1` and `str2`. If no such common divisor string exists, return an empty string. You must consider that a string `t` "divides" a string `s` if and only if `s` is exactly `t` repeated one or more times (e.g., `"ABC"` divides `"ABCABC"` but not `"ABCAB"`). The function should work for arbitrary ASCII strings, including those with repeated patterns, and must handle cases where one string is longer than the other or where lengths are not multiples of each other. You may use standard library facilities but must not use any global variables or helper functions that take inputs from the user.
The solution approach is to find the greatest common divisor (GCD) of two strings by iterating over possible divisor lengths from the length of the shorter string down to 1, and checking divisibility. First, identify the shorter and longer strings. For each candidate length `i` (from shortest length down to 1), check that both string lengths are divisible by `i` (i.e., `len1 % i == 0` and `len2 % i == 0`). If not, skip. If yes, then we need to verify that the first `i` characters of the longer string (or any of them) form a pattern that repeats exactly to cover both strings. A clean way is to take the prefix of the longer string of length `i` as a candidate divisor `candidate` (e.g., `longest.substr(0, i)`), then check whether both strings can be formed by repeating `candidate` exactly (i.e., `str1` equals `candidate` repeated `str1.length()/i` times and similarly for `str2`). This can be done by iterating through the string and comparing each character to `candidate[j % i]`. If the check passes, return `candidate` because we are iterating from largest to smallest, so the first match is the largest valid divisor. If no match is found, return an empty string. Edge cases include: one string being empty (though the problem says non-empty, but handle gracefully), lengths not sharing any common divisor, and identical strings (the whole string is the answer). Time complexity is O(n^2) in the worst case due to checking each candidate length and scanning strings; space complexity is O(n) for the candidate string or O(1) if we avoid constructing it (but using substring is simpler and acceptable). An improved approach uses the Euclidean algorithm by comparing concatenations: if `str1 + str2 != str2 + str1`, no common divisor exists; otherwise, the answer length is the GCD of the two lengths, and the answer is the prefix of either string of that length. That is O(n) time and O(1) extra space (ignoring the concatenated string). I will present the simpler divisor-iteration method for clarity, but the test will verify correctness.
#include <string>
#include <algorithm>

// Returns the largest string x such that x divides both str1 and str2.
// If no such string exists, returns an empty string.
std::string gcdOfStrings(const std::string& str1, const std::string& str2) {
    // Identify shorter and longer strings
    const std::string& shorter = (str1.size() < str2.size()) ? str1 : str2;
    const std::string& longer  = (str1.size() < str2.size()) ? str2 : str1;

    // Iterate from the length of the shorter string down to 1
    for (std::size_t len = shorter.size(); len > 0; --len) {
        // Both lengths must be divisible by len
        if (longer.size() % len != 0 || shorter.size() % len != 0) {
            continue;
        }

        // Candidate divisor is the prefix of the longer string of length len
        std::string candidate = longer.substr(0, len);
        bool valid = true;

        // Check that longer is exactly candidate repeated
        for (std::size_t i = 0; i < longer.size(); ++i) {
            if (longer[i] != candidate[i % len]) {
                valid = false;
                break;
            }
        }

        // Check that shorter is exactly candidate repeated
        if (valid) {
            for (std::size_t i = 0; i < shorter.size(); ++i) {
                if (shorter[i] != candidate[i % len]) {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            return candidate; // largest valid divisor found
        }
    }

    return ""; // no common divisor
}
#include <cassert>
#include <string>
#include <iostream>

// The solution function is declared above; include its definition here for the test.
std::string gcdOfStrings(const std::string& str1, const std::string& str2) {
    const std::string& shorter = (str1.size() < str2.size()) ? str1 : str2;
    const std::string& longer  = (str1.size() < str2.size()) ? str2 : str1;

    for (std::size_t len = shorter.size(); len > 0; --len) {
        if (longer.size() % len != 0 || shorter.size() % len != 0) {
            continue;
        }
        std::string candidate = longer.substr(0, len);
        bool valid = true;
        for (std::size_t i = 0; i < longer.size(); ++i) {
            if (longer[i] != candidate[i % len]) {
                valid = false;
                break;
            }
        }
        if (valid) {
            for (std::size_t i = 0; i < shorter.size(); ++i) {
                if (shorter[i] != candidate[i % len]) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            return candidate;
        }
    }
    return "";
}

int main() {
    // Example from the original problem
    assert(gcdOfStrings("ABCABC", "ABC") == "ABC");
    assert(gcdOfStrings("ABABAB", "ABAB") == "AB");
    assert(gcdOfStrings("LEET", "CODE") == "");

    // Additional edge cases
    assert(gcdOfStrings("AAAA", "AA") == "AA");
    assert(gcdOfStrings("ABABABAB", "ABAB") == "ABAB");
    assert(gcdOfStrings("A", "A") == "A");
    assert(gcdOfStrings("ABC", "ABCABCABC") == "ABC");
    assert(gcdOfStrings("XXYXXY", "XXY") == "XXY");
    assert(gcdOfStrings("a", "b") == "");
    assert(gcdOfStrings("ABCDEF", "ABC") == "");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
