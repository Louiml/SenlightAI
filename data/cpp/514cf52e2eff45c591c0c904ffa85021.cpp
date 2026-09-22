Implement a C++ function `FenwickRangeSum` that takes a vector of integers and a list of operations, where operations are either a range-sum query (type 0 with parameters `l` and `r`, returning the sum of elements from index `l` to `r` inclusive, using 1-based indexing) or a point update (type 1 with parameters `x` and `c`, adding `c` to the element at index `x`). The function should process all operations in order and return a vector of results for each query (type 0) in the same order they appear. Assume the input vector length is `n` (1 ≤ n ≤ 10^5), all values fit in `int`, and there are up to 10^5 operations. The function must use a Fenwick tree (Binary Indexed Tree) for efficiency, and must handle 1-based indexing internally (treating the input vector as having elements at indices 1..n). The operations are provided as a vector of tuples: each tuple has the form `(op, a, b, c)` where only relevant fields are used: for op=0, use `a` as `l`, `b` as `r` (and ignore `c`); for op=1, use `a` as `x`, `c` as the increment (ignore `b`). The function should not modify the original vector (except for internal updates to the Fenwick tree) and must be `const`-correct in the sense that the input vector and operation list are passed by const reference.

#include <cassert>
#include <vector>
#include <tuple>

// Declare the function (include the solution code above or here).
std::vector<long long> FenwickRangeSum(
    const std::vector<int>& initial,
    const std::vector<std::tuple<int, int, int, int>>& operations);

int main() {
    // Test 1: Basic query and update.
    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        std::vector<std::tuple<int, int, int, int>> ops = {
            {0, 1, 5, 0}, // sum 1..5 = 15
            {1, 3, 0, 10}, // add 10 to index 3 -> arr[3]=13
            {0, 2, 4, 0}  // sum 2..4 = 2+13+4=19
        };
        auto res = FenwickRangeSum(arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 15);
        assert(res[1] == 19);
    }

    // Test 2: Negative updates.
    {
        std::vector<int> arr = {5, -3, 2};
        std::vector<std::tuple<int, int, int, int>> ops = {
            {0, 1, 3, 0}, // 5-3+2=4
            {1, 2, 0, -5}, // arr[2] becomes -3-5=-8
            {0, 2, 2, 0}, // sum 2..2 = -8
            {0, 1, 2, 0}  // 5 + (-8) = -3
        };
        auto res = FenwickRangeSum(arr, ops);
        assert(res.size() == 3);
        assert(res[0] == 4);
        assert(res[1] == -8);
        assert(res[2] == -3);
    }

    // Test 3: Single element.
    {
        std::vector<int> arr = {10};
        std::vector<std::tuple<int, int, int, int>> ops = {
            {0, 1, 1, 0}, // 10
            {1, 1, 0, 7}, // add 7 -> 17
            {0, 1, 1, 0}  // 17
        };
        auto res = FenwickRangeSum(arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 10);
        assert(res[1] == 17);
    }

    // Test 4: Large sums requiring long long.
    {
        std::vector<int> arr(100000, 100000); // each element = 100000, sum = 10^10
        std::vector<std::tuple<int, int, int, int>> ops = {
            {0, 1, 100000, 0}, // sum = 100000 * 100000 = 10^10
            {1, 1, 0, -100000}, // arr[1] becomes 0
            {0, 1, 1, 0} // sum 1..1 = 0
        };
        auto res = FenwickRangeSum(arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 10000000000LL);
        assert(res[1] == 0);
    }

    return 0;
}

#include <vector>
#include <tuple>

// Process Fenwick tree operations. Returns results of all range-sum queries in order.
std::vector<long long> FenwickRangeSum(
    const std::vector<int>& initial,
    const std::vector<std::tuple<int, int, int, int>>& operations) {
    int n = static_cast<int>(initial.size());
    std::vector<long long> tree(n + 1, 0);

    // Build Fenwick tree from initial array (1-based indexing).
    auto update = [&](int idx, long long delta) {
        while (idx <= n) {
            tree[idx] += delta;
            idx += idx & (-idx);
        }
    };
    for (int i = 1; i <= n; ++i) {
        update(i, initial[i - 1]);
    }

    // Prefix sum query (1-based).
    auto query = [&](int idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += tree[idx];
            idx -= idx & (-idx);
        }
        return sum;
    };

    std::vector<long long> answers;
    for (const auto& op : operations) {
        int type = std::get<0>(op);
        if (type == 0) {
            int l = std::get<1>(op);
            int r = std::get<2>(op);
            answers.push_back(query(r) - query(l - 1));
        } else { // type == 1
            int x = std::get<1>(op);
            int c = std::get<3>(op);
            update(x, static_cast<long long>(c));
        }
    }
    return answers;
}

// The core algorithm is a Fenwick tree (Binary Indexed Tree) which supports point updates and prefix sum queries in O(log n) time each. We first build the tree from the initial array by performing an update for each element at its 1-based index. For each operation: if it is a query (op=0), we compute the sum from `l` to `r` as `prefix(r) - prefix(l-1)`. If it is an update (op=1), we add `c` to the Fenwick tree at index `x` (this automatically modifies the internal representation so future queries reflect the change). Edge cases: `l` and `r` are guaranteed to be within 1..n and `l ≤ r`; `x` is within 1..n; `c` can be negative, so sums may overflow `int`? Since values fit in `int` and operations are up to 10^5, the total sum could exceed `int` range; to be safe, we should use `long long` for the results and internal prefix sums. However, the problem statement says values fit in `int`, but cumulative sums might overflow `int`. Thus we store the Fenwick tree as `long long` or use `long long` for answers. Time complexity: building tree O(n log n) if we update each element, but we can build in O(n) by prefix sums; however, using update per element is fine for n ≤ 10^5. Each operation is O(log n), total O((n + q) log n). Space O(n). Important: The Fenwick tree is 1-indexed, so we allocate size n+1.
