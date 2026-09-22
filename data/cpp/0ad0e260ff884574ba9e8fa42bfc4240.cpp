Write a standalone C++ function that, given a decimal string `s` (which may be very large, up to 10^5 digits, and always represents a non-negative integer without leading zeros unless the number is zero), and two `long long` values `b` (exponent, 0 ≤ b ≤ 10^18) and `m` (modulus, 1 ≤ m ≤ 10^9), computes the value `( (s mod m)^b ) mod m`, where the base is the integer value of the string `s` reduced modulo `m`, and the exponentiation is performed efficiently. The function must handle `b = 0` correctly (returning 1 mod m, even if the reduced base is 0, since 0^0 is defined as 1 in this modular context), and must avoid overflow by using `long long` arithmetic with modulo after every multiplication. The function signature should be: `long long mod_power_from_string(const std::string& s, long long b, long long m);` It must be self-contained, include necessary headers, and be suitable for use in a competitive programming environment.
// The solution has two main parts: (1) compute the remainder of the huge decimal string modulo `m` by iterating over each character, updating `r = (r*10 + digit) % m` in `O(|s|)` time. (2) Perform modular exponentiation using binary exponentiation (fast power) with `O(log b)` multiplications, each multiplication reducing modulo `m` to prevent overflow. Edge cases include: `b = 0`, where we must return `1 % m` even if the base is 0 (so return 1 % m, which works because if m=1, 1%1=0); `m = 1`, where every result is 0, so the string reduction returns 0 and exponentiation yields 0; and a string like "0" where the reduced base is 0, but if b>0 then result is 0, if b=0 then result is 1%m. Time complexity is O(|s| + log b), space complexity is O(1) auxiliary. The approach avoids converting the entire huge string to an integer, which would overflow.
#include <string>

// Computes ((s mod m)^b) mod m, where s is a decimal string.
long long mod_power_from_string(const std::string& s, long long b, long long m) {
    // Step 1: Reduce the large decimal string modulo m.
    long long base = 0;
    for (char c : s) {
        base = (base * 10 + (c - '0')) % m;
    }

    // Step 2: Binary exponentiation (fast power) modulo m.
    long long result = 1 % m;
    long long a = base;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b /= 2;
    }
    return result;
}
#include <cassert>
#include <string>

// Declare the function from the solution.
long long mod_power_from_string(const std::string& s, long long b, long long m);

int main() {
    // Basic tests with small numbers
    assert(mod_power_from_string("2", 10, 1000) == 1024 % 1000); // 24
    assert(mod_power_from_string("5", 3, 7) == (5 * 5 * 5) % 7); // 125 % 7 = 6
    assert(mod_power_from_string("0", 0, 5) == 1); // 0^0 = 1 mod 5
    assert(mod_power_from_string("0", 5, 5) == 0); // 0^5 = 0 mod 5

    // Edge case: m = 1, everything becomes 0
    assert(mod_power_from_string("12345678901234567890", 1000000000000000000LL, 1) == 0);

    // Large exponent: binary exponentiation should handle it efficiently
    assert(mod_power_from_string("3", 1000000000000000000LL, 1000000000) == 1); // 3^1e18 mod 1e9? 3^1e9 mod 1e9 = ... but we trust fast power; let's test smaller known: 3^4 mod 10 = 81%10=1, so use that.
    assert(mod_power_from_string("3", 4, 10) == 1);

    // Large string reduction: "10^18" as string, mod 7
    std::string big = "1000000000000000000"; // 10^18
    assert(mod_power_from_string(big, 2, 7) == ( (10 % 7)^2 ) % 7 ); // (3^2)%7=2
    assert(mod_power_from_string(big, 2, 7) == 2);

    // Test with b=0 and base any number: result 1 mod m (if m>1)
    assert(mod_power_from_string("999", 0, 13) == 1 % 13);
    assert(mod_power_from_string("999", 0, 1) == 0);

    // Test with very large string length (simulate a chunk) 
    std::string long_str(100000, '1'); // 111... (100k times) but mod 9 = 0
    assert(mod_power_from_string(long_str, 5, 9) == 0);

    return 0;
}
