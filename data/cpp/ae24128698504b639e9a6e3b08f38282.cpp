Write a C++ function named `isPerfectNumber` that takes an integer parameter and returns a boolean value indicating whether the input is a perfect number. A perfect number is a positive integer that equals the sum of its proper divisors (all positive divisors excluding the number itself). The function should handle negative numbers, zero, and positive integers correctly, returning `false` for any non-positive input. Your implementation should be efficient enough to handle moderately large integers (e.g., up to 10^6) without excessive runtime.
#include <cassert>

int main() {
    // Small known perfect numbers.
    assert(isPerfectNumber(6) == true);   // 1+2+3 = 6
    assert(isPerfectNumber(28) == true);  // 1+2+4+7+14 = 28
    assert(isPerfectNumber(496) == true); // 1+2+4+8+16+31+62+124+248 = 496
    
    // Non-perfect positives.
    assert(isPerfectNumber(1) == false);   // No proper divisors, sum=0.
    assert(isPerfectNumber(12) == false);  // 1+2+3+4+6 = 16 ≠ 12.
    assert(isPerfectNumber(100) == false); // Sum of divisors > 100.
    
    // Non-positive inputs.
    assert(isPerfectNumber(0) == false);
    assert(isPerfectNumber(-6) == false);
    assert(isPerfectNumber(-28) == false);
    
    // Larger perfect number.
    assert(isPerfectNumber(8128) == true);
    
    return 0;
}
#include <cstdint>

// Returns true if and only if the given integer is a perfect number.
// A perfect number is a positive integer equal to the sum of its proper divisors.
bool isPerfectNumber(const int number) {
    if (number <= 0) {
        return false;  // Perfect numbers are defined only for positive integers.
    }
    
    int sumOfDivisors = 0;
    // Proper divisors are all positive divisors excluding the number itself.
    // No divisor can exceed number/2 (except the number itself), so iterate up to that.
    for (int i = 1; i <= number / 2; ++i) {
        if (number % i == 0) {
            sumOfDivisors += i;
        }
    }
    
    return sumOfDivisors == number;
}
// The main algorithm involves iterating through potential divisors from 1 up to half of the input number (`number/2`), because any divisor greater than half the number (except the number itself) is impossible. For each `i`, if `number % i == 0`, add `i` to an accumulator. After the loop, compare the accumulator to the original number. If equal and the number is positive, return true; otherwise false. Edge cases to handle: input `1` (no proper divisors, sum=0, so false), input `0` (no proper divisors, but should return false because it's not positive), negative numbers (immediately false). Complexity: O(n) time where n is the input, but in practice O(number/2) iterations; space O(1). For large inputs near 10^6, this is acceptable (500,000 iterations max). A more optimal solution using divisors up to sqrt(n) exists but is not required for the specified range.
