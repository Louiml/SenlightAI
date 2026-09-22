/*
Write a C++ function that takes three parameters: an integer `n`, an integer `q`, and a `std::vector<long long>` named `a` containing `n` positive integers (1-indexed conceptually), along with a list of `q` range update operations. Each update is defined by two integers `l` and `r` (1 ≤ l ≤ r ≤ n), meaning that for every index `i` in the inclusive range `[l, r]`, the "frequency" of that index is incremented by 1. After processing all `q` updates, each index `i` (1-indexed) will have an associated frequency `cnt[i]`. Your function should return the maximum possible sum of `a[i] * cnt[i]` over all permutations of the array `a` and the frequency array `cnt`, i.e., sort both arrays and compute the dot product of the sorted arrays. The function should take as input `n`, `q`, the array `a`, and a vector of pairs representing the updates, and return the result as a `long long`. The original code snippet uses a Fenwick tree (BIT) to efficiently apply range updates and then sorts both arrays to compute the maximum sum — replicate this exact behavior. Assume `n` can be up to 200,000 and `q` up to 200,000; the values in `a` and the result fit in 64-bit signed integers.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Fenwick tree (Binary Indexed Tree) for range add, point query.
class Fenwick {
private:
    std::vector<long long> bit;
    int size;
public:
    Fenwick(int n) : size(n + 2), bit(n + 2, 0) {}
    
    void update(int idx, long long delta) {
        for (; idx < (int)bit.size(); idx += idx & (-idx)) {
            bit[idx] += delta;
        }
    }
    
    long long query(int idx) const {
        long long sum = 0;
        for (; idx > 0; idx -= idx & (-idx)) {
            sum += bit[idx];
        }
        return sum;
    }
};

// Computes the maximum sum of a[i] * cnt[i] after applying all range updates.
// Parameters: n = number of indices, q = number of updates,
// a = vector of n values, updates = vector of pairs (l, r) with 1 <= l <= r <= n.
long long maxWeightedSum(int n, int q, const std::vector<long long>& a, 
                         const std::vector<std::pair<int,int>>& updates) {
    Fenwick bit(n);
    for (int i = 0; i < q; ++i) {
        int l = updates[i].first;
        int r = updates[i].second;
        bit.update(l, 1);
        bit.update(r + 1, -1);
    }
    
    std::vector<long long> cnt(n);
    for (int i = 1; i <= n; ++i) {
        cnt[i-1] = bit.query(i);
    }
    
    std::vector<long long> sorted_a = a;
    std::sort(sorted_a.begin(), sorted_a.end());
    std::sort(cnt.begin(), cnt.end());
    
    long long res = 0;
    for (int i = 0; i < n; ++i) {
        res += sorted_a[i] * cnt[i];
    }
    return res;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above.
long long maxWeightedSum(int n, int q, const std::vector<long long>& a, 
                         const std::vector<std::pair<int,int>>& updates);

int main() {
    // Test case 1: single update over all indices.
    std::vector<long long> a1 = {1, 2, 3};
    std::vector<std::pair<int,int>> upd1 = {{1, 3}};
    assert(maxWeightedSum(3, 1, a1, upd1) == (1*1 + 2*1 + 3*1)); // 6

    // Test case 2: no updates -> all frequencies zero -> result 0.
    std::vector<long long> a2 = {5, 7, 9};
    std::vector<std::pair<int,int>> upd2 = {};
    assert(maxWeightedSum(3, 0, a2, upd2) == 0);

    // Test case 3: overlapping updates create non-uniform frequencies.
    // indices: 1..2 get +1, 2..3 get +1, so cnt = [1,2,1]
    // a = [10,20,30] -> sorted a = [10,20,30], sorted cnt = [1,1,2]
    // sum = 10*1 + 20*1 + 30*2 = 90
    std::vector<long long> a3 = {10, 20, 30};
    std::vector<std::pair<int,int>> upd3 = {{1,2}, {2,3}};
    assert(maxWeightedSum(3, 2, a3, upd3) == 90);

    // Test case 4: a larger example with n=5, q=3.
    // updates: [1,5], [2,4], [3,3] -> cnt = [1,2,3,2,1]
    // a = [3,1,4,1,5] -> sorted a = [1,1,3,4,5], sorted cnt = [1,1,2,2,3]
    // sum = 1*1 + 1*1 + 3*2 + 4*2 + 5*3 = 1+1+6+8+15 = 31
    std::vector<long long> a4 = {3,1,4,1,5};
    std::vector<std::pair<int,int>> upd4 = {{1,5}, {2,4}, {3,3}};
    assert(maxWeightedSum(5, 3, a4, upd4) == 31);

    // Test case 5: all frequencies become 1 with non-overlapping covers.
    // updates: [1,1], [2,2], [3,3] -> cnt = [1,1,1]
    // a = [100, 200, 300] -> sum = 100+200+300 = 600
    std::vector<long long> a5 = {100, 200, 300};
    std::vector<std::pair<int,int>> upd5 = {{1,1}, {2,2}, {3,3}};
    assert(maxWeightedSum(3, 3, a5, upd5) == 600);

    return 0;
}

// The problem reduces to: given an array of frequencies `cnt[1..n]` initially all zero, for each update `[l, r]` we increment `cnt[i]` for all `i` in that range. After processing all updates, we want to maximize `sum(a[i]*cnt[i])` over all permutations of `a` and `cnt`. By the rearrangement inequality, the maximum dot product is achieved by sorting `a` ascending and `cnt` ascending (or both descending, but we sort both in ascending order and multiply pairwise). So the algorithm is: 
// 1. Use a Fenwick tree (Binary Indexed Tree) to support range update and point query. For each update `(l, r)`, perform `update(l, +1)` and `update(r+1, -1)` on the BIT (1-indexed). After all updates, query each index `i` from 1 to `n` to get `cnt[i]`. 
// 2. Sort the original array `a` and the frequency array `cnt` in non-decreasing order. 
// 3. Compute the sum `res = sum(a[i] * cnt[i])` over `i=0..n-1` and return it. 
// Edge cases: ensure BIT indices are within bounds (note `r+1` may be `n+1`, but we only query up to `n`, so update at `n+1` is harmless if BIT size is `n+2`). Also handle `q = 0` — then all frequencies are zero, result is 0. Time complexity: O((n+q) log n) for BIT updates/queries, plus O(n log n) for sorting, total O((n+q) log n). Space: O(n) for BIT and arrays.
