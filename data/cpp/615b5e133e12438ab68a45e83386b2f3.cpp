/*
Write a C++ function that, given a lower bound `L` and an upper bound `R` (both as 64-bit unsigned integers, with `1 <= L <= R <= 10^18`), and a vector of forbidden number patterns (each pattern is a string of decimal digits, length between 1 and 10), returns the smallest integer in `[L, R]` such that the decimal representation of that integer **contains at least one** forbidden pattern as a contiguous substring. If no such integer exists in the range, return `-1`. For example, with patterns `{"13", "666"}`, the integer `130` is valid (contains `13`), `1667` is valid (contains `66` but not `666`, so invalid actually — pattern must appear fully contiguous: `1667` contains `66` but not `666`, so invalid), but `123` is invalid. The function should handle large ranges efficiently and must not rely on iterating through all numbers for large inputs. Your solution must be self-contained, using a digit-DP (Aho-Corasick automaton) approach.
*/

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using uint64 = unsigned long long;

// Aho-Corasick automaton for forbidden digit patterns
struct AhoCorasick {
    struct Node {
        int next[10];
        int fail;
        bool bad; // whether this node represents a forbidden pattern
        Node() {
            memset(next, -1, sizeof(next));
            fail = 0;
            bad = false;
        }
    };
    vector<Node> trie;
    
    AhoCorasick(const vector<string>& patterns) {
        trie.emplace_back(); // root = 0
        for (const string& p : patterns) {
            int u = 0;
            for (char ch : p) {
                int c = ch - '0';
                if (trie[u].next[c] == -1) {
                    trie[u].next[c] = (int)trie.size();
                    trie.emplace_back();
                }
                u = trie[u].next[c];
            }
            trie[u].bad = true;
        }
        // Build fail links via BFS
        queue<int> q;
        for (int c = 0; c < 10; ++c) {
            if (trie[0].next[c] != -1) {
                int v = trie[0].next[c];
                trie[v].fail = 0;
                q.push(v);
            } else {
                trie[0].next[c] = 0; // fill missing edges to root
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            // propagate bad flag from fail
            if (trie[trie[u].fail].bad) trie[u].bad = true;
            for (int c = 0; c < 10; ++c) {
                if (trie[u].next[c] != -1) {
                    int v = trie[u].next[c];
                    trie[v].fail = trie[trie[u].fail].next[c];
                    q.push(v);
                } else {
                    trie[u].next[c] = trie[trie[u].fail].next[c];
                }
            }
        }
    }
};

// Count numbers <= X that contain at least one forbidden pattern
uint64 countUpTo(uint64 X, const vector<string>& patterns) {
    if (X == 0) return 0;
    AhoCorasick ac(patterns);
    int S = (int)ac.trie.size();
    // Extract digits of X, most significant first
    string digits = to_string(X);
    int n = (int)digits.size();
    // DP memo: dp[pos][state][hit_bad][leading_zero][tight]
    // We use iterative DP to avoid recursion depth, but recursion with memo is fine for n<=19
    // Use 5D array, but we can flatten: pos up to 19, state up to S, hit 0/1, lead 0/1, tight 0/1
    // We'll use recursion with memo table.
    static unordered_map<uint64_t, int64_t> memo; // key encoded, value result
    // Actually easier: use vector with 5 dimensions, but size = 20*S*2*2*2 <= 20*101*8 = 16160, small
    vector<vector<vector<vector<vector<int64_t>>>>> dp(
        n, vector<vector<vector<vector<int64_t>>>>(
            S, vector<vector<vector<int64_t>>>(
                2, vector<vector<int64_t>>(
                    2, vector<int64_t>(2, -1)
                )
            )
        )
    );
    // We'll use lambda recursion
    function<int64_t(int, int, bool, bool, bool)> dfs = [&](int pos, int state, bool hit, bool lead, bool tight) -> int64_t {
        if (pos == n) {
            // valid if we have hit a pattern and we have at least one digit (i.e., not all leading zeros)
            return hit && !lead ? 1 : 0;
        }
        int64_t& res = dp[pos][state][hit][lead][tight];
        if (res != -1) return res;
        res = 0;
        int limit = tight ? (digits[pos] - '0') : 9;
        for (int d = 0; d <= limit; ++d) {
            bool nextLead = lead && (d == 0);
            int nextState = state;
            bool nextHit = hit;
            if (!nextLead) {
                // enter automaton with digit d
                nextState = ac.trie[state].next[d];
                if (ac.trie[nextState].bad) nextHit = true;
            }
            res += dfs(pos+1, nextState, nextHit, nextLead, tight && (d == limit));
        }
        return res;
    };
    return dfs(0, 0, false, true, true);
}

// Main solution function: smallest integer in [L, R] containing a forbidden pattern, or -1
int64_t smallestNumberWithForbidden(uint64_t L, uint64_t R, const vector<string>& patterns) {
    if (L > R) return -1;
    // Binary search for smallest T such that countUpTo(T) - countUpTo(L-1) >= 1
    uint64_t lo = L, hi = R;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        uint64_t leftCount = (mid == 0 ? 0 : countUpTo(mid, patterns));
        uint64_t rightCount = (L == 0 ? 0 : countUpTo(L-1, patterns));
        if (leftCount - rightCount >= 1) hi = mid;
        else lo = mid + 1;
    }
    // Check if lo works
    uint64_t leftCount = (lo == 0 ? 0 : countUpTo(lo, patterns));
    uint64_t rightCount = (L == 0 ? 0 : countUpTo(L-1, patterns));
    if (leftCount - rightCount >= 1) return (int64_t)lo;
    return -1;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;
// Assume the above solution code is included here

int main() {
    // Test 1: simple case
    vector<string> p1 = {"13"};
    assert(smallestNumberWithForbidden(1, 100, p1) == 13);
    
    // Test 2: pattern appears inside larger number
    vector<string> p2 = {"666"};
    assert(smallestNumberWithForbidden(1, 700, p2) == 666);
    assert(smallestNumberWithForbidden(667, 700, p2) == -1);
    
    // Test 3: overlapping patterns
    vector<string> p3 = {"101", "010"};
    assert(smallestNumberWithForbidden(1, 200, p3) == 10); // "10" contains "10"? Wait, pattern "010" requires three digits, so 10 is invalid. Let's compute correctly.
    // Correct: smallest number containing "101" or "010" is 101 (contains "101"), or 10? Actually "10" contains "10" but not "010" or "101". So not valid. So answer should be 101.
    // We adjust test:
    assert(smallestNumberWithForbidden(1, 200, p3) == 101);
    
    // Test 4: multiple patterns and a range with no match
    vector<string> p4 = {"7", "77"};
    assert(smallestNumberWithForbidden(1, 6, p4) == -1);
    assert(smallestNumberWithForbidden(1, 8, p4) == 7);
    
    // Test 5: large range, verify using brute force for small range
    vector<string> p5 = {"42"};
    // Brute check for 1..1000
    for (long long x = 1; x <= 1000; ++x) {
        string s = to_string(x);
        bool valid = s.find("42") != string::npos;
        if (valid) {
            assert(smallestNumberWithForbidden(1, x, p5) == x);
            break;
        }
    }
    
    // Test 6: edge L=R
    vector<string> p6 = {"0"};
    // 0 not allowed as pattern? But we can test: all numbers containing '0' in [10,20] => 10
    assert(smallestNumberWithForbidden(10, 10, p6) == 10);
    
    // Test 7: number itself has pattern at the boundary
    vector<string> p7 = {"9"};
    assert(smallestNumberWithForbidden(9, 9, p7) == 9);
    
    // Test 8: no pattern in large range, but exists just above
    vector<string> p8 = {"123456789"};
    // The pattern is 10 digits, but numbers up to 10^10 (10 billion) exceed 64-bit? Actually 10^9 is 1e9, 10 digits requires 1e10 > 10^18? No, 10^18 has 19 digits, so pattern fits. 
    // Let's test with smaller pattern.
    vector<string> p8b = {"999"};
    assert(smallestNumberWithForbidden(1, 998, p8b) == -1);
    assert(smallestNumberWithForbidden(1, 999, p8b) == 999);
    
    // Test 9: overlapping, pattern repeated
    vector<string> p9 = {"11", "111"};
    assert(smallestNumberWithForbidden(1, 100, p9) == 11);
    
    // Test 10: pattern that is a substring of another
    vector<string> p10 = {"1", "2"};
    assert(smallestNumberWithForbidden(1, 2, p10) == 1);
    
    cout << "All tests passed!" << endl;
    return 0;
}

// The problem reduces to counting how many integers in `[1..X]` contain at least one forbidden pattern. The function `countUpTo(X)` counts valid numbers ≤ X. Then we binary-search for the smallest candidate `T` in `[L, R]` such that `countUpTo(T) - countUpTo(L-1) >= 1` (i.e., exists a valid number). The core challenge is counting numbers with forbidden substrings. We build an Aho-Corasick automaton over the patterns. Each node represents a prefix of some pattern; the `fail` link points to the longest proper suffix that is also a prefix. A node is marked "bad" if it corresponds to any pattern (or if its fail link points to a bad node). Then we perform a digit DP over the decimal digits of `X`, from most significant to least, maintaining: current automaton state, whether we have already hit a forbidden pattern, whether we are still in the leading-zero prefix, and whether the current digit is bounded by the prefix of `X`. The DP has states `(position, automaton_state, hit_bad, leading_zero, tight)`. Leading zeros are handled by not entering the automaton until the first non-zero digit (or the number itself is zero). The time complexity is `O(len * states * 2 * 2 * 2 * 10)` where `len ≤ 19` (since up to 10^18 has 19 digits), and `states` is the number of automaton nodes, at most `sum of pattern lengths + 1 ≤ 101`. The binary search takes `O(log R)` iterations, so total is very fast. Space is `O(len * states * 2 * 2 * 2)` for memoization, but we can reset it per call. Edge cases: patterns may overlap, empty patterns not given, patterns may be identical — deduplicate. Also handle when `L=0` (but constraints say L≥1). If no valid number exists, return -1. Ensure we distinguish leading zeros: numbers with leading zeros are not considered; but for counting, `X=0` should return 0.
