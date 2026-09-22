/*
You are given an array of `n` cells in a row, initially all unoccupied. There are `m` events, each specifying a position where a "blocked" marker is placed. After each event, we want to determine how many "free groups" of at least `a+1` consecutive unblocked cells exist, where a group of length `L` contributes `(L)/(a+1)` (integer division) to the count of the maximum number of non-overlapping segments of length `a` you can pack into that group. You are given a target `k` (the required number of such segments). You need to find the earliest event index (1-based) after which the total count becomes **strictly less than** `k`. If even before any event the count is already < `k`, output `-1`. If after all events the count never drops below `k`, output `0`? Actually the problem from the snippet asks: given initial placements of all `m` blocked cells at once, then process removals in reverse to find the latest event that still keeps count ≥ `k`. Specifically: all `m` events happen; after all events, if the count (number of segments) is still ≥ `k`, output `-1` (meaning even after all events, requirement satisfied). Otherwise, find the largest index `i` (1 ≤ i ≤ m) such that after removing the last (m-i) events (i.e., considering only the first `i` events as placed), the count is ≥ `k`. Output that `i`.

Write a function `int earliestEvent(int n, int a, int k, const std::vector<int>& events)` where `events` is a 1-indexed vector of positions (size `m`), each in [1, n], possibly duplicates, that returns:
- `-1` if after placing all events, the count is still ≥ `k`.
- Otherwise, the maximum index `i` (1-indexed) such that considering only the first `i` events (positions given in the first `i` entries of `events`), the count of non-overlapping segments of length `a` is ≥ `k`. If even after the first event count < k, output `1`? Actually the snippet processes from full set backward, so if after all events count < k, it finds the largest i where after removing events i+1..m the count becomes ≥ k. That i is exactly the earliest event that caused the count to drop below k when added. So output that i.

Constraints: n up to 1e6, m up to 1e6, a ≥ 1. Time O((n+m) α(n)) with union-find, space O(n).
*/
#include <vector>
#include <numeric>

class UnionFind {
    std::vector<int> parent, size;
public:
    UnionFind(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
    int getSize(int x) {
        return size[find(x)];
    }
};

// Given n cells, required segment length a, target k, and a 1-indexed list of event positions,
// return -1 if after all events count >= k, else the largest i (1..m) such that after first i events count >= k.
int earliestEvent(int n, int a, int k, const std::vector<int>& events) {
    const int m = (int)events.size();
    std::vector<bool> blocked(n + 2, false);
    // mark all event positions as blocked (only need to know if appears at least once)
    for (int pos : events) {
        if (pos >= 1 && pos <= n) blocked[pos] = true;
    }

    // Build initial union-find of unblocked consecutive cells
    UnionFind uf(n + 2);
    for (int i = 1; i < n; ++i) {
        if (!blocked[i] && !blocked[i+1]) {
            uf.unite(i, i+1);
        }
    }

    // Compute initial count
    long long now = 0;
    for (int i = 1; i <= n; ++i) {
        if (!blocked[i] && uf.find(i) == i) {
            now += (uf.getSize(i) + 1LL) / (a + 1LL); // integer division: (L)/(a+1) => floor(L/(a+1)) = (L)/(a+1) using integer division
        }
    }

    // If after all events count already >= k, return -1
    if (now >= k) return -1;

    // Process events in reverse to unblock
    std::vector<bool> unblocked(n + 2, false);
    for (int i = m - 1; i >= 0; --i) {
        int x = events[i];
        if (x < 1 || x > n) continue;
        if (unblocked[x]) continue; // already unblocked by a later event
        unblocked[x] = true;
        // Unblock x, create a new component of size 1
        blocked[x] = false;
        int merged_size = 1;
        // Merge left
        if (x > 1 && !blocked[x-1]) {
            int left_comp = uf.find(x-1);
            int sz = uf.getSize(x-1);
            now -= (sz + 1LL) / (a + 1LL);
            merged_size += sz;
            uf.unite(x, x-1);
        }
        // Merge right
        if (x < n && !blocked[x+1]) {
            int right_comp = uf.find(x+1);
            int sz = uf.getSize(x+1);
            now -= (sz + 1LL) / (a + 1LL);
            merged_size += sz;
            uf.unite(x, x+1);
        }
        now += (merged_size + 1LL) / (a + 1LL);
        if (now >= k) {
            return i + 1; // 1-based index
        }
    }
    // If never reached k even with all unblocked, return -1 (shouldn't happen as initial empty would give count)
    return -1;
}
#include <cassert>
#include <vector>

// (Include the solution function here)

int main() {
    // n=5, a=1, k=1, events block position 3
    // Initially after event: cells 1,2 and 4,5 free => two groups of size2 => each gives (2)/(2)=1 segment => total 2 >=1 => -1
    assert(earliestEvent(5, 1, 1, {3}) == -1);
    // n=5, a=1, k=2, block position 3 => total 2 >=2 => -1
    assert(earliestEvent(5, 1, 2, {3}) == -1);
    // n=5, a=1, k=3, block position 3 => total 2 <3 => now reverse: unblock 3 => full row size5 => (5)/(2)=2 <3, so never reaches 3 => -1
    assert(earliestEvent(5, 1, 3, {3}) == -1);
    // n=3, a=1, k=1, events block 2 => after event: groups [1] and [3] each size1 => (1)/(2)=0 each => total0 <1 => reverse: unblock2 => size3 => (3)/(2)=1 >=1 => answer 1
    assert(earliestEvent(3, 1, 1, {2}) == 1);
    // n=4, a=1, k=1, events: block 2 then block 3
    // after both: free cells [1] and [4] => each 0 => total0 <1 => reverse: unblock3 => now free [1] and [3,4] => sizes1 and2 => 0 + 1 =1 >=1 => answer 2 (since unblocking event 2 (index2) gives count>=k)
    assert(earliestEvent(4, 1, 1, {2,3}) == 2);
    // n=6, a=2, k=2, events: block 3 and 5
    // after both: free groups [1,2] (size2) gives (2)/(3)=0, [4] (1), [6] (1) => total0 <2 => reverse: unblock5 => groups [1,2],[4,6? Actually 5 unblocked connects 4-6? Wait 4 and6 separated by 5 unblocked, so group [4,5,6] size3 => (3)/(3)=1, and [1,2] size2 =>0 => total1<2 -> unblock3 => all 1-6 connected size6 => (6)/(3)=2 >=2 => answer 1
    assert(earliestEvent(6, 2, 2, {3,5}) == 1);
    // Test duplicate events: n=3, a=1, k=1, events {2,2}
    // after both events: only pos2 blocked (still one block) => groups [1],[3] => total0 <1 => reverse unblock first (index2) => unblock2 => full size3 => (3)/(2)=1 >=1 => answer 2
    assert(earliestEvent(3, 1, 1, {2,2}) == 2);
    // Edge case: a large n, m=0? but events vector may be empty. Then no events => all free => count = (n)/(a+1). If that >=k return -1 else return -1? Actually if events empty, reverse loop does nothing, so if initial count < k, return -1.
    assert(earliestEvent(5, 1, 5, {}) == -1);
    assert(earliestEvent(5, 1, 2, {}) == -1);
    // n=10, a=3, k=2, events block all positions? But events length < n, test small.
    // n=10, a=3, k=2, events {1,10}
    // after both: groups: 2-9 size8 => (8)/(4)=2 => >=2 => -1
    assert(earliestEvent(10, 3, 2, {1,10}) == -1);
    // n=10, a=3, k=3, events {1,10}
    // after both: group 2-9 size8 =>2<3 => reverse: unblock10 => group 2-10 size9 => (9)/(4)=2<3 -> unblock1 => group1-10 size10 => (10)/(4)=2<3 => never reaches3 => -1
    assert(earliestEvent(10, 3, 3, {1,10}) == -1);
    // n=10, a=3, k=2, events {5}
    // after event: groups:1-4 size4=>1, 6-10 size5=>1 => total2 >=2 => -1
    assert(earliestEvent(10, 3, 2, {5}) == -1);
    // n=10, a=3, k=3, events {5}
    // after event: total2 <3 => reverse unblock5 => full size10 => (10)/(4)=2<3 => -1
    assert(earliestEvent(10, 3, 3, {5}) == -1);

    return 0;
}
// The solution uses a union-find data structure to maintain connected components of consecutive unblocked cells. We start by marking all event positions as blocked (v[pos]=1). Then we union all adjacent unblocked cells to form initial components. For each component of size `sz`, the number of segments of length `a` that can fit is `(sz)/(a+1)` (because each segment needs `a` cells plus a gap of 1 cell, so maximal packing is floor((sz)/(a+1))). We compute the total count `now` by summing over all components.
//
// We then process events in reverse order. When we unblock a position `x`, we create a new component of size 1, then merge with left neighbor if unblocked, and right neighbor if unblocked. For each merge, we subtract the old contribution of the neighbor component and add the contribution of the merged component. After each unblock, we check if `now` becomes ≥ k; the first time it does (going backward), that index `i` is the answer. If we never reach ≥ k even after unblocking all events (i.e., original count was already < k), output -1.
//
// Edge cases: duplicate events: if a position is already blocked, unblocking it has no effect (it was already unblocked in the reverse process? Actually the original snippet sets v[x[i]]=1 once for each event, but if duplicate, it just sets it again; in reverse, unblocking sets v=0, and if same position appears twice, the first unblock (largest index) already makes it unblocked, subsequent unblocks (smaller index) would see v already 0 and would incorrectly add a new component? The snippet checks `if(!v[x[i]-1] && x[i]>1)` but not whether x[i] itself was blocked? Actually the original code sets v[x[i]]=0 before merging, but if it was already 0 (due to a later event in reverse), it would incorrectly treat it as unblocked and add an extra component. To fix, we should track whether the position was originally blocked (i.e., appeared in events) and only unblock if it was blocked. We can pre-mark a boolean `blockedInitially` and decrement a counter each time we unblock to avoid double-processing. In our function, we can simply process each event exactly once in reverse, and if we encounter a duplicate, the second time we try to unblock an already unblocked position, we should skip (but the contribution logic would be wrong if we don't skip). So we can maintain a set or boolean array that we toggle only for the first unblock. Simpler: process events in reverse and maintain a `blocked` array; if blocked[x] is already false, skip that event (because it was already unblocked by a later event). This ensures each position is unblocked only once.
//
// Time complexity: O((n+m) α(n)) for union-find. Space O(n).
