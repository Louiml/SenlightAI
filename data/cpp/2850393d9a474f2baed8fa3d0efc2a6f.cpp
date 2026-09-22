/*
Write a C++ function named `sumHalfDigits` that takes a non-empty string `s` consisting only of decimal digit characters ('0'–'9') and returns the integer sum obtained by adding, for each character in the string, the value of the digit divided by 2 using integer division. For example, for the string `"1234"`, the calculation is (1/2) + (2/2) + (3/2) + (4/2) = 0 + 1 + 1 + 2 = 4. Your function should be `const`-correct (accept the input as `const std::string&`) and must not print anything; it must return the computed sum. Handle edge cases such as a string of all zeros (result 0) and single-character strings.
*/
#include <string>

// Return the sum of (digit / 2) for each digit character in the input string.
// Example: "1234" -> (1/2)+(2/2)+(3/2)+(4/2) = 0+1+1+2 = 4
int sumHalfDigits(const std::string& s) {
    int sum = 0;
    for (char ch : s) {
        int digit = ch - '0';  // Convert character to integer value
        sum += digit / 2;      // Integer division by 2
    }
    return sum;
}
#include <cassert>
#include <string>

// Assume sumHalfDigits is defined above (include its declaration here if needed)
int sumHalfDigits(const std::string& s);

int main() {
    assert(sumHalfDigits("0") == 0);
    assert(sumHalfDigits("5") == 2);
    assert(sumHalfDigits("9") == 4);
    assert(sumHalfDigits("00") == 0);
    assert(sumHalfDigits("11") == 0);      // (1/2)+(1/2)=0+0
    assert(sumHalfDigits("22") == 2);      // (2/2)+(2/2)=1+1
    assert(sumHalfDigits("1234") == 4);
    assert(sumHalfDigits("55555") == 10);  // 5*(2) = 10
    assert(sumHalfDigits("888888") == 24); // 6*(8/2)=6*4
    assert(sumHalfDigits("0123456789") == 20); // sum of floors: 0+0+1+1+2+2+3+3+4+4=20
    return 0;
}
// The solution iterates over each character of the input string. For each character, we convert it to its numeric value by subtracting the ASCII value of '0' (i.e., 48). Then we perform integer division by 2, which truncates toward zero (for non-negative digits, this gives the floor). We accumulate these values into an integer sum and return it. Since the input is guaranteed to contain only digits, no validation is needed, but the function should still work for any string of digits. Edge cases include: a single character (e.g., '5' → 2), all zeros (sum = 0), and long strings where the sum may grow; use `int` which is sufficient for typical constraints (string length up to a few thousand). Time complexity is O(n) where n is the length of the string, and space complexity is O(1) aside from the input reference.
