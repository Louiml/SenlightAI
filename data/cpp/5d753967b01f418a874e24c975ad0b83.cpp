Write a C++ function `long long pathwaysCount(int n)` that returns the number of distinct ways to tile a 2×n board using 2×1 dominoes and 2×2 squares (where a 2×2 square can be placed anywhere, covering all 4 cells). The result must be returned modulo 1,000,000,007. The input `n` is a non-negative integer up to 10^6. For example, for n=1, there is exactly 1 way (a single vertical domino); for n=2, there are 3 ways (two vertical dominoes, two horizontal dominoes stacked, or one 2×2 square); for n=3, there are 5 ways. Ensure your function handles n=0 correctly (empty board, exactly 1 way) and is efficient for large n.
This problem is a classic linear recurrence. Let `dp[i]` be the number of tilings for a 2×i board.  
Base cases: `dp[0]=1` (empty board), `dp[1]=1` (one vertical domino).  
For `i≥2`, the last position can be filled by:
- A vertical domino occupying columns `i-1` and `i` in the top row and bottom row? No, a vertical domino covers one column fully (2×1). Wait, a 2×1 domino covers two cells in one column (top and bottom). So placing a vertical domino in the last column leaves `dp[i-1]`.
- Two horizontal dominoes stacked in columns `i-1` and `i` (covering both rows in those two columns) leaves `dp[i-2]`.
- One 2×2 square covering columns `i-1` and `i` leaves `dp[i-2]`.
Thus recurrence: `dp[i] = dp[i-1] + 2*dp[i-2]`.  
Check: dp[0]=1, dp[1]=1 → dp[2]=1+2=3, dp[3]=3+2*1=5, dp[4]=5+2*3=11. This matches the known sequence (Jacobsthal numbers shifted).  
We compute iteratively up to n in O(n) time and O(1) space (only two previous values needed). Edge cases: n=0 returns 1, n=1 returns 1, and we must take modulo at each step to avoid overflow. Space complexity is O(1) auxiliary. The time complexity is O(n), which is acceptable for n up to 10^6.
#include <cstdint>

// Returns the number of ways to tile a 2×n board using 2×1 dominoes and 2×2 squares, modulo 1,000,000,007.
long long pathwaysCount(int n) {
    const long long MOD = 1000000007LL;
    if (n == 0) return 1;
    if (n == 1) return 1;

    long long prev2 = 1; // dp[0]
    long long prev1 = 1; // dp[1]
    long long current = 0;

    for (int i = 2; i <= n; ++i) {
        current = (prev1 + 2 * prev2) % MOD;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
#include <cassert>

int main() {
    // Base cases
    assert(pathwaysCount(0) == 1);
    assert(pathwaysCount(1) == 1);
    // Known values from recurrence
    assert(pathwaysCount(2) == 3);
    assert(pathwaysCount(3) == 5);
    assert(pathwaysCount(4) == 11);
    assert(pathwaysCount(5) == 21);
    // Larger value check (manual computation or known sequence)
    assert(pathwaysCount(10) == 683);
    // Modulo check for large n (should not overflow and returns a valid remainder)
    long long result = pathwaysCount(1000000);
    assert(result >= 0 && result < 1000000007LL);
    // Additional check: recurrence relation holds for a moderate n
    assert(pathwaysCount(20) == (pathwaysCount(19) + 2 * pathwaysCount(18)) % 1000000007LL);
    return 0;
}
