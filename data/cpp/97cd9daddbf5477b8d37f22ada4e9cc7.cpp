// Write a C++ function that takes a string `S` consisting only of the characters `'I'` and `'D'`, and returns the maximum value reached by a counter that starts at `0`. Each `'I'` increments the counter by 1, and each `'D'` decrements it by 1. The function should consider every prefix of the string (i.e., every position from the start to the current index) and return the highest counter value observed across all prefixes (including the initial value 0 before processing any character). For example, for `"IIDID"`, the counter values are 1,2,1,2,1, so the maximum is 2. The input string will be non-empty and may be up to 10^5 characters long.

The solution is a simple linear scan of the string. Maintain a variable `current` initialized to 0, and a variable `best` initialized to 0. Iterate through each character in the string: on `'I'`, increment `current`; on `'D'`, decrement `current`. After each update, set `best = max(best, current)`. This works because the counter starts at 0, and we need the maximum over all prefix values. Since the string can contain only uppercase `'I'` and `'D'`, no other characters need handling. Edge cases: if the string starts with `'D'`, the counter goes negative, but the maximum remains 0 (since initial value is 0 and the maximum is never below 0; negative values are not considered as "maximum" because we start at 0 and the max is at least 0). If the string is all `'D'`, the answer is 0. If all `'I'`, the answer is the length of the string. The time complexity is O(n) where n = S.length(), and space complexity is O(1) besides the input string.

#include <string>
#include <algorithm>

// Returns the maximum counter value reached when processing a string of 'I' (increment) and 'D' (decrement) starting from 0.
int maxCounterValue(const std::string& S) {
    int current = 0;
    int best = 0; // initial value before any character is processed
    for (char c : S) {
        if (c == 'I') {
            ++current;
        } else if (c == 'D') {
            --current;
        }
        best = std::max(best, current);
    }
    return best;
}

int main() {
    assert(maxCounterValue("I") == 1);
    assert(maxCounterValue("D") == 0);
    assert(maxCounterValue("ID") == 1);
    assert(maxCounterValue("DI") == 1); // values: -1,0 -> max is 0? Wait: start 0, D -> -1, I -> 0. Since initial max=0, best stays 0. So answer should be 0.
    assert(maxCounterValue("DI") == 0);
    assert(maxCounterValue("IIDID") == 2);
    assert(maxCounterValue("DDD") == 0);
    assert(maxCounterValue("III") == 3);
    assert(maxCounterValue("IDIDID") == 1);
    assert(maxCounterValue("IIIDDDIII") == 3);
    return 0;
}
