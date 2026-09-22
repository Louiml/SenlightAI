// Write a C++ function named `classifyParity` that accepts a single integer parameter and returns a string indicating whether the number is even ("Even") or odd ("Odd"). The function must correctly handle positive integers, negative integers, and zero. Use the modulo operator to determine parity, and ensure the function is `const`-correct by taking the integer parameter by value.

// The solution uses the modulo operator `% 2` on the input integer. In C++, the result of `n % 2` is `0` for any even number (including negative even numbers and zero) and `1` or `-1` for odd numbers (depending on the sign). Therefore, the condition `n % 2 == 0` correctly identifies even numbers, while all other results identify odd numbers. Edge cases: zero is even, and negative odd numbers yield `-1`, which is not equal to `0`, so they are correctly classified as odd. The algorithm performs one arithmetic operation and one comparison, so both time and space complexity are O(1), independent of input magnitude.

#include <string>

// Return "Even" if the input is divisible by 2, otherwise "Odd".
// Handles positive, negative, and zero correctly.
std::string classifyParity(int n) {
    return (n % 2 == 0) ? "Even" : "Odd";
}

#include <cassert>
#include <string>

std::string classifyParity(int n);

int main() {
    assert(classifyParity(0) == "Even");
    assert(classifyParity(2) == "Even");
    assert(classifyParity(-4) == "Even");
    assert(classifyParity(7) == "Odd");
    assert(classifyParity(-3) == "Odd");
    assert(classifyParity(1000) == "Even");
    assert(classifyParity(999) == "Odd");
    assert(classifyParity(1) == "Odd");
    assert(classifyParity(-1) == "Odd");
    assert(classifyParity(2147483647) == "Odd"); // Edge case: INT_MAX
    return 0;
}
