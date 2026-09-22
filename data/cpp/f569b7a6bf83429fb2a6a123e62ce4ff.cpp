Given a positive integer `p` (with `1 <= p <= 10^7`), write a C++ function `int tower_2_mod_p(int p)` that computes the value of the infinite power tower \(2^{2^{2^{\cdots}}}\) modulo `p`, with the convention that the tower is evaluated from the top down, and the result should be in the range `[0, p-1]`. If `p == 1`, the result is `0`. For any other `p`, the function must return the correct residue. Use the identity: for an exponent `e`, \(a^e \mod p = a^{e \mod \varphi(p) + \varphi(p)} \mod p\) when `e >= phi(p)`, and rely on the fact that the value of the tower stabilizes via recursive reduction. Precompute Euler's totient function for all values up to `10^7` using a linear sieve to make the recursion efficient. The function should be standalone (no `main`), and must handle repeated calls efficiently by referencing a precomputed array (you may compute the totients inside the function using a static flag, or include a separate helper). The main challenge is to avoid infinite recursion and correctly handle the base case where `phi(p) == 1`.
// The key observation is that the infinite tower \(2^{2^{2^{\cdots}}}\) grows extremely fast, but modulo `p` it can be evaluated using Euler's theorem generalization: for any exponent `e` (which is the value of the lower part of the tower) and modulus `p`, we have \(2^e \mod p = 2^{e \mod \varphi(p) + \varphi(p)} \mod p\) when `e >= phi(p)`. Since the tower is infinite, the exponent is always at least as large as `phi(p)` for all `p > 1`, so the reduction is valid. Define a recursive function `f(x)` that returns the tower value modulo `x`. The base case is `x == 1`, where the answer is `0` (since any number modulo 1 is 0). For `x > 1`, compute `phi(x)` using precomputed Euler's totient values. Then recursively compute `f(phi(x))`, which gives the exponent reduced modulo `phi(x)`. The actual exponent to use is `f(phi(x)) + phi(x)`. Then compute `2` raised to that exponent modulo `x` using fast modular exponentiation. The recursion terminates because `phi(x) < x` for `x > 1` (except when `x == 2`, where `phi(2)=1`, and that leads to base case). To handle up to `10^7` efficiently, precompute `phi` for all numbers up to `10^7` using a linear sieve in `O(N)` time. Since the input `p` may be much smaller, the recursion depth is at most about `log(p)`, so each query is fast. The overall time complexity for `t` queries is `O(N + t log N)` where `N = 10^7` for the sieve, and `O(1)` per query after preprocessing (actually `O(log p)` for the fast exponentiation). Space complexity is `O(N)` for the phi array and auxiliary arrays.
#include <vector>

// Precomputed Euler's totient values up to 10^7.
std::vector<int> phi;
bool precomputed = false;

// Linear sieve to compute phi for all values up to MAXV.
void precompute_phi(int MAXV) {
    phi.resize(MAXV + 1);
    std::vector<int> primes;
    std::vector<bool> is_composite(MAXV + 1, false);
    phi[1] = 1;
    for (int i = 2; i <= MAXV; ++i) {
        if (!is_composite[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int j = 0; j < (int)primes.size() && i * primes[j] <= MAXV; ++j) {
            int cur = i * primes[j];
            is_composite[cur] = true;
            if (i % primes[j] == 0) {
                phi[cur] = phi[i] * primes[j];
                break;
            } else {
                phi[cur] = phi[i] * (primes[j] - 1);
            }
        }
    }
    precomputed = true;
}

// Fast modular exponentiation: (base^exp) % mod, with long long base.
int mod_pow(long long base, int exp, int mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (int)result;
}

// Compute infinite tower 2^(2^(2^...)) modulo p.
int tower_2_mod_p(int p) {
    const int MAXV = 10000000;
    if (!precomputed) {
        precompute_phi(MAXV);
    }
    // Recursive helper to compute the tower modulo x.
    // The infinite tower is well-defined modulo any x >= 1.
    // Use a lambda or a separate function; here we define a nested function via recursion.
    // To avoid capturing phi by reference in a recursive lambda, use a helper.
    struct Helper {
        const std::vector<int>& phi;
        Helper(const std::vector<int>& ph) : phi(ph) {}
        int solve(int x) const {
            if (x == 1) return 0;
            int ph = phi[x];
            int exp_reduced = solve(ph); // this is f(phi(x))
            int exponent = exp_reduced + ph;
            return mod_pow(2, exponent, x);
        }
    };
    Helper helper(phi);
    return helper.solve(p);
}
#include <cassert>

// Forward declaration (the solution is in the same translation unit)
int tower_2_mod_p(int p);

int main() {
    // Known values:
    // tower_2_mod_p(1) = 0
    assert(tower_2_mod_p(1) == 0);
    // For p=2: 2^(anything) mod 2 = 0, but exponent is at least 1, 2^1 mod 2 = 0
    assert(tower_2_mod_p(2) == 0);
    // For p=3: 2^(large) mod 3 cycles: 2^1=2, 2^2=1, 2^3=2, so 2^even? The tower value mod 3 is 0? Actually: 2^1 mod 3 = 2, 2^2=1, 2^3=2... The infinite tower exponent is even? Let's compute: 2^2=4 mod3=1, 2^(2^2)=2^4=16 mod3=1, so result is 1. Known result: tower_2_mod_p(3) = 2? Wait, let's verify manually: The tower value is 2^(2^(2^...)). The exponent is 2^(2^...) which is even (since 2^anything is even unless exponent=0, but exponent>=1). So 2^even mod 3 = 1. So result 1. Test:
    assert(tower_2_mod_p(3) == 1);
    // For p=4: phi(4)=2, f(2)=0, exponent=0+2=2, 2^2=4 mod4=0
    assert(tower_2_mod_p(4) == 0);
    // For p=5: phi(5)=4, f(4)? phi(4)=2, f(2)=0, exponent=0+2=2, 2^2=4 mod4=0? Actually f(4)=2^2 mod4=0, exponent=0+4=4, 2^4=16 mod5=1
    assert(tower_2_mod_p(5) == 1);
    // For p=6: phi(6)=2, f(2)=0, exponent=0+2=2, 2^2=4 mod6=4
    assert(tower_2_mod_p(6) == 4);
    // For p=7: phi(7)=6, phi(6)=2, f(2)=0, exponent=0+2=2, f(6)=2^2=4, exponent=4+6=10, 2^10=1024 mod7=2
    assert(tower_2_mod_p(7) == 2);
    // For p=8: phi(8)=4, f(4)=0, exponent=0+4=4, 2^4=16 mod8=0
    assert(tower_2_mod_p(8) == 0);
    // For p=10: phi(10)=4, f(4)=0, exponent=0+4=4, 2^4=16 mod10=6
    assert(tower_2_mod_p(10) == 6);
    // For p=100: phi(100)=40, phi(40)=16, phi(16)=8, phi(8)=4, phi(4)=2, phi(2)=1, f(1)=0, f(2)=0, f(4)=2^2=4? Wait compute: f(4)=2^(f(2)+2) mod4, f(2)=0, exponent=0+2=2, 2^2=4 mod4=0. f(8)=2^(f(4)+4) mod8, f(4)=0, exponent=4, 2^4=16 mod8=0. f(16)=2^(f(8)+8) mod16, f(8)=0, exponent=8, 2^8=256 mod16=0. f(40)=2^(f(16)+16) mod40, f(16)=0, exponent=16, 2^16=65536 mod40=16? Actually 65536 mod40 = 16. So f(40)=16. Then f(100)=2^(16+40)=2^56 mod100. 2^20 mod100=76, 2^40=76^2=5776 mod100=76, 2^56=2^40*2^16=76*65536? Let's compute 2^56 mod100: cycles: 2^1=2,2^2=4,2^3=8,2^4=16,2^5=32,2^6=64,2^7=28,2^8=56,2^9=12,2^10=24,2^11=48,2^12=96,2^13=92,2^14=84,2^15=68,2^16=36,2^17=72,2^18=44,2^19=88,2^20=76,2^21=52,2^22=4? Actually period 20 after 2^2? The period of 2^k mod100 is 20 for k>=2. 56 mod20=16, so 2^56 mod100 = 2^16 mod100 = 36? Wait 2^16=65536 mod100=36. So f(100)=36. Test:
    assert(tower_2_mod_p(100) == 36);
    // Test a large prime-1 like 97? Not needed but one more: p=11, phi(11)=10, f(10)=6? From above f(10)=6, exponent=6+10=16, 2^16=65536 mod11=65536 mod11 = 65536 - 11*5957=65536-65527=9. So assert 9.
    assert(tower_2_mod_p(11) == 9);
    return 0;
}
