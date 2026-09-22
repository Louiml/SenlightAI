// Write a C++ function `std::string largestTime(std::vector<int>& digits)` that takes a vector of exactly four integers (each between 0 and 9, inclusive) and returns the largest valid time in 24-hour format (HH:MM) that can be formed by using each digit exactly once. If no valid time can be formed, return an empty string. For example, given `{1,2,3,4}`, the valid times include `12:34`, `13:24`, `14:23`, etc., and the largest is `23:41`; given `{5,5,5,5}`, no valid time exists, so return `""`. You must implement the function as a free function, not inside a class, and it must handle duplicate digits naturally. Assume the input vector always has size exactly 4.
The algorithm sorts the digits in ascending order and then iterates over all permutations using `std::next_permutation`. For each permutation, treat the first two digits as the hour (tens and ones) and the last two as the minute (tens and ones). Check if the hour is between 0 and 23 and the minute between 0 and 59. Track the maximum valid time by comparing hours first, then minutes. Since sorting ensures we start from the lexicographically smallest permutation, `next_permutation` generates all possible orders, guaranteeing we examine every arrangement of the four digits. The time complexity is \(O(4! \cdot 4) = O(1)\) because the number of permutations is constant (24), and the space complexity is \(O(1)\) aside from the returned string. Edge cases include all digits being too large (e.g., `{9,9,9,9}`), all permutations invalid (e.g., `{5,5,5,5}`), and times with leading zeros (e.g., `{0,0,5,9}` gives `09:50`). Also note that hours like `00` and minutes like `05` must be formatted with leading zeros.
#include <string>
#include <vector>
#include <algorithm>

// Returns the largest valid 24-hour time (HH:MM) using each digit exactly once.
// If no valid time can be formed, returns an empty string.
// The input vector must contain exactly four digits (0-9).
std::string largestTime(std::vector<int>& digits) {
    std::sort(digits.begin(), digits.end());
    int bestHour = -1;
    int bestMinute = -1;

    do {
        int hour = 10 * digits[0] + digits[1];
        int minute = 10 * digits[2] + digits[3];
        if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
            if (hour > bestHour || (hour == bestHour && minute > bestMinute)) {
                bestHour = hour;
                bestMinute = minute;
            }
        }
    } while (std::next_permutation(digits.begin(), digits.end()));

    if (bestHour == -1) {
        return "";
    }

    std::string result;
    if (bestHour < 10) {
        result += '0';
    }
    result += std::to_string(bestHour);
    result += ':';
    if (bestMinute < 10) {
        result += '0';
    }
    result += std::to_string(bestMinute);
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (omitted here for brevity).

int main() {
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(largestTime(v1) == "23:41");

    std::vector<int> v2 = {5, 5, 5, 5};
    assert(largestTime(v2) == "");

    std::vector<int> v3 = {0, 0, 0, 0};
    assert(largestTime(v3) == "00:00");

    std::vector<int> v4 = {9, 0, 0, 0};
    assert(largestTime(v4) == "09:00");

    std::vector<int> v5 = {0, 1, 2, 3};
    assert(largestTime(v5) == "23:10");

    std::vector<int> v6 = {2, 0, 6, 6};
    assert(largestTime(v6) == "06:26");

    std::vector<int> v7 = {2, 3, 3, 2};
    assert(largestTime(v7) == "23:32");

    std::vector<int> v8 = {9, 9, 9, 0};
    assert(largestTime(v8) == "09:99" || largestTime(v8) == "");  // 09:99 invalid, so empty is expected
    // Correct expected: "" because minute 99 invalid. So we assert:
    assert(largestTime(v8) == "");

    std::vector<int> v9 = {1, 1, 5, 9};
    assert(largestTime(v9) == "19:51");
}
