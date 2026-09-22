Implement a C++ function named `findShortestUniquePrefix` that, given a vector of non-empty strings, returns a vector of strings where each string is the shortest unique prefix of the corresponding input string relative to all other strings in the input. A prefix is a leading substring; the entire word itself is allowed as a prefix. If a word is a prefix of another word, the unique prefix must be longer than that prefix. For example, for `{"dog", "doge", "cat"}`, the result should be `{"dog", "doge", "cat"}`, since `"dog"` is a prefix of `"doge"` so `"dog"` cannot be the unique prefix for `"dog"`; it must be `"dog"` itself only if `"dog"` is not a prefix of any other string. But `"dog"` is a prefix of `"doge"`, so for `"dog"` you need to go to `"dog"` (the whole word) and that is unique because no other string has `"dog"` as a prefix except `"doge"` has it but that's fine – wait, the rule: the unique prefix must be a prefix of the word that is not a prefix of any other word? Actually, more precisely: for each word, find the shortest prefix that is a prefix of this word but not a prefix of any other word. The entire word is always a valid prefix if no other word starts with that entire word (except itself). If the word itself is a prefix of another word, then the shortest unique prefix will be longer than that common prefix. For example, `{"a", "ab"}`: for `"a"`, the prefix `"a"` is also a prefix of `"ab"`, so not unique; the only prefix left is `"a"` itself? But that's also a prefix of `"ab"`? Wait, `"a"` is a prefix of `"ab"`, so `"a"` is not unique for `"a"`. But the whole word `"a"` is the only prefix; but it is a prefix of `"ab"` as well, so no unique prefix? Actually the problem says the entire word itself is allowed as a prefix, but if that entire word is also a prefix of another word, then it's not unique. In that case, the word has no unique prefix? But since we must return a prefix, you would return the whole word? The typical definition: you find the shortest prefix that is not a prefix of any other word. If the whole word is a prefix of another word, then you extend beyond the word? But you can't extend beyond the word. So the problem might assume that the answer is always the whole word if no shorter prefix works, even if the whole word is also a prefix of another. Actually that would make "a" and "ab" - for "a", the prefix "a" is also prefix of "ab", so not unique, so you take... whole word "a" is the only candidate and it's not unique. But the problem likely defines that if the whole word is also a prefix of another, you must take the whole word anyway because there's no longer prefix. To avoid contradiction, the task should define that you always return the shortest prefix that is not a prefix of any *other* string; if the whole word is a prefix of another, then you still return the whole word (as the only possible). But that would violate uniqueness. Better to define the problem clearly: For each word, find the shortest prefix such that it is not a prefix of any other word. It is guaranteed that such a prefix exists (which requires that no word is a full prefix of another word). To make it robust, we can state that inputs are such that no word is a prefix of another. For simplicity, we'll assume that. So the task: given a vector of distinct strings where no string is a prefix of another, return for each the shortest prefix that is unique relative to all others. If there is a tie, use the shortest. Use a trie to count prefixes. Provide the solution.

// The solution builds a trie (prefix tree) from all input strings. Each node stores a count of how many strings pass through it. For each string, we traverse its characters from the root, following the path, and at each node we check the count. The first node along the path where the count equals 1 (meaning only this string uses that prefix) is the shortest unique prefix. Since we are guaranteed that no string is a prefix of another, such a node always exists; in the worst case it will be the leaf node (the full string) because that node’s count is exactly 1 (only the string itself ends there). We insert all strings first, incrementing counts along the path, then for each string we walk the trie and find the first node with count 1. Complexity: building the trie takes O(total length of all strings) time and space. Finding prefixes takes O(total length) as well. Worst-case space is O(total length). Edge cases: single word – its full string will be its prefix. Duplicate strings are not allowed as per assumption. The function should handle empty input gracefully (return empty vector). Use `const` correctness.

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Trie node for prefix counting
struct TrieNode {
    int count = 0;
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
};

// Finds shortest unique prefix for each word in a list.
// Assumes input strings are non-empty, distinct, and no string is a prefix of another.
std::vector<std::string> findShortestUniquePrefix(const std::vector<std::string>& words) {
    TrieNode root;
    // Build trie and count prefixes
    for (const std::string& w : words) {
        TrieNode* node = &root;
        for (char c : w) {
            if (!node->children.count(c)) {
                node->children[c] = std::make_unique<TrieNode>();
            }
            node = node->children[c].get();
            node->count++;
        }
    }

    std::vector<std::string> result;
    result.reserve(words.size());

    // Find shortest unique prefix for each word
    for (const std::string& w : words) {
        TrieNode* node = &root;
        std::string prefix;
        for (char c : w) {
            prefix.push_back(c);
            node = node->children[c].get();
            if (node->count == 1) {
                break;
            }
        }
        result.push_back(prefix);
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Function declaration
std::vector<std::string> findShortestUniquePrefix(const std::vector<std::string>& words);

int main() {
    // Basic case
    std::vector<std::string> words1 = {"dog", "cat", "bird"};
    std::vector<std::string> result1 = findShortestUniquePrefix(words1);
    assert(result1 == words1); // every whole word is unique

    // Case where prefixes overlap
    std::vector<std::string> words2 = {"hello", "help", "hero"};
    std::vector<std::string> result2 = findShortestUniquePrefix(words2);
    assert(result2 == std::vector<std::string>({"hel", "help", "her"}));

    // Case with longer words sharing prefix
    std::vector<std::string> words3 = {"apple", "app", "apricot", "banana"};
    std::vector<std::string> result3 = findShortestUniquePrefix(words3);
    assert(result3 == std::vector<std::string>({"apple", "app", "apr", "b"}));

    // Single word
    std::vector<std::string> words4 = {"solo"};
    assert(findShortestUniquePrefix(words4) == std::vector<std::string>({"solo"}));

    // Empty input
    assert(findShortestUniquePrefix({}).empty());

    // Words with identical prefix until last character
    std::vector<std::string> words5 = {"aab", "aac"};
    assert(findShortestUniquePrefix(words5) == std::vector<std::string>({"aab", "aac"}));

    // Mixed lengths
    std::vector<std::string> words6 = {"ab", "ac", "ad"};
    assert(findShortestUniquePrefix(words6) == std::vector<std::string>({"ab", "ac", "ad"}));

    // Longer overlap
    std::vector<std::string> words7 = {"abcde", "abcfg", "abxyz"};
    assert(findShortestUniquePrefix(words7) == std::vector<std::string>({"abcd", "abcf", "abx"}));

    return 0;
}
