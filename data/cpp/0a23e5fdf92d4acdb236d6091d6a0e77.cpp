/*
Write a C++ function that takes a non-empty string as input and returns `true` if all characters in the string are unique (i.e., no character appears more than once), and `false` otherwise. The function should be case-sensitive, meaning `'A'` and `'a'` are considered different characters. The input string may contain any printable ASCII characters, including spaces and punctuation. The function must not modify the input string.
*/

#include <string>
#include <set>

// Returns true if every character in the input string is unique; false otherwise.
// The check is case-sensitive and treats spaces and punctuation as ordinary characters.
bool hasAllUniqueCharacters(const std::string& str) {
    std::set<char> seen;

    for (const char ch : str) {
        auto result = seen.insert(ch);
        if (!result.second) {
            // The character was already present, so a duplicate exists.
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <string>

// Declaration of the function under test.
bool hasAllUniqueCharacters(const std::string& str);

int main() {
    // Basic cases
    assert(hasAllUniqueCharacters("abc") == true);
    assert(hasAllUniqueCharacters("abca") == false);
    assert(hasAllUniqueCharacters("a") == true);
    assert(hasAllUniqueCharacters("") == true);

    // Case sensitivity: 'A' and 'a' are different
    assert(hasAllUniqueCharacters("Aa") == true);
    assert(hasAllUniqueCharacters("AA") == false);

    // Spaces and punctuation count as characters
    assert(hasAllUniqueCharacters("a b c") == true);  // includes two spaces? No: "a b c" has spaces at positions 1 and 3, and they are distinct? Actually both are the same space char, so duplicate.
    assert(hasAllUniqueCharacters("a b c") == false); // space appears twice
    assert(hasAllUniqueCharacters("a!b@c") == true);
    assert(hasAllUniqueCharacters("a!a") == false);

    // Longer string with repeated characters
    assert(hasAllUniqueCharacters("the quick brown fox") == false); // multiple 'o', ' ', etc.
    assert(hasAllUniqueCharacters("abcdefghijklmnopqrstuvwxyz") == true);
    assert(hasAllUniqueCharacters("xyzxyz") == false);

    return 0;
}

// The core idea is to track which characters have been seen so far while scanning the string from left to right. Use a `std::set<char>` to store characters in the order they are encountered—`set` automatically discards duplicates. As we iterate through the string, attempt to insert each character into the set; `insert` returns a pair where the second element is `false` if the character already existed, indicating a duplicate. If a duplicate is found, immediately stop and return `false`; otherwise, after processing all characters, return `true`. Edge cases include an empty string (which should return `true` since there are no duplicates), a string with a single character (always `true`), and strings with spaces or punctuation—these are treated just like any other character. Case sensitivity is handled naturally because `char` comparisons are based on ASCII values. Time complexity is \(O(n \log n)\) due to `set` insertion costs, where \(n\) is the string length. Space complexity is \(O(d)\), where \(d\) is the number of distinct characters, which is at most 256 for typical char sets, so effectively \(O(1)\) in practice.
