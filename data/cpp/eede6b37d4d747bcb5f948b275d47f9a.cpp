Write a standalone C++ function named `compareVersions` that takes two strings representing software version numbers (e.g., `"1.0.3"`, `"2.10.0"`) and returns an integer: `1` if the first version is greater than the second, `-1` if the second is greater, and `0` if they are equal. Versions consist of one or more numeric components separated by periods (`.`). Components may contain leading zeros (e.g., `"01.2"` is equivalent to `"1.2"`), and versions may have different numbers of components (e.g., `"1.0"` equals `"1.0.0"`). Assume the input strings are non-empty and contain only digits and periods (no spaces, letters, or negative signs). The function must be efficient and avoid any assumptions about the maximum number of components or their lengths.

The core algorithm processes both version strings component-by-component, comparing the numeric value of each segment separated by periods. Simultaneously scan both strings from left to right using two indices, building the current numeric segment for each string by multiplying the accumulated value by 10 and adding the digit (converted via `ch - '0'`). Stop building a segment when encountering a period or reaching the end of the string. Compare the two numeric values: if they differ, return immediately (`1` if v1's segment is larger, otherwise `-1`). If they are equal, increment both indices past the period (if any) and reset the numeric accumulators to zero. Continue until both strings are fully processed. Important edge cases: (1) different numbers of components—e.g., `"1.0"` vs `"1.0.0"`—the loop continues because the condition `(i < v1.length() || j < v2.length())` remains true until both are exhausted; the missing component is treated as `0`. (2) Leading zeros are naturally handled since numeric accumulation ignores them (e.g., `"01"` becomes `1`). (3) Empty segments (e.g., `"1..2"`) are not valid input per the constraints, so no special handling is needed, but if they occur, the logic would treat them as `0`. Time complexity is O(n + m) where n and m are lengths of the two strings, and space complexity is O(1) as only a few integer variables are used.

#include <string>

// Compare two version strings consisting of dot-separated numeric components.
// Returns 1 if v1 > v2, -1 if v1 < v2, and 0 if equal.
int compareVersions(const std::string& v1, const std::string& v2) {
    int i = 0, j = 0;
    int num1 = 0, num2 = 0;

    while (i < v1.length() || j < v2.length()) {
        // Parse next component from v1
        while (i < v1.length() && v1[i] != '.') {
            num1 = num1 * 10 + (v1[i] - '0');
            ++i;
        }

        // Parse next component from v2
        while (j < v2.length() && v2[j] != '.') {
            num2 = num2 * 10 + (v2[j] - '0');
            ++j;
        }

        if (num1 > num2) return 1;
        if (num2 > num1) return -1;

        // Reset for next pair and skip the period if present
        num1 = 0;
        num2 = 0;
        if (i < v1.length()) ++i; // skip '.'
        if (j < v2.length()) ++j; // skip '.'
    }

    return 0;
}

#include <cassert>
#include <string>

// Forward declaration of the function under test.
int compareVersions(const std::string& v1, const std::string& v2);

int main() {
    // Basic comparisons
    assert(compareVersions("1.0.3", "1.0.7") == -1);
    assert(compareVersions("1.0.7", "1.0.3") == 1);
    assert(compareVersions("1.0", "1.0") == 0);

    // Different number of components
    assert(compareVersions("1.0", "1.0.0") == 0);
    assert(compareVersions("2.0", "2.0.1") == -1);
    assert(compareVersions("2.0.1", "2.0") == 1);

    // Leading zeros
    assert(compareVersions("01.2", "1.2") == 0);
    assert(compareVersions("01.2.0", "1.2") == 0);

    // Larger components
    assert(compareVersions("1.10", "1.9") == 1);
    assert(compareVersions("1.9", "1.10") == -1);

    // Single component
    assert(compareVersions("5", "4") == 1);
    assert(compareVersions("4", "5") == -1);
    assert(compareVersions("4", "4") == 0);

    // Edge case with trailing periods (not valid per problem, but tests robustness) — not required but safe
    // Uncommenting the following would fail if input invalid; skip
    // assert(compareVersions("1.", "1.0") == 0); // Depending on logic, may fail; not part of spec

    return 0;
}
