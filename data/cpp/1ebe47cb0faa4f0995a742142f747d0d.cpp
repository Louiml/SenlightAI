Write a C++ function `transformNumber` that takes two integers `a` (a three-digit positive integer) and `b` (a single-digit integer from 0 to 9), and returns a struct `Result` containing three integers: `first`, `second`, and `third`. The transformation rules are based on the digits of `a`: let `hundreds = a/100`, `tens = (a%100)/10`, `units = a%10`. If `hundreds == b`, then the result should be: `first = a * 10` (append a zero), then if `tens == b`, multiply the original `tens` digit by 10 and store that in the `second` field; if `units == b`, multiply `a` (already multiplied by 10) by 10 again (append another zero) and store that in the `first` field. If `hundreds != b` but `units == b`, then the result should be: `first = a` (unchanged), `second = units * 10` (in the `second` field), and `third` remains the original units digit. In all other cases, the result should simply return `a` in the `first` field, the original `tens` digit in `second`, and the original `units` digit in `third`. Note that the original code's logic is flawed (e.g., variable misuse), so your function must implement the intended behavior as described above, not replicate the buggy code exactly.
// The solution requires decomposing the three-digit integer `a` into its hundreds, tens, and units digits using integer division and modulus. The main challenge is to carefully apply the conditional logic in the correct order, since the original code has ambiguities and bugs. The algorithm: compute `h = a/100`, `t = (a%100)/10`, `u = a%10`. If `h == b`: set `first = a*10` (append a zero). Then if `t == b`, set `second = t*10`; otherwise leave `second` as `t`. If `u == b`, multiply `first` by 10 again (append another zero). If `h != b` and `u == b`: set `first = a`, `second = u*10`, `third = u`. Otherwise (no special conditions): `first = a`, `second = t`, `third = u`. Edge cases: when `a` is exactly a multiple of 10 (unit digit zero), `u` can be zero; comparisons with `b` work fine. When `b` equals multiple digits, the order matters: the first condition (`h == b`) takes precedence, so if both hundreds and units equal `b`, only the first branch executes. The function returns a struct with three ints. Time complexity is O(1) and space complexity is O(1).
#include <cstddef>

struct Result {
    int first;
    int second;
    int third;
};

// Transforms a three-digit integer a based on digit b according to the rules.
Result transformNumber(int a, int b) {
    const int hundreds = a / 100;
    const int tens = (a % 100) / 10;
    const int units = a % 10;

    if (hundreds == b) {
        int first = a * 10;  // append one zero
        int second = tens;
        int third = units;

        if (tens == b) {
            second = tens * 10;  // multiply tens digit by 10
        }
        if (units == b) {
            first = first * 10;  // append another zero
        }
        return {first, second, third};
    }

    if (units == b) {
        return {a, units * 10, units};
    }

    return {a, tens, units};
}
#include <cassert>

int main() {
    // Basic case: no digits match b
    Result r1 = transformNumber(123, 9);
    assert(r1.first == 123);
    assert(r1.second == 2);
    assert(r1.third == 3);

    // Hundreds digit equals b, no other matches
    Result r2 = transformNumber(123, 1);
    assert(r2.first == 1230);  // appended zero
    assert(r2.second == 2);
    assert(r2.third == 3);

    // Hundreds and tens equal b
    Result r3 = transformNumber(223, 2);
    assert(r3.first == 2230);
    assert(r3.second == 20);  // tens digit multiplied by 10
    assert(r3.third == 3);

    // Hundreds and units equal b
    Result r4 = transformNumber(121, 1);
    assert(r4.first == 12100);  // two zeros appended
    assert(r4.second == 2);
    assert(r4.third == 1);

    // Hundreds not equal b, units equal b
    Result r5 = transformNumber(123, 3);
    assert(r5.first == 123);
    assert(r5.second == 30);  // units digit multiplied by 10
    assert(r5.third == 3);

    // Units digit is zero, but b equals hundreds
    Result r6 = transformNumber(100, 1);
    assert(r6.first == 1000);
    assert(r6.second == 0);
    assert(r6.third == 0);

    // b equals units which is zero
    Result r7 = transformNumber(100, 0);
    assert(r7.first == 100);
    assert(r7.second == 0);
    assert(r7.third == 0);

    // All three digits equal b
    Result r8 = transformNumber(222, 2);
    assert(r8.first == 22200);
    assert(r8.second == 20);
    assert(r8.third == 2);

    // Non-match case with units equal b
    Result r9 = transformNumber(405, 5);
    assert(r9.first == 405);
    assert(r9.second == 50);
    assert(r9.third == 5);
}
