/*
Given an integer `n` and an array `a` of exactly `n` non-negative integers, you must write a C++ function that simulates the output of a simple string-building algorithm. The function should return (not print) a single string that concatenates commands as follows: For the first element, output exactly `a[0]` repetitions of the pattern `"PRL"` (in that order). Then, for each subsequent element `a[i]` (where `i` from 1 to n-1), first output a single `'R'` character, then exactly `a[i]` repetitions of the pattern `"PLR"`. The final result is the concatenation of all these pieces in order. Your function must take a `const std::vector<int>&` (which contains the array) and return a `std::string`. Assume the input vector size is at least 1, and all values are non-negative. The function should be efficient, avoiding unnecessary intermediate strings where possible.
*/

#include <string>
#include <vector>

// Builds the command sequence as described for a given array of non-negative integers.
std::string buildCommands(const std::vector<int>& a) {
    if (a.empty()) {
        return "";
    }

    // Calculate total length to reserve.
    std::size_t total = static_cast<std::size_t>(a[0]) * 3;
    for (std::size_t i = 1; i < a.size(); ++i) {
        total += 1 + static_cast<std::size_t>(a[i]) * 3;
    }

    std::string result;
    result.reserve(total);

    // First element: repeat "PRL" a[0] times.
    for (int i = 0; i < a[0]; ++i) {
        result += "PRL";
    }

    // Remaining elements: 'R' then repeat "PLR" a[i] times.
    for (std::size_t i = 1; i < a.size(); ++i) {
        result += 'R';
        for (int j = 0; j < a[i]; ++j) {
            result += "PLR";
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
std::string buildCommands(const std::vector<int>& a); // forward declaration

int main() {
    assert(buildCommands({0}) == "");
    assert(buildCommands({1}) == "PRL");
    assert(buildCommands({3}) == "PRLPRLPRL");
    assert(buildCommands({1, 0}) == "PRLR");
    assert(buildCommands({0, 2}) == "RPLRPLR");
    assert(buildCommands({2, 1, 0}) == "PRLPRLRR");
    assert(buildCommands({1, 1, 1}) == "PRLRPLRRPLR");
    assert(buildCommands({0, 0, 3}) == "RRPLRPLRPLR");
    assert(buildCommands({2, 0, 0, 1}) == "PRLPRLRRRPLR");
}

// The problem is straightforward: we need to build a string according to a deterministic pattern. The key is to iterate through the vector, and for the first element (index 0), append the substring `"PRL"` `a[0]` times. For every subsequent element (index ≥ 1), first append a single `'R'`, then append `"PLR"` `a[i]` times. The total length of the resulting string can be computed as: for index 0, it contributes `3 * a[0]` characters; for each i ≥ 1, it contributes `1 + 3 * a[i]` characters. Since all values are non-negative, we don't have to worry about negative repetitions. Edge cases include: when `n` is 1 (only the first pattern is used), or when some `a[i]` is 0 (then for i ≥ 1 we still append a single `'R'`; for `a[0]` equals 0, we append nothing for that part). Time complexity is O(total characters) which is O(sum of a[i]) because each command character is appended exactly once. Space complexity is O(total characters) for the returned string. We can reserve the exact needed capacity upfront for efficiency.
