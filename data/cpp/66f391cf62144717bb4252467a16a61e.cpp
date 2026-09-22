Write a C++ function named `daysUntilChristmas` that takes a single integer `D` (1 ≤ D ≤ 25) representing the day of December, and returns a string containing the word "Christmas" followed by `(25 - D)` copies of the word " Eve", each separated by a single space, and with no trailing whitespace. For example, if `D = 25`, the function should return just `"Christmas"`; if `D = 24`, return `"Christmas Eve"`; if `D = 23`, return `"Christmas Eve Eve"`. The function must not read from standard input or write to standard output; it must only return the computed string.
#include <cassert>
#include <string>

// Declaration of the function under test
std::string daysUntilChristmas(int day);

int main() {
    assert(daysUntilChristmas(25) == "Christmas");
    assert(daysUntilChristmas(24) == "Christmas Eve");
    assert(daysUntilChristmas(23) == "Christmas Eve Eve");
    assert(daysUntilChristmas(22) == "Christmas Eve Eve Eve");
    assert(daysUntilChristmas(1) == "Christmas Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve");
    assert(daysUntilChristmas(20) == "Christmas Eve Eve Eve Eve Eve");
    assert(daysUntilChristmas(10) == "Christmas Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve");
    assert(daysUntilChristmas(13) == "Christmas Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve");
    assert(daysUntilChristmas(2) == "Christmas Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve Eve");
    assert(daysUntilChristmas(25) != "Christmas Eve");
    return 0;
}
#include <string>

// Returns a string with the word "Christmas" followed by (25 - day) copies of " Eve".
std::string daysUntilChristmas(int day) {
    std::string result = "Christmas";
    int number_of_evens = 25 - day;  // always between 0 and 24
    for (int i = 0; i < number_of_evens; ++i) {
        result += " Eve";
    }
    return result;
}
// The solution is straightforward: the number of "Eve" suffixes is exactly `25 - D`. Since `D` is guaranteed to be between 1 and 25 inclusive, the count is always between 0 and 24. The algorithm builds the result string by starting with `"Christmas"`, then appending `" Eve"` repeatedly `(25 - D)` times. This can be done using a loop or by using `std::string` concatenation. Edge cases: when `D = 25`, no " Eve" is appended, and the result is exactly `"Christmas"`. The function must not output anything; it only returns the string. Time complexity is O(25 - D) which is O(1) because the maximum is 24 iterations. Space complexity is O(length of result), which is at most `"Christmas"` plus 24 times `" Eve"` (each 4 characters including the leading space), so roughly 8 + 96 = 104 characters, thus O(1) in practice.
