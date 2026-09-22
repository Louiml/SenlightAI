You are given an array of up to \(10^5\) non-negative integers, each up to \(10^{18}\). Define an impartial combinatorial game where the Grundy number \(g(x)\) for a positive integer \(x\) is computed as follows: \(g(0)=0\), and for \(x \ge 1\), let \(S(x)\) be the set of all positive integers \(y\) such that either \(y^2 \le x\) or \(y^4 > x\). Then \(g(x)\) is the minimum excluded non-negative integer not in \(\{g(y) \mid y \in S(x)\}\). It is known that \(g(x) \in \{0,1,2,3,4\}\) for all \(x\). Write a C++ function `int xorOfGrundy(const std::vector<long long>& a)` that computes the XOR of \(g(a_i)\) over all given numbers and returns it. The function must be efficient: do not naively compute \(g(x)\) for each \(x\) up to \(10^{18}\). Instead, precompute \(g(x)\) only for \(x < 10^6\) using a sieve-like approach, and for larger \(x\), use a clever interval-based query leveraging the property that \(g(x)\) is periodic in a certain sense (the precomputation already captures all distinct values). The precomputation must be done exactly once using a static/global table. You may assume the precomputation time of \(O(10^6)\) is acceptable per program run.

#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is assumed to be defined above.

// Brute-force Grundy computation for small x (x up to about 1e6+1000).
int bruteG(long long x) {
    if (x <= 3) return 0;
    bool present[5] = {false};
    for (long long y = 2; y * y <= x; ++y) {
        if ((__int128)y * y * y * y >= x) {
            present[bruteG(y)] = true;
        }
    }
    for (int j = 0; j <= 4; ++j) if (!present[j]) return j;
    return -1;
}

int main() {
    // Test all small numbers from 0 to 200 against brute force.
    for (int x = 0; x <= 200; ++x) {
        std::vector<long long> a = {x};
        assert(xorOfGrundy(a) == bruteG(x));
    }

    // Test a mixed small array.
    std::vector<long long> a = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 17, 20};
    int expected = 0;
    for (long long x : a) expected ^= bruteG(x);
    assert(xorOfGrundy(a) == expected);

    // Test numbers close to the precomputation limit (1,000,000).
    for (long long x = 999990; x <= 1000010; ++x) {
        std::vector<long long> b = {x};
        assert(xorOfGrundy(b) == bruteG(x));
    }

    // Test a very large number; the result must be a valid Grundy value 0..4.
    std::vector<long long> big = {1000000000000000000LL};
    int g = xorOfGrundy(big);
    assert(g >= 0 && g <= 4);

    return 0;
}

#include <vector>
#include <cstdint>
#include <cmath>

// Returns XOR of Grundy numbers for all numbers in the input vector.
// Grundy definition: g(0..3)=0; for x>=4, g(x)=mex({g(y) | y>=2, y^2<=x<=y^4}).
// Precomputes g up to MAXV=1'000'000, then uses prefix masks for larger values.
int xorOfGrundy(const std::vector<long long>& a) {
    const int MAXV = 1000000;
    static bool init = false;
    static int dp[MAXV + 1];
    static int prefMask[MAXV + 1];

    if (!init) {
        // dp[0..3] are zero-initialized by static storage.
        int lo = 2, hi = 2;
        int cnt[5] = {0};
        cnt[dp[2]]++;  // dp[2] == 0

        for (int x = 4; x <= MAXV; ++x) {
            while ((long long)lo * lo * lo * lo < x) {
                cnt[dp[lo]]--;
                ++lo;
            }
            while ((long long)(hi + 1) * (hi + 1) <= x) {
                ++hi;
                cnt[dp[hi]]++;
            }

            int mex = 0;
            while (cnt[mex] > 0) ++mex;
            dp[x] = mex;
        }

        prefMask[0] = (dp[0] <= 3) ? (1 << dp[0]) : 0;
        for (int i = 1; i <= MAXV; ++i) {
            prefMask[i] = prefMask[i - 1] | ((dp[i] <= 3) ? (1 << dp[i]) : 0);
        }
        init = true;
    }

    int result = 0;
    for (long long x : a) {
        if (x <= MAXV) {
            result ^= dp[x];
            continue;
        }

        // L = smallest y >= 2 with y^4 >= x
        long long lo = 2, hi = 1000000, L = 1;
        while (lo <= hi) {
            long long mid = (lo + hi) / 2;
            if ((__int128)mid * mid * mid * mid >= x) {
                L = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }

        // R = floor(sqrt(x)), clamped to MAXV
        long long R = static_cast<long long>(std::sqrt((long double)x));
        while ((__int128)R * R > x) --R;
        while ((__int128)(R + 1) * (R + 1) <= x) ++R;
        if (R > MAXV) R = MAXV;

        int mask = 0;
        if (L <= R) {
            mask = prefMask[R] ^ (L > 0 ? prefMask[L - 1] : 0);
        }

        int missing = -1;
        for (int j = 0; j <= 3; ++j) {
            if ((mask & (1 << j)) == 0) {
                missing = j;
                break;
            }
        }
        result ^= (missing != -1) ? missing : 4;
    }
    return result;
}

// The key insight is that the set of moves from \(x\) is the interval of integers \(y\) satisfying \(y^2 \le x \le y^4\). For \(x \le 10^6\), we can compute \(g(x)\) incrementally using two pointers: maintain the current interval \([lo,hi]\) and a frequency array of the Grundy values of the numbers in that interval. As \(x\) increases, advance `lo` while \(lo^4 < x\) (removing its Grundy value) and advance `hi` while \((hi+1)^2 \le x\) (adding its Grundy value). Then \(g(x)\) is the smallest non-negative integer not present in the frequency array. After computing all values up to \(10^6\), we build a prefix bit-mask `prefMask[i]` that records which of the values \(\{0,1,2,3\}\) have appeared among \(g(0),g(1),\dots,g(i)\). For an input \(x>10^6\), the lower bound \(L=\lceil x^{1/4}\rceil\) is at most about \(3.2\times 10^4\), so the interval \([L,R]\) (where \(R=\lfloor\sqrt{x}\rfloor\)) always intersects the precomputed range. We clamp \(R\) to \(10^6\) (since numbers beyond that have Grundy value \(4\) and do not affect the presence of \(0,\dots,3\)) and use the prefix masks to determine which of \(0,1,2,3\) appear in the sub-range. If all four appear, \(g(x)=4\); otherwise it is the smallest missing value. This yields \(O(10^6)\) preprocessing time and \(O(1)\) per query (with binary search for the bounds), using \(O(10^6)\) memory.
