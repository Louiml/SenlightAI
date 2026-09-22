// Given two integers `a` and `b` (where `a` and `b` can be negative, zero, or positive), write a C++ function that determines whether the pair is "valid" according to these rules: if `a` is odd and `b` is even, the answer is "no"; if `a` is odd and `b` is odd, the answer is "yes" only if `b > a` (i.e., the difference `b - a` is at least 1); if `a` is even and `b` is even, the answer is "yes" unless `a < 1` and `b` is odd (which can't happen because if `a` is even and `a < 1`, then `a` is 0 or negative, and `b` could be odd — in that case return "no"); if `a` is even and `b` is odd, the answer is "yes" only if `b <= 1` (i.e., `b` is either 1 or less, e.g., 0 or negative odd numbers like -1, -3, etc.); if `a` is even and `a < 1` and `b` is odd, return "no". For all other cases, return "no". The function should return a string `"yes"` or `"no"` exactly.

The problem is a conditional logic puzzle. The solution approach is to systematically evaluate the parity (even/odd) of `a` and `b` and the relationship between them. The rules essentially encode the following logic:
- If `a` is odd and `b` is even → "no".
- If `a` is odd and `b` is odd → check if `b - a >= 1` (i.e., `b > a`). If yes → "yes", else "no".
- If `a` is even:
   - If `b` is even → "yes" (unless `a < 1` and `b` is odd, but `b` is even here, so always "yes").
   - If `b` is odd:
      - If `a < 1` (i.e., `a <= 0`) → "no".
      - Else (a >= 1, with a even, e.g., 2, 4, ...) → check if `b <= 1`. If yes → "yes", else "no".
- Any other combination not covered defaults to "no".

Edge cases include negative numbers, zero, and when both are equal (e.g., `a=2,b=2` → "yes"; `a=2,b=3` → `b<=1`? no → "no"; `a=0,b=1` → `a<1` and `b` odd → "no"; `a=0,b=0` → both even → "yes"). The algorithm runs in O(1) time and uses O(1) extra space, since only a few integer comparisons are involved.

#include <string>

// Determine if the pair (a, b) is valid per the specified rules.
// Returns "yes" or "no".
std::string isValidPair(int a, int b) {
    // Case: a is odd
    if (a % 2 != 0) {
        if (b % 2 == 0) {
            return "no";
        } else {
            // both odd
            return (b - a >= 1) ? "yes" : "no";
        }
    }
    // Now a is even
    if (b % 2 == 0) {
        // both even
        return "yes";
    }
    // b is odd
    if (a < 1) {
        return "no";
    }
    // a is even and >= 2 (since a even and >=1)
    return (b <= 1) ? "yes" : "no";
}

#include <cassert>
#include <string>

// The solution function is declared above in the solution section.
// This test file includes it for testing.
std::string isValidPair(int a, int b);

int main() {
    // Odd a, even b → "no"
    assert(isValidPair(1, 2) == "no");
    // Odd a, odd b, b > a → "yes"
    assert(isValidPair(3, 5) == "yes");
    // Odd a, odd b, b <= a → "no"
    assert(isValidPair(5, 3) == "no");
    // Even a, even b → "yes"
    assert(isValidPair(4, 4) == "yes");
    // Even a, odd b, a >= 2, b <= 1 → "yes"
    assert(isValidPair(2, 1) == "yes");
    // Even a, odd b, a >= 2, b > 1 → "no"
    assert(isValidPair(2, 3) == "no");
    // Even a, even b with a=0 → "yes"
    assert(isValidPair(0, 0) == "yes");
    // Even a, odd b with a=0 → "no"
    assert(isValidPair(0, 1) == "no");
    // Even a, odd b with a negative (e.g., -2) → "no"
    assert(isValidPair(-2, 3) == "no");
    // Odd a, odd b with negative numbers, e.g., b-a = 2 → "yes"
    assert(isValidPair(-3, -1) == "yes");
}
