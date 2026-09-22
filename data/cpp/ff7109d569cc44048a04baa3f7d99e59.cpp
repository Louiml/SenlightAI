Write a C++ function `std::vector<std::vector<int>> generateWelchCostasArrays(int p)` that takes a prime number `p > 2` and returns a vector of all distinct Costas arrays of order `p-1` generated using the Welch construction. A Welch Costas array of order `n = p-1` is constructed from a primitive root `g` of the prime `p` by placing a dot at row `i` and column `j` if `j = g^i mod p` (using 1-indexed positions based on the usual `1..p-1` range). The function must return all distinct arrays obtained from *all* primitive roots of `p` (excluding duplicates, since different primitive roots may yield the same array or a rotated/reflected version). For each primitive root, the array can be represented as a vector of length `n` where the value at index `i` (0-based) is `g^i mod p` (with values in `[1, p-1]`), directly corresponding to the column position of the dot in row `i+1`. The output should be a vector of such vectors, one for each primitive root, in the order the primitive roots are found (increasing order of the primitive root value). For example, for `p = 7`, primitive roots are `{3, 5}`, and the function returns `{ {1,3,2,6,4,5}, {1,5,4,6,2,3} }` (verify these are indeed Costas arrays). Ensure the function uses modular exponentiation efficiently and correctly handles the mathematical definitions; assume the input is always a prime greater than 2. Do not include a `main` function in your solution; provide only the function. The function must be self-contained (include all necessary headers and helper functions) and use `const` where appropriate.

The solution requires three main steps: (1) Find all primitive roots of the prime `p`. A primitive root `g` is an integer `1 < g < p` such that the order of `g` modulo `p` is exactly `p-1`, equivalently, the set `{g^1 mod p, g^2 mod p, ..., g^(p-1) mod p}` is a permutation of `{1,2,...,p-1}`. We can find all primitive roots by testing each candidate `g` from 2 to `p-1`. For each candidate, compute all powers `g^j mod p` for `j = 1..p-1` (using fast modular exponentiation `mypow`); if the set of results = `{1..p-1}` exactly once, then `g` is a primitive root. To avoid O(p) work per candidate with a full set check, we can instead check that `g^((p-1)/q) mod p != 1` for every prime divisor `q` of `p-1`—but a simpler and clear approach for typical small `p` is to generate the full power set and check it forms a permutation. For robustness, we can use the order condition: compute the factorization of `p-1` and test that `g^((p-1)/prime_factor) % p != 1` for all distinct prime factors. That is efficient (O(log p) time per candidate using exponentiation). (2) For each primitive root, construct the Costas array vector of length `p-1`: `costas[i] = mypow(g, i, p)` for `i = 0..p-2` (which gives `g^0=1`, `g^1`, ..., `g^(p-2)`, but note the standard representation uses `g^0..g^(p-2)`; however, the prompt says the array has length `p-1` and index `i` (0-based) holds `g^i mod p`. For a primitive root, the sequence of powers `g^0, g^1, ..., g^(p-2)` is a permutation of `{1,2,...,p-1}`? Actually `g^(p-1)=1 mod p`, so `g^0=1`, and `g^1..g^(p-2)` cover `2..p-1` plus `1` again? Wait: the order is `p-1`, so `g^0=1`, `g^1` through `g^(p-1)` are all distinct, and `g^(p-1)=1`. So the sequence `g^0, g^1, ..., g^(p-2)` gives `{1, g, g^2, ..., g^(p-2)}` which covers all nonzero residues exactly once? Since `g^(p-1)=1`, the set `{g^0, g^1, ..., g^(p-2)}` is a complete set of `p-1` distinct values, covering `1..p-1` but with `1` appearing at position 0 and also `g^(p-1)` would be 1 but that is not included. So yes, it is a permutation of `{1,...,p-1}`. So use `i=0..p-2`. (3) Return the vector of all such arrays, one per primitive root, in increasing order of the primitive root. Edge cases: `p=3` has only one primitive root (2) and array length 2, which is correct; the function should work for any prime >2. Time complexity: For each candidate `g` (there are O(p) candidates), we check primality of divisors or do set validation; with the order condition, each check uses at most O(log p) exponentiations of O(log p) time each, so overall O(p log^2 p) roughly. Space complexity: O(p) for storing output and temporary arrays. We must be careful with modular exponentiation to avoid overflow: use `long long` for intermediate multiplication. Also handle `p=3` correctly.

#include <vector>
#include <set>
#include <cmath>

// Helper: modular exponentiation (base^exp % mod) using fast exponentiation.
int modPow(int base, int exp, int mod) {
    long long result = 1;
    long long b = base % mod;
    long long e = exp;
    while (e > 0) {
        if (e & 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e >>= 1;
    }
    return static_cast<int>(result);
}

// Helper: compute distinct prime factors of n.
std::vector<int> distinctPrimeFactors(int n) {
    std::vector<int> factors;
    int temp = n;
    for (int d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            factors.push_back(d);
            while (temp % d == 0) {
                temp /= d;
            }
        }
    }
    if (temp > 1) {
        factors.push_back(temp);
    }
    return factors;
}

// Helper: check if g is a primitive root modulo prime p.
// Condition: for every prime factor q of p-1, g^((p-1)/q) % p != 1.
bool isPrimitiveRoot(int g, int p) {
    if (g <= 1 || g >= p) return false;
    int phi = p - 1;
    std::vector<int> primeFactors = distinctPrimeFactors(phi);
    for (int q : primeFactors) {
        if (modPow(g, phi / q, p) == 1) {
            return false;
        }
    }
    return true;
}

// Main function: generate all Welch Costas arrays of order p-1 for prime p.
std::vector<std::vector<int>> generateWelchCostasArrays(int p) {
    std::vector<std::vector<int>> result;
    if (p <= 2) return result; // not valid, but return empty

    // Find all primitive roots in increasing order.
    for (int g = 2; g < p; ++g) {
        if (isPrimitiveRoot(g, p)) {
            std::vector<int> costas(p - 1);
            for (int i = 0; i < p - 1; ++i) {
                costas[i] = modPow(g, i, p);
            }
            result.push_back(costas);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <set>

// The function to test is declared above; include it here in the same translation unit.

int main() {
    // Test p=3: only primitive root is 2, array length 2: {1,2}
    auto res3 = generateWelchCostasArrays(3);
    assert(res3.size() == 1);
    assert(res3[0] == std::vector<int>({1, 2}));

    // Test p=5: primitive roots are 2 and 3.
    auto res5 = generateWelchCostasArrays(5);
    assert(res5.size() == 2);
    // For g=2: powers: 2^0=1, 2^1=2, 2^2=4, 2^3=3 -> {1,2,4,3}
    // For g=3: powers: 3^0=1, 3^1=3, 3^2=4, 3^3=2 -> {1,3,4,2}
    assert(res5[0] == std::vector<int>({1, 2, 4, 3}));
    assert(res5[1] == std::vector<int>({1, 3, 4, 2}));

    // Test p=7: primitive roots are 3 and 5.
    auto res7 = generateWelchCostasArrays(7);
    assert(res7.size() == 2);
    // For g=3: powers: 3^0=1, 3^1=3, 3^2=2, 3^3=6, 3^4=4, 3^5=5 -> {1,3,2,6,4,5}
    // For g=5: powers: 5^0=1, 5^1=5, 5^2=4, 5^3=6, 5^4=2, 5^5=3 -> {1,5,4,6,2,3}
    assert(res7[0] == std::vector<int>({1, 3, 2, 6, 4, 5}));
    assert(res7[1] == std::vector<int>({1, 5, 4, 6, 2, 3}));

    // Test p=11: there are phi(10)=4 primitive roots: 2,6,7,8.
    auto res11 = generateWelchCostasArrays(11);
    assert(res11.size() == 4);
    // Verify each output is a permutation of 1..10.
    for (const auto& arr : res11) {
        assert(arr.size() == 10);
        std::set<int> s(arr.begin(), arr.end());
        assert(s.size() == 10 && *s.begin() >= 1 && *s.rbegin() <= 10);
    }

    // Also verify that the arrays are indeed Costas arrays (for small p, check no repeated differences).
    // A simple check: for all pairs (i,j) with i<j, the difference vector (j-i, arr[j]-arr[i]) must be unique.
    for (const auto& arr : res11) {
        std::set<std::pair<int,int>> diffs;
        int n = arr.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                auto key = std::make_pair(j - i, arr[j] - arr[i]);
                assert(diffs.count(key) == 0);
                diffs.insert(key);
            }
        }
    }

    // Edge: p=2 is invalid but should return empty.
    auto res2 = generateWelchCostasArrays(2);
    assert(res2.empty());

    return 0;
}
