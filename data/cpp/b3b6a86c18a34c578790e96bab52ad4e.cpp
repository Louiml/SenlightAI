// Write a standalone C++ function named `romanToInteger` that takes a `const std::string&` representing a valid Roman numeral (using the standard symbols I, V, X, L, C, D, M) and returns its integer value as an `int`. The function must correctly handle subtractive notation (e.g., IV = 4, IX = 9, XL = 40, XC = 90, CD = 400, CM = 900) by subtracting a smaller symbol when it appears immediately before a larger one, and adding otherwise. The input string will be non-empty and contain only valid Roman numeral characters in uppercase; you do not need to validate the input for correctness (e.g., "IIII" is considered valid for this exercise, but logically the algorithm should still produce the correct sum). For example, `romanToInteger("MCMXCIV")` must return 1994, `romanToInteger("LVIII")` must return 58, and `romanToInteger("IX")` must return 9. The function must be efficient for arbitrarily long valid inputs (e.g., up to several thousand characters) and must not modify the input string.

// The core algorithm iterates through each character of the Roman numeral string from left to right. For each character at index `i`, compare its integer value (looked up from a map or switch) with the value of the next character at index `i+1` (if it exists). If the current value is less than the next, subtract the current value from the result (because the next larger symbol indicates subtractive notation); otherwise, add the current value. After processing the last character, there is no next character, so the condition naturally falls to the "else" branch and adds its value. Edge cases include the very last character (index `s.length() - 1`) where accessing `mp[s[i+1]]` would be undefined behavior; therefore, we must guard the access by checking `i + 1 < s.length()` before comparing. Alternatively, we can append a sentinel value (like 0) to avoid the bound check. The time complexity is O(n) where n is the length of the string, since we perform a single pass. The space complexity is O(1) for the map (fixed 7 entries) and O(1) auxiliary for the result variable, ignoring the input string storage. The map lookup is O(1) on average with `std::map` (O(log 7) effectively constant) but we can use an array or switch for fully constant time.

#include <string>
#include <unordered_map>

// Convert a valid Roman numeral string to its integer value.
// Handles subtractive notation (e.g., IV=4, XC=90).
int romanToInteger(const std::string& s) {
    // Lookup table for Roman symbol values.
    const std::unordered_map<char, int> values = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };

    int result = 0;
    const int n = static_cast<int>(s.size());

    for (int i = 0; i < n; ++i) {
        int current = values.at(s[i]);
        // If there is a next symbol and its value is larger, subtract.
        if (i + 1 < n && current < values.at(s[i + 1])) {
            result -= current;
        } else {
            result += current;
        }
    }

    return result;
}

#include <cassert>
#include <string>

// Declare the function from the solution (for test linking)
int romanToInteger(const std::string& s);

int main() {
    // Basic single symbols
    assert(romanToInteger("I") == 1);
    assert(romanToInteger("V") == 5);
    assert(romanToInteger("X") == 10);
    assert(romanToInteger("L") == 50);
    assert(romanToInteger("C") == 100);
    assert(romanToInteger("D") == 500);
    assert(romanToInteger("M") == 1000);

    // Subtractive cases
    assert(romanToInteger("IV") == 4);
    assert(romanToInteger("IX") == 9);
    assert(romanToInteger("XL") == 40);
    assert(romanToInteger("XC") == 90);
    assert(romanToInteger("CD") == 400);
    assert(romanToInteger("CM") == 900);

    // Additive cases
    assert(romanToInteger("VI") == 6);
    assert(romanToInteger("XV") == 15);
    assert(romanToInteger("CXX") == 120);
    assert(romanToInteger("MDCLXVI") == 1666);

    // Mixed and complex numerals
    assert(romanToInteger("LVIII") == 58);
    assert(romanToInteger("MCMXCIV") == 1994);
    assert(romanToInteger("MMXXIII") == 2023);
    assert(romanToInteger("MMMCMXCIX") == 3999);

    return 0;
}
