/*
Write a C++ function that computes the digital root of a non-negative integer. The digital root is the recursive sum of all digits until a single-digit number is obtained. For example, the digital root of 942 is 9 + 4 + 2 = 15, then 1 + 5 = 6, so the result is 6. The function must handle all non-negative integers, including 0 (whose digital root is 0), and single-digit numbers (which are their own digital root). Do not use any loops; the solution must be recursive. The function should be named `computeDigitalRoot` and accept an `int` parameter, returning an `int`.
*/

// Compute the digital root of a non-negative integer recursively.
// Digital root: recursively sum digits until a single digit remains.
int computeDigitalRoot(int n) {
    if (n < 10) {
        return n;
    }
    // Sum the digits of n and recursively compute its digital root.
    return computeDigitalRoot(n % 10 + n / 10);
}

int main() {
    // Single-digit numbers return themselves.
    assert(computeDigitalRoot(0) == 0);
    assert(computeDigitalRoot(7) == 7);

    // Two-digit examples.
    assert(computeDigitalRoot(10) == 1);
    assert(computeDigitalRoot(99) == 9); // 9+9=18 -> 1+8=9

    // Larger numbers.
    assert(computeDigitalRoot(942) == 6);  // 9+4+2=15 -> 1+5=6
    assert(computeDigitalRoot(12345) == 6); // 1+2+3+4+5=15 -> 6

    // Maximum int value (2147483647) -> 2+1+4+7+4+8+3+6+4+7=46 -> 4+6=10 -> 1
    assert(computeDigitalRoot(2147483647) == 1);

    // Edge case: number with many 9s.
    assert(computeDigitalRoot(999999999) == 9); // all 9s -> 81 -> 9
}

// The algorithm directly mirrors the definition: if the input `n` is less than 10, it is already a single digit, so return it. Otherwise, sum the digits of `n` using integer arithmetic (`n % 10` extracts the last digit, `n / 10` removes it) and recursively call the function on that sum. This process repeats until the sum becomes a single digit. Edge cases: `0` returns `0` immediately because 0 < 10; any single-digit number (0-9) returns itself; large numbers that may produce a sum with multiple digits are handled by recursion. Time complexity is O(d * log10(n)) where d is the number of recursive calls, but effectively O(log10(n)) because each call reduces the number of digits at least by one (the sum of digits of a k-digit number is at most 9k, which is significantly smaller than n for large n). Space complexity is O(d) due to recursion stack depth, which is also O(log10(n)) in the worst case. No special overflow concerns because the sum of digits of an `int` cannot exceed 9 * 10 = 90 for a 10-digit number (since `INT_MAX` is ~2.1e9, but the sum of its digits is at most 81), well within `int` range.
