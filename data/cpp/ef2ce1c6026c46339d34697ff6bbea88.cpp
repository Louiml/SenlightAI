/*
Given a string consisting only of the characters '0' and '1', write a C++ function named `maxBalancedPairs` that takes the string as a parameter and returns an integer representing the maximum number of disjoint pairs that can be formed such that each pair contains exactly one '0' and exactly one '1'. The pairs do not need to be adjacent in the original string; you may select any two characters from different indices, but each character can be used at most once. The function should handle empty strings (returning 0) and strings that contain only one type of character (returning 0). The solution must be efficient for strings of length up to 10^5.
*/
#include <string>
#include <algorithm>

// Count the maximum number of disjoint pairs each containing one '0' and one '1'.
int maxBalancedPairs(const std::string& s) {
    int zeroCount = 0;
    int oneCount = 0;

    for (const char c : s) {
        if (c == '0') {
            ++zeroCount;
        } else if (c == '1') {
            ++oneCount;
        }
    }

    return std::min(zeroCount, oneCount);
}
#include <cassert>
#include <string>

int maxBalancedPairs(const std::string& s);

int main() {
    assert(maxBalancedPairs("") == 0);
    assert(maxBalancedPairs("0") == 0);
    assert(maxBalancedPairs("1") == 0);
    assert(maxBalancedPairs("01") == 1);
    assert(maxBalancedPairs("10") == 1);
    assert(maxBalancedPairs("111000") == 3);
    assert(maxBalancedPairs("101010") == 3);
    assert(maxBalancedPairs("00011") == 2);
    assert(maxBalancedPairs("1111100000") == 5);
    assert(maxBalancedPairs("0") == 0);
    return 0;
}
// The core observation is that each valid pair requires one '0' and one '1'. Therefore, the maximum number of disjoint pairs is limited by the smaller of the two counts. Count the total number of zeros and ones in the string. The answer is simply `min(zeroCount, oneCount)`. This works because we can greedily pair each zero with a distinct one until one type runs out. Edge cases: an empty string has zero pairs; a string with only zeros or only ones has zero pairs; a string with equal counts results in all characters being paired. Time complexity is O(n) for a single pass, and space complexity is O(1) since we only store two counters.
