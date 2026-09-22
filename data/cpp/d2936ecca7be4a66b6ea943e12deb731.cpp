Write a C++ function that converts a positive integer (1 ≤ num ≤ 3999) into its Roman numeral representation. The function must be named `intToRoman`, take an integer parameter, and return a `std::string`. Use the standard subtractive notation rules for Roman numerals: I=1, V=5, X=10, L=50, C=100, D=500, M=1000, IV=4, IX=9, XL=40, XC=90, CD=400, CM=900. The input will always be a valid positive integer within the specified range, so no error handling for out-of-range or negative values is required. The function must be `const`-correct, meaning any local variables that are not modified should be declared `const`.
#include <cassert>
#include <string>

// Forward declaration of the tested function.
std::string intToRoman(int num);

int main() {
    assert(intToRoman(1) == "I");
    assert(intToRoman(4) == "IV");
    assert(intToRoman(9) == "IX");
    assert(intToRoman(58) == "LVIII");
    assert(intToRoman(1994) == "MCMXCIV");
    assert(intToRoman(2024) == "MMXXIV");
    assert(intToRoman(3999) == "MMMCMXCIX");
    assert(intToRoman(10) == "X");
    assert(intToRoman(40) == "XL");
    assert(intToRoman(90) == "XC");
    return 0;
}
#include <string>
#include <vector>

// Convert a positive integer (1..3999) to Roman numeral representation.
std::string intToRoman(int num) {
    const std::vector<int> values = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const std::vector<std::string> symbols = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    
    std::string result;
    const int numValues = static_cast<int>(values.size());
    
    for (int i = 0; i < numValues; ++i) {
        const int count = num / values[i];
        num %= values[i];
        for (int j = 0; j < count; ++j) {
            result += symbols[i];
        }
    }
    
    return result;
}
// The solution uses a greedy algorithm that processes the integer from largest to smallest Roman numeral value. Maintain two parallel arrays: one for integer values in descending order `{1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1}` and one for their corresponding Roman symbols `{"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}`. For each value, count how many times it divides into the remaining `num`, append that many copies of the symbol to the result string, and reduce `num` by taking the modulo. Continue until `num` becomes zero. Edge cases include numbers like 4 (IV), 9 (IX), 40 (XL), 90 (XC), 400 (CD), and 900 (CM) which are handled by including these subtrahend pairs in the arrays. The time complexity is O(1) because the number of distinct Roman symbols is fixed (13), and at most a constant number of iterations occurs for numbers ≤ 3999. The space complexity is O(1) excluding the output string, which is at most 15 characters (e.g., 3888 = MMMDCCCLXXXVIII).
