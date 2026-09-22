Write a C++ function named `parityProduct` that takes two integer parameters, `a` and `b`, and returns a `std::string` containing either `"Even"` if the product of the two numbers is even, or `"Odd"` if the product is odd. The function must not actually multiply the numbers; instead, it must determine the parity using only the parity of the inputs (i.e., whether each is divisible by 2). The function should handle negative numbers, zero, and very large integers correctly (within the range of `int`), and must be `const`-correct and self-contained with necessary headers included.

// The parity of a product of two integers is even if either factor is even, and odd only if both factors are odd. This follows from the fundamental property of multiplication modulo 2: `(a mod 2) × (b mod 2) mod 2`. Therefore, we check if `a % 2 == 0` OR `b % 2 == 0`; if so, return `"Even"`, otherwise return `"Odd"`. Edge cases include: zero (which is even, so the product is even regardless of the other number), negative numbers (in C++, `%` for negative integers yields a negative remainder, but since we only compare against 0, the check `n % 2 == 0` works correctly for negative even numbers like `-4` and negative odd numbers like `-3` give `-1 != 0`). Large integers within `int` range are fine because we never multiply. Time complexity is O(1), space complexity is O(1).

#include <string>

// Determine the parity of the product of two integers without multiplying them.
// Returns "Even" if the product is even, "Odd" if the product is odd.
std::string parityProduct(const int a, const int b) {
    if (a % 2 == 0 || b % 2 == 0) {
        return "Even";
    }
    return "Odd";
}

#include <cassert>
#include <string>

// Forward declaration of the solution function (required for testing)
std::string parityProduct(const int a, const int b);

int main() {
    // Both odd -> product odd
    assert(parityProduct(3, 5) == "Odd");
    assert(parityProduct(-3, -5) == "Odd");
    
    // At least one even -> product even
    assert(parityProduct(2, 3) == "Even");
    assert(parityProduct(3, 2) == "Even");
    assert(parityProduct(0, 7) == "Even");
    assert(parityProduct(-4, 5) == "Even");
    
    // Both even -> product even
    assert(parityProduct(4, 6) == "Even");
    assert(parityProduct(-2, -8) == "Even");
    
    // Large values within int range (both odd)
    assert(parityProduct(2147483647, 2147483647) == "Odd");
    
    // Large and even
    assert(parityProduct(2147483646, 2147483647) == "Even");
    
    return 0;
}
