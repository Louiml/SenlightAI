Write a C++ function named `canFormRansomNote` that takes two `std::string` parameters: `ransomNote` and `magazine`. The function should return `true` if the `ransomNote` string can be constructed using the letters from `magazine`, with each letter in `magazine` used at most once, and `false` otherwise. The strings consist only of lowercase English letters ('a'–'z'). The function must be case‑sensitive and treat each character individually, meaning duplicate letters in `ransomNote` require the same number of duplicates in `magazine`. For example, if `ransomNote` is `"aab"` and `magazine` is `"aabc"`, the function should return `true`, but if `magazine` is `"ab"`, it should return `false`.

// The solution uses a frequency counting approach. We first count how many times each of the 26 lowercase letters appears in `magazine` using a fixed‑size array of integers indexed by `char - 'a'`. Then we iterate through `ransomNote`, and for each character, if its current available count in the array is zero, we immediately return `false` because that letter is missing. Otherwise, we decrement that count. If all characters in `ransomNote` are processed without hitting a zero‑count, we return `true`. Edge cases include an empty `ransomNote` (should return `true` because no letters are needed, and the magazine can be empty or not) and an empty `magazine` with a non‑empty `ransomNote` (should return `false` unless `ransomNote` is also empty). The algorithm runs in O(n + m) time, where n and m are the lengths of `ransomNote` and `magazine`, and uses O(1) auxiliary space.

#include <string>
#include <vector>

// Returns true if the ransom note can be built from the letters in the magazine.
bool canFormRansomNote(const std::string& ransomNote, const std::string& magazine) {
    // Frequency table for 26 lowercase letters.
    std::vector<int> available(26, 0);

    // Count each letter in the magazine.
    for (char c : magazine) {
        available[c - 'a']++;
    }

    // Check if the ransom note can be formed.
    for (char c : ransomNote) {
        if (available[c - 'a'] == 0) {
            return false;
        }
        available[c - 'a']--;
    }

    return true;
}

#include <cassert>

int main() {
    // Basic cases
    assert(canFormRansomNote("a", "b") == false);
    assert(canFormRansomNote("aa", "ab") == false);
    assert(canFormRansomNote("aa", "aab") == true);
    assert(canFormRansomNote("", "abc") == true);
    assert(canFormRansomNote("abc", "") == false);

    // Duplicate handling
    assert(canFormRansomNote("aab", "baa") == true);
    assert(canFormRansomNote("aabb", "ab") == false);

    // Full alphabet and long inputs
    assert(canFormRansomNote("abcdefghijklmnopqrstuvwxyz", "zyxwvutsrqponmlkjihgfedcba") == true);
    assert(canFormRansomNote("a".append(100, 'a'), "a".append(99, 'a')) == false);
    assert(canFormRansomNote("a".append(100, 'a'), "a".append(100, 'a')) == true);

    // Case sensitivity (though input is lowercase only, ensure function does not mix)
    assert(canFormRansomNote("A", "a") == false);

    // Same string
    std::string s = "hello";
    assert(canFormRansomNote(s, s) == true);
}
