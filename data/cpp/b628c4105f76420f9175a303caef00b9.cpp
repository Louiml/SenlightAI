Write a C++ function named `spiralPrimes` that, given a positive integer `n` (1 ≤ n ≤ 20), returns a 2D vector of `int` representing an `n x n` matrix filled by traversing the matrix in a clockwise spiral, starting from the top-left corner and moving right at first, and placing the first `n*n` prime numbers (starting from 2, 3, 5, 7, ...) in that order. The spiral should go right along the top row, then down the rightmost column, then left along the bottom, then up the leftmost column, then repeat inward. The function must be self-contained (no global precomputed prime list), should compute primes on the fly, and must handle the smallest valid `n` (n=1) correctly. The output matrix should have each element as the k-th prime where k goes from 1 to n*n in spiral order.
// The solution involves two main parts: generating the first `n*n` prime numbers and filling the 2D matrix in spiral order. For prime generation, use a simple trial-division method: for each candidate integer starting from 2, test divisibility by all previously found primes up to the square root of the candidate; if none divide, it is prime and gets added to the list. This is efficient enough for n≤20 since n*n≤400 and the 400th prime is 2741, so only a few thousand operations. For the spiral fill, maintain four boundaries: `top` (row index of first unfilled row), `bottom` (last unfilled row), `left` (first unfilled column), `right` (last unfilled column). While the number of filled cells is less than n*n, traverse in four steps: left-to-right along `top` row, increment `top`; top-to-bottom along `right` column, decrement `right`; right-to-left along `bottom` row (if `top <= bottom`), decrement `bottom`; bottom-to-top along `left` column (if `left <= right`), increment `left`. Assign each cell the next prime from the generated list in order. Edge case: n=1 → one cell containing 2. Time complexity: O(n²) for the spiral fill plus O(P) for prime generation where P=n*n (roughly O(P·sqrt(P)) in the worst case for trial division, but with n≤20 this is trivial). Space complexity: O(n²) for the matrix plus O(n²) for the prime list.
#include <vector>
#include <cmath>

// Generate the first count primes and return them in a vector.
std::vector<int> generatePrimes(int count) {
    std::vector<int> primes;
    if (count <= 0) return primes;
    primes.push_back(2);
    int candidate = 3;
    while (static_cast<int>(primes.size()) < count) {
        bool isPrime = true;
        for (int p : primes) {
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) primes.push_back(candidate);
        candidate += 2; // skip even numbers
    }
    return primes;
}

// Fill an n x n matrix with the first n*n primes in clockwise spiral order.
std::vector<std::vector<int>> spiralPrimes(int n) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    if (n <= 0) return matrix;

    std::vector<int> primes = generatePrimes(n * n);
    int idx = 0;

    int top = 0;
    int bottom = n - 1;
    int left = 0;
    int right = n - 1;

    while (idx < n * n) {
        // Traverse left to right along the top row.
        for (int col = left; col <= right; ++col) {
            matrix[top][col] = primes[idx++];
        }
        ++top;
        if (idx >= n * n) break;

        // Traverse top to bottom along the right column.
        for (int row = top; row <= bottom; ++row) {
            matrix[row][right] = primes[idx++];
        }
        --right;
        if (idx >= n * n) break;

        // Traverse right to left along the bottom row.
        if (top <= bottom) {
            for (int col = right; col >= left; --col) {
                matrix[bottom][col] = primes[idx++];
            }
            --bottom;
        }
        if (idx >= n * n) break;

        // Traverse bottom to top along the left column.
        if (left <= right) {
            for (int row = bottom; row >= top; --row) {
                matrix[row][left] = primes[idx++];
            }
            ++left;
        }
    }

    return matrix;
}
#include <cassert>
#include <iostream>
#include <vector>

// Declaration of the function to test (assume the solution is included above).
std::vector<std::vector<int>> spiralPrimes(int n);

int main() {
    // Test n=1: single cell with first prime 2.
    {
        auto result = spiralPrimes(1);
        assert(result.size() == 1 && result[0].size() == 1);
        assert(result[0][0] == 2);
    }

    // Test n=2: spiral order: [2,3; 5,7]? Let's check: top row L->R: (0,0)=2, (0,1)=3; right col top->bottom: (1,1)=5; bottom row R->L: (1,0)=7. So matrix is [[2,3],[7,5]].
    {
        auto result = spiralPrimes(2);
        assert(result[0][0] == 2);
        assert(result[0][1] == 3);
        assert(result[1][1] == 5);
        assert(result[1][0] == 7);
    }

    // Test n=3: first 9 primes: 2,3,5,7,11,13,17,19,23. Spiral fill:
    // (0,0)=2, (0,1)=3, (0,2)=5, then down: (1,2)=7, (2,2)=11, then left: (2,1)=13, (2,0)=17, then up: (1,0)=19, then right: (1,1)=23.
    {
        auto result = spiralPrimes(3);
        assert(result[0][0] == 2 && result[0][1] == 3 && result[0][2] == 5);
        assert(result[1][0] == 19 && result[1][1] == 23 && result[1][2] == 7);
        assert(result[2][0] == 17 && result[2][1] == 13 && result[2][2] == 11);
    }

    // Test n=4: ensure all numbers are exactly the first 16 primes.
    {
        auto result = spiralPrimes(4);
        std::vector<int> all;
        for (const auto& row : result) {
            for (int v : row) all.push_back(v);
        }
        std::vector<int> expected;
        for (int i = 0; i < 16; ++i) {
            // Generate expected primes manually: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53
            int primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
            expected.push_back(primes[i]);
        }
        std::sort(all.begin(), all.end());
        std::sort(expected.begin(), expected.end());
        assert(all == expected);
    }

    // Test n=5: verify spiral positions for some known coordinates.
    {
        auto result = spiralPrimes(5);
        // First prime 2 at (0,0), second 3 at (0,1) ... but let's just check a few key corners.
        // The outer layer: top row: 2,3,5,7,11; right col: 13,17,19,23; bottom row (from right to left): 29,31,37,41; left col: 43,47,53.
        assert(result[0][0] == 2);
        assert(result[0][4] == 11);
        assert(result[4][4] == 23);
        assert(result[4][0] == 41);
        assert(result[2][2] == 61); // The innermost cell after all layers?
        // But let's compute: after outer layer, remaining 3x3 starts with prime #17? Actually the 25 primes list: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97. The inner 3x3 spiral uses primes from index 16 (0-based) = 59? Let's not overcomplicate; just check that the center is the 25th prime? Actually the center is filled last, so it should be the 25th prime = 97. Let's verify.
        assert(result[2][2] == 97);
    }

    // Test n=1 again but with a different approach: ensure no out-of-bounds.
    {
        auto result = spiralPrimes(1);
        assert(result.size() == 1);
        assert(result[0][0] == 2);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
