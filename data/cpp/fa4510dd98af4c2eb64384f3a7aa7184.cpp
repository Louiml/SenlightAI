Write a standalone C++ function named `bigIntBinaryString` that takes a decimal integer as a `std::string` (which may include an optional leading `+` or `-` sign, and may contain leading zeros) and returns its binary representation as a `std::string` in two's complement form using a fixed 32-bit width. For non‑negative numbers, the result must be exactly 32 bits with leading zeros (e.g., `"000...001"` for `1`). For negative numbers, compute the two's complement of the absolute value over 32 bits (e.g., `-1` yields `"11111111111111111111111111111111"`). The input will always be a valid integer within the range of a signed 32‑bit integer (i.e., from `-2147483648` to `2147483647`). The function must handle leading zeros in the input, preserve the sign correctly, and trim any unnecessary leading zeros from the absolute value before computing two's complement for negative numbers. It must not use any external libraries beyond standard headers, and must not assume the input fits in a built‑in integer type during conversion (i.e., it should simulate arithmetic on digit strings).

#include <cassert>
#include <string>
#include <iostream>

// The solution function is declared above (include the code from )

int main() {
    // Basic positive numbers
    assert(bigIntBinaryString("0") == "00000000000000000000000000000000");
    assert(bigIntBinaryString("1") == "00000000000000000000000000000001");
    assert(bigIntBinaryString("2") == "00000000000000000000000000000010");
    assert(bigIntBinaryString("255") == "00000000000000000000000011111111");

    // Negative numbers (two's complement)
    assert(bigIntBinaryString("-1") == "11111111111111111111111111111111");
    assert(bigIntBinaryString("-2") == "11111111111111111111111111111110");
    assert(bigIntBinaryString("-255") == "11111111111111111111111100000001");

    // With leading plus sign and leading zeros
    assert(bigIntBinaryString("+00012") == "00000000000000000000000000001100");
    assert(bigIntBinaryString("0005") == "00000000000000000000000000000101");

    // Minimum and maximum 32-bit values
    assert(bigIntBinaryString("2147483647") == "01111111111111111111111111111111");
    assert(bigIntBinaryString("-2147483648") == "10000000000000000000000000000000");

    // Negative zero treated as positive zero
    assert(bigIntBinaryString("-0") == "00000000000000000000000000000000");

    // Edge case: odd and even large negative
    assert(bigIntBinaryString("-2147483647") == "10000000000000000000000000000001");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <string>
#include <algorithm>

// Helper: divide a decimal string by 2, return quotient as string (no leading zeros)
static std::string divideByTwo(const std::string& num) {
    std::string res;
    int remainder = 0;
    for (char c : num) {
        int digit = c - '0';
        int cur = remainder * 10 + digit;
        int q = cur / 2;
        remainder = cur % 2;
        if (!res.empty() || q != 0) {
            res.push_back(static_cast<char>('0' + q));
        }
    }
    // If result is empty, it means zero
    return res.empty() ? "0" : res;
}

// Helper: remove leading zeros from a decimal string (but keep at least one digit)
static std::string trimLeadingZeros(const std::string& s) {
    size_t pos = s.find_first_not_of('0');
    if (pos == std::string::npos) return "0";
    return s.substr(pos);
}

// Helper: compute binary string (MSB first) of a non‑negative decimal string (assumes no leading zeros)
static std::string toBinary(const std::string& decimal) {
    if (decimal == "0") return "0";
    std::string bin;
    std::string cur = decimal;
    while (cur != "0") {
        bool lastBit = ((cur.back() - '0') % 2) == 1;
        bin.push_back(lastBit ? '1' : '0');
        cur = divideByTwo(cur);
    }
    std::reverse(bin.begin(), bin.end());
    return bin;
}

// Main function: convert decimal string (with optional sign) to 32‑bit two's complement binary string
std::string bigIntBinaryString(const std::string& input) {
    // Determine sign and remove sign characters
    std::string num = input;
    bool positive = true;
    if (!num.empty() && (num[0] == '-' || num[0] == '+')) {
        positive = (num[0] != '-');
        num = num.substr(1);
    }

    // Trim leading zeros from absolute value
    num = trimLeadingZeros(num);

    // Handle zero specially (positive or negative zero is 0)
    if (num == "0") {
        return std::string(32, '0');
    }

    // Get binary of absolute value (MSB first)
    std::string absBin = toBinary(num);

    if (positive) {
        // Pad with leading zeros to 32 bits
        std::string padded(32 - absBin.size(), '0');
        padded += absBin;
        return padded;
    } else {
        // Compute two's complement of absBin over 32 bits
        // First, pad absBin to 32 bits (it already fits for valid input)
        std::string bin32 = std::string(32 - absBin.size(), '0') + absBin;

        // Invert all bits
        for (char& c : bin32) {
            c = (c == '0') ? '1' : '0';
        }

        // Add 1 to the inverted string (handle carry)
        int carry = 1;
        for (int i = 31; i >= 0; --i) {
            int bit = (bin32[i] - '0') + carry;
            if (bit == 2) {
                bin32[i] = '0';
                carry = 1;
            } else {
                bin32[i] = static_cast<char>('0' + bit);
                carry = 0;
            }
        }
        // If carry remains after 32 bits, overflow, but for valid range it won't
        return bin32;
    }
}

// The core task is to convert a decimal string to a 32‑bit two's complement binary string by simulating arithmetic on decimal digit strings without using built‑in integer types for the conversion (since the input may be large, though here it's within 32‑bit range, the approach must be general). The solution uses a `divide_by_two` helper that repeatedly halves a decimal string until it becomes empty (i.e., zero), collecting the least significant bit at each step (from `num.back() - '0' % 2`). This yields the binary representation of the absolute value in reverse order (least significant bit first). After collecting all bits, reverse the string to get the normal binary representation of the absolute value. For non‑negative numbers, pad the binary string with leading zeros to exactly 32 bits. For negative numbers, first obtain the binary of the absolute value, then invert all bits (flip 0↔1) and add 1 (handle carry) to get two's complement; finally, ensure the result is exactly 32 bits (if the absolute value is 0, special case: negative zero is not allowed, so input `-0` should be treated as `+0`). Edge cases: input may have leading zeros (e.g., `"00012"`), so trim them before conversion; input may be `"0"` (non‑negative) yielding 32 zeros; input `"-2147483648"` (minimum) whose absolute value is `2147483648`, which fits in 32 bits as two's complement (binary `1000...0`). The algorithm runs in O(k^2) time and O(k) space, where k is the number of decimal digits ( ≤ 10 here but general). The `divide_by_two` function processes each digit once, and the while loop in `bin` runs O(k) times because each division reduces the number of digits by about log10(2) ≈ 0.3, so the loop runs at most O(k) times, each taking O(k) to divide, giving O(k^2) total. For the fixed 32‑bit output, the time is effectively constant.
