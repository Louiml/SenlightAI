/*
Write a C++ function `simulate_process` that takes an integer `N` (number of states, 1 ≤ N ≤ 100), an integer `K` (number of transitions, 1 ≤ K ≤ 10^9), and a 2D vector `probabilities` of size `N x N` where each entry is an integer in `[0, 1000000]` representing a probability scaled by 10^6. The system starts in state 0, and at each step it transitions from state `i` to state `j` with probability `probabilities[i][j] / 1000000`. The function must return the expected state index after exactly `K` transitions, modulo `1e9+7`. That is, compute the probability distribution vector after `K` steps (starting from state 0), then return `(sum over i of i * probability_i) mod MOD`. All arithmetic must be done modulo `1e9+7` using the inverse of 1,000,000 modulo this prime. The probabilities may not sum to 1 per row due to scaling, but use them as given.
*/

#include <vector>
#include <cstdint>

const long long MOD = 1000000007LL;
const long long SCALE = 1000000LL;

// Compute modular exponentiation: (base^exp) % MOD
long long mod_pow(long long base, long long exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Compute modular multiplicative inverse using Fermat's Little Theorem
long long mod_inv(long long a) {
    return mod_pow(a, MOD - 2);
}

// Standard matrix multiplication modulo MOD
std::vector<std::vector<long long>> mat_mult(const std::vector<std::vector<long long>>& A,
                                             const std::vector<std::vector<long long>>& B) {
    int n = (int)A.size();
    std::vector<std::vector<long long>> C(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            long long sum = 0;
            for (int k = 0; k < n; ++k) {
                sum = (sum + A[i][k] * B[k][j]) % MOD;
            }
            C[i][j] = sum;
        }
    }
    return C;
}

// Fast matrix exponentiation: A^exp
std::vector<std::vector<long long>> mat_pow(std::vector<std::vector<long long>> A, long long exp) {
    int n = (int)A.size();
    std::vector<std::vector<long long>> result(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) result[i][i] = 1; // identity

    while (exp > 0) {
        if (exp & 1) result = mat_mult(result, A);
        A = mat_mult(A, A);
        exp >>= 1;
    }
    return result;
}

// Main function: compute expected state index after K transitions
long long simulate_process(int N, long long K, const std::vector<std::vector<long long>>& probabilities) {
    long long inv_scale = mod_inv(SCALE);
    std::vector<std::vector<long long>> A(N, std::vector<long long>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = (probabilities[i][j] * inv_scale) % MOD;
        }
    }

    std::vector<std::vector<long long>> P = mat_pow(A, K);

    long long ans = 0;
    for (int i = 0; i < N; ++i) {
        long long contribution = ( (long long)i * P[0][i] ) % MOD;
        ans = (ans + contribution) % MOD;
    }
    return ans;
}

#include <cassert>
#include <vector>

// Include the solution code above (or paste here).

int main() {
    // Test 1: Single state, any K, probability 1 => expected state is 0
    {
        std::vector<std::vector<long long>> probs = {{1000000}};
        assert(simulate_process(1, 1, probs) == 0);
        assert(simulate_process(1, 1000000000LL, probs) == 0);
    }

    // Test 2: Two states, always stay at 0 => expected 0
    {
        std::vector<std::vector<long long>> probs = {{1000000, 0}, {0, 1000000}};
        assert(simulate_process(2, 5, probs) == 0);
    }

    // Test 3: Two states, always go to 1 from 0 => after K>=1 expected 1
    {
        std::vector<std::vector<long long>> probs = {{0, 1000000}, {1000000, 0}};
        assert(simulate_process(2, 1, probs) == 1);
        assert(simulate_process(2, 3, probs) == 1); // 0->1->0->1
    }

    // Test 4: Two states, always go to 1 from 0, but from 1 stay => expected 1
    {
        std::vector<std::vector<long long>> probs = {{0, 1000000}, {0, 1000000}};
        assert(simulate_process(2, 2, probs) == 1);
    }

    // Test 5: Three states, only transition from 0 to 2 with prob 1 => expected 2
    {
        std::vector<std::vector<long long>> probs = {
            {0, 0, 1000000},
            {0, 1000000, 0},
            {0, 0, 1000000}
        };
        assert(simulate_process(3, 4, probs) == 2);
    }

    // Test 6: Two states with 50/50 from 0, and stay at 1 => after 1 step expected 0.5
    // Let's compute manually: P0 = 0.5*0 + 0.5*1 = 0.5 => 500000*inv(1e6) mod, but
    // we expect (0*500000 + 1*500000)*inv(1e6)? Actually probability distribution: [0.5, 0.5]
    // Expected = 0*0.5 + 1*0.5 = 0.5 mod MOD => (500000 * inv(1e6)) mod MOD = 500000 * 500000004 mod MOD = 250000002? Let's compute expected well.
    {
        // Since probabilities are integers, we use 500000 for prob of going to 1.
        std::vector<std::vector<long long>> probs = {
            {500000, 500000},
            {0, 1000000}
        };
        long long result = simulate_process(2, 1, probs);
        // result should be (0 * (500000*inv_scale) + 1 * (500000*inv_scale)) mod MOD
        // = 500000 * inv_scale mod MOD
        long long inv_scale = 500000004LL; // inverse of 1e6 mod 1e9+7
        long long expected = (500000LL * inv_scale) % MOD;
        assert(result == expected);
    }

    // Test 7: Large K, deterministic cycle length 2, N=3
    {
        std::vector<std::vector<long long>> probs = {
            {0, 1000000, 0},
            {0, 0, 1000000},
            {1000000, 0, 0}
        };
        // start 0 -> after 1: 1, after 2: 2, after 3: 0, after 4: 1,...
        // For K=1000000 (even) => 2
        assert(simulate_process(3, 1000000LL, probs) == 2);
        // For K=1000001 (odd) => 1
        assert(simulate_process(3, 1000001LL, probs) == 1);
    }

    return 0;
}

// The core problem is computing the state distribution after a large number of transitions, which is exactly matrix exponentiation. We represent the transition matrix `A` where `A[i][j] = probabilities[i][j] * inv(1000000) mod MOD`. The initial state distribution is a vector `v` with `v[0] = 1` and all others 0. After `K` steps, the distribution is `v * A^K` (or equivalently `A^K` times the column vector, depending on orientation). Since the code snippet uses `P = A^K` and then computes `P[0][i]` for the contribution from state 0, we can do the same: compute `M = A^K` via fast exponentiation (binary exponentiation), then for each `i`, add `i * M[0][i]` modulo `MOD`. 
//
// Edge cases: `K` is at least 1, so no need to handle zero exponent. For matrix exponentiation, we use a base case for `n == 1` to avoid infinite recursion. However, a robust recursive exponentiation should handle `n == 0` by returning identity matrix, but since `K >= 1`, we can directly use the base case `n == 1`. If we want to be general, we can add identity handling, but it is not required. The matrix multiplication is O(N^3) per multiplication, and exponentiation requires O(log K) multiplications, so total time O(N^3 log K). Space is O(N^2) for matrices.
//
// Important: The probabilities may not sum to 1 per row, but that's fine because the problem asks for the expected state index under the given transition probabilities; they are just numbers. Also, note that the matrix multiplication must use `long long` to prevent overflow before mod, since `MOD` is about 1e9 and multiplication of two modded values fits in `long long` (up to ~1e18). We use `const` references for the input matrix.
