// You are given a stream of queries. There are two types: type 1 inserts a positive integer `x` into a set (if not already present); type 2 asks, given integers `x`, `k`, and `s`, to find a number `y` already in the set such that: `x + y <= s`, `y` is a multiple of `k`, and the bitwise XOR `(x ^ y)` is maximized. If no such `y` exists, output `-1`. Constraints: `1 <= x, k, s <= 10^5`, and the total number of queries is up to `10^5`. Write a C++ function `int solve_query(const std::vector<std::array<int,3>>& queries)` that processes the given queries (each query is represented as: type 1 → `{1, x, 0}`; type 2 → `{2, x, k, s}`? Actually, to keep a single vector, each query has 4 elements: `{type, x, k, s}` for type 2, and for type 1 use `{1, x, 0, 0}`) and returns the answer for each type‑2 query in order, as a vector of integers. The function must be efficient for large `k` and small `k`.
We need to answer maximum XOR queries with a bound on sum and a divisibility requirement. For small divisors `k < 75`, we maintain a binary trie per divisor value `d` that stores all inserted numbers as binary strings (17 bits, up to 131071). Each trie node stores whether a number passes through that prefix. For each type‑2 query with small `k`, we search the trie for `d = k` to find a number ≤ `s - x` that maximizes XOR with `x`, using a greedy bit‑by‑bit search: at each bit, prefer the opposite bit of `x` if the corresponding trie subtree contains a number whose prefix is feasible and whose maximum possible value is ≤ `s - x`. Since the trie range is fixed [0, 131071], we perform a recursive search that prunes by the upper bound `ub = s - x`. For large `k >= 75`, the number of multiples of `k` up to `10^5` is small (at most ~1333), so we iterate through them, check if inserted, and compute XOR. Edge cases: `x % k != 0` → no solution because `y` must be a multiple of `k`? Wait, the condition is `y % k == 0`, not `x % k`; but the original code checks `if (x % k != 0)` which seems off; actually the original snippet checks `x % k != 0` and outputs -1, but that is incorrect per problem statement. In our task, we require `y % k == 0` only; the check on `x % k` is not needed. However, to align with the snippet's behavior, we will keep that check? The problem statement says "y is a multiple of k" only. So we will not check `x % k`. We'll implement correctly. For small k, we also need to handle the case where `s - x` might be negative — then no valid y. Time complexity: For each small‑k query, we traverse at most 17 trie levels per branch, but pruning reduces to O(17) per query. For large‑k queries, O(s/k) iterations. Insertion is O(17 * 75) worst-case (but only for its divisors). Overall, with q up to 1e5, it's acceptable. Space: For each divisor d (1..74), a trie with at most 2^18 nodes, but only allocate lazily; use `bool` arrays per trie.
#include <bits/stdc++.h>

const int MAX_BITS = 17;
const int MAX_VAL = (1 << MAX_BITS) - 1; // 131071
const int SMALL_D = 75;

struct XorTrie {
    std::vector<std::vector<char>> trie; // trie[d] is a trie for divisor d

    XorTrie() : trie(SMALL_D) {
        for (int d = 0; d < SMALL_D; ++d) {
            trie[d].assign(1 << (MAX_BITS + 1), 0);
            trie[d][1] = 1; // root is always present
        }
    }

    void insert(int x) {
        for (int d = 1; d < SMALL_D; ++d) {
            if (x % d != 0) continue;
            int node = 1;
            for (int i = MAX_BITS - 1; i >= 0; --i) {
                int bit = (x >> i) & 1;
                node = 2 * node + bit;
                trie[d][node] = 1;
            }
        }
    }

    // returns the smallest number >= lower_bound that is present in trie[d] and <= upper_bound
    // Actually we need max XOR with x, we search greedily.
    int seek(int d, int ub, int x, int bit, int node, int xl, int xr) {
        if (xl > ub || !trie[d][node]) return -1;
        if (bit < 0) return xl;
        int xm = (xl + xr) >> 1;
        int desired_bit = ((x >> bit) & 1) ^ 1; // we want opposite bit to maximize XOR
        // Try desired bit first
        int child = 2 * node + desired_bit;
        int left = (desired_bit == 0) ? xl : xm + 1;
        int right = (desired_bit == 0) ? xm : xr;
        int res = seek(d, ub, x, bit - 1, child, left, right);
        if (res != -1) return res;
        // else try the other bit
        int other_bit = desired_bit ^ 1;
        child = 2 * node + other_bit;
        left = (other_bit == 0) ? xl : xm + 1;
        right = (other_bit == 0) ? xm : xr;
        return seek(d, ub, x, bit - 1, child, left, right);
    }
};

std::vector<int> solve_queries(const std::vector<std::array<int, 4>>& queries) {
    std::vector<int> ans;
    XorTrie trie;
    std::vector<bool> inserted(100001, false);

    for (const auto& q : queries) {
        if (q[0] == 1) {
            int x = q[1];
            if (!inserted[x]) {
                inserted[x] = true;
                trie.insert(x);
            }
        } else {
            int x = q[1], k = q[2], s = q[3];
            if (s < x) {
                ans.push_back(-1);
                continue;
            }
            int ub = s - x;
            if (ub == 0) {
                // only possible y is 0, but 0 is not positive? Problem says positive integers, but 0 not inserted.
                ans.push_back(-1);
                continue;
            }
            if (k >= SMALL_D) {
                int best_val = -1, best_num = -1;
                for (int y = k; y <= ub; y += k) {
                    if (inserted[y]) {
                        int cur = x ^ y;
                        if (cur > best_val) {
                            best_val = cur;
                            best_num = y;
                        }
                    }
                }
                ans.push_back(best_num);
            } else {
                // small k: use trie[k]
                // all numbers in trie[k] are multiples of k
                int res = trie.seek(k, ub, x, MAX_BITS - 1, 1, 0, MAX_VAL);
                ans.push_back(res);
            }
        }
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <array>

// Solution function declared above

int main() {
    // Test 1: simple insert and query
    {
        std::vector<std::array<int,4>> queries = {
            {1, 5, 0, 0},
            {1, 10, 0, 0},
            {2, 3, 5, 20} // y must be multiple of 5 and x+y<=20; candidates: 5,10,15 (if exist); inserted:5,10; XOR: 3^5=6, 3^10=9 -> best 10
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == 10);
    }

    // Test 2: no valid candidate
    {
        std::vector<std::array<int,4>> queries = {
            {1, 7, 0, 0},
            {2, 3, 2, 5} // x=3, s=5 -> ub=2; multiples of 2 ≤2: 2 not inserted → -1
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == -1);
    }

    // Test 3: large k branch and tie-breaking (prefer larger y? Our algorithm picks max XOR, and if tie picks first encountered? Actually we pick max XOR, not max y. Test: x=0, k=10, s=30, inserted 10 and 20. Both XOR 0^10=10 and 0^20=20 → max 20 → output 20)
    {
        std::vector<std::array<int,4>> queries = {
            {1, 10, 0, 0},
            {1, 20, 0, 0},
            {2, 0, 10, 30} // ub=30, multiples:10,20,30; 30 not inserted; max XOR 20
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == 20);
    }

    // Test 4: exact bound: ub equals a number
    {
        std::vector<std::array<int,4>> queries = {
            {1, 8, 0, 0},
            {2, 1, 4, 9} // ub=8; multiples of 4:4,8; inserted:8; XOR 1^8=9 -> 8
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == 8);
    }

    // Test 5: multiple queries
    {
        std::vector<std::array<int,4>> queries = {
            {1, 12, 0, 0},
            {2, 4, 3, 20}, // ub=16; multiples of 3:3,6,9,12,15; inserted:12 → XOR 4^12=8 → 12
            {1, 6, 0, 0},
            {2, 4, 3, 20} // now multiples:6,12; XOR 4^6=2, 4^12=8 → best 12
        };
        auto res = solve_queries(queries);
        assert(res.size() == 2);
        assert(res[0] == 12);
        assert(res[1] == 12);
    }

    // Test 6: large k where no candidate
    {
        std::vector<std::array<int,4>> queries = {
            {1, 100, 0, 0},
            {2, 1, 99, 200} // ub=199; multiples of 99:99,198; none inserted → -1
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == -1);
    }

    // Test 7: small k with bound at maximum
    {
        std::vector<std::array<int,4>> queries = {
            {1, 131071, 0, 0},
            {2, 0, 1, 131071} // ub=131071; multiple of 1: all; inserted 131071 → XOR = 131071 → output 131071
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == 131071);
    }

    // Test 8: duplicate insert ignored
    {
        std::vector<std::array<int,4>> queries = {
            {1, 7, 0, 0},
            {1, 7, 0, 0},
            {2, 3, 7, 20} // y=7 → XOR 3^7=4 → output 7
        };
        auto res = solve_queries(queries);
        assert(res.size() == 1);
        assert(res[0] == 7);
    }

    return 0;
}
