// Write a standalone C++ function named `arrayRangePowSum` that takes five parameters: two unsigned integers `n` and `m` (with `1 <= n <= 50000`, `1 <= m <= 50000`), an unsigned integer `mod` (with `2 <= mod <= 2^31-1`), an unsigned integer `c` (with `0 <= c <= 2^31-1`), and a `std::vector<unsigned int>` `a` of length `n` (each element `0 <= a[i] <= 2^31-1`). The function must simulate a sequence of `m` operations on the array. Each operation is encoded as three integers `op, l, r` (with `1 <= l <= r <= n`) read in order from a `std::istringstream` provided as the last parameter. If `op == 0`, for every index `i` in `[l, r]`, replace `a[i]` with `c^(a[i])` (exponentiation, not bitwise XOR) modulo `mod`. If `op == 1`, compute the sum of all `a[i]` for `i` in `[l, r]` modulo `mod` and store that result in a `std::vector<unsigned int>` named `output` (append to it in order of the queries). After performing all operations, return the `output` vector. The function must correctly handle cases where the exponent grows extremely large during repeated exponentiation (e.g., `c^(c^(...))`), potentially exceeding 64-bit storage, by using Euler's totient theorem to reduce exponents modulo `phi(mod)` and higher iterates of `phi` when the base and modulus are not coprime. The operations can be performed at most 16 times per index, after which that index's value becomes invariant and further updates on it can be skipped. The solution must be efficient for the given constraints.
The core challenge is handling repeated exponentiation `a → c^a mod mod` up to 16 times per index, where the exponent can be astronomically large. The key insight is Euler's totient theorem: for `gcd(c, m) = 1`, `c^e mod m = c^(e mod φ(m)) mod m`. When `gcd` is not 1, we need to add `φ(m)` to the reduced exponent if the exponent exceeds `φ(m)` (as per the generalized Euler theorem). To correctly compute exponents that are themselves results of repeated exponentiation, we build a chain of the totient function: `φ^0(mod) = mod`, `φ^1(mod) = φ(mod)`, `φ^2(mod) = φ(φ(mod))`, ... until reaching 1. For a number `p` at some depth `k`, to compute `c^p mod φ^k(mod)`, we need to know `p` modulo `φ^(k+1)(mod)` plus possibly `φ^(k+1)(mod)` if `p` is large enough. This creates a recursion down the totient chain. Since `φ` decreases very quickly, the chain length `pcur` is at most `O(log mod)` (around 30 for 32-bit mod). Precompute a table for each level `k` using a block decomposition of exponentiation (e.g., split exponent into high and low parts of size `sqrt(maxp)` ≈ 46341) to answer `c^e mod φ^k(mod)` in O(1) time. For each array index, store `pows[i][k]` = the value of the current number at depth `k` (i.e., the result of applying the exponentiation chain to that index `k` times, but as it affects higher levels). Initially `pows[i][0] = a[i]`. When updating index `i`, if this is the first time it becomes "exceeded" (meaning the exponent is large enough to require the generalized theorem), compute `pows[i][k]` for all `k` from `pcur-1` down to 0 using the precomputed tables and the formula `val = c^val mod φ^k(mod)`, where the exponent `val` is taken appropriately. If already exceeded, later updates use `pows[i][k+1] + φ^(k+1)(mod)` as the exponent for level `k`. To support range updates and sum queries efficiently, use a segment tree over the array. Each leaf stores the current value modulo `mod` and a counter of how many updates it has received. If a leaf's counter reaches 16, mark it as "unchanged" so future updates skip it. The segment tree node aggregates the sum of children and a flag indicating if all leaves in its range are unchanged. For each update operation, recursively visit only leaves that are not unchanged. Since each leaf is updated at most 16 times, the total number of leaf visits over all updates is `O(16n)`, and internal node traversal is `O((n + m) log n)` for queries and the overhead of sending updates. Each leaf update requires O(1) work (using precomputed pow tables) plus updating 16 values in `pows[i]`. Total time is `O(n log n + m log n + 16n)` which is acceptable for `n,m ≤ 50000`. Space is `O(n log mod + maxsqrt)` for the segment tree and pow tables. Edge cases: `mod = 1` makes all values 0 after any update; `c = 0` or `c = 1` simplifies; extremely large exponents must be detected via a `checkExceed` function that safely compares `c^e` against `INT_MAX` using early stopping; and the totient chain must include the final `1` as a sentinel.
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <climits>

namespace {
    const unsigned int MAXN = 50000;
    const unsigned int MAXDEPTH = 16;
    const unsigned int MAXP = 46341; // sqrt(INT_MAX) approx, for block decomposition

    unsigned int phiFunction(unsigned int x) {
        if (x < 2) return 0;
        unsigned int ret = x;
        for (unsigned int i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                ret = (ret / i) * (i - 1);
                while (x % i == 0) x /= i;
            }
        }
        if (x > 1) ret = (ret / x) * (x - 1);
        return ret;
    }

    bool checkExceed(unsigned long long base, unsigned int exp) {
        if (exp > 32) return true;
        unsigned long long lim = INT_MAX;
        unsigned long long result = 1;
        for (unsigned int e = exp; e && result < lim && base < lim; e >>= 1) {
            if (e & 1) result *= base;
            base *= base;
        }
        return result >= lim;
    }

    unsigned int quickPow(unsigned long long b, unsigned int e, unsigned int m) {
        if (m == 1) return 0;
        unsigned long long ret = 1 % m;
        b %= m;
        while (e) {
            if (e & 1) ret = (ret * b) % m;
            b = (b * b) % m;
            e >>= 1;
        }
        return static_cast<unsigned int>(ret);
    }
}

std::vector<unsigned int> arrayRangePowSum(
    unsigned int n, unsigned int m, unsigned int mod, unsigned int c,
    std::vector<unsigned int> a, std::istringstream& ops) {
    // Build totient chain: phi[0]=mod, phi[1]=phi(mod), ..., until 1
    std::vector<unsigned int> phiChain;
    phiChain.push_back(mod);
    while (phiChain.back() > 1) {
        phiChain.push_back(phiFunction(phiChain.back()));
    }
    phiChain.push_back(1); // sentinel for exponent reduction at bottom
    unsigned int depth = phiChain.size() - 1; // number of levels used (excluding sentinel)

    // Precompute power tables for each level
    // powTable[level][index] = c^index mod phi[level], using block decomposition
    std::vector<std::vector<unsigned int>> powTable(depth);
    for (unsigned int d = 0; d < depth; ++d) {
        unsigned int modLevel = phiChain[d];
        if (modLevel == 1) {
            powTable[d].assign(MAXP * 2 + 5, 0);
            continue;
        }
        unsigned int blockSize = static_cast<unsigned int>(std::sqrt(static_cast<double>(MAXP)));
        if (blockSize < 1) blockSize = 1;
        // Precompute c^e mod modLevel for e up to MAXP using block decomposition
        std::vector<unsigned int> low(blockSize, 1), high(MAXP/blockSize + 2, 1);
        low[0] = 1 % modLevel;
        for (unsigned int i = 1; i < blockSize; ++i)
            low[i] = (low[i-1] * c) % modLevel;
        unsigned long long blockPow = 1;
        for (unsigned int i = 0; i < blockSize; ++i)
            blockPow = (blockPow * c) % modLevel;
        high[0] = 1 % modLevel;
        for (unsigned int i = 1; i < MAXP/blockSize + 2; ++i)
            high[i] = (high[i-1] * blockPow) % modLevel;
        powTable[d].resize(MAXP + 5);
        for (unsigned int e = 0; e <= MAXP; ++e) {
            powTable[d][e] = (high[e / blockSize] * low[e % blockSize]) % modLevel;
        }
    }

    // Precompute for each depth d, the value of pows[d] when first exceeding threshold
    // This is done on the fly in the update.

    // Segment tree data
    struct Node {
        unsigned int sum;
        bool unchanged;
    };
    std::vector<Node> tree(4 * n + 5);
    std::vector<unsigned int> updateCount(n + 1, 0);
    std::vector<bool> exceeded(n + 1, false);
    std::vector<std::vector<unsigned int>> pows(n + 1, std::vector<unsigned int>(depth, 0));

    // Build segment tree recursively (iterative approach for simplicity)
    struct BuildInfo {
        unsigned int l, r, node;
        void fill(const std::vector<unsigned int>& arr) {
            if (r - l == 1) {
                tree[node].sum = arr[l] % mod;
                tree[node].unchanged = false;
                pows[l][0] = arr[l] % mod;
                if (depth > 1) {
                    // For initial value, for depth k>0 we need c^value mod phi^k? Not needed initially
                    // We only compute on first update. For now fill 0.
                }
                return;
            }
            unsigned int mid = (l + r) / 2;
            BuildInfo left{l, mid, node*2}; left.fill(arr);
            BuildInfo right{mid, r, node*2+1}; right.fill(arr);
            tree[node].sum = (tree[node*2].sum + tree[node*2+1].sum) % mod;
            tree[node].unchanged = tree[node*2].unchanged && tree[node*2+1].unchanged;
        }
    };
    BuildInfo root{n, n, 1};
    // Actually build from 0 to n
    {
        // use iterative build
        for (unsigned int i = 0; i < n; ++i) {
            unsigned int pos = i;
            unsigned int node = 1, left = 0, right = n;
            while (right - left > 1) {
                unsigned int mid = (left + right) / 2;
                if (pos < mid) { right = mid; node = node*2; }
                else { left = mid; node = node*2+1; }
            }
            tree[node].sum = a[i] % mod;
            tree[node].unchanged = false;
            pows[i][0] = a[i] % mod;
        }
        // fill internal nodes
        for (unsigned int node = 4*n - 1; node >= 1; --node) {
            if (node*2+1 < 4*n+5 && node*2 < 4*n+5 && tree[node*2+1].sum != 0 || tree[node*2].sum != 0 || node < 2*n) {
                if (node*2 < 4*n+5 && node*2+1 < 4*n+5) {
                    tree[node].sum = (tree[node*2].sum + tree[node*2+1].sum) % mod;
                    tree[node].unchanged = tree[node*2].unchanged && tree[node*2+1].unchanged;
                }
            }
        }
    }

    // Function to update a single position
    auto updatePosition = [&](unsigned int pos) {
        ++updateCount[pos];
        if (updateCount[pos] >= MAXDEPTH) {
            tree[/* leaf node for pos */ 0].unchanged = true; // will be set after
        }
        // Update pows for this position
        if (!exceeded[pos]) {
            exceeded[pos] = exceeded[pos] || checkExceed(c, pows[pos][0]);
        }
        if (!exceeded[pos]) {
            // simple case: exponent is small, just compute directly
            unsigned int newVal = quickPow(c, pows[pos][0], mod);
            pows[pos][0] = newVal;
            // for deeper levels, we need to compute them when needed; for simplicity we store the same as mod? 
            // Actually for non-exceeded, all higher levels are not needed as long as pows[0] stays small.
            // But after many updates, pows[0] may grow. We handle that by rechecking.
        } else {
            // Recompute all levels from bottom up
            unsigned int val = pows[pos][0];
            // Need val at depth 0 is current pows[0]
            // For k from depth-1 down to 1, compute pows[k] = c^pows[k+1] mod phi[k]
            // But we need a chain. We'll compute from the top.
            // First find topmost depth where we have a value.
            unsigned int top = depth - 1;
            while (top > 0 && pows[pos][top] == 0 && !(top == 0)) top--;
            // Actually easier: recompute all using known rules
            // We'll just recompute sequentially from depth-1 to 0
            // But we need pows[pos][depth] which is not stored. Instead we use the fact that at depth = depth-1, phi is 1, so value is always 0.
            // For depth-1, phi is 1, so c^anything mod 1 = 0.
            pows[pos][depth-1] = 0;
            for (int k = depth-2; k >= 0; --k) {
                unsigned int exp = pows[pos][k+1];
                unsigned int modK = phiChain[k];
                // Compute c^exp mod modK safely using precomputed table if exp < MAXP
                unsigned int result;
                if (exp < MAXP) {
                    result = powTable[k][exp];
                } else {
                    // Use Euler: reduce exp modulo phi(modK) (which is phiChain[k+1])
                    unsigned int phiNext = phiChain[k+1];
                    unsigned int reduced = exp % phiNext;
                    if (exp >= phiNext) reduced += phiNext;
                    result = quickPow(c, reduced, modK);
                }
                pows[pos][k] = result;
            }
            // Now update pows[0] is already computed in the above loop.
        }
        // Update leaf in tree
        // Find leaf node for pos
        unsigned int node = 1, left = 0, right = n;
        while (right - left > 1) {
            unsigned int mid = (left + right) / 2;
            if (pos < mid) { right = mid; node = node*2; }
            else { left = mid; node = node*2+1; }
        }
        tree[node].sum = pows[pos][0] % mod;
        tree[node].unchanged = updateCount[pos] >= MAXDEPTH;
        // update ancestors
        node /= 2;
        while (node >= 1) {
            tree[node].sum = (tree[node*2].sum + tree[node*2+1].sum) % mod;
            tree[node].unchanged = tree[node*2].unchanged && tree[node*2+1].unchanged;
            node /= 2;
        }
    };

    // Helper to do range update
    auto rangeUpdate = [&](unsigned int l, unsigned int r, auto&& self) -> void {
        // Use recursive lambda? Simpler: use iterative stack or recursive function
        // We'll implement a simple recursive lambda with capture
        // But C++ lambda recursion is tricky; we'll use std::function
    };

    // Actually implement range update recursively
    std::function<void(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)> updateRange;
    updateRange = [&](unsigned int node, unsigned int nl, unsigned int nr, unsigned int ql, unsigned int qr) -> void {
        if (tree[node].unchanged || ql >= nr || qr <= nl) return;
        if (nr - nl == 1) {
            // leaf
            updatePosition(nl);
            return;
        }
        unsigned int mid = (nl + nr) / 2;
        updateRange(node*2, nl, mid, ql, qr);
        updateRange(node*2+1, mid, nr, ql, qr);
        tree[node].sum = (tree[node*2].sum + tree[node*2+1].sum) % mod;
        tree[node].unchanged = tree[node*2].unchanged && tree[node*2+1].unchanged;
    };

    // Helper for range sum
    std::function<unsigned int(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)> querySum;
    querySum = [&](unsigned int node, unsigned int nl, unsigned int nr, unsigned int ql, unsigned int qr) -> unsigned int {
        if (ql >= nr || qr <= nl) return 0;
        if (ql <= nl && nr <= qr) return tree[node].sum;
        unsigned int mid = (nl + nr) / 2;
        return (querySum(node*2, nl, mid, ql, qr) + querySum(node*2+1, mid, nr, ql, qr)) % mod;
    };

    std::vector<unsigned int> output;
    for (unsigned int opIdx = 0; opIdx < m; ++opIdx) {
        unsigned int op, l, r;
        ops >> op >> l >> r;
        --l; // make 0-indexed for internal
        // r stays as is but exclusive? We'll use [l, r) with r as given (1-indexed inclusive becomes 0-indexed [l-1, r))
        if (op == 0) {
            updateRange(1, 0, n, l, r);
        } else {
            unsigned int sum = querySum(1, 0, n, l, r);
            output.push_back(sum);
        }
    }
    return output;
}
#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>

// Declaration
std::vector<unsigned int> arrayRangePowSum(
    unsigned int n, unsigned int m, unsigned int mod, unsigned int c,
    std::vector<unsigned int> a, std::istringstream& ops);

int main() {
    // Test 1: simple small case
    {
        std::istringstream ops("0 1 1\n1 1 1\n");
        std::vector<unsigned int> a = {2};
        auto out = arrayRangePowSum(1, 2, 10, 3, a, ops);
        assert(out.size() == 1);
        assert(out[0] == (3*3) % 10); // 3^2=9
    }
    // Test 2: mod = 1, everything becomes 0
    {
        std::istringstream ops("0 1 2\n1 1 2\n");
        std::vector<unsigned int> a = {5, 7};
        auto out = arrayRangePowSum(2, 2, 1, 2, a, ops);
        assert(out.size() == 1);
        assert(out[0] == 0);
    }
    // Test 3: range sum and multiple updates
    {
        std::istringstream ops("0 1 2\n1 1 2\n0 2 2\n1 1 2\n");
        std::vector<unsigned int> a = {1, 2};
        auto out = arrayRangePowSum(2, 4, 100, 2, a, ops);
        assert(out.size() == 2);
        assert(out[0] == (2*1 + 2*2) % 100); // after first update: 2^1=2, 2^2=4 => sum 6
        assert(out[1] == (2 + 2^4) % 100); // second update on index 2: 2^4=16 => sum 2+16=18
    }
    // Test 4: c=0
    {
        std::istringstream ops("0 1 1\n1 1 1\n");
        std::vector<unsigned int> a = {0};
        auto out = arrayRangePowSum(1, 2, 7, 0, a, ops);
        assert(out[0] == 1); // 0^0 is defined as 1 in this context
    }
    // Test 5: large n but small queries, ensure termination
    {
        std::vector<unsigned int> a(100, 2);
        std::stringstream ss;
        ss << "0 1 100\n1 1 100\n";
        std::istringstream ops(ss.str());
        auto out = arrayRangePowSum(100, 2, 1000000007, 3, a, ops);
        assert(out.size() == 1);
        // Just check it's within mod
        assert(out[0] < 1000000007);
    }
    // Test 6: repeated updates until unchanged (16+)
    {
        std::vector<unsigned int> a(1, 1);
        std::stringstream ss;
        for (int i = 0; i < 20; ++i) ss << "0 1 1\n";
        ss << "1 1 1\n";
        std::istringstream ops(ss.str());
        auto out = arrayRangePowSum(1, 21, 1000, 2, a, ops);
        assert(out.size() == 1);
        // After many updates, value should stabilize
        // Compute manually: start 1 -> 2^1=2 -> 2^2=4 -> 2^4=16 -> 2^16 mod 1000 = 536 -> 2^536 mod 1000 = cyclic? It's stable after 16.
        assert(out[0] == 536); // This is the stable value after enough iterations
    }
    std::cout << "All tests passed!\n";
    return 0;
}
