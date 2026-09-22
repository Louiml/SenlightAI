// You are given a sequence of operations that build and modify a dynamic list of numbers. Initially, the list is empty. The operations are of three types:
// - Type 1: Given an index `k` (1-based) and a positive integer `v`, add `v` to every element in the list from position 1 up to position `k` (if `k` is larger than the current list length, apply to all current elements).
// - Type 2: Given a positive integer `v`, append `v` to the end of the list. After the operation, the list length increases by 1.
// - Type 3: Remove the last element of the list. If the list is empty, do nothing.
// After every operation, output the average value of all elements in the list (sum divided by current length). If the list is empty, output `0.0000000000`. The input begins with an integer `n` (number of operations), then `n` lines follow, each starting with the type `t` (1, 2, or 3). For type 1, the next two integers are `k` and `v`; for type 2, the next integer is `v`; for type 3, no more values. All values fit in 64-bit signed integers. Write a function `std::vector<double> simulateOperations(int n, const std::vector<std::vector<long long>>& ops)` that returns a vector of `n` doubles, each being the average after that operation, with at least `10` decimal places of precision (absolute or relative error ≤ 1e-9).

#include <cassert>
#include <cmath>
#include <vector>
#include <cstdint>

// Include the solution function here

int main() {
    // Simple case: append 10, append 20, then average
    {
        std::vector<std::vector<long long>> ops = {{2, 10}, {2, 20}, {3}};
        auto res = simulateOperations(3, ops);
        assert(std::abs(res[0] - 10.0) < 1e-9);
        assert(std::abs(res[1] - 15.0) < 1e-9);
        assert(std::abs(res[2] - 10.0) < 1e-9);
    }
    // Type 1 prefix add
    {
        std::vector<std::vector<long long>> ops = {{2, 5}, {1, 1, 3}, {3}};
        auto res = simulateOperations(3, ops);
        // after append: [5], avg=5
        // after prefix add to index1: [8], avg=8
        // after pop: [] avg=0
        assert(std::abs(res[0] - 5.0) < 1e-9);
        assert(std::abs(res[1] - 8.0) < 1e-9);
        assert(std::abs(res[2] - 0.0) < 1e-9);
    }
    // Type 1 with k larger than list size
    {
        std::vector<std::vector<long long>> ops = {{2, 1}, {1, 100, 2}, {3}, {2, 4}};
        auto res = simulateOperations(4, ops);
        // [1] avg=1
        // add 2 to all (since k=100 > len=1): [3] avg=3
        // pop: [] avg=0
        // append 4: [4] avg=4
        assert(std::abs(res[0] - 1.0) < 1e-9);
        assert(std::abs(res[1] - 3.0) < 1e-9);
        assert(std::abs(res[2] - 0.0) < 1e-9);
        assert(std::abs(res[3] - 4.0) < 1e-9);
    }
    // Many operations with large numbers
    {
        std::vector<std::vector<long long>> ops;
        for (int i = 0; i < 5; ++i) ops.push_back({2, 1000000000});
        ops.push_back({1, 3, 500000000});
        auto res = simulateOperations(6, ops);
        // After appends: [1e9]*5 avg=1e9
        // Add 5e8 to first 3: [1.5e9,1.5e9,1.5e9,1e9,1e9] sum=6.5e9 avg=1.3e9
        assert(std::abs(res[5] - 1.3e9) < 1e-1); // precision tolerance for double
    }
    // Pop empty list does nothing
    {
        std::vector<std::vector<long long>> ops = {{3, 0}, {2, 7}};
        auto res = simulateOperations(2, ops);
        assert(std::abs(res[0] - 0.0) < 1e-9);
        assert(std::abs(res[1] - 7.0) < 1e-9);
    }
    // Mixed operations
    {
        std::vector<std::vector<long long>> ops = {{2, 10}, {1, 2, 5}, {2, 20}, {1, 1, 100}, {3}, {2, 1}, {1, 10, 2}};
        auto res = simulateOperations(7, ops);
        // Step-by-step expected:
        // op1: [10] avg=10
        // op2: add 5 to first 2 (only index0) => [15] avg=15
        // op3: append 20 => [15,20] avg=17.5
        // op4: add 100 to first 1 => [115,20] avg=67.5
        // op5: pop last => [115] avg=115
        // op6: append 1 => [115,1] avg=58
        // op7: add 2 to first 10 (all) => [117,3] avg=60
        assert(std::abs(res[0] - 10.0) < 1e-9);
        assert(std::abs(res[1] - 15.0) < 1e-9);
        assert(std::abs(res[2] - 17.5) < 1e-9);
        assert(std::abs(res[3] - 67.5) < 1e-9);
        assert(std::abs(res[4] - 115.0) < 1e-9);
        assert(std::abs(res[5] - 58.0) < 1e-9);
        assert(std::abs(res[6] - 60.0) < 1e-9);
    }
    return 0;
}

#include <vector>
#include <cstdint>

class LazySegTree {
    static const int MAXN = 1 << 18;
    long long t[2 * MAXN];
    long long add[2 * MAXN];

    void push(int v, int tl, int tr) {
        t[v] += add[v] * (tr - tl + 1);
        if (tl != tr) {
            add[v * 2] += add[v];
            add[v * 2 + 1] += add[v];
        }
        add[v] = 0;
    }

    void update(int v, int tl, int tr, int l, int r, long long x) {
        push(v, tl, tr);
        if (l > r) return;
        if (tl == l && tr == r) {
            add[v] += x;
            push(v, tl, tr);
            return;
        }
        int tm = (tl + tr) >> 1;
        update(v * 2, tl, tm, l, std::min(r, tm), x);
        update(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, x);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }

    long long query(int v, int tl, int tr, int l, int r) {
        push(v, tl, tr);
        if (l > r) return 0;
        if (l == tl && r == tr) return t[v];
        int tm = (tl + tr) >> 1;
        return query(v * 2, tl, tm, l, std::min(r, tm)) +
               query(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r);
    }

public:
    LazySegTree() {
        for (int i = 0; i < 2 * MAXN; ++i) {
            t[i] = 0;
            add[i] = 0;
        }
    }

    void addRange(int l, int r, long long x) {
        update(1, 0, MAXN - 1, l, r, x);
    }

    long long getSum(int l, int r) {
        return query(1, 0, MAXN - 1, l, r);
    }
};

std::vector<double> simulateOperations(int n, const std::vector<std::vector<long long>>& ops) {
    LazySegTree seg;
    int cur = 0;
    std::vector<double> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        int type = (int)ops[i][0];
        if (type == 1) {
            long long k = ops[i][1];
            long long v = ops[i][2];
            int right = (int)std::min(k, (long long)cur) - 1;
            if (right >= 0) {
                seg.addRange(0, right, v);
            }
        } else if (type == 2) {
            long long v = ops[i][1];
            seg.addRange(cur, cur, v);
            ++cur;
        } else { // type 3
            if (cur > 0) {
                --cur;
                long long val = seg.getSum(cur, cur);
                seg.addRange(cur, cur, -val);
            }
        }
        if (cur == 0) {
            result.push_back(0.0);
        } else {
            long long total = seg.getSum(0, (1 << 18) - 1);
            result.push_back(static_cast<double>(total) / cur);
        }
    }
    return result;
}

// The key observation is that all operations affect either a prefix (type 1) or the last position (types 2 and 3). A lazy segment tree over a fixed large range (say `MAXN = 1 << 18`) supports range add and point query in `O(log MAXN)` time. We maintain a pointer `cur` representing the current length of the list. For type 1, we add `v` to the range `[0, min(k-1, cur-1)]`. For type 2, we add `v` to the point `cur` (position `cur`), then increment `cur`. For type 3, we first read the value at position `cur-1` using a point sum query, then subtract that value from that point (to set it to 0) and decrement `cur`. The total sum is obtained by querying the whole range `[0, MAXN-1]` after each operation. The average is sum divided by `cur`; if `cur == 0`, output 0. Time complexity: each operation performs at most two segment tree queries/updates, each `O(log MAXN)`, so total `O(n log MAXN)`. Space complexity: `O(MAXN)` for the segment tree. Edge cases: empty list (cur=0), type 1 with k larger than current length, and type 3 on empty list (should do nothing—just output 0).
