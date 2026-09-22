/*
Write a C++ function that takes a string representing a sequence of single-digit numbers (0-9) separated by '+' characters, extracts all digits in the order they appear, sorts only the extracted digits in ascending order (using any stable or unstable sort), and returns a new string where the sorted digits are re-inserted back into the original positions of the '+' separators, i.e., produce a string with the same length and same '+' positions, but with the digit characters sorted in ascending order. The input string will always contain at least one digit, and the digits are separated by exactly one '+'. The function must handle empty strings gracefully (return an empty string). It must not modify the input; it must return a new string.
*/
#include <string>
#include <vector>
#include <algorithm>

// Sorts the digits in a string of the form "d+d+...+d" (single digits separated by '+'),
// preserving the '+' positions and returning a new string with the digits sorted ascending.
std::string sortDigitsPreservingPlus(const std::string& input) {
    if (input.empty()) {
        return input;
    }

    // Extract all digit characters into a vector.
    std::vector<char> digits;
    for (char ch : input) {
        if (ch >= '0' && ch <= '9') {
            digits.push_back(ch);
        }
    }

    // Sort the digits in ascending order.
    std::sort(digits.begin(), digits.end());

    // Rebuild the output string, placing sorted digits back into their original positions.
    std::string result;
    result.reserve(input.size());
    size_t digitIndex = 0;
    for (char ch : input) {
        if (ch >= '0' && ch <= '9') {
            result.push_back(digits[digitIndex]);
            ++digitIndex;
        } else {
            // This will be the '+' character, but we keep it as-is.
            result.push_back(ch);
        }
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is already declared above; include it here if needed.
// Assume the function is defined before this test file.

int main() {
    // Basic case with multiple digits and pluses.
    assert(sortDigitsPreservingPlus("3+1+2") == "1+2+3");
    
    // Already sorted.
    assert(sortDigitsPreservingPlus("0+1+9") == "0+1+9");
    
    // Reverse order.
    assert(sortDigitsPreservingPlus("9+8+7+0") == "0+7+8+9");
    
    // Single digit no plus.
    assert(sortDigitsPreservingPlus("5") == "5");
    
    // Empty input.
    assert(sortDigitsPreservingPlus("") == "");
    
    // Multiple pluses with single digit in between.
    assert(sortDigitsPreservingPlus("+3+1+2+") == "+1+2+3+");
    
    // Duplicate digits.
    assert(sortDigitsPreservingPlus("2+2+1+2") == "1+2+2+2");
    
    // All same digits.
    assert(sortDigitsPreservingPlus("7+7+7") == "7+7+7");
    
    // Larger mixed order.
    assert(sortDigitsPreservingPlus("9+5+0+3+8") == "0+3+5+8+9");
    
    // String starting with digit and ending with plus.
    assert(sortDigitsPreservingPlus("1+9+") == "1+9+");
    
    return 0;
}
// The problem requires extracting all digit characters from the input string into a container (e.g., vector<int> or string), sorting them in ascending order, and then rebuilding the output string by iterating through the original string: whenever we encounter a digit, we take the next smallest digit from the sorted collection; whenever we encounter a '+', we append a '+' to the result. This preserves the positions of the plus signs while replacing the digits with sorted ones. Important edge cases: the input may be an empty string (should return an empty string); the input may contain only one digit with no '+' (in that case the output is the same as input); the input may contain multiple '+' signs, and the sorting must not affect the positions of the '+'. The algorithm involves: (1) one pass to collect digits, (2) sort them (using std::sort, which is O(n log n) where n is the number of digits), (3) one pass to build the result. Time complexity: O(n log n) due to sorting, where n is the number of digits (not the full string length if there are pluses). Space complexity: O(n) for storing the extracted digits and the result string. The solution must be const-correct, i.e., the input string is taken by const reference, and the function returns by value.
