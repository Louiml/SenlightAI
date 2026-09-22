/*
Write a C++ function that takes two non-negative integer strings (containing only digits, with no leading zeros unless the number is exactly "0") and returns their sum as a string. The function must handle arbitrarily large numbers that exceed the capacity of built-in integer types. You may assume the inputs are valid and non-empty. The function should be named `addLargeNumbers` and accept two `const std::string&` parameters, returning a `std::string`. The result should be represented without leading zeros (except for the case of the result being exactly "0").
*/

#include <string>
#include <algorithm>

// Add two non-negative integers represented as strings.
// Returns the sum as a string without leading zeros (except "0" itself).
std::string addLargeNumbers(const std::string& num1, const std::string& num2) {
    std::string a = num1;
    std::string b = num2;
    // Ensure a is the shorter string for simplicity
    if (a.size() > b.size()) {
        std::swap(a, b);
    }
    int n1 = a.size();
    int n2 = b.size();
    int diff = n2 - n1;
    int carry = 0;
    std::string result;

    // Add digits from the end of the shorter string
    for (int i = n1 - 1; i >= 0; --i) {
        int sum = (a[i] - '0') + (b[i + diff] - '0') + carry;
        result.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    // Add remaining digits of the longer string
    for (int i = n2 - n1 - 1; i >= 0; --i) {
        int sum = (b[i] - '0') + carry;
        result.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    // Add final carry if any
    if (carry) {
        result.push_back(carry + '0');
    }
    // Reverse to get correct order
    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <string>

// Assume addLargeNumbers is defined above

int main() {
    assert(addLargeNumbers("0", "0") == "0");
    assert(addLargeNumbers("1", "2") == "3");
    assert(addLargeNumbers("999", "1") == "1000");
    assert(addLargeNumbers("123", "456") == "579");
    assert(addLargeNumbers("100", "900") == "1000");
    assert(addLargeNumbers("0", "12345") == "12345");
    assert(addLargeNumbers("99999999999999999999", "1") == "100000000000000000000");
    assert(addLargeNumbers("11111111111111111111", "22222222222222222222") == "33333333333333333333");
    assert(addLargeNumbers("987654321", "123456789") == "1111111110");
    assert(addLargeNumbers("5", "5") == "10");
    return 0;
}

// The solution simulates manual digit-by-digit addition from right to left, similar to the provided snippet. First, ensure the two strings are aligned by padding the shorter one conceptually; instead of physically padding, we iterate from the end of the shorter string and compute each digit sum with the corresponding digit from the longer string (offset by the difference in lengths). A carry variable is maintained, initialized to 0. For each digit pair, compute `sum = (digit1 + digit2 + carry)`, append `sum % 10` to a result string, and update `carry = sum / 10`. After processing all digits of the shorter string, continue adding the remaining digits of the longer string with only the carry. Finally, if a non-zero carry remains, append it. The result string is built in reverse order (since we append least significant digits first), so reverse it before returning. Edge cases include: one string being empty (though inputs are non-empty), numbers of unequal lengths, and a final carry that creates an extra digit (e.g., "999" + "1" = "1000"). Time complexity is O(n) where n is the length of the longer string, and space complexity is O(n) for the result string.
