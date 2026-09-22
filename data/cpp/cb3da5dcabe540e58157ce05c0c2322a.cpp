/*
Write a C++ function that takes an integer `n` (which may be positive, negative, or zero) and returns a string where every digit `1` in the decimal representation of `n` is replaced with `9`, and every digit `9` is replaced with `1`. All other digits (including `0`, `2` through `8`, and the minus sign for negative numbers) remain unchanged. The function must preserve the sign and leading zeros are not present in the input. For example, given `n = 191`, the output should be `"919"`; given `n = -19`, the output should be `"-91"`. If the input is `0`, the output should be `"0"`. The function should operate on the integer's decimal string representation, not by arithmetic transformation.
*/

#include <string>

// Replace every digit '1' with '9' and every digit '9' with '1' in the decimal representation of n.
std::string swapOneAndNine(int n) {
    std::string result = std::to_string(n);
    for (char& c : result) {
        if (c == '1') {
            c = '9';
        } else if (c == '9') {
            c = '1';
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The function to test is declared here (or include the header).
std::string swapOneAndNine(int n);

int main() {
    // Basic cases
    assert(swapOneAndNine(1) == "9");
    assert(swapOneAndNine(9) == "1");
    assert(swapOneAndNine(191) == "919");
    assert(swapOneAndNine(919) == "191");

    // Digits unchanged
    assert(swapOneAndNine(1234567890) == "9234567810");

    // Zero
    assert(swapOneAndNine(0) == "0");

    // Negative numbers: sign preserved, digits swapped
    assert(swapOneAndNine(-19) == "-91");
    assert(swapOneAndNine(-91) == "-19");

    // Large number
    assert(swapOneAndNine(111999111) == "999111999");

    // No 1 or 9
    assert(swapOneAndNine(235) == "235");

    return 0;
}

// The solution is straightforward: convert the integer to its decimal string representation using `std::to_string`. Then iterate over each character in the string. For each character, if it is `'1'`, replace it with `'9'`; if it is `'9'`, replace it with `'1'`. Digits `'0'` and `'2'`–`'8'` remain unchanged, and the minus sign (if any) at the beginning is also left unchanged because it is not `'1'` or `'9'`. No special handling is required for zero, as `"0"` contains no `1` or `9`. The algorithm runs in O(d) time, where d is the number of digits in the decimal representation (including the sign), and uses O(1) extra space besides the returned string. Edge cases include negative numbers (sign is preserved), numbers with no 1 or 9 digits (output equals input string), and zero.
