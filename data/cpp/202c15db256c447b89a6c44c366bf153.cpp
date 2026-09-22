/*
Write a C++ function `int catalanMod10001(int n)` that computes the nth Catalan number modulo 10001. Catalan numbers are defined by the recurrence \( C_0 = 1 \), \( C_1 = 1 \), and for \( n \ge 2 \), \( C_n = \sum_{i=0}^{n-1} C_i \cdot C_{n-1-i} \). The function must take a non-negative integer `n` (with `0 <= n <= 1000`) and return the result modulo 10001. Use the computation pattern from the provided snippet: an iterative dynamic programming table `dp` of size `n+1`, initialized with `dp[0]=1` and `dp[1]=1` (for n≥1), then for each `i` from 2 to `n`, sum products `dp[j] * dp[i-1-j]` for `j` from 0 to `i-1`, applying modulo 10001 after each multiplication/addition to avoid overflow. Handle the edge case `n=0` by immediately returning 1.
*/
#include <vector>

// Computes the nth Catalan number modulo 10001.
// Precondition: n >= 0.
int catalanMod10001(int n) {
    const int MOD = 10001;
    if (n == 0) return 1;
    
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1;
    dp[1] = 1;
    
    for (int i = 2; i <= n; ++i) {
        int sum = 0;
        for (int j = 0; j < i; ++j) {
            sum = (sum + dp[j] * dp[i - 1 - j]) % MOD;
        }
        dp[i] = sum;
    }
    
    return dp[n];
}
#include <cassert>
#include <vector>

// Function under test
int catalanMod10001(int n);

int main() {
    // Base cases
    assert(catalanMod10001(0) == 1);
    assert(catalanMod10001(1) == 1);
    assert(catalanMod10001(2) == 2); // C2 = C0*C1 + C1*C0 = 1+1 = 2
    assert(catalanMod10001(3) == 5); // C3 = 1*2 + 1*1 + 2*1 = 5
    assert(catalanMod10001(4) == 14); // C4 = 14
    
    // Larger values from known Catalan numbers
    int catalan10 = 16796; // C10 = 16796
    assert(catalanMod10001(10) == catalan10 % 10001);
    
    int catalan15 = 9694845; // C15
    assert(catalanMod10001(15) == catalan15 % 10001);
    
    // Edge case: n=1000, just check it runs and returns a value in [0,10000]
    int result = catalanMod10001(1000);
    assert(result >= 0 && result < 10001);
    
    // Verify against a small brute-force recurrence using long long (for n up to 5)
    std::vector<long long> brute(6, 0);
    brute[0] = brute[1] = 1;
    for (int i = 2; i <= 5; ++i) {
        for (int j = 0; j < i; ++j) {
            brute[i] += brute[j] * brute[i-1-j];
        }
    }
    for (int n = 0; n <= 5; ++n) {
        assert(catalanMod10001(n) == static_cast<int>(brute[n] % 10001));
    }
    
    return 0;
}
// The Catalan numbers satisfy the recurrence \( C_n = \sum_{i=0}^{n-1} C_i \cdot C_{n-1-i} \). This is exactly the nested loop in the snippet: for each `i` (the index of the Catalan number we are computing), iterate over all possible splits `j` and `i-1-j` that sum to `i-1`. The dynamic programming approach fills the `dp` array in increasing order of `n`, so each `dp[j]` and `dp[i-1-j]` is already computed. The modulo operation is applied after each addition (`(dp[i] + dp[j]*dp[i-1-j]) % MOD`) to keep values within the range of a 32-bit integer, as the product `dp[j]*dp[i-1-j]` can be up to (10000*10000)=10^8, and the sum over up to 1000 terms could exceed 10^11 without frequent modulo, but the snippet applies modulo at each step, so no 64-bit is needed. The time complexity is \( O(n^2) \) because for each `i` from 2 to `n`, we iterate `j` from 0 to `i-1`, giving \(\sum_{i=2}^{n} i = O(n^2)\). The space complexity is \( O(n) \) for the `dp` array. Edge cases: `n=0` returns 1 directly (since dp[0]=1); `n=1` returns 1 from initialization and the loop doesn't run; `n` can be up to 1000, so the array size is at least 1001 to avoid out-of-bounds.
