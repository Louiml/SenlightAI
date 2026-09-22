/*
Write a C++ function `int packetChecksum(const std::string& packet)` that takes a single string representing a network packet in the format `"d..d|d..d|… >"` (one or more non-negative decimal integers separated by `|`, followed by a space and `>`; an empty packet is an empty string). The function must compute the checksum of the packet as the sum of all its integer components modulo 256. If the packet is equal to `""` (empty string), representing the termination packet, return -1. If the packet contains no integers (e.g., `">"` or `" >"`), treat the sum as 0 and return 0. The function must handle leading/trailing whitespace, multiple consecutive `|` separators (which should produce a value of 0 for the missing component), and any non-negative integer values (including very large ones). It must ignore any characters that are not digits, `|`, spaces, or `>`. If any integer is negative in the input, take its absolute value before summing.
*/
#include <string>
#include <cstdlib>
#include <cctype>

// Returns the checksum (sum modulo 256) of a packet in the format "d..d|d..d|… >".
// Returns -1 if the packet is empty (termination signal).
int packetChecksum(const std::string& packet) {
    if (packet.empty()) {
        return -1;
    }

    long long currentNumber = 0;
    bool hasDigits = false;
    long long checksum = 0;
    bool hasComponent = false;

    auto flushNumber = [&]() {
        if (hasDigits) {
            long long value = std::llabs(currentNumber);
            checksum += value;
            hasComponent = true;
            currentNumber = 0;
            hasDigits = false;
        }
    };

    for (char ch : packet) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            currentNumber = currentNumber * 10 + (ch - '0');
            hasDigits = true;
        } else if (ch == '|') {
            flushNumber();
            hasComponent = true;
        } else if (ch == '>') {
            flushNumber();
            return (hasComponent ? static_cast<int>(checksum % 256) : 0);
        }
        // All other characters (spaces, etc.) are ignored.
    }

    // If we never hit '>', flush and return as if complete.
    flushNumber();
    return (hasComponent ? static_cast<int>(checksum % 256) : 0);
}
#include <cassert>
#include <string>

// The packetChecksum function is defined above.

int main() {
    // Valid single component
    assert(packetChecksum("5 >") == 5);
    assert(packetChecksum("0 >") == 0);
    assert(packetChecksum("255 >") == 255);
    assert(packetChecksum("256 >") == 0);

    // Multiple components
    assert(packetChecksum("1|2|3 >") == 6);
    assert(packetChecksum("10|20|30 >") == 60);
    assert(packetChecksum("255|1 >") == 0);

    // Empty packet
    assert(packetChecksum("") == -1);

    // No components but '>' present
    assert(packetChecksum(" >") == 0);
    assert(packetChecksum(">") == 0);

    // Multiple '|' with missing numbers
    assert(packetChecksum("5||10 >") == 15);
    assert(packetChecksum("|3| >") == 3);

    // Leading/trailing spaces and ignored characters
    assert(packetChecksum("  7  |  8  > ") == 15);
    assert(packetChecksum("a1b|2c >") == 3);

    // Very large numbers (within long long range)
    assert(packetChecksum("1000000000|1 >") == 1); // sum = 1000000001, mod 256 = 1

    // Negative numbers are taken absolute value
    assert(packetChecksum("-3|4 >") == 7);

    // Just digits without '>' or '|' — flush at end
    assert(packetChecksum("123") == 123);

    assert(packetChecksum("| >") == 0);
    assert(packetChecksum("|| >") == 0);

    return 0;
}
// The solution requires parsing the input string character by character. Maintain a running numeric accumulator that builds a number from consecutive digit characters. Maintain a checksum accumulator initialized to 0. Also maintain a flag `hasComponent` to know if at least one number was separated by `|`. Process characters as follows: if the character is a digit, append it to the numeric accumulator. If the character is `|`, then take the current numeric accumulator (treat as 0 if empty), convert to integer, apply absolute value, add to checksum, reset the numeric accumulator, and set `hasComponent = true`. If the character is `>`, do the same as for `|` (flush any remaining number), then return `(hasComponent ? checksum % 256 : 0)` — but if the entire original string is empty, return -1 first. For any other character (e.g., spaces), ignore it. If after processing the entire string no `|` or `>` was found but digits exist (malformed), still flush and return the checksum. Edge cases include: empty string → -1; string with only `>` → 0; string with `|` only → 0; string with leading/trailing spaces → ignore; large numbers → use `long long` for the accumulator to avoid overflow during conversion (though `stoll` can handle arbitrarily large within limits); negative numbers → take absolute value. Time complexity is O(n) where n is the length of the input string, and space complexity is O(1) besides the input string itself.
