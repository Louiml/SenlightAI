// Given a positive integer \( n \), write a C++ function `long long countQuadraticPairs(int n)` that returns the number of ordered pairs \((a,b)\) with \( 1 \le a,b < n \) such that \( a^2 + b^2 \equiv 0 \pmod{n} \). The input \( n \) will be at least 2, and the result fits in a 64-bit signed integer. You may use any standard library or implement your own convolution using modular arithmetic with the NTT-friendly modulus 998244353.
#include <cassert>

// The solution function is defined above; include it here or link.
int main() {
    // n=2: residues 1,1. Only (1,1) works: 1+1=2≡0 mod2 → 1
    assert(countQuadraticPairs(2) == 1);
    // n=3: squares: 1^2=1, 2^2=1 → freq[1]=2. Pairs (1,1),(1,2),(2,1),(2,2): sums 2,0,0,2 mod3 → two pairs with sum 0: (1,2),(2,1)
    assert(countQuadraticPairs(3) == 2);
    // n=4: squares: 1->1,2->0,3->1 → freq[0]=1,freq[1]=2. Need sum 0 mod4: (0,0):1*1=1, (1,3): but 3 not present, (2,2): 0, (3,1):1 not present. So total 1.
    assert(countQuadraticPairs(4) == 1);
    // n=5: squares: 1,4,4,1 → freq[1]=2,freq[4]=2. Need sum 0 mod5: (0,0) none, (1,4):2*2=4, (2,3) none, (3,2) none, (4,1):2*2=4 → total 8.
    assert(countQuadraticPairs(5) == 8);
    // n=6: squares: 1,4,3,4,1 → freq[1]=2,freq[3]=1,freq[4]=2. Sum 0: (0,0) none, (1,5) none, (2,4):0*2? freq[2]=0, (3,3):1, (4,2):0, (5,1):0 → total 1? Let's brute count: ordered pairs with a,b in 1..5:
    // a=1 (sq1), b such that b^2 mod6 = 5 or 0? 5 not a square, 0 not in squares except? b= ? squares are 1,4,3,4,1. So b^2 mod6 can be 1,3,4. Need a^2 + b^2 ≡0 mod6: 1+? → need 5 mod6 not possible; 3+3=6→0, so a with sq=3 (only a=3) and b with sq=3 (only b=3) gives (3,3). Also 4+2=6 but 2 not a square. So only (3,3): 1.
    assert(countQuadraticPairs(6) == 1);
    // n=7: squares: 1,4,2,2,4,1 → freq[1]=2,freq[2]=2,freq[4]=2. Sum 0 mod7: (0,0) none, (1,6) none, (2,5) none, (3,4): freq[3]=0, (4,3):0, (5,2):0, (6,1):0. Wait also (2,5) not, but sum of two residues from {1,2,4} can be 0 mod7? 1+6 no, 2+5 no, 4+3 no, 1+2+? Actually pairs: (1,6) 6 not, (2,5) 5 not, (4,3) 3 not, (1,1)=2, (2,2)=4, (4,4)=1, (1,2)=3, (1,4)=5, (2,4)=6. None sum to 0. So 0.
    assert(countQuadraticPairs(7) == 0);
    // Small stress: brute force for n up to 20 compare
    for (int n = 2; n <= 20; ++n) {
        long long brute = 0;
        for (int a = 1; a < n; ++a)
            for (int b = 1; b < n; ++b)
                if ((1LL*a*a + 1LL*b*b) % n == 0) brute++;
        assert(countQuadraticPairs(n) == brute);
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

namespace ntt {
    const int MOD = 998244353;
    const int G = 3;

    int modpow(long long a, long long e) {
        long long r = 1;
        while (e) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return (int)r;
    }

    void ntt(vector<int>& a, bool invert) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int wlen = modpow(G, (MOD - 1) / len);
            if (invert) wlen = modpow(wlen, MOD - 2);
            for (int i = 0; i < n; i += len) {
                long long w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = a[i + j];
                    int v = (int)(a[i + j + len / 2] * w % MOD);
                    a[i + j] = (u + v) % MOD;
                    a[i + j + len / 2] = (u - v + MOD) % MOD;
                    w = w * wlen % MOD;
                }
            }
        }
        if (invert) {
            int inv_n = modpow(n, MOD - 2);
            for (int &x : a) x = (int)(1LL * x * inv_n % MOD);
        }
    }

    vector<int> convolution(const vector<int>& a, const vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        int n = 1;
        while (n < (int)(a.size() + b.size() - 1)) n <<= 1;
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n); fb.resize(n);
        ntt(fa, false); ntt(fb, false);
        for (int i = 0; i < n; i++) fa[i] = (int)(1LL * fa[i] * fb[i] % MOD);
        ntt(fa, true);
        fa.resize(a.size() + b.size() - 1);
        return fa;
    }
}

// Count ordered pairs (a,b) with 1 <= a,b < n and a^2 + b^2 ≡ 0 (mod n)
long long countQuadraticPairs(int n) {
    vector<int> freq(n, 0);
    for (int i = 1; i < n; ++i) {
        long long sq = 1LL * i * i % n;
        freq[(int)sq]++;
    }
    vector<int> conv = ntt::convolution(freq, freq);
    long long ans = conv[0];
    if (n < (int)conv.size()) ans += conv[n];
    return ans;
}
// The problem asks for the count of ordered pairs \((a,b)\) in \([1, n-1]\) satisfying \(a^2 + b^2 \equiv 0 \pmod{n}\). Let \(c[x]\) be the count of residues \(i\) in \([1, n-1]\) with \(i^2 \equiv x \pmod{n}\). Then \(a^2 \equiv x\), \(b^2 \equiv y\), and we need \(x + y \equiv 0 \pmod{n}\). For each residue \(r\) modulo \(n\), the number of ordered pairs with \(a^2 \equiv r\) and \(b^2 \equiv -r\) is \(c[r] \cdot c[n-r]\) (with \(r=0\) handled as \(c[0] \cdot c[0]\)). Naively summing over all \(r\) takes \(O(n)\) after computing \(c\), but computing \(c\) by brute force is \(O(n)\) per element, giving \(O(n^2)\) total. Instead, observe that \(c[x]\) for \(x\) ranging \(0..n-1\) can be obtained by convolving two identical arrays. Let `v[x]` be the count of \(i\) with \(i^2 \equiv x\). Then the convolution `conv = v * v` gives, at index \(t\), the sum over \(x+y \equiv t\) of \(v[x]v[y]\), which is exactly the number of ordered pairs \((a,b)\) such that \(a^2+b^2 \equiv t \pmod{n}\). Therefore `conv[0]` (with modular wrap-around) directly gives the answer. To handle the wrap-around, we need to sum `conv[t]` for all \(t\) that are congruent to 0 modulo \(n\). Since convolution length is \(2n-1\), we add `conv[0]` and `conv[n]`, but careful: `t` can be `0` or `n` (since indices go up to \(2n-2\)). However, we must also consider that the convolution index modulo \(n\) might double-count when \(n\) divides the index; indeed only `0` and `n` are in range (since \(2n-1\) < \(2n\), so no `2n`). The provided snippet computes `eqv[i]` for \(2i^2\) to correct double counting of pairs where \(a=b\) and also where both \(a^2\) and \(b^2\) produce the same residue? Actually, the snippet uses an alternative approach: it computes `r` by summing over all residues, but uses convolution to count unordered? Let's derive from first principles. The direct sum over residues is `ans = 0; for (int r=0; r<n; r++) ans += v[r] * v[(n-r)%n];` This counts ordered pairs. Since convolution gives `conv[t] = sum_{x+y=t} v[x]v[y]` with `t` up to `2n-2`, the total number of ordered pairs with `x+y` divisible by `n` is `sum_{k} conv[k*n]` for all k with `0 <= k*n < 2n-1`, i.e., k=0 (t=0) and k=1 (t=n). So answer = `conv[0] + conv[n]` (if n <= 2n-2). But be careful: `conv` is the convolution of `v` with itself, which counts ordered pairs exactly. However, the snippet in the prompt has a different formula involving `eqv` and dividing by 2, which suggests it might be counting unordered pairs or something else, but the problem statement I create is for ordered pairs. I will stick to the straightforward convolution approach. For efficiency, use NTT with modulus 998244353 if n is large, but since n up to maybe 1e5 or 1e6? The problem doesn't specify constraints, but we can assume n up to 1e5 or 2e5, where O(n log n) convolution is fine. Implementation: create vector `v` of size `n` (since residues 0..n-1). For i from 1 to n-1, increment `v[(long long)i*i % n]`. Then compute convolution using a simple O(n^2) if n is small, but better use NTT. In the solution, I will provide a self-contained NTT-based convolution function using the modulus 998244353 and primitive root 3, similar to the snippet. Then answer = `conv[0] + (n < (int)conv.size() ? conv[n] : 0)` as `long long`. Since `v` values are up to n-1, convolution values up to (n-1)^2, which fits in 64-bit. But if using NTT modulus 998244353, values are modulo that prime, which is fine because the true counts are way below that for n up to 1e5. Edge case: n=1? But n>=2. Also, when n is small, brute force is fine. The function should return the count as a 64-bit integer. Time complexity: O(n log n) for NTT, space O(n). If n is very small (<= 60), the convolution function in the snippet uses naive multiplication anyway, which is fine.
