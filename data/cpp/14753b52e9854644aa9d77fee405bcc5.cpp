Write a C++ function `int guessHiddenNumber(int hidden)`, where `hidden` is a 30-bit positive integer (between 1 and \(2^{30}-1\)) provided only for testing purposes; the function must determine `hidden` by issuing at most 30 queries to a global function `int query(int x, int y)` that returns `std::gcd(HIDDEN + x, HIDDEN + y)`, where `HIDDEN` is the secret value (the same as `hidden`). The function must return the discovered hidden number. The query function is already implemented (simulating an interactive judge) and must be called exactly as `query(a, b)` with integers `a,b` both in range `[1, 1e9]`. You may not access `HIDDEN` directly except through `query`. The task is to design a deterministic algorithm that always correctly identifies the hidden number using at most 30 queries.

#include <bits/stdc++.h>
using namespace std;

// Global hidden number for testing
int HIDDEN;

// Mock query: returns gcd(HIDDEN + x, HIDDEN + y)
int query(int x, int y) {
    return std::gcd(HIDDEN + x, HIDDEN + y);
}

// Include the solution function (as if from separate file)
int guessHiddenNumber(int hidden);

int main() {
    // Test 1: small numbers
    for (int h : {1, 2, 3, 5, 7, 10, 15, 31, 100}) {
        HIDDEN = h;
        assert(guessHiddenNumber(h) == h);
    }

    // Test 2: numbers with various bit patterns
    vector<int> tests = {
        1 << 0,
        1 << 5,
        (1 << 10) - 1,
        1 << 29,
        (1 << 30) - 1,
        123456789,
        987654321,
        536870911,  // 2^29 - 1
    };
    for (int h : tests) {
        HIDDEN = h;
        assert(guessHiddenNumber(h) == h);
    }

    // Test 3: random values (deterministic seed for reproducibility)
    mt19937 rng(42);
    uniform_int_distribution<int> dist(1, (1 << 30) - 1);
    for (int t = 0; t < 100; ++t) {
        int h = dist(rng);
        HIDDEN = h;
        assert(guessHiddenNumber(h) == h);
    }

    return 0;
}

#include <bits/stdc++.h>

// Global query function (simulates interactive judge)
int query(int x, int y);

// Determine a 30-bit hidden number using at most 30 queries.
// Precondition: hidden is between 1 and (1<<30)-1.
int guessHiddenNumber(int /*hidden*/) {
    long long add = 0; // sum of bits already determined to be 0? Actually lower bits known to be 0
    long long ans = 0; // accumulated bits known to be 1

    for (int i = 0; i < 30; ++i) {
        // Query with x = 1+add, y = 1+add+2^(i+1)
        long long a = 1 + add;
        long long b = 1 + add + (1LL << (i + 1)); // 2^(i+1)
        int rem = query(static_cast<int>(a), static_cast<int>(b));

        if (rem == (1LL << i)) {
            // bit i is 1
            ans += (1LL << i);
        } else {
            // bit i is 0, so add the 2^i to add for next queries
            add += (1LL << i);
        }
    }

    return static_cast<int>(ans);
}

// The solution exploits the fact that for any two numbers \(X\) and \(Y\), \(\gcd(H + X, H + Y)\) is related to the binary representation of \(H\). Specifically, for each bit position \(i\) (from 0 to 29), we want to determine whether bit \(i\) of `H` is 0 or 1. The key observation: if we already know the lower bits \(0..i-1\) and they sum to `add`, then we can query `Q = gcd(H + 1 + add, H + 1 + add + 2^{i+1})`. The second argument differs from the first by exactly \(2^{i+1}\). The result of `gcd` will be divisible by \(2^i\) always, but it will be divisible by \(2^{i+1}\) exactly when bit \(i\) of `H` is 0. This is because if bit \(i\) is 0, then both \(H+1+add\) and \(H+1+add+2^{i+1}\) have the same lower \(i+1\) bits (all zeros there), so their gcd gets an extra factor of 2. If bit \(i\) is 1, then the two numbers differ in that bit, so the gcd's power of 2 is only \(2^i\). Thus, we check whether `rem == (1<<i)`; if so, bit \(i\) is 1 (because the gcd didn't get the extra 2), so we add \(2^i\) to `ans`; otherwise bit is 0, and we add \(2^i\) to `add` for the next iteration. After processing bits 0..29, `ans` is exactly the hidden number. Complexity: exactly 30 queries, each doing a gcd of two numbers up to about \(2^{31}\), so O(30 * log(max)) time, O(1) space.
