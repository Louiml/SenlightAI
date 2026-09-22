// Write a C++ function named `digitSum` that takes a non-negative integer `n` as its parameter and returns the sum of its decimal digits. The function must be implemented recursively, following the logic of the given snippet: if `n` is zero, return zero; otherwise, add the last digit (`n % 10`) to the recursive sum of the remaining digits (`n / 10`). The function should handle the edge case of `n = 0` correctly (returning 0). Ensure the function is `const`-correct (i.e., it does not modify its parameter) and uses appropriate integer types. The solution must be self-contained with necessary headers, but no `main` function.

#include <cassert>

int digitSum(int n);  // declare to satisfy linker

int main() {
    assert(digitSum(0) == 0);
    assert(digitSum(5) == 5);
    assert(digitSum(10) == 1);
    assert(digitSum(123) == 6);
    assert(digitSum(999) == 27);
    assert(digitSum(100000) == 1);
    assert(digitSum(10101) == 3);
    assert(digitSum(2147483647) == 46); // max int, sum digits = 2+1+4+7+4+8+3+6+4+7 = 46
    return 0;
}

// Return the sum of the decimal digits of a non-negative integer.
// Recursive: base case returns 0 for 0, otherwise adds last digit to sum of the rest.
int digitSum(int n) {
    if (n == 0) {
        return 0;
    }
    return (n % 10) + digitSum(n / 10);
}

// The algorithm directly mirrors the recursive definition of digit sum: base case when `n == 0` returns 0, and the recursive case extracts the last digit via modulo 10 and sums it with the result of `digitSum(n / 10)`. For any non-negative integer, repeated division by 10 reduces the number of digits, guaranteeing termination. Edge cases include `n = 0` (returns 0), single-digit numbers (e.g., 5 → 5 + digitSum(0) = 5), and numbers with internal zeros (e.g., 100 → 0 + 0 + 1 = 1). The time complexity is \(O(d)\) where \(d\) is the number of decimal digits in `n` (since each call processes one digit). The space complexity is also \(O(d)\) due to the recursion stack. The implementation uses `int` for input and return, matching common constraints where `n` is a non-negative 32-bit integer.
