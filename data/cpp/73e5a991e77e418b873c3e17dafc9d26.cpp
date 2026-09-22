// Write a C++ function that takes a square matrix of floats (stored as `Eigen::ArrayXXf`) and a scalar threshold value, and returns a boolean indicating whether all elements of the matrix are strictly greater than the threshold. The function must work for any non-empty square matrix size (at least 1x1) and must not modify the input matrix. Use Eigen's array comparison functionality to produce the result efficiently and expressively. The function should be named `allGreaterThan` and must be `const`-correct.
// The solution uses Eigen's built-in element-wise comparison operator `>` on an `ArrayXXf`, which produces a boolean array (`Array<bool, Dynamic, Dynamic>`). The `.all()` method on that boolean array returns `true` if every element in the boolean array is `true`, and `false` otherwise. Since the input is an `ArrayXXf`, we can directly apply the comparison without any conversion. Edge cases: for a 1x1 matrix, `(a > threshold).all()` correctly returns `true` if that single element satisfies the condition, and `false` otherwise. For any non-empty matrix, the algorithm works correctly; there is no need to check emptiness because the problem guarantees a non-empty square matrix. Time complexity is \(O(n^2)\) where n is the number of rows (and columns), since the comparison visits each element once. Space complexity is \(O(1)\) auxiliary, aside from the temporary boolean array that Eigen may create internally (but that's also \(O(n^2)\) in worst case in terms of temporary storage—but in practice, Eigen can optimize this, and the problem doesn't require explicit memory analysis beyond the standard). Important note: Because the function takes the array by const reference, it won't modify the original matrix.
#include <Eigen/Dense>

// Returns true if all elements of the given square array are strictly greater than the threshold.
bool allGreaterThan(const Eigen::ArrayXXf& matrix, float threshold) {
    return (matrix > threshold).all();
}
#include <Eigen/Dense>
#include <cassert>

int main() {
    Eigen::ArrayXXf a(2, 2);
    a << 1.0f, 2.0f,
         3.0f, 4.0f;
    assert(allGreaterThan(a, 0.5f) == true);
    assert(allGreaterThan(a, 2.0f) == false); // because 1.0 and 2.0 are not > 2.0
    assert(allGreaterThan(a, 4.5f) == false);

    Eigen::ArrayXXf b(1, 1);
    b << 7.5f;
    assert(allGreaterThan(b, 7.0f) == true);
    assert(allGreaterThan(b, 7.5f) == false); // strictly greater

    Eigen::ArrayXXf c(3, 3);
    c << -1.0f, 0.0f, 1.0f,
          2.0f, 3.0f, 4.0f,
          5.0f, 6.0f, 7.0f;
    assert(allGreaterThan(c, -2.0f) == true);
    assert(allGreaterThan(c, -1.0f) == false); // because -1.0 is not > -1.0
}
