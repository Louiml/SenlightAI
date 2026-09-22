// Given two non-negative decimal integer strings `a` and `b` (each may have up to 10^5 digits, no leading zeros except for the value "0"), write a C++ function `std::string addDecimalStrings(const std::string& a, const std::string& b)` that returns a string containing their exact sum as a decimal integer. The function must not use big-integer libraries, must handle carry propagation correctly, and must handle the case where one string is empty or consists only of zeros. The result should have no leading zeros (except for the value "0" itself). This task is derived from a code snippet that currently just reads two strings and prints their digits; your job is to implement the actual addition.
// The classic approach to adding two decimal strings is to process them from least significant digit to most significant digit using two index pointers moving from the end of each string toward the front. Maintain a `carry` variable initialized to 0. At each step, compute the sum of the current digits from `a` and `b` (if indices are valid) plus `carry`. The digit for the result's current position is `sum % 10`, and the new carry is `sum / 10`. After processing all digits, if a carry remains, append it. Then reverse the result because we built it from least significant to most significant. Edge cases: (1) One string may be longer than the other – handle missing digits as 0; (2) Both strings may be "0" – result should be "0"; (3) Carries can propagate multiple positions (e.g., "999" + "1" = "1000"). Time complexity is O(max(n, m)) where n and m are lengths of inputs; space complexity is O(max(n, m)) for the result string. Const-correctness: pass inputs as `const std::string&` and do not modify them.
#include <string>
#include <algorithm>

// Returns the exact decimal sum of two non-negative integer strings.
std::string addDecimalStrings(const std::string& a, const std::string& b) {
    int i = a.size() - 1;
    int j = b.size() - 1;
    int carry = 0;
    std::string result;

    while (i >= 0 || j >= 0 || carry > 0) {
        int digitA = (i >= 0) ? a[i] - '0' : 0;
        int digitB = (j >= 0) ? b[j] - '0' : 0;
        int sum = digitA + digitB + carry;
        result.push_back('0' + (sum % 10));
        carry = sum / 10;
        --i;
        --j;
    }

    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <string>

int main() {
    assert(addDecimalStrings("0", "0") == "0");
    assert(addDecimalStrings("1", "2") == "3");
    assert(addDecimalStrings("999", "1") == "1000");
    assert(addDecimalStrings("12345", "67890") == "80235");
    assert(addDecimalStrings("99999999999999999999", "1") == "100000000000000000000");
    assert(addDecimalStrings("100000000000000000000", "0") == "100000000000000000000");
    assert(addDecimalStrings("123", "999999999999999999999999") == "1000000000000000000000122");
    assert(addDecimalStrings("123456789", "987654321") == "1111111110");
    return 0;
}
