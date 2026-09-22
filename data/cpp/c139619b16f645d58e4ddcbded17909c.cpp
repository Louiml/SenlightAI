Write a C++ function `int maxOccurrences(int n, const std::string& type)` that, given an integer `n` (1 ≤ n ≤ 366) and a string `type` which is either `"week"` or `"month"`, returns the maximum possible number of occurrences of that time unit in a year, based on the following calendar rules: For a week, a year can have either 52 or 53 occurrences of a particular weekday. If the weekday corresponding to the first day of the year has index `n` (where 0 ≤ n ≤ 6, and only indices 5 and 6 lead to 53 occurrences in the given model), return 53 for `n` equal to 5 or 6, otherwise return 52. For a month, the maximum number of months that can have at least `n` days is 12 if `n ≤ 29`, 11 if `n == 30`, and 7 if `n ≥ 31`. The function must handle both cases and ignore any other input (you may assume the input is always valid).
// The solution directly implements the two rules. For a valid `type` string, check its first character: if it is `'w'`, then return 53 when `n` is 5 or 6, and 52 otherwise. If it is `'m'`, then return 12 when `n ≤ 29`, 11 when `n == 30`, and 7 when `n > 30`. The check `n > 30` is equivalent to `n >= 31` since `n` is an integer. No extra data structures are needed; the algorithm runs in O(1) time and uses O(1) auxiliary space. Edge cases include when `n` is exactly 30 for the month case, which must return 11, and when `n` equals 5 or 6 for the week case, which must return 53. The function should be `const`-correct and avoid modifying inputs.
#include <string>

// Returns the maximum number of occurrences of a time unit in a year.
// If type is "week", n is the day index (0-6) and returns 53 for n==5 or 6, else 52.
// If type is "month", n is the day count and returns 12, 11, or 7 based on n.
int maxOccurrences(int n, const std::string& type) {
    if (type == "week") {
        if (n == 5 || n == 6) {
            return 53;
        }
        return 52;
    }
    // type == "month"
    if (n <= 29) {
        return 12;
    }
    if (n == 30) {
        return 11;
    }
    return 7;
}
#include <cassert>
#include <string>

int main() {
    assert(maxOccurrences(5, "week") == 53);
    assert(maxOccurrences(6, "week") == 53);
    assert(maxOccurrences(0, "week") == 52);
    assert(maxOccurrences(7, "week") == 52);
    assert(maxOccurrences(29, "month") == 12);
    assert(maxOccurrences(30, "month") == 11);
    assert(maxOccurrences(31, "month") == 7);
    assert(maxOccurrences(28, "month") == 12);
    assert(maxOccurrences(30, "week") == 52); // week case uses n=30
    return 0;
}
