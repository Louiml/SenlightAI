Write a C++ function `long long hafnian_mod(const std::vector<std::vector<long long>>& A, long long MOD)` that computes the hafnian of a symmetric matrix modulo a prime `MOD` (given as a parameter, with `MOD` guaranteed to be a prime larger than `N`). The matrix `A` is of even size `N` (N ≤ 20), symmetric, and all entries are non-negative integers. The hafnian is defined as the sum over all perfect matchings of the product of the weights of the edges, where the weight of edge `(i,j)` is `A[i][j]`. The function must return the result modulo `MOD`. The input matrix is guaranteed to be symmetric (`A[i][j] == A[j][i]`) and has zero on the diagonal. Handle the case `N=0` by returning `1` (the empty product). Use the subset DP approach with bitmasks to avoid exponential time in `N!`.

#include <cassert>
#include <vector>

// Declaration from solution (for clarity, in actual test just include the above)
static long long hafnian_mod(const std::vector<std::vector<long long>>& A, long long MOD);

int main() {
    const long long MOD = 998244353;

    // N=0 => 1
    assert(hafnian_mod({}, MOD) == 1);

    // N=2 matrix [[0,5],[5,0]] => hafnian = 5
    std::vector<std::vector<long long>> A2 = {{0,5},{5,0}};
    assert(hafnian_mod(A2, MOD) == 5);

    // N=4 matrix all ones off-diagonal, diagonal zero
    // hafnian = 3 (three perfect matchings each product 1)
    std::vector<std::vector<long long>> A4_all1 = {
        {0,1,1,1},
        {1,0,1,1},
        {1,1,0,1},
        {1,1,1,0}
    };
    assert(hafnian_mod(A4_all1, MOD) == 3);

    // N=4 with weights:
    // edges: (0,1)=2, (0,2)=3, (0,3)=5, (1,2)=7, (1,3)=11, (2,3)=13
    // Perfect matchings:
    // (0,1)*(2,3) = 2*13 = 26
    // (0,2)*(1,3) = 3*11 = 33
    // (0,3)*(1,2) = 5*7 = 35
    // sum = 94
    std::vector<std::vector<long long>> A4 = {
        {0,2,3,5},
        {2,0,7,11},
        {3,7,0,13},
        {5,11,13,0}
    };
    assert(hafnian_mod(A4, MOD) == 94);

    // N=6 matrix with diagonal zero, off-diagonal all 1
    // hafnian = number of perfect matchings on 6 vertices = 15
    std::vector<std::vector<long long>> A6(6, std::vector<long long>(6, 1));
    for (int i = 0; i < 6; ++i) A6[i][i] = 0;
    assert(hafnian_mod(A6, MOD) == 15);

    // Check modulo: use MOD=7 and a 4x4 matrix with large values
    // A = [[0, 10, 20, 30],
    //      [10,0,40,50],
    //      [20,40,0,60],
    //      [30,50,60,0]]
    // Matchings:
    // (0,1)*(2,3)=10*60=600
    // (0,2)*(1,3)=20*50=1000
    // (0,3)*(1,2)=30*40=1200
    // sum=2800, mod 7 = 0 (2800/7=400)
    std::vector<std::vector<long long>> A_mod = {
        {0,10,20,30},
        {10,0,40,50},
        {20,40,0,60},
        {30,50,60,0}
    };
    assert(hafnian_mod(A_mod, 7) == 0);

    // Another modulo test: MOD=11, same matrix, 2800 mod 11 = 6 (since 11*254=2794)
    assert(hafnian_mod(A_mod, 11) == 6);

    return 0;
}

#include <vector>
#include <cstdint>

// Computes the hafnian of symmetric matrix A (size N x N, N even, N <= 20)
// modulo MOD (a prime). A[i][j] == A[j][i], A[i][i] == 0.
// Returns hafnian mod MOD. For N=0, returns 1.
static long long hafnian_mod(const std::vector<std::vector<long long>>& A, long long MOD) {
    int N = (int)A.size();
    if (N == 0) return 1 % MOD;
    if (N & 1) return 0; // hafnian of odd-sized matrix is 0 by definition

    int fullMask = (1 << N) - 1;
    std::vector<long long> memo(1 << N, -1); // -1 means not computed

    // Recursive lambda with memoization
    // f(mask) = hafnian of submatrix induced by bits in mask.
    // mask must have even popcount.
    auto f = [&](auto&& self, int mask) -> long long {
        if (mask == 0) return 1 % MOD;
        if (memo[mask] != -1) return memo[mask];

        // Find smallest set bit index
        int i = __builtin_ctz(mask);
        int rest = mask ^ (1 << i);
        long long result = 0;
        // Iterate over all j > i present in rest
        for (int j = i + 1; j < N; ++j) {
            if (rest & (1 << j)) {
                int newMask = rest ^ (1 << j);
                long long term = (A[i][j] % MOD) * self(self, newMask) % MOD;
                result = (result + term) % MOD;
            }
        }
        memo[mask] = result;
        return result;
    };

    return f(f, fullMask);
}

// The hafnian is computed via recursion over the smallest set bit. Let `f(mask)` be the hafnian of the submatrix induced by vertices in `mask`. The recursion picks the smallest index `i` in `mask`, then iterates over all other `j` in `mask` with `j > i`, and adds `A[i][j] * f(mask without {i,j})`. The base case is `mask == 0`, return `1`. Memoize results in a vector of size `(1 << N)`, initialized to `-1` to indicate uncomputed. Because the matrix is symmetric and diagonal is zero, this recursion correctly sums over all perfect matchings. Complexity: there are `2^N` masks, but only masks with even popcount are needed (odd popcount states are never visited). For each mask, we iterate over at most `N` choices, so total time `O(N * 2^N)`, space `O(2^N)`. Edge cases: `N=0` returns 1; `N` must be even; all arithmetic is done modulo `MOD` using `long long` to avoid overflow. Since `N` ≤ 20, `2^N` ≤ 1,048,576, which is fine memory-wise.
