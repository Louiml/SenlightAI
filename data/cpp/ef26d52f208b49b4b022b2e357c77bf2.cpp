// Write a C++ function named `convertIntToString` that takes an integer (which may be negative, zero, or positive) and returns its decimal representation as a `std::string` without using any standard library conversion functions like `std::to_string`, `std::ostringstream`, or `std::to_chars`. The function must correctly handle the special case of `INT_MIN`, where taking the absolute value would overflow a signed 32-bit integer. Your implementation must work by repeatedly extracting digits from the number and building the string in the correct order, with a leading minus sign for negative numbers. The function should be `const`-correct (i.e., take the integer by value and return a `std::string` by value) and should not modify any global state.

// The core idea is to repeatedly divide the magnitude of the number by 10, extracting the last digit each time as a character, until the number becomes zero. However, for negative numbers, the safest approach is to work with the negative value itself by using `x % 10` (which in C++11 and later yields a negative remainder for negative numbers) and adding `'0' - (x % 10)` to get the correct digit character. To avoid overflow with `INT_MIN`, we do not take `abs(x)`; instead, we loop while `x != 0`, and for each iteration, we compute `digit = -(x % 10)` (which is non-negative for negative `x`) and then reduce `x /= 10`. After the loop, if the original number was negative, we append a `'-'` at the end and then reverse the string. Zero is handled by a `do-while` loop that executes at least once, producing the string `"0"`. Edge cases include `x = 0` (should return `"0"`), `x = INT_MIN` (should not overflow), and `x = INT_MAX`. Time complexity is O(number of digits) ≈ O(log₁₀|x|) and space complexity is O(number of digits) for the returned string.

#include <string>
#include <algorithm>
#include <climits>

// Convert an integer to its decimal string representation without standard conversion helpers.
std::string convertIntToString(int x) {
    bool isNegative = x < 0;

    std::string result;
    do {
        // For negative x, x % 10 is non-positive; negate to get digit value 0-9.
        int digit = isNegative ? -(x % 10) : (x % 10);
        result.push_back('0' + digit);
        x /= 10;
    } while (x != 0);

    if (isNegative) {
        result.push_back('-');
    }

    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <string>
#include <climits>

// Declare the function under test (or include the header in a real setup).
std::string convertIntToString(int);

int main() {
    // Positive numbers
    assert(convertIntToString(0) == "0");
    assert(convertIntToString(5) == "5");
    assert(convertIntToString(123) == "123");
    assert(convertIntToString(987654321) == "987654321");

    // Negative numbers
    assert(convertIntToString(-1) == "-1");
    assert(convertIntToString(-456) == "-456");
    assert(convertIntToString(-1000000) == "-1000000");

    // Extremes
    assert(convertIntToString(INT_MAX) == "2147483647");
    assert(convertIntToString(INT_MIN) == "-2147483648");

    // Edge case: number with trailing zeros
    assert(convertIntToString(100) == "100");
    assert(convertIntToString(-100) == "-100");

    return 0;
}
