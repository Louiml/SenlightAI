// Given a vector of strings, each string consisting only of lowercase English letters, write a C++ function that computes the product (modulo \(10^9+7\)) of a special "period" value assigned to each string. For a string of length \(n\):
// - If \(n \le 1\), its period is \(1\).
// - Otherwise, the period is the smallest positive integer \(t\) such that \(t \cdot (t+1)/2\) (the \(t\)-th triangular number) is divisible by \(n\).
// After computing the periods for all strings, reduce the list so that for each index \(i\) (from \(0\) to \(len-1\)), every subsequent period \(v[j]\) for \(j>i\) is divided by the greatest common divisor of \(v[j]\) and \(v[i]\) (i.e., eliminate redundant prime factors across the whole list), but note that this reduction only happens while \(v[i] \neq 1\). Finally, return the product of all (reduced) periods modulo \(10^9+7\). The number of strings can be up to \(10^5\), and each string length can be up to \(10^9\) (so use efficient computation of periods).
#include <bits/stdc++.h>
using namespace std;
// The function from the solution is assumed to be declared above.
int main() {
    // Test cases
    assert(computeSpecialPeriodProduct({"a"}) == 1);           // n=1
    assert(computeSpecialPeriodProduct({"ab"}) == 3);          // n=2, period=3
    assert(computeSpecialPeriodProduct({"abc"}) == 2);         // n=3, period=2
    assert(computeSpecialPeriodProduct({"ab", "c"}) == 3);     // periods 3,1 => product 3
    assert(computeSpecialPeriodProduct({"ab", "abc"}) == 6);   // periods 3,2 => reduced 3,2 => product 6
    assert(computeSpecialPeriodProduct({"abcd"}) == 7);        // n=4, period=7
    assert(computeSpecialPeriodProduct({"a", "b", "c"}) == 1); // all n=1
    assert(computeSpecialPeriodProduct({"abc", "abcd"}) == 14); // periods 2,7 => reduced: 2, 7/gcd(7,2)=7 => product 14
    assert(computeSpecialPeriodProduct({"abcdef"}) == 3);      // n=6, T(3)=6 divisible by 6 => period=3
    assert(computeSpecialPeriodProduct({"ab", "ab", "ab"}) == 3); // periods 3,3,3 => reduction: i=0: v[1]=1, v[2]=1 => product 3
    cout << "All tests passed!" << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

// Compute smallest t such that t(t+1)/2 is divisible by n.
static long long getPeriod(long long n) {
    if (n <= 1) return 1;
    long long m = 2 * n;
    // Factor m into prime powers.
    vector<pair<long long,int>> factors;
    long long temp = m;
    for (long long p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            factors.push_back({p, cnt});
        }
    }
    if (temp > 1) factors.push_back({temp, 1});

    // Generate all divisors of m via recursion over prime factors.
    vector<long long> divisors;
    function<void(int,long long)> gen = [&](int idx, long long cur) {
        if (idx == (int)factors.size()) {
            divisors.push_back(cur);
            return;
        }
        long long p = factors[idx].first;
        long long pow = 1;
        for (int e = 0; e <= factors[idx].second; ++e) {
            gen(idx + 1, cur * pow);
            pow *= p;
        }
    };
    gen(0, 1);

    long long best = LLONG_MAX;
    for (long long a : divisors) {
        long long b = m / a;
        if (std::gcd(a, b) != 1) continue;
        // Solve t ≡ 0 (mod a), t ≡ -1 (mod b) using CRT.
        // Since gcd(a,b)=1, b has an inverse modulo a.
        if (a == 1) {
            // t ≡ -1 (mod b) => smallest positive is b-1 (if b>1), else 0? but n>=2 => m>=4, b>=1.
            if (b > 1) best = min(best, b - 1);
            else best = min(best, 1LL); // b=1 => a=m, t ≡ 0 mod m, smallest positive is m
            continue;
        }
        if (b == 1) {
            // t ≡ 0 (mod a) => smallest positive is a
            best = min(best, a);
            continue;
        }
        // Extended Euclidean to find inverse of b modulo a.
        long long t = 0, newt = 1;
        long long r = a, newr = b;
        while (newr != 0) {
            long long q = r / newr;
            long long tt = t - q * newt;
            t = newt;
            newt = tt;
            long long rr = r - q * newr;
            r = newr;
            newr = rr;
        }
        // r is gcd, which is 1.
        long long inv_b_mod_a = (t % a + a) % a;
        // x = a * ( (inv_b_mod_a * (-1)) mod a )
        long long tmp = ((inv_b_mod_a * (a - 1)) % a + a) % a; // -1 mod a is a-1
        long long candidate = (a * tmp) % (a * b);
        if (candidate == 0) candidate = a * b;
        best = min(best, candidate);
    }
    return best;
}

// Main function: compute product of reduced periods modulo MOD.
long long computeSpecialPeriodProduct(const vector<string>& A) {
    int len = (int)A.size();
    vector<long long> v(len);
    static unordered_map<long long, long long> cache;
    for (int k = 0; k < len; ++k) {
        long long n = (long long)A[k].size();
        if (cache.find(n) != cache.end()) {
            v[k] = cache[n];
        } else {
            v[k] = getPeriod(n);
            cache[n] = v[k];
        }
    }

    long long ans = 1;
    for (int i = 0; i < len; ++i) {
        if (v[i] != 1) {
            for (int j = i + 1; j < len; ++j) {
                v[j] = v[j] / std::gcd(v[j], v[i]);
            }
        }
        ans = (ans * (v[i] % MOD)) % MOD;
    }
    return ans;
}
// For a string length \(n\), we need the smallest \(t\) such that \(t(t+1)\) is divisible by \(2n\). Because \(\gcd(t, t+1)=1\), each prime power factor of \(2n\) must be fully contained in either \(t\) or \(t+1\). Therefore, choose any divisor \(a\) of \(2n\) such that \(\gcd(a, 2n/a)=1\) and solve the system \(t \equiv 0 \pmod a\), \(t \equiv -1 \pmod {2n/a}\) via the Chinese Remainder Theorem. The period is the smallest positive solution among all such coprime divisor pairs. To find all candidate pairs, factor \(2n\) to generate all divisors; for each divisor \(d\), check if \(\gcd(d, 2n/d)=1\). Use caching to avoid recomputing periods for repeated lengths. After obtaining the periods, perform the given reduction by dividing later values by the gcd with earlier non-1 values. The final product is taken modulo \(10^9+7\). The overall time complexity is \(O(L \cdot D)\) where \(L\) is the number of strings and \(D\) is the maximum number of divisors of \(2n\) (at most a few thousand), plus the cost of factoring each distinct \(2n\), which is \(O(\sqrt{n})\) per unique length. Space complexity is \(O(L + U)\) for the period array and the cache of unique lengths.
