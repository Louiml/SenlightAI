Create a C++ function that determines whether a given positive integer is an "automorphic number," meaning its square ends with the same digits as the number itself. The function should take a single integer parameter and return a boolean value: `true` if the number is automorphic, `false` otherwise. The function must handle any positive integer, including 1-digit numbers and large numbers, without using floating-point arithmetic for the core logic. Additionally, the function should be `const`-correct and not modify the input parameter. You may use `std::pow` with integer types or construct the appropriate power of 10 using integer operations, but avoid floating-point rounding issues.

The solution involves counting the number of digits in the input number, then computing the modulus divisor as 10 raised to that digit count. For example, if the number is 25, it has 2 digits, so the divisor is 100. Then check if `(number * number) % divisor == number`. If true, the square ends with the original number, making it automorphic.

Key edge cases:
- Single-digit numbers: all single-digit numbers 0–9 except 0? Actually 0 and 1 are automorphic, but the problem states positive integers, so handle 1–9. For example, 5²=25 ends with 5, so 5 is automorphic; 6²=36 ends with 6, so 6 is automorphic; 2²=4 does not end with 2.
- Numbers where the square might overflow `int`: use `long long` for the square to avoid overflow for typical 32-bit inputs (up to ~46340 for int square), but if the input can be larger, use `unsigned long long`. Since the task doesn't specify range, it's safer to use `long long`.
- The digit-count loop: use a copy of the number and divide by 10 until zero. For number 0, the loop would skip, but since the input is positive, it's fine.
- For numbers like 100, the digit count is 3, and 100²=10000, which ends with 100, so automorphic. The algorithm works.
- Avoid `pow` from `<cmath>` with floating-point because it may lose precision for large exponents; instead, compute the divisor by multiplying 10 in a loop.

Time complexity: O(d) where d is the number of digits (since we loop to count digits and then compute the divisor). Space complexity: O(1).

#include <cstddef>

// Check if a positive integer is automorphic (its square ends with the same digits).
// Returns true if the number's square ends with the number itself; false otherwise.
bool isAutomorphic(long long num) {
    if (num <= 0) {
        return false; // Handle non-positive as not automorphic (or could handle 0 but task says positive)
    }
    
    // Count the number of digits in num
    long long temp = num;
    std::size_t digitCount = 0;
    while (temp > 0) {
        temp /= 10;
        digitCount++;
    }
    
    // Compute 10^digitCount using integer multiplication to avoid floating-point issues
    long long divisor = 1;
    for (std::size_t i = 0; i < digitCount; ++i) {
        divisor *= 10;
    }
    
    // Check if the square ends with the original number
    long long square = num * num;
    return (square % divisor) == num;
}

#include <cassert>

// Global main function for testing the solution
int main() {
    // Single-digit automorphic numbers (positive)
    assert(isAutomorphic(1) == true);  // 1²=1 ends with 1
    assert(isAutomorphic(5) == true);  // 5²=25 ends with 5
    assert(isAutomorphic(6) == true);  // 6²=36 ends with 6
    assert(isAutomorphic(2) == false); // 2²=4 does not end with 2
    
    // Multi-digit automorphic numbers
    assert(isAutomorphic(25) == true);  // 25²=625 ends with 25
    assert(isAutomorphic(76) == true);  // 76²=5776 ends with 76
    assert(isAutomorphic(100) == true); // 100²=10000 ends with 100
    
    // Non-automorphic numbers
    assert(isAutomorphic(12) == false); // 12²=144 does not end with 12
    assert(isAutomorphic(99) == false); // 99²=9801 does not end with 99
    
    // Larger number without overflow (square fits in long long)
    assert(isAutomorphic(90625) == true);  // 90625²=8212890625 ends with 90625
    assert(isAutomorphic(890625) == true); // 890625²=793212890625 ends with 890625
    
    return 0;
}
