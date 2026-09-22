Write a C++ function named `isPalindromeNumber` that takes an integer `n` and returns a `bool` indicating whether `n` is a numeric palindrome (i.e., reads the same forward and backward). The function must handle negative numbers correctly: for negative inputs, consider the absolute value (ignoring the minus sign) when determining palindromicity, and return `true` if the absolute value is a palindrome. For example, `-121` should return `true` (since `121` is a palindrome), and `-123` should return `false`. The function should not use any standard library string conversion or reverse functions; implement the reversal using arithmetic operations on the integer itself.
#include <cassert>

int main() {
    // Basic positive palindromes.
    assert(isPalindromeNumber(0) == true);
    assert(isPalindromeNumber(5) == true);
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(12321) == true);

    // Positive non-palindromes.
    assert(isPalindromeNumber(123) == false);
    assert(isPalindromeNumber(100) == false);
    assert(isPalindromeNumber(12345) == false);

    // Negative inputs (ignoring sign).
    assert(isPalindromeNumber(-121) == true);
    assert(isPalindromeNumber(-5) == true);
    assert(isPalindromeNumber(-123) == false);
    assert(isPalindromeNumber(-1001) == true);

    // Edge case: largest int values (should not overflow due to long long).
    assert(isPalindromeNumber(2147483647) == false);
    assert(isPalindromeNumber(-2147483648) == false); // absolute value 2147483648 fits in long long

    return 0;
}
// Returns true if the integer n is a numeric palindrome, ignoring any negative sign.
bool isPalindromeNumber(int n) {
    // Work with the absolute value to handle negative inputs uniformly.
    long long original = (n < 0) ? -static_cast<long long>(n) : static_cast<long long>(n);
    long long temp = original;
    long long reversed = 0;

    // Reverse the digits using arithmetic operations.
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }

    // A zero is a palindrome; also compare reversed with initial absolute value.
    return original == reversed;
}
// The main algorithm uses an arithmetic reversal technique: repeatedly extract the last digit of the number using the modulo operator (`% 10`), build the reversed number by multiplying a running `reversed` variable by 10 and adding the extracted digit, then remove the last digit by integer division (`/ 10`). This continues until the original number becomes zero. To correctly handle negative numbers, take the absolute value at the start and use that in the reversal loop. After computing the reversed value, compare it with the absolute value of the original input. Edge cases include `0` (which is a palindrome), single-digit numbers (all palindromes), numbers ending with zeros (e.g., `100` is not a palindrome because its reversal is `1`), and large values that might overflow an `int`; to avoid overflow, we can use a `long long` for the reversed value or check if the reversed value would exceed `INT_MAX` during construction (but since the task is pedagogical, using `long long` is safe and straightforward). Time complexity is O(d) where d is the number of digits (since each digit is processed once), and space complexity is O(1) auxiliary.
