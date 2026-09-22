Write a C++ function `countNonOverlappingMatches(const std::string& text, const std::vector<std::string>& patterns)` that returns the maximum number of non-overlapping occurrences of any pattern from the given set that can be found by scanning the text from left to right. At each position in the text, if the current suffix matches any of the patterns (i.e., one of the patterns ends at that position), you may choose to "consume" the match by incrementing the count and moving the scanning position to the character immediately after the match (so matches cannot overlap), or you may skip this match and continue scanning. The goal is to maximize the total number of matches. All patterns and the text are non-empty, consist of lowercase English letters, and the total length of all patterns plus the text length is at most \(2 \times 10^5\). The function must handle cases where multiple patterns end at the same position, and the patterns may be duplicates.

#include <cassert>
#include <vector>
#include <string>

// The solution function is already defined above. This is the test runner.
int main() {
    // Single pattern, multiple occurrences non-overlapping.
    assert(countNonOverlappingMatches("ababab", {"ab"}) == 3);
    // Pattern not found.
    assert(countNonOverlappingMatches("hello", {"xyz"}) == 0);
    // Multiple patterns, choose the one that gives more matches.
    // Since we take earliest match, "a" gives more than "aa".
    assert(countNonOverlappingMatches("aaaa", {"a", "aa"}) == 4);
    // Overlapping patterns: if we take "aa", we can only get 2, but taking "a" gives 4.
    assert(countNonOverlappingMatches("aaaa", {"aa"}) == 2);
    // Pattern that is a suffix of another, earlier end should be taken.
    assert(countNonOverlappingMatches("ababa", {"aba", "ba"}) == 2); // Either "aba" at pos 3 and then "ba"? No, after reset, "ba" at pos 4? Let's compute: text "ababa", first match at "aba" ends at index 2 (0-based), reset, then "ba" ends at index 4? Actually after reset, scanning "ba" from index 3, "ba" ends at index 4, so 2 matches. Alternatively taking "ba" first at index 1, then "ba" at index 3? That gives 2 as well. So answer 2.
    // Empty text not allowed per spec, but test with empty just in case.
    assert(countNonOverlappingMatches("", {"a"}) == 0);
    // Duplicate patterns.
    assert(countNonOverlappingMatches("abcabc", {"abc", "abc"}) == 2);
    // No overlapping when pattern appears overlapping.
    assert(countNonOverlappingMatches("aaa", {"aa"}) == 1); // first "aa" consumes, then only "a" left.
    // Longer text with multiple patterns.
    assert(countNonOverlappingMatches("xyxyxyx", {"xyx", "y"}) == 3); // take "xyx" at 0, then "y" at 3? Actually after reset at index 2, scan "xyxyx" from index 3: "xyx" at index 5? Let's simulate: text "xyxyxyx", take "xyx" at 0-2, reset. Remaining "xyxyx" (indices 3-7). At index 3 'x', transition, at index 4 'y', at index 5 'x' => "xyx" ends at 5, take it. Reset. Remaining "yx" (indices 6-7). At 6 'y' matches pattern "y", take it, reset. Then at 7 'x' no match. Total 3. Correct.
    return 0;
}

#include <string>
#include <vector>
#include <array>
#include <queue>

// Aho-Corasick automaton to detect any pattern ending at a given position.
class AhoCorasick {
    struct Node {
        std::array<int, 26> next;
        int fail;
        bool accept;
        Node() : fail(0), accept(false) {
            next.fill(-1);
        }
    };
    std::vector<Node> trie;

public:
    AhoCorasick(const std::vector<std::string>& patterns) {
        trie.emplace_back(); // root = 0
        for (const auto& p : patterns) {
            int node = 0;
            for (char ch : p) {
                int c = ch - 'a';
                if (trie[node].next[c] == -1) {
                    trie[node].next[c] = static_cast<int>(trie.size());
                    trie.emplace_back();
                }
                node = trie[node].next[c];
            }
            trie[node].accept = true;
        }
        // Build failure links using BFS.
        std::queue<int> q;
        for (int c = 0; c < 26; ++c) {
            if (trie[0].next[c] != -1) {
                int child = trie[0].next[c];
                trie[child].fail = 0;
                q.push(child);
            } else {
                trie[0].next[c] = 0; // make root transition to itself
            }
        }
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            // Propagate accept status through failure links.
            trie[v].accept = trie[v].accept || trie[trie[v].fail].accept;
            for (int c = 0; c < 26; ++c) {
                if (trie[v].next[c] != -1) {
                    int child = trie[v].next[c];
                    trie[child].fail = trie[trie[v].fail].next[c];
                    q.push(child);
                } else {
                    trie[v].next[c] = trie[trie[v].fail].next[c];
                }
            }
        }
    }

    int initial_state() const {
        return 0;
    }

    int next_state(int state, char ch) const {
        return trie[state].next[ch - 'a'];
    }

    bool is_accept(int state) const {
        return trie[state].accept;
    }
};

// Count the maximum number of non-overlapping matches of any pattern in the text.
int countNonOverlappingMatches(const std::string& text, const std::vector<std::string>& patterns) {
    AhoCorasick ac(patterns);
    int ans = 0;
    int state = ac.initial_state();
    for (char c : text) {
        state = ac.next_state(state, c);
        if (ac.is_accept(state)) {
            ++ans;
            state = ac.initial_state();
        }
    }
    return ans;
}

// The problem is a classic greedy/trie-based dynamic programming-like scanning problem. The optimal strategy is to always take the earliest possible match that ends at the current position, because taking a match earlier never reduces the number of future matches (since we are maximizing count, consuming a match as soon as possible frees up later text). However, we must be careful: if multiple patterns end at the same position, taking any one of them yields the same count increment (exactly 1), so we can simply consume any match. The key is to efficiently detect whether any pattern ends at the current position while scanning.
//
// We build an Aho-Corasick automaton from all patterns. The automaton has states representing the longest prefix of any pattern that is a suffix of the processed text. Each state has a failure link to the next longest such prefix. We also precompute a boolean `accept` for each state: true if any pattern ends at that state (i.e., the state corresponds to a pattern or a failure link leads to a pattern). During scanning, we maintain the current state. For each character in the text, we transition to the next state (following failure links as needed). If the new state is `accept`, we increment the answer, reset to the initial state (since we consumed the match and no overlap is allowed), and continue. This works because the longest match ending at the current position is detected by the automaton; resetting ensures no overlap. The algorithm runs in \(O(\text{total length of patterns} + \text{length of text})\) time, because each character transition takes amortized \(O(1)\) due to the automaton's failure links. Space complexity is \(O(\text{total length of patterns} \times \text{alphabet size})\) for the transition table (or we can use a map for memory efficiency, but with constraint \(2 \times 10^5\) and alphabet size 26, a fixed 26-array is fine). Edge cases: empty patterns not allowed (given non-empty), duplicates handled naturally, and patterns that are substrings of each other: the automaton's `accept` flags must propagate through failure links so that if a shorter pattern ends at a state that is a suffix of a longer pattern, the state is also marked accept. This ensures we take a match as early as possible. Since we reset after every accepted match, we never double-count overlapping matches.
