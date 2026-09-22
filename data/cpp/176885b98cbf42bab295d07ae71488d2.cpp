/*
Given a binary string `s` consisting only of characters `'0'` and `'1'`, write a C++ function `minimumSteps` that returns the minimum number of adjacent swaps (swapping neighboring characters) required to move all `'1'` characters to the right side of the string, so that the final string has all `'0'`s before all `'1'`s. Each swap exchanges two adjacent characters. The string length can be up to 100,000, so the solution must be efficient. For example, for `"101"`, one swap of the two middle characters yields `"011"`, so the answer is `1`. For `"100"`, no swaps are needed (already sorted), answer `0`.
*/
#include <string>

// Returns the minimum number of adjacent swaps to move all '1's to the right,
// effectively sorting the string so all '0's appear before all '1's.
long long minimumSteps(const std::string& s) {
    long long ones_seen = 0;
    long long swaps = 0;
    for (char c : s) {
        if (c == '1') {
            ++ones_seen;
        } else { // c == '0'
            swaps += ones_seen;
        }
    }
    return swaps;
}
#include <cassert>
#include <string>

// Function under test
long long minimumSteps(const std::string& s);

int main() {
    assert(minimumSteps("0") == 0);
    assert(minimumSteps("1") == 0);
    assert(minimumSteps("00") == 0);
    assert(minimumSteps("11") == 0);
    assert(minimumSteps("01") == 0);
    assert(minimumSteps("10") == 1);
    assert(minimumSteps("101") == 1);
    assert(minimumSteps("100") == 0);
    assert(minimumSteps("1100") == 0);
    assert(minimumSteps("0011") == 0);
    assert(minimumSteps("1010") == 3);
    assert(minimumSteps("111000") == 0);
    assert(minimumSteps("000111") == 0);
    assert(minimumSteps("101010") == 6);
    assert(minimumSteps("1001") == 2);
    assert(minimumSteps("010101") == 6);
    // Long string test: 50,000 zeros then 50,000 ones, already sorted -> 0
    std::string large = std::string(50000, '0') + std::string(50000, '1');
    assert(minimumSteps(large) == 0);
    // Reversed: 50,000 ones then 50,000 zeros -> each zero crosses all ones
    std::string reversed = std::string(50000, '1') + std::string(50000, '0');
    assert(minimumSteps(reversed) == 50000LL * 50000);
    return 0;
}
// The optimal strategy is to process the string from right to left. Maintain a stack (or simply count) of positions of `'1'`s encountered so far. For each position from right to left, if the current character is `'0'`, we want to swap it with the nearest `'1'` to its right (which we have already seen). The number of swaps needed to move that `'1'` to the current position is exactly the distance between them. We accumulate the sum of these distances. Each `'1'` is moved at most once, so we can process with a stack. Alternatively, a simpler approach: iterate from left to right counting the number of `'1'`s seen so far; whenever we encounter a `'0'`, add the current count of `'1'`s to the answer (because that `'0'` must cross all previous `'1'`s). This works because each swap of adjacent `'0'` and `'1'` moves the `'0'` left across exactly one `'1'`. The total number of such crossings is the sum over each `'0'` of the number of `'1'`s before it. Edge cases: empty string, all zeros, all ones, already sorted. Time complexity: O(n) with O(1) extra space. Space complexity: O(1) auxiliary.
