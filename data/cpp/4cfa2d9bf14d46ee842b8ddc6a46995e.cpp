/*
Write a C++ function `int maximumSatisfiedChildren(vector<int>& greedFactors, vector<int>& cookieSizes)` that determines the maximum number of children who can be satisfied given each child's greed factor (minimum cookie size they will accept) and a list of available cookie sizes. A child can be assigned at most one cookie, and each cookie can be given to at most one child. The function should return the largest possible count of satisfied children. For example, if greed = [1,2,3] and cookies = [1,1], only 1 child can be satisfied; if greed = [1,2] and cookies = [1,2,3], both children can be satisfied. The input vectors are not required to be sorted, and the function must not modify the original input vectors (so copy or sort local copies). The function must handle empty inputs gracefully, returning 0 if either list is empty.
*/

#include <vector>
#include <algorithm>

// Returns the maximum number of children that can be satisfied.
// Input vectors are not modified; copies are sorted internally.
int maximumSatisfiedChildren(std::vector<int> greedFactors, std::vector<int> cookieSizes) {
    // If either list is empty, no child can be satisfied.
    if (greedFactors.empty() || cookieSizes.empty()) {
        return 0;
    }

    // Sort copies of the input vectors in non-decreasing order.
    std::sort(greedFactors.begin(), greedFactors.end());
    std::sort(cookieSizes.begin(), cookieSizes.end());

    int childIdx = 0; // Index of the next unsatisfied child (in sorted order).
    int greedySize = static_cast<int>(greedFactors.size());

    // Iterate through all cookies in ascending order of size.
    for (int cookieIdx = 0; cookieIdx < static_cast<int>(cookieSizes.size()); ++cookieIdx) {
        // If all children are already satisfied, no need to continue.
        if (childIdx >= greedySize) {
            break;
        }
        // If the current cookie can satisfy the current child, assign it.
        if (cookieSizes[cookieIdx] >= greedFactors[childIdx]) {
            ++childIdx; // Move to next child.
        }
        // Otherwise, skip this cookie and try the next larger one.
    }

    return childIdx;
}

#include <cassert>
#include <vector>

// Declaration of the function (or include the header if separate).
int maximumSatisfiedChildren(std::vector<int> greedFactors, std::vector<int> cookieSizes);

int main() {
    // Basic examples from the problem description.
    assert(maximumSatisfiedChildren({1, 2, 3}, {1, 1}) == 1);
    assert(maximumSatisfiedChildren({1, 2}, {1, 2, 3}) == 2);

    // Empty inputs should return 0.
    assert(maximumSatisfiedChildren({}, {1, 2}) == 0);
    assert(maximumSatisfiedChildren({1, 2}, {}) == 0);
    assert(maximumSatisfiedChildren({}, {}) == 0);

    // Unsorted inputs.
    assert(maximumSatisfiedChildren({3, 1, 2}, {2, 3, 1}) == 3);

    // Duplicate values and some unsatisfied children.
    assert(maximumSatisfiedChildren({2, 2, 2}, {1, 2, 2}) == 2);

    // Larger cookies than needed but limited count.
    assert(maximumSatisfiedChildren({1, 1, 1}, {5}) == 1);

    // All cookies too small.
    assert(maximumSatisfiedChildren({5, 6}, {1, 2}) == 0);

    return 0;
}

// The optimal strategy is a greedy approach: sort both the greed factors and the cookie sizes in non-decreasing order. Use two indices: one iterating over cookies (`cookieIdx`) and one tracking the next unsatisfied child (`childIdx`). Initialize `childIdx = 0`, then for each cookie size in sorted order, check if that cookie is large enough for the current child (i.e., `cookie >= greed[childIdx]`). If yes, we "assign" the cookie to that child, increment `childIdx`, and move to the next cookie. If not, skip that cookie and try the next one. This works because assigning the smallest possible cookie that can satisfy a child leaves larger cookies for more demanding children later. We stop when all cookies are processed or all children are satisfied. Edge cases: empty greed or empty cookie list yields 0; duplicate values are handled naturally. Time complexity is O(G log G + S log S) due to sorting, where G = size of greed, S = size of cookies. Space complexity is O(G + S) if we copy the vectors to avoid modifying input, or O(1) extra if we sort in place (but the task requires not modifying input, so we copy). The main loop is O(G + S) after sorting.
