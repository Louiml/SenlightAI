// Write a C++ function named `trieLowestCommonAncestorValue` that operates on a ternary-like trie structure defined by the following `Node` class: each node contains a `std::string value` (possibly empty if no word ends there) and an array `Node* next[26]` initialized to `nullptr`. The function takes a root pointer `Node* root` and two non-empty strings `word1` and `word2` consisting only of lowercase English letters. It must return the string value stored at the deepest node that is an ancestor of both `word1` and `word2` in the trie (i.e., the longest common prefix of the two words, but only if that prefix corresponds to a node that has a non-empty `value`; if no such common-prefix node has a value, return the empty string). The function should not modify the trie, should handle the case where one word is a prefix of the other (the shorter word's ending node counts as an ancestor of the longer word), and must use the trie’s path-following logic similar to a search, without assuming the words exist as complete entries (i.e., handle missing nodes gracefully by stopping the common-prefix traversal). The function must be `const`-correct (take `const Node*` and `const std::string&` parameters).

#include <cassert>
#include <string>

int main() {
    // Build a simple trie manually:
    // root -> 'h' (value "hello") -> 'i' (value "hi")
    // root -> 'c' (value "cat")
    Node* root = new Node();
    root->next['h'-'a'] = new Node();
    root->next['h'-'a']->value = "hello";
    root->next['h'-'a']->next['i'-'a'] = new Node();
    root->next['h'-'a']->next['i'-'a']->value = "hi";
    root->next['c'-'a'] = new Node();
    root->next['c'-'a']->value = "cat";

    // Common prefix "hi": deepest node with value is "hi"
    assert(trieLowestCommonAncestorValue(root, "hi", "him") == "hi");

    // Common prefix "h": deepest node with value is "hello" (since "h" node has value)
    assert(trieLowestCommonAncestorValue(root, "hi", "hello") == "hello");

    // No common prefix after root? Actually both start with 'h', but one is "hi", other "hello", common prefix "h" -> value "hello"
    assert(trieLowestCommonAncestorValue(root, "hi", "h") == "hello");

    // Different first letters: common prefix is empty, root value empty -> ""
    assert(trieLowestCommonAncestorValue(root, "hi", "cat") == "");

    // One word is prefix of the other: "cat" and "cat" -> value at "cat" node
    assert(trieLowestCommonAncestorValue(root, "cat", "cat") == "cat");

    // Path missing deeper: "hello" and "hell" (assuming "hell" not inserted) -> common prefix "hell" but no node for 'l' after 'e'? Actually we only have 'h'->'i', so common prefix 'h' only -> "hello"
    assert(trieLowestCommonAncestorValue(root, "hello", "hell") == "hello");

    delete root->next['h'-'a']->next['i'-'a'];
    delete root->next['h'-'a'];
    delete root->next['c'-'a'];
    delete root;
    return 0;
}

#include <string>
#include <cstddef>

struct Node {
    std::string value;
    Node* next[26];
    Node() : value("") {
        for (int i = 0; i < 26; ++i) next[i] = nullptr;
    }
};

// Return the value at the deepest common-prefix node that has a non-empty value.
// If no such node exists, return an empty string.
std::string trieLowestCommonAncestorValue(const Node* root, const std::string& word1, const std::string& word2) {
    const Node* current = root;
    std::string result = current->value;  // Root's value (usually empty)
    size_t len = word1.size() < word2.size() ? word1.size() : word2.size();

    for (size_t i = 0; i < len; ++i) {
        if (word1[i] != word2[i]) break;  // Divergence point

        int idx = word1[i] - 'a';
        const Node* next = current->next[idx];
        if (next == nullptr) break;  // Path ends

        current = next;
        if (!current->value.empty()) {
            result = current->value;  // Update to the deepest value seen so far
        }
    }
    return result;
}

// The core idea is to traverse both words simultaneously character by character from the root, maintaining a candidate result that is updated only when we reach a node that has a non-empty `value` and is common to both paths. For each position `i`, we compute the index for `word1[i]` and `word2[i]`. If the two characters differ, the common prefix ends, and we break out of the loop. Otherwise, we follow the child pointer from the current node; if that pointer is `nullptr`, the common prefix also ends because the path for at least one word does not exist deeper. If the child exists, we move to it and then check if that node’s `value` is non-empty; if so, we update the candidate to that value (this captures cases where a prefix itself is a complete word). After the loop, return the last candidate value. Edge cases: if the first characters differ, the function returns the root’s value (if non-empty) or empty, but the root usually has an empty value; if one word is a prefix of the other, the traversal proceeds along the shorter word’s full length, and at each node we update the candidate when the node’s value is non-empty, so the final candidate is the value at the shorter word’s end node. Time complexity is O(min(L1, L2)) where L1 and L2 are the lengths of the words, since we traverse only the common prefix. Space complexity is O(1) auxiliary, aside from the recursion stack if implemented iteratively (which we do with a simple loop).
