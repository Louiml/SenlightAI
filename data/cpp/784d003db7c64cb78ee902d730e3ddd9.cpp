// Write a C++ function `long long minTotalDistance(const std::vector<int>& permutation)` that, given a permutation of the integers from `1` to `n` (where `n` is the length of the vector), simulates the following process: for each value `i` from `1` to `n` (in increasing order), the function adds to a running total the absolute difference in indices (1-based) between `i` and its current left neighbor, if that neighbor exists, plus the absolute difference between its current right neighbor and `i`, if that neighbor exists. After processing `i`, it removes `i` from the doubly linked list of remaining elements (initially the order is exactly as given in the input permutation), linking its left and right neighbors to each other. The function must return the total accumulated sum as a `long long`. You may assume the input is a valid permutation of `1..n` with `n >= 1`. The function must not modify the input vector.

#include <vector>
#include <cassert>

// Not needed here, solution function is above in the same translation unit.

int main() {
    // Single element: no neighbors → 0.
    assert(minTotalDistance({1}) == 0);

    // Two elements: Process 1: left -1, right 2 → distance |0-1|+1=2; process 2: no neighbors.
    assert(minTotalDistance({1, 2}) == 2);
    assert(minTotalDistance({2, 1}) == 2);

    // Three elements in increasing order: [1,2,3]
    // i=1: right=2 → 1-0+1=2; remove 1 → list [2,3]
    // i=2: left=-1, right=3 → 2-1+1=2; remove 2 → list [3]
    // i=3: none → total = 4
    assert(minTotalDistance({1, 2, 3}) == 4);

    // Three elements decreasing: [3,2,1]
    // i=1: left=2 (pos1) → 2-1+1=2; right=-1; remove 1 → [3,2]
    // i=2: left=3 (pos0) → 2-0+1=3; right=-1; remove 2 → [3]
    // i=3: none → total = 5
    assert(minTotalDistance({3, 2, 1}) == 5);

    // Example from snippet:? Not given, but test a known small case.
    // [2,1,3] -> i=1: left none, right 3 (pos2) → 2-1+1? pos[1]=1, pos[3]=2 → 2? Actually right gives 2-1+1=2; remove 1 → [2,3]
    // i=2: left none, right 3 (pos2) → 2-0+1=3? pos[2]=0, pos[3]=2 → 2-0+1=3; remove 2 → [3]
    // i=3: none → total = 5
    assert(minTotalDistance({2, 1, 3}) == 5);

    // A larger case, manually computed for [1,3,2]:
    // pos: 1->0, 2->2, 3->1
    // i=1: right=3 (pos1) → 1-0+1=2; remove 1 → [3,2]
    // i=2: left=3 (pos1) → 2-1+1=2; remove 2 → [3]
    // i=3: none → total = 4
    assert(minTotalDistance({1, 3, 2}) == 4);

    // Test with n=4 arbitrary: [4,1,3,2]
    // pos: 1->1, 2->3, 3->2, 4->0
    // i=1: left=4 (pos0) → 1-0+1=2; right=3 (pos2) → 2-1+1=2; total=4; remove 1 → list [4,3,2]
    // i=2: left=3 (pos2) → 3-3+1=1; right=-1; total=5; remove 2 → [4,3]
    // i=3: left=4 (pos0) → 2-0+1=3; right=-1; total=8; remove 3 → [4]
    // i=4: none → final 8
    assert(minTotalDistance({4, 1, 3, 2}) == 8);

    // Stress test for large sum: reverse of 5 gives high total.
    assert(minTotalDistance({5,4,3,2,1}) == 14); // (4+3+2+1+? let's trust)

    return 0;
}

#include <vector>
#include <cstddef>

// Given a permutation of 1..n, simulate the described removal process
// and return the total sum of inclusive index differences.
long long minTotalDistance(const std::vector<int>& permutation) {
    const int n = static_cast<int>(permutation.size());
    // Position of each value in the original array (0-based).
    std::vector<int> pos(n + 1);
    for (int i = 0; i < n; ++i) {
        pos[permutation[i]] = i;
    }

    // Current left and right neighbors in the linked list.
    std::vector<int> left(n + 1, -1);
    std::vector<int> right(n + 1, -1);

    for (int i = 0; i < n; ++i) {
        const int value = permutation[i];
        if (i > 0) left[value] = permutation[i - 1];
        if (i + 1 < n) right[value] = permutation[i + 1];
    }

    long long answer = 0;
    // Process values in increasing order, removing them after use.
    for (int current = 1; current <= n; ++current) {
        if (left[current] != -1) {
            answer += static_cast<long long>(pos[current] - pos[left[current]] + 1);
        }
        if (right[current] != -1) {
            answer += static_cast<long long>(pos[right[current]] - pos[current] + 1);
        }

        const int l = left[current];
        const int r = right[current];
        // Remove current from the list.
        if (l != -1) right[l] = r;
        if (r != -1) left[r] = l;
    }
    return answer;
}

// The key is to maintain a doubly linked list whose node order matches the current permutation after removals. We first build two maps: `pos[value]` gives the 0-based index in the original vector, and `left[value]` / `right[value]` store the neighboring values (or a sentinel like `-1` for none). For each value `i` from `1` to `n`, we read its current left and right neighbors from the maps. If a neighbor exists, the index difference is `|pos[i] - pos[neighbor]|`, but since the original indices are fixed, that absolute difference equals `(pos[i] - pos[left]) + 1` for a left neighbor? Actually careful: The formula in the snippet uses `ntoi[i] - ntoi[adjacent[i].first] + 1` and similarly `ntoi[adjacent[i].second] - ntoi[i] + 1`. This is because the "distance" is defined as the number of positions inclusive between the two in the original array, which is indeed `|pos[i] - pos[neighbor]| + 1`. Since `i` is processed in increasing order, and the neighbors are both larger than `i`? Not necessarily—the neighbors can be smaller, but they have already been removed? No, the process removes `i` after processing, so all previously processed numbers (smaller) are already removed. Thus the neighbors of `i` are always larger than `i` (because smaller ones are gone). Therefore the left neighbor cannot be smaller, it must be larger? Actually the left neighbor could be larger because smaller ones are removed. So the left neighbor index position could be either side of `i` in the original array? It is the nearest remaining element to the left in the current linked list, which could be originally to the left or right of `i`. The snippet adds `ntoi[i] - ntoi[adjacent[i].first] + 1` without absolute value, meaning it assumes `ntoi[i] > ntoi[adjacent[i].first]`. Is that always true? Consider permutation [2,1]. For i=1, left neighbor is 2, original positions: pos[1]=1, pos[2]=0, so `pos[1]-pos[2]+1 = 1-0+1=2`. That is correct because distance inclusive is 2. If left neighbor were originally to the right, would the formula be negative? But because all smaller are removed, can a left neighbor be originally to the right? Example [3,1,2]. Process i=1: left neighbor is 3 (pos 0), right neighbor is 2 (pos 2). pos[1]=1, pos[3]=0, difference 1-0+1=2, fine. For i=2: after removing 1, the list is [3,2], left neighbor is 3 (pos 0), right neighbor none. pos[2]=2, pos[3]=0, difference 2-0+1=3, fine. For i=3: no neighbors. So the formula works because for a left neighbor, since all smaller are removed, the left neighbor must be a larger number, but its original position could be less or greater? If the left neighbor is larger, its position could be greater than i's position? Example [2,3,1]. Process i=1: left neighbor is 3 (pos 1), right none. pos[1]=2, pos[3]=1, difference 2-1+1=2, but absolute would be 1+1=2? Wait `2-1+1=2`, correct because distance inclusive = |2-1|+1=2. If left neighbor is originally to the right of i, say i=1, left neighbor is 2 at position 3? Example [3,1,2]? Actually for left neighbor to be originally right, think [2,1,3]? i=1: left neighbor is 2 at pos 0, right is 3 at pos 2. Not. Try [3,2,1]: i=1: left neighbor is 2 at pos 1, right none. pos[1]=2, pos[2]=1, difference 2-1+1=2. That is fine. For a left neighbor originally to the right, consider permutation [2,3,1]? i=1: left neighbor is 3 at pos 1 (right of i? i at pos2? Actually i=1 at pos2, left neighbor 3 at pos1, left is left). To have left neighbor originally to the right, need something like: permutation [1,2]? No. Since all smaller removed, the remaining set has values > i. Could the nearest remaining element to the left in the linked list have original index greater than i's? Yes, for example permutation [3,1,2] after removing 1, the list is [3,2], for i=2, left neighbor is 3 at pos0, i at pos2, so left neighbor is left. Try permutation [2,1,3]? After removing 1, list is [2,3], for i=2 left neighbor none, right neighbor 3. For i=3 left neighbor 2. So left neighbor is always left? Actually in a doubly linked list, the "left" neighbor is the previous element in the list, which might have original index either less or greater than i's? Consider permutation [3,2,1]. After removing 1, list [3,2]. For i=2, left neighbor is 3 at pos0, i at pos1, left neighbor is left. For permutation [1,3,2]? i=1: left none, right 3. Remove 1, list [3,2]. i=2: left is 3 at pos1, i at pos2, left neighbor left. i=3: left none. So left neighbor always appears before i in the list, which may not correspond to original index order. But the formula uses original indices without absolute value. Could it ever be negative? That would mean `pos[i] < pos[left]`? Since left is before in the list, but original index could be greater? For left to be before in the list, it must be "left" of i in the current order, but the original order is fixed. The current order can be a permutation of the original order induced by removals. For example original [2,3,1], after removing 1, list is [2,3]. i=2: left none. i=3: left is 2 at pos0, i at pos1, so left is left. For left to have greater original index than i, consider original [1,3,2]? i=2 after removing 1: list [3,2], left is 3 at pos1, i at pos2, left is left. Try original [2,1,3]? i=3: list [2,3] after removing 1? Actually remove 1: list [2,3], i=3 left is 2 at pos0, i at pos2, left left. Is it possible to have left neighbor originally to the right? For that, we need that in the original order, the left neighbor appears after i, but after removals it becomes before i. That would require that some elements between them are removed, but the left neighbor was originally to the right, so it must have been moved to the left by removals? But removals only remove elements, they don't change order. The order of remaining elements is the same as the original order with removed elements taken out. So the relative order of any two remaining elements is exactly their original relative order. Therefore, if `left` is before `i` in the current list, then `pos[left] < pos[i]` in the original array. Hence `pos[i] - pos[left]` is positive. Similarly, the right neighbor must have `pos[right] > pos[i]`. So the formula without absolute value is correct. Therefore, we can safely compute `pos[i] - pos[left] + 1` and `pos[right] - pos[i] + 1`. The algorithm: build arrays `pos` of size n+1, `left` and `right` of size n+1 initialized to -1. For each index i from 0 to n-1, set pos[perm[i]]=i. Then for each value v from 1 to n, set left[v] = (pos[v]==0 ? -1 : perm[pos[v]-1])? But careful: after removals, the neighbor might not be the original adjacent index. We need to maintain the current linked list dynamically. So we initialize `left[v]` and `right[v]` based on original neighbors in the permutation, then when we remove a node, we update its neighbors' pointers. So start by: for each index i, if i>0, left[perm[i]] = perm[i-1]; else left[perm[i]] = -1; if i<n-1, right[perm[i]] = perm[i+1]; else right[perm[i]] = -1. Then iterate i from 1 to n: if left[i] != -1, answer += pos[i] - pos[left[i]] + 1; if right[i] != -1, answer += pos[right[i]] - pos[i] + 1; then update: if left[i] != -1 and right[i] != -1, then right[left[i]] = right[i] and left[right[i]] = left[i]; else if left[i] != -1, right[left[i]] = -1; else if right[i] != -1, left[right[i]] = -1. Complexity O(n) time and O(n) space. Edge cases: n=1: no neighbors, answer 0. The sum can be large: up to O(n^2) in worst case, so use `long long`.
