/*
Write a C++ function `std::string mostFrequentDate(const std::string& text)` that takes a string `text` containing only uppercase and lowercase letters, digits, and hyphens (`-`). The function must examine every contiguous substring of length exactly 10 (i.e., every 10-character window) in `text`, and for each substring that represents a valid date in the format `DD-MM-YYYY`, count how many times that exact date string appears. Return the date string (in the same `DD-MM-YYYY` format with leading zeros preserved) that occurs most frequently among all valid date substrings. If multiple dates share the maximum frequency, return the one that appears first in `text` (i.e., with the smallest starting index). If no valid date substring exists, return an empty string. A date is considered valid if: the year is between 2013 and 2015 inclusive, the month is between 01 and 12, the day is valid for that month and year (including leap-year rules for February), and the date is a real calendar date (i.e., normalization via `mktime` returns the same day/month/year). Note that substrings may overlap, and the same date may appear multiple times.
*/
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <cstdlib>

// Checks if a 10-character string is a valid date in DD-MM-YYYY format with year 2013-2015.
// Returns true if valid, false otherwise.
static bool isValidDate(const std::string& s) {
    // Format check: s[2] and s[5] must be '-', all other positions must be digits.
    if (s[2] != '-' || s[5] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (s[i] < '0' || s[i] > '9') return false;
    }

    int day = std::atoi(s.substr(0, 2).c_str());
    int month = std::atoi(s.substr(3, 2).c_str());
    int year = std::atoi(s.substr(6, 4).c_str());

    if (year < 2013 || year > 2015) return false;

    std::tm timeinfo = {};
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;

    std::mktime(&timeinfo);

    // After normalization, check if fields still match the intended values.
    return (timeinfo.tm_year == year - 1900 &&
            timeinfo.tm_mon == month - 1 &&
            timeinfo.tm_mday == day);
}

// Returns the most frequent valid date substring (DD-MM-YYYY) in text.
// Ties are broken by earliest first occurrence in text.
std::string mostFrequentDate(const std::string& text) {
    if (text.length() < 10) return "";

    // Map from date string to pair (count, first_occurrence_index)
    std::map<std::string, std::pair<int, int>> freq;

    for (size_t i = 0; i + 10 <= text.length(); ++i) {
        std::string cur = text.substr(i, 10);
        if (isValidDate(cur)) {
            auto it = freq.find(cur);
            if (it == freq.end()) {
                freq[cur] = {1, static_cast<int>(i)};
            } else {
                it->second.first++;
            }
        }
    }

    if (freq.empty()) return "";

    std::string best;
    int bestCount = -1;
    int bestFirst = -1;
    for (const auto& entry : freq) {
        int count = entry.second.first;
        int first = entry.second.second;
        if (count > bestCount || (count == bestCount && first < bestFirst)) {
            best = entry.first;
            bestCount = count;
            bestFirst = first;
        }
    }
    return best;
}
#include <cassert>
#include <iostream>
#include <string>

// Declare the function from the solution (include the header or copy declaration)
std::string mostFrequentDate(const std::string& text);

int main() {
    // Simple valid date repeated
    assert(mostFrequentDate("15-01-2015 15-01-2015") == "15-01-2015");
    // Overlapping windows: "01-01-201501-01-2015" has two valid dates
    assert(mostFrequentDate("01-01-201501-01-2015") == "01-01-2015");
    // No valid date (invalid day)
    assert(mostFrequentDate("31-02-2015") == "");
    // Invalid date: 29 Feb in non-leap year 2013
    assert(mostFrequentDate("29-02-2013") == "");
    // Year out of range
    assert(mostFrequentDate("01-01-2012") == "");
    // Tie-breaking by first occurrence
    // Both appear once, "01-01-2015" appears at index 0 first
    assert(mostFrequentDate("01-01-2015 02-01-2015") == "01-01-2015");
    // Most frequent with two different dates
    // "01-01-2015" appears twice, "02-01-2015" once
    assert(mostFrequentDate("01-01-2015 02-01-2015 01-01-2015") == "01-01-2015");
    // Empty or too short string
    assert(mostFrequentDate("") == "");
    assert(mostFrequentDate("12345") == "");
    // Valid leap year? 2015 is not leap, 2013 not leap, so 29-02 never valid
    assert(mostFrequentDate("29-02-2015") == "");
    // Test that day 31 works for months with 31 days
    assert(mostFrequentDate("31-03-2015") == "31-03-2015");
    // Test that day 31 fails for April (30 days)
    assert(mostFrequentDate("31-04-2015") == "");
    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The main algorithm is a sliding window over the input string of length `n`. For each starting index `i` from 0 to `n-10`, extract the 10-character substring `s = text.substr(i,10)`. First, validate the fixed format: positions 2 and 5 must be hyphens, and all other positions must be digits (including checking that no digit position contains a hyphen). Then parse the day, month, and year from the appropriate substrings. Reject if year is outside [2013, 2015]. To validate the day/month combination, use `std::tm` and `mktime`: set the fields, call `mktime`, and check that after normalization the `tm_year`, `tm_mon`, and `tm_mday` match the intended values. This correctly handles leap years and days-per-month. For each valid date, increment its count in an ordered map (using `std::map` to preserve lexicographic order, but we need insertion order for tie-breaking). To handle tie-breaking by first occurrence, we can track the first occurrence index for each date when first inserted. After scanning all windows, iterate through the map and select the date with maximum count; if a tie occurs, choose the one with the smaller first occurrence index. Time complexity is O(n * 10) for the sliding window plus O(k log k) for map operations where k is the number of distinct valid dates, so O(n) in practice since 10 is constant and k ≤ n. Space complexity is O(k) for the map. Edge cases: string length less than 10 returns empty; overlapping substrings are allowed; dates like "31-02-2015" must be rejected; "29-02-2013" must be rejected (not a leap year); "29-02-2015" also rejected; but "29-02-2016" is not in range, so year 2016 is rejected by range check anyway. Also ensure that the input contains no other characters except letters/digits/hyphens, but we don't need to pre-validate; we just check each window.
