Write a C++ function named `countSeniors` that takes a vector of strings, where each string is a passenger detail record in the format `"phone-number gender age seat-number"` (e.g., `"7868190130M7522"`). In this format, the age is always a two-digit number located at indices 11-12 (0-based) of the string. The function should return the number of passengers who are strictly older than 60 years. Assume all input strings are well-formed and at least 13 characters long. The function must be `const` correct and use efficient substring extraction.

The solution iterates over each string in the vector. For each string, we extract the two characters at positions 11 and 12 (i.e., `substr(11,2)`) which represent the age as a two-digit number. We convert this substring to an integer using `std::stoi`, then compare it to 60. If the age is greater than 60, we increment a counter. Edge cases: the input is guaranteed to have at least 13 characters per string, so the substring is always valid; ages are always exactly two digits, so we don't need to handle variable-length ages. The algorithm runs in O(n) time overall, where n is the number of strings, and uses O(1) auxiliary space, not counting the input vector.

#include <string>
#include <vector>
#include <cstddef>

// Counts the number of passengers whose age (extracted from the string at positions 11-12) is > 60.
int countSeniors(const std::vector<std::string>& details) {
    int seniorCount = 0;
    for (const std::string& passengerInfo : details) {
        // Extract the two characters representing the age.
        std::string ageStr = passengerInfo.substr(11, 2);
        int age = std::stoi(ageStr);
        if (age > 60) {
            ++seniorCount;
        }
    }
    return seniorCount;
}

#include <cassert>
#include <string>
#include <vector>

// Forward declaration of the solution function (assume it's in the same file).
int countSeniors(const std::vector<std::string>& details);

int main() {
    std::vector<std::string> test1 = {"7868190130M7522", "5303914400F9211", "9273338290F4010"};
    assert(countSeniors(test1) == 2); // Ages: 75, 92, 40 -> 2 seniors

    std::vector<std::string> test2 = {"1313579440F2036", "2921522980M5644"};
    assert(countSeniors(test2) == 0); // Ages: 20, 56 -> 0 seniors

    std::vector<std::string> test3 = {"1234567890M6101"}; // Age 61
    assert(countSeniors(test3) == 1);

    std::vector<std::string> test4 = {"1234567890F6001"}; // Age 60, not > 60
    assert(countSeniors(test4) == 0);

    std::vector<std::string> test5 = {"1234567890M7510", "1234567890F6010", "1234567890M5900"};
    assert(countSeniors(test5) == 1); // Only the 75-year-old

    std::vector<std::string> test6 = {};
    assert(countSeniors(test6) == 0); // Empty vector

    return 0;
}
