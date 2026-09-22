Write a C++ function that converts a non-negative integer in the range 1 to 3999 into its Roman numeral representation. The function should take an `int` parameter and return a `std::string`. Roman numerals use the standard subtractive notation (e.g., IV for 4, IX for 9, XL for 40, XC for 90, CD for 400, CM for 900), and the output must have no extra spaces or characters. You may assume the input is always valid (i.e., between 1 and 3999, inclusive). The function should be named `intToRoman` and be placed in a header-compatible standalone form. Ensure the implementation is efficient, avoids dynamic allocation beyond the final string, and is self-contained with necessary includes.
The solution uses a greedy digit‑by‑digit approach with lookup tables. First, we pre‑define arrays for thousands, hundreds, tens, and ones, where each index corresponds to the digit value (0–3 for thousands, 0–9 for others). Then we decompose the input number by integer division and modulo: `thousands = num / 1000`, then `num %= 1000`; `hundreds = num / 100`, then `num %= 100`; `tens = num / 10`, then `ones = num % 10`. Finally, we concatenate the four corresponding strings from the arrays. This avoids loops and conditionals, making it O(1) time and O(1) auxiliary space (ignoring the output string). Edge cases include the digit 0, which maps to an empty string in each table, and numbers like 1994 that produce "MCMXCIV" correctly due to the lookup tables containing subtractive forms (e.g., "CM", "XC", "IV"). The approach is robust and simple, with no risk of overflow since the input is bounded.
#include <string>

// Convert a non-negative integer (1..3999) to its Roman numeral representation.
std::string intToRoman(int num) {
    const std::string thousands[] = {"", "M", "MM", "MMM"};
    const std::string hundreds[]  = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    const std::string tens[]      = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    const std::string ones[]      = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

    const int thousands_digit = num / 1000;
    num %= 1000;
    const int hundreds_digit  = num / 100;
    num %= 100;
    const int tens_digit      = num / 10;
    const int ones_digit      = num % 10;

    return thousands[thousands_digit] + hundreds[hundreds_digit] +
           tens[tens_digit] + ones[ones_digit];
}
#include <cassert>
#include <string>

// Declaration of the tested function (normally in a header).
std::string intToRoman(int num);

int main() {
    assert(intToRoman(1) == "I");
    assert(intToRoman(4) == "IV");
    assert(intToRoman(9) == "IX");
    assert(intToRoman(19) == "XIX");
    assert(intToRoman(40) == "XL");
    assert(intToRoman(90) == "XC");
    assert(intToRoman(400) == "CD");
    assert(intToRoman(900) == "CM");
    assert(intToRoman(1994) == "MCMXCIV");
    assert(intToRoman(3999) == "MMMCMXCIX");
    return 0;
}
