/*
Given a set of \(n\) lowercase strings, then \(T\) query strings, design a C++ function `countPrefixMatches` that for each query counts how many of the \(n\) strings have the query as a prefix. A string \(s\) is considered to have prefix \(p\) if the first \(|p|\) characters of \(s\) equal \(p\). The function should take a vector of the \(n\) strings, and a vector of query strings, and return a vector of integers with the counts in the same order as the queries. The total length of all \(n\) strings and all queries together may be up to \(2 \times 10^5\). All characters are lowercase English letters.
*/
#include <vector>
#include <string>
#include <cassert>

// Count for each query how many of the given strings have that query as a prefix.
std::vector<int> countPrefixMatches(const std::vector<std::string>& strings,
                                    const std::vector<std::string>& queries) {
    // Trie node structure: children for 26 lowercase letters, count of strings that reach this node, and last string ID that updated this node.
    struct TrieNode {
        int child[26];
        int count;
        int lastUsedID;
        TrieNode() {
            for (int i = 0; i < 26; ++i) child[i] = -1;
            count = 0;
            lastUsedID = -1;
        }
    };

    std::vector<TrieNode> nodes(1); // root at index 0

    // Insert all strings into the trie, creating nodes only when necessary.
    for (const std::string& s : strings) {
        int node = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[node].child[idx] == -1) {
                nodes[node].child[idx] = static_cast<int>(nodes.size());
                nodes.emplace_back();
            }
            node = nodes[node].child[idx];
        }
        // No need to mark end-of-string; we only care about prefixes.
    }

    // For each string, traverse its path and increment count at each node, but only once per string per node.
    for (int id = 0; id < static_cast<int>(strings.size()); ++id) {
        const std::string& s = strings[id];
        int node = 0;
        for (char c : s) {
            int idx = c - 'a';
            node = nodes[node].child[idx];
            if (nodes[node].lastUsedID != id) {
                nodes[node].lastUsedID = id;
                ++nodes[node].count;
            }
        }
    }

    // Answer each query.
    std::vector<int> result;
    result.reserve(queries.size());
    for (const std::string& q : queries) {
        if (q.empty()) {
            // Empty query matches all strings.
            result.push_back(static_cast<int>(strings.size()));
            continue;
        }
        int node = 0;
        bool ok = true;
        for (char c : q) {
            int idx = c - 'a';
            if (nodes[node].child[idx] == -1) {
                ok = false;
                break;
            }
            node = nodes[node].child[idx];
        }
        result.push_back(ok ? nodes[node].count : 0);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Declaration of the solution function (as defined above)
std::vector<int> countPrefixMatches(const std::vector<std::string>& strings,
                                    const std::vector<std::string>& queries);

int main() {
    // Example 1: basic
    std::vector<std::string> s1 = {"apple", "app", "apricot", "banana"};
    std::vector<std::string> q1 = {"app", "ap", "ban", "apple", "x"};
    std::vector<int> r1 = countPrefixMatches(s1, q1);
    std::vector<int> e1 = {2, 3, 1, 1, 0};
    assert(r1 == e1);

    // Example 2: duplicate strings
    std::vector<std::string> s2 = {"ab", "ab", "a", "a"};
    std::vector<std::string> q2 = {"a", "ab", "abc"};
    std::vector<int> r2 = countPrefixMatches(s2, q2);
    std::vector<int> e2 = {4, 2, 0};
    assert(r2 == e2);

    // Example 3: empty string in strings (just to test)
    std::vector<std::string> s3 = {"", "a", "ab"};
    std::vector<std::string> q3 = {"", "a", "b"};
    std::vector<int> r3 = countPrefixMatches(s3, q3);
    std::vector<int> e3 = {3, 2, 0};
    assert(r3 == e3);

    // Example 4: all strings share identical prefix
    std::vector<std::string> s4 = {"aaa", "aab", "aac"};
    std::vector<std::string> q4 = {"aa", "a", "aab", "aac", "aad"};
    std::vector<int> r4 = countPrefixMatches(s4, q4);
    std::vector<int> e4 = {3, 3, 1, 1, 0};
    assert(r4 == e4);

    // Example 5: no matches
    std::vector<std::string> s5 = {"xyz"};
    std::vector<std::string> q5 = {"xy", "x", "xyz", "w"};
    std::vector<int> r5 = countPrefixMatches(s5, q5);
    std::vector<int> e5 = {1, 1, 1, 0};
    assert(r5 == e5);

    return 0;
}
// The core idea is to use a compressed trie (or a generalized suffix automaton) to store all \(n\) strings such that each node represents a distinct prefix that appears in at least one string. We insert all strings character by character, ensuring that each distinct prefix is represented only once (i.e., if a prefix is shared, we do not create duplicate nodes). After inserting all strings, we traverse each string again to increment the count at every node along its path, but only once per string per node (i.e., if two strings share a prefix, that node gets incremented twice). To avoid double counting when a string is inserted multiple times, we use a version tag per node. For each query, we walk down the trie following the query's characters; if we successfully reach the node corresponding to the entire query, we return the stored count at that node; otherwise, we return 0. Since the trie is built from all prefixes of all strings, the counting is straightforward. Time complexity: insertion and counting each take \(O(\text{total length of all strings})\), and each query takes \(O(\text{length of query})\). Space complexity: \(O(\text{total number of distinct prefixes across all strings})\). Edge case: an empty query? The problem likely assumes non-empty queries, but if an empty query appears, it should match all strings (count = n). For safety, we can handle that by returning n for empty query.
