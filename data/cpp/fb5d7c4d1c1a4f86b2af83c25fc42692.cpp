Write a standalone C++ function that models a simplified binary-string "Individual" from a genetics simulation. Given a non-empty string consisting only of characters `'0'` and `'1'`, the function must return a single integer representing the **length of the longest contiguous run of `'1'` characters** in that string. If there are no `'1'` characters, return `0`. For example, for `"110011101"` the result should be `3` (the run `"111"`), for `"000"` the result should be `0`, and for `"1"` the result should be `1`. The function must be `const`‑correct, take the input string by `const std::string&`, and avoid modifying the input. The implementation must be self‑contained (no external classes) and efficient for strings up to 100,000 characters.

The task reduces to a classic linear‑scan problem: we iterate through the string once, maintaining a counter for the current consecutive `'1'` run and a variable for the maximum run seen so far. At each character, if it is `'1'`, we increment the current counter; otherwise, we reset the counter to zero. After updating the counter, we compare it with the current maximum and update if needed. Edge cases include an empty string (though the task guarantees non‑empty, returning `0` is safe), a string with no `'1'` (the maximum stays `0`), a string with a single `'1'` (maximum becomes `1`), and a string ending with a long run (the final comparison must capture it). The algorithm runs in `O(n)` time and uses `O(1)` auxiliary space, where `n` is the string length.

#include <string>

// Return the length of the longest contiguous run of '1' characters in the input.
int longestOnesRun(const std::string& binaryString) {
    int currentRun = 0;
    int maxRun = 0;

    for (char ch : binaryString) {
        if (ch == '1') {
            ++currentRun;
            if (currentRun > maxRun) {
                maxRun = currentRun;
            }
        } else {
            currentRun = 0;
        }
    }

    return maxRun;
}

#include <cassert>

int main() {
    assert(longestOnesRun("1") == 1);
    assert(longestOnesRun("0") == 0);
    assert(longestOnesRun("00") == 0);
    assert(longestOnesRun("11") == 2);
    assert(longestOnesRun("110011101") == 3);
    assert(longestOnesRun("111000111") == 3);
    assert(longestOnesRun("1010101") == 1);
    assert(longestOnesRun("000111000") == 3);
    assert(longestOnesRun("1111111111") == 10);
    assert(longestOnesRun("0101010101") == 1);
    return 0;
}
