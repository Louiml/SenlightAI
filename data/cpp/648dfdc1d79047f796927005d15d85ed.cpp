// Write a C++ function that converts a valid Roman numeral string (containing only the characters I, V, X, L, C, D, M) into its integer equivalent. The function must handle all standard Roman numeral rules, including subtractive notation (e.g., IV = 4, IX = 9, XL = 40, XC = 90, CD = 400, CM = 900). The input is guaranteed to be a valid Roman numeral between 1 and 3999 inclusive; you do not need to validate input. The function should accept the string by const reference and return an int.
The algorithm works by scanning the Roman numeral from right to left. Initialize the result with the integer value of the last character. For each preceding character i (from second-to-last down to the first), compare its value with the value of the character immediately to its right. If the current character's value is less than the right neighbor's value, subtract it from the result (this handles subtractive cases like IV, where I=1 is less than V=5, so we subtract 1). Otherwise, add its value to the result. This works because in valid Roman numerals, a smaller value symbol appearing before a larger one always indicates subtraction, and otherwise values are added. Edge cases include single-character strings (e.g., "V" → 5) where the loop simply doesn't execute, and cases like "MCMXCIV" (1994) where multiple subtractive patterns occur; the right-to-left traversal handles all of them naturally. The time complexity is O(n) where n is the length of the string (one pass over all characters), and the space complexity is O(1) because we only store a constant-size mapping table and a few integers.
#include <string>
#include <unordered_map>

// Convert a valid Roman numeral string to its integer value.
// Input is guaranteed to be a valid Roman numeral between 1 and 3999.
int romanToInt(const std::string& s) {
    static const std::unordered_map<char, int> romanValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };

    int result = romanValues.at(s.back());
    for (int i = static_cast<int>(s.size()) - 2; i >= 0; --i) {
        int current = romanValues.at(s[i]);
        int next = romanValues.at(s[i + 1]);
        if (current < next) {
            result -= current;
        } else {
            result += current;
        }
    }
    return result;
}
#include <cassert>

int main() {
    assert(romanToInt("I") == 1);
    assert(romanToInt("IV") == 4);
    assert(romanToInt("IX") == 9);
    assert(romanToInt("LVIII") == 58);
    assert(romanToInt("MCMXCIV") == 1994);
    assert(romanToInt("MMMCMXCIX") == 3999);
    assert(romanToInt("CDXLIV") == 444);
    assert(romanToInt("MCMLXXXIV") == 1984);
    assert(romanToInt("XV") == 15);
    assert(romanToInt("DCCCXC") == 890);
}
