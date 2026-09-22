// Write a C++ function that takes two equal-length integer vectors (or arrays, passed with their size) and returns the minimum possible sum of products of corresponding elements after independently rearranging the order of elements within each vector. That is, given two vectors \(A\) and \(B\) of length \(n\), you may permute the elements of \(A\) and permute the elements of \(B\) arbitrarily; the goal is to minimize \(\sum_{i=0}^{n-1} A_i \cdot B_i\). The function should be named `minScalarProduct` and accept a `const std::vector<int>&` for each input, returning a `long long` (since products of up to \(10^5\) and \(10^5\) can exceed 32‑bit int). The input vectors are guaranteed to have the same non‑zero length. Your function must be self‑contained and not rely on global variables.
The classical solution is based on the rearrangement inequality, which states that for two sequences of real numbers, the sum of pairwise products is minimized when one sequence is sorted in ascending order and the other in descending order (and maximized when both are sorted the same way). Therefore, to minimize the scalar product, sort one vector in non‑decreasing order and the other in non‑increasing order, then multiply corresponding elements and accumulate their sum. This works for any integers (including negatives) because the inequality holds for all real numbers. Edge cases: the vectors have equal size (guaranteed), but the function should still handle the empty‑vector case (though not required) by returning 0. For large absolute values, use `long long` for the result to avoid overflow; each product uses `long long` cast, and accumulation is in `long long`. Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(1)\) auxiliary if we sort copies of the input vectors (because the inputs are `const`, we must copy them, so \(O(n)\) auxiliary space is needed for the copies). Alternatively, if the inputs were non‑const, we could sort in‑place. Here we create copies to respect `const` correctness.
#include <vector>
#include <algorithm>
#include <cstdint>

// Compute the minimum possible sum of products of corresponding elements
// after arbitrarily permuting each input vector. Uses rearrangement inequality:
// sort one vector ascending, the other descending, then accumulate products.
long long minScalarProduct(const std::vector<int>& A, const std::vector<int>& B) {
    std::vector<int> a = A;  // copy to preserve input constness
    std::vector<int> b = B;

    std::sort(a.begin(), a.end());                     // ascending
    std::sort(b.begin(), b.end(), std::greater<int>()); // descending

    long long result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += static_cast<long long>(a[i]) * static_cast<long long>(b[i]);
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above or in a header.

int main() {
    // Basic case: positive numbers
    std::vector<int> A1 = {3, 1, 2};
    std::vector<int> B1 = {4, 5, 6};
    assert(minScalarProduct(A1, B1) == 1*6 + 2*5 + 3*4); // 6+10+12 = 28

    // Negative numbers: rearrangement inequality still holds
    std::vector<int> A2 = {-1, -2, 3};
    std::vector<int> B2 = {4, -5, 2};
    // Sorted A: -2,-1,3 ; sorted B desc: 4,2,-5
    // Products: -8 + (-2) + (-15) = -25
    assert(minScalarProduct(A2, B2) == -25);

    // Duplicate values and zeros
    std::vector<int> A3 = {0, 5, 5, 0};
    std::vector<int> B3 = {7, -2, -2, 7};
    // Sorted A: 0,0,5,5 ; B desc: 7,7,-2,-2
    // Products: 0+0+(-10)+(-10) = -20
    assert(minScalarProduct(A3, B3) == -20);

    // Single element
    std::vector<int> A4 = {42};
    std::vector<int> B4 = {17};
    assert(minScalarProduct(A4, B4) == 42*17);

    // All equal values
    std::vector<int> A5 = {3, 3, 3};
    std::vector<int> B5 = {2, 2, 2};
    assert(minScalarProduct(A5, B5) == 18);

    // Mixed signs and large magnitude to test long long
    std::vector<int> A6 = {100000, -100000};
    std::vector<int> B6 = {100000, -100000};
    // Sorted A: -100000,100000 ; B desc: 100000,-100000
    // Products: -100000*100000 + 100000*(-100000) = -2e10
    assert(minScalarProduct(A6, B6) == -20000000000LL);

    // Check original vectors are not modified (const correctness)
    // Already const in function signature, but verify values remain
    assert(A1[0] == 3 && B1[2] == 6);
}
