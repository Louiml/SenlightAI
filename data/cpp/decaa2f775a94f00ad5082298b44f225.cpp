Given a 1-indexed array `line` of `n` distinct positive integers (where `1 <= n <= 100000` and each value is between 1 and 1000000), write a C++ function that returns the sum over all contiguous subarrays of (maximum value in that subarray minus minimum value in that subarray). The result fits in a 64-bit signed integer. The function should take the array as a `std::vector<long long>` (where indices 0..n-1 represent the values at positions 1..n) and return a `long long`. For example, for `line = [2, 1, 3]`, the subarrays and their max-min differences are: [2]→0, [1]→0, [3]→0, [2,1]→1, [1,3]→2, [2,1,3]→2, total = 5. The solution must not use any global variables and must be self-contained in a single free function.

#include <cassert>
#include <vector>
#include "solution.h" // or just paste the function above
int main() {
    // Test 1: simple case
    std::vector<long long> arr1 = {2, 1, 3};
    assert(sumOfSubarrayMaxMinusMin(arr1) == 5);
    // Test 2: single element
    std::vector<long long> arr2 = {5};
    assert(sumOfSubarrayMaxMinusMin(arr2) == 0);
    // Test 3: increasing sequence
    std::vector<long long> arr3 = {1, 2, 3, 4};
    // Subarrays: length1:0 each; length2:1+1+1=3; length3:2+2=4; length4:3 => total 10
    assert(sumOfSubarrayMaxMinusMin(arr3) == 10);
    // Test 4: decreasing sequence
    std::vector<long long> arr4 = {4, 3, 2, 1};
    assert(sumOfSubarrayMaxMinusMin(arr4) == 10);
    // Test 5: random distinct values
    std::vector<long long> arr5 = {10, 1, 100, 2};
    // Brute force manually: all subarrays
    // [10]=0, [1]=0, [100]=0, [2]=0,
    // [10,1]=9, [1,100]=99, [100,2]=98,
    // [10,1,100]=99, [1,100,2]=99,
    // [10,1,100,2]=99 => sum = 0+0+0+0+9+99+98+99+99+99 = 503? Let's compute: 9+99=108, +98=206, +99+99=404, +99=503.
    assert(sumOfSubarrayMaxMinusMin(arr5) == 503);
    // Test 6: all values same (but distinct requirement not enforced here, but still works)
    std::vector<long long> arr6 = {7, 7, 7};
    assert(sumOfSubarrayMaxMinusMin(arr6) == 0);
    // Test 7: two elements
    std::vector<long long> arr7 = {5, 2};
    assert(sumOfSubarrayMaxMinusMin(arr7) == 3);
    // Test 8: longer random, use known sum from manual small
    std::vector<long long> arr8 = {3, 1, 2};
    // Subarrays: [3]=0, [1]=0, [2]=0, [3,1]=2, [1,2]=1, [3,1,2]=2 => sum=5
    assert(sumOfSubarrayMaxMinusMin(arr8) == 5);
    // Test 9: empty vector
    std::vector<long long> arr9;
    assert(sumOfSubarrayMaxMinusMin(arr9) == 0);
    // Test 10: two elements reversed
    std::vector<long long> arr10 = {2, 5};
    assert(sumOfSubarrayMaxMinusMin(arr10) == 3);
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Helper structure for segment tree storing max and min with their positions
struct Node {
    long long maxVal;
    int maxPos;
    long long minVal;
    int minPos;
};

// Build segment tree from array
void build(const std::vector<long long>& arr, std::vector<Node>& tree, int node, int l, int r) {
    if (l == r) {
        tree[node] = {arr[l], l, arr[l], l};
        return;
    }
    int mid = (l + r) / 2;
    build(arr, tree, node*2, l, mid);
    build(arr, tree, node*2+1, mid+1, r);
    const Node& left = tree[node*2];
    const Node& right = tree[node*2+1];
    // Combine for max
    if (left.maxVal > right.maxVal) {
        tree[node].maxVal = left.maxVal;
        tree[node].maxPos = left.maxPos;
    } else {
        tree[node].maxVal = right.maxVal;
        tree[node].maxPos = right.maxPos;
    }
    // Combine for min
    if (left.minVal < right.minVal) {
        tree[node].minVal = left.minVal;
        tree[node].minPos = left.minPos;
    } else {
        tree[node].minVal = right.minVal;
        tree[node].minPos = right.minPos;
    }
}

// Query function returns node info for range [ql, qr]
Node query(const std::vector<Node>& tree, int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tree[node];
    int mid = (l + r) / 2;
    if (qr <= mid) return query(tree, node*2, l, mid, ql, qr);
    if (ql > mid) return query(tree, node*2+1, mid+1, r, ql, qr);
    Node left = query(tree, node*2, l, mid, ql, qr);
    Node right = query(tree, node*2+1, mid+1, r, ql, qr);
    Node res;
    if (left.maxVal > right.maxVal) {
        res.maxVal = left.maxVal;
        res.maxPos = left.maxPos;
    } else {
        res.maxVal = right.maxVal;
        res.maxPos = right.maxPos;
    }
    if (left.minVal < right.minVal) {
        res.minVal = left.minVal;
        res.minPos = left.minPos;
    } else {
        res.minVal = right.minVal;
        res.minPos = right.minPos;
    }
    return res;
}

// Recursive divide and conquer to accumulate sums
void dac(const std::vector<long long>& arr, const std::vector<Node>& tree,
         int l, int r, long long& sumMax, long long& sumMin) {
    if (l > r) return;
    Node info = query(tree, 1, 0, (int)arr.size()-1, l, r);
    // Contribution of max at info.maxPos
    long long leftCount = (long long)(info.maxPos - l + 1);
    long long rightCount = (long long)(r - info.maxPos + 1);
    sumMax += info.maxVal * leftCount * rightCount;
    // Contribution of min at info.minPos
    leftCount = (long long)(info.minPos - l + 1);
    rightCount = (long long)(r - info.minPos + 1);
    sumMin += info.minVal * leftCount * rightCount;
    // Recurse on left and right parts
    dac(arr, tree, l, info.maxPos-1, sumMax, sumMin);
    dac(arr, tree, info.maxPos+1, r, sumMax, sumMin);
    // Note: For min contribution we already added, but recursion uses maxPos to split,
    // which is fine because all elements distinct and min also splits differently? Actually we need to process min separately.
    // To avoid double processing, we can just call dac twice with different split positions, but that would cause overlap.
    // Better: we need two separate recursive calls: one for max, one for min.
    // So we restructure: define two functions dacMax and dacMin.
}

// Redefine properly with separate functions
void dacMax(const std::vector<long long>& arr, const std::vector<Node>& tree,
            int l, int r, long long& sumMax) {
    if (l > r) return;
    Node info = query(tree, 1, 0, (int)arr.size()-1, l, r);
    long long leftCount = (long long)(info.maxPos - l + 1);
    long long rightCount = (long long)(r - info.maxPos + 1);
    sumMax += info.maxVal * leftCount * rightCount;
    dacMax(arr, tree, l, info.maxPos-1, sumMax);
    dacMax(arr, tree, info.maxPos+1, r, sumMax);
}

void dacMin(const std::vector<long long>& arr, const std::vector<Node>& tree,
            int l, int r, long long& sumMin) {
    if (l > r) return;
    Node info = query(tree, 1, 0, (int)arr.size()-1, l, r);
    long long leftCount = (long long)(info.minPos - l + 1);
    long long rightCount = (long long)(r - info.minPos + 1);
    sumMin += info.minVal * leftCount * rightCount;
    dacMin(arr, tree, l, info.minPos-1, sumMin);
    dacMin(arr, tree, info.minPos+1, r, sumMin);
}

// Main function: returns sum of (max - min) over all subarrays
long long sumOfSubarrayMaxMinusMin(const std::vector<long long>& arr) {
    if (arr.empty()) return 0;
    int n = (int)arr.size();
    std::vector<Node> tree(4*n);
    build(arr, tree, 1, 0, n-1);
    long long sumMax = 0;
    long long sumMin = 0;
    dacMax(arr, tree, 0, n-1, sumMax);
    dacMin(arr, tree, 0, n-1, sumMin);
    return sumMax - sumMin;
}

// The problem asks for the sum of (max - min) over all subarrays. We can compute this as `sumMax - sumMin`, where `sumMax` is the sum of maximums of all subarrays, and `sumMin` is the sum of minimums. A standard divide-and-conquer on the index range works: for each segment `[l, r]`, find the position of the maximum value in that segment (via a segment tree or RMQ structure). The contribution of that maximum to `sumMax` is its value multiplied by the number of subarrays that contain it and are entirely inside `[l, r]` but do not include any other element equal to or greater than it (since all values are distinct, it's simply the number of subarrays where it is the maximum). Since the maximum is unique, the subarrays where it is maximum are those that contain its position and do not cross outside `[l, r]`, but we must exclude subarrays that also include the boundary elements that are smaller? Actually the standard approach: the number of subarrays in `[l, r]` where the current maximum (at position `m`) is the maximum is `(m - l + 1) * (r - m + 1)`. Because any subarray that includes `m` and stays within `[l, r]` has `m` as its maximum. So we add `max * (m-l+1)*(r-m+1)` to `sumMax`, then recursively process the left part `[l, m-1]` and right part `[m+1, r]`. Similarly for the minimum: find the position of the minimum, add its contribution `min * (m-l+1)*(r-m+1)` to `sumMin`, and recurse. Since values are distinct, the maximum and minimum positions are unique. Edge case: when segment is empty (l > r), return. The algorithm efficiently uses a segment tree to query max/min and their positions in O(log n) per query. The recursion visits each position exactly once, so the total time is O(n log n) due to each query costing O(log n) and there are O(n) queries. Space complexity is O(n) for the segment tree and recursion stack. The result fits in long long because n ≤ 100000, each subarray difference ≤ 1e6, number of subarrays ~5e9, product ~5e15 which fits in 64-bit signed (max ~9e18).
