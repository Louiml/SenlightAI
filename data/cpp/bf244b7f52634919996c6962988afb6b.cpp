Given positive integers \(n\) and \(m\) with \(1 \le n,m \le 10^6\), write a C++ function `fallingBinomialSum` that computes the sum  
\[
\sum_{i=1}^{\min(n,m)} (m+1-i) \cdot (m+2-i) \cdots m? 
\]
Actually, the snippet computes \(\sum_{i=1}^{\min(n,m)} P(m,i) \cdot C(n,i-1)\) modulo \(998244353\), where \(P(m,i) = m(m-1)\cdots(m-i+1)\) is the falling factorial, and \(C(n,i-1)\) is the binomial coefficient. More precisely, the term for index \(i\) is \(\text{down}[i] \cdot \text{comb}[i-1]\), where \(\text{down}[i] = m(m-1)\cdots(m-i+1)\) and \(\text{comb}[i-1] = \binom{n}{i-1}\). Return the sum modulo \(998244353\). The function should accept two `long long` parameters `n` and `m`, and return a `long long` representing the result. This formula arises from combinatorial counting problems (e.g., counting injective mappings with a fixed point condition). Handle many test cases efficiently by precomputing modular inverses up to \(10^6\) once, but the function itself should compute the sum for a single pair \((n,m)\). The modulus is prime \(998244353\). Ensure computations use 64-bit types to avoid overflow before taking mod.

#include <cassert>

int main() {
    precomputeInverses();
    // Test small values manually verifiable.
    // n=1, m=1: i=1, down=1, comb=C(1,0)=1 => ans=1
    assert(fallingBinomialSum(1, 1) == 1);
    // n=2, m=2:
    // i=1: down=2, comb=1 => 2
    // i=2: down=2*1=2, comb=C(2,1)=2 => 4 => total 6
    assert(fallingBinomialSum(2, 2) == 6);
    // n=3, m=2:
    // i=1: down=2, comb=1 => 2
    // i=2: down=2*1=2, comb=C(3,1)=3 => 6 => total 8
    assert(fallingBinomialSum(3, 2) == 8);
    // n=2, m=3:
    // i=1: down=3, comb=1 => 3
    // i=2: down=3*2=6, comb=C(2,1)=2 => 12 => total 15
    assert(fallingBinomialSum(2, 3) == 15);
    // n=100, m=1: only i=1, down=1, comb=1 => ans=1
    assert(fallingBinomialSum(100, 1) == 1);
    // n=1, m=100: only i=1, down=100, comb=1 => ans=100
    assert(fallingBinomialSum(1, 100) == 100);
    // Large values, check modulo consistency: n=1000000, m=1000000, we just ensure it does not crash and returns within [0, MOD).
    long long res = fallingBinomialSum(1000000, 1000000);
    assert(res >= 0 && res < MOD);
    // Test known result: n=5, m=5 manually: 
    // i=1: down=5, comb=1 => 5
    // i=2: down=20, comb=5 => 100
    // i=3: down=60, comb=10 => 600
    // i=4: down=120, comb=10 => 1200
    // i=5: down=120, comb=5 => 600
    // total=2505
    assert(fallingBinomialSum(5, 5) == 2505);
    return 0;
}

#include <vector>
#include <cstdint>

// Precomputed modular inverses up to MAXN (suggested to be 1000000).
static const long long MOD = 998244353LL;
static const int MAXN = 1000000;
static std::vector<long long> inv;

// Must be called once before using fallingBinomialSum, e.g., in main.
void precomputeInverses() {
    inv.resize(MAXN + 1);
    inv[1] = 1;
    for (int i = 2; i <= MAXN; ++i) {
        inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
    }
}

// Computes sum_{i=1}^{min(n,m)} P(m,i) * C(n, i-1) modulo MOD.
// Requires precomputeInverses() to have been called.
long long fallingBinomialSum(long long n, long long m) {
    long long limit = std::min(n, m);
    long long down = 1;       // P(m, i) for current i
    long long comb = 1;       // C(n, i-1) for current i-1 (starting with i-1=0)
    long long ans = 0;
    for (long long i = 1; i <= limit; ++i) {
        down = down * ((m + 1 - i) % MOD) % MOD;
        // comb currently is C(n, i-1). To compute next comb for next iteration, but we use current.
        ans = (ans + down * comb) % MOD;
        // Now update comb to C(n, i) for next iteration (i+1)
        comb = comb * ((n - i) % MOD) % MOD * inv[i] % MOD;
    }
    return ans;
}

// The term \(\text{down}[i]\) is the falling factorial \(m^{\underline{i}} = m(m-1)\cdots(m-i+1)\). The term \(\text{comb}[i-1]\) is \(\binom{n}{i-1}\). The sum ranges over \(i=1\) to \(\min(n,m)\). We compute these iteratively: initialize \(\text{down}[0]=1\) and \(\text{comb}[0]=1\). For each \(i\) from 1 to \(k=\min(n,m)\), update:
// - \(\text{down}[i] = \text{down}[i-1] \cdot ((m+1-i) \mod \text{MOD}) \mod \text{MOD}\)
// - \(\text{comb}[i] = \text{comb}[i-1] \cdot ((n-i) \mod \text{MOD}) \cdot \text{inv}[i] \mod \text{MOD}\), where \(\text{inv}[i]\) is the modular inverse of \(i\) modulo \(\text{MOD}\).
//
// Then accumulate \(\text{down}[i] \cdot \text{comb}[i-1]\) into the answer. Precompute modular inverses for all integers up to \(10^6\) using the recurrence \(\text{inv}[i] = \text{MOD} - (\text{MOD}/i) \cdot \text{inv}[\text{MOD} \% i] \% \text{MOD}\). Edge cases: when \(\min(n,m)=0\), return 0 (though constraints say positive, but handle gracefully). When \(m < i\), \(\text{down}[i]\) becomes zero because one factor becomes zero modulo MOD (since \(m+1-i\) could be zero if \(m+1-i\) is multiple of MOD, but MOD is large ~1e9, while \(m \le 1e6\), so never zero). So no special handling needed beyond range. Time complexity per call is \(O(\min(n,m))\) and space is \(O(1)\) auxiliary (if precomputed inverses are passed or computed globally). Precomputation of inverses is O(MAX) time and O(MAX) space.
