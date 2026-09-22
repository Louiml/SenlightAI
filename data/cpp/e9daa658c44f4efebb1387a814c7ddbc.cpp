Write a C++ function `void matrix_power(std::vector<std::vector<uint64_t>>& result, const std::vector<std::vector<uint64_t>>& base, uint32_t exponent)` that computes the matrix power `result = base^exponent` using fast exponentiation (binary exponentiation) for square matrices. The matrix entries are `uint64_t`, and multiplication should be performed modulo `1000000007` to avoid overflow. The function must handle exponent `0` (returning the identity matrix), exponent `1` (returning the base), and any positive exponent. The input matrix is guaranteed square and non-empty. The function should be self-contained, include all necessary headers, and use `const` correctness appropriately.

The core algorithm is binary exponentiation (also known as fast matrix exponentiation). For a square matrix \(A\) of size \(n \times n\) and an exponent \(e\), we compute \(A^e\) in \(O(n^3 \log e)\) time. The idea:
- Initialize `result` as the identity matrix (1s on diagonal, 0 elsewhere).
- While `exponent > 0`:
  - If the lowest bit of `exponent` is 1, multiply `result` by the current base (`result = result * base` mod MOD).
  - Square the base matrix (`base = base * base` mod MOD).
  - Right-shift `exponent` by 1.
- The multiplication of two \(n \times n\) matrices takes \(O(n^3)\) time; we perform at most \(O(\log e)\) such multiplications (one per bit, plus extra for the result updates only when a bit is set). So total time is \(O(n^3 \log e)\). Space is \(O(n^2)\) for the temporary matrices used in multiplication, plus the output matrix.

Edge cases:
- Exponent `0`: return identity.
- Exponent `1`: return base (but still use the loop; it works fine).
- Matrix size 1×1: multiplication reduces to scalar multiplication mod MOD.
- Use `uint64_t` and take modulo after each addition and multiplication to prevent overflow. For multiplication, since `uint64_t` values are up to ~1.8e19 and MOD is ~1e9, the product of two modded values fits in `uint64_t` without overflow (max product is ~1e18 < 1.8e19). So safe.

#include <vector>
#include <cstdint>

const uint64_t MOD = 1000000007ULL;

// Multiply two square matrices modulo MOD.
std::vector<std::vector<uint64_t>> matrix_multiply(
    const std::vector<std::vector<uint64_t>>& A,
    const std::vector<std::vector<uint64_t>>& B) {
    size_t n = A.size();
    std::vector<std::vector<uint64_t>> C(n, std::vector<uint64_t>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            uint64_t sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum = (sum + A[i][k] * B[k][j]) % MOD;
            }
            C[i][j] = sum;
        }
    }
    return C;
}

// Compute result = base^exponent modulo MOD using binary exponentiation.
void matrix_power(
    std::vector<std::vector<uint64_t>>& result,
    const std::vector<std::vector<uint64_t>>& base,
    uint32_t exponent) {
    size_t n = base.size();
    // Initialize result to identity matrix.
    result.assign(n, std::vector<uint64_t>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        result[i][i] = 1;
    }
    // Work on a copy of base.
    std::vector<std::vector<uint64_t>> current = base;
    uint32_t e = exponent;
    while (e > 0) {
        if (e & 1) {
            result = matrix_multiply(result, current);
        }
        current = matrix_multiply(current, current);
        e >>= 1;
    }
}

#include <cassert>
#include <vector>
#include <cstdint>

// Declare the solution function (already defined above).
void matrix_power(
    std::vector<std::vector<uint64_t>>& result,
    const std::vector<std::vector<uint64_t>>& base,
    uint32_t exponent);

int main() {
    // Test 1: 2x2 matrix, exponent 0 -> identity.
    std::vector<std::vector<uint64_t>> A = {{1, 2}, {3, 4}};
    std::vector<std::vector<uint64_t>> R;
    matrix_power(R, A, 0);
    assert(R == std::vector<std::vector<uint64_t>>({{1, 0}, {0, 1}}));

    // Test 2: 2x2 matrix, exponent 1 -> same as base.
    matrix_power(R, A, 1);
    assert(R == A);

    // Test 3: 2x2 matrix, exponent 2 -> A^2 = [[7,10],[15,22]].
    matrix_power(R, A, 2);
    assert(R == std::vector<std::vector<uint64_t>>({{7, 10}, {15, 22}}));

    // Test 4: 3x3 matrix, exponent 3, compare with direct multiplication.
    std::vector<std::vector<uint64_t>> B = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};
    matrix_power(R, B, 3);
    // Compute B^2 manually: B^2 = B*B
    std::vector<std::vector<uint64_t>> B2 = {{2, 1, 1}, {1, 1, 1}, {1, 0, 0}};
    // B^3 = B^2 * B
    std::vector<std::vector<uint64_t>> B3 = {{3, 2, 1}, {2, 1, 1}, {1, 1, 1}};
    assert(R == B3);

    // Test 5: 1x1 matrix with large numbers, check modulo.
    std::vector<std::vector<uint64_t>> C = {{1000000000}};
    matrix_power(R, C, 2);
    // (1e9)^2 = 1e18, modulo 1000000007 = 1000000007? Actually (1e9)^2 mod MOD:
    // 1000000000^2 = 1e18, 1e18 mod 1000000007 = compute: 1000000007*999999993 = 999999999999999951, remainder 49. So check.
    assert(R[0][0] == 49);

    // Test 6: Larger exponent ensures binary exponentiation works.
    std::vector<std::vector<uint64_t>> D = {{2, 0}, {0, 3}};
    matrix_power(R, D, 10);
    // Diagonal: 2^10=1024, 3^10=59049, all mod unchanged.
    assert(R[0][0] == 1024 && R[1][1] == 59049);

    return 0;
}
