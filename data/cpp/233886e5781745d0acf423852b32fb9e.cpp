Given an array `id` of length `N` containing a permutation of the integers from `1` to `N`, and `M` queries, each defined by a left index `A` and right index `B` (1-indexed), write a C++ function `countGroups` that returns a vector of integers. For each query, consider the contiguous subarray `id[A..B]`. A “group” is a maximal set of consecutive numbers (in value, not position) that all appear within that subarray. For example, if the subarray contains `{3, 1, 2}`, the values 1,2,3 form one group. If it contains `{1, 3, 5}`, each is its own group (three groups). The function must return, for each query in input order, the number of groups in the corresponding subarray. The input is guaranteed to be a permutation of 1..N. Handle up to N=100,000 and M=100,000, with T test cases (but the function processes one test case at a time).
#include <cassert>
#include <vector>
#include <utility>

// (Include the solution code here or in the same file)

int main() {
    // Basic case from the problem description.
    std::vector<int> id1 = {0, 3, 1, 2}; // N=3, 1-indexed
    std::vector<std::pair<int,int>> q1 = {{1,3}, {1,2}, {2,3}, {1,1}};
    std::vector<int> ans1 = countGroups(id1, q1);
    assert(ans1 == std::vector<int>({1, 2, 2, 1}));

    // Permutation 1..5, queries.
    std::vector<int> id2 = {0, 5, 1, 4, 2, 3};
    // Subarray [1,5] = {5,1,4,2,3} -> values 1..5 all present -> 1 group.
    // [1,3] = {5,1,4} -> groups: {1}, {4,5} -> 2 groups.
    // [3,5] = {4,2,3} -> groups: {2,3,4} -> 1 group.
    std::vector<std::pair<int,int>> q2 = {{1,5}, {1,3}, {3,5}};
    std::vector<int> ans2 = countGroups(id2, q2);
    assert(ans2 == std::vector<int>({1, 2, 1}));

    // N=1.
    std::vector<int> id3 = {0, 1};
    std::vector<std::pair<int,int>> q3 = {{1,1}};
    assert(countGroups(id3, q3) == std::vector<int>({1}));

    // Reversed permutation.
    std::vector<int> id4 = {0, 4, 3, 2, 1}; // N=4
    // Full array: all values present -> 1 group.
    // [1,2] = {4,3} -> group {3,4} -> 1.
    // [1,1] = {4} -> 1.
    // [2,4] = {3,2,1} -> {1,2,3} -> 1.
    std::vector<std::pair<int,int>> q4 = {{1,4}, {1,2}, {1,1}, {2,4}};
    assert(countGroups(id4, q4) == std::vector<int>({1, 1, 1, 1}));

    // More complex: id = [1,3,5,2,4], N=5
    std::vector<int> id5 = {0, 1, 3, 5, 2, 4};
    // [1,5] = {1,3,5,2,4} -> all values -> 1
    // [1,3] = {1,3,5} -> three separate groups -> 3
    // [2,4] = {3,5,2} -> {2,3} and {5} -> 2
    std::vector<std::pair<int,int>> q5 = {{1,5}, {1,3}, {2,4}};
    assert(countGroups(id5, q5) == std::vector<int>({1, 3, 2}));

    // Edge: subarray of length 1 always 1.
    std::vector<int> id6 = {0, 2, 1, 3};
    std::vector<std::pair<int,int>> q6 = {{1,1}, {2,2}, {3,3}, {2,3}};
    // [2,3] = {1,3} -> two groups.
    assert(countGroups(id6, q6) == std::vector<int>({1, 1, 1, 2}));

    return 0;
}
#include <vector>
#include <algorithm>
#include <functional>

// Count groups in subarrays for each query.
// id: permutation of 1..N (1-indexed vector, size N+1)
// queries: vector of pairs (A, B) with 1 <= A <= B <= N
// Returns vector of answers in query order.
std::vector<int> countGroups(const std::vector<int>& id,
                             const std::vector<std::pair<int,int>>& queries) {
    const int N = (int)id.size() - 1;
    const int M = (int)queries.size();
    std::vector<int> ans(M);

    // Build segment tree for range add and point query.
    int size = 1;
    while (size < N) size <<= 1;
    std::vector<int> seg(2 * size, 0);

    // Precompute positions of each value.
    std::vector<int> pos(N + 1, 0);
    for (int i = 1; i <= N; ++i) pos[id[i]] = i;

    // Group queries by right endpoint.
    std::vector<std::vector<int>> byRight(N + 1);
    for (int q = 0; q < M; ++q) {
        byRight[queries[q].second].push_back(q);
    }

    // Range add function on segment tree.
    std::function<void(int,int,int,int,int,int)> add = [&](int node, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            seg[node] += val;
            return;
        }
        int mid = (l + r) / 2;
        if (ql <= mid) add(node * 2, l, mid, ql, qr, val);
        if (qr > mid) add(node * 2 + 1, mid + 1, r, ql, qr, val);
    };

    // Point query.
    std::function<int(int,int,int,int)> query = [&](int node, int l, int r, int x) {
        if (l == r) return seg[node];
        int mid = (l + r) / 2;
        if (x <= mid) return seg[node] + query(node * 2, l, mid, x);
        else return seg[node] + query(node * 2 + 1, mid + 1, r, x);
    };

    // Scan positions from left to right.
    for (int i = 1; i <= N; ++i) {
        // Start new group for all starts up to i.
        add(1, 1, size, 1, i, 1);

        // Connect to previous smaller value.
        int small = id[i] - 1;
        if (small >= 1 && pos[small] < i) {
            add(1, 1, size, 1, pos[small], -1);
        }
        // Connect to previous larger value.
        int large = id[i] + 1;
        if (large <= N && pos[large] < i) {
            add(1, 1, size, 1, pos[large], -1);
        }

        // Answer queries that end here.
        for (int qIdx : byRight[i]) {
            ans[qIdx] = query(1, 1, size, queries[qIdx].first);
        }
    }

    return ans;
}
// The key insight is to process queries offline by sorting them by their right endpoint `B`. As we scan the array from left to right, we maintain a segment tree over positions `1..i` where each position `p` stores the number of connected components in the subarray `id[p..i]`. Initially, when we include a new element `id[i]`, we add 1 to all positions `1..i` (because a new element starts as its own group when appended). However, if the value immediately smaller (`id[i]-1`) already appeared earlier at position `pre1`, then within any subarray that starts before `pre1`, the current element connects to that existing group, so we subtract 1 from all positions `1..pre1`. Similarly, if the value immediately larger (`id[i]+1`) appeared earlier at `pre2`, subtract 1 from `1..pre2`. After updating, for every query with `B == i`, the answer is the point query at position `A`. This works because the segment tree stores for each start position the current number of groups in `[start, i]`. The algorithm is O((N+M) log N). Edge cases: when `pre1` or `pre2` is 0 (not present), skip; when both are present, both subtractions are applied. The segment tree uses lazy propagation for range additions and point queries.
