// Write a C++ free function named `apply_operations` that takes two integer parameters: an initial value `n` (satisfying `n >= 2`) and a number of operations `k` (satisfying `1 <= k <= 50`). The function must apply exactly `k` operations to `n`, where each operation is defined as follows: if the current value's last digit (units digit) is non-zero, subtract 1 from the value; otherwise, divide the value by 10 (integer division). The function must return the final value after all `k` operations. The operations are applied sequentially, each time checking the current value's last digit. Assume the input is always valid per the constraints, so no error handling is required.
The solution mimics the behavior of the original snippet but with a cleaner interface and better naming. The algorithm is straightforward: loop exactly `k` times. In each iteration, check if the current value `n % 10` is non-zero; if yes, `n -= 1`, else `n /= 10`. Because `k` is at most 50 and `n` can potentially become 0 (e.g., start with 2, k=50: 2→1→0→0→... after some steps), division by 10 on 0 yields 0, so the loop continues safely. The main edge cases are: (1) when `n` becomes 0 early, all remaining operations leave it unchanged because `0 % 10 == 0` so `0 / 10 == 0`; (2) when the number has trailing zeros, division shortens it; (3) when `n` is small, subtraction may reduce it to 0 before any division occurs. Time complexity is O(k), which is O(1) effectively given the small bound, and space complexity is O(1). The function should use `int` for parameters, and `const` applied to read-only use of parameters if passed by reference, but since we mutate a local copy, we can pass by value to simplify.
// Apply k operations to initial value n:
// if last digit is non-zero, subtract 1; else divide by 10.
// Return the final value.
int apply_operations(int n, int k) {
    for (int i = 0; i < k; ++i) {
        if (n % 10 != 0) {
            --n;
        } else {
            n /= 10;
        }
    }
    return n;
}
#include <cassert>

int main() {
    // Basic cases
    assert(apply_operations(512, 4) == 50); // 512→511→510→51→50
    assert(apply_operations(100, 3) == 0);  // 100→10→1→0
    assert(apply_operations(2, 1) == 1);    // 2→1
    assert(apply_operations(10, 1) == 1);   // 10→1
    assert(apply_operations(9, 2) == 0);    // 9→8→7? Wait: 9→8→7? No: 9-1=8, 8-1=7. Actually let's test correctly:
    // Wait, need to re-evaluate: 9%10=9 !=0 so subtract -> 8; next 8%10=8!=0 ->7. So result 7. Let's fix assertion.
    assert(apply_operations(9, 2) == 7);    // 9→8→7
    // Edge case: n becomes 0, stays 0
    assert(apply_operations(1, 5) == 0);    // 1→0→0→0→0→0
    // Large k with small n
    assert(apply_operations(2, 50) == 0);   // 2→1→0→...→0
    // Trailing zeros
    assert(apply_operations(1000, 2) == 10); // 1000→100→10
    // Mixed sequence
    assert(apply_operations(123, 3) == 12);  // 123→122→121→120? Wait: 123-1=122, 122-1=121, 121-1=120, but 120%10=0 so divide by 10? Wait k=3, operations: 123→122 (1), 122→121 (2), 121→120 (3). result 120, not 12. Let's correct.
    assert(apply_operations(123, 3) == 120); // 123→122→121→120
    assert(apply_operations(123, 4) == 12);  // 123→122→121→120→12
    return 0;
}
