// Write a C++ function named `isDotSeparatedIPv4` that takes a constant string reference `s` representing a candidate IPv4 address and returns a boolean. The function must verify that the string contains exactly three dots, that each dot appears at a valid position such that the string is logically divided into four non-empty numeric groups, and that each group consists only of digits and represents an integer in the range 0 to 255. The function should return `false` if any condition fails, including strings with leading or trailing dots, consecutive dots, empty groups, non-digit characters, or out-of-range values. The input is assumed non-empty but may contain any printable ASCII characters. The solution must not use external libraries beyond the standard library and should treat each part independently.

The task is to parse and validate an IPv4-like string. The main algorithm first checks whether the total number of dots in the string equals three. If not, return false immediately. Then, split the string by the dot character into four substrings. For each substring, verify it is non-empty and contains only digit characters; if any character is not a digit, return false. Convert the substring to an integer using `std::stoi` (or manual accumulation to avoid exceptions), then check that the value is between 0 and 255 inclusive. If all four groups pass, return true. Edge cases include strings with more than three dots, missing groups (e.g., "1.2.3" or "1..2.3"), leading/trailing dots, empty input, non-digit characters like letters or spaces, and numbers with leading zeros (which are acceptable if within range, e.g., "01" is valid as 1). The time complexity is O(n) where n is the length of the string, because we traverse each character at most once. The space complexity is O(1) auxiliary, ignoring the storage needed for the string itself and its substrings if we choose to build them; alternatively we can parse in-place, but using a vector of substrings is simpler and still O(n) worst-case memory, which is acceptable. For robustness, we avoid `std::stoi` exceptions by manually parsing digits or using a safe conversion with error checking.

#include <string>
#include <vector>
#include <cctype>

// Checks if the string is a valid IPv4 address with exactly four decimal groups.
bool isDotSeparatedIPv4(const std::string& s) {
    // Count dots to ensure exactly three.
    int dotCount = 0;
    for (char c : s) {
        if (c == '.') dotCount++;
    }
    if (dotCount != 3) return false;

    // Split by dots into four non-empty groups.
    std::vector<std::string> groups;
    std::string current;
    for (char c : s) {
        if (c == '.') {
            groups.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    groups.push_back(current);  // last group after final dot

    if (groups.size() != 4) return false;

    for (const std::string& part : groups) {
        if (part.empty()) return false;
        // Each group must consist solely of digits.
        for (char ch : part) {
            if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
        }
        // Convert to number without using stoi (avoids exceptions).
        int value = 0;
        for (char ch : part) {
            value = value * 10 + (ch - '0');
            if (value > 255) return false; // early exit for overflow prevention
        }
        if (value > 255) return false;
    }
    return true;
}

#include <cassert>

int main() {
    // Valid addresses
    assert(isDotSeparatedIPv4("0.0.0.0") == true);
    assert(isDotSeparatedIPv4("255.255.255.255") == true);
    assert(isDotSeparatedIPv4("192.168.1.1") == true);
    assert(isDotSeparatedIPv4("1.2.3.4") == true);
    assert(isDotSeparatedIPv4("001.002.003.004") == true); // leading zeros allowed

    // Invalid addresses
    assert(isDotSeparatedIPv4("256.1.1.1") == false);       // out of range
    assert(isDotSeparatedIPv4("1.2.3") == false);           // missing dot
    assert(isDotSeparatedIPv4("1.2.3.4.5") == false);       // too many dots
    assert(isDotSeparatedIPv4("1..2.3.4") == false);        // empty group
    assert(isDotSeparatedIPv4(".1.2.3") == false);          // leading dot
    assert(isDotSeparatedIPv4("1.2.3.") == false);          // trailing dot
    assert(isDotSeparatedIPv4("1.2.3.a") == false);         // non-digit
    assert(isDotSeparatedIPv4("1.2.3.4 ") == false);        // space
    assert(isDotSeparatedIPv4("") == false);                // empty
    assert(isDotSeparatedIPv4("-1.2.3.4") == false);        // negative sign
}
