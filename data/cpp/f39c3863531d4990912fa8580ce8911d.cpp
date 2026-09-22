/*
Write a C++ function named `addBinaryStrings` that accepts two non-empty strings representing non-negative binary numbers (containing only characters `'0'` and `'1'`, with no leading zeros except possibly the single string `"0"`) and returns a string representing their exact binary sum, also without unnecessary leading zeros. The function must handle inputs of arbitrary length (up to the limits of available memory) and must not convert the binary strings to integer arithmetic types. The implementation should perform manual column-wise binary addition, accounting for carry propagation across all positions, and must correctly return `"0"` when the sum is zero. The function should be `const`-correct and well-commented.
*/

#include <string>
#include <algorithm>

// Adds two binary strings (without leading zeros) and returns their binary sum.
// Inputs must contain only '0' or '1' and represent non-negative numbers.
std::string addBinaryStrings(const std::string& a, const std::string& b) {
    std::string result;
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    int carry = 0;

    // Process from least significant bit to most significant.
    while (i >= 0 || j >= 0 || carry) {
        int bit_a = (i >= 0) ? a[i] - '0' : 0;
        int bit_b = (j >= 0) ? b[j] - '0' : 0;
        int sum = bit_a + bit_b + carry;
        result.push_back(static_cast<char>((sum % 2) + '0'));
        carry = sum / 2;
        --i;
        --j;
    }

    // The result is currently reversed; reverse it to get normal order.
    std::reverse(result.begin(), result.end());

    // Remove leading zeros, but keep at least one digit.
    size_t first_non_zero = result.find_first_not_of('0');
    if (first_non_zero == std::string::npos) {
        return "0";
    }
    return result.substr(first_non_zero);
}

#include <cassert>
#include <string>

// Include the solution function here or link appropriately.
std::string addBinaryStrings(const std::string& a, const std::string& b);

int main() {
    // Basic cases
    assert(addBinaryStrings("0", "0") == "0");
    assert(addBinaryStrings("0", "1") == "1");
    assert(addBinaryStrings("1", "0") == "1");
    assert(addBinaryStrings("1", "1") == "10");
    assert(addBinaryStrings("10", "11") == "101");
    assert(addBinaryStrings("111", "1") == "1000");
    assert(addBinaryStrings("1010", "1011") == "10101");

    // Different lengths
    assert(addBinaryStrings("1", "1000") == "1001");
    assert(addBinaryStrings("1000", "1") == "1001");

    // Larger numbers and carry overflow
    assert(addBinaryStrings("1111111111", "1") == "10000000000");
    assert(addBinaryStrings("1101", "1011") == "11000");

    // Long equal-length inputs
    assert(addBinaryStrings("10101010101010101010101010101010", "01010101010101010101010101010101") == "111111111111111111111111111111111");

    // All ones: results in leading 1 with many zeros
    assert(addBinaryStrings("1111", "1111") == "11110");

    // Verify that result has no unnecessary leading zeros
    assert(addBinaryStrings("0", "0").front() == '0');
    assert(addBinaryStrings("1", "1")[0] != '0');
    assert(addBinaryStrings("10", "01") == "11");

    return 0;
}

// The main algorithm processes the two binary strings from right to left (least significant bit to most significant bit) using two indices. At each step, sum the corresponding bits plus a carry flag. The carry is set if the sum is 2 or 3 (i.e., ≥2), and the output bit is the sum modulo 2. Continue until both strings are exhausted, then append a final carry if it is 1. After building the result in reverse order, reverse it to get the final binary representation. Leading zeros are removed by trimming all initial `'0'` characters except when the result would become empty — in that case, keep a single `'0'`. Edge cases include one string being longer than the other (the shorter is treated as having implicit leading zeros), both strings being `"0"`, and sums that produce an extra most significant digit (e.g., `"1" + "1" = "10"`). Time complexity is O(max(n, m)) where n and m are the lengths of the inputs, and space complexity is O(max(n, m) + 1) for the result string.
