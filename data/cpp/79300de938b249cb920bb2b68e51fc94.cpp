// Write a C++ function `numberToWords(int num)` that converts a non-negative integer into its English words representation. The function must handle numbers from 0 to 2,147,483,647 (inclusive). The output must be in standard American English format: use "Hundred" for hundreds place, and "Thousand", "Million", "Billion" for larger groups. Words are separated by single spaces, with no leading or trailing spaces. The number 0 must produce "Zero". For numbers like 100, output "One Hundred" (no trailing space). For numbers like 1000000, output "One Million". The function must be case-sensitive (first letter of each word capitalized). Use a free function (not a class method) named `numberToWords`. The function should be `const`-correct, and you may use any standard library containers or helper functions.
#include <cassert>
#include <string>

int main() {
    // Basic numbers
    assert(numberToWords(0) == "Zero");
    assert(numberToWords(1) == "One");
    assert(numberToWords(9) == "Nine");
    
    // Teens
    assert(numberToWords(10) == "Ten");
    assert(numberToWords(11) == "Eleven");
    assert(numberToWords(19) == "Nineteen");
    
    // Tens and combined
    assert(numberToWords(20) == "Twenty");
    assert(numberToWords(21) == "Twenty One");
    assert(numberToWords(99) == "Ninety Nine");
    
    // Hundreds
    assert(numberToWords(100) == "One Hundred");
    assert(numberToWords(101) == "One Hundred One");
    assert(numberToWords(110) == "One Hundred Ten");
    assert(numberToWords(999) == "Nine Hundred Ninety Nine");
    
    // Thousands
    assert(numberToWords(1000) == "One Thousand");
    assert(numberToWords(1001) == "One Thousand One");
    assert(numberToWords(1234) == "One Thousand Two Hundred Thirty Four");
    
    // Millions and Billions
    assert(numberToWords(1000000) == "One Million");
    assert(numberToWords(1000001) == "One Million One");
    assert(numberToWords(1234567) == "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven");
    assert(numberToWords(1000000000) == "One Billion");
    assert(numberToWords(2147483647) == "Two Billion One Hundred Forty Seven Million Four Hundred Eighty Three Thousand Six Hundred Forty Seven");
    
    // Edge cases: trailing zeros in groups
    assert(numberToWords(2000000) == "Two Million");
    assert(numberToWords(1000000100) == "One Billion One Hundred");
    
    return 0;
}
#include <string>
#include <unordered_map>
#include <vector>

// Convert a non-negative integer to its English words representation.
// Handles 0 to 2,147,483,647. Case-sensitive with first letter capitalized.
std::string numberToWords(int num) {
    static const std::unordered_map<int, std::string> ones = {
        {1, "One"}, {2, "Two"}, {3, "Three"}, {4, "Four"}, {5, "Five"},
        {6, "Six"}, {7, "Seven"}, {8, "Eight"}, {9, "Nine"}
    };
    static const std::unordered_map<int, std::string> teens = {
        {10, "Ten"}, {11, "Eleven"}, {12, "Twelve"}, {13, "Thirteen"},
        {14, "Fourteen"}, {15, "Fifteen"}, {16, "Sixteen"}, {17, "Seventeen"},
        {18, "Eighteen"}, {19, "Nineteen"}
    };
    static const std::unordered_map<int, std::string> tens = {
        {2, "Twenty"}, {3, "Thirty"}, {4, "Forty"}, {5, "Fifty"},
        {6, "Sixty"}, {7, "Seventy"}, {8, "Eighty"}, {9, "Ninety"}
    };

    // Helper to convert a number 0-999 to words.
    std::function<std::string(int)> threeDigits = [&](int n) -> std::string {
        std::string result;
        if (n >= 100) {
            result += ones.at(n / 100) + " Hundred";
            n %= 100;
            if (n) result += " ";
        }
        if (n >= 20) {
            result += tens.at(n / 10);
            n %= 10;
            if (n) result += " " + ones.at(n);
        } else if (n >= 10) {
            result += teens.at(n);
        } else if (n >= 1) {
            result += ones.at(n);
        }
        return result;
    };

    if (num == 0) return "Zero";

    std::string result;
    const int groups[4] = {1000000000, 1000000, 1000, 1};
    const std::string scales[4] = {"Billion", "Million", "Thousand", ""};

    int remaining = num;
    for (int i = 0; i < 4; ++i) {
        int group = remaining / groups[i];
        remaining %= groups[i];
        if (group > 0) {
            if (!result.empty()) result += " ";
            result += threeDigits(group);
            if (!scales[i].empty()) result += " " + scales[i];
        }
    }
    return result;
}
// The approach is to split the number into groups of three digits from the right: billions, millions, thousands, and the last three digits. Each group is converted to words using a helper that handles numbers 1–999. The helper uses three lookup tables: ones (1–9), teens (10–19), and tens (20–90). The logic for a three-digit number: if it’s less than 10, use the ones map; if less than 20, use the teens map; if less than 100, combine the tens word with the remainder (recursively); if 100 or more, combine the hundreds word with "Hundred" and the remainder. After each group, append the appropriate scale word (Billion, Million, Thousand) with a trailing space, being careful to avoid extra spaces when skipping zero groups. For the final group, trim any trailing space. Edge cases include: num == 0 → return "Zero"; numbers like 1000 → "One Thousand" (the "ones" group is zero, so skip it); numbers like 1000000 → "One Million"; numbers like 1,000,001 → "One Million One". Time complexity is O(1) since the number of operations is constant (at most 4 groups, each with constant work). Space complexity is O(1) for the maps, and the output string length is bounded.
