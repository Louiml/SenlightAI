/*
Write a C++ function `int findContentChildren(vector<int>& g, vector<int>& s)` that returns the maximum number of content children, where each child has a greed factor `g[i]` (minimum cookie size they will accept) and each cookie has a size `s[j]`. A child can only be assigned one cookie, and a cookie can only be given to one child. A child is content if the cookie size is at least their greed factor. The goal is to maximize the number of content children. The input vectors are unsorted and may contain duplicate values; the function must not modify the original vectors (pass them by const reference). The solution should handle empty inputs gracefully.
*/
#include <vector>
#include <algorithm>

// Returns the maximum number of children that can be content.
// Children's greed factors (g) and cookie sizes (s) are passed by const reference.
int findContentChildren(const std::vector<int>& g, const std::vector<int>& s) {
    // Sort copies to avoid modifying the original inputs.
    std::vector<int> greed = g;
    std::vector<int> sizes = s;
    std::sort(greed.begin(), greed.end());
    std::sort(sizes.begin(), sizes.end());

    size_t i = 0; // child index
    size_t j = 0; // cookie index
    int content = 0;

    while (i < greed.size() && j < sizes.size()) {
        if (sizes[j] >= greed[i]) {
            // This cookie satisfies the least greedy unsatisfied child.
            ++i;
            ++j;
            ++content;
        } else {
            // Cookie is too small for this child; skip it.
            ++j;
        }
    }
    return content;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(findContentChildren({1,2,3}, {1,1}) == 1);
    assert(findContentChildren({1,2}, {1,2,3}) == 2);
    // Empty inputs
    assert(findContentChildren({}, {1,2}) == 0);
    assert(findContentChildren({1,2}, {}) == 0);
    // All cookies too small
    assert(findContentChildren({2,3,4}, {1,1}) == 0);
    // Duplicate greed and sizes
    assert(findContentChildren({1,1,1}, {1,1,1}) == 3);
    // Unsorted inputs
    assert(findContentChildren({3,1,2}, {2,3,1}) == 3);
    // One child many cookies
    assert(findContentChildren({5}, {1,2,3,4,5}) == 1);
    // One cookie many children
    assert(findContentChildren({1,2,3}, {3}) == 1);
    // Mixed sizes and greed
    assert(findContentChildren({10,9,8,7}, {5,6,7,8}) == 2);
    return 0;
}
// The optimal strategy is a greedy two-pointer approach after sorting both arrays. Sort the children’s greed factors and the cookie sizes in ascending order. Use two indices: `i` for children and `j` for cookies. Iterate while both indices are within bounds. If the current cookie satisfies the current child (`s[j] >= g[i]`), assign it, increment both indices, and increase the count. Otherwise, the current cookie is too small for this child (and all remaining children, since they are sorted), so skip it by incrementing `j` only. This ensures each cookie is either used to satisfy the least greedy unsatisfied child or discarded, maximizing matches. Since both arrays are sorted, once a cookie is too small for the smallest remaining greed, it can never satisfy any later (larger) greed either. The algorithm runs in O(n log n + m log m) time due to sorting and O(1) auxiliary space (ignoring input storage). Edge cases: any empty vector returns 0; all cookies too small returns 0; all children satisfied returns the size of the smaller of the two arrays; duplicate values are handled naturally by sorting.
