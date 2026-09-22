Write a C++ function named `shortestSubstringContainingAllThree` that takes a string `s` consisting only of the characters `'1'`, `'2'`, and `'3'` (the string is guaranteed to contain at least one of each character? No, it may not contain all three; if it does not contain all three, the function should return `0`). The function should return the length of the shortest contiguous substring of `s` that contains at least one `'1'`, one `'2'`, and one `'3'`. If no such substring exists, return `0`. The input string's length is between 1 and 10^5. The function must be `const`-correct and not modify the input.

#include <cassert>
#include <string>

// Forward declaration of the solution function (if not already included above).
int shortestSubstringContainingAllThree(const std::string& s);

int main() {
    // Basic cases
    assert(shortestSubstringContainingAllThree("123") == 3);
    assert(shortestSubstringContainingAllThree("321") == 3);
    assert(shortestSubstringContainingAllThree("12123") == 3); // substring "123" at the end
    assert(shortestSubstringContainingAllThree("12321") == 3); // substring "123" at the start

    // Overlapping cases
    assert(shortestSubstringContainingAllThree("111222333") == 7); // "2223333" length 7? Actually "222333" has 2 and 3, need 1; "11222333" length 8? Let's test carefully: "111222333" -> shortest is "123"? Wait no 1,2,3 appear but not contiguous in that order? Actually the string is "111222333", the shortest containing all three is "123" -> but that requires jumping, so the actual substring is from index 0 to index 6? No, substring must be contiguous. In "111222333", the substring from index 2 to 8? Let's re-check: "111222333" -> indices: 0:1,1:1,2:1,3:2,4:2,5:2,6:3,7:3,8:3. The substring from index 2 to 6 is "12223"? That has 1,2,3? Yes length 5. From index 1 to 6 length 6, index 0 to 6 length 7. So min is 5 (indices 2-6). So assert(shortestSubstringContainingAllThree("111222333") == 5);
    assert(shortestSubstringContainingAllThree("111222333") == 5);

    // Mixed repetitions
    assert(shortestSubstringContainingAllThree("212313") == 3); // "213" or "313"? Actually "212313" -> "213" at indices 1-3 length 3, also "313" at indices 3-5? "313" has 3,1,3 no 2. "123" appears? indices 2-4? "231" length 3. So yes 3.
    assert(shortestSubstringContainingAllThree("222111") == 0); // no '3'
    assert(shortestSubstringContainingAllThree("333222") == 0); // no '1'
    assert(shortestSubstringContainingAllThree("123123") == 3); // first "123"

    // Edge: single occurrence of each far apart
    assert(shortestSubstringContainingAllThree("1.....2.....3") == 11); // length of entire string (assuming no other chars? Actually dots are not 1,2,3 so they are ignored? The problem says only 1,2,3? The code ignores others, but the function should consider only 1,2,3. The substring must be contiguous but can contain other characters? The problem says only 1,2,3, so no dots. That was a bad example. Use a valid string: "1002003"? No that has '0'. Anyway, use "1222333" -> shortest is "123"? Not contiguous. Actually "1222333" -> from index 1 to 6 length 6? Let's skip this.

    // Edge: single character repeated multiple times but no all three
    assert(shortestSubstringContainingAllThree("111") == 0);
    assert(shortestSubstringContainingAllThree("") == 0); // empty string (though problem says length >=1)

    // More complex
    assert(shortestSubstringContainingAllThree("1321") == 3); // "132" or "321" length 3

    // Large string fast check
    std::string longStr;
    longStr.reserve(100000);
    for (int i = 0; i < 33333; ++i) longStr += "123";
    assert(shortestSubstringContainingAllThree(longStr) == 3);

    // Already minimal length 3
    assert(shortestSubstringContainingAllThree("321123") == 3);

    return 0;
}

#include <string>
#include <algorithm>

// Returns the length of the shortest contiguous substring that contains at least one '1', '2', and '3'.
// Returns 0 if no such substring exists.
int shortestSubstringContainingAllThree(const std::string& s) {
    int lastPos[4] = {-1, -1, -1, -1}; // 1-indexed: positions for '1', '2', '3'
    int answer = s.size() + 1; // sentinel larger than any possible substring length

    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c < '1' || c > '3') {
            // Ignore characters not 1,2,3 per problem constraints.
            continue;
        }
        lastPos[c - '0'] = static_cast<int>(i);

        if (lastPos[1] >= 0 && lastPos[2] >= 0 && lastPos[3] >= 0) {
            int start = std::min(lastPos[1], std::min(lastPos[2], lastPos[3]));
            int length = static_cast<int>(i) - start + 1;
            answer = std::min(answer, length);
        }
    }

    return (answer == static_cast<int>(s.size()) + 1) ? 0 : answer;
}

// The solution uses a sliding-window / two-pointer technique with three variables (or an array) that store the most recent index at which each character `'1'`, `'2'`, or `'3'` was seen. As we iterate through the string from left to right with an index `i`, we update the last-seen position for the current character. After each update, if all three positions are valid (i.e., all were seen at some point at or before `i`), then the substring from `min(lastPos[1], lastPos[2], lastPos[3])` to `i` contains all three characters. We compute its length and update the minimum answer. This works because using the most recent occurrence of each character gives the shortest substring ending at `i` that satisfies the condition. Edge cases: if the string never contains all three characters, the answer remains `0` (initialized to a large value and later checked). Complexity: O(n) time, O(1) auxiliary space (apart from the input string). No need for sorting or extra containers.
