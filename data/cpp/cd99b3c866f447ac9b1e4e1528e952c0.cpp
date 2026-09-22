// Write a C++ function `addBinary` that takes two non-empty strings, `a` and `b`, each containing only the characters `'0'` and `'1'` (representing binary numbers without leading zeros, except for the number zero itself), and returns a string representing their sum in binary. The function must handle arbitrarily long input strings (up to tens of thousands of digits) and must not convert the entire input to an integer type (e.g., `int` or `long long`), as that would overflow. The result must also have no leading zeros, except for the single digit `"0"` when the sum is zero. For example, `addBinary("101", "11")` should return `"1000"`.
The solution processes the two binary strings from the least significant bit (rightmost) to the most significant bit (leftmost). Since the inputs may have different lengths, the algorithm first aligns them by padding the shorter string with leading zeros (or equivalently, by starting from the end of each string and tracking indices). A simpler approach is to iterate from the end of both strings, adding corresponding bits plus a carry (which is 0 or 1). For each position, compute `bit_a` and `bit_b` (0 or 1 based on whether the index is valid and the character is `'1'`), then compute `sum = bit_a + bit_b + carry`. The result bit is `sum % 2` (appended to a result string that is built in reverse), and the new carry is `sum / 2`. After processing all digits, if a carry remains, append `'1'` to the result. Finally, reverse the result string to get the correct order. Edge cases include: one string being much longer than the other (handled by treating out-of-range indices as 0), both strings being `"0"` (result is `"0"`), and the final carry producing an extra leading digit (e.g., `"1"` + `"1"` -> `"10"`). Time complexity is O(max(n, m)) where n and m are the lengths of the inputs, and space complexity is O(max(n, m)) for the result string (plus O(1) extra for variables). The solution uses only character comparisons and integer arithmetic on digits, so it is safe for arbitrarily long inputs.
#include <string>
#include <algorithm>

// Add two binary strings and return the sum as a binary string.
// Precondition: a and b contain only '0' and '1'; no leading zeros except "0".
std::string addBinary(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0;
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;

    while (i >= 0 || j >= 0 || carry) {
        int bit_a = (i >= 0) ? a[i] - '0' : 0;
        int bit_b = (j >= 0) ? b[j] - '0' : 0;
        int sum = bit_a + bit_b + carry;
        result.push_back(static_cast<char>('0' + (sum % 2)));
        carry = sum / 2;
        --i;
        --j;
    }

    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(addBinary("0", "0") == "0");
    assert(addBinary("1", "0") == "1");
    assert(addBinary("0", "1") == "1");
    assert(addBinary("1", "1") == "10");

    // Different lengths
    assert(addBinary("101", "11") == "1000");
    assert(addBinary("110", "101") == "1011");

    // Carry propagation across multiple bits
    assert(addBinary("111", "1") == "1000");
    assert(addBinary("1111", "1111") == "11110");

    // Long string (no overflow)
    std::string long_a(10000, '1');
    std::string long_b("1");
    std::string result = addBinary(long_a, long_b);
    assert(result.size() == 10001);
    assert(result[0] == '1');
    for (size_t idx = 1; idx < result.size(); ++idx) {
        assert(result[idx] == '0');
    }

    // Uneven lengths with no overlap
    assert(addBinary("1000", "1") == "1001");
    assert(addBinary("1", "1000") == "1001");
}
