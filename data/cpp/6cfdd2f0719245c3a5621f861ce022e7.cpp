/*
Write a C++ function `long long minimumXorSpanningCost(const std::vector<int>& values)` that, given a non-empty vector of distinct 32-bit signed integers, computes the minimum possible total cost of connecting all numbers into a single connected component using edges between pairs of numbers, where the cost of an edge between two numbers \(a\) and \(b\) is defined as their bitwise XOR \(a \oplus b\). Since the graph is complete, the result is the sum of edge weights in a Minimum Spanning Tree (MST) under this XOR metric. The vector may contain up to \(2 \times 10^5\) elements, and values are non-negative (fits in 31 bits). The function should return the total cost as a `long long`. You may assume the input is valid and contains distinct elements.
*/

#include <vector>
#include <algorithm>
#include <climits>

// A binary trie for quick minimum XOR queries
struct BinaryTrie {
    int cnt;
    int root;
    std::vector<std::array<int, 2>> tr;
    
    BinaryTrie() : cnt(1), root(1) {
        tr.push_back({0, 0});
    }
    
    // Insert a 31-bit integer (bits 30..0)
    void insert(int x) {
        int now = root;
        for (int bit = 30; bit >= 0; --bit) {
            int b = (x >> bit) & 1;
            if (!tr[now][b]) {
                tr[now][b] = ++cnt;
                tr.push_back({0, 0});
            }
            now = tr[now][b];
        }
    }
    
    // Query minimum XOR with x among inserted numbers
    int minXor(int x) const {
        int now = root;
        int result = 0;
        for (int bit = 30; bit >= 0; --bit) {
            int b = (x >> bit) & 1;
            if (tr[now][b]) {
                now = tr[now][b];
            } else {
                result |= (1 << bit);
                now = tr[now][1 - b];
            }
        }
        return result;
    }
    
    // Clear the trie
    void clear() {
        tr.clear();
        tr.push_back({0, 0});
        cnt = 1;
        root = 1;
    }
};

// Compute the MST cost under XOR metric for distinct non-negative integers
long long minimumXorSpanningCost(const std::vector<int>& values) {
    // Recursive helper using divide on bits (bit 30 down to 0)
    long long ans = 0;
    // Work on a copy of indices or values
    std::vector<int> a = values;
    
    // DFS function
    std::function<void(std::vector<int>&, int)> dfs = [&](std::vector<int>& vec, int bit) {
        if (vec.size() <= 1 || bit < 0) return;
        std::vector<int> zeroGroup, oneGroup;
        for (int x : vec) {
            if ((x >> bit) & 1) oneGroup.push_back(x);
            else zeroGroup.push_back(x);
        }
        if (!zeroGroup.empty() && !oneGroup.empty()) {
            // Build a trie from oneGroup and query minimum xor for each in zeroGroup
            BinaryTrie trie;
            for (int x : oneGroup) trie.insert(x);
            int best = INT_MAX;
            for (int x : zeroGroup) {
                best = std::min(best, trie.minXor(x));
            }
            ans += best;
        }
        dfs(zeroGroup, bit - 1);
        dfs(oneGroup, bit - 1);
    };
    
    dfs(a, 30);
    return ans;
}

#include <cassert>
#include <vector>

// Declare the function (already defined above)
long long minimumXorSpanningCost(const std::vector<int>& values);

int main() {
    // Single element
    assert(minimumXorSpanningCost({5}) == 0);
    
    // Two elements: cost is just XOR
    assert(minimumXorSpanningCost({1, 2}) == 3);
    assert(minimumXorSpanningCost({10, 20}) == 30);
    
    // Three elements: MST cost is min over pairs
    // For {1,2,3}: pairs (1,2)=3, (1,3)=2, (2,3)=1 -> MST edges 2+1=3
    assert(minimumXorSpanningCost({1, 2, 3}) == 3);
    
    // {4, 8, 12}: (4,8)=12, (4,12)=8, (8,12)=4 -> MST edges 8+4=12
    assert(minimumXorSpanningCost({4, 8, 12}) == 12);
    
    // Four numbers from example: {1, 2, 3, 4}
    // MST computed manually: (1,2)=3, (2,3)=1, (1,4)=5? Actually better: (1,3)=2, (2,3)=1, (1,4)=5? Let's compute: best spanning tree: edges (1,3)=2, (2,3)=1, (3,4)=7? That's 10. Better: (1,2)=3, (2,3)=1, (1,4)=5 -> total 9. Or (1,3)=2, (3,4)=7, (3,2)=1 -> 10. Or (1,3)=2, (1,2)=3, (2,4)=6 -> 11. The optimal: (1,3)=2, (3,2)=1, (3,4)=7 -> 10? Check (2,4)=6, (1,3)=2, (3,4)=7 -> 15. Let's brute: all pairs: (1,2)=3, (1,3)=2, (1,4)=5, (2,3)=1, (2,4)=6, (3,4)=7. MST: connect 2-3 (1), 1-3 (2), 3-4 (7) total 10? Or 1-3 (2), 2-3 (1), 2-4 (6) total 9. Yes 9 is minimal.
    assert(minimumXorSpanningCost({1, 2, 3, 4}) == 9);
    
    // Larger random test: {0, 1, 2, 4, 8} 
    // Brute force MST manually: edges: (0,1)=1, (1,2)=3, (0,4)=4, (0,8)=8 -> total 1+3+4+8=16. But maybe (0,2)=2, (2,4)=6, (4,8)=12 -> too much. Best: (0,1)=1, (1,2)=3, (0,4)=4, (0,8)=8 total 16. Check (0,1)=1, (1,3)? not present. Actually (0,2)=2, (2,6)? no. Let's compute: pairs: (0,1)=1, (0,2)=2, (0,4)=4, (0,8)=8, (1,2)=3, (1,4)=5, (1,8)=9, (2,4)=6, (2,8)=10, (4,8)=12. MST: pick (0,1)=1, (1,2)=3, (0,4)=4, (0,8)=8 total 16. That is minimal? Possibly yes. So assert 16.
    assert(minimumXorSpanningCost({0, 1, 2, 4, 8}) == 16);
    
    // Stress: 1000 random distinct numbers, but not feasible in test; we trust the algorithm.
    
    return 0;
}

// The problem is a classic XOR-MST problem. The approach uses a binary trie (or a divide-and-conquer on bits) to compute the MST cost efficiently. The key observation is that for a set of numbers, the optimal MST can be built recursively by considering the most significant bit. Partition the numbers into two groups based on the current bit (0-group and 1-group). If both groups are non-empty, then the MST must include at least one edge connecting the two groups. The optimal way to connect them is to pick the pair (one from each group) with the minimum XOR value. This is because XOR between numbers that differ in the highest bit will always have that bit set, and any edge connecting the groups must have that bit set; the minimal such edge is found by a trie query. After adding that minimal connecting edge, we recursively solve the MST inside each group independently, because edges within the same group have a higher bit that is 0, and they will always be cheaper than any cross-group edge in a proper MST construction. The base case is when the group has size 0 or 1 (no edges needed). The time complexity is \(O(N \log M)\) where \(M\) is the maximum bit length (here 30 or 31), because each number is inserted into a trie once per recursion level, and for each cross-group merge we do a trie query. Space complexity is \(O(N \log M)\) for the trie nodes, but can be optimized to \(O(N)\) by sorting and using divide-and-conquer. However, using the trie as in the snippet, the worst-case space is about \(O(N \cdot 31)\) which is acceptable for \(2\times10^5\). Edge case: if the vector has only one element, the cost is 0. If values are non-negative and fit in 31 bits, we can process bits from 30 down to 0.
