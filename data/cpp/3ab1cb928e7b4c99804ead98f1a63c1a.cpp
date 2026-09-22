// Write a C++ function named `hasDangerousRun` that takes a string `players` consisting only of the characters `'0'` and `'1'`, representing a sequence of football team players (each character is a different player on the same team), and returns a boolean indicating whether the team is considered "dangerous" — that is, if there are at least 7 identical consecutive characters in the string. The function should handle strings of any length, including empty strings, and should not modify the input. The comparison must be case-sensitive and the character set is strictly binary (`'0'` and `'1'`). The function should be efficient for very long strings, and it must be `const`-correct by taking the input as a `const std::string&`.

#include <cassert>
#include <string>

bool hasDangerousRun(const std::string& players);

int main() {
    assert(hasDangerousRun("0000000") == true);
    assert(hasDangerousRun("1111111") == true);
    assert(hasDangerousRun("000000") == false);
    assert(hasDangerousRun("1111110") == true);
    assert(hasDangerousRun("001001001001001") == false);
    assert(hasDangerousRun("") == false);
    assert(hasDangerousRun("1") == false);
    assert(hasDangerousRun("0000000000") == true);
    assert(hasDangerousRun("1010101010101") == false);
    assert(hasDangerousRun("000000001111111") == true);
    return 0;
}

#include <string>

// Returns true if the string contains at least 7 identical consecutive characters.
bool hasDangerousRun(const std::string& players) {
    if (players.size() < 7) {
        return false;
    }

    int currentRun = 1;
    int maxRun = 1;

    for (size_t i = 1; i < players.size(); ++i) {
        if (players[i] == players[i - 1]) {
            ++currentRun;
            if (currentRun > maxRun) {
                maxRun = currentRun;
            }
        } else {
            currentRun = 1;
        }
    }

    return maxRun >= 7;
}

// The algorithm scans the string once from left to right, keeping track of the current run length of identical consecutive characters. Initialize `currentRun` to 1 (since we start at the first character) and `maxRun` to 1. If the string is empty or has length less than 7, we can immediately return `false` because no run of length 7 or more is possible. For each subsequent character starting at index 1, if it equals the previous character, increment `currentRun` and update `maxRun` if needed. If it differs, reset `currentRun` to 1. After the loop, return `maxRun >= 7`. Edge cases include empty string (return false), all characters identical (e.g., "0000000" returns true), a run ending exactly at 7 (e.g., "11111110" returns true), and a run that goes beyond 7 (e.g., "000000000" returns true). The time complexity is O(n) where n is the length of the string, and space complexity is O(1) auxiliary, since we only use a few integer variables.
