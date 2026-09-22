Write a C++ function named `classifyParity` that takes two integers (which may be negative, zero, or positive) and returns a `std::string` containing two lines, each line being `"even"` or `"odd"` depending on the parity of the corresponding input number. The first line must correspond to the first integer, and the second line to the second integer. The function must not read from or write to standard input/output; it should only return the result as a string. The returned string must end with a newline character after the second line.
The solution requires checking the parity of each integer independently. A number is even if it is divisible by 2 (i.e., `number % 2 == 0`); otherwise, it is odd. Since the problem allows negative numbers, the modulo operator in C++ yields a negative remainder for negative odd numbers (e.g., `-3 % 2 == -1`), which is non-zero, so the condition `number % 2 == 0` still correctly identifies even numbers. Zero is even. The main algorithm is straightforward: evaluate the parity of `a`, append the corresponding word to the result string, add a newline, then do the same for `b`, and finally append a trailing newline. Edge cases include both numbers being the same, zero, negative, or any mix; no special handling is required. Time complexity is \(O(1)\) because only constant operations are performed, and space complexity is \(O(1)\) for the returned string, excluding the output itself.
#include <string>

// Returns a two-line string classifying the parity of a and b.
std::string classifyParity(int a, int b) {
    std::string result;
    result += (a % 2 == 0) ? "even" : "odd";
    result += "\n";
    result += (b % 2 == 0) ? "even" : "odd";
    result += "\n";
    return result;
}
#include <cassert>

int main() {
    // Both even
    assert(classifyParity(4, 8) == "even\neven\n");
    // Both odd
    assert(classifyParity(3, 7) == "odd\nodd\n");
    // First odd, second even
    assert(classifyParity(1, 2) == "odd\neven\n");
    // First even, second odd
    assert(classifyParity(-2, 5) == "even\nodd\n");
    // Zero is even
    assert(classifyParity(0, 0) == "even\neven\n");
    // Negative odd
    assert(classifyParity(-3, -4) == "odd\neven\n");
    // Same values
    assert(classifyParity(6, 6) == "even\neven\n");
    // Large numbers
    assert(classifyParity(1000000000, 999999999) == "even\nodd\n");
}
