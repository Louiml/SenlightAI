// Write a C++ function named `multiplyLargeNumbers` that takes two strings, each representing a non-negative integer (possibly with leading zeros), and returns a string representing their exact product. The input strings can be arbitrarily long (up to tens of thousands of digits), so the result must be computed without using built-in arbitrary-precision arithmetic or converting the full numbers to native integer types. Leading zeros in the output should be removed (e.g., "000" becomes "0"), and the function must handle the case where either operand is "0" or contains only zeros. The function should be `const`-correct and use only standard library facilities.

The core algorithm is grade-school multiplication using digit-by-digit multiplication with base-10 arithmetic, but implemented via string operations to avoid overflow. A naive approach that repeatedly adds one operand a digit times (like the snippet) is too slow for large inputs. A more efficient approach: multiply each digit of `num2` (the multiplier) with the entire `num1` (the multiplicand), producing partial products that are shifted left by the position of the multiplier digit, then summed together using a simple string addition function. To avoid `O(n^2)` string concatenations and repeated allocations, we can use a reverse storage of digits in vectors and perform multiplication in one pass, then propagate carries. Specifically, for two numbers of lengths `N` and `M`, create a result vector of size `N+M` initialized to zero. For each digit `i` in `num1` (from rightmost) and digit `j` in `num2` (from rightmost), add `(num1[i]-'0') * (num2[j]-'0')` to `result[i+j]`. After accumulating all products, propagate carries from the least significant digit to the most significant, converting each digit to a character. Finally, strip leading zeros and return "0" if the result is all zeros. Time complexity is `O(N*M)` (where `N` and `M` are the digit lengths of the inputs), which is optimal for grade-school multiplication. Space complexity is `O(N+M)` for the result vector plus the returned string. Edge cases: (1) If either input is "0" or consists solely of zeros, return "0" immediately. (2) Leading zeros in inputs should not affect correctness—they are handled naturally because they contribute zero to products. (3) The carry propagation must handle carries that can exceed 9; the maximum product at any position is `9*9 + carry`, so `carry` always fits in an `int`. (4) After the final carry propagation, if the most significant position has a carry, it is simply placed at the front.

#include <string>
#include <vector>
#include <algorithm>

// Multiply two non-negative integers represented as decimal digit strings.
// Returns the exact product as a string, without leading zeros (except for "0").
std::string multiplyLargeNumbers(const std::string& num1, const std::string& num2) {
    // Handle trivial zero cases to avoid unnecessary work.
    if (num1.empty() || num2.empty()) return "0";
    // Check if either number is zero (including strings like "0000").
    bool num1_zero = true, num2_zero = true;
    for (char ch : num1) if (ch != '0') { num1_zero = false; break; }
    for (char ch : num2) if (ch != '0') { num2_zero = false; break; }
    if (num1_zero || num2_zero) return "0";

    const int len1 = static_cast<int>(num1.size());
    const int len2 = static_cast<int>(num2.size());

    // Result vector length: len1 + len2. Index i+j accumulates products.
    std::vector<int> result(len1 + len2, 0);

    // Multiply each pair of digits, storing intermediate products without carry.
    for (int i = len1 - 1; i >= 0; --i) {
        int digit1 = num1[i] - '0';
        for (int j = len2 - 1; j >= 0; --j) {
            int digit2 = num2[j] - '0';
            result[i + j] += digit1 * digit2;
        }
    }

    // Propagate carries from least significant (right end) to most significant.
    int carry = 0;
    for (int i = 0; i < len1 + len2; ++i) {
        int sum = result[i] + carry;
        result[i] = sum % 10;
        carry = sum / 10;
    }

    // Build the output string by converting digits to characters in reverse order.
    std::string output;
    output.reserve(len1 + len2);
    // The most significant digit is at the end of the vector (after carry propagation).
    // Find the first non-zero digit from the end.
    int start = len1 + len2 - 1;
    while (start >= 0 && result[start] == 0) {
        --start;
    }
    // If all digits are zero (shouldn't happen after zero check, but safe).
    if (start < 0) {
        return "0";
    }
    for (int i = start; i >= 0; --i) {
        output.push_back(static_cast<char>('0' + result[i]));
    }
    return output;
}

#include <cassert>
#include <string>
#include <iostream>

int main() {
    // Basic tests.
    assert(multiplyLargeNumbers("0", "123") == "0");
    assert(multiplyLargeNumbers("000", "123") == "0");
    assert(multiplyLargeNumbers("123", "0") == "0");
    assert(multiplyLargeNumbers("1", "1") == "1");
    assert(multiplyLargeNumbers("2", "3") == "6");
    assert(multiplyLargeNumbers("10", "10") == "100");
    assert(multiplyLargeNumbers("999", "999") == "998001");
    assert(multiplyLargeNumbers("123456789", "987654321") == "121932631112635269");
    // Large numbers with many digits and no overflow issues.
    std::string big1 = "9369162965141127216164882458728854782080715827760307787224298083754";
    std::string big2 = "71103396869999767678843";
    std::string expected = "666152372921775693236164201176254349832257661209118619436733449870996163741440380327622";
    assert(multiplyLargeNumbers(big1, big2) == expected);
    // Leading zeros in inputs should be handled.
    assert(multiplyLargeNumbers("00123", "0456") == "56088");
    assert(multiplyLargeNumbers("000", "000") == "0");
    // Multiplication by 1 and by large powers of 10.
    assert(multiplyLargeNumbers("1", "100000000000000000000") == "100000000000000000000");
    assert(multiplyLargeNumbers("12345", "100000") == "1234500000");
    // Symmetry and correctness with reversed inputs.
    assert(multiplyLargeNumbers("987", "123") == multiplyLargeNumbers("123", "987"));
    // Final output check for the provided example (confirming correctness).
    assert(multiplyLargeNumbers("999", "1") == "999");
    std::cout << "All tests passed.\n";
    return 0;
}
