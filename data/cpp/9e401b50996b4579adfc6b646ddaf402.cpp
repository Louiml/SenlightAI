// Given an integer `k` (1 ≤ k ≤ 26) and `n` (1 ≤ n ≤ 10^5) strings consisting only of lowercase English letters, where each string may be up to 10^5 characters long, write a C++ function `std::string smallestAlphabetOrder(int n, int k, const std::vector<std::string>& words)` that returns a string representing a valid ordering of the first `k` lowercase letters (i.e., 'a' through the `k`-th letter) such that for every consecutive pair of input words, the first word is lexicographically smaller than or equal to the second word under that ordering. If no such ordering exists, return the empty string. If multiple valid orderings exist, return the lexicographically smallest one (where 'a' < 'b' < ...). The ordering must be a permutation of exactly the first `k` letters.

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it).

int main() {
    // Basic valid ordering: a before b, b before c
    {
        std::vector<std::string> words = {"ab", "ac"};
        assert(smallestAlphabetOrder(2, 2, words) == "ab");
    }
    // Already sorted lexicographically, any order works, lexicographically smallest is "ab...k"
    {
        std::vector<std::string> words = {"apple", "banana", "cherry"};
        assert(smallestAlphabetOrder(3, 3, words) == "abc");
    }
    // Explicit dependency: "b" must come before "a"
    {
        std::vector<std::string> words = {"ba", "bb"};
        assert(smallestAlphabetOrder(2, 2, words) == "ba");
    }
    // Impossible due to prefix: "a" is prefix of "ab", but "ab" appears before "a"
    {
        std::vector<std::string> words = {"ab", "a"};
        assert(smallestAlphabetOrder(2, 2, words) == "");
    }
    // Cycle: a < b and b < a
    {
        std::vector<std::string> words = {"ab", "aa"}; // actually no cycle? 
        // Better: "b a" and "a b" via multiple pairs
        std::vector<std::string> words2 = {"ba", "ab"};
        assert(smallestAlphabetOrder(2, 2, words2) == "");
    }
    // Only one letter
    {
        std::vector<std::string> words = {"a", "a", "a"};
        assert(smallestAlphabetOrder(3, 1, words) == "a");
    }
    // k larger than needed, but still must output permutation of first k
    {
        std::vector<std::string> words = {"z", "y"}; // but k=26, need all letters
        // Simpler: k=3, words only give z before y? but z,y are beyond first 3? we ignore
        std::vector<std::string> words3 = {"a", "b"};
        assert(smallestAlphabetOrder(2, 3, words3) == "abc");
    }
    // Lexicographically smallest among multiple: a<b, a<c but b and c independent
    {
        std::vector<std::string> words = {"ab", "ac"};
        // a before b and a before c, so a must be first, then b and c in any order, smallest is "abc"
        assert(smallestAlphabetOrder(2, 3, words) == "abc");
    }
    // More complex: dependencies force order
    {
        std::vector<std::string> words = {"bca", "bac", "cab"};
        // b < a from bca vs bac? Actually bca vs bac: first char same, second c vs a => c < a
        // bac vs cab: b < c
        // So b < c < a
        assert(smallestAlphabetOrder(3, 3, words) == "bca");
    }
    // All identical strings
    {
        std::vector<std::string> words = {"same", "same", "same"};
        assert(smallestAlphabetOrder(3, 4, words) == "abcd");
    }
    return 0;
}

#include <bits/stdc++.h>

// Returns a valid lexicographically smallest ordering of first k letters
// such that given words are in non-decreasing lexicographical order.
// Returns empty string if impossible.
std::string smallestAlphabetOrder(int n, int k, const std::vector<std::string>& words) {
    std::vector<std::vector<int>> adj(k);
    std::vector<int> indegree(k, 0);
    
    // Build directed graph from character comparison.
    for (int i = 1; i < n; ++i) {
        const std::string& prev = words[i-1];
        const std::string& curr = words[i];
        size_t len = std::min(prev.size(), curr.size());
        bool found = false;
        for (size_t j = 0; j < len; ++j) {
            if (prev[j] != curr[j]) {
                int u = prev[j] - 'a';
                int v = curr[j] - 'a';
                if (u < k && v < k) {
                    adj[u].push_back(v);
                    ++indegree[v];
                } else if (u >= k || v >= k) {
                    // ignore if letters beyond k are involved? But problem says only first k letters.
                    // Since all inputs are lowercase, we only need first k.
                }
                found = true;
                break;
            }
        }
        if (!found && prev.size() > curr.size()) {
            // prev is longer and prefix of curr, impossible because prev would be larger.
            return "";
        }
    }
    
    // Min-heap for lexicographically smallest topological order.
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < k; ++i) {
        if (indegree[i] == 0) pq.push(i);
    }
    
    std::string result;
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        result.push_back('a' + u);
        for (int v : adj[u]) {
            if (--indegree[v] == 0) {
                pq.push(v);
            }
        }
    }
    
    if ((int)result.size() != k) return ""; // cycle detected
    return result;
}

// The problem reduces to building a directed graph of dependencies between letters. For each consecutive pair of words, find the first position where they differ: if the earlier word has a character `u` and the later word has a character `v` at that differing position, then `u` must come before `v` in the final ordering. If one word is a prefix of the other and the longer word appears after the shorter one, no ordering is possible (since the shorter word would always be strictly smaller). After building the adjacency list and indegree counts for only the first `k` letters, perform Kahn's algorithm (BFS topological sort) using a priority queue (min-heap) to always extract the smallest available letter, ensuring the lexicographically smallest valid ordering. If the number of letters placed in the result is less than `k`, a cycle exists and we return an empty string. Time complexity: O(total length of all strings) for building edges, plus O(k + number of edges) for topological sort, which is O(total word length + k + edges) where edges ≤ n-1. Space complexity: O(k + edges) for the adjacency list and indegree.
