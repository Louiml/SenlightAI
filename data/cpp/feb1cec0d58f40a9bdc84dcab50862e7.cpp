Write a standalone C++ function named `differenceOfSets` that takes two vectors of integers, `nums1` and `nums2`, and returns a `vector<vector<int>>` where the first inner vector contains all distinct integers present in `nums1` but not in `nums2`, and the second inner vector contains all distinct integers present in `nums2` but not in `nums1`. The output vectors must be sorted in ascending order, and duplicates in the input should be ignored. The function must be `const`-correct: it should take the input vectors by const reference and not modify them. Assume inputs may be empty, and may contain negative numbers, zero, and large values. The result should contain no duplicate elements.

#include <cassert>
#include <vector>

// The solution function is declared above; main checks correctness.
int main() {
    // Basic case with distinct elements
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> a2 = {2, 4, 6};
    auto res = differenceOfSets(a1, a2);
    assert(res[0] == std::vector<int>({1, 3}));
    assert(res[1] == std::vector<int>({4, 6}));

    // Empty inputs
    std::vector<int> b1 = {};
    std::vector<int> b2 = {};
    res = differenceOfSets(b1, b2);
    assert(res[0].empty() && res[1].empty());

    // One empty, one non-empty
    std::vector<int> c1 = {5, -1, 5};
    std::vector<int> c2 = {};
    res = differenceOfSets(c1, c2);
    assert(res[0] == std::vector<int>({-1, 5}));
    assert(res[1].empty());

    // Duplicate values in both
    std::vector<int> d1 = {7, 7, 8, 9};
    std::vector<int> d2 = {8, 8, 10};
    res = differenceOfSets(d1, d2);
    assert(res[0] == std::vector<int>({7, 9}));
    assert(res[1] == std::vector<int>({10}));

    // Identical sets produce empty result
    std::vector<int> e1 = {1, 2, 3};
    std::vector<int> e2 = {3, 2, 1};
    res = differenceOfSets(e1, e2);
    assert(res[0].empty() && res[1].empty());

    // Negative numbers and zeros
    std::vector<int> f1 = {-3, -3, 0, 4};
    std::vector<int> f2 = {0, 5, -3};
    res = differenceOfSets(f1, f2);
    assert(res[0] == std::vector<int>({4}));
    assert(res[1] == std::vector<int>({5}));

    // Large numbers
    std::vector<int> g1 = {1000000, 1, 1000000};
    std::vector<int> g2 = {999999, 1, 2};
    res = differenceOfSets(g1, g2);
    assert(res[0] == std::vector<int>({1000000}));
    assert(res[1] == std::vector<int>({2, 999999}));

    // All elements unique to each set
    std::vector<int> h1 = {10, 20};
    std::vector<int> h2 = {30, 40};
    res = differenceOfSets(h1, h2);
    assert(res[0] == std::vector<int>({10, 20}));
    assert(res[1] == std::vector<int>({30, 40}));

    // Single element in each
    std::vector<int> i1 = {42};
    std::vector<int> i2 = {42};
    res = differenceOfSets(i1, i2);
    assert(res[0].empty() && res[1].empty());
}

#include <vector>
#include <set>

// Return two sorted vectors: elements unique to nums1, then elements unique to nums2.
std::vector<std::vector<int>> differenceOfSets(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    std::set<int> set1(nums1.begin(), nums1.end());
    std::set<int> set2(nums2.begin(), nums2.end());

    std::vector<std::vector<int>> result(2);

    for (int value : set1) {
        if (set2.find(value) == set2.end()) {
            result[0].push_back(value);
        }
    }

    for (int value : set2) {
        if (set1.find(value) == set1.end()) {
            result[1].push_back(value);
        }
    }

    return result;
}

// The problem is solved by leveraging sets to eliminate duplicates and to enable fast membership testing. First, construct two `std::set<int>` objects from each input vector; insertion automatically removes duplicates and sorts the elements in ascending order. Then, for each element in the first set, check if it exists in the second set using the `contains` member function (C++20) or `find` for earlier standards; if not, append it to the first result vector. Repeat the process for the second set and append to the second result vector. Edge cases: empty inputs produce empty result vectors; identical inputs produce two empty vectors; inputs with all unique elements produce full results; negatives and zeros are handled naturally by the set ordering. Time complexity is O(N log N + M log M + N + M) due to set construction and linear scans, where N and M are the input sizes. Space complexity is O(N + M) for the sets, plus the result vectors.
