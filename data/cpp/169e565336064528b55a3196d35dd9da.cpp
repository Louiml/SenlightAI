Write a C++ function named `determineFriendshipOutcome` that takes a single non-empty string `word` consisting only of lowercase English letters and returns a `std::string` indicating whether the number of distinct characters in the word is even or odd. Specifically, if the count of unique letters is even, return `"CHAT WITH HER!"`; if the count is odd, return `"IGNORE HIM!"`. The function must be `const`-correct and should not modify the input. Assume the input contains at least one character.
#include <cassert>
#include <string>

// Assume the solution function is declared above.
int main() {
    assert(determineFriendshipOutcome("a") == "IGNORE HIM!");
    assert(determineFriendshipOutcome("ab") == "CHAT WITH HER!");
    assert(determineFriendshipOutcome("abc") == "IGNORE HIM!");
    assert(determineFriendshipOutcome("aabb") == "CHAT WITH HER!");
    assert(determineFriendshipOutcome("zzz") == "IGNORE HIM!");
    assert(determineFriendshipOutcome("abcdefghijklmnopqrstuvwxyz") == "CHAT WITH HER!");
    assert(determineFriendshipOutcome("hello") == "IGNORE HIM!");  // h,e,l,o = 4 even
}
#include <string>
#include <set>

// Returns "CHAT WITH HER!" if the input word has an even number of distinct
// characters, otherwise returns "IGNORE HIM!".
std::string determineFriendshipOutcome(const std::string& word) {
    std::set<char> uniqueChars;
    for (char c : word) {
        uniqueChars.insert(c);
    }
    return (uniqueChars.size() % 2 == 0) ? "CHAT WITH HER!" : "IGNORE HIM!";
}
// The core task is to count the number of distinct characters in the given string and decide the output based on parity. The simplest robust approach is to insert every character into a `std::set<char>` because a set automatically discards duplicates and stores only unique elements. After processing all characters, the size of the set equals the number of distinct letters. Then, check if `set.size() % 2 == 0` — if true, return `"CHAT WITH HER!"`, otherwise return `"IGNORE HIM!"`. Edge cases include a single-character string (size 1, odd → ignore), all characters identical (size 1), and strings with all 26 letters (size 26, even → chat). The algorithm runs in \(O(n \log n)\) time due to set insertion, or \(O(n)\) if using an unordered set, but the `std::set` version is simple and adequate. Space complexity is \(O(k)\) where \(k\) is the number of distinct characters, at most 26. Since the input is lowercase letters only, we could also use a boolean array of size 26 for O(1) space, but the set version is cleaner and directly mirrors the snippet.
