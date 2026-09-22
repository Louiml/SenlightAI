Write a C++ function named `mergeDescending` that takes three linked lists of integers (the lists are provided as `std::list<int>` containers) and merges the first two lists into the third list in strictly non-increasing (descending) order. The function must remove all elements from the first two lists as they are merged, leaving both input lists empty. The input lists are not guaranteed to be sorted initially, and the third list must be empty (or cleared) before the merge begins. The function should handle lists of any length, including empty lists, and duplicate values should be preserved.

#include <list>
#include <cassert>

// Assume mergeDescending is declared above or included from header.

int main() {
    // Test 1: Basic merge of two sorted-ish lists.
    std::list<int> a{1, 5, 3};
    std::list<int> b{4, 2};
    std::list<int> c;
    mergeDescending(a, b, c);
    assert(a.empty() && b.empty());
    assert(c == std::list<int>({5, 4, 3, 2, 1}));

    // Test 2: Both input lists empty.
    std::list<int> d, e, f;
    mergeDescending(d, e, f);
    assert(d.empty() && e.empty() && f.empty());

    // Test 3: One list empty, other non-empty.
    std::list<int> g, h{7, 1, 9}, i;
    mergeDescending(g, h, i);
    assert(g.empty() && h.empty());
    assert(i == std::list<int>({9, 7, 1}));

    // Test 4: Duplicate values preserved.
    std::list<int> j{3, 3, 1}, k{3, 2}, l;
    mergeDescending(j, k, l);
    assert(l == std::list<int>({3, 3, 3, 2, 1}));

    // Test 5: Negative numbers.
    std::list<int> m{-5, -1, -10}, n{-2, -20}, o;
    mergeDescending(m, n, o);
    assert(o == std::list<int>({-1, -2, -5, -10, -20}));

    // Test 6: Third list initially non-empty, should be cleared.
    std::list<int> p{2}, q{1}, r{99, 100};
    mergeDescending(p, q, r);
    assert(r == std::list<int>({2, 1}));

    // Test 7: Single element each.
    std::list<int> s{10}, t{20}, u;
    mergeDescending(s, t, u);
    assert(u == std::list<int>({20, 10}));

    // Test 8: Large values and many duplicates.
    std::list<int> v{100, 100, 0}, w{100, -100}, x;
    mergeDescending(v, w, x);
    assert(x == std::list<int>({100, 100, 100, 0, -100}));

    // Test 9: Already sorted descending.
    std::list<int> y{5, 4, 3}, z{2, 1}, aa;
    mergeDescending(y, z, aa);
    assert(aa == std::list<int>({5, 4, 3, 2, 1}));

    // Test 10: Reverse order.
    std::list<int> ab{1, 2}, ac{3, 4}, ad;
    mergeDescending(ab, ac, ad);
    assert(ad == std::list<int>({4, 3, 2, 1}));

    return 0;
}

#include <list>
#include <algorithm>

/**
 * Merges two integer lists into a third list in descending order.
 * The input lists are emptied during the merge.
 * @param L1 First input list (will be emptied).
 * @param L2 Second input list (will be emptied).
 * @param L3 Output list; must be empty or will be cleared before filling.
 */
void mergeDescending(std::list<int>& L1, std::list<int>& L2, std::list<int>& L3) {
    // Clear the output list to ensure it starts empty.
    L3.clear();

    // Sort both input lists in descending order.
    L1.sort(std::greater<int>());
    L2.sort(std::greater<int>());

    // Two-pointer merge.
    auto it1 = L1.begin();
    auto it2 = L2.begin();

    while (it1 != L1.end() && it2 != L2.end()) {
        if (*it1 >= *it2) {
            L3.push_back(*it1);
            it1 = L1.erase(it1); // erase returns next iterator
        } else {
            L3.push_back(*it2);
            it2 = L2.erase(it2);
        }
    }

    // Append remaining elements from L1.
    while (it1 != L1.end()) {
        L3.push_back(*it1);
        it1 = L1.erase(it1);
    }

    // Append remaining elements from L2.
    while (it2 != L2.end()) {
        L3.push_back(*it2);
        it2 = L2.erase(it2);
    }
}

// The core algorithm is a two-way merge procedure adapted for unsorted input. Since the input lists are not sorted, we cannot directly compare their current heads in a single pass. Instead, we repeatedly extract the maximum remaining element from either list and append it to the result list. To do this efficiently without scanning the entire lists repeatedly, we can first sort each input list in descending order (or use a priority queue), but the simplest and clearest approach for a teaching exercise is: while either input list is non-empty, scan both lists to find the largest remaining value, remove that occurrence, and append it to the result. This guarantees the result is in descending order. Alternatively, we can sort each list first using `std::sort` with a greater comparator, then perform a standard two-pointer merge. The sorting approach is more efficient and easier to reason about. Edge cases include empty input lists, lists with a single element, duplicates, and the requirement that the third list is cleared before merging (or assumed empty). Time complexity: if we sort each list, O(n log n + m log m) for sorting plus O(n + m) for the merge, where n and m are the initial sizes. Space complexity: O(1) auxiliary besides the result list itself, since we reuse the existing nodes by splicing them.
