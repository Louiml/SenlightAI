// Write a C++ function `std::string parityMessage(int number)` that takes an integer and returns a string indicating whether it is even or odd, in the format `"The number is even."` or `"The number is odd."` (with the period). The function must handle negative numbers correctly (e.g., -4 is even, -3 is odd), zero (even), and any integer within the standard `int` range. Do not read from standard input or print anything; the function should purely compute and return the result.

// The solution uses the modulo operator `%` to check divisibility by 2. In C++, the expression `number % 2` yields 0 for even numbers and 1 or -1 for odd numbers, depending on the sign (e.g., -3 % 2 equals -1). Therefore, we cannot simply compare to 1; we must compare to 0. So the main algorithm is: if `number % 2 == 0`, return the even message; otherwise, return the odd message. Edge cases include zero (0 % 2 == 0 → even), negative even numbers (-4 % 2 == 0 → even), and negative odd numbers (-3 % 2 == -1 → odd). Since the function only performs a constant-time operation and string construction, the time complexity is O(1) and auxiliary space is O(1) for the returned string (the string's length is fixed, ~20 characters). No iteration or extra data structures are needed.

#include <string>

// Return a message indicating whether the input integer is even or odd.
std::string parityMessage(int number) {
    if (number % 2 == 0) {
        return "The number is even.";
    } else {
        return "The number is odd.";
    }
}

#include <cassert>
#include <string>

std::string parityMessage(int number);

int main() {
    assert(parityMessage(0) == "The number is even.");
    assert(parityMessage(2) == "The number is even.");
    assert(parityMessage(-4) == "The number is even.");
    assert(parityMessage(1) == "The number is odd.");
    assert(parityMessage(-3) == "The number is odd.");
    assert(parityMessage(1000000) == "The number is even.");
    assert(parityMessage(-1000001) == "The number is odd.");
    return 0;
}
