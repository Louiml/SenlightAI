// Given a large integer represented as a string (possibly with a leading sign, without leading zeros except for "0" itself), write a C++ function `std::string addLargeNumbers(const std::string& a, const std::string& b)` that returns the sum as a string. The numbers may be positive or negative, and the result must not have leading zeros (except for the single digit "0"). Do not use any existing big‑integer library; implement the arithmetic manually. Assume the input strings are valid and non-empty, with maximum length up to 10^5 digits. Handle the sign correctly, including cases where the result is zero (return "0") and where the absolute values are identical but opposite signs.
// The solution processes the two numbers digit by digit from the least significant digit to the most significant, similar to traditional hand addition, but must account for signs. The main idea is to use the sign of each number to decide whether to add or subtract their absolute values.
//
// - **Case both same sign:** Add their absolute values digit by digit with carry. If both negative, the result is negative; otherwise positive.
// - **Case different signs:** Effectively subtract the smaller absolute value from the larger absolute value. To avoid negative intermediate results in subtraction, determine which absolute value is larger (by length first, then lexicographic comparison). Subtract the smaller from the larger, handling borrowing. The sign of the result is the sign of the number with the larger absolute value.
// - **Edge cases:**
//   - If the result of subtraction is all zeros, return "0" with no sign.
//   - If one number is "0", return the other number (with its sign).
//   - Leading zeros in the result must be removed, but if the result is zero, keep a single "0".
//   - Avoid using `std::stoll` or any conversion to built‑in integer types; work purely on strings to support large inputs.
//
// **Time complexity:** Both addition and subtraction process each digit exactly once, so O(n) time, where n is the maximum length of the two input strings (including their signs). **Space complexity:** O(n) for the result string.
#include <string>
#include <algorithm>

// Helper: compare absolute values as strings without leading zeros.
// Returns 1 if abs(a) > abs(b), 0 if equal, -1 if abs(a) < abs(b).
int compareAbs(const std::string& a, const std::string& b) {
    // Remove leading zeros from absolute value representation for fair comparison.
    auto trim = [](const std::string& s) {
        size_t pos = s.find_first_not_of('0');
        if (pos == std::string::npos) return std::string("0");
        return s.substr(pos);
    };
    std::string ta = trim(a);
    std::string tb = trim(b);
    if (ta.size() != tb.size()) return ta.size() > tb.size() ? 1 : -1;
    if (ta == tb) return 0;
    return (ta > tb) ? 1 : -1;
}

// Add two non‑negative digit strings (no sign, no leading zeros except "0").
std::string addUnsigned(const std::string& a, const std::string& b) {
    int i = a.size() - 1, j = b.size() - 1;
    int carry = 0;
    std::string result;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result.push_back('0' + (sum % 10));
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// Subtract b from a, where a >= b (both non‑negative digit strings).
std::string subtractUnsigned(const std::string& a, const std::string& b) {
    int i = a.size() - 1, j = b.size() - 1;
    int borrow = 0;
    std::string result;
    while (i >= 0) {
        int diff = (a[i] - '0') - borrow - (j >= 0 ? (b[j] - '0') : 0);
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back('0' + diff);
        --i; if (j >= 0) --j;
    }
    std::reverse(result.begin(), result.end());
    // Remove leading zeros, but keep at least one digit.
    size_t first_nonzero = result.find_first_not_of('0');
    if (first_nonzero == std::string::npos) return "0";
    return result.substr(first_nonzero);
}

// Main function: add two large integers represented as strings with optional sign.
std::string addLargeNumbers(const std::string& a, const std::string& b) {
    // Extract signs and absolute value strings.
    bool negA = (!a.empty() && a[0] == '-');
    bool negB = (!b.empty() && b[0] == '-');
    std::string absA = (negA || (!a.empty() && a[0] == '+')) ? a.substr(1) : a;
    std::string absB = (negB || (!b.empty() && b[0] == '+')) ? b.substr(1) : b;

    // Handle trivial "0" inputs.
    auto isZero = [](const std::string& s) {
        return s.find_first_not_of('0') == std::string::npos;
    };
    if (isZero(absA)) return (negB ? "-" : "") + absB;
    if (isZero(absB)) return (negA ? "-" : "") + absA;

    // Case 1: same sign -> add absolute values.
    if (negA == negB) {
        std::string sum = addUnsigned(absA, absB);
        return (negA && !isZero(sum)) ? "-" + sum : sum;
    }

    // Case 2: different signs -> subtract smaller absolute value from larger.
    int cmp = compareAbs(absA, absB);
    if (cmp == 0) return "0";
    if (cmp > 0) {
        // |a| > |b|
        std::string diff = subtractUnsigned(absA, absB);
        return (negA ? "-" : "") + diff;
    } else {
        // |b| > |a|
        std::string diff = subtractUnsigned(absB, absA);
        return (negB ? "-" : "") + diff;
    }
}
#include <cassert>
#include <string>

// The solution function is declared above; include its definition.

int main() {
    // Same positive sign
    assert(addLargeNumbers("123", "456") == "579");
    // Same negative sign
    assert(addLargeNumbers("-123", "-456") == "-579");
    // Different signs, positive larger
    assert(addLargeNumbers("1000", "-999") == "1");
    // Different signs, negative larger
    assert(addLargeNumbers("-1000", "999") == "-1");
    // Result zero
    assert(addLargeNumbers("123", "-123") == "0");
    assert(addLargeNumbers("-123", "123") == "0");
    // Leading zeros in input
    assert(addLargeNumbers("007", "0008") == "15");
    // Large numbers (simulate by string length)
    std::string big1(100000, '9'); // 999...9
    std::string big2(100000, '1'); // 111...1
    std::string big_result = addLargeNumbers(big1, big2);
    // First digit should be '1' then 100000 '1's? Actually 999+111 = 1110, pattern check:
    assert(big_result.size() == 100001);
    assert(big_result.front() == '1');
    // Add with zero
    assert(addLargeNumbers("0", "42") == "42");
    assert(addLargeNumbers("-0", "0") == "0");
    // Single digit negative
    assert(addLargeNumbers("-5", "3") == "-2");
    return 0;
}
