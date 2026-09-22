/*
Write a C++ function named `makeToeplitz` that, given a sequence of real numbers stored in an Eigen vector (`Eigen::VectorXd`), returns a square Toeplitz matrix where each row is a cyclic left shift of the previous row by one position. Specifically, the first row equals the input vector, the second row is the input vector shifted left by one (with the first element wrapping to the end), and so on. The function must allocate the returned matrix dynamically (as `Eigen::MatrixXd`) and must not modify the input vector. The returned matrix should have dimensions `n x n` where `n` is the length of the input vector. Handle the trivial case `n==0` by returning an empty matrix (0x0). The implementation must be efficient, avoiding explicit nested loops for element assignment (i.e., use Eigen's nullary expression mechanism or equivalent lazy evaluation).
*/
#include <Eigen/Core>
#include <cassert>

// Functor for cyclic left shift Toeplitz matrix
template <class ArgType>
class toeplitz_functor {
  const ArgType& m_vec;
public:
  toeplitz_functor(const ArgType& arg) : m_vec(arg) {}

  const typename ArgType::Scalar& operator() (Eigen::Index row, Eigen::Index col) const {
    Eigen::Index n = m_vec.size();
    return m_vec((row + col) % n);
  }
};

// Free function: returns a matrix where each row is a cyclic left shift of the input vector.
template <class ArgType>
Eigen::MatrixXd makeToeplitz(const Eigen::MatrixBase<ArgType>& arg) {
  // Determine size
  Eigen::Index n = arg.size();
  if (n == 0) {
    return Eigen::MatrixXd(0, 0);
  }
  // Generate matrix using NullaryExpr, then convert to dynamic dense matrix
  Eigen::MatrixXd mat = Eigen::MatrixXd::NullaryExpr(n, n, toeplitz_functor<ArgType>(arg.derived()));
  return mat;
}
#include <cassert>
#include <iostream>
#include <Eigen/Core>

// Include the solution function here (or paste above)
template <class ArgType>
class toeplitz_functor {
  const ArgType& m_vec;
public:
  toeplitz_functor(const ArgType& arg) : m_vec(arg) {}
  const typename ArgType::Scalar& operator() (Eigen::Index row, Eigen::Index col) const {
    return m_vec((row + col) % m_vec.size());
  }
};

template <class ArgType>
Eigen::MatrixXd makeToeplitz(const Eigen::MatrixBase<ArgType>& arg) {
  Eigen::Index n = arg.size();
  if (n == 0) return Eigen::MatrixXd(0, 0);
  return Eigen::MatrixXd::NullaryExpr(n, n, toeplitz_functor<ArgType>(arg.derived()));
}

int main() {
  // Test 1: Basic case n=4
  Eigen::VectorXd vec1(4);
  vec1 << 1, 2, 4, 8;
  Eigen::MatrixXd mat1 = makeToeplitz(vec1);
  Eigen::MatrixXd expected1(4,4);
  expected1 << 1, 2, 4, 8,
               2, 4, 8, 1,
               4, 8, 1, 2,
               8, 1, 2, 4;
  assert((mat1 - expected1).cwiseAbs().maxCoeff() < 1e-12);

  // Test 2: n=1, single element
  Eigen::VectorXd vec2(1);
  vec2 << 42;
  Eigen::MatrixXd mat2 = makeToeplitz(vec2);
  assert(mat2.rows() == 1 && mat2.cols() == 1);
  assert(std::abs(mat2(0,0) - 42) < 1e-12);

  // Test 3: empty vector
  Eigen::VectorXd vec3(0);
  Eigen::MatrixXd mat3 = makeToeplitz(vec3);
  assert(mat3.rows() == 0 && mat3.cols() == 0);

  // Test 4: n=2 with negative values
  Eigen::VectorXd vec4(2);
  vec4 << -3, 5;
  Eigen::MatrixXd mat4 = makeToeplitz(vec4);
  Eigen::MatrixXd expected4(2,2);
  expected4 << -3, 5,
                5, -3;
  assert((mat4 - expected4).cwiseAbs().maxCoeff() < 1e-12);

  // Test 5: n=5 with fractions
  Eigen::VectorXd vec5(5);
  vec5 << 0.5, -1.5, 2.0, 3.5, -0.25;
  Eigen::MatrixXd mat5 = makeToeplitz(vec5);
  // Verify property: mat5.row(i) == leftShift(vec5, i)
  for (int i=0; i<5; ++i) {
    for (int j=0; j<5; ++j) {
      double expected = vec5((i+j) % 5);
      assert(std::abs(mat5(i,j) - expected) < 1e-12);
    }
  }

  // Test 6: Ensure input not modified
  Eigen::VectorXd vec6(3);
  vec6 << 1, 2, 3;
  Eigen::VectorXd original = vec6;
  Eigen::MatrixXd mat6 = makeToeplitz(vec6);
  assert((vec6 - original).cwiseAbs().maxCoeff() < 1e-12);

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
// The core idea is to construct the Toeplitz matrix using a functor that maps each matrix element `(row, col)` to the corresponding index in the input vector. For a cyclic left shift, the element at `(row, col)` corresponds to input index `(row + col) % n`. This is because the first row (`row=0`) takes input indices `col` directly (`0..n-1`). The second row (`row=1`) takes indices `(1+col)%n`, which is the left shift by one with wrap. This pattern continues. To implement efficiently, we use a templated functor that holds a const reference to the input vector and computes `m_vec((row+col) % size)`. Then, using Eigen's `NullaryExpr`, we generate the matrix lazily without materializing intermediate data. Edge cases: when `n==0`, the modulo operation is undefined; we must guard against it by returning an empty matrix directly. Also, since the input is `Eigen::VectorXd`, the scalar type is `double`, but the function is templated to accept any `Eigen::MatrixBase` for generality, but the test will focus on `VectorXd`. Time complexity: O(n^2) for the matrix size, but since we use lazy evaluation, the actual computation happens when the matrix is assigned to a concrete `MatrixXd`, which is O(n^2). Space complexity: O(n^2) for the returned matrix, with O(1) auxiliary for the functor.
