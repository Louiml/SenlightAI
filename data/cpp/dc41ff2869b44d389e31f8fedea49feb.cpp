Write a C++ function `minimumRemovalsToAvoidTripleX(const std::string& s)` that takes a string consisting only of lowercase letters 'x' and 'o' (where 'o' represents any non-'x' character) and returns the minimum number of 'x' characters that must be removed so that the resulting string contains no three consecutive 'x' characters. The input will always contain at least one character. You may assume only the characters 'x' and 'o' appear. The function should count how many extra 'x's exist in each maximal consecutive run of 'x's beyond the allowed two per run, and return the total number of removals needed.
#include <cassert>
#include <string>

int minimumRemovalsToAvoidTripleX(const std::string& s);

int main() {
    // No removals needed for short or mixed runs.
    assert(minimumRemovalsToAvoidTripleX("o") == 0);
    assert(minimumRemovalsToAvoidTripleX("x") == 0);
    assert(minimumRemovalsToAvoidTripleX("xx") == 0);
    assert(minimumRemovalsToAvoidTripleX("xox") == 0);
    
    // Need removals for runs of three or more consecutive 'x's.
    assert(minimumRemovalsToAvoidTripleX("xxx") == 1);
    assert(minimumRemovalsToAvoidTripleX("xxxx") == 2);
    assert(minimumRemovalsToAvoidTripleX("xxxxx") == 3);
    
    // Multiple separated runs.
    assert(minimumRemovalsToAvoidTripleX("xxoxxx") == 1);
    assert(minimumRemovalsToAvoidTripleX("xxxoxxxx") == 1 + 2);
    
    // Entire string is x's.
    assert(minimumRemovalsToAvoidTripleX("xxxxxxxx") == 6);
    
    // Edge: one character only.
    assert(minimumRemovalsToAvoidTripleX("x") == 0);
    
    // Mixed with single o separators.
    assert(minimumRemovalsToAvoidTripleX("oxoxoxox") == 0);
    
    // Complex case.
    assert(minimumRemovalsToAvoidTripleX("xxoxxxo") == 1);
    
    return 0;
}
#include <string>
#include <algorithm>

// Given a string of 'x' and 'o' characters, return the minimum number of
// 'x' characters to remove so that no three consecutive 'x' remain.
int minimumRemovalsToAvoidTripleX(const std::string& s) {
    int removals = 0;
    int currentRun = 0;
    
    for (char c : s) {
        if (c == 'x') {
            ++currentRun;
        } else {
            if (currentRun >= 3) {
                removals += currentRun - 2;
            }
            currentRun = 0;
        }
    }
    // Handle a possible trailing run of 'x's.
    if (currentRun >= 3) {
        removals += currentRun - 2;
    }
    
    return removals;
}
// The problem reduces to counting, for every maximal contiguous block of 'x' characters, how many characters exceed the allowed maximum of two per block. For example, a run of length 1 or 2 requires no removals; a run of length 3 requires 1 removal; a run of length 5 requires 3 removals (since we can keep at most 2, we must remove length-2). The algorithm scans the string once, grouping consecutive 'x's. When a non-'x' character is encountered (including at the string’s end), the current run length is processed by adding `max(0, runLength - 2)` to the answer, and the run counter is reset. Edge cases include a string with no 'x' (answer 0), a string entirely of 'x's (apply the same rule to the single run), and runs of length exactly 2 (no removal needed). Time complexity is O(n) where n is the string length, and space complexity is O(1) beyond the input string.
