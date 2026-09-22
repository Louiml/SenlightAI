Write a C++ function that processes an array of integers and answers range sum queries. Given an array `v` of `n` integers and `q` queries, each query specifies two indices `l` and `r` (1-based), and the function must return the sum of elements from index `l` to index `r` inclusive. The function should be efficient for large `n` and `q` (up to 10^5), and must handle arbitrary query ranges within `1 ≤ l ≤ r ≤ n`. The function signature should be `long long rangeSum(const std::vector<long long>& arr, long long l, long long r)` that returns the sum for a single query, but since multiple queries are expected, the implementation must preprocess the array so that each query runs in O(log n) time (e.g., using a Fenwick tree). The function must be const-correct, cannot modify the input array, and should include appropriate headers and a brief comment describing the algorithm.
int main() {
    // Test with 1-based array: indices 1..5
    std::vector<long long> arr = {0, 5, 3, -2, 8, 1};  // 0 is dummy at index 0
    assert(rangeSum(arr, 1, 1) == 5);
    assert(rangeSum(arr, 1, 5) == 15);
    assert(rangeSum(arr, 2, 4) == 9);
    assert(rangeSum(arr, 5, 5) == 1);
    assert(rangeSum(arr, 3, 3) == -2);

    // Test with negative values and larger range
    std::vector<long long> arr2 = {0, -10, 20, -30, 40, -50, 60};
    assert(rangeSum(arr2, 1, 6) == 30);
    assert(rangeSum(arr2, 2, 5) == -20);
    assert(rangeSum(arr2, 4, 6) == 50);

    // Test with single-element array
    std::vector<long long> arr3 = {0, 42};
    assert(rangeSum(arr3, 1, 1) == 42);

    // Test with all zeros
    std::vector<long long> arr4 = {0, 0, 0, 0};
    assert(rangeSum(arr4, 1, 4) == 0);
    assert(rangeSum(arr4, 2, 3) == 0);
}
#include <vector>

// Fenwick tree (Binary Indexed Tree) implementation for range sum queries.
// The input array is 1-based indexed (indices 1..n). The function returns
// the sum of elements from index l to r inclusive, using a precomputed BIT.
long long rangeSum(const std::vector<long long>& arr, long long l, long long r) {
    long long n = static_cast<long long>(arr.size()) - 1; // arr[0] is dummy
    std::vector<long long> BIT(n + 1, 0);

    // Build Fenwick tree from arr.
    auto update = [&](long long idx, long long delta) {
        while (idx <= n) {
            BIT[idx] += delta;
            idx += (idx & -idx);
        }
    };

    auto prefixSum = [&](long long idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += BIT[idx];
            idx -= (idx & -idx);
        }
        return sum;
    };

    for (long long i = 1; i <= n; ++i) {
        update(i, arr[i]);
    }

    return prefixSum(r) - prefixSum(l - 1);
}
// The solution uses a Fenwick tree (Binary Indexed Tree) for efficient prefix-sum queries and point updates. Build the tree from the input array by initializing it with zeros and calling `update` for each element at its index. Each `update` adds the value to the tree at the index and propagates up the indices using the `i += (i & -i)` pattern. For a query `(l, r)`, compute `prefixSum(r) - prefixSum(l-1)`, where `prefixSum(i)` uses the `i -= (i & -i)` pattern to accumulate values from the tree. Edge cases include: `l = 1` (then `l-1 = 0`, and `prefixSum(0)` should return 0), `r = n` (works fine), and arrays with negative numbers (handled naturally by sum). If `n` is small or queries are few, a simpler prefix-sum array would suffice, but the Fenwick tree is chosen for consistency with the original snippet and to handle dynamic updates if needed. Time complexity: building the tree is O(n log n), each query is O(log n), and space is O(n).
