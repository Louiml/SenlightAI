Write a C++ function named `sumColumns` that takes two constant references to `std::vector<int>` objects (representing two sequences of exactly five integer values each) and returns a `std::vector<int>` containing the element-wise sums of the corresponding positions (i.e., result[i] = a[i] + b[i]). The function must assume both vectors are exactly of size 5. If either vector is not of size 5, the function should return an empty vector as a safety check. The function should not modify the input vectors.

#include <cassert>
#include <vector>

// (Include the solution function above or in the same file)

int main() {
    // Normal case
    std::vector<int> a1 = {1, 2, 3, 4, 5};
    std::vector<int> b1 = {10, 20, 30, 40, 50};
    assert(sumColumns(a1, b1) == std::vector<int>({11, 22, 33, 44, 55}));

    // All zeros
    std::vector<int> a2 = {0, 0, 0, 0, 0};
    std::vector<int> b2 = {0, 0, 0, 0, 0};
    assert(sumColumns(a2, b2) == std::vector<int>({0, 0, 0, 0, 0}));

    // Negative values
    std::vector<int> a3 = {-1, -2, -3, -4, -5};
    std::vector<int> b3 = {1, 2, 3, 4, 5};
    assert(sumColumns(a3, b3) == std::vector<int>({0, 0, 0, 0, 0}));

    // Mixed signs
    std::vector<int> a4 = {5, -5, 10, -10, 0};
    std::vector<int> b4 = {-5, 5, -10, 10, 0};
    assert(sumColumns(a4, b4) == std::vector<int>({0, 0, 0, 0, 0}));

    // First vector too short
    std::vector<int> a5 = {1, 2, 3};
    std::vector<int> b5 = {1, 2, 3, 4, 5};
    assert(sumColumns(a5, b5).empty());

    // Second vector too long
    std::vector<int> a6 = {1, 2, 3, 4, 5};
    std::vector<int> b6 = {1, 2, 3, 4, 5, 6};
    assert(sumColumns(a6, b6).empty());

    // Both empty
    std::vector<int> a7, b7;
    assert(sumColumns(a7, b7).empty());

    // Large values within int range
    std::vector<int> a8 = {1000000, 2000000, 3000000, 4000000, 5000000};
    std::vector<int> b8 = {9000000, 8000000, 7000000, 6000000, 5000000};
    assert(sumColumns(a8, b8) == std::vector<int>({10000000, 10000000, 10000000, 10000000, 10000000}));
}

#include <vector>

// Compute element-wise sums of two fixed-size (5) vectors.
// Returns an empty vector if either input is not exactly size 5.
std::vector<int> sumColumns(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != 5 || b.size() != 5) {
        return {};
    }
    std::vector<int> result(5);
    for (int i = 0; i < 5; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

// The solution is straightforward: first check that both input vectors have a size equal to 5. If either size is not 5, return an empty vector immediately to avoid out-of-bounds access. Otherwise, create a result vector of size 5, iterate from index 0 to 4, and set each element of the result to the sum of the corresponding elements of the two input vectors. Because the input vectors are passed by `const` reference, no copying occurs and the original data is not modified. Edge cases include vectors of different sizes (either less or greater than 5) and empty vectors—both are handled by the size check. The algorithm runs in O(5) = O(1) time and uses O(1) auxiliary space beyond the output vector itself.
