Write a C++ function named `spmm_diagonal_pattern` that accepts two sparse matrices `A` (size M×K) and `B` (size K×N) stored as `Eigen::SparseMatrix<uint32_t>` and performs the matrix multiplication `C = A * B`, returning the resulting sparse matrix `C` (size M×N). The matrices have the property that every nonzero entry in a given row of `A` is located at a column index that is congruent modulo some stride `s` (i.e., columns `j, j+s, j+2s, ...` for a row-specific starting column). Similarly, `B` has a pattern where each row has nonzeros at column indices congruent modulo a stride `t`. However, the function should not assume any specific stride values; it must simply compute the standard sparse-matrix product correctly for arbitrary sparse matrices, using insertion order that preserves the block-structure performance (i.e., iterate over nonzeros in row-major order and accumulate products into a temporary dense accumulator for each output row to avoid costly random insertions). The function must be `const`-correct, take matrices by `const&`, and return `SparseMatrix<uint32_t>` by value. Do not modify the input matrices. Include necessary Eigen headers and `std::vector` for triplets.
The main algorithm is the classic sparse-matrix multiplication using a row-by-row outer product approach with a dense accumulator per output row. For each row `i` of `A`, we iterate over all nonzeros `(i, k, valA)` in that row. For each such nonzero, we iterate over all nonzeros in row `k` of `B`, i.e., `(k, j, valB)`, and accumulate `valA * valB` into a temporary dense vector `acc[j]` for the current output row. After processing all nonzeros of row `i` in `A`, we scan the dense accumulator and insert any nonzero value (after modulo 2^32 overflow) into the result matrix using triplet insertion. This approach is efficient because it avoids repeated random access into a sparse structure and works well for the given block-like patterns. Edge cases: when `K` is zero, the result is an empty M×N zero matrix; when a row of `A` has no nonzeros, the corresponding output row is all zeros and we skip it; products may overflow `uint32_t`, but we use natural unsigned wrap-around which matches the original behavior. Time complexity: let `nnzA` and `nnzB` be the number of nonzeros in A and B. For each nonzero of A, we iterate over the entire row of B at the corresponding column. In the worst case this is `O(nnzA * nnzB / K)` operations. Space complexity is `O(N)` for the temporary accumulator and `O(nnzC)` for the result triplets.
#include <Eigen/Sparse>
#include <vector>
#include <cstdint>

// Perform C = A * B on sparse matrices of uint32_t.
Eigen::SparseMatrix<uint32_t> spmm_diagonal_pattern(
    const Eigen::SparseMatrix<uint32_t>& A,
    const Eigen::SparseMatrix<uint32_t>& B) {
    
    const int M = A.rows();
    const int K = A.cols();
    const int N = B.cols();
    
    // Result matrix initialized with correct dimensions and zero nonzeros.
    Eigen::SparseMatrix<uint32_t> C(M, N);
    std::vector<Eigen::Triplet<uint32_t>> triplets;
    triplets.reserve(A.nonZeros() * (B.nonZeros() / (K > 0 ? K : 1)));
    
    // Temporary dense accumulator for one output row.
    std::vector<uint32_t> acc(N, 0);
    
    // Iterate over each row of A.
    for (int i = 0; i < M; ++i) {
        // Reset accumulator for this row (only positions that might be touched).
        // To be efficient, we track which entries are non-zero.
        std::vector<int> touched;
        
        // Iterate over nonzeros in row i of A.
        for (Eigen::SparseMatrix<uint32_t>::InnerIterator itA(A, i); itA; ++itA) {
            const int k = itA.col();
            const uint32_t valA = itA.value();
            
            // Iterate over nonzeros in row k of B.
            for (Eigen::SparseMatrix<uint32_t>::InnerIterator itB(B, k); itB; ++itB) {
                const int j = itB.col();
                const uint32_t valB = itB.value();
                if (acc[j] == 0 && valA * valB != 0) {
                    touched.push_back(j);
                }
                acc[j] += valA * valB;
            }
        }
        
        // Extract nonzeros from accumulator into triplets.
        for (int j : touched) {
            if (acc[j] != 0) {
                triplets.emplace_back(i, j, acc[j]);
                acc[j] = 0;  // Reset for next row.
            }
        }
    }
    
    C.setFromTriplets(triplets.begin(), triplets.end());
    return C;
}
#include <cassert>
#include <Eigen/Sparse>
#include <vector>
#include <cstdint>

// The solution function declaration.
Eigen::SparseMatrix<uint32_t> spmm_diagonal_pattern(
    const Eigen::SparseMatrix<uint32_t>& A,
    const Eigen::SparseMatrix<uint32_t>& B);

int main() {
    // Test 1: Simple 2x2 times 2x2 with known result.
    std::vector<Eigen::Triplet<uint32_t>> triA, triB;
    triA.emplace_back(0,0,1); triA.emplace_back(0,1,2);
    triA.emplace_back(1,0,3); triA.emplace_back(1,1,4);
    Eigen::SparseMatrix<uint32_t> A(2,2);
    A.setFromTriplets(triA.begin(), triA.end());
    
    triB.emplace_back(0,0,5); triB.emplace_back(0,1,6);
    triB.emplace_back(1,0,7); triB.emplace_back(1,1,8);
    Eigen::SparseMatrix<uint32_t> B(2,2);
    B.setFromTriplets(triB.begin(), triB.end());
    
    Eigen::SparseMatrix<uint32_t> C = spmm_diagonal_pattern(A, B);
    assert(C.rows() == 2 && C.cols() == 2);
    assert(C.coeff(0,0) == 19); // 1*5 + 2*7
    assert(C.coeff(0,1) == 22); // 1*6 + 2*8
    assert(C.coeff(1,0) == 43); // 3*5 + 4*7
    assert(C.coeff(1,1) == 50); // 3*6 + 4*8
    
    // Test 2: Multiplication with zero matrices yields zero matrix.
    Eigen::SparseMatrix<uint32_t> Z1(3,2), Z2(2,4);
    Eigen::SparseMatrix<uint32_t> C2 = spmm_diagonal_pattern(Z1, Z2);
    assert(C2.rows() == 3 && C2.cols() == 4);
    assert(C2.nonZeros() == 0);
    
    // Test 3: K=0 (empty inner dimension) results in MxN zero matrix.
    Eigen::SparseMatrix<uint32_t> A3(2,0), B3(0,3);
    Eigen::SparseMatrix<uint32_t> C3 = spmm_diagonal_pattern(A3, B3);
    assert(C3.rows() == 2 && C3.cols() == 3);
    assert(C3.nonZeros() == 0);
    
    // Test 4: One nonzero product with overflow wrap.
    std::vector<Eigen::Triplet<uint32_t>> triA4, triB4;
    triA4.emplace_back(0,0,0xFFFFFFFFu);
    triB4.emplace_back(0,0,2);
    Eigen::SparseMatrix<uint32_t> A4(1,1);
    A4.setFromTriplets(triA4.begin(), triA4.end());
    Eigen::SparseMatrix<uint32_t> B4(1,1);
    B4.setFromTriplets(triB4.begin(), triB4.end());
    Eigen::SparseMatrix<uint32_t> C4 = spmm_diagonal_pattern(A4, B4);
    assert(C4.coeff(0,0) == 0xFFFFFFFEu); // 4294967295 * 2 -> overflow to 4294967294
    
    // Test 5: Larger sparse where one row of A is empty.
    std::vector<Eigen::Triplet<uint32_t>> triA5, triB5;
    triA5.emplace_back(0,1,3);  // row 0 has one nonzero
    triB5.emplace_back(1,0,4);
    triB5.emplace_back(1,2,5);
    Eigen::SparseMatrix<uint32_t> A5(2,2);
    A5.setFromTriplets(triA5.begin(), triA5.end());
    Eigen::SparseMatrix<uint32_t> B5(2,3);
    B5.setFromTriplets(triB5.begin(), triB5.end());
    Eigen::SparseMatrix<uint32_t> C5 = spmm_diagonal_pattern(A5, B5);
    assert(C5.rows() == 2 && C5.cols() == 3);
    assert(C5.coeff(0,0) == 12);
    assert(C5.coeff(0,2) == 15);
    assert(C5.coeff(1,0) == 0); // row 1 is all zero
    assert(C5.nonZeros() == 2);
    
    return 0;
}
