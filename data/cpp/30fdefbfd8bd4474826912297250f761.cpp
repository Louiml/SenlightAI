/*
Write a C++ function `long long digitAtPosition(long long k)` that accepts a positive integer `k` and returns the `k`-th digit in the infinite concatenated sequence of all positive integers (starting from 1): `"123456789101112131415161718192021..."`. For example, the 1st digit is `1`, the 10th digit is `1` (the first digit of `10`), and the 11th digit is `0`. The input `k` can be as large as `10^18`, so the function must avoid overflowing and run efficiently. The function should handle the fact that there are 9 single‑digit numbers, 90 two‑digit numbers, 900 three‑digit numbers, and so on.
*/
#include <cstdint>

// Binary exponentiation for non‑negative integer powers.
long long ipow(long long base, long long exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

// Return the k‑th digit (1‑based) in the concatenation of all positive integers.
long long digitAtPosition(long long k) {
    long long digits = 1;       // current number of digits per number
    long long count = 9;        // how many numbers have exactly 'digits' digits

    // Find the block where the k‑th digit resides.
    while (k > digits * count) {
        k -= digits * count;
        ++digits;
        count *= 10;
    }

    // 1‑based position within the current block.
    long long positionInBlock = k;
    // The number containing the digit (1‑based offset among numbers in block).
    long long number = ipow(10, digits - 1) + (positionInBlock - 1) / digits;
    // Which digit (0‑based) inside that number.
    long long digitIndex = (positionInBlock - 1) % digits;

    // Extract that specific digit.
    long long divisor = ipow(10, digits - digitIndex - 1);
    return (number / divisor) % 10;
}
#include <cassert>

int main() {
    // Single‑digit block: positions 1..9
    assert(digitAtPosition(1) == 1);
    assert(digitAtPosition(5) == 5);
    assert(digitAtPosition(9) == 9);

    // Start of two‑digit numbers: position 10 is first digit of "10"
    assert(digitAtPosition(10) == 1);
    assert(digitAtPosition(11) == 0);
    // 13th digit is '1' from "11", 14th is '1'
    assert(digitAtPosition(13) == 1);
    assert(digitAtPosition(14) == 1);

    // Last two‑digit digit at position 9 + 90*2 = 189
    // 189th digit is last digit of "99" → 9
    assert(digitAtPosition(189) == 9);
    // 190th digit is first digit of "100" → 1
    assert(digitAtPosition(190) == 1);
    // 192nd digit is first '0' of "100" → 0
    assert(digitAtPosition(192) == 0);

    // Three‑digit block start: 190..2889
    // 193rd digit: "101" -> digits: 1,0,1 → 193 → second digit '0'
    assert(digitAtPosition(193) == 0);
    // 194th digit: third digit of "101" → 1
    assert(digitAtPosition(194) == 1);

    // Large value: k = 10^12 (within 18‑digit numbers)
    // Verify this does not crash and returns a plausible digit (0..9)
    long long result = digitAtPosition(1000000000000LL);
    assert(result >= 0 && result <= 9);
}
// The key is to first determine which "block" of numbers (by digit length) contains the `k`-th digit. We maintain `digits = 1` (current number of digits per number) and `base = 9` (count of numbers with that many digits). While `k > digits * base`, we subtract `digits * base` from `k` and move to the next block (`digits++`, `base *= 10`). After this loop, `k` is the 1‑based index within the current block.  
//
// Let `index = k % digits` (0‑based digit position inside the target number). The target number is `start + (k - 1) / digits`, where `start = 10^(digits-1)`. If `index == 0`, the digit is the last digit of that number; otherwise we divide out the extra leading digits and take the last digit. Using binary exponentiation for powers of 10 avoids overflow (though values can be up to about `10^18`). Special care: when `digits` can be large, the power function must be done with `long long` safely, but since the number of digits won't exceed 18, it's fine.  
//
// Edge cases: `k = 1` gives `1`; `k` exactly at a block boundary (e.g., `k = 9` → last single‑digit `9`; `k = 10` → first digit of `10` which is `1`). The algorithm runs in `O(log k)` time (number of digit blocks is at most 18) and `O(1)` auxiliary space.
