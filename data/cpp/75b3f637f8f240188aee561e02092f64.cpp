Write a C++ function named `classifyInteger` that takes an integer `n` and returns a `std::string` containing either `"odd"` or `"even"` based on the parity of `n`. The function must handle negative integers correctly (e.g., -3 is odd, -4 is even), as well as zero (which is even). Do not use the modulo operator (`%`); instead, use a bitwise operation to determine parity. The function should be `const`-correct, meaning it does not modify its input, and it should reside in the global namespace with no external dependencies other than standard headers.
#include <cassert>
#include <string>

// Declaration of the function under test (already defined in the solution above)
std::string classifyInteger(int n);

int main() {
    // Basic positive numbers
    assert(classifyInteger(1) == "odd");
    assert(classifyInteger(2) == "even");
    assert(classifyInteger(0) == "even");
    
    // Negative numbers
    assert(classifyInteger(-1) == "odd");
    assert(classifyInteger(-2) == "even");
    assert(classifyInteger(-3) == "odd");
    
    // Extremes
    assert(classifyInteger(INT_MAX) == "odd");  // 2147483647 is odd
    assert(classifyInteger(INT_MIN) == "even"); // -2147483648 is even (LSB 0)
    
    // Larger values within int range
    assert(classifyInteger(1000000) == "even");
    assert(classifyInteger(999999) == "odd");
    
    // A few more cases to ensure consistency
    assert(classifyInteger(7) == "odd");
    assert(classifyInteger(8) == "even");
    
    return 0;
}
#include <string>

// Determines whether an integer is odd or even using a bitwise operation.
// Returns "odd" if n is odd, "even" otherwise (including zero and negatives).
std::string classifyInteger(int n) {
    return (n & 1) ? "odd" : "even";
}
// The simplest way to determine parity without the modulo operator is to use the bitwise AND operator with 1: `(n & 1)`. In two's complement representation, the least significant bit (LSB) is 1 for odd numbers and 0 for even numbers, regardless of sign. For example, -3 in binary (assuming 32-bit) is `...11111101`, so `(-3 & 1)` equals 1, identifying it as odd; -4 is `...11111100`, so `(-4 & 1)` equals 0, identifying it as even. The function returns a `std::string` constructed from a ternary operator: `(n & 1) ? "odd" : "even"`. Edge cases: zero has LSB 0, so it is correctly classified as even; large values (e.g., INT_MAX and INT_MIN) work because bitwise operations are defined for signed integers. Time complexity is O(1) since only a single bitwise operation and a string comparison/construction are performed. Space complexity is O(1) for the computation, though the returned string itself requires constant space for its characters (the strings `"odd"` and `"even"` are literals, stored in static storage). No loops or recursion are involved.
