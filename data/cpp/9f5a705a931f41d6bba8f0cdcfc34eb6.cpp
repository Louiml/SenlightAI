Write a standalone C++ function that receives two `std::vector<int>` containers of equal size and returns an integer count of how many positions have identical values at the same index. The function must handle empty vectors (returning 0), vectors of unequal length (you may assume they are always equal, but if not, return -1 as an error sentinel), and should treat negative numbers, zeros, and duplicates normally. The function must be `const`-correct, take vectors by const reference, and be efficient in both time and space.
The solution simply iterates through both vectors simultaneously using a single index. For each index `i`, compare `vec1[i]` and `vec2[i]`; if equal, increment a counter. After the loop, return the counter. Edge cases: if either vector is empty, the loop runs zero times and we return 0 (correct). If the vectors have different sizes, return -1 immediately to signal invalid input (the task specification allows this because it says "you may assume they are always equal, but if not, return -1 as an error sentinel"). Time complexity is O(n) where n is the number of elements, and space complexity is O(1) extra space (just the counter). No special handling is needed for negative numbers or duplicates—direct equality comparison works.
#include <vector>

// Count positions where two vectors have equal values at the same index.
// Returns -1 if the vectors have different sizes, otherwise returns the count.
int countMatchingPositions(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    // If sizes differ, return error sentinel
    if (vec1.size() != vec2.size()) {
        return -1;
    }

    int matchCount = 0;
    // Iterate once, comparing element-wise
    for (std::size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] == vec2[i]) {
            ++matchCount;
        }
    }
    return matchCount;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // Basic equal vectors
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {1, 2, 3};
    assert(countMatchingPositions(a1, b1) == 3);

    // No matches
    std::vector<int> a2 = {1, 2, 3};
    std::vector<int> b2 = {4, 5, 6};
    assert(countMatchingPositions(a2, b2) == 0);

    // Partial matches with negatives and zeros
    std::vector<int> a3 = {-1, 0, 2, 5};
    std::vector<int> b3 = {-1, 0, 9, 5};
    assert(countMatchingPositions(a3, b3) == 3);

    // Empty vectors
    std::vector<int> e1;
    std::vector<int> e2;
    assert(countMatchingPositions(e1, e2) == 0);

    // Different sizes -> error sentinel
    std::vector<int> a4 = {1, 2};
    std::vector<int> b4 = {1, 2, 3};
    assert(countMatchingPositions(a4, b4) == -1);

    // Single element matching
    std::vector<int> a5 = {7};
    std::vector<int> b5 = {7};
    assert(countMatchingPositions(a5, b5) == 1);

    // Duplicate values, all match
    std::vector<int> a6 = {5, 5, 5};
    std::vector<int> b6 = {5, 5, 5};
    assert(countMatchingPositions(a6, b6) == 3);

    // Mixed with large numbers
    std::vector<int> a7 = {1000000, -1000000, 0};
    std::vector<int> b7 = {1000000, -1000000, 1};
    assert(countMatchingPositions(a7, b7) == 2);
}
