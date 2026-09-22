// Write a C++ function `string addLargeNumbers(const string& num1, const string& num2)` that takes two non-negative integer strings (each containing only digits '0'–'9', with no leading zeros except for the string "0" itself) and returns their sum as a string. The numbers can be extremely large (up to 10^4 digits), so you must not convert them to built‑in integer types. Use recursion to process the digits from right to left, handling carries, and produce the final sum in correct order.
#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(addLargeNumbers("0", "0") == "0");
    assert(addLargeNumbers("1", "2") == "3");
    assert(addLargeNumbers("5", "5") == "10");

    // Different lengths
    assert(addLargeNumbers("999", "1") == "1000");
    assert(addLargeNumbers("1", "999") == "1000");
    assert(addLargeNumbers("123", "4567") == "4690");

    // Large numbers with many digits
    assert(addLargeNumbers("11111111111111111111", "22222222222222222222") == "33333333333333333333");
    assert(addLargeNumbers("99999999999999999999", "1") == "100000000000000000000");

    // Both same length with multiple carries
    assert(addLargeNumbers("999", "999") == "1998");

    // One number is zero
    assert(addLargeNumbers("123456", "0") == "123456");
    assert(addLargeNumbers("0", "987654") == "987654");

    // More random checks
    assert(addLargeNumbers("1000", "1000") == "2000");
    assert(addLargeNumbers("123456789", "987654321") == "1111111110");

    return 0;
}
#include <string>
#include <algorithm>

// Recursive helper that processes digits from right to left.
// index is the current position (0 = least significant), carry holds the current carry.
void addDigits(const std::string& num1, const std::string& num2, int index, int carry, std::string& ans) {
    int len1 = num1.size();
    int len2 = num2.size();

    // Base case: both strings fully processed and no carry remains.
    if (index >= len1 && index >= len2 && carry == 0) {
        return;
    }

    // Extract digit at current position if available, else use 0.
    int digit1 = (index < len1) ? num1[len1 - 1 - index] - '0' : 0;
    int digit2 = (index < len2) ? num2[len2 - 1 - index] - '0' : 0;

    int sum = digit1 + digit2 + carry;
    carry = sum / 10;
    ans.push_back(static_cast<char>('0' + (sum % 10)));

    addDigits(num1, num2, index + 1, carry, ans);
}

// Sums two non-negative integer strings without converting to built-in types.
std::string addLargeNumbers(const std::string& num1, const std::string& num2) {
    std::string ans;
    addDigits(num1, num2, 0, 0, ans);
    std::reverse(ans.begin(), ans.end());
    return ans.empty() ? "0" : ans;
}
// The algorithm processes the two strings from their least significant (rightmost) digits toward the most significant. A recursive helper function takes a current index `i` (starting at 0), a carry value (initially 0), and a reference to an answer string that will store the result digits in reverse order. For each recursion level, if both strings are exhausted and the carry is zero, the recursion stops. Otherwise, it extracts the digit from each string if the index is within bounds (else uses 0), computes `digit = (digit1 + digit2 + carry)`, sets the new carry as `digit / 10`, and appends `digit % 10` (as a character) to the answer string. After the recursion completes, the answer string is reversed to obtain the correct most‑significant‑first representation. Edge cases include one string being empty (though the problem guarantees non‑empty), different lengths (handled by the bounds check), and a final carry (e.g., "999" + "1" yields "1000"), which the recursion handles because the base case only stops when carry is zero. Time complexity is O(n) where n is the maximum length of the two strings, because each digit is visited once. Space complexity is O(n) for the recursion stack and the output string.
