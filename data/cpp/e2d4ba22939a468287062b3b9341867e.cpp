Write a C++ function named `minPartitions` that takes a non-empty string `n` representing a positive decimal integer (with no leading zeros unless the number itself is "0") and returns the smallest number of "deci-binary" numbers (numbers composed only of digits `0` and `1`) that sum up to `n`. For example, `n = "32"` can be expressed as `11 + 11 + 10 = 32`, requiring 3 numbers, while `n = "82734"` requires 8 numbers. The function must handle any length of input string (up to a very large size) and must not perform any conversion to a numeric type (since the number may exceed typical 64-bit limits). The input string contains only digits '0'–'9', and the output must be an integer `int`. Provide an efficient solution.
// The key observation is that when adding `k` deci-binary numbers, the digit at any position in the result is at most `k` (since each deci-binary contributes at most 1 to that position). Therefore, for the sum to produce a digit `d` at a given position, we need at least `d` deci-binary numbers. Thus, the minimum number of deci-binary numbers required equals the maximum digit present in the input string. For example, for `n = "321"`, the maximum digit is 3, and indeed we can use 3 numbers `111`, `111`, `110` to sum to `321`. The algorithm simply iterates over each character in the string, converts it to an integer by subtracting `'0'`, and tracks the maximum value seen. Edge cases: the string may contain only `'0'` (then max is 0, and zero deci-binary numbers sum to 0, but the problem likely expects at least 1? Actually 0 deci-binary numbers sum to 0, but typically you'd return 0 for "0" since no positive sum is needed; but the problem statement says "positive integer", so "0" may not appear. If it does, return 0). For any other digit, the max digit gives the answer. Time complexity is O(L) where L is the length of the string, and space complexity is O(1) auxiliary.
#include <string>
#include <algorithm>

// Returns the minimum number of deci-binary numbers (digits 0 and 1 only)
// needed to sum to the positive integer represented by the string n.
// The answer is the maximum digit in n.
int minPartitions(const std::string& n) {
    int maxDigit = 0;
    for (char c : n) {
        maxDigit = std::max(maxDigit, c - '0');
    }
    return maxDigit;
}
#include <cassert>
#include <string>

int minPartitions(const std::string& n);

int main() {
    assert(minPartitions("32") == 3);
    assert(minPartitions("82734") == 8);
    assert(minPartitions("1") == 1);
    assert(minPartitions("0") == 0);
    assert(minPartitions("99999999999999999999999999999") == 9);
    assert(minPartitions("10") == 1);
    assert(minPartitions("1234567890") == 9);
    assert(minPartitions("27346209830709182346") == 9);
    assert(minPartitions("101") == 1);
    assert(minPartitions("550") == 5);
    return 0;
}
