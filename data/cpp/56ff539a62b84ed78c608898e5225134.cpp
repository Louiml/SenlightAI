Write a C++ function `countAttendancePermutations(int n)` that returns the number of distinct attendance records of length `n` (where each day is either absent `'A'`, late `'L'`, or present `'P'`) such that the record is considered **rewardable**. A record is rewardable if it contains **at most one `'A'`** and does **not contain three consecutive `'L'`s**. The result must be returned modulo \(10^9+7\). The function should handle any positive integer `n` (including up to very large values like \(10^9\)) efficiently. Do not rely on recursion or dynamic programming that iterates through every day individually; instead, use matrix exponentiation on a constant-size state transition.

#include <cassert>

int main() {
    assert(countAttendancePermutations(0) == 1);
    assert(countAttendancePermutations(1) == 3);
    assert(countAttendancePermutations(2) == 8);
    // n=3: all 27 minus those with "LLL" (2 patterns) minus those with two A's? Let's compute manually:
    // rewardable = 27 - 2 (LLL with no A) - 1 (LLL with one A? actually LLL with one A is invalid too, so subtract 2 for LLL positions? let's just trust known answer)
    assert(countAttendancePermutations(3) == 19);
    assert(countAttendancePermutations(4) == 43);
    assert(countAttendancePermutations(10) == 3536);
    assert(countAttendancePermutations(1000000000) == 84151903); // known value for n=1e9 modulo 1e9+7
    return 0;
}

#include <vector>
#include <cstdint>

using Matrix = std::vector<std::vector<long long>>;

const long long MOD = 1000000007LL;

// Multiply two matrices modulo MOD
Matrix multiply(const Matrix& a, const Matrix& b) {
    int n = a.size();
    int m = b[0].size();
    int p = b.size();
    Matrix res(n, std::vector<long long>(m, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            long long sum = 0;
            for (int k = 0; k < p; ++k) {
                sum = (sum + a[i][k] * b[k][j]) % MOD;
            }
            res[i][j] = sum;
        }
    }
    return res;
}

// Fast exponentiation of a square matrix to the power of exponent
Matrix matrixPower(Matrix base, long long exponent) {
    int n = base.size();
    Matrix result(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) result[i][i] = 1; // identity
    while (exponent > 0) {
        if (exponent & 1) result = multiply(result, base);
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

// Count rewardable attendance records of length n
long long countAttendancePermutations(long long n) {
    if (n == 0) return 1;
    // Transition matrix: rows = current state, columns = next state
    Matrix trans = {
        {1,1,0,1,0,0}, // from state0
        {1,0,1,1,0,0}, // from state1
        {1,0,0,1,0,0}, // from state2
        {0,0,0,1,1,0}, // from state3
        {0,0,0,1,0,1}, // from state4
        {0,0,0,1,0,0}  // from state5
    };
    Matrix powered = matrixPower(trans, n);
    // Initial vector: [1,0,0,0,0,0]
    long long total = 0;
    for (int j = 0; j < 6; ++j) {
        total = (total + powered[0][j]) % MOD;
    }
    return total;
}

// The problem can be modeled as a finite-state automaton with six states representing the current situation regarding absences and consecutive lates.  
// Define states as:  
// - `0` = no `'A'` so far, last two days not `LL`  
// - `1` = no `'A'`, last one day `L` but not two consecutive `L` (last day exactly `L`, previous not `L`)  
// - `2` = no `'A'`, last two days are `LL`  
// - `3` = one `'A'` used, last two days not `LL`  
// - `4` = one `'A'`, last one day `L` but not two consecutive `L`  
// - `5` = one `'A'`, last two days are `LL`  
//
// From each state, we can transition to another state by appending `'P'`, `'L'`, or `'A'` (but `'A'` is only allowed if no `'A'` has been used yet). The transition matrix `M` of size 6×6 encodes these allowed moves. The initial state vector for an empty record is `[1,0,0,0,0,0]` (representing state 0). After `n` days, the total rewardable records is the sum of all entries in the resulting vector.  
//
// We compute \(v \cdot M^n\) using fast exponentiation (binary exponentiation) on the matrix, which runs in \(O(6^3 \log n)\) time = \(O(\log n)\) with constant factors. Space is \(O(1)\) for the matrices. Edge cases: `n=0` (empty record is rewardable, sum=1), `n=1` (all 3 records are rewardable), and large `n` requiring modular arithmetic to avoid overflow. All arithmetic is done modulo \(10^9+7\), and since matrix multiplication involves multiplying 64-bit integers, we use `long long` and take modulo at each addition.
