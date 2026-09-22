// Write a C++ function `vectorDotProduct(const std::vector<int>& a, const std::vector<int>& b)` that computes the dot product of two n-dimensional integer vectors. The function should accept two vectors of equal length, where each element has absolute value ≤ 1000, and return the result as a `long long` to safely accommodate the maximum possible sum (1000*1000*1000 = 1e9, but using long long for general safety). The function must handle input vectors of size n (1 ≤ n ≤ 1000) and return the sum of products a[i] * b[i] for all i. You may assume the input vectors have identical sizes (no need to check), but for robustness you can return 0 if sizes differ. Do not modify the input vectors.
The dot product is a simple linear combination: iterate through each index from 0 to n-1, multiply the corresponding elements from vectors a and b, and accumulate the sum. Initialize the result as `long long` because even though the inputs are within ±1000 and n ≤ 1000, the maximum product sum is 1000*1000*1000 = 1,000,000,000, which fits in a 32-bit integer but using long long avoids any overflow concerns and is good practice. The algorithm runs in O(n) time, iterating once through the vectors. Space complexity is O(1) beyond the input storage, since we only use a few scalar variables. Edge cases: empty vectors (if sizes are 0, return 0), vectors of size 1 (simple product), and vectors with negative numbers (handled naturally by multiplication and addition). No special handling for duplicates or ordering is required.
#include <vector>
#include <cstddef>

// Computes the dot product of two integer vectors of equal length.
// Returns 0 if the vectors have different sizes (handles empty input).
long long vectorDotProduct(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) {
        return 0; // Defensive; problem guarantees equal sizes.
    }
    long long sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        sum += static_cast<long long>(a[i]) * b[i];
    }
    return sum;
}
#include <cassert>
#include <vector>

// The solution function is defined above (or in a header). 
// Test cases:
int main() {
    // Example from problem statement: (1,4,6) · (2,1,5) = 2 + 4 + 30 = 36
    std::vector<int> a1 = {1, 4, 6};
    std::vector<int> b1 = {2, 1, 5};
    assert(vectorDotProduct(a1, b1) == 36);

    // Single element
    std::vector<int> a2 = {5};
    std::vector<int> b2 = {-7};
    assert(vectorDotProduct(a2, b2) == -35);

    // Both negative
    std::vector<int> a3 = {-2, -3};
    std::vector<int> b3 = {-4, -5};
    assert(vectorDotProduct(a3, b3) == (8 + 15));

    // With zeros
    std::vector<int> a4 = {0, 10, -10};
    std::vector<int> b4 = {100, 0, -10};
    assert(vectorDotProduct(a4, b4) == 0 + 0 + 100);

    // Large values to check overflow safety
    std::vector<int> a5(1000, 1000);
    std::vector<int> b5(1000, 1000);
    assert(vectorDotProduct(a5, b5) == 1000000000LL); // 1000*1000*1000

    // Mixed sizes (defensive: should return 0)
    std::vector<int> a6 = {1, 2, 3};
    std::vector<int> b6 = {4, 5};
    assert(vectorDotProduct(a6, b6) == 0);

    // Empty vectors
    std::vector<int> a7, b7;
    assert(vectorDotProduct(a7, b7) == 0);

    // Identical vectors
    std::vector<int> a8 = {2, 2, 2};
    assert(vectorDotProduct(a8, a8) == 12);

    return 0;
}
