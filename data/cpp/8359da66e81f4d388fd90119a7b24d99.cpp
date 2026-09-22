Write a standalone C++ function that reads a sequence of non-negative integers from a vector (passed as a parameter) and returns the k-th element from the end of the sequence, where k is also passed as a parameter. If k is invalid (i.e., less than 1 or greater than the number of elements in the vector), the function should return a special sentinel value of -1 to indicate an error. The function must handle empty vectors, edge cases where k equals the vector size, and large k values. The input vector may contain zeros and arbitrary non-negative integers, and the function must be const-correct, not modifying the input.
#include <cassert>
#include <vector>

// Function under test (declaration, implementation above in actual file)
int kthFromEnd(const std::vector<int>&, int);

int main() {
    // Basic cases
    std::vector<int> v1 = {10, 20, 30, 40, 50};
    assert(kthFromEnd(v1, 1) == 50);
    assert(kthFromEnd(v1, 2) == 40);
    assert(kthFromEnd(v1, 5) == 10);

    // Single element
    std::vector<int> v2 = {7};
    assert(kthFromEnd(v2, 1) == 7);

    // All zeros
    std::vector<int> v3 = {0, 0, 0};
    assert(kthFromEnd(v3, 1) == 0);
    assert(kthFromEnd(v3, 3) == 0);

    // Invalid k values
    assert(kthFromEnd(v1, 0) == -1);
    assert(kthFromEnd(v1, 6) == -1);
    assert(kthFromEnd(v1, -3) == -1);

    // Empty vector
    std::vector<int> v4;
    assert(kthFromEnd(v4, 1) == -1);
    assert(kthFromEnd(v4, 5) == -1);

    // Large vector with edge k = size
    std::vector<int> v5 = {5, 6, 7};
    assert(kthFromEnd(v5, 3) == 5);

    // k equals size+1
    assert(kthFromEnd(v5, 4) == -1);
}
#include <vector>
#include <cstddef>

// Returns the k-th element from the end of the vector, or -1 if k is invalid.
// k must be in the range [1, v.size()]. Returns -1 for invalid k or empty vector.
int kthFromEnd(const std::vector<int>& v, int k) {
    if (k <= 0 || k > static_cast<int>(v.size())) {
        return -1;
    }
    // Index is size - k; k > 0 so no underflow before cast.
    return v[v.size() - static_cast<std::size_t>(k)];
}
// The main algorithm is straightforward: given a vector `v` of non-negative integers and an integer `k`, we need to access the element at position `v.size() - k` (0‑based index). If `k` is out of range (i.e., `k < 1` or `k > v.size()`), we return -1. For valid input, we return `v[v.size() - k]`. Edge cases include: empty vector (k is always invalid since v.size() = 0), k equal to v.size() (returns the first element), k equal to 1 (returns the last element), and k greater than v.size() (invalid). Negative k is also invalid because k is defined as a positive distance from the end. Time complexity is O(1) since we directly index the vector. Space complexity is O(1) as no extra storage is needed. The function should be declared with `const std::vector<int>&` to avoid copying and to enforce const-correctness.
