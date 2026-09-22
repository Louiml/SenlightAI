// Write a C++ function `canFormRansomNote(const std::string& ransomNote, const std::string& magazine)` that determines whether the string `ransomNote` can be constructed using only letters from the string `magazine`. Each letter in `magazine` may be used at most once, but you may use only lowercase English letters ('a'–'z'). The function should return `true` if `ransomNote` can be formed from `magazine`, and `false` otherwise. The strings may be empty, and if `ransomNote` is empty the function must return `true` regardless of the content of `magazine`. Assume both inputs consist only of lowercase English letters.

The problem reduces to comparing character frequency counts between the two strings. For each of the 26 lowercase letters, we count how many times it appears in `ransomNote` and how many times it appears in `magazine`. If for any letter the count in `ransomNote` exceeds the count in `magazine`, construction is impossible. Otherwise, the function returns `true`. Edge cases include an empty `ransomNote` (always true, since zero letters needed), an empty `magazine` with a non-empty `ransomNote` (returns false unless ransomNote is empty), and cases where `magazine` has all required letters but with extra unused letters (still true). Iterating over both strings takes O(n + m) time where n and m are lengths, and using two fixed-size arrays of size 26 gives O(1) auxiliary space.

#include <string>
#include <vector>

// Returns true if ransomNote can be formed using letters from magazine.
bool canFormRansomNote(const std::string& ransomNote, const std::string& magazine) {
    std::vector<int> required(26, 0);
    std::vector<int> available(26, 0);
    
    for (char c : ransomNote) {
        required[c - 'a']++;
    }
    for (char c : magazine) {
        available[c - 'a']++;
    }
    
    for (int i = 0; i < 26; ++i) {
        if (required[i] > available[i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <string>

// Assume canFormRansomNote is defined above.

int main() {
    // Basic cases
    assert(canFormRansomNote("a", "b") == false);
    assert(canFormRansomNote("aa", "ab") == false);
    assert(canFormRansomNote("aa", "aab") == true);
    
    // Empty ransomNote always true
    assert(canFormRansomNote("", "anything") == true);
    assert(canFormRansomNote("", "") == true);
    
    // Empty magazine with non-empty ransomNote false
    assert(canFormRansomNote("a", "") == false);
    
    // Exact match
    assert(canFormRansomNote("abc", "cba") == true);
    
    // Extra letters in magazine allowed
    assert(canFormRansomNote("code", "coodde") == true);
    
    // All 26 letters, magazine has enough
    std::string allLetters = "abcdefghijklmnopqrstuvwxyz";
    std::string magazineAll = allLetters + "zzyyxxwwvvuuttssrrqqppoonnmmllkkjjiihhggffeeddccbbaa";
    assert(canFormRansomNote(allLetters, magazineAll) == true);
    
    // Missing one letter in magazine
    std::string missingZ = "abcdefghijklmnopqrstuvwxy";
    std::string magazineNoZ = "abcdefghijklmnopqrstuvwxy";
    assert(canFormRansomNote(missingZ + 'z', magazineNoZ) == false);
    
    // Duplicates
    assert(canFormRansomNote("aaaa", "aaaaa") == true);
    assert(canFormRansomNote("aaaa", "aaa") == false);
    
    return 0;
}
