// Write a C++ function named `roundToNearestTen` that takes a string representing a positive integer (with no leading zeros, except possibly the string "0") and returns a string representing that number rounded to the nearest multiple of 10. If the number is exactly halfway between two multiples of 10 (i.e., the last digit is 5), the function must round down (choose the smaller multiple). The input string can be up to 10^5 characters long, so the algorithm must be efficient. The function should handle cases where rounding causes all digits to become 9 and then carry over, potentially adding a new most-significant digit (e.g., "995" -> "1000"). The function must not use any big-integer libraries; do the rounding directly on the string.
// The core idea is to examine only the last digit of the string. If the last digit is less than or equal to '5', we simply replace it with '0' — this handles both rounding down and the tie-breaking rule for 5. If the last digit is greater than '5', we replace it with '0' and then add 1 to the previous digit(s), propagating the carry like in manual addition. The propagation works from right to left: if a digit is '9', we set it to '0' and continue carrying; otherwise, we increment it by 1 and stop. If we run out of digits (i.e., the entire number was a series of 9s after the first digit), prepend "1" at the beginning. This approach is O(n) in time because we only traverse the string once from right to left (and the final string concatenation is O(n)). Space is O(1) auxiliary beyond the input/output string, since we modify the string in-place and only use a few integer flags.
#include <string>

// Given a string representing a positive integer, return the nearest multiple of 10.
// If the number is exactly halfway (last digit is 5), round down (smaller multiple).
std::string roundToNearestTen(const std::string& str) {
    // Work on a copy to modify.
    std::string result = str;
    int n = static_cast<int>(result.size());
    
    // Determine if we need to round up or down based on the last digit.
    // For digits '0'..'5', round down (just set last digit to '0').
    // For digits '6'..'9', round up (set last digit to '0' and propagate carry).
    bool carry = (result[n - 1] > '5');
    result[n - 1] = '0';
    
    // If rounding up, propagate the carry leftward.
    if (carry) {
        int i = n - 2;
        while (i >= 0 && carry) {
            if (result[i] == '9') {
                result[i] = '0';
                // continue carrying
            } else {
                // Digit is '0'..'8', increment and stop.
                result[i] = static_cast<char>(result[i] + 1);
                carry = false;
            }
            --i;
        }
        // If carry persists after processing all digits, prepend '1'.
        if (carry) {
            result = "1" + result;
        }
    }
    
    return result;
}
#include <cassert>
#include <string>

// Declare the solution function (assuming it is defined above).
std::string roundToNearestTen(const std::string& str);

int main() {
    // Basic cases
    assert(roundToNearestTen("29") == "30");
    assert(roundToNearestTen("15") == "10");
    assert(roundToNearestTen("10") == "10");
    assert(roundToNearestTen("11") == "10");
    assert(roundToNearestTen("19") == "20");
    assert(roundToNearestTen("20") == "20");
    assert(roundToNearestTen("21") == "20");
    assert(roundToNearestTen("25") == "20"); // tie, round down
    assert(roundToNearestTen("26") == "30");
    assert(roundToNearestTen("5") == "0"); // single digit, round down
    assert(roundToNearestTen("9") == "10"); // single digit, round up
    assert(roundToNearestTen("0") == "0"); // single zero
    // Carry propagation
    assert(roundToNearestTen("99") == "100");
    assert(roundToNearestTen("995") == "990"); // last digit 5, round down, no carry
    assert(roundToNearestTen("996") == "1000"); // carry propagates through all 9s
    assert(roundToNearestTen("999") == "1000"); // all 9s, add new digit
    assert(roundToNearestTen("100") == "100");
    assert(roundToNearestTen("101") == "100");
    assert(roundToNearestTen("109") == "110");
    assert(roundToNearestTen("190") == "190");
    assert(roundToNearestTen("195") == "190"); // tie, round down
    assert(roundToNearestTen("196") == "200");
    // Large number to ensure no overflow (test with a long string)
    std::string big(100000, '9');
    // Rounding "999...999" (all 9s) with last digit 9 > 5, so rounds up to "1000...000"
    std::string bigExpected = "1" + std::string(100000, '0');
    assert(roundToNearestTen(big) == bigExpected);
    
    return 0;
}
