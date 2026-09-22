// Given two arrays of equal length N (1 ≤ N ≤ 150), where each element is a binary string of length L (1 ≤ L ≤ 40), write a C++ function `int minFlipsToMatch(const vector<string>& devices, const vector<string>& outlets)` that returns the minimum number of bit flips (applied identically to all outlets) needed to make the multiset of transformed outlets equal to the multiset of devices. If it is impossible, return -1. A bit flip means inverting a specific bit position (0→1 or 1→0) for every outlet string. For example, flipping bit position 0 changes "101" to "100" for all outlets. The function should handle up to 150 strings per array, each up to 40 characters.
The key insight: For each outlet-device pair, the XOR of their bit strings represents a potential global flip mask—if we apply exactly those flips to all outlets, that specific outlet becomes that device. For a given mask (XOR value), all outlets that can be matched to some device under that mask must form a perfect matching in a bipartite graph where left nodes are outlets and right nodes are devices, with an edge if outlet XOR mask == device. Since N ≤ 150, we can try each distinct XOR mask (at most N² = 22500) and run a maximum bipartite matching (Hopcroft-Karp or simple DFS). If the maximum matching size equals N, then the mask is valid, and the answer is the number of set bits (population count) of that mask. We take the minimum over all valid masks. Edge cases: if N=0 (though constraints say ≥1), or if no mask achieves perfect matching, return -1. The number of bits in the mask is computed by counting '1' characters or using bit operations after converting to an integer (but L ≤ 40 fits in 64-bit). Complexity: O(N² * N * E) for simple DFS matching, which is O(N^4) worst-case but with N=150 might be heavy; better to use Hopcroft-Karp per mask, yielding O(N² * E√V) ≈ O(N³√N) ≈ feasible. Space O(N²) for storing masks, O(N) for matching arrays.
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstring>

// Convert binary string to 64-bit integer (assuming L <= 40).
static uint64_t toBits(const std::string& s) {
    uint64_t res = 0;
    for (char c : s) {
        res = (res << 1) | (c - '0');
    }
    return res;
}

// Count set bits in an integer.
static int popcount(uint64_t x) {
    int cnt = 0;
    while (x) {
        cnt += (x & 1);
        x >>= 1;
    }
    return cnt;
}

// Hopcroft-Karp bipartite matching. Returns size of maximum matching.
static int hopcroftKarp(const std::vector<std::vector<int>>& adj, int n_left, int n_right) {
    std::vector<int> pairU(n_left, -1), pairV(n_right, -1);
    std::vector<int> dist(n_left);
    const int INF = 1e9;

    // BFS to build layered graph
    std::function<bool()> bfs = [&]() -> bool {
        std::queue<int> q;
        for (int u = 0; u < n_left; ++u) {
            if (pairU[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int pu = pairV[v];
                if (pu == -1) {
                    found = true;
                } else if (dist[pu] == INF) {
                    dist[pu] = dist[u] + 1;
                    q.push(pu);
                }
            }
        }
        return found;
    };

    // DFS to augment
    std::function<bool(int)> dfs = [&](int u) -> bool {
        for (int v : adj[u]) {
            int pu = pairV[v];
            if (pu == -1 || (dist[pu] == dist[u] + 1 && dfs(pu))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = INF;
        return false;
    };

    int matching = 0;
    while (bfs()) {
        for (int u = 0; u < n_left; ++u) {
            if (pairU[u] == -1 && dfs(u)) {
                ++matching;
            }
        }
    }
    return matching;
}

// Solution function: return min flips or -1.
int minFlipsToMatch(const std::vector<std::string>& devices, const std::vector<std::string>& outlets) {
    int N = (int)devices.size();
    if (N == 0) return 0;
    int L = (int)devices[0].size();

    std::vector<uint64_t> devBits(N), outBits(N);
    for (int i = 0; i < N; ++i) {
        devBits[i] = toBits(devices[i]);
        outBits[i] = toBits(outlets[i]);
    }

    // Collect all possible XOR masks (unique)
    std::vector<uint64_t> masks;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            masks.push_back(outBits[i] ^ devBits[j]);
        }
    }
    std::sort(masks.begin(), masks.end());
    masks.erase(std::unique(masks.begin(), masks.end()), masks.end());

    int best = -1;
    for (uint64_t mask : masks) {
        // Build adjacency list: outlet i can go to device j if outBits[i] ^ mask == devBits[j]
        std::vector<std::vector<int>> adj(N);
        for (int i = 0; i < N; ++i) {
            uint64_t target = outBits[i] ^ mask;
            for (int j = 0; j < N; ++j) {
                if (target == devBits[j]) {
                    adj[i].push_back(j);
                }
            }
        }
        int matchSize = hopcroftKarp(adj, N, N);
        if (matchSize == N) {
            int flips = popcount(mask);
            if (best == -1 || flips < best) {
                best = flips;
            }
        }
    }
    return best;
}
#include <cassert>
#include <string>
#include <vector>

// Declaration of the solution function (already defined above)
int minFlipsToMatch(const std::vector<std::string>& devices, const std::vector<std::string>& outlets);

int main() {
    // Example 1: Two outlets, two devices, one flip needed
    std::vector<std::string> d1 = {"01", "10"};
    std::vector<std::string> o1 = {"00", "11"};
    assert(minFlipsToMatch(d1, o1) == 1); // flip bit 1: 00->01, 11->10

    // Example 2: Impossible case
    std::vector<std::string> d2 = {"0", "1"};
    std::vector<std::string> o2 = {"0", "0"};
    assert(minFlipsToMatch(d2, o2) == -1);

    // Example 3: Already matching, no flips
    std::vector<std::string> d3 = {"101", "010"};
    std::vector<std::string> o3 = {"101", "010"};
    assert(minFlipsToMatch(d3, o3) == 0);

    // Example 4: Single element
    std::vector<std::string> d4 = {"1"};
    std::vector<std::string> o4 = {"0"};
    assert(minFlipsToMatch(d4, o4) == 1);

    // Example 5: Multiple flips needed (two bits)
    std::vector<std::string> d5 = {"000", "111"};
    std::vector<std::string> o5 = {"110", "001"};
    assert(minFlipsToMatch(d5, o5) == 2); // flip bits 0 and 1: 110->000, 001->111

    // Example 6: Duplicate masks, only one valid
    std::vector<std::string> d6 = {"00", "00"};
    std::vector<std::string> o6 = {"00", "01"};
    assert(minFlipsToMatch(d6, o6) == 1);

    // Example 7: Larger example, all zeros vs all ones
    std::vector<std::string> d7 = {"00", "00"};
    std::vector<std::string> o7 = {"11", "11"};
    assert(minFlipsToMatch(d7, o7) == 2);

    // Example 8: No bits length but N>0 not allowed; test with length 1 and duplicates
    std::vector<std::string> d8 = {"0", "0", "1"};
    std::vector<std::string> o8 = {"1", "1", "0"};
    assert(minFlipsToMatch(d8, o8) == 1); // flip the only bit

    return 0;
}
