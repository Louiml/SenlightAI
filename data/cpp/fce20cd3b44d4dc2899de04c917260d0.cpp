Write a C++ function `sortPlusExpression(const std::string& input)` that takes a string containing single-digit numbers (0-9) separated by plus signs (e.g., `"3+1+2"`) and returns a new string with the digits sorted in non-decreasing order while preserving the plus separators, so the result is a canonical form like `"1+2+3"`. The input is guaranteed to be non-empty, well-formed (no leading/trailing or consecutive plus signs), and contain only digits and plus signs. The function must not modify the input string and must handle the case where there is only one digit (returning that digit with no plus). For full credit, avoid using string streams or regex; instead, use a simple character collection and sorting approach.
The core idea is to extract all digit characters from the input string into a separate container (e.g., a `std::string` or `std::vector<char>`), ignoring the plus signs. Then sort that container in ascending order using `std::sort`. Finally, reconstruct the output by iterating over the sorted digits and inserting a `'+'` before every digit except the first one. Edge cases: (1) input with a single digit must not have any plus; (2) duplicate digits are preserved; (3) since we only collect digits, the position of plus signs is irrelevant after sorting. Time complexity is O(n log n) for sorting, where n is the number of digits, and the string traversal is O(m) where m is the total input length, so overall O(m + n log n). Auxiliary space is O(n) for the container of digits.
#include <string>
#include <algorithm>

// Given a string like "3+1+2", return "1+2+3".
// Input contains only digits and plus signs, non-empty.
std::string sortPlusExpression(const std::string& input) {
    std::string digits;
    digits.reserve((input.size() + 1) / 2); // at most half are digits

    // Extract all digit characters
    for (char ch : input) {
        if (ch != '+') {
            digits.push_back(ch);
        }
    }

    // Sort the digits in non-decreasing order
    std::sort(digits.begin(), digits.end());

    // Build the result with plus separators
    std::string result;
    result.reserve(digits.size() * 2 - 1); // digits + (digits-1) pluses
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i > 0) {
            result.push_back('+');
        }
        result.push_back(digits[i]);
    }

    return result;
}
#include <cassert>
#include <string>

int main() {
    assert(sortPlusExpression("3+1+2") == "1+2+3");
    assert(sortPlusExpression("5") == "5");
    assert(sortPlusExpression("9+0+9+0") == "0+0+9+9");
    assert(sortPlusExpression("1+1+1") == "1+1+1");
    assert(sortPlusExpression("2+3+1+4+5") == "1+2+3+4+5");
    assert(sortPlusExpression("7+7") == "7+7");
    assert(sortPlusExpression("0+8+2") == "0+2+8");
    assert(sortPlusExpression("4+2") == "2+4");
    assert(sortPlusExpression("6+5+4") == "4+5+6");
    assert(sortPlusExpression("3") == "3");
}
