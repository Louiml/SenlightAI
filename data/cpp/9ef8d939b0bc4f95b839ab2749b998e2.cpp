Write a C++ function named `findFirstPalindrome` that accepts a vector of strings by const reference and returns the first string in the vector that is a palindrome, i.e., a word that reads the same forward and backward. If no word in the vector is a palindrome, return an empty string. The function must be case-sensitive, consider only letter characters (all input is guaranteed to be alphabetical), and must efficiently check each word without creating copies. Empty strings are considered valid palindromes, so if one appears, it should be returned as the first palindrome. The vector may be empty, in which case return an empty string. Do not modify the input vector.
#include <cassert>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> test1 = {"hello", "world", "level", "code"};
    assert(findFirstPalindrome(test1) == "level");

    std::vector<std::string> test2 = {"abc", "def", "ghi"};
    assert(findFirstPalindrome(test2) == "");

    std::vector<std::string> test3 = {};
    assert(findFirstPalindrome(test3) == "");

    std::vector<std::string> test4 = {"racecar"};
    assert(findFirstPalindrome(test4) == "racecar");

    std::vector<std::string> test5 = {"", "nonempty"};
    assert(findFirstPalindrome(test5) == "");

    std::vector<std::string> test6 = {"a", "bb", "ccc"};
    assert(findFirstPalindrome(test6) == "a");

    std::vector<std::string> test7 = {"not", "a", "palindrome"};
    assert(findFirstPalindrome(test7) == "a");

    std::vector<std::string> test8 = {"AbcdDcba", "pal"};
    assert(findFirstPalindrome(test8) == "pal");

    std::vector<std::string> test9 = {"x", "y", "z"};
    assert(findFirstPalindrome(test9) == "x");

    std::vector<std::string> test10 = {"ab", "ba", ""};
    assert(findFirstPalindrome(test10) == "ab");
}
#include <string>
#include <vector>

// Returns the first palindrome in the given vector of strings, or an empty string if none exists.
std::string findFirstPalindrome(const std::vector<std::string>& words) {
    for (const std::string& word : words) {
        int left = 0;
        int right = static_cast<int>(word.length()) - 1;
        bool isPal = true;
        while (left < right) {
            if (word[left] != word[right]) {
                isPal = false;
                break;
            }
            ++left;
            --right;
        }
        if (isPal) {
            return word;
        }
    }
    return "";
}
// The solution iterates through the vector from the first element to the last. For each word, it checks whether it is a palindrome using a two-pointer technique: one pointer starts at the first character, the other at the last character. While the left pointer is less than the right pointer, compare the characters; if they differ, the word is not a palindrome, and we move to the next word. If all comparisons succeed, the word is a palindrome and is returned immediately. Edge cases include an empty vector (return empty string), an empty string inside the vector (it is a palindrome by definition, so return it), and single‑character strings (always palindromes). For a vector with `n` words and each word having at most `m` characters, the time complexity is `O(n * m)` in the worst case, since each character may be examined at most once per word. The space complexity is `O(1)` extra space beyond the input, as only two integer indices are used per check.
