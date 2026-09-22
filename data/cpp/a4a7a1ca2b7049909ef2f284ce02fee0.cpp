/*
Write a C++ function named `intToRoman` that converts a given integer in the range [1, 3999] into its Roman numeral representation. The function must take an integer as input and return a `std::string` containing the Roman numeral. Roman numerals use subtractive notation (e.g., 4 as IV, 9 as IX, 40 as XL, 90 as XC, 400 as CD, 900 as CM) and combine symbols from largest to smallest value. Handle all valid inputs in the inclusive range from 1 to 3999; for any input outside this range, the function may return an empty string or behave in a defined manner (e.g., treat as 0). Ensure the function is robust and uses appropriate data structures to avoid hardcoded conditional chains.
*/
#include <string>
#include <vector>
#include <utility>

// Convert an integer (1..3999) to its Roman numeral representation.
// Returns an empty string for inputs outside [1, 3999].
std::string intToRoman(int num) {
    if (num < 1 || num > 3999) {
        return "";
    }
    
    const std::vector<std::pair<int, std::string>> valueSymbols = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
        {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"},
        {1, "I"}
    };
    
    std::string result;
    
    for (const auto& [value, symbol] : valueSymbols) {
        while (num >= value) {
            num -= value;
            result += symbol;
        }
        if (num == 0) {
            break;
        }
    }
    
    return result;
}
#include <cassert>
#include <string>

// Forward declaration or include the solution function
std::string intToRoman(int num);

int main() {
    // Basic single symbols
    assert(intToRoman(1) == "I");
    assert(intToRoman(5) == "V");
    assert(intToRoman(10) == "X");
    assert(intToRoman(50) == "L");
    assert(intToRoman(100) == "C");
    assert(intToRoman(500) == "D");
    assert(intToRoman(1000) == "M");
    
    // Subtractive cases
    assert(intToRoman(4) == "IV");
    assert(intToRoman(9) == "IX");
    assert(intToRoman(40) == "XL");
    assert(intToRoman(90) == "XC");
    assert(intToRoman(400) == "CD");
    assert(intToRoman(900) == "CM");
    
    // Composite numbers
    assert(intToRoman(58) == "LVIII");
    assert(intToRoman(1994) == "MCMXCIV");
    assert(intToRoman(2024) == "MMXXIV");
    assert(intToRoman(3999) == "MMMCMXCIX");
    
    // Edge cases and invalid input
    assert(intToRoman(0) == "");
    assert(intToRoman(4000) == "");
    
    return 0;
}
// The solution leverages a greedy algorithm. We predefine a vector of pairs mapping each Roman numeral symbol (including subtractive combinations) to its integer value, sorted in descending order of value. Starting from the largest value, we repeatedly subtract the current value from the input number and append its corresponding symbol to the result string, continuing until the number becomes zero. This approach guarantees the shortest possible Roman numeral representation because it always uses the largest possible symbol first. The subtractive cases (like CM for 900) are explicitly included in the mapping, so the algorithm automatically handles them without special logic. Edge cases include the number 4 (should produce "IV"), 9 ("IX"), 40 ("XL"), 90 ("XC"), 400 ("CD"), and 900 ("CM"). Also, numbers like 3999 ("MMMCMXCIX") test the upper boundary. Time complexity is O(1) because the loop runs at most a constant number of times (13 symbols, each subtracted a bounded number of times, at most 4 per symbol for 1s, 10s, 100s, 1000s). Space complexity is O(1) as the mapping is fixed size and the output string length is bounded (maximum length 15 for 3888).
