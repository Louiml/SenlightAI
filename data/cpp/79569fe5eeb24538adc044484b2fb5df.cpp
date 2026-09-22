Write a C++ function named `addLargeNumbers` that accepts two non-empty strings `a` and `b`, each consisting only of decimal digits (no leading zeros unless the number itself is "0"), and returns a string representing their decimal sum. The strings can be up to 100,000 digits long, so the result must be computed digit-by-digit without converting the entire numbers to built-in integer types. The function must handle carry propagation correctly, including a final carry that adds an extra leading digit, and must return the result as a string with no leading zeros (unless the sum is exactly "0").

#include <cassert>
#include <string>

// Declaration of the tested function
std::string addLargeNumbers(const std::string& a, const std::string& b);

int main() {
    // Basic cases
    assert(addLargeNumbers("0", "0") == "0");
    assert(addLargeNumbers("1", "2") == "3");
    assert(addLargeNumbers("9", "1") == "10");
    assert(addLargeNumbers("99", "1") == "100");
    assert(addLargeNumbers("123", "456") == "579");
    assert(addLargeNumbers("1", "999") == "1000");
    assert(addLargeNumbers("999", "999") == "1998");

    // Unequal lengths and repeated carries
    assert(addLargeNumbers("785", "9999") == "10784");

    // Large number with many carries
    assert(addLargeNumbers("55555555555555555555", "44444444444444444445") == "100000000000000000000");

    // One number is zero
    assert(addLargeNumbers("0", "12345") == "12345");
    assert(addLargeNumbers("12345", "0") == "12345");

    return 0;
}

#include <string>
#include <algorithm>

// Add two non-negative decimal integers represented as strings of digits.
// The input strings contain only digits and no leading zeros except "0".
// Returns the sum as a string without leading zeros.
std::string addLargeNumbers(const std::string& a, const std::string& b) {
    std::string result;
    result.reserve(std::max(a.size(), b.size()) + 1); // Reserve space for possible final carry

    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    int carry = 0;

    // Process digits from least significant to most significant
    while (i >= 0 || j >= 0 || carry) {
        int digit_sum = carry;
        if (i >= 0) {
            digit_sum += a[i] - '0';
            --i;
        }
        if (j >= 0) {
            digit_sum += b[j] - '0';
            --j;
        }
        carry = digit_sum / 10;
        result.push_back(static_cast<char>('0' + (digit_sum % 10)));
    }

    // Reverse to get the correct order
    std::reverse(result.begin(), result.end());
    return result;
}

// The problem is a classic big-integer addition. The main algorithm processes the digits from the least significant (rightmost) position to the most significant, aligning the two numbers by their right ends. For each position, we add the corresponding digits from both strings (if they exist) plus a carry from the previous position, compute the new digit as the sum modulo 10, and update the carry as the integer division of the sum by 10. After processing all digits of both strings, if a carry remains, append it as the most significant digit of the result. Since we build the result from right to left, we either insert at the beginning (which is O(n) per insertion, leading to O(n²) overall) or, better, build a temporary vector/string in reverse order and then reverse it once at the end, yielding O(n) time. Edge cases include numbers of unequal lengths (iterate until both are exhausted), a carry that propagates beyond the longer number, and the case where both inputs are "0" (the result "0" must be returned without an extra leading zero). The time complexity is O(max(len(a), len(b))) and the auxiliary space is O(max(len(a), len(b))) for the result string.
