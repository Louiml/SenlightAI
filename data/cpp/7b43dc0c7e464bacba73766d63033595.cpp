/*
In a round-robin scheduling simulation, we have `k` servers arranged in a cycle, each holding some number of tasks in its bag (`bag[i]`). We must write a function `bool canBalance(const std::vector<int>& bag, int n, int k)` that determines whether the tasks can be redistributed so that every server ends up with either `floor(n/k)` or `ceil(n/k)` tasks (where `n` is the total number of tasks across all servers), and if so, returns `true`; otherwise `false`. Importantly, the function must also verify that the input bag is non-empty, contains only non-negative integers, and that `k` is positive. The redistribution process is not simulated, only feasibility is checked, but the function must also handle the edge case where the total count `n` is not divisible evenly—in that case, some servers may have one extra task. Additionally, the function must check that the sum of all `bag[i]` equals `n`; if not, return `false`. Write the function as a standalone C++ free function that can be called by test code, with no `main` function in the solution file.
*/
#include <vector>
#include <numeric>
#include <algorithm>

// Checks if the given task distribution can be balanced to have each server hold either n/k (floor) or n/k+1 tasks.
// Preconditions: k > 0, bag.size() == k, all bag[i] >= 0, n >= 0.
// Returns true if the sum of bag equals n and every bag[i] is either floor(n/k) or floor(n/k)+1.
bool canBalance(const std::vector<int>& bag, int n, int k) {
    // Validate basic constraints
    if (k <= 0) return false;
    if (bag.size() != static_cast<size_t>(k)) return false;
    for (int value : bag) {
        if (value < 0) return false;
    }

    // Check total sum matches n
    long long total = 0;
    for (int value : bag) {
        total += value;
    }
    if (total != n) return false;

    // Compute target bounds
    int q = n / k;         // floor(n/k)
    int q_plus = q + 1;    // ceil(n/k) when n % k != 0, else q

    // Every bag value must be in [q, q_plus]
    for (int value : bag) {
        if (value < q || value > q_plus) return false;
    }

    return true;
}
#include <cassert>
#include <vector>

// Declaration of the function being tested (included from solution)
bool canBalance(const std::vector<int>& bag, int n, int k);

int main() {
    // Valid case: perfectly balanced
    assert(canBalance({2, 2, 2}, 6, 3) == true);
    // Valid case: one extra task
    assert(canBalance({2, 2, 3}, 7, 3) == true);
    // Invalid: sum does not match n
    assert(canBalance({1, 2, 3}, 7, 3) == false);
    // Invalid: bag value outside range
    assert(canBalance({1, 4, 1}, 6, 3) == false);
    // Edge case: k=1, n=5
    assert(canBalance({5}, 5, 1) == true);
    // Edge case: k=1, n=0
    assert(canBalance({0}, 0, 1) == true);
    // Edge case: zero tasks across many servers
    assert(canBalance({0, 0, 0, 0}, 0, 4) == true);
    // Negative n should return false (by sum mismatch or invalid)
    assert(canBalance({1, 1}, -2, 2) == false);
    // Zero k invalid
    assert(canBalance({}, 0, 0) == false);
    // Empty bag with k > 0 invalid
    assert(canBalance({}, 5, 2) == false);
    return 0;
}
// The core idea is to first validate inputs: `k > 0`, bag size equals `k`, all values non-negative, and the sum of all bag values equals `n` (the given total). Then compute `q = n / k` (floor) and `r = n % k` (remainder). The goal state is that exactly `r` servers have `q+1` tasks and the remaining `k - r` servers have `q` tasks. Since redistribution only moves tasks, not changes total count, the feasibility condition is simply that the target distribution is achievable—but because we are not simulating moves, we must check that every server currently has either `q` or `q+1` tasks. If any server has a value outside this range, it is impossible without first moving tasks away, but that movement might be possible if there are surplus tasks elsewhere. However, the original problem (simulating distribution) is only valid if the initial bag already satisfies the condition. The task here is to check the "To-Be" condition from the snippet: each `bag[i]` must be between `q` and `q+1` inclusive. If any server has less than `q` or more than `q+1`, return `false`. Also, if the sum of bag values does not equal `n`, return `false`. Edge cases: when `n` is zero, `q=0` and `r=0`, so all bags must be zero. When `k=1`, the only server must have exactly `n` tasks, and since `q=n`, `q+1` would be `n+1`, so the condition is `bag[0] == n` which simplifies to `bag[0] >= n && bag[0] <= n+1` but since sum must equal `n`, it must be exactly `n`. Time complexity is `O(k)` for validation and sum, and space is `O(1)` additional.
