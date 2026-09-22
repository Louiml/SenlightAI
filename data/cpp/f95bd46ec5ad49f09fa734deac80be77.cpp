Write a C++ function `timeBreakdown(int totalDays)` that takes a non-negative integer representing a number of days (where 1 year = 365 days, 1 month = 30 days) and returns a `std::string` in the format `"X years, Y months, Z days"` with exact spelling (singular "year"/"month"/"day" when the quantity is 1, otherwise plural). The function must not modify the input and must handle `0` as an input (returning `"0 days"`). For example, `timeBreakdown(400)` should return `"1 year, 1 month, 5 days"` (since 400 = 365 + 30 + 5). Edge cases include inputs with only years, only months, only days, or zero, and the output must have no leading/trailing spaces and no extra commas.
The algorithm extracts the number of full years by integer division by 365, then updates the remaining days by taking the modulus. The same process is repeated for months (30 days per month) on the remainder, leaving the remaining days. The formatting requires careful handling of singular/plural nouns and comma placement. A clean approach is to build a vector of non-zero components (e.g., `"1 year"`, `"3 months"`, `"5 days"`) as strings, then join them with `", "` if there are multiple components, or use the single component alone. If all components are zero, return `"0 days"`. The time complexity is O(1) since the number of arithmetic operations is constant, and space complexity is O(1) besides the output string. Edge cases: `0 → "0 days"`, `365 → "1 year"`, `30 → "1 month"`, `1 → "1 day"`, and `400 → "1 year, 1 month, 5 days"`.
#include <string>
#include <vector>

// Convert a total number of days into "X years, Y months, Z days" format.
// Uses 1 year = 365 days, 1 month = 30 days. The input must be non-negative.
std::string timeBreakdown(int totalDays) {
    const int daysPerYear = 365;
    const int daysPerMonth = 30;

    int years = totalDays / daysPerYear;
    int remaining = totalDays % daysPerYear;
    int months = remaining / daysPerMonth;
    int days = remaining % daysPerMonth;

    std::vector<std::string> parts;
    if (years > 0) {
        parts.push_back(std::to_string(years) + (years == 1 ? " year" : " years"));
    }
    if (months > 0) {
        parts.push_back(std::to_string(months) + (months == 1 ? " month" : " months"));
    }
    if (days > 0) {
        parts.push_back(std::to_string(days) + (days == 1 ? " day" : " days"));
    }

    if (parts.empty()) {
        return "0 days";
    }

    std::string result;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += parts[i];
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string timeBreakdown(int totalDays);

int main() {
    assert(timeBreakdown(0) == "0 days");
    assert(timeBreakdown(1) == "1 day");
    assert(timeBreakdown(30) == "1 month");
    assert(timeBreakdown(365) == "1 year");
    assert(timeBreakdown(400) == "1 year, 1 month, 5 days");
    assert(timeBreakdown(730) == "2 years");
    assert(timeBreakdown(45) == "1 month, 15 days");
    assert(timeBreakdown(396) == "1 year, 1 month, 1 day");
    assert(timeBreakdown(1000) == "2 years, 9 months, 0 days"); // Note: 1000 % 365 = 270, 270/30=9, 270%30=0
    assert(timeBreakdown(365*2 + 30*3 + 7) == "2 years, 3 months, 7 days");
    return 0;
}
