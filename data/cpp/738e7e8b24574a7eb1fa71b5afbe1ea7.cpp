Write a C++ function `string alienOrder(const vector<string>& words, int numLetters)` that, given a sorted dictionary of an alien language containing `words.size()` words and using only the first `numLetters` lowercase English letters (e.g., `numLetters = 4` means letters `a`–`d`), returns a string representing any valid topological ordering of the characters in that alien language. The returned string must contain exactly `numLetters` characters, each lowercase letter from `'a'` to the `(numLetters-1)`-th letter, with no spaces, such that the ordering is consistent with all consecutive word pairs in the dictionary. If no valid ordering exists (e.g., due to a cycle or inconsistent constraints), return an empty string. You may assume all words are non‑empty and contain only lowercase letters from the valid alphabet.
// The problem is a classic topological sort applied to a graph of characters. For each consecutive pair of words, find the first differing character; that gives a directed edge from the earlier character to the later one (e.g., if `"baa"` precedes `"abcd"`, then `'b'` must come before `'a'` in the alien order). Build an adjacency list of size `numLetters` for all letters. Then perform Kahn’s algorithm (BFS‑based topological sort) using an indegree array. Initialize the queue with all letters that have indegree zero and process them, reducing indegrees of neighbors. If the number of processed nodes equals `numLetters`, a valid topological order exists; otherwise, there is a cycle. Edge cases: (1) If `numLetters` is 0 or 1, handle trivially; (2) If a word is a prefix of the next word, no edge is added (and this is valid because the shorter word comes first); (3) If a longer word precedes a shorter word that is its prefix (e.g., `"abc"` before `"ab"`), that is invalid → return empty string; (4) The answer may have multiple valid orders, any is acceptable. Time complexity: building the graph takes `O(total characters across all words)` and topological sort takes `O(numLetters + edges)`, where edges ≤ N‑1. Space complexity: `O(numLetters + N)` for adjacency and queue.
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

// Returns a valid topological ordering of the alien alphabet, or an empty string if impossible.
std::string alienOrder(const std::vector<std::string>& words, int numLetters) {
    if (numLetters == 0) return "";
    if (numLetters == 1) return "a";

    std::vector<std::vector<int>> adj(numLetters);
    std::vector<int> inDegree(numLetters, 0);
    bool possible = true;

    // Build graph from consecutive word pairs.
    for (size_t i = 0; i + 1 < words.size() && possible; ++i) {
        const std::string& w1 = words[i];
        const std::string& w2 = words[i + 1];
        size_t minLen = std::min(w1.size(), w2.size());
        size_t pos = 0;
        while (pos < minLen && w1[pos] == w2[pos]) ++pos;
        if (pos < minLen) {
            int u = w1[pos] - 'a';
            int v = w2[pos] - 'a';
            // Avoid duplicate edges (though not strictly necessary).
            if (std::find(adj[u].begin(), adj[u].end(), v) == adj[u].end()) {
                adj[u].push_back(v);
                inDegree[v]++;
            }
        } else if (w1.size() > w2.size()) {
            // w1 is longer and is a prefix of w2 -> invalid order.
            possible = false;
        }
    }

    if (!possible) return "";

    // Kahn's algorithm.
    std::queue<int> q;
    for (int i = 0; i < numLetters; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }

    std::string result;
    result.reserve(numLetters);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(static_cast<char>('a' + u));
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }

    // If we didn't visit all letters, a cycle exists.
    if (static_cast<int>(result.size()) != numLetters) return "";
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Function under test is declared above (alienOrder). This is the test harness.
int main() {
    // Example from prompt: N=5, K=4, dict = ["baa","abcd","abca","cab","cad"]
    std::vector<std::string> dict1 = {"baa", "abcd", "abca", "cab", "cad"};
    std::string order1 = alienOrder(dict1, 4);
    // Valid orders: "bdac" (as given) or "bdca" etc. Check that it contains all letters and respects constraints.
    assert(order1.size() == 4);
    assert(order1.find('a') != std::string::npos);
    assert(order1.find('b') != std::string::npos);
    assert(order1.find('c') != std::string::npos);
    assert(order1.find('d') != std::string::npos);
    // Check key constraints:
    // b before a, d before a, a before c, b before d
    assert(order1.find('b') < order1.find('a'));
    assert(order1.find('d') < order1.find('a'));
    assert(order1.find('a') < order1.find('c'));
    assert(order1.find('b') < order1.find('d'));

    // Example: N=3, K=3, dict = ["caa","aaa","aab"] -> c a b
    std::vector<std::string> dict2 = {"caa", "aaa", "aab"};
    std::string order2 = alienOrder(dict2, 3);
    assert(order2 == "cab"); // Only valid order (c->a->b)

    // Single word, multiple letters: no edges, any order is valid.
    std::vector<std::string> dict3 = {"z"};
    std::string order3 = alienOrder(dict3, 3);
    assert(order3.size() == 3);
    // Must be a permutation of a,b,c
    std::string sorted = order3;
    std::sort(sorted.begin(), sorted.end());
    assert(sorted == "abc");

    // Invalid case: longer word prefix of shorter (impossible order)
    std::vector<std::string> dict4 = {"abc", "ab"};
    assert(alienOrder(dict4, 3) == "");

    // Another valid example: all distinct first letters
    std::vector<std::string> dict5 = {"b", "a"};
    assert(alienOrder(dict5, 2) == "ba"); // b before a

    // Cycle: a->b, b->a
    std::vector<std::string> dict6 = {"ab", "ba"};
    // Consecutive pair gives a->b (from "ab" vs "ba"), but no b->a from pair? Actually second pair doesn't exist. 
    // To force a cycle we need more than one pair: e.g., {"ab", "ba", "aa"}? Let's use {"ab", "b"} gives a->b, then {"b","a"} gives b->a => cycle.
    std::vector<std::string> dict7 = {"ab", "b", "a"};
    assert(alienOrder(dict7, 2) == ""); // cycle

    // Empty dictionary? Not typical, but handles gracefully.
    std::vector<std::string> dict8;
    assert(alienOrder(dict8, 2) == "ab" || alienOrder(dict8, 2) == "ba"); // any order valid

    return 0;
}
