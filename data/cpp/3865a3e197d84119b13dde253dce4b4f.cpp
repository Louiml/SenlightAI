/*
Given a sequence of integers `a[0..n-1]`, process the elements in order and for each prefix, output the minimum over all indices `j` in `[0, i]` of `(a[j] + j)` where the values are adjusted after a dynamic threshold shift. More precisely, maintain a counter `dec` that starts at 0. After reading each element `x = a[i]`, if `x + dec <= 0`, increment a counter `sum` and if `sum - (count of elements in the current prefix equal to -dec) > dec`, then subtract that count from `sum` and increment `dec`. After this update, output the minimum value of `(a[j] + (number of elements in positions > j that are <= -dec_at_that_time))` over all `j` in `[0, i]`, where the adjustment is based on the current `dec`. Implement a function `solve(const std::vector<int>& a)` that returns a vector of integers, each being the output for the corresponding prefix. The sequence length `n` satisfies `1 ≤ n ≤ 10^5`, and each `a[i]` satisfies `-10^9 ≤ a[i] ≤ 10^9`. The function must run efficiently within time and memory limits.
*/
#include <vector>
#include <algorithm>
#include <cstdint>

using i64 = long long;

constexpr int V = 1 << 18;
constexpr int INF = 1e9;

// Segment tree global arrays
int cnt_[4 * (2 * V)] = {};
int mn[4 * (2 * V)];
int c[4 * (2 * V)];

void build(int p, int l, int r) {
    mn[p] = l - V;
    if (r - l == 1) return;
    int m = (l + r) / 2;
    build(2 * p, l, m);
    build(2 * p + 1, m, r);
}

void modify(int p, int l, int r, int x) {
    c[p]++;
    if (r - l == 1) return;
    int m = (l + r) / 2;
    if (x < m) modify(2 * p, l, m, x);
    else modify(2 * p + 1, m, r, x);
    mn[p] = std::min(mn[2 * p] + c[2 * p + 1], mn[2 * p + 1]);
}

int query(int p, int l, int r, int x) {
    if (r <= x) return INF;
    if (l >= x) return mn[p];
    int m = (l + r) / 2;
    return std::min(query(2 * p, l, m, x) + c[2 * p + 1],
                    query(2 * p + 1, m, r, x));
}

std::vector<long long> processPrefixMins(const std::vector<int>& a) {
    build(1, 0, 2 * V);
    std::vector<int> freq(2 * V, 0);
    int dec = 0, sum = 0;
    std::vector<long long> ans;
    ans.reserve(a.size());
    for (int x : a) {
        freq[x + V]++;
        if (x <= -dec) {
            sum++;
            if (sum - freq[-dec + V] > dec) {
                sum -= freq[-dec + V];
                dec++;
            }
        }
        modify(1, 0, 2 * V, x + V);
        int res = query(1, 0, 2 * V, -dec + V);
        ans.push_back(res);
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Assume the function processPrefixMins is defined above.

int main() {
    assert(processPrefixMins({1, -2}) == std::vector<long long>({1, -1}));
    assert(processPrefixMins({-1, -1}) == std::vector<long long>({-1, -1}));
    assert(processPrefixMins({0, -1}) == std::vector<long long>({0, 0}));
    assert(processPrefixMins({5, -100}) == std::vector<long long>({5, -99}));
    // Additional test with a single element
    assert(processPrefixMins({-42}) == std::vector<long long>({-42}));
    // Test with all large positive values (dec never changes)
    assert(processPrefixMins({100000, 99999}) == std::vector<long long>({100000, 99999}));
    return 0;
}
// The problem can be solved efficiently with a segment tree over the value range `[-V, V-1]` where `V = 1 << 18 = 262144`. Each leaf at index `i` represents the integer value `i - V`. For each node, we store two values: `cnt` – the number of inserted elements falling in that node's range, and `mn` – the minimum of `v + number_of_inserted_elements_greater_than_v` over all leaves `v` in that node's subtree. When the tree is empty, `mn` for a leaf is just its value, and for an internal node it is the smallest value in its range. Inserting an element increments `cnt` along the path to the leaf and recomputes `mn` bottom-up using `mn[p] = min(mn[left] + cnt[right], mn[right])`. This works because for any leaf in the left child, all elements in the right child are strictly greater, and their count is added; the right child's `mn` already accounts for elements greater than its own leaves. To query the minimum over a suffix `[x, 2V)`, we recursively split: if the node lies completely in the suffix, return its `mn`; otherwise, combine the left child's query (adding the full count of the right child, which is within the suffix) with the right child's query. Each insertion and query takes `O(log V)` time. The `dec` and `sum` updates require a frequency array for the exact value `-dec`, giving `O(1)` per step. Total time is `O(n log V)` and space is `O(V)` with `V=262144`. Edge cases include duplicate values (handled by counting frequencies) and missing values in the query suffix (returns large `INF`).
