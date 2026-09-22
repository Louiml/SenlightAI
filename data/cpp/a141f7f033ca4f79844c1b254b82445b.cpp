Write a C++ function `bool isValidIPv4(const std::string& str)` that determines whether a given string represents a valid IPv4 address. An IPv4 address consists of exactly four decimal integers separated by periods (`.`), where each integer must be in the range 0 to 255 inclusive, must not contain leading zeros (unless the integer itself is "0"), and must not be empty. The function should return `true` for valid addresses and `false` otherwise. The input string contains only digits and periods, with no spaces, and has a length between 1 and 100. Your solution must correctly handle edge cases such as missing segments, extra segments, empty segments (e.g., `"1..2.3"`), leading zeros (e.g., `"01.2.3.4"`), out-of-range numbers (e.g., `"256.1.1.1"`), and exactly four segments.

The main approach is to split the input string by the period character `.` into segments, then verify that there are exactly four segments and that each segment is a valid decimal representation of a number between 0 and 255, with no leading zeros. However, a naive approach using `std::stoi` is insufficient because it silently ignores invalid characters and also accepts leading zeros, and it may throw exceptions on empty strings or overflow. Therefore, we must manually parse each segment: check that the segment is not empty, contains only digits, has no leading zeros (unless it's exactly "0"), and that its numeric value (computed carefully to avoid overflow) is between 0 and 255. We also need to ensure there are exactly four segments by iterating through the string and counting separators. The algorithm iterates through the entire string once, separating by periods and validating each segment as it goes. Edge cases include: exactly four segments required; empty segments (two consecutive periods or a leading/trailing period) are invalid; segments with non-digit characters are invalid; segments with leading zeros beyond the single "0" are invalid; and numeric value overflow must be avoided by checking the digit count or by accumulating with an early break. Time complexity is O(n) where n is the length of the string, and space complexity is O(1) aside from minor temporary storage for each segment.

#include <string>
#include <cctype>

// Check if a string is a valid IPv4 address.
// Returns true if the string has exactly four decimal numbers (0-255)
// separated by periods, with no leading zeros allowed except "0" itself.
bool isValidIPv4(const std::string& str) {
    int segmentCount = 0;
    std::string currentSegment;
    
    for (size_t i = 0; i <= str.length(); ++i) {
        // Process when we hit a period or end of string
        if (i == str.length() || str[i] == '.') {
            // Empty segment is invalid
            if (currentSegment.empty()) {
                return false;
            }
            // Segment length cannot exceed 3 digits for 0-255
            if (currentSegment.length() > 3) {
                return false;
            }
            // No leading zeros allowed (except "0" itself)
            if (currentSegment.length() > 1 && currentSegment[0] == '0') {
                return false;
            }
            // Ensure all characters are digits
            int value = 0;
            for (char c : currentSegment) {
                if (!std::isdigit(static_cast<unsigned char>(c))) {
                    return false;
                }
                value = value * 10 + (c - '0');
            }
            // Check range
            if (value < 0 || value > 255) {
                return false;
            }
            segmentCount++;
            currentSegment.clear();
        } else {
            currentSegment += str[i];
        }
    }
    
    // Must have exactly 4 segments
    return segmentCount == 4;
}

#include <cassert>
#include <string>

// Function declaration (since solution is separate)
bool isValidIPv4(const std::string& str);

int main() {
    // Valid addresses
    assert(isValidIPv4("0.0.0.0") == true);
    assert(isValidIPv4("255.255.255.255") == true);
    assert(isValidIPv4("192.168.1.1") == true);
    assert(isValidIPv4("1.2.3.4") == true);
    
    // Invalid: not exactly four segments
    assert(isValidIPv4("1.2.3") == false);
    assert(isValidIPv4("1.2.3.4.5") == false);
    assert(isValidIPv4("1.2.3.4.") == false);
    assert(isValidIPv4(".1.2.3.4") == false);
    
    // Invalid: empty segments
    assert(isValidIPv4("1..2.3") == false);
    assert(isValidIPv4("1.2.3.4") == true); // control check
    assert(isValidIPv4("1.2..4") == false);
    
    // Invalid: leading zeros
    assert(isValidIPv4("01.2.3.4") == false);
    assert(isValidIPv4("0.02.3.4") == false);
    assert(isValidIPv4("0.0.0.00") == false);
    
    // Invalid: out of range
    assert(isValidIPv4("256.1.1.1") == false);
    assert(isValidIPv4("1.300.1.1") == false);
    assert(isValidIPv4("1.1.1.999") == false);
    
    // Invalid: non-digit characters
    assert(isValidIPv4("1a.2.3.4") == false);
    assert(isValidIPv4("1.2.3.-4") == false);
    assert(isValidIPv4("1.2.3.4a") == false);
    
    // Edge: single "0" is valid, but "00" is not
    assert(isValidIPv4("0.0.0.0") == true);
    assert(isValidIPv4("00.1.2.3") == false);
    
    return 0;
}
