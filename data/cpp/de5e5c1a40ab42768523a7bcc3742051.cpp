// Write a standalone C++ function named `parseMonomial` that takes a string representation of a monomial and returns a `std::string` in normalized form. The input format is: an optional integer coefficient (which can be signed, e.g., `-3`, `+2`, or absent meaning coefficient 1), followed by a sequence of variable factors, each either a single lowercase letter `a`–`z` (implicit exponent 1) or a letter followed by `^` and a positive integer exponent (e.g., `x^4`). No multiplication signs or spaces appear. The function must combine like variables by summing exponents, simplify the coefficient, and output the monomial in canonical form: first output the coefficient if it is not 1 or -1 (output `-` for -1), then for each variable in ascending alphabetical order that appears (exponent > 0), output the letter and, if its total exponent is 1, just the letter; otherwise output `^` and the exponent. If the monomial simplifies to just a constant (no variables), output that integer. If the simplified coefficient is 0, output `"0"`. You may assume the input is non-empty and well-formed.

#include <cassert>
#include <string>

// Declaration of the function being tested.
std::string parseMonomial(const std::string& s);

int main() {
    assert(parseMonomial("x") == "x");
    assert(parseMonomial("-x") == "-x");
    assert(parseMonomial("+2x^2") == "2x^2");
    assert(parseMonomial("3a^2b") == "3a^2b");
    assert(parseMonomial("a^2a^3") == "a^5");
    assert(parseMonomial("0") == "0");
    assert(parseMonomial("-1") == "-1");
    assert(parseMonomial("5") == "5");
    assert(parseMonomial("xy") == "xy");
    assert(parseMonomial("-2x^3y^2") == "-2x^3y^2");
    return 0;
}

#include <string>
#include <vector>
#include <cctype>

// Parse a monomial string and return its normalized canonical form.
std::string parseMonomial(const std::string& s) {
    std::vector<int> exp(26, 0);
    int k = 0;
    size_t i = 0;

    // Parse optional signed coefficient.
    std::string coeffStr;
    while (i < s.size() && (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '-' || s[i] == '+')) {
        coeffStr += s[i];
        ++i;
    }

    if (coeffStr.size() == 0) {
        k = 1;
    } else if (coeffStr == "+") {
        k = 1;
    } else if (coeffStr == "-") {
        k = -1;
    } else {
        k = std::stoi(coeffStr);
    }

    // Parse variable factors.
    while (i < s.size()) {
        if (std::isalpha(static_cast<unsigned char>(s[i]))) {
            int idx = s[i] - 'a';
            if (i + 1 >= s.size() || std::isalpha(static_cast<unsigned char>(s[i + 1]))) {
                // Single letter with implicit exponent 1.
                ++exp[idx];
                ++i;
            } else {
                // Letter followed by '^' and digits.
                if (i + 1 < s.size() && s[i + 1] != '^') {
                    // Should not happen for valid input, but treat as exponent 1.
                    ++exp[idx];
                    ++i;
                } else {
                    i += 2; // skip letter and '^'
                    std::string numStr;
                    while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) {
                        numStr += s[i];
                        ++i;
                    }
                    int cnt = std::stoi(numStr);
                    exp[idx] += cnt;
                }
            }
        } else {
            // Skip unexpected character (shouldn't happen).
            ++i;
        }
    }

    // Build normalized output.
    if (k == 0) {
        return "0";
    }

    std::string result;
    if (k == -1) {
        result += '-';
    } else if (k != 1) {
        result += std::to_string(k);
    }

    for (int idx = 0; idx < 26; ++idx) {
        if (exp[idx] == 1) {
            result += static_cast<char>('a' + idx);
        } else if (exp[idx] > 1) {
            result += static_cast<char>('a' + idx);
            result += '^';
            result += std::to_string(exp[idx]);
        }
    }

    if (result.empty()) {
        result = "1"; // Constant with coefficient 1 but no variables.
    }

    return result;
}

// The core task is to parse a string into a coefficient and a vector of 26 integer exponents (indices 0–25 correspond to `a`–`z`). Start by scanning the beginning of the string: while the current character is a digit, `-`, or `+`, collect it into a coefficient substring. If that substring is empty or exactly `"+"` or `"-"` then the coefficient is ±1 respectively (empty means +1, `-` means -1, `+` also +1). Otherwise convert the substring to an integer with `std::stoi`. Then iterate through the rest of the string: if the next character is a letter (and either it is the last character or the character after it is also a letter, i.e., the letter appears alone), increment that variable's exponent by 1 and advance one character; otherwise the letter is followed by `^` and then digits representing a positive exponent; parse that number, add it to the variable's count, and advance past the digits. After parsing, build the output: first handle the zero case (coefficient == 0 or all exponents are zero? Actually if coefficient is 0, output "0"; also if all exponents are zero, it is a constant and we output coefficient as integer). Then output coefficient using rules: if k is -1, output `-`; else if k != 1, output the integer. Then for i from 0 to 25, if exponent > 0, if exponent == 1 append `'a'+i`; else append letter, `^`, and the exponent. Edge cases: input like `"x"`, `"-x"`, `"x^0"`? The spec says positive exponents only, so `x^0` won't appear, but note `x^1` is possible and should output as just `x` (exponent 1). Also duplicate variables like `"a^2a^3"` should combine to `a^5`. Time complexity is O(L) where L is string length, plus O(26) for output; space O(26) for storage and O(L) for output string.
