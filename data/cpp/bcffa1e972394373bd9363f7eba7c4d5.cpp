// Write a C++ function named `compareReversedNumbers` that takes two positive three-digit integers `a` and `b` (both guaranteed to be between 100 and 999 inclusive) and returns a string indicating their relationship after reversing the order of their digits. Specifically, for each input number, form a new number by reversing its digits (e.g., 123 → 321, 700 → 7 as 007 → 7). Then compare the two reversed numbers using standard integer ordering. The function must return exactly one of the following strings: `"a < b"`, `"b < a"`, or `"a = b"`, where `a` and `b` appear in the string as their original values, not the reversed ones. If the reversed numbers are equal (for example, 123 and 321, or 121 and 121), return the equality string with the original values in the order they were passed. The function should handle all three-digit numbers correctly, including those with zeros in the hundreds or tens places (e.g., 100 reverses to 1, 120 reverses to 21).
#include <cassert>
#include <string>

// Assume the solution function is defined above.
int main() {
    // Basic ordering after reversal
    assert(compareReversedNumbers(123, 321) == "321 < 123");
    assert(compareReversedNumbers(321, 123) == "321 < 123");
    assert(compareReversedNumbers(123, 456) == "123 < 456"); // 321 < 654

    // Equal reversed values
    assert(compareReversedNumbers(121, 121) == "121 = 121");
    assert(compareReversedNumbers(123, 321) == "321 < 123");
    assert(compareReversedNumbers(100, 1) == "1 < 100"); // Note: input must be three-digit, so 001 is invalid; test 100 vs 200
    assert(compareReversedNumbers(100, 200) == "100 < 200"); // rev 1 < 2
    assert(compareReversedNumbers(200, 100) == "100 < 200"); // rev 2 > 1 → "100 < 200"

    // Reverse creates equal values from different originals
    assert(compareReversedNumbers(123, 321) == "321 < 123");
    assert(compareReversedNumbers(101, 110) == "110 < 101"); // rev 101 vs 11 → 11 < 101

    // Identical numbers
    assert(compareReversedNumbers(999, 999) == "999 = 999");
    assert(compareReversedNumbers(100, 100) == "100 = 100");
}
#include <string>

// Compare two three-digit numbers after reversing their digits.
// Returns "a < b", "b < a", or "a = b" based on the reversed values.
std::string compareReversedNumbers(int a, int b) {
    // Reverse digits of a
    const int a_hundreds = a / 100;
    const int a_tens = (a / 10) % 10;
    const int a_units = a % 10;
    const int rev_a = a_units * 100 + a_tens * 10 + a_hundreds;

    // Reverse digits of b
    const int b_hundreds = b / 100;
    const int b_tens = (b / 10) % 10;
    const int b_units = b % 10;
    const int rev_b = b_units * 100 + b_tens * 10 + b_hundreds;

    // Compare reversed numbers, but output original values
    if (rev_a < rev_b) {
        return std::to_string(a) + " < " + std::to_string(b);
    } else if (rev_b < rev_a) {
        return std::to_string(b) + " < " + std::to_string(a);
    } else {
        return std::to_string(a) + " = " + std::to_string(b);
    }
}
// The main idea is to reverse the digits of each input number. Since the inputs are guaranteed to be three-digit integers (100–999), we can extract the hundreds, tens, and units digits using integer division and modulo operations: `h = a / 100`, `t = (a / 10) % 10`, `u = a % 10`. The reversed number is then `u * 100 + t * 10 + h`. Note that if the original number ends with zeros (like 100), the reversed number becomes a smaller integer (e.g., 1), which is correct because leading zeros in the reversed representation are ignored numerically. After computing both reversed values, compare them with standard `<` and `>`. If `rev_a < rev_b`, return `"a < b"` (using the original values); if `rev_b < rev_a`, return `"b < a"`; otherwise return `"a = b"`. Edge cases include numbers like 100 and 200 (reversed 1 vs 2), 101 and 110 (reversed 101 vs 11), and identical numbers. The algorithm runs in constant time \(O(1)\) because it only performs a fixed number of arithmetic operations, and uses \(O(1)\) auxiliary space beyond the returned string.
