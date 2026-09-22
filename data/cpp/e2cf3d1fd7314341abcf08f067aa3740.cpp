/*
Given a string `s` consisting of lowercase English letters, write a C++ function `std::vector<std::string> allReachableSubstrings(const std::string& s)` that returns a sorted list of all distinct non-empty substrings of `s` that can be formed by starting at any character position in `s`, and then repeatedly moving to the next occurrence (to the right) of any chosen lowercase letter, respecting the following rule: from your current position `i`, you may jump to the smallest index `j > i` such that `s[j]` equals any letter `c` in `'a'`–`'z'`. You may perform zero or more jumps, and each jump must use the *nearest* future occurrence of the chosen letter. Collect every string that can be built along any such path, including the single-character starting strings. Return the sorted list of these strings. If the input string is empty, return an empty vector. The function must be self-contained, handle repeated letters and overlapping possibilities, and produce all distinct strings exactly once in lexicographical order.
*/

#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <utility>

// Build the next occurrence table: for each position i and letter c, next[i][c] gives
// the smallest index j > i with s[j] == c, or -1 if none exists.
std::vector<std::vector<int>> buildNextTable(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<std::vector<int>> next(n, std::vector<int>(26, -1));
    // For each position from right to left, maintain the earliest occurrence of each letter after current.
    std::vector<int> earliest(26, -1);
    for (int i = n - 1; i >= 0; --i) {
        // Copy the current earliest occurrences to position i
        for (int c = 0; c < 26; ++c) {
            next[i][c] = earliest[c];
        }
        // Update earliest for the current character
        earliest[s[i] - 'a'] = i;
    }
    return next;
}

// Main function: returns all distinct non-empty reachable substrings in sorted order.
std::vector<std::string> allReachableSubstrings(const std::string& s) {
    if (s.empty()) return {};
    int n = static_cast<int>(s.size());
    auto next = buildNextTable(s);

    // Determine starting positions: first occurrence of each letter
    std::vector<bool> isFirstOccurrence(26, false);
    std::vector<bool> seenLetter(26, false);
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        if (!seenLetter[c]) {
            isFirstOccurrence[i] = true;
            seenLetter[c] = true;
        }
    }

    // BFS generating all reachable strings
    std::vector<std::string> result;
    std::queue<std::pair<int, std::string>> q;
    for (int i = 0; i < n; ++i) {
        if (isFirstOccurrence[i]) {
            std::string start(1, s[i]);
            q.push({i, start});
        }
    }

    while (!q.empty()) {
        auto [pos, current] = q.front();
        q.pop();
        result.push_back(current);
        // Try to extend with any letter that has a next occurrence after pos
        for (int c = 0; c < 26; ++c) {
            int nxt = next[pos][c];
            if (nxt != -1) {
                q.push({nxt, current + static_cast<char>('a' + c)});
            }
        }
    }

    // Sort and deduplicate
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function declaration is above; include it here.
std::vector<std::vector<int>> buildNextTable(const std::string& s);
std::vector<std::string> allReachableSubstrings(const std::string& s);

int main() {
    // Single character
    {
        std::vector<std::string> ans = allReachableSubstrings("a");
        std::vector<std::string> expected = {"a"};
        assert(ans == expected);
    }
    // Repeated same letter
    {
        std::vector<std::string> ans = allReachableSubstrings("aaa");
        // Start at first 'a' (pos 0). Can jump to next 'a' at 1, then to 2.
        // Possible strings: "a", "aa", "aaa"
        std::vector<std::string> expected = {"a", "aa", "aaa"};
        assert(ans == expected);
    }
    // Two distinct letters
    {
        std::vector<std::string> ans = allReachableSubstrings("ab");
        // Start at 'a' (pos0) -> "a", jump to 'b' at pos1 -> "ab", also start at 'b' -> "b"
        std::vector<std::string> expected = {"a", "ab", "b"};
        assert(ans == expected);
    }
    // Example with all distinct letters
    {
        std::vector<std::string> ans = allReachableSubstrings("abc");
        // "a", "ab", "abc", "ac" (a->c), "b", "bc", "c"
        std::vector<std::string> expected = {"a", "ab", "abc", "ac", "b", "bc", "c"};
        assert(ans == expected);
    }
    // Empty string
    {
        std::vector<std::string> ans = allReachableSubstrings("");
        std::vector<std::string> expected = {};
        assert(ans == expected);
    }
    // String with duplicates but different paths
    {
        std::vector<std::string> ans = allReachableSubstrings("aba");
        // Starts: first 'a' (0), first 'b' (1). 
        // From 0: "a", can jump to 'b' at 1 -> "ab", from 1 jump to 'a' at 2 -> "aba"; also from 0 jump to 'a' at 2 -> "aa"
        // From 1: "b", jump to 'a' at 2 -> "ba"
        // All: "a", "aa", "ab", "aba", "b", "ba"
        std::vector<std::string> expected = {"a", "aa", "ab", "aba", "b", "ba"};
        assert(ans == expected);
    }
    // Ensure sorted order and no duplicates
    {
        std::vector<std::string> ans = allReachableSubstrings("aa");
        std::vector<std::string> expected = {"a", "aa"};
        assert(ans == expected);
    }
    return 0;
}

// The problem can be modeled as a directed graph where each node is a position `i` in the string. From position `i`, for each letter `c` in `'a'`–`'z'`, there is an edge to the smallest index `j > i` such that `s[j] == c`, if such `j` exists. These edges form a DAG because all edges move strictly forward. We then perform a breadth-first search (or any traversal) starting from all positions that are the first occurrence of their respective letters, because starting from a duplicate letter at a later index yields the same set of continuations as starting from the earliest occurrence (since future edges depend only on `i`, and using the earliest occurrence gives at least as many options). We generate all paths from these starts, accumulating the string built so far. Each discovered path corresponds to a valid reachable substring. We collect them in a vector, sort lexicographically, and remove duplicates (though duplicates are naturally avoided if we use a set or ensure each node–string pair is visited once). The key edge case is handling repeated letters: because we only start from the first occurrence of each letter, we avoid redundant exploration. The complexity is O(26 * n) for building the next-occurrence table, and the traversal visits at most O(n * 26) edges, but the number of distinct strings can be exponential in worst case (though bounded by the number of distinct subsequences in a string). In terms of n, the worst-case number of distinct reachable strings is O(2^n) (e.g., all letters distinct), so time complexity is O(n + 26n + k log k) where k is the number of strings, and space is O(n * 26) for the table plus O(k) for the answer.
