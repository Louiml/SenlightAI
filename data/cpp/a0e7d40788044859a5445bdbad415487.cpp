Create a C++ function that takes an integer `N` as input and returns the string `"odd"` if the number is odd and `"even"` if the number is even. The function should use bitwise operations only (specifically, the bitwise AND operator with `1`) to determine the parity, avoiding the modulo operator. The input integer can be negative, zero, or positive, and negative odd numbers must still be correctly identified. The function should be declared as a member of a class named `Solution` with a public method `oddEven`, but for the standalone reference solution, provide a free function `classifyParity` that accomplishes the same task. The function must return a `std::string` in lowercase exactly as specified.
The core algorithm is straightforward: a number is odd if and only if its least significant bit (LSB) is `1`; otherwise, it is even. This is determined by evaluating `(N & 1)`. The bitwise AND operation with `1` extracts only the LSB, yielding `1` for odd numbers and `0` for even numbers. This works for negative integers in two's complement representation because the LSB still correctly indicates parity (e.g., `-3` in two's complement has `1` in the LSB, making it odd). The main edge case is zero: `0 & 1` equals `0`, so it is correctly classified as even. No other special cases exist, and duplicate or large values do not affect the result. Time complexity is \(O(1)\) because only a single bitwise operation is performed, and space complexity is \(O(1)\) since only a constant amount of memory is used (excluding the returned string). The solution must ensure the comparison `(N & 1) == 1` is written correctly with parentheses to avoid operator precedence issues (in the original snippet, `if (N&1==1)` is a common pitfall because `==` has higher precedence than `&`, but the corrected version explicitly parenthesizes `(N & 1)`).
#include <string>

// Classify an integer as "odd" or "even" using bitwise AND.
// Returns "odd" if the least significant bit is 1, otherwise "even".
std::string classifyParity(const int N) {
    // (N & 1) evaluates to exactly 1 for odd numbers and 0 for even numbers.
    if ((N & 1) == 1) {
        return "odd";
    }
    return "even";
}
#include <cassert>
#include <string>

// The solution function declared here must be defined above.
std::string classifyParity(const int N);

int main() {
    // Positive odd and even numbers
    assert(classifyParity(1) == "odd");
    assert(classifyParity(2) == "even");
    assert(classifyParity(7) == "odd");
    assert(classifyParity(100) == "even");
    
    // Zero is even
    assert(classifyParity(0) == "even");
    
    // Negative odd and even numbers
    assert(classifyParity(-1) == "odd");
    assert(classifyParity(-2) == "even");
    assert(classifyParity(-3) == "odd");
    assert(classifyParity(-4) == "even");
    
    // Large values (both positive and negative)
    assert(classifyParity(2147483647) == "odd");
    assert(classifyParity(-2147483648) == "even");
    
    return 0;
}
