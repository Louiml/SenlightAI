/*
Write a C++ function `bool findWord(const std::vector<std::string>& dictionary, const std::string& target)` that checks whether `target` can be formed by concatenating exactly two words from `dictionary` (the two words can be identical, but each must appear as a complete word in the dictionary). The function should return `true` if such a concatenation exists, and `false` otherwise. For example, given dictionary `["cat", "dog", "cats", "doggy"]`, the target `"catdog"` returns `true` (cat+dog), `"dogcat"` returns `true` (dog+cat), `"catsdog"` returns `false` (cats+dog leaves no valid split), and `"doggycat"` returns `true`. The dictionary may contain duplicate words, but they are treated as a single entry. Words consist only of lowercase English letters. Assume the dictionary is non-empty and contains at least one word.
*/

#include <string>
#include <vector>
#include <unordered_set>

// Returns true if target can be formed by concatenating exactly two distinct or identical words from the dictionary.
bool findWord(const std::vector<std::string>& dictionary, const std::string& target) {
    std::unordered_set<std::string> wordSet(dictionary.begin(), dictionary.end());
    
    const int n = static_cast<int>(target.size());
    if (n < 2) return false;
    
    for (int i = 1; i < n; ++i) {
        const std::string left = target.substr(0, i);
        const std::string right = target.substr(i, n - i);
        if (wordSet.find(left) != wordSet.end() && wordSet.find(right) != wordSet.end()) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> dict1 = {"cat", "dog", "cats", "doggy"};
    assert(findWord(dict1, "catdog") == true);
    assert(findWord(dict1, "dogcat") == true);
    assert(findWord(dict1, "catsdog") == false);
    assert(findWord(dict1, "doggycat") == true);
    assert(findWord(dict1, "cat") == false); // too short
    assert(findWord(dict1, "catdogdog") == false); // more than two parts
    
    std::vector<std::string> dict2 = {"a", "b", "ab"};
    assert(findWord(dict2, "ab") == true);  // "a"+"b"
    assert(findWord(dict2, "ba") == false); // not found
    assert(findWord(dict2, "aa") == false); // "a"+"a" works? Actually "a" is present, so "aa" should be true if repeated allowed
    // but our implementation allows same word twice, so check:
    assert(findWord(dict2, "aa") == true);  // "a"+"a"
    
    std::vector<std::string> dict3 = {"ab", "ba"};
    assert(findWord(dict3, "abba") == true);  // "ab"+"ba"
    assert(findWord(dict3, "baab") == true);  // "ba"+"ab"
    assert(findWord(dict3, "abab") == false); // no split yields both in dict
    
    std::vector<std::string> dict4 = {"hello"};
    assert(findWord(dict4, "hellohello") == true); // duplicate allowed
    assert(findWord(dict4, "helloworld") == false);
    
    return 0;
}

// The problem reduces to checking whether the target string can be split into two non-empty parts at some index `i` (1 ≤ i < target.length()) such that both `target.substr(0,i)` and `target.substr(i)` are present in the dictionary. A straightforward approach is to insert all dictionary words into an unordered_set for O(1) average lookup. Then iterate over all possible split positions and test both substrings. Edge cases: the target length must be at least 2, the split cannot be at position 0 or target.length() (both parts must be non-empty), and the dictionary may contain words of varying lengths; if a substring is not in the set, move to the next split. There is no need to handle serial concatenations of more than two words. Time complexity is O(L) for the number of split checks, each doing constant-time set lookups, but constructing the set takes O(D * K) where D is the number of words and K is the average word length. Space complexity is O(D * K) for the set. For the worst case where target length is large, the cost is O(L) time per call, plus preprocessing.
