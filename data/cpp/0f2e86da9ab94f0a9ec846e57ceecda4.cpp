Write a C++ function `lotteryPolynomialProduct(const std::vector<std::vector<long long>>& factors)` that takes a vector of polynomials, where each polynomial is represented by its coefficient vector indexed from degree 0 upward (e.g., `{c0, c1, c2}` means \(c_0 + c_1 x + c_2 x^2\)). The function must compute the product of all these polynomials modulo a fixed prime \(MOD = 786433\) (which is a prime number of the form \(k \cdot 2^{18} + 1\)). The result should be returned as a `std::vector<long long>` containing the coefficients of the product polynomial, truncated so that trailing zero coefficients are removed (i.e., if the highest-degree coefficient is zero, it should not appear in the result). You must implement an efficient Number Theoretic Transform (NTT)‑based multiplication. The function should handle an arbitrary number of input polynomials (including zero or one), with each polynomial possibly having different degrees. The coefficients are all non‑negative and less than \(MOD\). The total size of the product (sum of degrees plus 1) may be up to \(2^{18}\). The function must not use any external libraries beyond the standard C++ headers, and it should avoid quadratic-time naive multiplication.
We need to multiply many polynomials modulo a NTT‑friendly prime. The key is to use a divide‑and‑conquer approach: recursively split the list of polynomials into two halves, compute the product of each half, and then multiply the two resulting polynomials using NTT. The NTT requires a primitive root of the modulus. For \(MOD = 786433\), the prime factorization of \(MOD-1 = 786432 = 2^{18} \cdot 3\), so it supports transform lengths up to \(2^{18}\). We find a primitive root by testing candidates \(g\) such that \(g^{(MOD-1)/2} \neq 1\) and \(g^{(MOD-1)/3} \neq 1\). The NTT implementation uses precomputed roots of unity and bit‑reversal permutation. For a transform length \(n\) (power of two), we compute the primitive \(n\)-th root as \(r = g^{(MOD-1)/n}\). The forward transform computes the polynomial evaluation at these roots, pointwise multiplication is performed, and the inverse transform uses the same roots with a direction flag and multiplies by \(n^{-1}\). Edge cases: if there are zero polynomials, the product is an empty polynomial (or we can treat it as the constant 1). If there is one polynomial, return it unchanged. If a polynomial is empty (zero polynomial), the product should be an empty vector. The divide‑and‑conquer reduces the number of NTT operations to \(O(\log m)\) multiplications of large polynomials, and each multiplication takes \(O(L \log L)\) time where \(L\) is the transform size needed. The total time complexity is \(O(M \log^2 M)\) where \(M\) is the total output degree, but in practice it is \(O(M \log M \log m)\) because each level of divide‑and‑conquer does NTTs for polynomials of total size \(M\) across all nodes. Space complexity is \(O(M)\) for the global buffers used in NTT.
#include <vector>
#include <cstdint>
#include <algorithm>

const long long MOD = 786433;
const long long MAX_N = 1 << 18;

static long long primRoot = -1;
static long long rev[MAX_N];
static long long w[MAX_N];
static long long inv_n;

long long modPow(long long base, long long exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

long long findPrimitiveRoot() {
    std::vector<long long> factors;
    long long phi = MOD - 1;
    long long n = phi;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors.push_back(n);
    for (long long g = 2; g <= MOD; ++g) {
        bool ok = true;
        for (long long factor : factors) {
            if (modPow(g, phi / factor) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

void prepareNTT(long long n) {
    if (primRoot == -1) primRoot = findPrimitiveRoot();
    long long sz = 0;
    while ((1LL << sz) < n) ++sz;
    long long r = modPow(primRoot, (MOD - 1) / n);
    inv_n = modPow(n, MOD - 2);
    w[0] = w[n] = 1;
    for (long long i = 1; i < n; ++i) w[i] = (w[i - 1] * r) % MOD;
    rev[0] = 0;
    for (long long i = 1; i < n; ++i) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (sz - 1));
    }
}

void ntt(std::vector<long long>& a, long long n, bool invert) {
    for (long long i = 1; i < n - 1; ++i) {
        if (i < rev[i]) std::swap(a[i], a[rev[i]]);
    }
    for (long long len = 2; len <= n; len <<= 1) {
        for (long long i = 0; i < n; i += len) {
            for (long long j = 0; j < (len >> 1); ++j) {
                long long idx = invert ? n - n / len * j : n / len * j;
                long long u = a[i + j];
                long long v = (a[i + j + (len >> 1)] * w[idx]) % MOD;
                long long sum = u + v;
                if (sum >= MOD) sum -= MOD;
                long long diff = u - v;
                if (diff < 0) diff += MOD;
                a[i + j] = sum;
                a[i + j + (len >> 1)] = diff;
            }
        }
    }
    if (invert) {
        for (long long i = 0; i < n; ++i) a[i] = (a[i] * inv_n) % MOD;
    }
}

std::vector<long long> multiplyPolynomials(const std::vector<long long>& p, const std::vector<long long>& q) {
    if (p.empty() || q.empty()) return {};
    long long n = p.size(), m = q.size();
    long long t = n + m - 1;
    long long sz = 1;
    while (sz < t) sz <<= 1;
    prepareNTT(sz);
    std::vector<long long> fa(sz, 0), fb(sz, 0);
    for (long long i = 0; i < n; ++i) fa[i] = p[i];
    for (long long i = 0; i < m; ++i) fb[i] = q[i];
    ntt(fa, sz, false);
    ntt(fb, sz, false);
    for (long long i = 0; i < sz; ++i) fa[i] = (fa[i] * fb[i]) % MOD;
    ntt(fa, sz, true);
    fa.resize(t);
    while (!fa.empty() && fa.back() == 0) fa.pop_back();
    return fa;
}

std::vector<long long> solveRange(const std::vector<std::vector<long long>>& polys, long long l, long long r) {
    if (l > r) return {1}; // empty product is constant 1
    if (l == r) return polys[l];
    long long mid = (l + r) / 2;
    auto left = solveRange(polys, l, mid);
    auto right = solveRange(polys, mid + 1, r);
    return multiplyPolynomials(left, right);
}

std::vector<long long> lotteryPolynomialProduct(const std::vector<std::vector<long long>>& factors) {
    if (factors.empty()) return {1};
    return solveRange(factors, 0, static_cast<long long>(factors.size()) - 1);
}
#include <cassert>
#include <vector>
#include <iostream>

// Declaration (assume the solution code is included above)
std::vector<long long> lotteryPolynomialProduct(const std::vector<std::vector<long long>>& factors);

int main() {
    // Test 1: Product of two constants: (2) * (3) = 6
    std::vector<std::vector<long long>> f1 = {{2}, {3}};
    assert(lotteryPolynomialProduct(f1) == std::vector<long long>{6});

    // Test 2: Product of (1 + x) and (1 - x) => 1 - x^2, but mod 786433, -1 becomes MOD-1
    std::vector<std::vector<long long>> f2 = {{1, 1}, {1, MOD-1}};
    auto r2 = lotteryPolynomialProduct(f2);
    assert(r2.size() == 2);
    assert(r2[0] == 1);
    assert(r2[1] == 0); // coefficient of x is 1*1 + 1*(MOD-1) = 0 mod MOD
    // but trailing zero removed after multiplication? Actually 0 coefficient at middle stays, trailing zeros removed

    // Test 3: Product of three simple polynomials: x * x * x = x^3
    std::vector<std::vector<long long>> f3 = {{0,1}, {0,1}, {0,1}};
    auto r3 = lotteryPolynomialProduct(f3);
    assert(r3.size() == 4 && r3[0]==0 && r3[1]==0 && r3[2]==0 && r3[3]==1);

    // Test 4: Empty list should return {1}
    std::vector<std::vector<long long>> f4 = {};
    assert(lotteryPolynomialProduct(f4) == std::vector<long long>{1});

    // Test 5: Single polynomial (2 + 3x) returns unchanged
    std::vector<std::vector<long long>> f5 = {{2,3}};
    assert(lotteryPolynomialProduct(f5) == std::vector<long long>{2,3});

    // Test 6: Product of (1+x) and (1+x) = 1 + 2x + x^2
    std::vector<std::vector<long long>> f6 = {{1,1},{1,1}};
    auto r6 = lotteryPolynomialProduct(f6);
    assert(r6.size() == 3 && r6[0]==1 && r6[1]==2 && r6[2]==1);

    // Test 7: Product with a zero polynomial => empty vector (zero polynomial)
    std::vector<std::vector<long long>> f7 = {{1,1}, {0}};
    assert(lotteryPolynomialProduct(f7).empty());

    // Test 8: Product of (1) * (5) * (7) = 35
    std::vector<std::vector<long long>> f8 = {{1},{5},{7}};
    auto r8 = lotteryPolynomialProduct(f8);
    assert(r8.size() == 1 && r8[0] == 35);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
