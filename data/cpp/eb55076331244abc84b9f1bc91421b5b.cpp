// Write a C++ function named `convertToBase7` that accepts an integer `num` and returns its representation in base 7 as a `std::string`. The result must include a leading minus sign for negative numbers, and the string must contain no leading zeros except for the single digit "0" when the input is exactly zero. The function should handle all representable 32-bit signed integers, including the most negative value (`INT_MIN`), without relying on overflow-prone negation.

#include <cassert>
#include <string>

// Declare the function (it is defined elsewhere, but for testing we include a forward declaration)
std::string convertToBase7(int num);

int main() {
    assert(convertToBase7(0) == "0");
    assert(convertToBase7(7) == "10");
    assert(convertToBase7(-7) == "-10");
    assert(convertToBase7(100) == "202");
    assert(convertToBase7(-100) == "-202");
    assert(convertToBase7(1) == "1");
    assert(convertToBase7(-1) == "-1");
    assert(convertToBase7(48) == "66");   // 6*7 + 6 = 48
    assert(convertToBase7(-49) == "-100");
    assert(convertToBase7(2147483647) == "341234102134104"); // 2^31-1 in base 7
    // INT_MIN = -2147483648 -> base 7 representation, check without overflow
    assert(convertToBase7(-2147483647 - 1) == "-341234102134105");
    return 0;
}

#include <string>

// Convert an integer to its base-7 representation as a string.
// Handles negative numbers and the special case of INT_MIN without overflow.
std::string convertToBase7(int num) {
    if (num == 0) {
        return "0";
    }

    bool negative = (num < 0);
    // Use long long to safely hold abs(INT_MIN)
    long long value = num;
    if (negative) {
        value = -value;
    }

    std::string result = "";
    while (value > 0) {
        int digit = static_cast<int>(value % 7);
        result = std::to_string(digit) + result;
        value /= 7;
    }

    if (negative) {
        result = "-" + result;
    }
    return result;
}

// The core algorithm repeatedly extracts the least significant base-7 digit using `num % 7`, appends it to the front of the result string, and then divides `num` by 7 to shift to the next digit. Because `%` with negative numbers yields a non-positive remainder in C++, we first record whether the input is negative, then work with the absolute value in a non-overflowing way. A common pitfall is `INT_MIN`, whose absolute value cannot be represented in a signed 32-bit `int`. To avoid this, we can use a `long long` copy of the absolute value, or process the magnitude using unsigned or long arithmetic. Edge cases include zero (which must return "0"), negative numbers (prefix with "-"), and numbers like 7 or -7 (which yield "10" and "-10", respectively). The time complexity is O(log₇|num|), as each iteration reduces the number by a factor of 7. Space complexity is O(log₇|num|) for the output string, which is necessary to store the result.
