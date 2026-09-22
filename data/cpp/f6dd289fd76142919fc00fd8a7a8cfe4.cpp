// Write a C++ function `int productOfDigits(int num)` that accepts a positive three-digit integer (100–999) and returns the product of its hundreds, tens, and units digits. For example, input 253 should return `2 * 5 * 3 = 30`. The function must handle only valid three-digit inputs; you may assume the caller passes a value in the range [100, 999] (no need to validate). The solution must use integer arithmetic only (no string conversion) and must be free of side effects.

// The task is straightforward: extract each digit using integer division and modulo operations. Given `num`, the hundreds digit is obtained by integer division by 100 (`num / 100`). The tens digit is obtained by first taking the remainder after division by 100 (`num % 100`), then integer division by 10. The units digit is simply `num % 10`. Multiply the three digits together and return the result. Edge cases: since the input is guaranteed to be a three-digit positive integer, no special handling for zero or negative numbers is needed. If num were 100, the product is 1*0*0 = 0; if num were 999, product = 729. Time complexity is O(1) (constant number of arithmetic operations), and space complexity is O(1) (only a few integer variables).

#include <cstdint>

// Returns the product of the hundreds, tens, and units digits of a three-digit integer.
// Precondition: num is in the range [100, 999].
int productOfDigits(int num) {
    const int hundreds = num / 100;
    const int tens = (num % 100) / 10;
    const int units = num % 10;
    return hundreds * tens * units;
}

#include <cassert>

int productOfDigits(int num);

int main() {
    assert(productOfDigits(100) == 0);  // 1*0*0
    assert(productOfDigits(123) == 6);  // 1*2*3
    assert(productOfDigits(251) == 10); // 2*5*1
    assert(productOfDigits(999) == 729);// 9*9*9
    assert(productOfDigits(370) == 0);  // 3*7*0
    assert(productOfDigits(505) == 0);  // 5*0*5
    assert(productOfDigits(864) == 192);// 8*6*4
    return 0;
}
