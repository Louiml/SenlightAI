/*
Write a C++ function `void unionAndIntersection(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& uni, std::vector<int>& inter)` that takes two sorted arrays (vectors) of integers as input and outputs two vectors: one containing the union of the two arrays (distinct elements that appear in either array, in sorted order) and another containing the intersection (distinct elements that appear in both arrays, in sorted order). The input arrays are guaranteed to be sorted in non-decreasing order and may contain duplicates. The function must handle empty arrays and arrays where one is completely a subset of the other or where they share no common elements. The output vectors must contain only distinct elements, in sorted order, with no duplicates. Your implementation should use the two-pointer technique described in the provided snippet, but should be robust and avoid the bugs present in the original code (such as infinite loops when duplicates are handled incorrectly or when indices go out of bounds). The function must be efficient for large inputs with sizes \(n\) and \(m\), running in \(O(n+m)\) time and using \(O(1)\) extra space beyond the output vectors.
*/

#include <vector>
#include <algorithm>

// Compute union and intersection of two sorted arrays (vectors) with possible duplicates.
// Outputs are distinct sorted elements.
void unionAndIntersection(const std::vector<int>& a, const std::vector<int>& b,
                          std::vector<int>& uni, std::vector<int>& inter) {
    uni.clear();
    inter.clear();

    size_t i = 0, j = 0;
    size_t n = a.size(), m = b.size();

    // Helper lambda to append to union without duplicates
    auto appendUnion = [&](int val) {
        if (uni.empty() || uni.back() != val) {
            uni.push_back(val);
        }
    };

    // Helper lambda to append to intersection without duplicates
    auto appendInter = [&](int val) {
        if (inter.empty() || inter.back() != val) {
            inter.push_back(val);
        }
    };

    // Merge for union and intersection simultaneously
    while (i < n && j < m) {
        if (a[i] < b[j]) {
            appendUnion(a[i]);
            ++i;
        } else if (a[i] > b[j]) {
            appendUnion(b[j]);
            ++j;
        } else { // equal
            appendUnion(a[i]);
            appendInter(a[i]);
            ++i;
            ++j;
        }
    }

    // Remaining elements from a
    while (i < n) {
        appendUnion(a[i]);
        ++i;
    }

    // Remaining elements from b
    while (j < m) {
        appendUnion(b[j]);
        ++j;
    }
}

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is defined here (include the above code).

int main() {
    // Test 1: basic case with overlap and duplicates
    std::vector<int> a = {1, 2, 2, 3, 4};
    std::vector<int> b = {2, 3, 5, 6};
    std::vector<int> uni, inter;
    unionAndIntersection(a, b, uni, inter);
    std::vector<int> expected_uni = {1, 2, 3, 4, 5, 6};
    std::vector<int> expected_inter = {2, 3};
    assert(uni == expected_uni);
    assert(inter == expected_inter);

    // Test 2: one array empty
    a = {};
    b = {1, 1, 2};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({1, 2}));
    assert(inter.empty());

    // Test 3: both empty
    a = {};
    b = {};
    unionAndIntersection(a, b, uni, inter);
    assert(uni.empty());
    assert(inter.empty());

    // Test 4: no overlap
    a = {1, 3, 5};
    b = {2, 4, 6};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({1, 2, 3, 4, 5, 6}));
    assert(inter.empty());

    // Test 5: all same elements
    a = {7, 7, 7};
    b = {7, 7};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({7}));
    assert(inter == std::vector<int>({7}));

    // Test 6: one is subset of the other
    a = {1, 2, 3, 4};
    b = {2, 3};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({1, 2, 3, 4}));
    assert(inter == std::vector<int>({2, 3}));

    // Test 7: negative numbers and large values
    a = {-10, -5, -5, 0, 3};
    b = {-8, -5, 2, 3, 3, 10};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({-10, -8, -5, 0, 2, 3, 10}));
    assert(inter == std::vector<int>({-5, 3}));

    // Test 8: single element arrays
    a = {5};
    b = {5};
    unionAndIntersection(a, b, uni, inter);
    assert(uni == std::vector<int>({5}));
    assert(inter == std::vector<int>({5}));

    return 0;
}

// The core idea is the classic two-pointer merge for sorted arrays. For the **union**, we maintain indices `i` and `j` for arrays `a` and `b` respectively. At each step, we compare `a[i]` and `b[j]`. If `a[i] < b[j]`, we append `a[i]` to the union (if it is not equal to the last appended value) and increment `i`. If `b[j] < a[i]`, we similarly append `b[j]` and increment `j`. If they are equal, we append the value once and increment both `i` and `j`. After one array is exhausted, we append all remaining distinct elements from the other array. To avoid duplicates, we always check before appending whether the value equals the last value in the union (or use a sentinel or check if the vector is empty). For the **intersection**, we again use two pointers. While both indices are in bounds, if `a[i] < b[j]`, increment `i`; if `a[i] > b[j]`, increment `j`; if they are equal, we append that value to the intersection (if it is not the same as the last appended value) and increment both `i` and `j`. This naturally handles duplicates because we increment past any equal consecutive elements by moving both pointers. Edge cases: empty arrays—union returns the other array's distinct elements, intersection returns empty. Arrays with all identical elements—union has one element, intersection has one element. Arrays with no overlap—union merges both, intersection empty. Time complexity is \(O(n+m)\) because each index is incremented at most \(n+m\) times total. Space complexity is \(O(1)\) extra besides the output vectors.
