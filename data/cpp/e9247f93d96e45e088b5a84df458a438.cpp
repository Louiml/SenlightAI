/*
Write a C++ function that, given two vectors of integers representing the greed factor of children and the size of cookies, returns the maximum number of content children. A child is content if they receive a cookie whose size is at least their greed factor. Each child can receive at most one cookie, and each cookie can be given to at most one child. The function must handle empty vectors, non-positive values, and unsorted input. Do not modify the input vectors; return the count as an integer.
*/
#include <vector>
#include <algorithm>

// Returns the maximum number of content children given their greed factors and cookie sizes.
int findMaxContentChildren(const std::vector<int>& greed, const std::vector<int>& cookieSizes) {
    if (greed.empty() || cookieSizes.empty()) {
        return 0;
    }

    // Copy and sort to avoid modifying the input vectors.
    std::vector<int> sortedGreed = greed;
    std::vector<int> sortedCookies = cookieSizes;
    std::sort(sortedGreed.begin(), sortedGreed.end());
    std::sort(sortedCookies.begin(), sortedCookies.end());

    int childIndex = 0;
    int cookieIndex = 0;

    while (childIndex < sortedGreed.size() && cookieIndex < sortedCookies.size()) {
        if (sortedCookies[cookieIndex] >= sortedGreed[childIndex]) {
            // This cookie satisfies the current child's greed.
            ++childIndex;
        }
        // Move to the next cookie regardless.
        ++cookieIndex;
    }

    return childIndex;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    std::vector<int> g1 = {1, 2, 3};
    std::vector<int> s1 = {1, 1};
    assert(findMaxContentChildren(g1, s1) == 1);

    std::vector<int> g2 = {1, 2};
    std::vector<int> s2 = {1, 2, 3};
    assert(findMaxContentChildren(g2, s2) == 2);

    std::vector<int> g3 = {10, 9, 8, 7};
    std::vector<int> s3 = {5, 6, 7, 8};
    assert(findMaxContentChildren(g3, s3) == 2);

    std::vector<int> g4 = {};
    std::vector<int> s4 = {1, 2, 3};
    assert(findMaxContentChildren(g4, s4) == 0);

    std::vector<int> g5 = {1, 1, 1};
    std::vector<int> s5 = {};
    assert(findMaxContentChildren(g5, s5) == 0);

    // Negative and zero values
    std::vector<int> g6 = {-1, 0, 2};
    std::vector<int> s6 = {-5, 0, 1, 3};
    assert(findMaxContentChildren(g6, s6) == 2);

    // Unsorted input, duplicates
    std::vector<int> g7 = {3, 1, 2, 1};
    std::vector<int> s7 = {2, 1, 3, 1};
    assert(findMaxContentChildren(g7, s7) == 3);

    // All cookies too small
    std::vector<int> g8 = {5, 5, 5};
    std::vector<int> s8 = {1, 2, 3};
    assert(findMaxContentChildren(g8, s8) == 0);

    // All cookies large enough
    std::vector<int> g9 = {1, 2, 3};
    std::vector<int> s9 = {4, 5, 6};
    assert(findMaxContentChildren(g9, s9) == 3);

    // Single child and single cookie
    std::vector<int> g10 = {7};
    std::vector<int> s10 = {7};
    assert(findMaxContentChildren(g10, s10) == 1);

    return 0;
}
// The optimal strategy is a greedy approach: sort both vectors in ascending order to pair the smallest possible cookie with the smallest possible greed. Initialize two indices, one for children (`i`) and one for cookies (`j`). Iterate while both indices are within bounds. If the current cookie size can satisfy the current child’s greed, increment the child index (meaning that child is content). Always increment the cookie index regardless, because the cookie is either used or too small for the current and all future children. The final value of `i` is the maximum number of content children. Edge cases: if either vector is empty, return 0; if the smallest cookie is smaller than the smallest greed, no child can be satisfied; all negative or zero values are handled naturally by sorting. Time complexity is O(n log n + m log m) due to sorting, where `n` is the number of children and `m` is the number of cookies. Space complexity is O(1) auxiliary, assuming the input vectors are not modified and we only use a few integer variables.
