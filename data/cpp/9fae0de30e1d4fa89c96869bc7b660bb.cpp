/*
Given an array of integers `A` of length `N` and another array `B` of length `M` (each integer is positive, at least 1), define a function `weight(x)` that returns the sum of the exponents in the prime factorization of `x`, where each prime factor that appears in `B` contributes its exponent negatively, and every other prime contributes positively. Then, consider partitioning the array `A` into contiguous non-empty blocks. The total score of a partition is: for each block, we subtract (block length) * weight(gcd of all elements in the block) from the sum of the weights of all individual elements `weight(A[i])`. More precisely, if `C[i] = weight(A[i])` and `G[l..r] = gcd(A[l], ..., A[r])`, then for a partition into blocks `[l1,r1], [l2,r2], ..., [lk,rk]`, the final answer is `sum_{i=1}^N C[i] - sum_{j=1}^k (r_j - l_j + 1) * weight(G[l_j..r_j])`. You must output the maximum possible final score over all valid partitions (including the trivial partition where each element is its own block, and the single-block partition). Write a function `long long maximumScore(const std::vector<long long>& A, const std::vector<long long>& B)` that returns this maximum value.
*/
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using u128 = __uint128_t;

// --- Miller-Rabin / Pollard-Rho (simplified) ---
struct m64 {
    using u64 = uint64_t;
    using u128 = __uint128_t;
    inline static u64 m, r, n2;
    static void set_mod(u64 m) {
        assert(m < (1ull << 62) && (m & 1));
        m64::m = m;
        n2 = -u128(m) % m;
        r = m;
        for (int _ = 0; _ < 5; ++_) r *= 2 - m*r;
        r = -r;
    }
    static u64 reduce(u128 b) { return (b + u128(u64(b) * r) * m) >> 64; }
    u64 x;
    m64() : x(0) {}
    m64(u64 x) : x(reduce(u128(x) * n2)) {}
    u64 val() const { u64 y = reduce(x); return y >= m ? y-m : y; }
    m64 &operator*=(m64 y) { x = reduce(u128(x) * y.x); return *this; }
    m64 operator*(m64 y) const { return m64(*this) *= y; }
    m64 pow(u64 n) const {
        m64 y(1), z(*this);
        for (; n; n >>= 1, z = z*z) if (n & 1) y = y*z;
        return y;
    }
};

bool isPrime(ull x) {
    if (x < 2) return false;
    for (ull p : {2ULL,3ULL,5ULL,7ULL}) if (x == p) return true;
    if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
    if (x < 121) return x > 1;
    ull d = x-1, s = __builtin_ctzll(d);
    d >>= s;
    m64::set_mod(x);
    m64 one(1), minus_one(x-1);
    auto check = [&](ull a) {
        m64 y(a); y = y.pow(d);
        if (y.val() == 1 || y.val() == x-1) return true;
        for (ull r = 1; r < s; ++r) {
            y = y*y;
            if (y.val() == x-1) return true;
        }
        return false;
    };
    if (x < (1ull << 32)) {
        for (ull a : {2ULL,7ULL,61ULL}) if (!check(a)) return false;
    } else {
        for (ull a : {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL}) {
            if (x == a) return true;
            if (!check(a)) return false;
        }
    }
    return true;
}

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
ll rho(ll n, ll c) {
    m64::set_mod(n);
    m64 cc(c);
    auto f = [&](m64 x) { return x*x + cc; };
    m64 x(1), y(2), z(1), q(1);
    ll g = 1;
    const ll m = 1LL << (__lg(n)/5 + 1);
    for (ll r = 1; g == 1; r <<= 1) {
        x = y;
        for (ll i = 0; i < r; ++i) y = f(y);
        for (ll k = 0; k < r && g == 1; k += m) {
            z = y;
            for (ll i = 0; i < min(m, r-k); ++i) {
                y = f(y);
                q = q * (x - y);
            }
            g = std::gcd(q.val(), n);
        }
    }
    if (g == n) {
        do {
            z = f(z);
            g = std::gcd((x - z).val(), n);
        } while (g == 1);
    }
    return g;
}

ll primeFactor(ll n) {
    assert(n > 1);
    if (isPrime(n)) return n;
    for (int _ = 0; _ < 100; ++_) {
        ll m = rho(n, rng() % (n-1) + 1);
        if (isPrime(m)) return m;
        n = m;
    }
    return n; // fallback
}

vector<pair<ll,ll>> factor(ll n) {
    vector<pair<ll,ll>> pf;
    for (ll p = 2; p*p <= n && p < 100; ++p) {
        if (n % p == 0) {
            ll e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pf.emplace_back(p, e);
        }
    }
    while (n > 1) {
        ll p = primeFactor(n);
        ll e = 0;
        while (n % p == 0) { n /= p; ++e; }
        pf.emplace_back(p, e);
    }
    sort(pf.begin(), pf.end());
    return pf;
}

// The main solution function
long long maximumScore(const std::vector<long long>& A, const std::vector<long long>& B) {
    int N = (int)A.size();
    std::set<ll> bad(B.begin(), B.end());
    auto weight = [&](ll x) -> ll {
        ll w = 0;
        auto pf = factor(x);
        for (auto &pr : pf) {
            ll p = pr.first, e = pr.second;
            if (bad.count(p)) w -= e;
            else w += e;
        }
        return w;
    };
    std::vector<ll> C(N);
    for (int i = 0; i < N; ++i) C[i] = weight(A[i]);
    const ll INF = 1LL << 60;
    std::vector<ll> dp(N+1, -INF);
    dp[0] = 0;
    for (int R = 1; R <= N; ++R) {
        ll g = 0;
        for (int L = R-1; L >= 0; --L) {
            g = std::gcd(g, A[L]);
            ll w = weight(g);
            dp[R] = std::max(dp[R], dp[L] - (R - L) * w);
        }
    }
    ll total = std::accumulate(C.begin(), C.end(), 0LL);
    return total + dp[N];
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above; for brevity, assume it is available.
long long maximumScore(const std::vector<long long>& A, const std::vector<long long>& B);

int main() {
    // Example 1: simple numbers, no bad primes
    std::vector<long long> A1 = {12, 18};
    std::vector<long long> B1 = {};
    // C = [weight(12)= (2^2*3) => 2+1=3, weight(18)=2*3^2 => 1+2=3], total=6
    // Partition options: [12][18]: penalties 1*weight(12)+1*weight(18)=6 => score 0
    // [12,18]: gcd=6, weight(6)= (2,3) => 1+1=2, penalty 2*2=4 => score 2
    assert(maximumScore(A1, B1) == 2);

    // Example 2: all ones
    std::vector<long long> A2 = {1, 1, 1};
    std::vector<long long> B2 = {2};
    // weights are all 0, gcd any block is 1, weight(1)=0, so all penalties 0, total 0
    assert(maximumScore(A2, B2) == 0);

    // Example 3: bad prime reduces weight
    std::vector<long long> A3 = {8, 4};
    std::vector<long long> B3 = {2};
    // weight(8)= (2^3) => -3, weight(4)= (2^2) => -2, total=-5
    // Partition separately: penalties -3 + -2 = -5, total = -5 - (-5)? Let's compute formula:
    // total = sum(C) + dp[N]. dp[1] = max(dp[0]-1*weight(8)) = 0 -1*(-3)=3? Wait dp[0]=0, weight(8)=-3, so dp[1]=0 -1*(-3)=3
    // dp[2]: L=0: gcd(8,4)=4 weight(4)=-2, dp[0]-2*(-2)=4; L=1: dp[1]-1*weight(4)=3 -1*(-2)=5, so dp[2]=5
    // total = -5 + 5 = 0. Separate partition: -5 + (dp[1]+dp[1]?) Actually separate gives dp[2] via L=1: dp[1]-1*weight(4) = 3+2=5, same.
    assert(maximumScore(A3, B3) == 0);

    // Example 4: better to group because gcd has positive weight
    std::vector<long long> A4 = {6, 10, 15};
    std::vector<long long> B4 = {};
    // weights: 6=2*3=>2, 10=2*5=>2, 15=3*5=>2, total=6
    // dp[1] = 0-1*2 = -2
    // dp[2]: L=0: gcd(6,10)=2 weight=1, 0-2*1=-2; L=1: dp[1]-1*weight(10)= -2-2=-4, so dp[2]=-2
    // dp[3]: L=0: gcd(6,10,15)=1 weight=0, 0-3*0=0; L=1: gcd(10,15)=5 weight=1, dp[1]-2*1=-2-2=-4; L=2: dp[2]-1*weight(15)= -2-2=-4, so dp[3]=0
    // total = 6+0=6. All separate gives 6 + (-2-2-2)=0? Actually separate gives sumC + dp[3] with separate blocks = 6 + (-6)=0, so grouping all is better.
    assert(maximumScore(A4, B4) == 6);

    // Example 5: mixed, with bad prime forcing grouping? 
    std::vector<long long> A5 = {4, 9};
    std::vector<long long> B5 = {2};
    // weight(4)=-2, weight(9)= (3^2)=2, total=0
    // separate: dp[1]=0-1*(-2)=2, dp[2]: L=0 gcd(4,9)=1 weight=0, 0; L=1: dp[1]-1*weight(9)=2-2=0, so dp[2]=0
    // total=0
    assert(maximumScore(A5, B5) == 0);

    // Example 6: large prime
    std::vector<long long> A6 = {1000000007LL, 1000000009LL};
    std::vector<long long> B6 = {};
    // both are prime weight 1 each, total=2, gcd=1 weight=0, so dp[2] = max over L: L=0 gives 0-2*0=0, L=1 gives dp[1]-1*1= (0-1*1)-1 = -2, so dp[2]=0, total=2
    assert(maximumScore(A6, B6) == 2);

    // Example 7: single element
    std::vector<long long> A7 = {12};
    std::vector<long long> B7 = {3};
    // weight(12)=2^2*3 => 2-1=1, total=1, dp[1]=0-1*1=-1, total=0
    assert(maximumScore(A7, B7) == 0);

    // Example 8: all same repeats, gcd is the number itself
    std::vector<long long> A8 = {6, 6, 6};
    std::vector<long long> B8 = {2};
    // weight(6)= (2,3) => -1+1=0, total=0
    // gcd any block is 6 weight=0, so all penalties 0, total=0
    assert(maximumScore(A8, B8) == 0);

    // Example 9: mixed where grouping reduces penalty
    std::vector<long long> A9 = {2, 4, 8};
    std::vector<long long> B9 = {2};
    // weights: 2: -1, 4: -2, 8: -3, total=-6
    // dp[1]=0-1*(-1)=1? Wait dp[1]=0 -1*weight(2)= -1? Actually dp[0]=0, weight(2)=-1, so dp[1]=0 -1*(-1)=1
    // dp[2]: L=0: gcd(2,4)=2 weight=-1, 0-2*(-1)=2; L=1: dp[1]-1*weight(4)=1 -1*(-2)=3 => dp[2]=3
    // dp[3]: L=0: gcd(2,4,8)=2 weight=-1, 0-3*(-1)=3; L=1: gcd(4,8)=4 weight=-2, dp[1]-2*(-2)=1+4=5; L=2: dp[2]-1*weight(8)=3-(-3)=6 => dp[3]=6
    // total = -6+6=0. Separate would be -6 + (1+3+? actually separate gives dp[3] via L=2? Let's not overcomplicate, assert value.
    assert(maximumScore(A9, B9) == 0);

    // Example 10: negative weight forces grouping with positive gcd? 
    std::vector<long long> A10 = {6, 35};
    std::vector<long long> B10 = {2, 5};
    // weight(6)= (2,3) => -1+1=0, weight(35)= (5,7) => -1+1=0, total=0
    // gcd(6,35)=1 weight=0, so any partition gives 0
    assert(maximumScore(A10, B10) == 0);

    std::cout << "All tests passed!\n";
    return 0;
}
// The key observation is that the total score equals `sum(C) + max_over_partitions( - sum_{blocks} length * weight(gcd) )`. Since the sum of `C` is fixed, we need to maximize the negative penalty sum. Let `DP[r]` be the maximum value of `-sum_{blocks} length*weight(gcd)` for the prefix of length `r` (i.e., elements `A[0..r-1]`). The recurrence is `DP[0]=0`, and for `r>0`, `DP[r]=max_{0<=L<r} (DP[L] - (r-L)*weight(gcd(A[L..r-1])))`. Directly computing this is O(N^2). However, we can leverage the fact that as `L` decreases, the gcd of `A[L..r-1]` changes at most O(log(max A)) times because each time it changes it strictly divides the previous gcd, hence at most about 60 distinct values. For a fixed `r`, we can maintain a list of pairs `(start, g)` for the distinct gcd values of suffixes ending at `r-1`. When moving to `r+1`, we update the list by taking gcd with `A[r]` and merging adjacent equal gcds. For each distinct gcd value `g` covering a contiguous range of `L` from `L_start` to `L_end`, the term `DP[L] - (r-L)*weight(g)` is `(DP[L] + L*weight(g)) - r*weight(g)`. To maximize over a range of `L`, we need a data structure that supports range maximum of `DP[L] + L*w`. Since `weight(g)` can be positive, negative, or zero, and each `L` appears only once in each suffix-gcd range per `r`, a naive approach with a segment tree per weight is not feasible. Instead, we can use the standard trick: maintain a sparse table over `DP` and for each range of `L` (which is contiguous) and a fixed `w`, we need max of `DP[L]+L*w` over that interval. Because the number of distinct suffix gcds is O(log A) per `r`, and each range is processed in O(1) with a precomputed convex hull trick if we have static arrays? Actually, a simpler approach: observe that `DP[L]` is computed iteratively, and for a given `r`, the ranges of `L` are contiguous. We can maintain a segment tree over `L` that stores for each node the maximum `DP[L]+L*w` for all possible `w` that appear? That is heavy. Given the problem constraints are not specified, but typical competitive programming constraints (N up to 1e5, values up to 1e18) allow O(N * log(maxA) * log N) if we use a Li Chao tree for each suffix range? But the reference solution given uses O(N^2) DP? Actually the snippet computes DP in O(N^2) with a nested loop, which is acceptable only for small N. Since the task is to create a standalone programming problem, we can assume N up to 2000 or 3000, allowing O(N^2) with a simple DP but we need to optimize computing the gcd and weight. The provided snippet precomputes C[i] and then does O(N^2) DP with gcd computation on the fly using the fact that `g = gcd(g, A[R-1])` in the loop. That is O(N^2 log(maxA)). That is fine for N <= 2000. So the intended solution is: compute weight(x) via factorization (using the provided prime test and rho factorization), then compute C[i], then do O(N^2) DP. Edge cases: all elements are 1, which have weight 0; B can contain primes not necessarily dividing any A[i]; the trivial partition (each element alone) is covered by DP recurrence; also the single block partition is covered. Time complexity: factorization of each A[i] and each gcd value encountered, but we can precompute weights for all distinct divisors that appear as prefixes of gcds. Simpler: for each DP step, we compute gcd and then factor that gcd exactly once, which is O(N^2 * factor cost). The factor cost is roughly O(log n) for small primes plus Pollard-Rho for large primes. Overall complexity: O(N^2 log(maxA) + N * factorization). Space O(N). The solution below uses a simpler method: we compute C[i] once, then for DP, we maintain a list of (gcd_value, start_index) for the suffix ending at R-1, and for each such range we compute the best DP[L] - (R-L)*g_weight by iterating over L in that range? That would be O(N^2) anyway. To keep it clean and correct, we will implement the O(N^2) DP exactly as in the snippet but with factor() function included. That is acceptable for a standalone task. We'll include necessary headers and a clean factorization routine (using trial division up to 100 and Pollard-Rho for the rest). The function `weight(x)` will factor x and sum exponents with sign based on membership in B (passed as a set). The main solution function returns the maximum score.
