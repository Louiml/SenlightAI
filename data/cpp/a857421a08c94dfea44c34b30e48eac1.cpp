Write a C++ function named `exponentialPower` that computes the value of `x` raised to the power `y` using recursion, where `x` is an integer and `y` is a non-negative integer. The function must return an `int` result. You may assume the inputs are such that the result fits within the range of `int`. Do not use any built-in exponentiation operators or library functions like `pow`. Provide a recursive implementation that mirrors the divide-and-conquer property of exponentiation: if `y` is zero, return 1; otherwise, return `x` multiplied by the result of the function called with `y-1`. Keep the function efficient and correct for edge cases such as `y=0`, `x=0`, and `x=1` (with any exponent).
#include <cassert>

int main() {
    // Basic cases
    assert(exponentialPower(7, 3) == 343);
    assert(exponentialPower(2, 10) == 1024);
    // y = 0
    assert(exponentialPower(5, 0) == 1);
    assert(exponentialPower(0, 0) == 1); // convention: 0^0 = 1
    assert(exponentialPower(-3, 0) == 1);
    // x = 0, y > 0
    assert(exponentialPower(0, 5) == 0);
    // x = 1, any y
    assert(exponentialPower(1, 100) == 1);
    // Negative base, even/odd exponent
    assert(exponentialPower(-2, 3) == -8);
    assert(exponentialPower(-2, 4) == 16);
    // Large but valid result
    assert(exponentialPower(10, 6) == 1000000);
    // Single recursion step
    assert(exponentialPower(3, 1) == 3);

    return 0;
}
#include <cstdint>

// Recursively computes x raised to the power y, where y >= 0.
// Returns int; assumes the result fits within int range.
int exponentialPower(const int x, const int y) {
    if (y == 0) {
        return 1;
    }
    return x * exponentialPower(x, y - 1);
}
// The solution uses a straightforward recursive definition: `power(x, y) = x * power(x, y-1)` with a base case at `y == 0` returning `1`. This directly implements the mathematical definition of integer exponentiation. Important edge cases include: `y == 0` (return 1 regardless of `x`, including `0^0` which by convention we return 1 here), `x == 0` with `y > 0` (returns 0), and `x == 1` (returns 1 for any `y`). Negative exponents are not in the problem specification, so the function assumes `y >= 0`. The time complexity is O(y) because each call reduces `y` by 1, resulting in exactly `y+1` calls. The space complexity is O(y) due to the recursion stack. The implementation must be `const`-correct by marking parameters as `const` where appropriate (though here `int` parameters are passed by value, so `const` on the parameters is optional but can be used for clarity).
