/*
Write a C++ function that takes a positive integer `n` and returns a `std::vector<int>` containing all integers `i` from 1 to 10000 (inclusive) such that `i % n == 2`. The function should handle any positive `n`, including 1 (where every integer from 1 to 10000 has remainder 0, so the result is empty), and large `n` (where no integers satisfy the condition if `n > 10000`). The function must be pure (no I/O) and return the results in ascending order, which naturally occurs by iterating upward.
*/
#include <vector>

// Return all integers i in [1, 10000] such that i % n == 2, in ascending order.
std::vector<int> numbersWithRemainderTwo(int n) {
    std::vector<int> result;
    constexpr int limit = 10000;
    for (int i = 1; i <= limit; ++i) {
        if (i % n == 2) {
            result.push_back(i);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Declaration matching the solution.
std::vector<int> numbersWithRemainderTwo(int n);

int main() {
    // n = 1: all remainders are 0, so empty.
    assert(numbersWithRemainderTwo(1) == std::vector<int>({}));

    // n = 2: remainders are 0 or 1, so empty.
    assert(numbersWithRemainderTwo(2) == std::vector<int>({}));

    // n = 3: i % 3 == 2 for i = 2,5,8,... but test a small subset.
    std::vector<int> v3 = numbersWithRemainderTwo(3);
    assert(v3.size() == 3333);  // (2 + 3*3332) = 9998 ≤ 10000, next would be 10001.
    assert(v3.front() == 2);
    assert(v3.back() == 9998);

    // n = 100: i % 100 == 2 for i = 2,102,202,... up to 9902.
    std::vector<int> v100 = numbersWithRemainderTwo(100);
    assert(v100.size() == 100);  // (9902 - 2) / 100 + 1 = 100.
    assert(v100.front() == 2);
    assert(v100.back() == 9902);

    // n = 10000: only i = 2 gives remainder 2.
    std::vector<int> v10000 = numbersWithRemainderTwo(10000);
    assert((v10000 == std::vector<int>{2}));

    // n = 10001: since i < n, i % n = i, only i = 2 matches.
    std::vector<int> v10001 = numbersWithRemainderTwo(10001);
    assert((v10001 == std::vector<int>{2}));

    // n large > 10000, same as above.
    std::vector<int> vBig = numbersWithRemainderTwo(999999);
    assert((vBig == std::vector<int>{2}));

    return 0;
}
// The solution iterates through all integers from 1 to 10000 and checks whether the remainder when dividing by `n` equals 2 using the modulo operator (`%`). Since the loop goes from 1 upward, the collected results are automatically sorted ascending. Key edge cases: if `n == 1`, then `i % 1` is always 0, so the result is empty. If `n > 10000`, then all remainders are between 0 and `n-1` but since `i ≤ 10000 < n`, the remainder is always `i` itself, which is never 2 (except when `i=2`, but `2 < n` so `2 % n = 2`, but this only happens if `n > 2`; for `n > 10000`, `2 % n = 2` is still true because `2 < n`). Wait, careful: for `n > 10000`, `i % n = i` for all `i <= 10000`, so the condition holds only for `i == 2`. So the function returns `{2}` for any `n > 2`. For `n == 2`, `i % 2` equals 2? No, remainder is 0 or 1, so the result is empty. For `n == 3`, we get `i = 2, 5, 8, ...` up to 10000. The algorithm is a simple linear scan: for each integer in the fixed range, perform one modulo operation and a comparison. Time complexity is O(10000) = O(1) since the range is constant, and space complexity is O(k) where k is the number of matches (at most about 3333 for small n), but since the range is fixed, it is also effectively O(1) in asymptotic terms. No special optimizations are needed.
