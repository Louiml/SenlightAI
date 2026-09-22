Write a C++ function `long long sumOfAllInsertedPlusValues(const std::string& s)` that, given a string `s` consisting only of digits `'0'` through `'9'` (no leading zeros except for the single digit `"0"`), computes the sum of all possible numeric values obtained by inserting any number of `+` signs between digits (including zero plus signs, i.e., the whole string as one number). For example, for input `"123"`, the possible expressions are `"123"`, `"1+23"`, `"12+3"`, and `"1+2+3"`, producing values 123, 24, 15, and 6, whose sum is 168. The function must return that total sum as a `long long` (the result can exceed 32-bit range). The input string length will be at most 10, so brute force is acceptable, but you must implement a recursive or iterative solution that avoids overflow and handles the whole string correctly. Your function should not print anything; it should return the computed value.

#include <cassert>

int main() {
    // Single digit: only one way, the number itself.
    assert(sumOfAllInsertedPlusValues("5") == 5);
    // Two digits: "12" and "1+2" -> 12 + 3 = 15.
    assert(sumOfAllInsertedPlusValues("12") == 15);
    // Classic "123": 123 + 24 + 15 + 6 = 168.
    assert(sumOfAllInsertedPlusValues("123") == 168);
    // All zeros: every split yields zero.
    assert(sumOfAllInsertedPlusValues("000") == 0);
    // "10": "10" and "1+0" -> 10 + 1 = 11.
    assert(sumOfAllInsertedPlusValues("10") == 11);
    // "99": "99" and "9+9" -> 99 + 18 = 117.
    assert(sumOfAllInsertedPlusValues("99") == 117);
    // "1234": verify manually:
    // 1234 + 1+234(235) + 12+34(46) + 123+4(127) + 1+2+34(37) + 1+23+4(28) + 12+3+4(19) + 1+2+3+4(10) = 1736
    assert(sumOfAllInsertedPlusValues("1234") == 1736);
    // Longest allowed: 10 digits, should not overflow.
    assert(sumOfAllInsertedPlusValues("1111111111") > 0);
    return 0;
}

#include <string>
#include <cstdint>

// Helper recursive function that adds all possible sums to 'result'.
static void dfs(const std::string& s, int pos, long long currentSum, long long& result) {
    int length = static_cast<int>(s.size());
    if (pos == length) {
        result += currentSum;
        return;
    }
    for (int i = pos + 1; i <= length; ++i) {
        long long num = std::stoll(s.substr(pos, i - pos));
        dfs(s, i, currentSum + num, result);
    }
}

// Returns the sum of all values obtained by inserting any number of '+' signs.
long long sumOfAllInsertedPlusValues(const std::string& s) {
    long long result = 0;
    dfs(s, 0, 0, result);
    return result;
}

// The problem is a classic enumeration of all ways to split a digit string into contiguous substrings separated by `+`. The key is to recursively place a `+` after each possible position. At each step, we maintain the current position index `pos` and the accumulated sum so far. At any position, we try every possible next split point `i` from `pos+1` to the string length. The substring from `pos` to `i-1` is converted to an integer using `std::stoll` (or manual parsing), added to the current sum, and then we recurse with `i` as the new position. When `pos` equals the string length, we add the accumulated sum to a global or reference result. This is exactly the logic in the provided snippet, but we restructure it into a clean free function with a helper that accumulates the result via a reference. Edge cases: the string may be a single digit (only one split: the whole number), or all digits may be `'0'` (multiple splits all produce zero). Since the maximum length is 10, the number of splits is at most \(2^{9} = 512\), so time complexity is \(O(2^{n} \cdot n)\) due to substring conversions, which is tiny. Space complexity is \(O(n)\) for recursion depth and substring storage. We must use `long long` to avoid overflow because for a string like `"9999999999"` (10 digits), the whole number is ~10^10, and summing all combinations could be much larger, but still within 64-bit range (worst-case sum is around \(n \cdot 10^n\) which for n=10 is ~10^11, safe).
