/*
Given a list of binary strings of equal length \(d\), each representing a bitmask, write a C++ function `std::vector<char> findMinCommands(int d, const std::vector<std::string>& masksInput)` that returns a minimal-length sequence of commands to visit all given masks. The sequence starts with all bits cleared. Each command is either the character `'R'` (reset all bits to 0) or a digit `'0'` through `'9'` (set that bit to 1). After each command, the current mask may coincide with any of the input masks (including duplicates), and all input masks must be visited at least once. Duplicate masks are considered satisfied at the same state without extra commands. The order of visiting masks is arbitrary, and resets may be used any number of times. You may assume \(1 \le d \le 10\) and that the number of distinct masks is at most 100. Return the empty vector if the only mask is zero. The sequence must be valid: starting from zero, each bit command sets exactly that bit, each reset zeros all bits, and the final state after the last command is the last visited mask.
*/

#include <bits/stdc++.h>
using namespace std;

// Hungarian algorithm for minimum cost assignment on an n x n matrix.
// Returns assignment[i] = j meaning row i is assigned to column j.
static vector<int> hungarian(const vector<vector<int>>& cost) {
    int n = cost.size();
    const int INF = 1e9;
    vector<int> u(n+1), v(n+1), p(n+1), way(n+1);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n+1, INF);
        vector<char> used(n+1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    int cur = cost[i0-1][j-1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vector<int> ans(n);
    for (int j = 1; j <= n; ++j) {
        if (p[j] > 0) ans[p[j]-1] = j-1;
    }
    return ans;
}

// Returns the minimal command sequence to visit all given masks.
vector<char> findMinCommands(int d, const vector<string>& masksInput) {
    // Convert to integer masks and deduplicate.
    unordered_set<int> uniqueSet;
    for (const string& s : masksInput) {
        int mask = 0;
        for (int j = 0; j < d; ++j) {
            if (s[j] == '1') mask |= (1 << j);
        }
        uniqueSet.insert(mask);
    }
    vector<int> masks;
    for (int m : uniqueSet) masks.push_back(m);
    int n = masks.size();
    if (n == 0) return {};

    // Build weight matrix: w[i][j] = 1 + popcount(masks[j]) if i is proper subset of j.
    vector<vector<int>> w(n, vector<int>(n, 0));
    auto popcount = [](int x) { return __builtin_popcount(x); };
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if ((masks[i] & masks[j]) == masks[i]) {
                w[i][j] = 1 + popcount(masks[j]);
            }
        }
    }

    // Convert to cost matrix for Hungarian (maximize weight => minimize cost).
    const int maxW = n * (d + 2) + 1;
    vector<vector<int>> cost(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cost[i][j] = maxW - w[i][j];
        }
    }

    vector<int> assignment = hungarian(cost);

    // Build successor relation from the matching.
    vector<int> succ(n, -1);
    for (int i = 0; i < n; ++i) {
        int j = assignment[i];
        if (w[i][j] > 0) succ[i] = j;
    }

    // Find chain starts: vertices with no predecessor.
    vector<int> pred(n, 0);
    for (int i = 0; i < n; ++i) {
        if (succ[i] != -1) pred[succ[i]]++;
    }
    vector<int> starts;
    for (int i = 0; i < n; ++i) {
        if (pred[i] == 0) starts.push_back(i);
    }

    // Simulate chains to produce commands.
    vector<char> ans;
    bool firstChain = true;
    int currentMask = 0;
    for (int start : starts) {
        if (!firstChain) {
            ans.push_back('R');
            currentMask = 0;
        }
        firstChain = false;
        int v = start;
        while (true) {
            int target = masks[v];
            int delta = target ^ currentMask;
            for (int k = 0; k < d; ++k) {
                if ((delta >> k) & 1) {
                    ans.push_back(char('0' + k));
                }
            }
            currentMask = target;
            if (succ[v] == -1) break;
            v = succ[v];
        }
    }
    return ans;
}

#include <bits/stdc++.h>
using namespace std;

// The solution function is declared above (or linked from the solution code).
vector<char> findMinCommands(int d, const vector<string>& masksInput);

// Helper to verify that a command sequence visits all given masks.
static bool isValid(int d, const vector<string>& input, const vector<char>& seq) {
    unordered_set<int> targets;
    for (const string& s : input) {
        int m = 0;
        for (int j = 0; j < d; ++j) if (s[j] == '1') m |= (1 << j);
        targets.insert(m);
    }
    int cur = 0;
    for (char c : seq) {
        if (c == 'R') cur = 0;
        else cur |= (1 << (c - '0'));
        if (targets.find(cur) != targets.end()) targets.erase(cur);
    }
    return targets.empty();
}

int main() {
    // Test 1: two incomparable masks -> need a reset.
    {
        auto seq = findMinCommands(2, {"01", "10"});
        assert(seq.size() == 3);
        assert(isValid(2, {"01", "10"}, seq));
    }

    // Test 2: chain of three masks -> no reset needed.
    {
        auto seq = findMinCommands(3, {"001", "011", "111"});
        assert(seq.size() == 3);
        assert(isValid(3, {"001", "011", "111"}, seq));
    }

    // Test 3: mixed comparabilities -> minimal length 6.
    {
        auto seq = findMinCommands(3, {"001", "010", "011", "100"});
        assert(seq.size() == 6);
        assert(isValid(3, {"001", "010", "011", "100"}, seq));
    }

    // Test 4: duplicates do not add commands.
    {
        auto seq = findMinCommands(2, {"01", "01", "11"});
        assert(seq.size() == 2);
        assert(isValid(2, {"01", "01", "11"}, seq));
    }

    // Test 5: only zero mask -> empty sequence.
    {
        auto seq = findMinCommands(2, {"00", "00"});
        assert(seq.size() == 0);
    }

    // Test 6: single non-zero mask.
    {
        auto seq = findMinCommands(2, {"10"});
        assert(seq.size() == 1);
        assert(isValid(2, {"10"}, seq));
    }

    // Test 7: all masks of d=3 except zero? Example with 4 masks.
    {
        auto seq = findMinCommands(3, {"001", "010", "011", "100", "101"});
        // Manual reasoning: chains like 001->011, 010->? could be 010->? not comparable with 011? Actually 010 not subset of 011? yes (010 & 011)=010, so 010->011 possible. Then 100->101. So chains: 001->011, 010->011? but 011 used twice? Not allowed. Better: chain1: 001->011, chain2: 010->? maybe 010 has no superset among these except 011 but used. So chain2: 010 alone, chain3: 100->101. Ends: 011(pop2),010(pop1),101(pop2) sum=5, chains=3, resets=2 -> 7. Or chain1: 010->011, chain2: 001 alone, chain3: 100->101: ends 011(2),001(1),101(2)=5, same. So length 7.
        auto seq = findMinCommands(3, {"001", "010", "011", "100", "101"});
        assert(seq.size() == 7);
        assert(isValid(3, {"001", "010", "011", "100", "101"}, seq));
    }

    return 0;
}

// We first convert each string to an integer mask and discard duplicates, since repeating the same mask costs nothing extra. The core observation is that without a reset, we can transition from mask \(A\) to mask \(B\) only if \(A\) is a proper subset of \(B\); the cost in bit commands is \(\operatorname{popcount}(B)-\operatorname{popcount}(A)\). With a reset, we can transition from any mask to any other mask at cost \(1+\operatorname{popcount}(B)\) (the reset plus setting bits from zero). Thus any optimal strategy partitions the set of masks into a collection of chains, each chain being a sequence where every mask is a subset of the next. For a chain ending at mask \(M\), the total number of bit commands in that chain equals \(\operatorname{popcount}(M)\) (because the sum of differences from zero through the chain telescopes). Resets are needed between consecutive chains: the first chain needs no reset, each subsequent chain adds one `'R'`. Therefore the total cost is \((\#chains-1) + \sum_{\text{chains}} \operatorname{popcount}(\text{last mask})\), where the last mask of a chain is the one with no successor in that chain.
//
// This is exactly a weighted path-cover problem on a DAG with vertices being the distinct masks, and an edge from \(u\) to \(v\) exists iff \(u\) is a proper subset of \(v\). A path cover corresponds to a matching in the bipartite graph where both sides are the set of masks and an edge from left copy \(u\) to right copy \(v\) exists iff \(u \subset v\). If we select a matching edge \(u\to v\), it means \(v\) has predecessor \(u\), reducing the number of chains by 1 (saving one reset) and also removing \(v\) from the set of chain ends. The saving from matching \(u\to v\) is \(1 + \operatorname{popcount}(v)\): the \(1\) because the chain count drops, and \(\operatorname{popcount}(v)\) because \(v\) is no longer an end. Thus maximizing the total weight \(\sum_{\text{matched}}(1+\operatorname{popcount}(v))\) is equivalent to minimizing the total cost. Since all weights are positive, the optimal matching will also maximize cardinality, but we solve it directly as a maximum-weight matching.
//
// We implement the maximum-weight perfect matching using the Hungarian algorithm on an \(n \times n\) cost matrix where the real edges have cost \(\text{maxW} - (1+\operatorname{popcount}(v))\) and all other entries have cost \(\text{maxW}\) (i.e., weight 0). After obtaining the assignment, we reconstruct the chains: each matched edge gives a successor relation, and vertices without an incoming edge are chain starts. We then simulate the chains to build the command sequence, emitting a reset only between consecutive chains. The time complexity is \(O(n^3)\) for the Hungarian algorithm, and \(O(n^2 + n \cdot d)\) for building and simulating, where \(n \le 100\). Space is \(O(n^2)\).
