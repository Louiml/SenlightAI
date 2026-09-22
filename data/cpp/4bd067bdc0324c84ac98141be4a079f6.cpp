/*
Write a C++ function named `hasInfiniteBinaryString` that takes a vector of binary strings (each consisting only of characters '0' and '1') representing forbidden patterns (viruses). The function must return `true` if there exists an infinitely long binary string that does **not** contain any forbidden pattern as a contiguous substring, and `false` otherwise. The patterns may be empty, repeated, or arbitrarily long. The function should handle up to 10,000 patterns with total length up to 1,000,000 characters, and must be efficient.
*/
#include <string>
#include <vector>
#include <queue>

struct TrieNode {
    TrieNode* children[2] = {nullptr, nullptr};
    TrieNode* fail = nullptr;
    bool isBad = false;
    bool visited = false;
    bool inStack = false;
};

// Build the Aho–Corasick automaton for given patterns.
// Returns the root node of the trie. The caller must delete it.
TrieNode* buildAutomaton(const std::vector<std::string>& patterns) {
    TrieNode* root = new TrieNode();
    for (const std::string& pattern : patterns) {
        if (pattern.empty()) {
            // Empty pattern is always a substring; mark root as bad.
            root->isBad = true;
            continue;
        }
        TrieNode* node = root;
        for (char ch : pattern) {
            int idx = ch - '0';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isBad = true; // mark terminal as forbidden
    }

    // Build failure links using BFS.
    std::queue<TrieNode*> q;
    root->fail = nullptr;
    for (int i = 0; i < 2; ++i) {
        if (root->children[i]) {
            root->children[i]->fail = root;
            q.push(root->children[i]);
        } else {
            root->children[i] = root; // shortcut: missing edge points to root itself
        }
    }
    while (!q.empty()) {
        TrieNode* current = q.front();
        q.pop();
        if (current->fail && current->fail->isBad) {
            current->isBad = true;
        }
        for (int i = 0; i < 2; ++i) {
            if (current->children[i]) {
                TrieNode* child = current->children[i];
                TrieNode* fallback = current->fail;
                while (fallback && !fallback->children[i]) {
                    fallback = fallback->fail;
                }
                child->fail = fallback ? fallback->children[i] : root;
                q.push(child);
            } else {
                current->children[i] = current->fail ? current->fail->children[i] : root;
            }
        }
    }
    return root;
}

// DFS to find a cycle reachable from the node that does not include any bad node.
bool dfsForCycle(TrieNode* node) {
    if (node->inStack) return true;  // found a back edge / cycle
    if (node->isBad || node->visited) return false;
    node->inStack = true;
    for (int i = 0; i < 2; ++i) {
        if (dfsForCycle(node->children[i])) {
            node->inStack = false;
            return true;
        }
    }
    node->inStack = false;
    node->visited = true; // mark fully explored, no cycle in its subtree
    return false;
}

// Returns true if there exists an infinitely long binary string avoiding all given patterns.
bool hasInfiniteBinaryString(const std::vector<std::string>& patterns) {
    TrieNode* root = buildAutomaton(patterns);
    // If root itself is bad due to an empty pattern, no infinite string exists.
    bool result = false;
    if (!root->isBad) {
        result = dfsForCycle(root);
    }
    // Cleanup: recursively delete all nodes (simple post-order deletion).
    std::function<void(TrieNode*)> deleteTree = [&](TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 2; ++i) {
            if (node->children[i] && node->children[i] != root) {
                deleteTree(node->children[i]);
            }
        }
        delete node;
    };
    // Due to shortcuts, root appears in children as self-reference; handle carefully.
    // Use a separate cleanup function that avoids double deletion.
    // We'll do a BFS/stack cleanup ignoring self-pointers.
    std::vector<TrieNode*> allNodes;
    std::queue<TrieNode*> cleanupQueue;
    cleanupQueue.push(root);
    while (!cleanupQueue.empty()) {
        TrieNode* cur = cleanupQueue.front();
        cleanupQueue.pop();
        if (cur == root) {
            // Only push actual children (not root itself)
            for (int i = 0; i < 2; ++i) {
                if (cur->children[i] && cur->children[i] != root) {
                    cleanupQueue.push(cur->children[i]);
                }
            }
        } else {
            for (int i = 0; i < 2; ++i) {
                if (cur->children[i] && cur->children[i] != root) {
                    cleanupQueue.push(cur->children[i]);
                }
            }
        }
        allNodes.push_back(cur);
    }
    for (TrieNode* node : allNodes) delete node;
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // No patterns → infinite binary string exists (e.g., "0000...")
    assert(hasInfiniteBinaryString({}) == true);

    // Only pattern "0" → cannot avoid it (any string contains '0' if infinite? Actually we can use all '1's, so true)
    assert(hasInfiniteBinaryString({"0"}) == true);

    // Patterns "0" and "1" → no infinite string possible (every character is forbidden)
    assert(hasInfiniteBinaryString({"0", "1"}) == false);

    // Pattern "01" only → we can use "1111..." (no "01"), so true
    assert(hasInfiniteBinaryString({"01"}) == true);

    // Patterns "00" and "11" → we can alternate "0101...", avoiding both, so true
    assert(hasInfiniteBinaryString({"00", "11"}) == true);

    // Patterns "01" and "10" → "000..." avoids both? "000" contains no "01" or "10", so true
    assert(hasInfiniteBinaryString({"01", "10"}) == true);

    // Patterns "0" and "10" → "1" is allowed, but "1" alone is infinite? "111..." contains "0"? No, so true
    assert(hasInfiniteBinaryString({"0", "10"}) == true);

    // Patterns "0", "10", "11" → only "1" is allowed, but "1" alone is fine → true
    assert(hasInfiniteBinaryString({"0", "10", "11"}) == true);

    // Patterns "00", "01", "10", "11" → all length-2 strings forbidden → no infinite string (every pair bad, but "0" and "1" alone are okay? Actually we can't have any length 2 substring, impossible for infinite string) → false
    assert(hasInfiniteBinaryString({"00", "01", "10", "11"}) == false);

    // Empty pattern forces false
    assert(hasInfiniteBinaryString({""}) == false);

    // Pattern "000" only → infinite string of "1"s avoids it → true
    assert(hasInfiniteBinaryString({"000"}) == true);

    // Pattern "010" only → "111..." avoids it → true
    assert(hasInfiniteBinaryString({"010"}) == true);

    // Pattern "1" only → "000..." avoids it → true
    assert(hasInfiniteBinaryString({"1"}) == true);

    // Pattern "1" and "0" → false as above
    assert(hasInfiniteBinaryString({"1", "0"}) == false);

    // Pattern "111" and "000" → alternate "0101..."? Check each length-3 window: "010" is fine, "101" is fine → true
    assert(hasInfiniteBinaryString({"111", "000"}) == true);

    // Complex case: patterns "01" and "10" and "00" → "111..." works? "111" contains no "01","10","00" → true
    assert(hasInfiniteBinaryString({"01", "10", "00"}) == true);

    // All length-1 and length-2 patterns: {"0","1"} already false
    // Test large repeated pattern: "01" repeated many times
    std::vector<std::string> manyPatterns;
    for (int i = 0; i < 100; ++i) manyPatterns.push_back("01");
    assert(hasInfiniteBinaryString(manyPatterns) == true);

    // Test a pattern that is a substring of another: "0" and "00" → "0" alone covers everything → true (use "1"s)
    assert(hasInfiniteBinaryString({"0", "00"}) == true);

    return 0;
}
// The problem reduces to determining whether there is a cycle reachable from the root in a trie (prefix tree) augmented with failure links (Aho–Corasick automaton) after marking all nodes that correspond to a forbidden pattern or have a failure ancestor that is forbidden. If such a cycle exists, we can follow it indefinitely to generate an infinite string avoiding all patterns. The main steps: (1) Build a trie from all patterns, marking terminal nodes as "bad". (2) Build failure links via BFS; propagate the "bad" flag through failure links. (3) Perform DFS from the root, tracking visited nodes in the current recursion stack; if we revisit a node (a cycle) that is not bad, return true. (4) If no cycle is found, return false. Edge cases: empty patterns (if any, return false immediately because any string contains an empty substring and we cannot avoid it); multiple patterns with shared prefixes; patterns that are substrings of others. Time complexity is O(total pattern length) for building and O(total number of automaton states) for DFS, which is linear. Space complexity is O(total pattern length) for storing the automaton.
