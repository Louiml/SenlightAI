Given a vector of positive integers `nums` and a positive integer `threshold`, write a C++ function `int validSubarraySize(vector<int>& nums, int threshold)` that returns the smallest length `k` (1 ≤ k ≤ n) such that there exists a contiguous subarray of length exactly `k` where every element in that subarray is strictly greater than `threshold / k`. If no such subarray exists for any `k`, return `-1`. The function must handle up to 10^5 elements efficiently.
#include <cassert>
#include <vector>

int main() {
    // Test from the original problem example
    std::vector<int> nums1 = {1, 3, 4, 3, 1};
    assert(validSubarraySize(nums1, 6) == 3);

    // No valid subarray
    std::vector<int> nums2 = {1, 1, 1};
    assert(validSubarraySize(nums2, 10) == -1);

    // Single element qualifies
    std::vector<int> nums3 = {10};
    assert(validSubarraySize(nums3, 5) == 1);

    // All elements identical and just above threshold
    std::vector<int> nums4 = {2, 2, 2};
    assert(validSubarraySize(nums4, 4) == 3);

    // Test with larger values
    std::vector<int> nums5 = {100, 200, 300};
    assert(validSubarraySize(nums5, 50) == 1);

    // Test where only long subarray works
    std::vector<int> nums6 = {1, 2, 3, 4, 5};
    assert(validSubarraySize(nums6, 4) == 2); // because for k=2, req=2, elements >2 are 3,4,5 -> subarray [3,4] length 2 exists

    // Test with zero threshold
    std::vector<int> nums7 = {1, 2, 3};
    assert(validSubarraySize(nums7, 1) == 1); // req=1 for k=1, all >1? Actually 1 is not >1, so need k=2? Let's check carefully: for k=1, req=1, elements >1 are 2,3 -> subarray [2] exists length 1, yes.

    // Edge case all large numbers
    std::vector<int> nums8 = {1000000, 1000000, 1000000, 1000000};
    assert(validSubarraySize(nums8, 1000000) == 1);

    // Test with duplicate boundary
    std::vector<int> nums9 = {5, 5, 5, 5};
    assert(validSubarraySize(nums9, 20) == -1); // for k=4, req=5, need >5, none.

    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>

class DisjointSet {
public:
    DisjointSet(int n) : N(n), sz(n), pars(n, -1) {}
    void reset() { sz = N; std::fill(pars.begin(), pars.end(), -1); }
    int find(int x) { return pars[x] < 0 ? x : pars[x] = find(pars[x]); }
    int size() const { return sz; }
    int count(int x) { return -pars[find(x)]; }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        --sz;
        if (pars[x] < pars[y]) std::swap(x, y);
        pars[y] += pars[x];
        pars[x] = y;
        return true;
    }
private:
    int N, sz;
    std::vector<int> pars;
};

// Returns the smallest k such that some contiguous subarray of length k
// has all elements > threshold / k, or -1 if no such k exists.
int validSubarraySize(std::vector<int>& nums, int threshold) {
    int n = (int)nums.size();
    std::priority_queue<std::pair<int, int>> pq; // max-heap by value, then index
    for (int i = 0; i < n; ++i) {
        pq.push({nums[i], i});
    }
    DisjointSet ds(n);
    std::vector<bool> marked(n, false);
    for (int k = 1; k <= n; ++k) {
        long long req = threshold / k; // using long long to avoid overflow
        while (!pq.empty() && pq.top().first > req) {
            int idx = pq.top().second;
            pq.pop();
            marked[idx] = true;
            if (idx > 0 && marked[idx - 1]) {
                ds.unite(idx, idx - 1);
            }
            if (idx + 1 < n && marked[idx + 1]) {
                ds.unite(idx, idx + 1);
            }
            if (ds.count(idx) >= k) {
                return k;
            }
        }
    }
    return -1;
}
// The key observation is that for a fixed length `k`, the condition for an element `v` to be part of a valid subarray is `v > threshold / k`. As `k` increases, the required minimum value decreases. Therefore, we process lengths `k` from 1 to n in increasing order. For each `k`, we need to check whether there exists a contiguous block of at least `k` marked elements, where marked means `v > threshold / k`. Since the threshold decreases as `k` increases, more elements become newly eligible at each step. We maintain a max-heap of `(value, index)` to pop indices whose value is now large enough. For each newly popped index, we mark it and union it with marked neighbors using a disjoint set union (DSU). After each union, if the size of the component containing the newly added index reaches at least `k`, we immediately return `k`. This works because any length-`k` valid subarray implies the existence of a run of `k` consecutive marked elements; the DSU tracks the sizes of contiguous marked segments. Time complexity is O(n log n) for the heap and near O(n α(n)) for DSU, resulting in O(n log n) overall. Space complexity is O(n).
