Write a C++ function `int maxChildrenServed(vector<int>& greed, vector<int>& cookieSizes)` that takes two vectors: `greed` representing the minimum cookie size each child requires, and `cookieSizes` representing the sizes of available cookies. Each child can receive at most one cookie, and each cookie can be given to at most one child. A child is satisfied if they receive a cookie of size at least their greed value. The function must return the maximum number of children that can be satisfied. The input vectors may be unsorted, can contain duplicate values, and may be empty. The function should not modify the original input vectors; it should work on copies or sort them internally.

The problem is a classic greedy assignment problem. Sort both the greed requirements and cookie sizes in non-decreasing order. Use two pointers: one index `i` for children (greed) and one index `j` for cookies (sizes). Iterate through cookies from smallest to largest. For each cookie, if its size is at least the current child's greed requirement, assign that cookie to that child, increment the child pointer, and increase the count of satisfied children. Regardless of assignment, always move to the next cookie. Continue until either all cookies are processed or all children are satisfied. This works because a larger cookie should never be wasted on a child with smaller greed when a smaller cookie could suffice; by processing in sorted order, we maximize the number of satisfied children. Edge cases: empty greed or empty cookieSizes returns 0. If the smallest cookie is too small for the most greedy child, it is skipped. Duplicate greed values or cookie sizes are handled naturally by sorting. Time complexity is O(N log N + M log M) due to sorting, and space complexity is O(1) auxiliary (ignoring input storage) if we sort copies or sort in place; the function should avoid modifying inputs, so sorting copies would make space O(N+M) but we can sort the vectors by value (pass by value) or sort references and restore; better to pass by value to keep the interface clean. In practice, we can pass by value, sort the copies, and proceed.

#include <vector>
#include <algorithm>

// Returns the maximum number of children that can be satisfied.
// Each child requires a cookie of size at least their greed value.
// Each cookie can be assigned to at most one child.
int maxChildrenServed(std::vector<int> greed, std::vector<int> cookieSizes) {
    std::sort(greed.begin(), greed.end());
    std::sort(cookieSizes.begin(), cookieSizes.end());

    int childIdx = 0;
    int cookieIdx = 0;
    int satisfied = 0;

    while (childIdx < static_cast<int>(greed.size()) &&
           cookieIdx < static_cast<int>(cookieSizes.size())) {
        if (cookieSizes[cookieIdx] >= greed[childIdx]) {
            ++childIdx;
            ++satisfied;
        }
        ++cookieIdx;
    }

    return satisfied;
}

#include <cassert>
#include <vector>

int maxChildrenServed(std::vector<int> greed, std::vector<int> cookieSizes);

int main() {
    // Basic case
    std::vector<int> g1 = {1, 2, 3};
    std::vector<int> c1 = {1, 1};
    assert(maxChildrenServed(g1, c1) == 1);

    // All children can be satisfied
    std::vector<int> g2 = {1, 2};
    std::vector<int> c2 = {1, 2, 3};
    assert(maxChildrenServed(g2, c2) == 2);

    // No cookies
    std::vector<int> g3 = {1, 2};
    std::vector<int> c3 = {};
    assert(maxChildrenServed(g3, c3) == 0);

    // No children
    std::vector<int> g4 = {};
    std::vector<int> c4 = {5, 6};
    assert(maxChildrenServed(g4, c4) == 0);

    // Duplicate greed and cookie sizes
    std::vector<int> g5 = {3, 3, 3};
    std::vector<int> c5 = {3, 3, 4};
    assert(maxChildrenServed(g5, c5) == 3);

    // Cookies too small
    std::vector<int> g6 = {5, 6};
    std::vector<int> c6 = {1, 2};
    assert(maxChildrenServed(g6, c6) == 0);

    // Unsorted inputs with large cookies
    std::vector<int> g7 = {2, 1, 3};
    std::vector<int> c7 = {4, 1, 2};
    assert(maxChildrenServed(g7, c7) == 2);

    // One child with exact match
    std::vector<int> g8 = {7};
    std::vector<int> c8 = {7};
    assert(maxChildrenServed(g8, c8) == 1);

    // Many extra cookies
    std::vector<int> g9 = {1};
    std::vector<int> c9 = {1, 2, 3, 4};
    assert(maxChildrenServed(g9, c9) == 1);

    // Large greed, large cookies
    std::vector<int> g10 = {10, 20, 30};
    std::vector<int> c10 = {30, 20, 10, 5};
    assert(maxChildrenServed(g10, c10) == 3);

    return 0;
}
