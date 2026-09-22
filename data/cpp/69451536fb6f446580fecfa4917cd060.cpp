/*
Write a C++ function named `classifyEvenOdd` that takes an integer parameter `number` and returns a `std::string` indicating whether the number is even or odd. The returned string must follow the exact format: `"<number> is even."` for even numbers and `"<number> is odd."` for odd numbers, where `<number>` is the integer value itself. The function must handle negative integers correctly, treat zero as even, and use `const` for the parameter where appropriate. The function should not perform any console I/O; all output must be returned via the string. Ensure the implementation is self-contained with necessary headers and a descriptive free function (no `main` function required).
*/
#include <string>

// Classify an integer as even or odd and return a descriptive string.
// The string format is "<number> is even." or "<number> is odd."
std::string classifyEvenOdd(const int number) {
    if (number % 2 == 0) {
        return std::to_string(number) + " is even.";
    } else {
        return std::to_string(number) + " is odd.";
    }
}
#include <cassert>
#include <string>

// Forward declaration of the function under test (should be in same translation unit)
std::string classifyEvenOdd(const int number);

int main() {
    // Positive even and odd
    assert(classifyEvenOdd(4) == "4 is even.");
    assert(classifyEvenOdd(7) == "7 is odd.");
    
    // Zero is even
    assert(classifyEvenOdd(0) == "0 is even.");
    
    // Negative numbers
    assert(classifyEvenOdd(-2) == "-2 is even.");
    assert(classifyEvenOdd(-3) == "-3 is odd.");
    
    // Large values
    assert(classifyEvenOdd(2147483646) == "2147483646 is even.");
    assert(classifyEvenOdd(-2147483647) == "-2147483647 is odd.");
    
    // Single digit extremes
    assert(classifyEvenOdd(1) == "1 is odd.");
    assert(classifyEvenOdd(2) == "2 is even.");
    
    return 0;
}
// The solution is straightforward: check the parity of the integer using the modulo operator `%`. For an integer `n`, `n % 2` yields `0` for even numbers and `1` for odd numbers in C++ (note: for negative odd numbers, `n % 2` returns `-1` in some implementations, but the condition `n % 2 == 0` still correctly identifies evenness, and the else branch handles both `1` and `-1` as odd). Zero is even because `0 % 2 == 0`. The algorithm uses `std::to_string` to convert the integer to its string representation, then concatenates with the appropriate suffix. Time complexity is O(1) as it performs a constant number of operations. Space complexity is O(1) auxiliary, plus O(k) for the returned string where k is the number of digits in the integer (bounded by the integer range, so effectively O(1)). Edge cases include negative numbers (e.g., `-3` should return `"-3 is odd."`) and zero (should return `"0 is even."`). The function should use `const int number` or `const int&`? Since it's a simple type, passing by value with `const` is acceptable, but to demonstrate `const` correctness, we'll declare the parameter as `const int number` (though it's redundant, it shows intent) or use `[[maybe_unused]]` if needed. For clarity, we'll use `const int number` to satisfy the const-correctness requirement.
