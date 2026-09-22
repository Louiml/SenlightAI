/*
Given a circular array of positive integers and an integer n (the length of the original array before triplication), write a C++ function `std::vector<int> nearestGreaterWithHalf(const std::vector<int>& arr)` that, for each element in the first n positions of the circular array, finds the distance (number of steps to the right, i.e., positive index difference modulo the circular length) to the nearest position j such that arr[j] > 2 * arr[i] (strictly more than double). If no such position exists in the entire circular array, return -1 for that index. The input `arr` is already tripled (length = 3*n) representing the circular repetition of the original array of length n. The function should return a vector of length n containing the distances (or -1). The circular distance is measured as the positive number of steps from i to j moving to the right (in increasing index, wrapping around), i.e., (j - i) if j >= i, else (j - i + 3n), but we only care about the minimal such distance (the first occurrence to the right). Note: The array has positive integers, n can be up to 200,000, and the tripled length is 600,000. The solution must run in O(n log n) time or better with O(n) auxiliary space.
*/
#include <bits/stdc++.h>
using namespace std;

class SegmentTree {
    int n;
    vector<int> tree; // stores minimum index, initialized to INF
public:
    SegmentTree(int size) : n(size), tree(4 * size, INT_MAX) {}
    void update(int node, int l, int r, int pos, int val) {
        if (l == r) {
            tree[node] = min(tree[node], val);
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(node * 2, l, mid, pos, val);
        else update(node * 2 + 1, mid + 1, r, pos, val);
        tree[node] = min(tree[node * 2], tree[node * 2 + 1]);
    }
    int query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l || ql > qr) return INT_MAX;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return min(query(node * 2, l, mid, ql, qr), query(node * 2 + 1, mid + 1, r, ql, qr));
    }
    void update(int pos, int val) { update(1, 0, n - 1, pos, val); }
    int query(int l, int r) { return query(1, 0, n - 1, l, r); }
};

std::vector<int> nearestGreaterWithHalf(const std::vector<int>& arr) {
    int len = arr.size(); // len = 3*n
    int n = len / 3;
    // compress values
    vector<long long> vals(arr.begin(), arr.end());
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int m = vals.size();
    SegmentTree seg(m);

    vector<int> ans(n, -1);
    for (int i = len - 1; i >= 0; --i) {
        long long threshold = 2LL * arr[i];
        // find first value > threshold using upper_bound
        auto it = upper_bound(vals.begin(), vals.end(), threshold);
        if (it != vals.end()) {
            int idx = it - vals.begin();
            int minIdx = seg.query(idx, m - 1);
            if (minIdx != INT_MAX) {
                if (i < n) ans[i] = minIdx - i;
            }
        }
        // insert current position
        int comp = lower_bound(vals.begin(), vals.end(), arr[i]) - vals.begin();
        seg.update(comp, i);
    }
    return ans;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// include the solution function here (copy from above)

int main() {
    // Test 1: n=3, circular repeated once manually tripled
    vector<int> arr1 = {2, 4, 6, 2, 4, 6, 2, 4, 6}; // n=3, original [2,4,6]
    vector<int> res1 = nearestGreaterWithHalf(arr1);
    // For i=0 (val 2): need >4 -> find 6 at index 2 (distance 2) or at index5 (distance5) first is 2
    // For i=1 (val 4): need >8 -> none (max 6) -> -1
    // For i=2 (val 6): need >12 -> none -> -1
    assert(res1.size() == 3);
    assert(res1[0] == 2);
    assert(res1[1] == -1);
    assert(res1[2] == -1);

    // Test 2: all same values, no greater than double
    vector<int> arr2 = {5,5,5,5,5,5}; // n=2
    vector<int> res2 = nearestGreaterWithHalf(arr2);
    assert(res2.size() == 2);
    assert(res2[0] == -1);
    assert(res2[1] == -1);

    // Test 3: strictly increasing tripled, each has next as double or more? 
    vector<int> arr3 = {1,3,9,1,3,9,1,3,9}; // n=3, original [1,3,9]
    // i=0 (1): need >2 -> first 3 at index1 (dist1)
    // i=1 (3): need >6 -> first 9 at index2 (dist1)
    // i=2 (9): need >18 -> none -> -1
    vector<int> res3 = nearestGreaterWithHalf(arr3);
    assert(res3[0] == 1);
    assert(res3[1] == 1);
    assert(res3[2] == -1);

    // Test 4: wrap around case: original [10,1] n=2 tripled [10,1,10,1,10,1]
    vector<int> arr4 = {10,1,10,1,10,1};
    vector<int> res4 = nearestGreaterWithHalf(arr4);
    // i=0 (10): need >20 none -> -1
    // i=1 (1): need >2 -> first 10 at index2 (dist1) because index1+1=2
    assert(res4[0] == -1);
    assert(res4[1] == 1);

    // Test 5: large array with edges, ensure correct for n=1
    vector<int> arr5 = {100, 100, 100}; // n=1, original [100]
    vector<int> res5 = nearestGreaterWithHalf(arr5);
    assert(res5.size() == 1);
    assert(res5[0] == -1);

    // Test 6: handler where threshold exactly matches but need strictly greater
    vector<int> arr6 = {2,4,2,4,2,4}; // n=2 original [2,4]
    // i=0 (2): need >4 -> strict, so 4 not enough, next >4? none (max4) -> -1
    // i=1 (4): need >8 -> none -> -1
    vector<int> res6 = nearestGreaterWithHalf(arr6);
    assert(res6[0] == -1);
    assert(res6[1] == -1);

    // Test 7: mixed case with wrap distance longer than n
    vector<int> arr7 = {1,2,3, 1,2,3, 1,2,3}; // n=3 original [1,2,3]
    // i=0 (1): need >2 -> first 3 at index2 (dist2)
    // i=1 (2): need >4 -> none -> -1
    // i=2 (3): need >6 -> none -> -1
    vector<int> res7 = nearestGreaterWithHalf(arr7);
    assert(res7[0] == 2);
    assert(res7[1] == -1);
    assert(res7[2] == -1);

    // Test 8: check that results are int values, no overflow
    vector<int> arr8 = {1, 5, 1, 5, 1, 5}; // n=2 original [1,5]
    vector<int> res8 = nearestGreaterWithHalf(arr8);
    // i=0 (1): need >2 -> first 5 at index1 (dist1)
    // i=1 (5): need >10 -> none
    assert(res8[0] == 1);
    assert(res8[1] == -1);

    // Test 9: edge with value 0? The problem says positive, but test with 1
    vector<int> arr9 = {1,1,1,1,1,1}; // n=2
    vector<int> res9 = nearestGreaterWithHalf(arr9);
    assert(res9[0] == -1 && res9[1] == -1);

    // Test 10: huge single element test
    vector<int> arr10 = {2000000, 2000000, 2000000}; // n=1
    vector<int> res10 = nearestGreaterWithHalf(arr10);
    assert(res10[0] == -1);

    cout << "All tests passed!" << endl;
    return 0;
}
// The problem requires for each index i (0-based, within 0..n-1) to find the minimum positive offset d such that arr[i + d] > 2*arr[i] (with wrapping over the tripled array). Since the array is tripled, any element to the right within the circular sense is represented within the range i+1 to i+3n (exclusive at 3n) because the tripled array covers exactly two full circles plus one extra to handle wrapping. We need to process all n positions efficiently. A key observation: we can precompute for each position its "next greater than double" using a monotonic stack (or binary lifting) because we only care about elements that are strictly greater than twice the current value. However, the condition is not monotonic in a simple stack sense because the threshold depends on the current value. Instead, we can use a different approach: For each value x, we need the first position to the right where the value > 2x. We can process from right to left and maintain a data structure that supports querying the minimum index among values > threshold. Since values are positive, we can use a segment tree or Fenwick tree over compressed values, storing for each value the minimum index seen so far to the right. For each position i from right to left, query for minimum index among values in range (2*arr[i]+1, INF), then set the answer as that minimum index minus i (or -1 if none). Then update the segment tree at position arr[i] to store index i (taking the minimum because we want the closest to the right). Because we process from right to left, when processing i, all positions > i are already inserted, so the minimum index found is exactly the first occurrence to the right within the tripled array. However, we must also consider wrapping: since the array is tripled, we process all 3n positions, but only output answers for the first n. The query returns the absolute index in the tripled array; the distance is simply that index - i, which will always be positive because we process right to left and only have later indices. But careful: because we insert indices as we go from right to left, the minimum index among values > threshold will be the smallest index greater than i (since all inserted are > i). That gives the first to the right. We need to handle the fact that arr is up to 3n length, and we only output for first n. For indices beyond n (i.e., > n), we also process them to maintain the structure for earlier ones. The time complexity: O(3n log(3n)) due to segment tree operations, with compression of values. Space O(3n). Edge cases: If no value > 2*arr[i] exists, answer -1. Since arr values are positive, 2*arr[i] could exceed max value; handle by query range that may be empty. Also note that 2*arr[i] might overflow int? arr[i] up to 1e9? The snippet uses int, but we can use long long safely. We'll assume arr fits in int, but use long long for threshold.
