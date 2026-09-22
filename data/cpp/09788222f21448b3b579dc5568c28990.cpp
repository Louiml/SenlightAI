/*
Write a C++ function that simulates a simplified matrix iteration benchmark using a dense square matrix with `double` entries. The function must accept three integers: the matrix dimension `N` (with `N >= 1`), the number of iterations `R` (with `R >= 0`), and a small scalar parameter `eps` (with `0 < eps < 1`). It must return the top-left element `(0,0)` of the final matrix after performing the following operations `R` times, starting from a matrix `m` where `m(i,j) = (i + j + 1) / (double)(N * N)` for all `0 <= i,j < N`, and `I` is the `N x N` identity matrix: in each iteration, set `m = I + eps * (m + m * m)`. Use standard matrix multiplication. The function must be standalone, not rely on any external linear algebra library, and work for any reasonable `N` and `R` that fit in memory. Ensure correct handling of integer division in the initialization and proper const-correctness where appropriate.
*/

#include <vector>
#include <stdexcept>

// Compute the top-left element after R iterations of the matrix recurrence.
// N: matrix dimension (>=1), R: number of iterations (>=0), eps: scalar (0<eps<1).
double matrixIterationTopLeft(int N, int R, double eps) {
    if (N < 1 || R < 0 || eps <= 0.0 || eps >= 1.0) {
        throw std::invalid_argument("Invalid parameters");
    }

    // Identity matrix
    std::vector<std::vector<double>> I(N, std::vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        I[i][i] = 1.0;
    }

    // Initialize m
    std::vector<std::vector<double>> m(N, std::vector<double>(N, 0.0));
    double denominator = static_cast<double>(N) * static_cast<double>(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            m[i][j] = static_cast<double>(i + j + 1) / denominator;
        }
    }

    // Iterate
    std::vector<std::vector<double>> temp(N, std::vector<double>(N, 0.0));
    for (int iter = 0; iter < R; ++iter) {
        // temp = m * m
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                double sum = 0.0;
                for (int k = 0; k < N; ++k) {
                    sum += m[i][k] * m[k][j];
                }
                temp[i][j] = sum;
            }
        }
        // m = I + eps * (m + temp)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] = I[i][j] + eps * (m[i][j] + temp[i][j]);
            }
        }
    }

    return m[0][0];
}

#include <cassert>
#include <cmath>

int main() {
    // N=1, R=0, any eps: initial m(0,0) = (0+0+1)/(1*1) = 1
    assert(std::abs(matrixIterationTopLeft(1, 0, 0.5) - 1.0) < 1e-12);

    // N=2, R=0: initial m(0,0) = 1/(2*2) = 0.25
    assert(std::abs(matrixIterationTopLeft(2, 0, 0.3) - 0.25) < 1e-12);

    // N=1, R=1, eps=0.1: m = I + 0.1*(m + m*m); m initially [1], I=[1]
    // m_new = 1 + 0.1*(1 + 1*1) = 1 + 0.1*2 = 1.2
    assert(std::abs(matrixIterationTopLeft(1, 1, 0.1) - 1.2) < 1e-12);

    // N=2, R=1, eps=0.0001: compute hand-check
    // m initial = [[1/4, 2/4],[2/4? i+j+1: (0,1)=2/4=0.5, (1,0)=0.5, (1,1)=3/4]]
    // m*m: [[0.25*0.25 + 0.5*0.5 = 0.0625+0.25=0.3125, 0.25*0.5+0.5*0.75=0.125+0.375=0.5],
    //       [0.5*0.25 + 0.75*0.5 = 0.125+0.375=0.5, 0.5*0.5+0.75*0.75=0.25+0.5625=0.8125]]
    // m+ m*m = [[0.25+0.3125=0.5625, 0.5+0.5=1.0], [0.5+0.5=1.0, 0.75+0.8125=1.5625]]
    // eps* = 0.0001* those, then add I: top-left = 1 + 0.0001*0.5625 = 1.00005625
    double result = matrixIterationTopLeft(2, 1, 0.0001);
    assert(std::abs(result - 1.00005625) < 1e-12);

    // Large N but zero iterations should match initial top-left
    assert(std::abs(matrixIterationTopLeft(10, 0, 0.5) - (1.0/100.0)) < 1e-12);
}

// The core algorithm is straightforward: allocate two `N x N` matrices, one for the identity matrix `I` and one for the current state `m`. Initialize `m` using the given formula, where care must be taken to cast the numerator to `double` before dividing by `N*N` to avoid integer truncation. For each of the `R` iterations, compute a temporary matrix `tmp = m * m` using the standard triple-loop matrix multiplication (or a cache-friendly blocked approach if needed), then compute `m = I + eps * (m + tmp)` element-wise. The identity matrix has 1s on the diagonal and 0s elsewhere. Edge cases: `N=1` makes the matrix multiplication trivial; `R=0` must return the initial top-left element unchanged; `eps` being very small should not cause issues since we just scale. Time complexity: each matrix multiplication is `O(N^3)`, and there are `R` iterations, so total `O(R * N^3)` time. Space complexity: `O(N^2)` for the matrices. For large `N` and `R` this can be expensive, but for the test sizes it is fine.
