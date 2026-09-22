// Given a sorted array of integers `citations` in ascending order, where each element represents the number of citations a researcher's paper has received, write a C++ function `int hIndex(const std::vector<int>& citations)` that computes the researcher's h-index. The h-index is defined as the maximum value `h` such that the researcher has at least `h` papers with at least `h` citations each. The input array is guaranteed to be non-empty and sorted in non-decreasing order. Your function must handle edge cases such as all zeros, all very large values, and arrays with a single element, and must run in `O(log n)` time or better.
// The h-index can be found using binary search because the array is sorted. For any index `i` (0-based), the number of papers with at least `citations[i]` citations is `n - i` (since everything from `i` to the end has at least that value). The condition for a candidate `h` is that there exists some index where `n - i <= citations[i]` and we want the smallest such index, because then `h = n - i` will be the maximum possible. The binary search maintains `lo = 0` and `hi = n` (exclusive upper bound). At each step, compute `mid = (lo + hi) / 2`. If `citations[mid] >= n - mid`, then a valid `h` exists at this position or earlier, so we search the left half (`hi = mid`). Otherwise, we need a smaller `mid` to satisfy the condition, so we move `lo = mid + 1`. After the loop, `lo` is the first index where the condition holds, and the answer is `n - lo`. Edge cases: if all citations are zero, `lo` becomes `n` and `h=0`; if all citations are large enough, `lo` becomes `0` and `h=n`. Time complexity is `O(log n)` and space complexity is `O(1)`.
#include <vector>

// Compute the h-index from a sorted ascending vector of citations.
// The h-index is the largest h such that at least h papers have >= h citations.
int hIndex(const std::vector<int>& citations) {
    int n = static_cast<int>(citations.size());
    int lo = 0, hi = n;  // hi is exclusive
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (citations[mid] >= n - mid) {
            hi = mid;  // possible h at mid or earlier
        } else {
            lo = mid + 1;  // need a smaller mid
        }
    }
    return n - lo;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(hIndex({0, 1, 3, 5, 6}) == 3);
    assert(hIndex({1, 1, 3}) == 1);
    assert(hIndex({0, 0, 0}) == 0);
    assert(hIndex({5, 5, 5}) == 3);
    assert(hIndex({1}) == 1);
    assert(hIndex({0}) == 0);
    assert(hIndex({100}) == 1);
    // Edge: a large array
    std::vector<int> large(1000, 500);
    assert(hIndex(large) == 500);
    // Edge: strictly increasing
    assert(hIndex({1, 2, 3, 4, 5}) == 3);
    assert(hIndex({2, 3, 4, 5, 6}) == 4);
    return 0;
}
