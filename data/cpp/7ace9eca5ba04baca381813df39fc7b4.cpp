/*
Write a C++ function `countIntersection` that takes two vectors of integers representing the catalogue numbers of CDs owned by Jack and Jill, and returns the number of CDs that both own. The input vectors are not sorted, may contain duplicate catalogue numbers within each owner's collection (but each unique CD is counted only once even if owned multiple times by the same person), and the function should handle empty vectors. The function must count only catalogue numbers that appear at least once in both vectors.
*/

#include <vector>
#include <unordered_set>

// Count the number of distinct integer values that appear in both vectors.
int countIntersection(const std::vector<int>& vecA, const std::vector<int>& vecB) {
    std::unordered_set<int> uniqueA(vecA.begin(), vecA.end());
    int count = 0;
    for (const int value : vecB) {
        if (uniqueA.find(value) != uniqueA.end()) {
            ++count;
            uniqueA.erase(value); // avoid double-counting duplicates in vecB
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Declaration of the function under test.
int countIntersection(const std::vector<int>& vecA, const std::vector<int>& vecB);

int main() {
    // Empty vectors
    assert(countIntersection({}, {}) == 0);
    assert(countIntersection({1,2}, {}) == 0);
    assert(countIntersection({}, {3,4}) == 0);

    // No common elements
    assert(countIntersection({1,2,3}, {4,5,6}) == 0);

    // All elements common
    assert(countIntersection({1,2,3}, {1,2,3}) == 3);

    // Duplicates within each vector
    assert(countIntersection({1,1,2,2}, {1,2,2}) == 2);

    // Mixed common and unique
    assert(countIntersection({1,2,3,4}, {2,3,5}) == 2);

    // Unsorted vectors, negative values
    assert(countIntersection({-1,0,2,5}, {5,7,-1}) == 2);

    // Large values and duplicates
    assert(countIntersection({100,100,200}, {200,200,300}) == 1);

    // One element
    assert(countIntersection({42}, {42}) == 1);
    assert(countIntersection({42}, {43}) == 0);

    // More duplicates in second
    assert(countIntersection({1,2}, {1,1,1,1,1}) == 1);

    return 0;
}

// The problem is a classic set intersection count. The most efficient approach is to insert all elements from the first vector into an `unordered_set` (or `std::set` if deterministic order is desired, but `unordered_set` gives average O(1) lookups). Then, iterate through the second vector; for each element, check if it exists in the set and has not already been counted (to avoid counting duplicates in the second vector). We can achieve this by using a separate `unordered_set` to track already counted values, or by erasing the found element from the first set after counting it. The latter is simpler: when we find a match, increment the counter and erase that value from the set, so subsequent duplicates in the second vector will not match. Edge cases: empty vectors yield 0; vectors with no common values yield 0; vectors with all common values yield the size of the unique intersection. Time complexity is O(n + m) on average for `unordered_set`, where n and m are the sizes of the vectors; space complexity is O(n) for the set (or O(min(n,m)) if we choose the smaller vector to build the set — but for simplicity we build from the first).
