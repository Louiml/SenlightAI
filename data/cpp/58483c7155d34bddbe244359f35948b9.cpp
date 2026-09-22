Write a C++ function `int maxCompatiblePairs(int n, int t, const std::vector<std::pair<int,int>>& pairs)` that, given `n` items labeled `0` to `n-1`, `t` time steps, and a sequence of `t-1` pairs (each pair connects two items), returns the maximum number of items that can be grouped together at the end of the process. The process works as follows: at step 0, the first pair's two items are marked as compatible. For each subsequent step `i` (1 ≤ i < t), given a new pair `(a,b)`, you must check whether item `a` or item `b` is already in any existing compatibility group (initially only the first pair forms one group). If one of them is already in a group, add the other item to that same group (merging them into one group). If neither is in any group, they form a new group together. The function must return the size of the largest compatibility group after processing all pairs. If the input sequence is invalid (e.g., a pair includes an index out of range, or the number of pairs is not exactly `t-1`), return `-1`. Assume `t ≥ 1` and `n ≥ 1`, and pairs may contain duplicate values but not self-loops (a != b for each pair). The function should handle cases where a pair tries to connect two items already in different groups — in that case, the pair is ignored (no merging occurs), and the process continues.

The problem is a variant of union-find (disjoint-set) with a twist: we only union when exactly one of the two items is already in a group, and only one group per connected component is maintained. The approach is to use a disjoint-set data structure that tracks the parent and size of each group. Initially, after reading the first pair, we union those two items. Then for each subsequent pair `(a,b)`, we first validate indices (if out of range or a==b, return -1 immediately). We then find the roots of `a` and `b`. If both roots are the same, the pair is ignored (they are already in the same group). If exactly one of the roots is an "active" group (meaning it contains at least one item from the initial set) and the other root is an "inactive" singleton group (a root that has not yet been merged into any group), we merge them. To implement this, we can track a boolean `active[root]` that is true only for roots that are part of the main compatibility group(s). Initially, after unioning the first pair, that root is active. For each new pair, if exactly one of `active[find(a)]` and `active[find(b)]` is true, we union them (making the merged group active). If both are active, ignore; if both are inactive, we simply union them into a new inactive group (though this case should not occur because we only union when exactly one is active? Actually, the problem says: "If neither is in any group, they form a new group together." So we must also union when both are inactive — but then that group is still inactive. However, the original code snippet only handles the case when exactly one is in a group; it does not handle the case when neither is in a group. Let me re-read: The snippet initializes `chek[t1][0] = 1` and `chek[t2][0] = 1` for the first pair, then for each subsequent step it checks if `arr[t1][j]` or `arr[t2][j]` is 1, and if so, marks the other's column as 1. It never handles the case where neither is marked — in that case, `pos_found` stays 0 and nothing happens (the pair is ignored). So the intended behavior is: only merge when exactly one of the two items is already part of an existing group. If neither is part of a group, the pair is ignored. So we follow that. Therefore, our union operation only happens when exactly one root has `active=true` and the other has `active=false`. At the end, we return the size of the largest active group (the group that contains the initial pair). Inactive groups are never considered for the final answer. Edge cases: if the first pair itself has invalid indices or a==b, return -1. If t=1, there are no subsequent pairs, and the largest group size is 2 (the initial pair). If n=1 and t=1, but the first pair would be invalid because indices must be distinct? The problem says a != b for each pair, so n must be at least 2 for t>0. If n=1 and t=1, no pairs are given, but the first pair is required (since t-1 = 0, but we still need the first pair? Actually the snippet reads `t1 t2` before the loop, so there is always a first pair. So we require at least one pair always, meaning t >= 1 and we always have a first pair. So if t=1, we still process one pair (the first). So the function must accept a vector of pairs of size `t-1`? No, the snippet reads the first pair before the loop and then t-1 more pairs, so total pairs = t. Wait: the loop runs from i=1 to i<t, and inside it reads a new pair each iteration. So total pairs read = 1 (first) + (t-1) = t. So the input has exactly `t` pairs. The problem statement says "a sequence of `t-1` pairs" — that might be a misinterpretation. Let's clarify: the snippet reads `n, t`, then reads `t1 t2` (the first pair), then loops `for i=1 to i<t` reading one more pair each iteration. So total pairs = 1 + (t-1) = t. So the correct input has exactly `t` pairs. In the task specification, I wrote "a sequence of `t-1` pairs" but that's incorrect; it should be `t` pairs. Let me correct: The function takes `n`, `t`, and a vector of exactly `t` pairs. The first pair forms the initial compatibility group. Then for each subsequent pair (from index 1 to t-1), we apply the rule. So the function signature should be `int maxCompatiblePairs(int n, int t, const std::vector<std::pair<int,int>>& pairs)` where `pairs.size() == t`. The function returns the size of the largest active group after processing all pairs. Time complexity: O(t α(n)) where α is the inverse Ackermann function (near constant). Space complexity: O(n) for the disjoint-set arrays.

#include <vector>
#include <numeric>
#include <functional>

// Returns the size of the largest compatibility group after processing all pairs.
// Uses a disjoint-set with an 'active' flag indicating membership in the main group.
int maxCompatiblePairs(int n, int t, const std::vector<std::pair<int,int>>& pairs) {
    if (pairs.size() != static_cast<size_t>(t) || t < 1 || n < 2) return -1;
    // Validate the first pair
    if (pairs[0].first < 0 || pairs[0].first >= n || pairs[0].second < 0 || pairs[0].second >= n || pairs[0].first == pairs[0].second) return -1;

    std::vector<int> parent(n);
    std::vector<int> size(n, 1);
    std::vector<bool> active(n, false);
    std::iota(parent.begin(), parent.end(), 0);

    std::function<int(int)> find = [&](int x) -> int {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    auto unite = [&](int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (size[ra] < size[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        size[ra] += size[rb];
        active[ra] = active[ra] || active[rb];
    };

    // First pair: union them and mark active
    unite(pairs[0].first, pairs[0].second);
    active[find(pairs[0].first)] = true;

    // Process remaining pairs
    for (int i = 1; i < t; ++i) {
        int a = pairs[i].first, b = pairs[i].second;
        if (a < 0 || a >= n || b < 0 || b >= n || a == b) return -1;
        int ra = find(a), rb = find(b);
        if (ra == rb) continue; // already in same group
        bool active_a = active[ra], active_b = active[rb];
        if (active_a && active_b) continue; // both active -> ignore
        if (active_a || active_b) {
            // exactly one active -> merge
            unite(a, b);
        }
        // if neither active -> ignore
    }

    // Find largest active group
    int max_size = 0;
    for (int i = 0; i < n; ++i) {
        if (i == find(i) && active[i]) {
            max_size = std::max(max_size, size[i]);
        }
    }
    return max_size;
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the solution function (include the implementation above in the same file)
int maxCompatiblePairs(int n, int t, const std::vector<std::pair<int,int>>& pairs);

int main() {
    // Basic case: first pair (0,1), then (1,2) merges 2 into the group -> size 3
    assert(maxCompatiblePairs(3, 2, {{0,1},{1,2}}) == 3);
    // Two separate groups, but only the first is active -> largest active size 2
    assert(maxCompatiblePairs(4, 2, {{0,1},{2,3}}) == 2);
    // Merge two active groups? Both become active only if a second active group is created? Actually only one active group exists, so merging an active with inactive increases size
    assert(maxCompatiblePairs(5, 3, {{0,1},{1,2},{2,3}}) == 4);
    // Pair with both already active (same group) ignored
    assert(maxCompatiblePairs(3, 3, {{0,1},{1,2},{0,2}}) == 3);
    // Invalid index
    assert(maxCompatiblePairs(3, 1, {{0,3}}) == -1);
    // Self-loop invalid
    assert(maxCompatiblePairs(3, 1, {{1,1}}) == -1);
    // Wrong number of pairs
    assert(maxCompatiblePairs(3, 2, {{0,1}}) == -1);
    // Single pair only, t=1, group size 2
    assert(maxCompatiblePairs(2, 1, {{0,1}}) == 2);
    // n too small for t=1 pair
    assert(maxCompatiblePairs(1, 1, {{0,1}}) == -1);
    // Complex: active group merges multiple singletons, ignores separate inactive groups
    assert(maxCompatiblePairs(6, 5, {{0,1},{1,2},{3,4},{2,3},{4,5}}) == 4); // group {0,1,2,3} active size 4, {4,5} inactive
}
