Write a C++ function named `selectByCondition` that takes two integer parameters `base` and `flag`. The function must return the mathematical expression equivalent of an if-else statement: when `flag == -1`, it must return `2 * base + 1`; otherwise (for any other integer value of `flag`), it must return `base * base`. You must implement this without using any conditional statements (no `if`, `else`, ternary operator, or switch). Instead, use arithmetic with integer division and multiplication to compute a weight that is `1` when `flag == -1` and `0` otherwise, and another weight that is the opposite. The function must work correctly for all integer inputs (including `INT_MAX`, `INT_MIN`, and zero) and must not suffer from division-by-zero or undefined behavior.
The core idea is to convert the boolean condition `flag == -1` into an arithmetic quantity. Notice that `flag + 1` is zero exactly when `flag == -1`. However, division by zero is undefined, so we cannot directly divide by `flag + 1`. Instead, we can use the fact that for integers, `(flag + 1) / 2` is not reliable because negative values round toward zero in C++ (e.g., for `flag = -2`, `(-1)/2 = 0`, for `flag = 0`, `(1)/2 = 0`, for `flag = -1`, `(0)/2 = 0`). A more robust approach: For `flag == -1`, `flag + 1 == 0`, and `0 * 2 == 0`. For all other integers, `flag + 1` is non-zero, and `(flag + 1) * 2` is also non-zero but could be negative or positive. To convert any non-zero value to exactly `1` and zero to `0`, we need a function that maps 0→0 and any non-zero→1. A common trick is to use `(x != 0)` but that is a conditional. Instead, we can use the identity: For any integer `x`, `( (x == 0) ? 1 : 0 )` is not possible without conditionals. However, we can use the fact that `abs(x)` is non-negative, and for any integer `x`, `x / (abs(x) + 1)` is not reliable for edge cases like `INT_MIN`. A safer arithmetic method: Compute `delta = flag + 1`. Then use `is_neg_one = (delta == 0) ? 1 : 0` is not allowed; but we can compute `is_neg_one = (delta * delta == 0) ? 1 : 0` still conditional. The intended trick from the snippet is simpler: It uses `(b / 2) + 1` and `(b / 2)` to generate weights for `-1` and other values. For `b = -1`, `b/2 = 0` (since -1/2 truncates toward zero = 0), so `isNegOne = 0+1 = 1` and `isNotNegOne = 0`. For `b = 0`, `b/2 = 0`, then `isNegOne = 1` is wrong (should be 0 for b=0). So the snippet is actually buggy for many inputs. We must create a correct version. The correct arithmetic test for `flag == -1` without conditionals: Use integer division with the property that `(flag + 1) / (flag + 1)` is 1 when non-zero, but fails at zero. Instead, we can use the sign function trick: For any integer `n`, `(n > 0) - (n < 0)` gives -1,0,1 but uses comparisons. However, there is a known arithmetic way: `int is_zero = 1 / (abs(n) + 1)` gives 1 only when n=0, but abs(INT_MIN) is UB. Another trick: Use `bool` conversion is conditional. The simplest robust approach: Since we only care about `flag == -1`, we can observe that `flag + 1` is zero exactly then. We can compute a value that is 1 when zero and 0 otherwise using the formula: `1 / ( (flag+1)*(flag+1) + 1 )`? That gives 1 when denominator=1 (i.e., when flag+1=0), else denominator>1 so fraction is 0 due to integer division. Indeed, `(flag+1)*(flag+1)` is always non-negative, and when flag+1=0 it is 0, denominator=1, so `1/1=1`; otherwise denominator ≥ 2, `1/denominator = 0`. This works for all integers including INT_MIN because we only square `flag+1` which is within range (since flag+1 might overflow if flag=INT_MAX? If flag=INT_MAX, flag+1 overflows. So we need to avoid overflow. We can use `long long` to safely compute. Let `long long d = static_cast<long long>(flag) + 1;` then `d*d` is safe (since d is in range of long long). Then `is_neg = 1 / (d*d + 1)` works. `is_not_neg = 1 - is_neg`. Then result = `(2*base+1)*is_neg + (base*base)*is_not_neg`. This uses no conditionals. Edge case: base*base might overflow for INT_MAX, but the problem likely expects 64-bit result? The original snippet uses int multiplication and may overflow; we can use `long long` for the result to avoid overflow. The time complexity is O(1) and space O(1). The solution must handle all integer flags correctly.
#include <cstdint>

// Return 2*base+1 when flag == -1, otherwise base*base, using arithmetic only.
long long selectByCondition(int base, int flag) {
    const long long d = static_cast<long long>(flag) + 1; // 0 exactly when flag == -1
    const long long isNegOne = 1 / (d * d + 1); // 1 if d==0, else 0
    const long long isNotNegOne = 1 - isNegOne; // opposite
    return (2LL * base + 1) * isNegOne + (static_cast<long long>(base) * base) * isNotNegOne;
}
#include <cassert>
#include <climits>

// Declaration (included here for completeness; normally from header)
long long selectByCondition(int base, int flag);

int main() {
    // Basic cases
    assert(selectByCondition(10, -1) == 21);       // 2*10+1
    assert(selectByCondition(10, 0) == 100);       // 10^2
    assert(selectByCondition(10, 1) == 100);       // 10^2
    assert(selectByCondition(0, -1) == 1);         // 2*0+1
    assert(selectByCondition(-5, -1) == -9);       // 2*(-5)+1
    assert(selectByCondition(-5, 3) == 25);        // (-5)^2

    // Boundary flag values
    assert(selectByCondition(7, INT_MIN) == 49);   // not -1 -> square
    assert(selectByCondition(7, INT_MAX) == 49);   // not -1 -> square

    // Large base to check overflow safety
    assert(selectByCondition(INT_MAX, -1) == 2LL * INT_MAX + 1); // 4294967295
    assert(selectByCondition(INT_MAX, 5) == 1LL * INT_MAX * INT_MAX); // 4611686014132420609

    // Many random checks (spot-test a few)
    assert(selectByCondition(-100, -1) == -199);
    assert(selectByCondition(3, 42) == 9);
    assert(selectByCondition(1, -1) == 3);
    assert(selectByCondition(2, -1) == 5);
}
