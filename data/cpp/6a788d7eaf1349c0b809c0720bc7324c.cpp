/*
Write a C++ function named `isChatAllowed` that takes a constant reference to a string `s` containing only lowercase English letters. The function should determine whether the number of distinct characters in the string is even. If the count of distinct letters is even, return `true` (meaning "CHAT WITH HER!"), otherwise return `false` (meaning "IGNORE HIM!"). The input string may be empty, contain repeated letters, or contain all 26 letters. The function must not modify the input string and should use only a fixed-size boolean or integer array for tracking seen characters.
*/
#include <string>
#include <vector>

// Returns true if the number of distinct lowercase letters in s is even.
bool isChatAllowed(const std::string& s) {
    std::vector<int> seen(26, 0);
    
    for (char ch : s) {
        // Convert character to index 0-25 for 'a'-'z'
        int index = ch - 'a';
        seen[index] = 1;
    }
    
    int distinctCount = 0;
    for (int count : seen) {
        distinctCount += count;
    }
    
    return (distinctCount % 2 == 0);
}
#include <cassert>
#include <string>

// Declaration of the function (assumed to be in the same translation unit)
bool isChatAllowed(const std::string& s);

int main() {
    // Empty string: 0 distinct letters (even) -> true
    assert(isChatAllowed("") == true);
    
    // "a": 1 distinct letter (odd) -> false
    assert(isChatAllowed("a") == false);
    
    // "aa": 1 distinct letter (odd) -> false
    assert(isChatAllowed("aa") == false);
    
    // "ab": 2 distinct letters (even) -> true
    assert(isChatAllowed("ab") == true);
    
    // "abcabc": 3 distinct letters (odd) -> false
    assert(isChatAllowed("abcabc") == false);
    
    // "zzz": 1 distinct letter (odd) -> false
    assert(isChatAllowed("zzz") == false);
    
    // "abcdefghijklmnopqrstuvwxyz": 26 distinct (even) -> true
    assert(isChatAllowed("abcdefghijklmnopqrstuvwxyz") == true);
    
    // "hello": letters h,e,l,o -> 4 distinct (even) -> true
    assert(isChatAllowed("hello") == true);
    
    // "world": w,o,r,l,d -> 5 distinct (odd) -> false
    assert(isChatAllowed("world") == false);
    
    // "aabbccddeeff": a,b,c,d,e,f -> 6 distinct (even) -> true
    assert(isChatAllowed("aabbccddeeff") == true);
    
    return 0;
}
// The core idea is to track which of the 26 lowercase letters appear in the string. Since the alphabet is fixed and small, we can use an integer array of size 26 initialized to zero. For each character in the string, convert it to an index by subtracting the ASCII value of `'a'` (97), then set the corresponding array element to 1 (or true) to mark it as seen. After scanning the entire string, sum all elements in the array to get the count of distinct letters. If this sum is even, return `true`; otherwise, return `false`. Edge cases include: an empty string (sum is 0, which is even, so return `true`), a string with repeated characters (only counted once), and a string containing all 26 letters (sum is 26, even, so return `true`). Time complexity is O(n) where n is the length of the input string, and space complexity is O(1) because the array size is constant (26).
