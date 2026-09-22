// You are given a positive integer `N` representing the number of positions in a single row. You need to determine the number of ways to arrange buildings and open spaces such that no two buildings are adjacent. Each position can either have a building (B) or an open space (S). Count all valid configurations of length `N` using these two symbols. However, the final answer must be the square of that count, because the same arrangement rule applies independently to two parallel rows (imagine the row is duplicated, and each position in both rows must satisfy the "no adjacent buildings" rule independently). Since the answer can be very large, return it modulo `1,000,000,007`. Write a C++ function `int totalWays(int n)` that computes this value efficiently.

#include <cassert>

int totalWays(int n); // declaration from solution

int main() {
    // N=1: single row has 2 sequences (B, S), square = 4
    assert(totalWays(1) == 4);
    // N=2: single row valid: BS, SB, SS (3), square = 9
    assert(totalWays(2) == 9);
    // N=3: single row valid: SSS, SSB, SBS, BSS, BSB (5), square = 25
    assert(totalWays(3) == 25);
    // N=4: Fibonacci-like: 8 single-row sequences, square = 64
    assert(totalWays(4) == 64);
    // N=5: 13 single-row sequences, square = 169
    assert(totalWays(5) == 169);
    // N=6: 21 single-row sequences, square = 441
    assert(totalWays(6) == 441);
    // N=10: Check against known Fibonacci sequence: F_{12}? Actually count is F_{n+2} where F_1=1,F_2=1 → for n=10, F_12=144, square=20736
    assert(totalWays(10) == 20736);
    // Large N to ensure modulo works
    assert(totalWays(1000000) == 274698350); // Precomputed expected value
    return 0;
}

#include <cstdint>

// Compute (totalWaysForSingleRow)^2 modulo MOD for a row of length n.
// A row is a sequence of 'B'(building) and 'S'(space) where no two B's are adjacent.
int totalWays(int n) {
    constexpr long long MOD = 1000000007LL;

    if (n <= 0) return 0; // Edge case: invalid input, but problem states positive N.

    long long endWithBuilding = 1; // sequences of length 1 ending with B
    long long endWithSpace = 1;    // sequences of length 1 ending with S

    for (int i = 2; i <= n; ++i) {
        long long nextEndWithBuilding = endWithSpace % MOD;
        long long nextEndWithSpace = (endWithSpace + endWithBuilding) % MOD;
        endWithBuilding = nextEndWithBuilding;
        endWithSpace = nextEndWithSpace;
    }

    long long singleRowTotal = (endWithBuilding + endWithSpace) % MOD;
    long long result = (singleRowTotal * singleRowTotal) % MOD;
    return static_cast<int>(result);
}

// The problem reduces to counting binary strings of length `N` over alphabet {B, S} with no two consecutive B's. Let `ocb` be the number of valid sequences of length `i` ending with a building (B), and `ocs` be the number ending with an open space (S). For length 1, `ocb = 1` (just "B") and `ocs = 1` (just "S"). For each next position:
// - A building can only follow an open space, so `new_ocb = ocs`.
// - An open space can follow either, so `new_ocs = ocs + ocb`.
// After processing length `N`, the total valid single-row sequences = `ocb + ocs`. For two independent rows, the total is that total squared, taken modulo `1,000,000,007`. Edge cases: For `N=1`, the loop doesn't run, and the values remain 1 and 1, giving total = 2, square = 4, which matches manual enumeration (BB, BS, SB, SS? Wait: Actually for two rows of length 1, each row can be B or S, but no adjacency constraint within a row of length 1 is trivial, so both rows independently have 2 options, total 4 configurations). For `N=2`, single-row valid sequences: BB? No, BB invalid, so valid: BS, SB, SS = 3, square = 9. The algorithm runs in `O(N)` time and `O(1)` space. All arithmetic is done modulo the prime to avoid overflow.
