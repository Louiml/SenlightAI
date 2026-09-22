// Write a C++ function that takes a vector of non-negative integers representing the number of citations a researcher has received for each of their papers, sorted in non-decreasing order, and returns the researcher's h-index. The h-index is defined as the maximum integer h such that the researcher has at least h papers with at least h citations each. The input vector may be empty, and the values are guaranteed to be sorted in ascending order. The function should be efficient and handle large inputs gracefully.

The classic h-index problem is usually solved with sorting and binary search or by counting. Since the input is already sorted in ascending order, we can scan from the end of the array (largest citations) backwards. We maintain a count `k` representing how many papers we have seen so far that have at least `k+1` citations. Starting from the last element, for each element we check if it is greater than or equal to `k+1`. If it is, we increment `k` and move to the previous element; otherwise, we stop. The reason this works is that the array is sorted, so if a paper with lower index has fewer citations than `k+1`, then all papers before it will also have fewer, so no further progress can be made. Edge cases: if the vector is empty, return 0; if all papers have at least n citations, return n; if the largest citation is 0, return 0. Time complexity is O(n) in the worst case (when the h-index is n), but it terminates early for smaller h-indices. Space complexity is O(1) beyond the input.

#include <vector>

// Compute the h-index from a sorted (non-decreasing) vector of citation counts.
// The h-index is the maximum h such that at least h papers have at least h citations.
int hIndexFromSorted(const std::vector<int>& citations) {
    const int n = static_cast<int>(citations.size());
    int h = 0;
    // Scan from the highest citations downward.
    for (int i = n - 1; i >= 0; --i) {
        if (citations[i] >= h + 1) {
            ++h;
        } else {
            break; // Since sorted, no earlier paper can satisfy the condition.
        }
    }
    return h;
}

#include <cassert>
#include <vector>

int main() {
    // Empty vector
    assert(hIndexFromSorted({}) == 0);
    // All zeros
    assert(hIndexFromSorted({0, 0, 0}) == 0);
    // Single element
    assert(hIndexFromSorted({0}) == 0);
    assert(hIndexFromSorted({1}) == 1);
    assert(hIndexFromSorted({5}) == 1); // h can't exceed number of papers
    // Classic examples
    assert(hIndexFromSorted({0, 1, 3, 5, 6}) == 3);
    assert(hIndexFromSorted({1, 1, 3}) == 1);
    assert(hIndexFromSorted({2, 2, 2}) == 2);
    assert(hIndexFromSorted({0, 0, 0, 1, 1}) == 1);
    assert(hIndexFromSorted({1, 2, 100}) == 2);
    assert(hIndexFromSorted({10, 10, 10, 10, 10}) == 5); // all papers have >= 5
    // Large values and large n
    std::vector<int> big(1000000, 1000000);
    assert(hIndexFromSorted(big) == 1000000);
    return 0;
}
