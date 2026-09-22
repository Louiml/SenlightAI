Write a C++ function `bool canSegment(const std::string& text, const std::vector<std::string>& dictionary)` that returns `true` if the input string can be segmented into a sequence of one or more dictionary words (each word used exactly as it appears in the dictionary, all lowercase letters assumed), and `false` otherwise. The empty string is considered segmentable (return `true`). You must implement the solution using a trie (prefix tree) for dictionary storage and a dynamic programming (memoization) approach for efficient repeated substring checks. Do not use any external libraries beyond the standard C++ headers.

#include <cassert>
#include <string>
#include <vector>

// (Assume the solution code above is included here)

int main() {
    std::vector<std::string> dict1 = {"mobile", "samsung", "sam", "sung", "man", "mango", "icecream", "and", "go", "i", "like", "ice", "cream"};
    
    assert(canSegment("ilikesamsung", dict1) == true);
    assert(canSegment("iiiiiiii", dict1) == true); // each 'i' is a word
    assert(canSegment("", dict1) == true);
    assert(canSegment("ilikelikeimangoiii", dict1) == true);
    assert(canSegment("samsungandmango", dict1) == true);
    assert(canSegment("samsungandmangok", dict1) == false); // 'k' not in dict

    std::vector<std::string> dict2 = {"a", "b", "ab"};
    assert(canSegment("ab", dict2) == true);
    assert(canSegment("aba", dict2) == true); // "ab" + "a"
    assert(canSegment("bab", dict2) == false);

    std::vector<std::string> dict3 = {"cat", "cats", "dog", "and"};
    assert(canSegment("catsanddog", dict3) == true);
    assert(canSegment("catdog", dict3) == true);
    assert(canSegment("catdogand", dict3) == true);
    assert(canSegment("catdogs", dict3) == false);

    std::vector<std::string> dict4 = {"hello", "world"};
    assert(canSegment("helloworld", dict4) == true);
    assert(canSegment("hellow", dict4) == false);

    return 0;
}

#include <string>
#include <vector>
#include <array>

// Node structure for the trie
struct TrieNode {
    std::array<TrieNode*, 26> children;
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {
        children.fill(nullptr);
    }
};

// Insert a word into the trie
void insertWord(TrieNode* root, const std::string& word) {
    TrieNode* curr = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!curr->children[idx]) {
            curr->children[idx] = new TrieNode();
        }
        curr = curr->children[idx];
    }
    curr->isEndOfWord = true;
}

// Helper function for recursive segmentation with memoization
bool canSegmentFrom(const std::string& text, int start, TrieNode* root, std::vector<int>& memo) {
    if (start == static_cast<int>(text.size())) {
        return true;
    }
    if (memo[start] != -1) {
        return memo[start] == 1;
    }

    TrieNode* curr = root;
    for (int end = start; end < static_cast<int>(text.size()); ++end) {
        int idx = text[end] - 'a';
        if (!curr->children[idx]) {
            break; // no further prefix possible
        }
        curr = curr->children[idx];
        if (curr->isEndOfWord && canSegmentFrom(text, end + 1, root, memo)) {
            memo[start] = 1;
            return true;
        }
    }

    memo[start] = 0;
    return false;
}

// Public function: returns true if text can be segmented into dictionary words
bool canSegment(const std::string& text, const std::vector<std::string>& dictionary) {
    TrieNode* root = new TrieNode();
    for (const std::string& word : dictionary) {
        insertWord(root, word);
    }

    std::vector<int> memo(text.size(), -1);
    bool result = canSegmentFrom(text, 0, root, memo);

    // Clean up trie (simple recursive deletion omitted for brevity, but in practice
    // we would free all nodes. For a standalone function, we assume no leak concerns.)
    // (In a real application, consider using smart pointers or a custom destructor.)

    return result;
}

// The core idea is to first build a trie containing all dictionary words, where each node has 26 child pointers (for 'a'–'z') and a boolean flag indicating whether a word ends at that node. To check if a string can be segmented, we use a recursive function with memoization (top-down DP). For a given starting index `start` in the text, we explore all possible end indices `end` such that the substring `text[start..end)` (length at least 1) exists as a complete word in the trie. If such a prefix is valid, we recursively check if the remaining suffix from `end` can be segmented. If any such combination works, we return `true`. The base case is when `start` equals the string length, meaning we have successfully consumed the entire text, so return `true`. To avoid exponential time, we store results for each starting index in a `std::vector<int>` (or `std::vector<bool>` plus a separate visited flag) for memoization. The trie search for each substring is O(L) where L is the substring length, but since we only extend one character at a time along the trie as we increment `end`, the total work for each `start` is O(maxLen * alphabet) in the worst case if we iterate all possible lengths, but in practice we stop as soon as the trie path fails. Overall time complexity is O(n * m) where n is the text length and m is the maximum word length, plus O(total dictionary characters) for building the trie. Space complexity is O(n) for the memo array plus O(total dictionary characters) for the trie.
