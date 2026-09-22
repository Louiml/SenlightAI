// You are given an array of `n` integers, each between `0` and `100000`. You must answer `q` range queries, each defined by a half-open interval `[l, r)` (0-indexed). For each query, consider the multiset of values that appear in that subarray with their multiplicities. You need to compute the minimum possible total cost to merge all those occurrences into a single group, where merging two groups of sizes `a` and `b` costs `a+b`. This is exactly the cost of building an optimal Huffman tree (also known as the optimal merge pattern). More simply, the answer is the minimum total cost to combine all the frequencies of distinct values in the range using a Huffman-like merging process. If the range is empty, the answer is 0. If the range contains only one occurrence (one group), the answer is 0 (no merge needed). Write a standalone C++ function `long long rangeMergeCost(const std::vector<int>& arr, const std::vector<std::pair<int,int>>& queries)` that takes the initial array and a list of queries (each pair is `(l, r)`) and returns a vector of answers in the same order as the queries.
// The key is to notice that the problem asks for the Huffman coding cost of the frequency distribution of values in a subarray. For each query, we need the multiset of frequencies of each distinct value in that range, then compute the Huffman tree cost. Doing this naively for each query is too slow if `n` and `q` are large (up to 100,000). We apply a common trick: **heavy-light decomposition on values** plus **Mo's algorithm**.  
// - Choose a threshold `S` (e.g., 1300). A value is "heavy" if its total occurrence in the whole array is at least `S`. There can be at most `n/S` heavy values.  
// - For any range, the frequency of a light value is at most `S-1`, and we can maintain counts of exactly how many values have each frequency from 1 to `S-1` using an array `cntFreq`. For heavy values, we maintain their exact current frequency in the range in a separate array.  
// - Use Mo's algorithm to reorder queries so that moving the range endpoints costs `O(n sqrt n)` total changes. Each add/remove operation updates either `cntFreq` (for light values) or a heavy value's count.  
// - To compute the Huffman cost for a current range, we need to merge the frequency multiset: we have `cntFreq[1..S-1]` counts plus the heavy values' frequencies (which can be large). The Huffman algorithm can be computed in `O(S log S + H log H)` per query by simulating the process using a priority queue, but we can do better: since light frequencies are small, we can use the standard technique of combining small frequencies first, then handling the remaining large ones. A simple efficient implementation: use a min-heap (priority queue) containing all nonzero frequencies, then pop two smallest, add their sum, push back, and accumulate cost. Since the number of distinct frequencies in the range is at most `S + H` (H ≤ n/S), and S is about sqrt(n), each query's heap construction takes `O(S + H)` time and Huffman simulation `O((S+H) log (S+H))`. With S≈1300 and n≤1e5, this is acceptable for typical limits.  
// - Edge cases: empty range gives 0; single occurrence gives 0; frequencies of 0 are ignored.  
// - Time complexity: Mo's algorithm moves are `O((n+q) sqrt n)`, each add/remove is O(1). Each query's Huffman computation is `O(S log S)`, total `O(q S log S)`. With S chosen sqrt(n), this is `O((n+q) sqrt n log n)`. Space is `O(n + S)`.
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdint>

// Returns the minimum total merge cost for each query [l, r) on the array arr.
// The cost is computed as Huffman coding cost of the multiset of frequencies.
std::vector<long long> rangeMergeCost(const std::vector<int>& arr,
                                      const std::vector<std::pair<int,int>>& queries) {
    int n = (int)arr.size();
    int q = (int)queries.size();
    const int MAXV = 100001; // values are 0..100000
    const int S = 1300;      // threshold for heavy values

    // Precompute total frequency of each value
    std::vector<int> totalFreq(MAXV, 0);
    for (int v : arr) totalFreq[v]++;

    std::vector<bool> isHeavy(MAXV, false);
    std::vector<int> heavyValues;
    for (int v = 0; v < MAXV; ++v) {
        if (totalFreq[v] >= S) {
            isHeavy[v] = true;
            heavyValues.push_back(v);
        }
    }

    // Mo's ordering of queries
    std::vector<int> order(q);
    for (int i = 0; i < q; ++i) order[i] = i;
    auto mo_block = [&](int i) { return queries[i].first / S; };
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        int ba = mo_block(a), bb = mo_block(b);
        if (ba != bb) return ba < bb;
        // alternate direction for second key to reduce moves
        if (ba % 2 == 0) return queries[a].second < queries[b].second;
        else return queries[a].second > queries[b].second;
    });

    // Current range [curL, curR)
    int curL = 0, curR = 0;

    // cntFreq[i] = how many light values currently have frequency i (i from 1 to S-1)
    std::vector<int> cntFreq(S, 0); // index 0 unused
    // curFreq[v] = current frequency in the range for heavy values
    std::vector<int> curFreqHeavy(MAXV, 0);

    // Helper lambdas for adding/removing an element at position pos
    auto add_pos = [&](int pos) {
        int val = arr[pos];
        if (isHeavy[val]) {
            curFreqHeavy[val]++;
        } else {
            int old = 0;
            // find current frequency of this light value in range
            // We maintain a separate array freqLight for current frequencies
            // But we can recompute? Better maintain a per-position? 
            // To keep O(1), we need a map from value to its current frequency.
            // Since total occurrences of light value is < S, we can store in a vector<int> curFreqLight(MAXV,0)
            // Let's do that below.
        }
    };

    // Since we need O(1) per add/remove, we maintain curFreqLight array as well.
    std::vector<int> curFreqAll(MAXV, 0); // current frequency of any value in range

    auto add_pos = [&](int pos) {
        int val = arr[pos];
        int old = curFreqAll[val];
        curFreqAll[val]++;
        if (isHeavy[val]) {
            curFreqHeavy[val]++;
        } else {
            if (old > 0) cntFreq[old]--;
            cntFreq[old+1]++;
        }
    };
    auto remove_pos = [&](int pos) {
        int val = arr[pos];
        int old = curFreqAll[val];
        curFreqAll[val]--;
        if (isHeavy[val]) {
            curFreqHeavy[val]--;
        } else {
            cntFreq[old]--;
            if (old-1 > 0) cntFreq[old-1]++;
        }
    };

    // Function to compute Huffman cost for current range
    auto compute_cost = [&]() -> long long {
        // Build multiset of frequencies
        std::priority_queue<long long, std::vector<long long>, std::greater<long long>> pq;
        // Add light frequencies
        for (int f = 1; f < S; ++f) {
            int cnt = cntFreq[f];
            for (int i = 0; i < cnt; ++i) {
                pq.push(f);
            }
        }
        // Add heavy frequencies
        for (int hv : heavyValues) {
            int f = curFreqHeavy[hv];
            if (f > 0) pq.push(f);
        }
        long long cost = 0;
        while (pq.size() > 1) {
            long long a = pq.top(); pq.pop();
            long long b = pq.top(); pq.pop();
            long long sum = a + b;
            cost += sum;
            pq.push(sum);
        }
        return cost;
    };

    std::vector<long long> res(q);
    for (int qi : order) {
        int l = queries[qi].first, r = queries[qi].second;
        while (curR < r) add_pos(curR++);
        while (curR > r) remove_pos(--curR);
        while (curL < l) remove_pos(curL++);
        while (curL > l) add_pos(--curL);
        res[qi] = compute_cost();
    }

    return res;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function declaration here (or paste the code above)
// For brevity, we assume rangeMergeCost is defined above.

int main() {
    // Test 1: Single element array, one query
    {
        std::vector<int> arr = {5};
        std::vector<std::pair<int,int>> queries = {{0,1}};
        auto res = rangeMergeCost(arr, queries);
        assert(res.size() == 1 && res[0] == 0); // no merge needed
    }

    // Test 2: All same value, three occurrences -> merge cost 1+2=3? Actually frequencies: {3} -> cost 0? Wait: frequency of value =3, one group -> cost 0.
    {
        std::vector<int> arr = {2,2,2};
        std::vector<std::pair<int,int>> queries = {{0,3}};
        auto res = rangeMergeCost(arr, queries);
        // Frequency multiset: {3} -> no merge -> 0
        assert(res[0] == 0);
    }

    // Test 3: Two distinct values each once -> frequencies {1,1} -> merge cost 1+1=2
    {
        std::vector<int> arr = {1,2,1,2};
        std::vector<std::pair<int,int>> queries = {{0,4}};
        auto res = rangeMergeCost(arr, queries);
        // frequencies: value1 appears 2, value2 appears 2 -> multiset {2,2} -> merge cost 2+2=4, then {4} done -> total 4
        assert(res[0] == 4);
    }

    // Test 4: Query range [1,3) from [1,2,3,3]
    {
        std::vector<int> arr = {1,2,3,3};
        std::vector<std::pair<int,int>> queries = {{1,3}};
        auto res = rangeMergeCost(arr, queries);
        // range [2,3] -> values {2,3} each once -> frequencies {1,1} -> cost 2
        assert(res[0] == 2);
    }

    // Test 5: Empty range
    {
        std::vector<int> arr = {1,2,3};
        std::vector<std::pair<int,int>> queries = {{2,2}};
        auto res = rangeMergeCost(arr, queries);
        assert(res[0] == 0);
    }

    // Test 6: Mixed frequencies: [1,1,2,2,3] full range -> frequencies {2,2,1} -> Huffman: merge 1+2=3 (cost 3), then 3+2=5 (cost 5) total 8
    {
        std::vector<int> arr = {1,1,2,2,3};
        std::vector<std::pair<int,int>> queries = {{0,5}};
        auto res = rangeMergeCost(arr, queries);
        assert(res[0] == 8);
    }

    // Test 7: Large test with heavy value, ensure it works
    {
        int n = 10000;
        std::vector<int> arr(n);
        for (int i = 0; i < n; ++i) arr[i] = (i % 5);
        std::vector<std::pair<int,int>> queries = {{0,n}};
        auto res = rangeMergeCost(arr, queries);
        // frequencies: each of 5 values appears 2000 times -> multiset of 2000 repeated 5 times -> Huffman cost?
        // We can check by brute force in a small version, but here just assert no crash and positive cost.
        assert(res[0] > 0);
    }

    return 0;
}
