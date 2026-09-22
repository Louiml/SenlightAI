Write a C++ function that takes three parameters: two strings `dividend` and `op` (which is either `"/"` or `"%"`), and a non-zero integer `divisor`. The function must perform an arbitrary-precision integer division or modulo operation on a very large non-negative integer represented as a decimal string (without leading zeros, except possibly the single digit `0`). If `op == "/"`, return the quotient as a string without leading zeros (or the single character `"0"` if the quotient is zero). If `op == "%"`, return the remainder as an integer (using `long long`). The returned quotient must be computed digit-by-digit using long division, and must handle cases where the dividend is smaller than the divisor, the dividend is exactly divisible, or the quotient itself contains internal zeros.

The solution simulates the standard long-division algorithm on a digit-by-digit basis. Maintain a running remainder `current` initialized to 0. For each character `c` in the dividend string, compute `current = current * 10 + (c - '0')`. Then, for the quotient string, append the digit `(current / divisor)` (as a character) in each iteration. After appending, update the remainder as `current %= divisor`. At the end of the loop, the `current` variable holds the final remainder. For division, the accumulated quotient string may contain leading zeros (e.g., when dividend is smaller than divisor); strip all leading zeros by finding the first non-zero character and building a new string. If the resulting string is empty (i.e., all digits were zeros), return `"0"`. For modulo, directly return `current` (cast to `long long`). Edge cases: divisor is always non-zero per constraints; dividend may be `"0"`, in which case the quotient string is all zeros and the remainder is 0. If the dividend is shorter than the divisor, every quotient digit will be 0, so the answer for division is `"0"`. Complexity: Let `n` be the length of the dividend string; time is O(n) for a single pass and O(n) for building the stripped quotient string, so O(n) total. Space is O(n) to store the intermediate quotient string and the final stripped answer.

#include <string>
#include <cstdint>

// Perform arbitrary-precision division or modulo on a decimal string dividend.
// op: "/" for quotient, "%" for remainder. divisor must be non-zero.
std::string longDivision(const std::string& dividend, const std::string& op, long long divisor) {
    std::int64_t current = 0;
    std::string quotient;

    for (char c : dividend) {
        current = current * 10 + (c - '0');
        quotient.push_back(static_cast<char>('0' + (current / divisor)));
        current %= divisor;
    }

    if (op == "%") {
        return std::to_string(current);
    }

    // Strip leading zeros for division result.
    std::size_t start = quotient.find_first_not_of('0');
    if (start == std::string::npos) {
        return "0";
    }
    return quotient.substr(start);
}

#include <cassert>
#include <string>

// (The solution function is assumed to be defined above.)

int main() {
    // Simple cases
    assert(longDivision("10", "/", 2) == "5");
    assert(longDivision("10", "%", 2) == "0");

    // Dividend smaller than divisor
    assert(longDivision("5", "/", 10) == "0");
    assert(longDivision("5", "%", 10) == "5");

    // Large dividend with many digits
    assert(longDivision("12345678901234567890", "/", 1) == "12345678901234567890");
    assert(longDivision("12345678901234567890", "%", 1) == "0");

    // Internal zeros in quotient
    assert(longDivision("1000000000000000000000", "/", 3) == "333333333333333333333");
    assert(longDivision("1000000000000000000000", "%", 3) == "1");

    // Exact multiples with large divisor
    assert(longDivision("9999999999", "/", 9999999999) == "1");
    assert(longDivision("9999999999", "%", 9999999999) == "0");

    // Dividend is zero
    assert(longDivision("0", "/", 7) == "0");
    assert(longDivision("0", "%", 7) == "0");

    // Remainder larger than usual when quotient is zero
    assert(longDivision("7", "%", 5) == "2");
    assert(longDivision("7", "/", 5) == "1");

    // Ensure remainder returned as string of integer (not truncated)
    assert(longDivision("2147483647", "%", 100003) == "114873");
    assert(longDivision("2147483647", "/", 100003) == "21474");
}
