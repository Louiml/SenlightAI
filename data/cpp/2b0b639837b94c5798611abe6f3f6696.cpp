// You are given a set of `n` distinct 64-bit integers and a positive non-negative integer `m`. You must process a sequence of `m` update operations on the set, each of the form `+ l r` (increment every element in the set that lies in the inclusive range `[l, r]` by 1) or `- l r` (decrement every element in the set that lies in `[l, r]` by 1). After each operation, before printing the answer, you must repeatedly remove the smallest element from the set as long as the difference between the current maximum and minimum exceeds `m`. After each operation (including the initial state before any updates), you must output the current maximum element of the set. The ranges `[l, r]` may be large (up to `1e18`), and you may assume that the set is never empty throughout the process. Write a C++ function `long long processSet(long long m, vector<long long> initial, vector<pair<char,pair<long long,long long>>> ops)` that returns a vector of long long, where the first element is the maximum after the initial pruning, and each subsequent element is the maximum after applying the corresponding operation (with pruning applied immediately after that operation).
// The key observation is that we only ever care about the maximum element, and we only ever remove the smallest elements. The operations add or subtract 1 to elements in a range. But note that an element that is currently the maximum may be popped if a smaller element gets increased enough to exceed it, but that is automatically handled by always keeping the set sorted. The naive approach of storing all elements and iterating over the range is impractical because `l,r` can be huge. Instead, we maintain a sorted set (e.g., `std::set`) of the current distinct values. For each operation, we need to update only those elements whose values fall in `[l, r]`. Since we know the current maximum `maxVal = *st.rbegin()`, any element `v` with `v > maxVal` doesn't exist, and any element with `v > r` won't be affected. Also, any element `< l` won't be affected. Therefore the affected elements are exactly those in `[l, r]` intersect `[minVal, maxVal]`. Since the set size is limited by `n` (though operations can create new distinct values, but the total distinct values can grow, but we can bound it because each operation adds at most a constant number of new values per element touched). However, in the worst case, if `m` is large and operations touch many elements, the set can become large. But we can handle it by iterating over all elements in the range `[l, r]` from the set using `lower_bound` and iterating forward. For each such element, we erase it and insert the modified value. One critical detail: when incrementing (d=+1), if we modify an element, it might collide with an existing element (since original values are distinct, incrementing by 1 could collide with the next value). But because we process iteratively from the smallest affected to the largest, when we increment `v` to `v+1`, if `v+1` already exists, we must merge them: the new value becomes `v+1`, and we remove the duplicate. The provided code snippet uses a clever trick with `dont` flag to avoid erasing the inserted element incorrectly when `d = -1` (decrement). For decrement, if we process from larger to smaller, we might create a collision with a smaller existing value, and we need to keep only one copy. The safest approach is: for each affected value, erase it, then insert the new value; if the insertion fails (i.e., the value already exists), we discard the duplicate (since the set contains distinct values). For decrement, processing from large to small avoids accidentally skipping an element because after decrementing an element it might become equal to the next smaller element we haven't processed yet; we must handle that by not double-processing. The snippet's logic is intricate but correct. We can simplify: for `+` operation, collect all affected values into a list, sort them ascending, then for each, erase and insert `v+1` (if `v+1` already exists, we simply don't add a duplicate). For `-` operation, collect all affected values, sort descending, then for each, erase and insert `v-1` (if collision, ignore). This avoids complex iterators. After updating, we prune: while `*st.rbegin() - *st.begin() > m`, erase the smallest. Then output the maximum. The initial state also requires pruning before the first output. Since `m` can be up to `1e18` and values up to `1e18`, and number of updates `m` (the same `m` as gap threshold? Actually the problem input has `n` and `m` where `m` is both the gap threshold and the number of operations? The snippet reads `n, m`, then `m` operations, but also uses `m` as the gap threshold. That is confusing. In the snippet, `ll n, m; cin>>n>>m;` then it reads `n` initial values, then does while(m--) operations, and uses `m` inside `upd()` as the threshold. But after decrementing `m`, the threshold changes! That's a bug. So for our task, we must decouple: let `gap` be the threshold, and `q` be the number of operations. In our task, we'll specify gap and operations separately. Complexity: each operation touches at most the number of elements in the range, which can be large, but in practice the set size is bounded by `n` plus at most `q` new values (since each operation on a value can create at most one new distinct value per operation). So worst-case total time is O((n+q) log n). However, if a single operation touches many elements, it could be O(k log n) where k is the number of affected elements. But since we only iterate over existing elements in the range, and each element is processed once per operation that covers it, overall across all operations it could be O(q * n) in worst case. But typical constraints assume that the total number of updates to elements is manageable. We'll state that the solution runs in O((n+q) log n + total_updates log n) time, where total_updates is the number of element modifications. Space O(n+q). Edge cases: operation range may be entirely above maximum or below minimum, then no changes. After deletion of smallest, if set becomes empty? We assume never empty because at least one element remains. When using `std::set`, we need to use `long long`. Also note that values can become negative, but set handles it. We'll implement a robust function.
#include <vector>
#include <set>
#include <algorithm>

// Process a set of initial values under a series of range increment/decrement operations.
// Parameters:
//   gap: after each operation, repeatedly remove the smallest element while (max - min) > gap.
//   initial: initial distinct set elements (may be unsorted, duplicates ignored).
//   ops: sequence of operations, each as pair<char, pair<long long, long long>>.
//        op.first == '+' means add 1 to all elements in [l, r]; '-' means subtract 1.
// Returns a vector of long long: first element is the maximum after initial pruning,
// and each subsequent element is the maximum after each operation (with pruning applied).
std::vector<long long> processSet(
        long long gap,
        const std::vector<long long>& initial,
        const std::vector<std::pair<char, std::pair<long long, long long>>>& ops) {
    
    std::set<long long> st(initial.begin(), initial.end());
    std::vector<long long> answer;

    // Helper to prune the set: remove smallest while max-min > gap.
    auto prune = [&]() {
        while (!st.empty() && *st.rbegin() - *st.begin() > gap) {
            st.erase(st.begin());
        }
    };

    prune();
    answer.push_back(*st.rbegin());

    for (const auto& op : ops) {
        char type = op.first;
        long long l = op.second.first;
        long long r = op.second.second;

        // Bound the range to elements currently in the set.
        if (st.empty()) {
            answer.push_back(0); // Not expected by problem, but safe.
            continue;
        }

        long long minVal = *st.begin();
        long long maxVal = *st.rbegin();
        // Only elements in [l, r] intersect [minVal, maxVal] can be affected.
        long long lower = std::max(l, minVal);
        long long upper = std::min(r, maxVal);
        if (lower > upper) {
            answer.push_back(*st.rbegin());
            continue;
        }

        std::vector<long long> affected;
        auto it = st.lower_bound(lower);
        while (it != st.end() && *it <= upper) {
            affected.push_back(*it);
            ++it;
        }

        if (type == '+') {
            // Process in ascending order to handle collisions.
            std::sort(affected.begin(), affected.end());
            for (long long v : affected) {
                st.erase(v);
                st.insert(v + 1);
            }
        } else { // type == '-'
            // Process in descending order to handle collisions.
            std::sort(affected.rbegin(), affected.rend());
            for (long long v : affected) {
                st.erase(v);
                st.insert(v - 1);
            }
        }

        prune();
        answer.push_back(*st.rbegin());
    }

    return answer;
}
#include <cassert>
#include <vector>
#include <utility>
#include <iostream>

// Include the solution function here (or #include the solution file).

int main() {
    // Test 1: basic increment and decrement
    {
        std::vector<long long> init = {1, 5, 10};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {2, 6}});  // 5 becomes 6, set {1,6,10}, max=10
        ops.push_back({'-', {1, 10}}); // decrement all -> {0,5,9}, max=9
        std::vector<long long> res = processSet(100, init, ops);
        assert(res == std::vector<long long>({10, 10, 9}));
    }

    // Test 2: pruning after operation
    {
        std::vector<long long> init = {1, 2, 3};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {1, 3}}); // becomes {2,3,4}, max=4, min=2, gap=2 <= 2? gap=2, okay
        // but with gap=1: threshold 1, after update {2,3,4} diff=2>1 -> prune smallest (2) -> {3,4} max=4
        std::vector<long long> res = processSet(1, init, ops);
        assert(res == std::vector<long long>({3, 4})); // initial max=3 after pruning (1 removed? initial {1,2,3} diff=2>1, prune 1 -> {2,3} max=3)
    }

    // Test 3: collision on increment
    {
        std::vector<long long> init = {1, 2, 3};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {1, 1}}); // 1 becomes 2, merge with existing 2 -> set {2,3}, max=3
        std::vector<long long> res = processSet(10, init, ops);
        assert(res[0] == 3);
        assert(res[1] == 3);
    }

    // Test 4: collision on decrement
    {
        std::vector<long long> init = {2, 3, 4};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'-', {3, 3}}); // 3 becomes 2, merge with existing 2 -> set {2,4}, max=4
        std::vector<long long> res = processSet(10, init, ops);
        assert(res[0] == 4);
        assert(res[1] == 4);
    }

    // Test 5: operation range that doesn't affect any element
    {
        std::vector<long long> init = {10, 20};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {0, 5}}); // none affected
        ops.push_back({'-', {100, 200}}); // none affected
        std::vector<long long> res = processSet(0, init, ops);
        assert(res == std::vector<long long>({20, 20, 20}));
    }

    // Test 6: large values and negative values after decrement
    {
        std::vector<long long> init = {-1000000000000000000LL, 0, 1000000000000000000LL};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'-', {0, 1000000000000000000LL}}); // 0->-1, max->-1? actually 0 becomes -1, max 1e18 unchanged
        std::vector<long long> res = processSet(2000000000000000000LL, init, ops);
        assert(res[0] == 1000000000000000000LL);
        assert(res[1] == 1000000000000000000LL);
    }

    // Test 7: heavy pruning
    {
        std::vector<long long> init = {1, 2, 100};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {1, 2}}); // becomes {2,3,100}, diff=98 > gap=5 -> prune 2,3 -> {100} max=100
        std::vector<long long> res = processSet(5, init, ops);
        assert(res == std::vector<long long>({100, 100}));
    }

    // Test 8: no operations
    {
        std::vector<long long> init = {5, 1, 9};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        std::vector<long long> res = processSet(0, init, ops);
        assert(res == std::vector<long long>({5})); // initial pruning: diff=8>0 -> remove 1 and 5? Actually {1,5,9} diff=8>0 -> remove 1, then {5,9} diff=4>0 -> remove 5, then {9} max=9. Wait: let's recalc: gap=0, while max-min>0, so remove min until all equal. So after pruning, set becomes {9}? Actually initial: {1,5,9} -> remove 1 -> {5,9} diff=4>0 -> remove 5 -> {9} diff=0 stop, max=9. So assert res[0]==9. Let's correct.
        assert(res[0] == 9);
    }

    // Test 9: operation that reduces the max and then pruning may change answer
    {
        std::vector<long long> init = {1, 5};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'-', {5, 5}}); // 5 becomes 4 -> set {1,4}, max=4, gap=1, diff=3>1 -> remove 1 -> {4}, max=4
        std::vector<long long> res = processSet(1, init, ops);
        assert(res == std::vector<long long>({5, 4})); // initial: {1,5} diff=4>1 -> remove 1 -> {5} max=5
    }

    // Test 10: many operations with collisions
    {
        std::vector<long long> init = {0, 1, 2};
        std::vector<std::pair<char, std::pair<long long, long long>>> ops;
        ops.push_back({'+', {0, 2}}); // becomes {1,2,3}, max=3
        ops.push_back({'-', {1, 3}}); // becomes {0,1,2}, max=2
        std::vector<long long> res = processSet(10, init, ops);
        assert(res == std::vector<long long>({2, 3, 2}));
    }

    std::cout << "All tests passed!\n";
    return 0;
}
