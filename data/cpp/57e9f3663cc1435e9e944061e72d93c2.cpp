// Write a C++ function named `calculateMutualInformation` that takes two `std::vector<uint32_t>` arguments representing discrete sequences X and Y, and returns a `double` value equal to their mutual information I(X;Y) = H(X) + H(Y) − H(X,Y). The function must compute the marginal entropies H(X) and H(Y) and the joint entropy H(X,Y) directly from the element sequences. Assume both sequences are composed of non‑negative integers (0 to 2³²−1), and the alphabet of each sequence may be smaller than the largest value present. The function must handle cases where the two vectors have different lengths: in that case, it should print a warning message to `std::cout` and then compute the mutual information using the minimum length (truncating the longer sequence). Values with zero probability must be ignored in entropy calculations. Use base‑e (natural logarithm) for entropy computation. The function should be `const`‑correct (take `const std::vector<uint32_t>&` parameters), and return a finite double precision result.
The solution computes three entropy terms over discrete distributions.  
- **Marginal entropy H(X)**: Count frequencies of each unique value in `seq_X`. Let `N` be the number of elements used (after possible truncation). For each count `c`, probability `p = c / N`. Contribution `−p * log(p)`. Sum over all distinct values with `c>0`.  
- **Marginal entropy H(Y)**: Same approach on `seq_Y`.  
- **Joint entropy H(X,Y)**: Build a map of pairs `(x,y)` to counts. Probability `p = count / N`. Sum `−p * log(p)` over all pairs.  
- **Edge cases**:  
  - Different lengths: find `N = min(sizeX, sizeY)` and use only the first `N` elements of each sequence. Print warning to `std::cout`.  
  - Empty sequences (or zero after truncation): cannot compute probabilities; return 0.0 (or handle gracefully).  
  - All elements same value: entropy = 0 for that variable; joint entropy may also be 0.  
  - Large counts may overflow `size_t`? Use `uint64_t` for counters and `double` for sums.  
- **Time complexity**: O(N) to count frequencies (using unordered_map). **Space complexity**: O(K) where K is number of distinct values in X plus distinct in Y plus distinct pairs, which in worst case is O(N).  
- **Precision**: Use `std::log` (natural log) from `<cmath>`. Summation may have floating‑point errors, but acceptable for typical inputs.
#include <vector>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <cstdint>

// Compute mutual information I(X;Y) = H(X) + H(Y) - H(X,Y)
// Uses natural logarithms. If sequences have different lengths, truncates to shorter length.
double calculateMutualInformation(const std::vector<uint32_t>& seq_X, const std::vector<uint32_t>& seq_Y) {
    size_t nX = seq_X.size();
    size_t nY = seq_Y.size();
    size_t N = std::min(nX, nY);
    if (nX != nY) {
        std::cout << "Warning: X and Y have different lengths in the computation of mutual information." << std::endl;
    }
    if (N == 0) return 0.0;

    // Count marginal frequencies for X
    std::unordered_map<uint32_t, uint64_t> countX;
    for (size_t i = 0; i < N; ++i) {
        countX[seq_X[i]]++;
    }

    // Count marginal frequencies for Y
    std::unordered_map<uint32_t, uint64_t> countY;
    for (size_t i = 0; i < N; ++i) {
        countY[seq_Y[i]]++;
    }

    // Count joint frequencies
    std::unordered_map<uint64_t, uint64_t> countXY; // key = (x<<32) | y
    for (size_t i = 0; i < N; ++i) {
        uint64_t key = (static_cast<uint64_t>(seq_X[i]) << 32) | seq_Y[i];
        countXY[key]++;
    }

    double H_X = 0.0;
    for (const auto& kv : countX) {
        double p = static_cast<double>(kv.second) / static_cast<double>(N);
        H_X -= p * std::log(p);
    }

    double H_Y = 0.0;
    for (const auto& kv : countY) {
        double p = static_cast<double>(kv.second) / static_cast<double>(N);
        H_Y -= p * std::log(p);
    }

    double H_XY = 0.0;
    for (const auto& kv : countXY) {
        double p = static_cast<double>(kv.second) / static_cast<double>(N);
        H_XY -= p * std::log(p);
    }

    return H_X + H_Y - H_XY;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdint>

// The solution function is declared above; include it here for completeness.
double calculateMutualInformation(const std::vector<uint32_t>& seq_X, const std::vector<uint32_t>& seq_Y);

int main() {
    // Case 1: Independent sequences (each distinct value pairs uniquely) -> MI = 0
    std::vector<uint32_t> X1 = {0, 1, 2};
    std::vector<uint32_t> Y1 = {0, 1, 2};
    assert(std::abs(calculateMutualInformation(X1, Y1) - 0.0) < 1e-9);

    // Case 2: Perfectly dependent (Y = X) -> MI = H(X) = log(3) for uniform 3 values
    std::vector<uint32_t> X2 = {0, 1, 2};
    std::vector<uint32_t> Y2 = {0, 1, 2}; // same as X, joint has only 3 pairs each with p=1/3 -> MI = log(3)
    double expected2 = std::log(3.0);
    assert(std::abs(calculateMutualInformation(X2, Y2) - expected2) < 1e-9);

    // Case 3: Constant sequences -> entropy zero, MI = 0
    std::vector<uint32_t> X3 = {5, 5, 5};
    std::vector<uint32_t> Y3 = {7, 7, 7};
    assert(std::abs(calculateMutualInformation(X3, Y3) - 0.0) < 1e-9);

    // Case 4: Different lengths -> truncate to first 2 elements, X={1,2}, Y={1,2} -> MI = log(2)
    std::vector<uint32_t> X4 = {1, 2, 3};
    std::vector<uint32_t> Y4 = {1, 2};
    double expected4 = std::log(2.0);
    assert(std::abs(calculateMutualInformation(X4, Y4) - expected4) < 1e-9);

    // Case 5: One sequence empty -> returns 0.0
    std::vector<uint32_t> X5 = {};
    std::vector<uint32_t> Y5 = {1, 2};
    assert(calculateMutualInformation(X5, Y5) == 0.0);

    // Case 6: Large values (up to 2^32-1) and many duplicates, uniform distribution of 4 values -> MI = log(4)
    std::vector<uint32_t> X6 = {0, 1, 2, 3, 0, 1, 2, 3};
    std::vector<uint32_t> Y6 = {0, 0, 0, 0, 1, 1, 1, 1}; // independent -> MI = 0
    assert(std::abs(calculateMutualInformation(X6, Y6) - 0.0) < 1e-9);

    return 0;
}
