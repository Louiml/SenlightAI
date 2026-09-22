// Write a C++ function `rangePower` that takes a non-empty vector of positive integers (`arr`), and a vector of queries, where each query specifies a half-open interval `[start, end)` (using 0-based indices, with start inclusive and end exclusive) and an index to store the result. For a given interval, the "power" is defined as the sum over each distinct value `v` in the interval of `(frequency of v)^2 * v`. The function must return a vector of `long long` where the element at position `query.index` contains the power for that query. The intervals are guaranteed to be valid (0 <= start < end <= arr.size()) and the array values are positive and fit in a `long long`. The function should use Mo's algorithm to process the queries efficiently in offline manner.

// The problem is a classic Mo's algorithm application for range queries. The key idea is to sort the queries in a specific order that minimizes the total movement of left and right pointers. The block size is typically set to `sqrt(N)`, where `N` is the length of the array. Queries are sorted by the block of their left endpoint, and within the same block, by their right endpoint (increasing). This ensures the left pointer moves O(Q * sqrt(N)) times and the right pointer moves O(N * sqrt(N)) times in total.
//
// We maintain a frequency array `cnt` (size = max value in arr + 1). When adding an element `x` into the current window, the change in power is given by `( (f+1)^2 - f^2 ) * x = (2*f + 1) * x`. Similarly, when removing an element `x`, the change is `( (f-1)^2 - f^2 ) * x = -(2*f - 1) * x`. We adjust the current power value accordingly.
//
// Edge cases: The current window is initialized from the first query's range. If a query interval is empty (start == end) it is not allowed by constraints. Values can be large, so use `long long` for the power computation. The maximum value in the array might be up to 1e9, so allocating an array of that size is not feasible; instead, we could use coordinate compression. However, the snippet given uses `max_element` to size the count vector, which is only safe if the max value is small. For the general task, to make it robust, we either assume values are small (<= 1e5) or use an unordered_map for frequency. For the reference solution, we will assume values are positive and fit in `long long`, but we will use coordinate compression to map values to indices to keep the count vector manageable.
//
// Time complexity: Sorting queries takes O(Q log Q). The total movement of pointers is O((N + Q) * sqrt(N)). Each add/remove operation is O(1) after compression. Space complexity: O(N + max_value_after_compression) for the count array and the answer vector.

#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>

// Process range queries using Mo's algorithm. Each query is a half-open interval [l, r).
// The "power" of an interval is sum over each value v of (frequency(v)^2 * v).
// The result is placed in ans[query.index].
std::vector<long long> rangePower(const std::vector<long long>& arr,
                                  const std::vector<std::pair<int,int>>& queries) {
    int n = (int)arr.size();
    int q = (int)queries.size();
    
    // Coordinate compression of values
    std::vector<long long> vals = arr;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    std::unordered_map<long long, int> comp;
    for (int i = 0; i < (int)vals.size(); ++i) {
        comp[vals[i]] = i;
    }
    std::vector<int> compressed(n);
    for (int i = 0; i < n; ++i) {
        compressed[i] = comp[arr[i]];
    }
    int distinct = (int)vals.size();
    
    // Query struct for sorting
    struct Query {
        int l, r, idx;
    };
    std::vector<Query> qs(q);
    for (int i = 0; i < q; ++i) {
        qs[i] = {queries[i].first, queries[i].second, i};
    }
    
    int block = std::max(1, (int)std::sqrt(n));
    std::sort(qs.begin(), qs.end(), [&](const Query& a, const Query& b) {
        int block_a = a.l / block;
        int block_b = b.l / block;
        if (block_a != block_b) return block_a < block_b;
        // Alternate ordering for right endpoint to reduce movement (optional)
        if (block_a % 2 == 0) return a.r < b.r;
        else return a.r > b.r;
    });
    
    std::vector<long long> cnt(distinct, 0);
    std::vector<long long> ans(q, 0);
    
    long long curPower = 0;
    int curL = qs[0].l, curR = qs[0].l; // empty window initially
    
    auto add = [&](int pos) {
        int val = compressed[pos];
        long long f = cnt[val];
        cnt[val]++;
        curPower += (2 * f + 1) * arr[pos];
    };
    
    auto remove = [&](int pos) {
        int val = compressed[pos];
        long long f = cnt[val];
        cnt[val]--;
        curPower -= (2 * f - 1) * arr[pos];
    };
    
    for (const auto& query : qs) {
        while (curL > query.l) {
            curL--;
            add(curL);
        }
        while (curR < query.r) {
            add(curR);
            curR++;
        }
        while (curL < query.l) {
            remove(curL);
            curL++;
        }
        while (curR > query.r) {
            curR--;
            remove(curR);
        }
        ans[query.idx] = curPower;
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is already included above.
// Test main function with several cases.

int main() {
    // Basic case
    std::vector<long long> arr1 = {1, 2, 1, 3, 2};
    std::vector<std::pair<int,int>> q1 = {{0, 5}, {1, 3}, {0, 2}, {2, 4}};
    std::vector<long long> ans1 = rangePower(arr1, q1);
    // For [0,5): values 1(freq2),2(freq2),3(freq1) => (2^2*1)+(2^2*2)+(1^2*3)=4+8+3=15
    // For [1,3): {2,1} => each freq1 => 2+1=3
    // For [0,2): {1,2} => 1+2=3
    // For [2,4): {1,3} => 1+3=4
    assert(ans1[0] == 15);
    assert(ans1[1] == 3);
    assert(ans1[2] == 3);
    assert(ans1[3] == 4);

    // Single element array
    std::vector<long long> arr2 = {7};
    std::vector<std::pair<int,int>> q2 = {{0, 1}};
    std::vector<long long> ans2 = rangePower(arr2, q2);
    assert(ans2[0] == 7);

    // All same values
    std::vector<long long> arr3 = {5, 5, 5};
    std::vector<std::pair<int,int>> q3 = {{0, 3}, {0, 1}, {1, 2}};
    std::vector<long long> ans3 = rangePower(arr3, q3);
    // [0,3): freq3 => 9*5=45
    // [0,1): 5
    // [1,2): 5
    assert(ans3[0] == 45);
    assert(ans3[1] == 5);
    assert(ans3[2] == 5);

    // Larger distinct values
    std::vector<long long> arr4 = {100, 1, 100, 1, 100};
    std::vector<std::pair<int,int>> q4 = {{0, 5}, {0, 3}, {2, 5}};
    std::vector<long long> ans4 = rangePower(arr4, q4);
    // [0,5): 100 freq3 => 9*100=900, 1 freq2 => 4*1=4 => total 904
    // [0,3): 100 freq2 => 4*100=400, 1 freq1 => 1 => total 401
    // [2,5): 100 freq2 => 400, 1 freq1 => 1 => total 401
    assert(ans4[0] == 904);
    assert(ans4[1] == 401);
    assert(ans4[2] == 401);

    // Queries out of order and overlapping
    std::vector<long long> arr5 = {2, 3, 2, 4, 2, 3};
    std::vector<std::pair<int,int>> q5 = {{4, 6}, {1, 5}, {0, 2}, {2, 4}, {0, 6}};
    std::vector<long long> ans5 = rangePower(arr5, q5);
    // [4,6): {2,3} => 2+3=5
    // [1,5): {3,2,4,2} => 3+2^2*2? Wait: 3 freq1=>3, 2 freq2=>4*2=8, 4 freq1=>4 => total 3+8+4=15
    // [0,2): {2} => 2
    // [2,4): {2,4} => 2+4=6
    // [0,6): 2 freq3=>9*2=18, 3 freq2=>4*3=12, 4 freq1=>4 => total 34
    assert(ans5[0] == 5);
    assert(ans5[1] == 15);
    assert(ans5[2] == 2);
    assert(ans5[3] == 6);
    assert(ans5[4] == 34);

    return 0;
}
