// Write a C++ function `int countSatisfiedChildren(vector<int>& greed, vector<int>& cookies)` that determines the maximum number of children who can be satisfied given each child has a minimum greed factor (how much cookie size they require) and each cookie has a size. A child can be satisfied if assigned a cookie with size at least their greed factor. Each child can get at most one cookie, and each cookie can be given to at most one child. You may assign cookies in any order. The function should return the maximum possible count of satisfied children. The input vectors may be unsorted and may contain duplicates.
#include <cassert>
#include <vector>

int countSatisfiedChildren(std::vector<int>& greed, std::vector<int>& cookies);

int main() {
    // Basic case: both vectors sorted
    std::vector<int> g1 = {1, 2, 3};
    std::vector<int> c1 = {1, 1, 2};
    assert(countSatisfiedChildren(g1, c1) == 2);

    // Unsorted input
    std::vector<int> g2 = {3, 1, 2};
    std::vector<int> c2 = {2, 3, 1};
    assert(countSatisfiedChildren(g2, c2) == 3);

    // No cookies
    std::vector<int> g3 = {1, 2};
    std::vector<int> c3 = {};
    assert(countSatisfiedChildren(g3, c3) == 0);

    // No children
    std::vector<int> g4 = {};
    std::vector<int> c4 = {1, 2};
    assert(countSatisfiedChildren(g4, c4) == 0);

    // All cookies too small
    std::vector<int> g5 = {5, 6};
    std::vector<int> c5 = {1, 2};
    assert(countSatisfiedChildren(g5, c5) == 0);

    // All children satisfied with extra cookies
    std::vector<int> g6 = {1, 2};
    std::vector<int> c6 = {10, 20, 30};
    assert(countSatisfiedChildren(g6, c6) == 2);

    // Duplicates and same values
    std::vector<int> g7 = {2, 2, 2};
    std::vector<int> c7 = {2, 2};
    assert(countSatisfiedChildren(g7, c7) == 2);

    // Large greed, small cookies mixed
    std::vector<int> g8 = {10, 1, 5, 3};
    std::vector<int> c8 = {4, 2, 11};
    assert(countSatisfiedChildren(g8, c8) == 2);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum number of children that can be satisfied by assigning cookies.
// Each child has a minimum greed factor; a cookie can satisfy a child if its size >= greed.
int countSatisfiedChildren(std::vector<int>& greed, std::vector<int>& cookies) {
    // Sort both arrays to apply greedy matching.
    std::sort(greed.begin(), greed.end());
    std::sort(cookies.begin(), cookies.end());

    int childIdx = 0;
    int satisfied = 0;

    for (int cookieIdx = 0; cookieIdx < static_cast<int>(cookies.size()) && childIdx < static_cast<int>(greed.size()); ++cookieIdx) {
        if (cookies[cookieIdx] >= greed[childIdx]) {
            // This cookie satisfies the current least greedy child.
            ++satisfied;
            ++childIdx;
        }
        // If the cookie is too small, skip it; try the next cookie.
    }

    return satisfied;
}
// The task is a classic greedy matching problem. To maximize the number of satisfied children, sort both the greed factors and cookie sizes in non-decreasing order. Then use two pointers: one for children (starting at index 0) and one for cookies (also starting at index 0). Iterate through cookies; if the current cookie size is at least the current child’s greed, assign it and move to the next child. Regardless of assignment, always move to the next cookie. This works because sorting ensures that we always try to satisfy the least greedy child first with the smallest possible cookie that can satisfy them, leaving larger cookies for more demanding children. Edge cases: empty greed or cookies (return 0), cookies smaller than the smallest greed (no matches), or all cookies larger than all greed (all children satisfied). Time complexity is O(n log n + m log m) due to sorting, and space complexity is O(1) extra if sorting in-place (or O(n) if we copy vectors). The original code snippet had a bug where it read greed values twice and forgot to read cookie sizes; the corrected task addresses that.
