// Write a C++ function `std::string formLargestNumber(const std::vector<std::string>& numbers)` that takes a vector of strings, each containing a non-negative integer (with no leading zeros except for "0" itself), and returns a single string representing the largest possible number that can be formed by concatenating all the numbers in any order. For example, given `{"3", "30", "34", "5", "9"}`, the largest number is `"9534330"` (not `"3303459"`). The function should handle edge cases such as empty input (return an empty string) and cases where all numbers are zeros (return `"0"`). You may assume the input vector contains at least one element and each string consists only of digits, contains no leading zeros (except "0"), and has length between 1 and 10.

#include <cassert>
#include <vector>
#include <string>

std::string formLargestNumber(const std::vector<std::string>& numbers);

int main() {
    // Basic case
    assert(formLargestNumber({"3", "30", "34", "5", "9"}) == "9534330");
    // Single element
    assert(formLargestNumber({"10"}) == "10");
    // All zeros
    assert(formLargestNumber({"0", "0", "0"}) == "0");
    // Edge case with "0" and "0" should not produce "00"
    assert(formLargestNumber({"0", "0"}) == "0");
    // Larger numbers
    assert(formLargestNumber({"824", "938", "1399", "5607", "6973", "5703", "9609", "4398", "8247"}) == "9609938824824769735703560713981399");
    // Reverse order
    assert(formLargestNumber({"9", "91"}) == "991");
    // Duplicates
    assert(formLargestNumber({"121", "12"}) == "12121");
    // Equal concatenation (both orders give same) should be stable or accept either
    assert(formLargestNumber({"2", "2"}) == "22");
    // Mixed lengths
    assert(formLargestNumber({"1", "10", "9"}) == "9110");
    // Empty vector
    assert(formLargestNumber({}) == "");
    return 0;
}

#include <string>
#include <vector>
#include <algorithm>

// Returns the largest number formed by concatenating all given numeric strings.
std::string formLargestNumber(const std::vector<std::string>& numbers) {
    if (numbers.empty()) return "";

    // Copy to sort without modifying the input.
    std::vector<std::string> sorted = numbers;

    // Custom comparator: a before b if a+b > b+a.
    std::sort(sorted.begin(), sorted.end(),
              [](const std::string& a, const std::string& b) {
                  return a + b > b + a;
              });

    // Concatenate all strings.
    std::string result;
    for (const auto& s : sorted) {
        result += s;
    }

    // Handle the case where all numbers are zero.
    if (!result.empty() && result[0] == '0') {
        return "0";
    }

    return result;
}

// The core idea is to define a custom comparator for sorting the strings based on the concatenation result. Specifically, for two strings `a` and `b`, we consider `a+b` versus `b+a`. If `a+b` is lexicographically greater than `b+a`, then `a` should come before `b` in the final concatenation. This works because comparing concatenated strings is equivalent to comparing the actual numeric values when the strings have the same length (after padding), and it correctly handles cases like `"9"` vs `"98"` (since `"998"` > `"989"`). Use `std::sort` with this custom comparator. After sorting, concatenate all strings in order. An important edge case is when the largest number is zero (e.g., input `{"0", "0"}`): after sorting, the first string might be `"0"`, and concatenating all zeros yields `"00"`, which should be normalized to `"0"`. So after building the result, if the first character is `'0'`, return `"0"` instead. Time complexity is O(n log n) due to sorting, where each comparison costs O(L) where L is the total length of the two strings (max 20). Space complexity is O(1) auxiliary beyond the input and output.
