/*
Write a C++ function `numberToBase` that takes two integers as parameters: a non-negative integer `n` and a base `k` where `2 <= k <= 20`. The function should convert `n` into its representation in base `k` using the digit characters `'0'`-`'9'` for values 0-9 and `'A'`-`'J'` for values 10-19, and return that representation as a `std::string`. If `n` is 0, return the string `"0"`. The function must not read from standard input or write to standard output; it should only return the string. For example, `numberToBase(255, 16)` should return `"FF"`, and `numberToBase(10, 2)` should return `"1010"`.
*/

#include <string>

// Convert a non-negative integer n to its representation in base k (2 <= k <= 20).
// Uses digits '0'-'9' and 'A'-'J' for values 10-19.
std::string numberToBase(int n, int k) {
    const std::string digits = "0123456789ABCDEFGHIJ";
    if (n == 0) {
        return "0";
    }
    std::string result;
    while (n > 0) {
        result = digits[n % k] + result;
        n /= k;
    }
    return result;
}

#include <cassert>
#include <string>

// The function under test is declared here (or included from a header).
std::string numberToBase(int n, int k);

int main() {
    // Basic base-2 conversion
    assert(numberToBase(0, 2) == "0");
    assert(numberToBase(1, 2) == "1");
    assert(numberToBase(10, 2) == "1010");
    assert(numberToBase(255, 2) == "11111111");

    // Base-10 conversion
    assert(numberToBase(0, 10) == "0");
    assert(numberToBase(123, 10) == "123");

    // Hex (base-16) with letters
    assert(numberToBase(255, 16) == "FF");
    assert(numberToBase(16, 16) == "10");
    assert(numberToBase(10, 16) == "A");

    // Base-20 with letters up to J
    assert(numberToBase(19, 20) == "J");
    assert(numberToBase(20, 20) == "10");
    assert(numberToBase(399, 20) == "JJ");

    // Large base-2 number
    assert(numberToBase(1024, 2) == "10000000000");

    return 0;
}

// The algorithm repeatedly extracts the least significant digit of `n` in base `k` by computing `n % k`, mapping that digit value (0 to k-1) to the appropriate character using a lookup table (e.g., a string containing `"0123456789ABCDEFGHIJ"`), and prepending that character to the result string. Then it updates `n` by integer division `n /= k`. This process continues until `n` becomes zero. The edge case `n == 0` is handled by returning `"0"` immediately, because the loop would otherwise produce an empty string. Since `k` is at most 20, the lookup table covers all required digit characters. The algorithm runs in O(log_k(n)) time, because each iteration reduces `n` by a factor of `k`, and uses O(log_k(n)) auxiliary space for the result string (plus O(1) for the lookup table). No special handling is needed for negative `n` because the task specifies non-negative input; if negative were allowed, behavior would be undefined, but here it is not.
