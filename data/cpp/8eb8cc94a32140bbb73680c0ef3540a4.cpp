Write a C++ function named `decodeSequence` that takes a single positive integer `n` (where `1 <= n <= 10^18`) and returns a string representing the sequence of operations needed to reduce `n` to `0` using this rule: if the current number is even, divide it by 2 and append `'B'` to the front of the result; if the current number is odd, subtract 1 and append `'A'` to the front of the result. Repeat until the number becomes 0. For example, for `n = 5` (odd) → subtract 1 → `n = 4` (append 'A'), then even → divide → `n = 2` (append 'B'), even → divide → `n = 1` (append 'B'), odd → subtract → `n = 0` (append 'A'), so the result is `"ABBA"`. The function must handle large numbers without overflow by using an appropriate unsigned integer type.

#include <cassert>
#include <string>

// Function declaration from solution
std::string decodeSequence(uint64_t n);

int main() {
    assert(decodeSequence(1) == "A");
    assert(decodeSequence(2) == "B");
    assert(decodeSequence(3) == "BA");
    assert(decodeSequence(4) == "BB");
    assert(decodeSequence(5) == "ABBA");
    assert(decodeSequence(6) == "ABBB");
    assert(decodeSequence(7) == "BBBA");
    assert(decodeSequence(10) == "BABBA");
    assert(decodeSequence(100) == "BBBBABBA");
    assert(decodeSequence(1000000000000000000ULL) == "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBA");
    return 0;
}

#include <string>
#include <cstdint>

// Returns the sequence of operations to reduce n to 0.
// For even n: divide by 2 and prepend 'B'.
// For odd n: subtract 1 and prepend 'A'.
std::string decodeSequence(uint64_t n) {
    std::string result;
    while (0 < n) {
        if (n % 2 == 0) {
            n /= 2;
            result = 'B' + result;
        } else {
            --n;
            result = 'A' + result;
        }
    }
    return result;
}

// The problem is the inverse of a known binary encoding technique: starting from `n`, repeatedly apply the transformation until reaching 0. Each step produces one character: if `n` is even, we divide by 2 and prepend `'B'`; if `n` is odd, we subtract 1 and prepend `'A'`. This works because the operation is deterministic: for even `n`, the last operation was a division (since if we subtracted 1 from an even number we'd get an odd, but we only subtract from odd numbers), and for odd `n`, the last operation was a subtraction (since dividing an odd number by 2 is not integer-exact). The algorithm builds the result string from the back to the front, prepending each character, which matches the required order. Edge cases: `n = 0` is not a valid input per the constraint, but if it were, the loop would not execute and an empty string would be returned; we can handle it defensively. Since `n` is up to `10^18`, using `unsigned long long` ensures no overflow. The number of iterations is the number of characters in the output, which is at most about 60 (since each step reduces `n` roughly by half or by 1, and the worst case for numbers like `2^k-1` yields about `2k` steps), so time complexity is O(log n) in terms of steps, with each step being O(1). Space complexity is O(log n) for storing the result string.
