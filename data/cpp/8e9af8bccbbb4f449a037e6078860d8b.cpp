Write a C++ function named `determineChatOutcome` that takes a single string `s` consisting only of lowercase English letters and returns a string. The function should count how many distinct letters appear in `s`. If the number of distinct letters is even, return `"CHAT WITH HER!"`; otherwise, return `"IGNORE HIM!"`. The input string is guaranteed to be non-empty and contain only lowercase letters. The output must exactly match the given messages.
#include <cassert>
#include <string>
#include "determineChatOutcome.h"  // Assume the function is declared in this header, or include the implementation directly above.

int main() {
    // Example from the snippet: "x" has 1 distinct letter (odd) → IGNORE
    assert(determineChatOutcome("x") == "IGNORE HIM!");
    
    // "ab" has 2 distinct letters (even) → CHAT
    assert(determineChatOutcome("ab") == "CHAT WITH HER!");
    
    // "aaa" has 1 distinct letter → IGNORE
    assert(determineChatOutcome("aaa") == "IGNORE HIM!");
    
    // "hello" has 4 distinct letters (h,e,l,o) → even → CHAT
    assert(determineChatOutcome("hello") == "CHAT WITH HER!");
    
    // "abcabc" has 3 distinct letters (a,b,c) → odd → IGNORE
    assert(determineChatOutcome("abcabc") == "IGNORE HIM!");
    
    // All 26 letters → even → CHAT
    std::string allLetters = "abcdefghijklmnopqrstuvwxyz";
    assert(determineChatOutcome(allLetters) == "CHAT WITH HER!");
    
    // Single repeated varied string: "zyx" has 3 distinct → IGNORE
    assert(determineChatOutcome("zyx") == "IGNORE HIM!");
    
    // Two letters, one repeated three times: "aab" has 2 distinct → CHAT
    assert(determineChatOutcome("aab") == "CHAT WITH HER!");
    
    return 0;
}
#include <string>
#include <vector>

// Determine whether the number of distinct letters in s is even.
std::string determineChatOutcome(const std::string& s) {
    std::vector<int> freq(26, 0);
    for (char c : s) {
        ++freq[c - 'a'];
    }
    
    int distinct = 0;
    for (int count : freq) {
        if (count > 0) {
            ++distinct;
        }
    }
    
    return (distinct % 2 == 0) ? "CHAT WITH HER!" : "IGNORE HIM!";
}
// The solution counts the frequency of each lowercase letter using a fixed-size integer array of length 26 (indexed by `s[i] - 'a'`). After counting, iterate through the array and increment a distinct-count variable for every letter whose frequency is greater than zero (note: characters with zero frequency are skipped, but characters with frequency exactly 1 or greater than 1 both count as one distinct letter). Then check whether the distinct count is even or odd and return the appropriate message. Edge cases: a string with all same letters (e.g., `"aaa"`) has distinct count = 1 (odd → IGNORE), a string with all 26 letters has distinct count = 26 (even → CHAT). Time complexity is O(n) for counting plus O(26) for the check, so O(n) overall. Space complexity is O(1) since the frequency array has constant size.
