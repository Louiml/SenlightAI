// Write a standalone C++ function named `matrixOperations` that takes three arguments: a 2x2 matrix of doubles, a 2-element vector of doubles, and another 2-element vector of doubles. The function must return a `std::string` containing five lines of output, each separated by a newline character, in this exact order: (1) the product of the matrix with itself, (2) the product of the matrix and the first vector, (3) the product of the transpose of the first vector and the matrix (which yields a 1x2 row vector), (4) the dot product of the two vectors, and (5) the outer product of the two vectors (a 2x2 matrix). Each numerical output must be printed using the default stream formatting (no fixed precision), with spaces separating numbers in a matrix row and a comma and space separating row entries? For simplicity, print matrices in the same style as Eigen's `operator<<`, i.e., each row on its own line with entries separated by spaces, and vectors printed as a single row with spaces between entries. For the dot product, print just the scalar value. The function must not modify its inputs.

// The solution requires performing basic linear algebra operations using the Eigen library. First, compute `mat * mat`, which is a 2x2 matrix multiplication. Then compute `mat * u`, giving a 2D vector. Next, compute `u.transpose() * mat`, resulting in a 1x2 row vector (this is effectively a row vector). Then compute `u.transpose() * v`, which is a scalar (dot product). Finally, compute `u * v.transpose()`, yielding a 2x2 matrix (outer product). To format the output, use `Eigen::IOFormat` with default settings, but since we need each row on a new line, we can directly stream the matrices/vectors to a `std::ostringstream`. For a vector (2D or row vector), Eigen streams it as one line with spaces. For a matrix, Eigen streams each row on a new line. The dot product is a double, so stream it directly. Concatenate the five outputs with `\n` between them. Edge cases: no special cases; all inputs are arbitrary doubles, including negatives and zeros. Time complexity is O(1) since fixed sizes; space complexity O(1) for outputs.

#include <string>
#include <sstream>
#include <Eigen/Dense>

// Perform five matrix/vector operations and return formatted results as a string.
std::string matrixOperations(const Eigen::Matrix2d& mat,
                             const Eigen::Vector2d& u,
                             const Eigen::Vector2d& v) {
    std::ostringstream oss;

    // 1: mat * mat
    oss << mat * mat << "\n";
    // 2: mat * u
    oss << mat * u << "\n";
    // 3: u^T * mat (row vector)
    oss << u.transpose() * mat << "\n";
    // 4: u^T * v (dot product scalar)
    oss << u.transpose() * v << "\n";
    // 5: u * v^T (outer product)
    oss << u * v.transpose();

    return oss.str();
}

#include <cassert>
#include <sstream>
#include <Eigen/Dense>

// The solution function declaration (assumed to be in scope)
std::string matrixOperations(const Eigen::Matrix2d& mat,
                             const Eigen::Vector2d& u,
                             const Eigen::Vector2d& v);

int main() {
    Eigen::Matrix2d mat;
    mat << 1, 2,
           3, 4;
    Eigen::Vector2d u(-1, 1), v(2, 0);

    std::string result = matrixOperations(mat, u, v);
    std::istringstream iss(result);
    std::string line1, line2, line3, line4, line5, line6;
    // Expected output lines:
    // mat*mat: "7 10\n15 22"
    // mat*u: "-1 1"  (actually mat*u = (-1*1+2*1, 3*(-1)+4*1) = (1,1)? Wait check)
    // Let's compute: mat*u = [1* -1 + 2*1 = 1; 3*-1 + 4*1 = 1] => "1 1"
    // u^T*mat = [-1*1 + 1*3 = 2, -1*2 + 1*4 = 2] => "2 2"
    // u^T*v = -1*2 + 1*0 = -2 => "-2"
    // u*v^T = [-1*2, -1*0; 1*2, 1*0] => "-2 0\n2 0"

    std::getline(iss, line1);
    std::getline(iss, line2);
    std::getline(iss, line3);
    std::getline(iss, line4);
    std::getline(iss, line5);
    std::getline(iss, line6);

    // Note: line3 is the first line of mat*mat's second row? Actually output format: 
    // For mat*mat: Eigen prints:
    // 7 10
    // 15 22
    // Then a newline from our "\n", then "1 1" for mat*u, then newline, then "2 2", then newline, then "-2", then newline, then "-2 0\n2 0"
    // So total lines: "7 10", "15 22", "1 1", "2 2", "-2", "-2 0", "2 0"
    assert(line1 == "7 10");
    assert(line2 == "15 22");
    assert(line3 == "1 1");
    assert(line4 == "2 2");
    assert(line5 == "-2");
    assert(line6 == "-2 0");
    // Need extra line for last row:
    std::string line7;
    std::getline(iss, line7);
    assert(line7 == "2 0");

    // Test with different values
    Eigen::Matrix2d m2;
    m2 << 0, -1,
          2, 3;
    Eigen::Vector2d a(1, 2), b(3, 4);
    std::string res2 = matrixOperations(m2, a, b);
    std::istringstream iss2(res2);
    std::string t1, t2, t3, t4, t5, t6, t7;
    std::getline(iss2, t1);
    std::getline(iss2, t2);
    std::getline(iss2, t3);
    std::getline(iss2, t4);
    std::getline(iss2, t5);
    std::getline(iss2, t6);
    std::getline(iss2, t7);
    // m2*m2 = [0*-1? compute: [0*0 + -1*2 = -2, 0*-1 + -1*3 = -3; 2*0+3*2=6, 2*-1+3*3=7] => "-2 -3\n6 7"
    assert(t1 == "-2 -3");
    assert(t2 == "6 7");
    // m2*a = [0*1 + -1*2 = -2; 2*1+3*2=8] => "-2 8"
    assert(t3 == "-2 8");
    // a^T*m2 = [1*0 + 2*2=4, 1*-1+2*3=5] => "4 5"
    assert(t4 == "4 5");
    // a^T*b = 1*3+2*4=11
    assert(t5 == "11");
    // a*b^T = [1*3,1*4;2*3,2*4] => "3 4\n6 8"
    assert(t6 == "3 4");
    assert(t7 == "6 8");

    return 0;
}
