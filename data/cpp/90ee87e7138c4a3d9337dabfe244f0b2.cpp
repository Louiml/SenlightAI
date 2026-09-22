/*
Given a binary string `s` of length `n` (1 ≤ n ≤ 200,000), write a C++ function `int decomposeBinaryString(const std::string& s, std::vector<std::vector<int>>& components)` that returns the minimum number of disjoint paths (each path is a sequence of distinct indices from 1 to n) that can be formed such that: each index is used exactly once, every edge in a path goes from a lower index to a higher index, no two consecutive indices in a path have the same character (the characters alternate between '0' and '1' in each path), and each path must start with a '0' and end with the last index of the path. More precisely, the original problem forms paths where each '1' at position i is paired with a preceding '0' (from a smaller index) and a following '0' (from a larger index), ensuring that '1's are always between two '0's in the same path. The function should return the minimal number of such paths, and fill `components` with the indices of each path (each path's indices in increasing order). If it is impossible to assign all indices to valid paths (e.g., a '1' cannot be matched with a preceding and following '0' that are not already used), the function should return -1 and leave `components` empty.
*/

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// Decompose a binary string into alternating 0-1 paths starting with 0.
// Returns the number of paths, or -1 if impossible, and fills components.
int decomposeBinaryString(const std::string& s, std::vector<std::vector<int>>& components) {
    const int n = static_cast<int>(s.size());
    if (n == 0) return 0;
    
    std::vector<int> nxt(n + 1);
    std::vector<bool> vis(n + 1, false);
    for (int i = 1; i <= n; ++i) nxt[i] = i;
    
    // First pass: pair each '1' with a preceding '0'
    int head = 1;
    for (int i = 1; i <= n; ++i) {
        if (s[i - 1] == '1') {
            while (head < i && s[head - 1] != '0') ++head;
            if (head < i) {
                nxt[head] = i;
                ++head;
            } else {
                return -1;
            }
        }
    }
    
    // Second pass: pair each '1' with a following '0'
    int tail = n;
    for (int i = n; i >= 1; --i) {
        if (s[i - 1] == '1') {
            while (tail > i && s[tail - 1] != '0') --tail;
            if (tail > i) {
                nxt[i] = tail;
                --tail;
            } else {
                return -1;
            }
        }
    }
    
    // Count paths: number of nodes that are not successors of someone else.
    std::vector<bool> has_pred(n + 1, false);
    for (int i = 1; i <= n; ++i) {
        if (nxt[i] != i) has_pred[nxt[i]] = true;
    }
    int ans = 0;
    std::vector<int> starts;
    for (int i = 1; i <= n; ++i) {
        if (!has_pred[i]) {
            ++ans;
            starts.push_back(i);
        }
    }
    
    components.clear();
    components.reserve(ans);
    for (int start : starts) {
        std::vector<int> path;
        int cur = start;
        while (true) {
            path.push_back(cur);
            if (nxt[cur] == cur) break;
            cur = nxt[cur];
        }
        components.push_back(std::move(path));
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// Declare the function (in actual test, include the solution header)
int decomposeBinaryString(const std::string& s, std::vector<std::vector<int>>& components);

int main() {
    // Example from the original snippet: "0101" should be possible with 2 paths? Let's check.
    std::vector<std::vector<int>> comps;
    int res = decomposeBinaryString("0101", comps);
    // Two '1's need two preceding '0's, but only one '0' before the second '1', impossible.
    assert(res == -1);
    comps.clear();
    
    // "0011" - two 0s then two 1s - each 1 pairs with a 0 before it and a 0 after it (none after), impossible.
    res = decomposeBinaryString("0011", comps);
    assert(res == -1);
    
    // "1010" - first char '1' has no preceding 0, impossible.
    res = decomposeBinaryString("1010", comps);
    assert(res == -1);
    
    // "01010" - possible? 0-1-0-1-0: one path 1->2->3->4->5, should be 1 path.
    comps.clear();
    res = decomposeBinaryString("01010", comps);
    assert(res == 1);
    assert(comps.size() == 1);
    assert(comps[0] == std::vector<int>({1,2,3,4,5}));
    
    // "001" - one 1 pairs with first 0, second 0 is isolated -> 2 paths.
    comps.clear();
    res = decomposeBinaryString("001", comps);
    assert(res == 2);
    assert(comps.size() == 2);
    // first path: index1->3, second path: index2 alone
    assert(comps[0] == std::vector<int>({1,3}) || comps[0] == std::vector<int>({2}));
    
    // "000" - no ones, every index isolated -> 3 paths.
    comps.clear();
    res = decomposeBinaryString("000", comps);
    assert(res == 3);
    assert(comps.size() == 3);
    
    // "010" - one 1 between two 0s -> one path.
    comps.clear();
    res = decomposeBinaryString("010", comps);
    assert(res == 1);
    assert(comps[0] == std::vector<int>({1,2,3}));
    
    // "010010" - two separate 0-1-0 patterns, each forms a path.
    comps.clear();
    res = decomposeBinaryString("010010", comps);
    assert(res == 2);
    assert(comps.size() == 2);
    
    // Single "0" -> one path with one node.
    comps.clear();
    res = decomposeBinaryString("0", comps);
    assert(res == 1);
    assert(comps[0] == std::vector<int>({1}));
    
    // "1" -> impossible.
    comps.clear();
    res = decomposeBinaryString("1", comps);
    assert(res == -1);
    
    return 0;
}

// The solution uses a greedy two-pass approach to pair each '1' with a preceding '0' and a following '0'. First, we scan left to right: for each '1' at position i, we find the leftmost available '0' before it (using a pointer `Head` that moves forward). If no such '0' exists, the configuration is impossible. We set `nxt[Head] = i` to link that '0' to this '1'. Then we scan right to left: for each '1' at position i, we find the rightmost available '0' after it (using a pointer `Tail` that moves backward). If no such '0' exists, impossible. We set `nxt[i] = Tail`. This creates a forest of linked lists: each node points to its successor in the path. Nodes that are not pointed to by any other node are the starts of paths (these are the '0's that were not matched as a preceding '0' for any '1'). The number of such starts equals the number of paths. We then traverse each unvisited start to output the path. Edge cases: a '1' that cannot be matched on either side, consecutive '0's with no '1' between them leading to extra paths, and indices that become isolated (e.g., a leading '0' with no following '1'). Time complexity is O(n) because each pointer moves at most n times total. Space complexity is O(n) for the `nxt` array and visited flags.
