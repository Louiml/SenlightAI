Write a C++ function that maintains a dynamic array of `long long` integers under range updates and range sum queries, but with two distinct update types: type 1 multiplies every element in a given range by a specified factor modulo a given prime, and type 2 adds a specified value to every element in the range modulo the same prime. The function should take the array size, the modulo value, the initial array, and a list of operations (each being either a range multiply, range add, or range sum query) and return the results of all range sum queries in order. All operations and values are non-negative, and the modulo is a positive integer (not necessarily prime). The function must handle `1 <= n <= 100000`, `1 <= m <= 100000`, and `1 <= p <= 10^9`, with operation parameters `0 <= k < p` for updates and `1 <= l <= r <= n` for ranges. The updates must use lazy propagation to achieve efficiency.

// The solution uses a segment tree with lazy propagation storing two tags per node: a multiplication factor `tagc` and an addition value `tagj`. For a node covering interval `[L,R]`, the actual value of the node is `(original_value * tagc + (R - L + 1) * tagj) % p`. The key insight is that when a multiplication by `k` is applied, both tags must be updated as `tagc = (tagc * k) % p` and `tagj = (tagj * k) % p`. When an addition by `k` is applied, only the addition tag is incremented (`tagj = (tagj + k) % p`) and the node sum is increased by `(interval_length * k) % p`. During `pushdown`, children receive the parent's tags in this order: first multiply the child's sum and tags by `tagc`, then add `tagj` to the child's sum (scaled by child's length) and to its `tagj`. This order ensures that multiple lazy tags compose correctly. For range sum queries, the function returns the sum modulo `p` after applying any pending lazy tags via `pushdown` along the path. Edge cases include: when `p == 1`, all results are `0` since everything is taken mod `1`; when intervals are fully covered, no `pushdown` is needed; when `k == 0` for multiplication, the range effectively becomes zeros. Time complexity is `O((n + m) log n)` for building and each operation, space complexity is `O(n)` for the segment tree arrays.

#include <vector>

// Segment tree with lazy propagation for range multiply, range add, and range sum modulo p.
// Returns a vector of sums for each query operation (type 3).
// operations: each is a tuple {type, l, r, k}. type=1 multiply, type=2 add, type=3 query.
std::vector<long long> rangeUpdateSumMod(
    int n,
    long long p,
    const std::vector<long long>& initial,
    const std::vector<std::tuple<int,int,int,long long>>& operations) {
    
    const long long MOD = p;
    // Tree arrays (index 1-based)
    std::vector<long long> sum(4*n+5), mul(4*n+5), add(4*n+5);
    
    // Build
    auto build = [&](auto&& self, int node, int l, int r) -> void {
        mul[node] = 1;
        add[node] = 0;
        if (l == r) {
            sum[node] = initial[l-1] % MOD;
            return;
        }
        int mid = (l + r) / 2;
        self(self, node*2, l, mid);
        self(self, node*2+1, mid+1, r);
        sum[node] = (sum[node*2] + sum[node*2+1]) % MOD;
    };
    build(build, 1, 1, n);
    
    // Push down lazy tags from node to children
    auto pushdown = [&](int node, int l, int r) -> void {
        if (mul[node] != 1 || add[node] != 0) {
            int mid = (l + r) / 2;
            int left = node*2, right = node*2+1;
            
            // Apply parent's multiplication to left child
            sum[left] = (sum[left] * mul[node]) % MOD;
            mul[left] = (mul[left] * mul[node]) % MOD;
            add[left] = (add[left] * mul[node]) % MOD;
            
            // Apply parent's addition to left child (scaled by length)
            sum[left] = (sum[left] + ((mid - l + 1) % MOD) * add[node]) % MOD;
            add[left] = (add[left] + add[node]) % MOD;
            
            // Apply to right child similarly
            sum[right] = (sum[right] * mul[node]) % MOD;
            mul[right] = (mul[right] * mul[node]) % MOD;
            add[right] = (add[right] * mul[node]) % MOD;
            
            sum[right] = (sum[right] + ((r - mid) % MOD) * add[node]) % MOD;
            add[right] = (add[right] + add[node]) % MOD;
            
            mul[node] = 1;
            add[node] = 0;
        }
    };
    
    // Range update
    auto update = [&](auto&& self, int node, int l, int r, int ql, int qr, long long k, int type) -> void {
        if (ql <= l && r <= qr) {
            if (type == 1) { // multiply
                sum[node] = (sum[node] * k) % MOD;
                mul[node] = (mul[node] * k) % MOD;
                add[node] = (add[node] * k) % MOD;
            } else { // add
                long long len = (r - l + 1) % MOD;
                sum[node] = (sum[node] + len * k) % MOD;
                add[node] = (add[node] + k) % MOD;
            }
            return;
        }
        pushdown(node, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) self(self, node*2, l, mid, ql, qr, k, type);
        if (qr > mid) self(self, node*2+1, mid+1, r, ql, qr, k, type);
        sum[node] = (sum[node*2] + sum[node*2+1]) % MOD;
    };
    
    // Range query
    auto query = [&](auto&& self, int node, int l, int r, int ql, int qr) -> long long {
        if (ql <= l && r <= qr) return sum[node] % MOD;
        pushdown(node, l, r);
        int mid = (l + r) / 2;
        long long res = 0;
        if (ql <= mid) res = (res + self(self, node*2, l, mid, ql, qr)) % MOD;
        if (qr > mid) res = (res + self(self, node*2+1, mid+1, r, ql, qr)) % MOD;
        return res % MOD;
    };
    
    // Process operations
    std::vector<long long> results;
    for (const auto& op : operations) {
        int type = std::get<0>(op);
        int l = std::get<1>(op);
        int r = std::get<2>(op);
        long long k = std::get<3>(op);
        if (type == 1 || type == 2) {
            update(update, 1, 1, n, l, r, k % MOD, type);
        } else { // type 3 query
            results.push_back(query(query, 1, 1, n, l, r));
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <tuple>

// Function declaration (included from solution)
// ...

int main() {
    // Test 1: Basic multiply and add
    {
        std::vector<long long> init = {1, 2, 3, 4, 5};
        std::vector<std::tuple<int,int,int,long long>> ops = {
            {2, 1, 5, 10},      // add 10 to all -> [11,12,13,14,15]
            {1, 2, 4, 2},       // multiply by 2 on [2,4] -> [11,24,26,28,15]
            {3, 1, 5, 0}        // query all -> sum = 11+24+26+28+15=104
        };
        auto res = rangeUpdateSumMod(5, 1000000007, init, ops);
        assert(res.size() == 1);
        assert(res[0] == 104);
    }
    
    // Test 2: Modulo 10 with p=1
    {
        std::vector<long long> init = {5, 7, 9};
        std::vector<std::tuple<int,int,int,long long>> ops = {
            {3, 1, 3, 0},   // query -> all mod 1 = 0
            {1, 1, 2, 3},   // multiply
            {2, 2, 3, 4},   // add
            {3, 1, 3, 0}    // query still 0
        };
        auto res = rangeUpdateSumMod(3, 1, init, ops);
        assert(res.size() == 2);
        assert(res[0] == 0 && res[1] == 0);
    }
    
    // Test 3: Multiply by 0 zeroes range
    {
        std::vector<long long> init = {2, 4, 6};
        std::vector<std::tuple<int,int,int,long long>> ops = {
            {1, 1, 3, 0},   // multiply all by 0
            {3, 1, 3, 0}    // query -> 0
        };
        auto res = rangeUpdateSumMod(3, 100, init, ops);
        assert(res.size() == 1);
        assert(res[0] == 0);
    }
    
    // Test 4: Single element
    {
        std::vector<long long> init = {42};
        std::vector<std::tuple<int,int,int,long long>> ops = {
            {2, 1, 1, 8},   // add 8 -> 50
            {3, 1, 1, 0},   // query -> 50
            {1, 1, 1, 2},   // multiply by 2 -> 100
            {3, 1, 1, 0}    // query -> 100
        };
        auto res = rangeUpdateSumMod(1, 1000, init, ops);
        assert(res.size() == 2);
        assert(res[0] == 50 && res[1] == 100);
    }
    
    // Test 5: Large modulo and mixed operations
    {
        std::vector<long long> init = {1, 1, 1, 1, 1};
        std::vector<std::tuple<int,int,int,long long>> ops = {
            {2, 1, 5, 7},    // add 7 -> all become 8
            {1, 1, 5, 3},    // multiply by 3 -> all become 24
            {3, 1, 5, 0},    // query -> sum = 120 mod 100 = 20
            {2, 2, 4, 10},   // add 10 to [2,4] -> [24,34,34,34,24]
            {3, 2, 4, 0}     // query -> 34+34+34=102 mod 100 = 2
        };
        auto res = rangeUpdateSumMod(5, 100, init, ops);
        assert(res.size() == 2);
        assert(res[0] == 20 && res[1] == 2);
    }
    
    // Test 6: Empty operations
    {
        std::vector<long long> init = {10, 20};
        std::vector<std::tuple<int,int,int,long long>> ops = {};
        auto res = rangeUpdateSumMod(2, 1000, init, ops);
        assert(res.empty());
    }
    
    return 0;
}
