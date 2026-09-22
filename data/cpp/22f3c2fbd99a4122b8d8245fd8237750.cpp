// Write a C++ function `std::pair<int,int> peakCounts(const std::vector<int>& heights)` that takes a non-empty vector of integers representing heights of consecutive peaks (or visible points from left to right). The function must return a pair `{leftVisible, rightVisible}` where `leftVisible` is the number of peaks visible when viewing the sequence from the left end (i.e., a peak is visible if it is strictly greater than every peak to its left), and `rightVisible` is the number of peaks visible when viewing the sequence from the right end (i.e., a peak is visible if it is strictly greater than every peak to its right). For example, for `heights = {1, 3, 2, 5}`, left visible peaks are 1, 3, 5 (since 3>1, 2 not >3, 5>3) so leftVisible=3; right visible peaks are 5, 2, 1 (since 2 not >5? Actually from right view: start with rightmost 5, then move left: 2 is not >5, 3 is not >5, 1 is not >5 — so only 5 is visible from right? Wait careful: the original snippet counts from right by tracking the maximum from the rightmost end. Let’s re-read the snippet: it first sets ff=a[n] (rightmost), then loops i=n-1 down to 1, counting those a[i] > ff and updating ff. So visible from right means starting at the rightmost peak, then moving left, each peak that is greater than all peaks to its right is visible. So for {1,3,2,5}, rightmost 5 is visible, then moving left: 2 (not >5), 3 (not >5), 1 (not >5) — only 1 peak visible from right. So leftVisible=3, rightVisible=1. The function must return `{3,1}`. Duplicates never count as strictly greater. The input vector has at least one element. Return a `std::pair<int,int>` where first is leftVisible and second is rightVisible.
// The problem is exactly what the given snippet does, but cleaned up: we iterate from left to right, maintaining the current maximum seen so far. Initially, the first element is always visible (since there’s nothing to its left), so we set `leftCount=1` and `currentMax=heights[0]`. Then for each subsequent element, if it is strictly greater than `currentMax`, we increment `leftCount` and update `currentMax` to that value. Similarly, for the right view, we iterate from right to left, setting `rightCount=1` with `currentMax=heights.back()`, then for each element going leftward, if it is strictly greater than `currentMax`, increment `rightCount` and update `currentMax`. This works because we are counting "record highs" from each end. Edge cases: vector of size 1 returns {1,1}; all equal values return {1,1} because only the first/last are visible (since duplicates are not strictly greater). Negative numbers are fine. Time complexity is O(n) with two passes, space O(1) besides the input vector.
#include <vector>
#include <utility>

// Count visible peaks from left and right ends.
// Returns {leftVisible, rightVisible} where a peak is visible if it is strictly
// greater than all peaks before it in that viewing direction.
std::pair<int, int> peakCounts(const std::vector<int>& heights) {
    if (heights.empty()) {
        return {0, 0};
    }

    int leftCount = 1;
    int leftMax = heights.front();
    for (size_t i = 1; i < heights.size(); ++i) {
        if (heights[i] > leftMax) {
            ++leftCount;
            leftMax = heights[i];
        }
    }

    int rightCount = 1;
    int rightMax = heights.back();
    for (size_t i = heights.size() - 1; i-- > 0; ) {
        if (heights[i] > rightMax) {
            ++rightCount;
            rightMax = heights[i];
        }
    }

    return {leftCount, rightCount};
}
#include <cassert>
#include <vector>
#include <utility>

// Assume peakCounts is already defined above
int main() {
    std::vector<int> v1 = {1, 3, 2, 5};
    assert(peakCounts(v1) == std::pair<int,int>(3, 1));

    std::vector<int> v2 = {5};
    assert(peakCounts(v2) == std::pair<int,int>(1, 1));

    std::vector<int> v3 = {3, 3, 3};
    assert(peakCounts(v3) == std::pair<int,int>(1, 1));

    std::vector<int> v4 = {7, 7, 8, 7, 9};
    // left: 7,8,9 => 3; right: 9,7,7? Actually right: start 9, then 7 not >9, 8 not >9, 7 not >9 => only 1? Wait careful: rightmost is 9, then going left: 7 (not >9), 8 (not >9), 7 (not >9) => right=1. So {3,1}
    assert(peakCounts(v4) == std::pair<int,int>(3, 1));

    std::vector<int> v5 = {10, 9, 8, 7};
    // left: only 10 =1; right: 7,8,9,10 =4
    assert(peakCounts(v5) == std::pair<int,int>(1, 4));

    std::vector<int> v6 = {1, 2, 3, 4, 5};
    assert(peakCounts(v6) == std::pair<int,int>(5, 1));

    std::vector<int> v7 = {5, 4, 3, 2, 1};
    assert(peakCounts(v7) == std::pair<int,int>(1, 5));

    std::vector<int> v8 = {2, 1, 2, 1, 2};
    // left: 2,2? Actually first 2 visible, then 1 not, then 2 >2? No, strict greater, so not; so left=1. Right: rightmost 2, then 1 not, then 2 not >2, so right=1. {1,1}
    assert(peakCounts(v8) == std::pair<int,int>(1, 1));

    std::vector<int> v9 = {-1, -2, -3};
    assert(peakCounts(v9) == std::pair<int,int>(1, 3));

    std::vector<int> v10 = {0, 0, 1, 0};
    assert(peakCounts(v10) == std::pair<int,int>(2, 1));
    return 0;
}
