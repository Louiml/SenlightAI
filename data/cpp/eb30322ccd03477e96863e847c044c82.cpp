Implement a C++ function that computes the sum of elements in a given subarray `[l, r]` (0-indexed, inclusive) of an integer array using a sparse table for range sum queries. The function should support static arrays (no updates) and handle multiple queries efficiently. The sparse table must be precomputed from the array, and each query must be answered by decomposing the interval into disjoint power-of-two blocks and summing their precomputed values. Handle edge cases such as empty subarrays (where `l > r` should return 0), single-element ranges, and ranges that exactly align with power-of-two lengths. The provided array length `n` and the maximum query index are guaranteed to be within `[1, 10^6]`, and the number of queries can be up to `10^5`. All values in the array are 32-bit integers; sums may exceed 32-bit range, so use a 64-bit integer for results.

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Basic test 1: small array
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    SparseTableSum st1(arr1);
    assert(st1.query(0, 4) == 15);
    assert(st1.query(1, 3) == 9);
    assert(st1.query(2, 2) == 3);
    assert(st1.query(0, 0) == 1);
    assert(st1.query(4, 4) == 5);
    assert(st1.query(3, 4) == 9);
    
    // Test empty range
    assert(st1.query(4, 3) == 0);
    
    // Test negative numbers
    std::vector<int> arr2 = {-5, 10, -3, 7, 0, 2};
    SparseTableSum st2(arr2);
    assert(st2.query(0, 5) == 11);
    assert(st2.query(1, 2) == 7);
    assert(st2.query(0, 1) == 5);
    assert(st2.query(2, 4) == 4);
    
    // Test larger array with single element
    std::vector<int> arr3 = {42};
    SparseTableSum st3(arr3);
    assert(st3.query(0, 0) == 42);
    assert(st3.query(0, 0) == 42);
    
    // Test full range of large array (power-of-two length)
    std::vector<int> arr4 = {1, 2, 3, 4, 5, 6, 7, 8};
    SparseTableSum st4(arr4);
    assert(st4.query(0, 7) == 36);
    assert(st4.query(2, 5) == 18);
    
    // Test non-power-of-two length decomposition
    std::vector<int> arr5 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // 10 ones
    SparseTableSum st5(arr5);
    assert(st5.query(0, 9) == 10);
    assert(st5.query(3, 7) == 5);
    
    // Test large values summing to 64-bit
    std::vector<int> arr6 = {1000000000, 1000000000, 1000000000};
    SparseTableSum st6(arr6);
    assert(st6.query(0, 2) == 3000000000LL); // exceeds int32
    
    return 0;
}

#include <vector>
#include <cstdint>
#include <cmath>

class SparseTableSum {
public:
    // Constructor builds the sparse table from the input array
    SparseTableSum(const std::vector<int>& arr) : n(arr.size()) {
        if (n == 0) return;
        // Maximum power of two that fits in n
        K = static_cast<int>(std::floor(std::log2(n))) + 1;
        st.resize(n, std::vector<int64_t>(K, 0));
        
        // Initialize level 0
        for (int i = 0; i < n; ++i) {
            st[i][0] = arr[i];
        }
        
        // Build higher levels
        for (int j = 1; j < K; ++j) {
            int block = 1 << (j - 1); // half length
            int full_len = 1 << j;
            for (int i = 0; i + full_len <= n; ++i) {
                st[i][j] = st[i][j-1] + st[i + block][j-1];
            }
        }
    }
    
    // Query sum on [l, r] inclusive, 0-indexed
    int64_t query(int l, int r) const {
        if (l > r || l < 0 || r >= n) return 0;
        int64_t sum = 0;
        int len = r - l + 1;
        // Decompose into power-of-two blocks from largest to smallest
        for (int j = K - 1; j >= 0; --j) {
            int block_len = 1 << j;
            if (len >= block_len) {
                sum += st[l][j];
                l += block_len;
                len -= block_len;
            }
        }
        return sum;
    }
    
private:
    int n;
    int K;
    std::vector<std::vector<int64_t>> st;
};

// The solution uses a sparse table, a classic static data structure for idempotent or associative operations. For range sums, the operation is associative but not idempotent, so we cannot use the overlapping-query trick (which works for min/max); instead we decompose the query interval into disjoint power-of-two intervals. The sparse table `st[i][j]` stores the sum of the subarray starting at index `i` with length `2^j`. Precomputation fills `st[i][0]` with `a[i]`, then for each level `j` from 1 to `K` (where `K = floor(log2(N))` after ensuring N is large enough), we combine two halves: `st[i][j] = st[i][j-1] + st[i + 2^(j-1)][j-1]`. For a query `[l, r]`, we loop `j` from `K` down to 0; while the remaining interval length is at least `2^j`, we add the corresponding block sum and advance `l` by `2^j`. This guarantees each position is covered exactly once. Proper bounds must be checked to avoid out-of-range access during precomputation; the snippet uses `i + (1 << j) <= N` but the actual array length is `n`, so we should use `i + (1 << j) <= n` to avoid reading garbage. Edge cases: empty subarray (return 0), single element (sum is that element), and large sums requiring 64-bit. Time complexity: precomputation O(n log n) and each query O(log n), or O(1) per query if we decompose into at most two blocks? Not for sum; we need O(log n) decomposition. Space: O(n log n). The function should be designed as a class or a free function that accepts the array and builds internal tables; the task specifies a free function, so we can return a lambda or a struct, but the reference solution will present a class-like structure with a method for queries. Since the task asks for a free function that matches the specification, we can implement a function `rangeSumSparseTable` that takes the array and returns a callable query function via a struct, but the simplest is to define a class inside the solution.
