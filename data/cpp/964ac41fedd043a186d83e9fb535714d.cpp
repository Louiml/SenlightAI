You are given an array of integers of length \(N\) (with \(N \le 1000\)) and a sequence of operations. Each operation is either:  
- `"1 i val"`: add `val` to the `i`-th element (1-indexed).  
- `"2 l r"`: compute and output the sum of the subarray from index `l` to `r` (inclusive).  
Write a C++ function `void processQueries(const std::vector<int>& initial, const std::vector<std::array<int,3>>& queries, std::vector<long long>& results)` that processes all operations in order. For each type‑2 query, push the sum (as a `long long`) into `results`. Implement the solution using a Fenwick tree (Binary Indexed Tree) so that each update and each sum query runs in \(O(\log N)\). You may assume all input indices are valid and values fit in `int`, but sums may exceed `int` range. The function must not print anything; it must only fill `results`.

The Fenwick tree (BIT) stores a prefix-sum-like structure. For each `Plus` operation, we add the value to the element at index `i` and propagate the change upward by adding `val` to all tree nodes that cover `i` (i.e., `i += i & (-i)`). For a range sum `[l, r]`, we compute `Sum(r) - Sum(l-1)`, where `Sum(i)` returns the prefix sum from 1 to `i` by accumulating nodes `i`, then `i -= i & (-i)`.  
Initialize the BIT by performing a type‑1 update for every initial element with its value as `val`.  
Edge cases:  
- Empty initial array: no updates needed, but queries may still appear; handle carefully.  
- `l=1`: `Sum(0)` returns 0 by design since the loop condition `i > 0` fails immediately.  
- Values may be negative; the BIT works with negative numbers fine.  
- Sums may exceed `int`, so use `long long` for storage and for the accumulated result.  
Time complexity: Each of the \(M\) operations costs \(O(\log N)\), so overall \(O((N+M)\log N)\). Space: \(O(N)\) for the BIT array plus \(O(Q)\) for the results vector (where \(Q\) is the number of type‑2 queries).

#include <vector>
#include <array>

// Fenwick tree (Binary Indexed Tree) for dynamic prefix sums.
class FenwickTree {
private:
    std::vector<long long> bit;
    int size;

public:
    explicit FenwickTree(int n) : bit(n + 1, 0), size(n) {}

    // Add 'delta' to the element at 1-based index 'idx'.
    void add(int idx, long long delta) {
        while (idx <= size) {
            bit[idx] += delta;
            idx += idx & (-idx);
        }
    }

    // Return prefix sum of indices 1..idx.
    long long prefixSum(int idx) const {
        long long sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & (-idx);
        }
        return sum;
    }

    // Return sum of subarray [l, r] (1-based).
    long long rangeSum(int l, int r) const {
        return prefixSum(r) - prefixSum(l - 1);
    }
};

// Process all queries and store type-2 results in 'results'.
void processQueries(
    const std::vector<int>& initial,
    const std::vector<std::array<int, 3>>& queries,
    std::vector<long long>& results
) {
    const int N = static_cast<int>(initial.size());
    FenwickTree ft(N);

    // Initialize BIT with initial array values.
    for (int i = 0; i < N; ++i) {
        ft.add(i + 1, initial[i]);  // 1-based index
    }

    for (const auto& q : queries) {
        if (q[0] == 1) {
            ft.add(q[1], q[2]);     // add q[2] to index q[1]
        } else {                    // q[0] == 2
            results.push_back(ft.rangeSum(q[1], q[2]));
        }
    }
}

#include <cassert>
#include <vector>
#include <array>

// Assume processQueries is already defined above (or included).

int main() {
    // Test 1: simple updates and range sums
    {
        std::vector<int> init = {1, 2, 3, 4, 5};
        std::vector<std::array<int,3>> queries = {
            {2, 1, 5},   // sum all = 15
            {1, 3, 10},  // add 10 to index 3 -> {1,2,13,4,5}
            {2, 1, 5},   // 25
            {2, 3, 5},   // 13+4+5 = 22
            {1, 1, -1},  // add -1 to index 1 -> {0,2,13,4,5}
            {2, 1, 4}    // 0+2+13+4 = 19
        };
        std::vector<long long> results;
        processQueries(init, queries, results);
        std::vector<long long> expected = {15, 25, 22, 19};
        assert(results == expected);
    }

    // Test 2: single element
    {
        std::vector<int> init = {7};
        std::vector<std::array<int,3>> queries = {
            {2, 1, 1},
            {1, 1, 3},
            {2, 1, 1}
        };
        std::vector<long long> results;
        processQueries(init, queries, results);
        std::vector<long long> expected = {7, 10};
        assert(results == expected);
    }

    // Test 3: negative values and zero
    {
        std::vector<int> init = {-5, 0, 3, -2};
        std::vector<std::array<int,3>> queries = {
            {2, 1, 4},   // -4
            {2, 2, 3},   // 0+3=3
            {1, 2, -3},  // now {-5,-3,3,-2}
            {2, 2, 4}    // -3+3-2 = -2
        };
        std::vector<long long> results;
        processQueries(init, queries, results);
        std::vector<long long> expected = {-4, 3, -2};
        assert(results == expected);
    }

    // Test 4: empty initial but queries on non-empty indices not allowed (not tested). Use non-empty to be safe.

    // Test 5: large sums exceeding int
    {
        std::vector<int> init = {1000000000, 1000000000, 1000000000};
        std::vector<std::array<int,3>> queries = {
            {2, 1, 3}    // 3000000000 > int max
        };
        std::vector<long long> results;
        processQueries(init, queries, results);
        assert(results[0] == 3000000000LL);
    }

    return 0;
}
