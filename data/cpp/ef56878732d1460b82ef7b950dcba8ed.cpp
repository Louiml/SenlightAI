Write a C++ function that takes a vector of strings representing a list of words and two strings `word1` and `word2`, and returns the shortest distance (i.e., the minimum absolute difference in indices) between any occurrence of `word1` and any occurrence of `word2` in the list. The function should handle the case where the words appear multiple times; for example, if `word1` appears twice and `word2` appears once, the shortest distance between any pair of occurrences must be returned. The input vector must not be modified. You may assume `word1` and `word2` are distinct and both appear at least once in the list. Use only standard C++ libraries.
The solution uses a single pass over the vector while tracking the most recent index of each word. We initialize `idx1` and `idx2` to -1 to indicate that neither word has been seen yet. For each element at index `i`, if it matches `word1`, we update `idx1 = i`; if it matches `word2`, we update `idx2 = i`. After each update, if both indices are valid (i.e., not -1), we compute the absolute difference between them and update the running minimum distance. This works because the shortest distance between any two occurrences must involve the most recent occurrence of one word and the most recent occurrence of the other word at the time of the second word's occurrence—any older occurrence would give a larger distance. Edge cases: if the words are adjacent, the distance is 1; if the list has only the two words in any order, the function still works because the first time both indices become set, the distance is computed. The time complexity is O(n) where n is the number of words, and the space complexity is O(1) extra space beyond the input vector (which is passed by const reference to avoid copying).
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <climits>

// Returns the shortest distance between any occurrence of word1 and word2 in the given list.
int shortestWordDistance(const std::vector<std::string>& words, 
                         const std::string& word1, 
                         const std::string& word2) {
    int idx1 = -1, idx2 = -1;
    int distance = INT_MAX;
    for (int i = 0; i < static_cast<int>(words.size()); ++i) {
        if (words[i] == word1) {
            idx1 = i;
            if (idx2 != -1) {
                distance = std::min(distance, std::abs(idx1 - idx2));
            }
        } else if (words[i] == word2) {
            idx2 = i;
            if (idx1 != -1) {
                distance = std::min(distance, std::abs(idx1 - idx2));
            }
        }
    }
    return distance;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> words1 = {"practice", "makes", "perfect", "coding", "makes"};
    assert(shortestWordDistance(words1, "coding", "practice") == 3);
    assert(shortestWordDistance(words1, "makes", "coding") == 1);

    std::vector<std::string> words2 = {"a", "b", "a", "c", "b"};
    assert(shortestWordDistance(words2, "a", "b") == 1); // a at index 2, b at 1 or 4 -> min distance 1
    assert(shortestWordDistance(words2, "a", "c") == 1); // a at 2, c at 3 -> distance 1
    assert(shortestWordDistance(words2, "b", "c") == 1); // b at 4, c at 3 -> distance 1

    std::vector<std::string> words3 = {"x", "y"};
    assert(shortestWordDistance(words3, "x", "y") == 1);

    std::vector<std::string> words4 = {"z", "z", "q", "z"};
    assert(shortestWordDistance(words4, "z", "q") == 1); // q at 2, nearest z at 1 or 3 -> distance 1
}
