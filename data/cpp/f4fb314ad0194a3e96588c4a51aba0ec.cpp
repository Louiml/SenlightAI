// Given an \(N \times N\) matrix of integers (where \(2 \le N \le 100\), values between \(0\) and \(10^9\)), write a C++ function `long long asymmetricSum(const std::vector<std::vector<int>>& G, int N)` that computes the sum over all pairs of distinct indices \(i < j\) of the element in the matrix at the position that is the minimum of the two corresponding asymmetric entries: \(\min(G[i][j], G[j][i])\). Specifically, for every unordered pair \((i,j)\), add the smaller of \(G[i][j]\) and \(G[j][i]\) to the total. Diagonal entries are ignored. Return the sum as a `long long`. The input matrix is guaranteed to be square and the function must not modify the matrix.
#include <cassert>
#include <vector>

// The solution function is declared above (include the header or paste it here).

int main() {
    // Test 1: N=2 simple asymmetric values
    std::vector<std::vector<int>> m1 = {{5, 3}, {8, 1}};
    assert(asymmetricSum(m1, 2) == 3); // min(3,8)=3, diagonal ignored

    // Test 2: N=3 with symmetric entries
    std::vector<std::vector<int>> m2 = {
        {0, 7, 2},
        {7, 0, 4},
        {2, 4, 0}
    };
    assert(asymmetricSum(m2, 3) == 13); // pairs: (0,1):7, (0,2):2, (1,2):4 -> sum=13

    // Test 3: N=1 has no pairs
    std::vector<std::vector<int>> m3 = {{42}};
    assert(asymmetricSum(m3, 1) == 0);

    // Test 4: N=4 with all zeros except one pair
    std::vector<std::vector<int>> m4(4, std::vector<int>(4, 0));
    m4[1][3] = 100;
    m4[3][1] = 50;
    assert(asymmetricSum(m4, 4) == 50);

    // Test 5: Large values to ensure long long overflow not an issue
    std::vector<std::vector<int>> m5 = {{0, 1000000000}, {1, 0}};
    assert(asymmetricSum(m5, 2) == 1); // min(1e9,1)=1

    // Test 6: N=5 with all entries filled non-zero
    std::vector<std::vector<int>> m6 = {
        {0, 1, 2, 3, 4},
        {5, 0, 6, 7, 8},
        {9, 10, 0, 11, 12},
        {13, 14, 15, 0, 16},
        {17, 18, 19, 20, 0}
    };
    // Pairs (i<j): compute min manually
    // (0,1):min(1,5)=1
    // (0,2):min(2,9)=2
    // (0,3):min(3,13)=3
    // (0,4):min(4,17)=4
    // (1,2):min(6,10)=6
    // (1,3):min(7,14)=7
    // (1,4):min(8,18)=8
    // (2,3):min(11,15)=11
    // (2,4):min(12,19)=12
    // (3,4):min(16,20)=16
    // Sum = 1+2+3+4+6+7+8+11+12+16 = 70
    assert(asymmetricSum(m6, 5) == 70);

    return 0;
}
#include <vector>
#include <algorithm>

// Computes the sum of min(G[i][j], G[j][i]) over all unordered pairs i<j.
// The matrix G is read-only and N is its dimension.
long long asymmetricSum(const std::vector<std::vector<int>>& G, int N) {
    long long total = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            // For each unordered pair, add the smaller directional value.
            total += std::min(G[i][j], G[j][i]);
        }
    }
    return total;
}
// The problem asks to combine the redundant information in a matrix where each unordered pair of indices has two directional values. We need to iterate over all unordered pairs \((i,j)\) exactly once. The simplest way is to loop \(i\) from \(0\) to \(N-1\), and for each \(i\), loop \(j\) from \(0\) to \(i-1\) (or equivalently \(i+1\) to \(N-1\)) to avoid double counting. For each pair, take `std::min(G[i][j], G[j][i])` and accumulate. The diagonal entries (\(i==j\)) are never considered. Edge cases: \(N=1\) yields sum \(0\) because no pairs exist; \(N=2\) only one pair. Large values up to \(10^9\) and up to \(\frac{N(N-1)}{2} \approx 5000\) pairs, the maximum sum is about \(5 \times 10^{12}\), which fits in `long long` but not `int`, so the return type must be 64-bit. The algorithm is simple nested loops: time complexity \(O(N^2)\), space complexity \(O(1)\) beyond the input matrix.
