/*
You are given a sequence of `n` pairs `(a_i, b_i)` where `1 <= n <= 10^5`, `1 <= a_i, b_i <= 10^9`. Starting from the last pair (index `n-1`) and moving backward to the first pair (index `0`), you want to make each `a_i` divisible by its corresponding `b_i` by adding a non-negative integer `cnt` that accumulates globally. Specifically, before processing pair `i`, you add the current accumulated value `cnt` to `a_i`. Let `push = a_i % b_i`. If `push != 0`, then you must increase `cnt` by exactly `(b_i - push)` so that after the addition, `(a_i + cnt)` is divisible by `b_i`. This increment to `cnt` also affects all previously processed (i.e., smaller-index) pairs because the cumulative `cnt` is added to them later. Write a C++ function `long long minimum_total_increment(const std::vector<std::pair<long long, long long>>& pairs)` that returns the final total value of `cnt` after processing all pairs from last to first according to this rule. The function must handle large inputs efficiently and avoid overflow.
*/
#include <vector>
#include <cstdint>

// Computes the total extra amount needed so that after processing from right to left,
// each a_i plus the cumulative increment becomes divisible by b_i.
long long minimum_total_increment(const std::vector<std::pair<long long, long long>>& pairs) {
    long long cnt = 0;
    for (int i = static_cast<int>(pairs.size()) - 1; i >= 0; --i) {
        long long current_a = pairs[i].first + cnt;
        long long b = pairs[i].second;
        long long remainder = current_a % b;
        if (remainder != 0) {
            cnt += (b - remainder);
        }
    }
    return cnt;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link directly).

int main() {
    // Basic case: no increments needed because all are divisible.
    std::vector<std::pair<long long, long long>> test1 = {{10, 5}, {8, 4}, {12, 3}};
    assert(minimum_total_increment(test1) == 0);

    // Single pair needing increment.
    std::vector<std::pair<long long, long long>> test2 = {{3, 2}};
    assert(minimum_total_increment(test2) == 1);

    // Multiple pairs: increments accumulate for earlier ones.
    std::vector<std::pair<long long, long long>> test3 = {{3, 2}, {7, 3}};
    // Process i=1: a=7, cnt=0, remainder=1 => cnt=2. Process i=0: a=3+2=5, remainder=1 => cnt+=1 => total 3? Wait check: 5%2=1 => cnt=3. So result 3.
    assert(minimum_total_increment(test3) == 3);

    // All pairs with b=1, no increments.
    std::vector<std::pair<long long, long long>> test4 = {{5, 1}, {100, 1}, {7, 1}};
    assert(minimum_total_increment(test4) == 0);

    // Large values, ensure overflow-safe.
    std::vector<std::pair<long long, long long>> test5 = {{1000000000LL, 999999937LL}, {1, 2}};
    // Process i=1: a=1, remainder=1 -> cnt=1. Process i=0: a=1000000000+1=1000000001, remainder=64? 1000000001 % 999999937 = 64? Actually compute: 1000000001 - 999999937 = 64 => cnt+=999999937-64=999999873 => total 999999874.
    assert(minimum_total_increment(test5) == 999999874LL);

    // Already correct after prior increments.
    std::vector<std::pair<long long, long long>> test6 = {{2, 2}, {4, 2}, {6, 2}};
    assert(minimum_total_increment(test6) == 0);

    // Single pair with a multiple.
    std::vector<std::pair<long long, long long>> test7 = {{10, 5}};
    assert(minimum_total_increment(test7) == 0);

    // Two pairs where first needs adjustment after second increments.
    std::vector<std::pair<long long, long long>> test8 = {{1, 3}, {1, 2}};
    // i=1: a=1%2=1 => cnt=1. i=0: a=1+1=2%3=2 => cnt+=1 => total 2.
    assert(minimum_total_increment(test8) == 2);

    // Empty input.
    std::vector<std::pair<long long, long long>> test9 = {};
    assert(minimum_total_increment(test9) == 0);

    // One pair with huge values.
    std::vector<std::pair<long long, long long>> test10 = {{1000000000000000000LL, 1000000000000000000LL}};
    assert(minimum_total_increment(test10) == 0);

    return 0;
}
// The algorithm iterates from the last pair to the first, maintaining a global cumulative `cnt` that represents the total amount added so far. For each pair `(a, b)` at index `i`, we first add `cnt` to `a[i]` because all previous increments affect this element. Then we compute `push = a[i] % b[i]`. If `push` is zero, no additional increment is needed; otherwise, we need to increase `cnt` by `(b[i] - push)` so that the new `a[i] + cnt` becomes the smallest multiple of `b[i]` at least as large as the current sum. This greedy strategy works because processing from right to left ensures that any increment we add for a higher index will be added to all lower indices, so we must adjust the lower indices later with the updated `cnt`. Edge cases include when `b[i]` equals 1 (then `a[i] % 1 == 0` always, no increment), and when `a[i]` is already divisible by `b[i]` after adding `cnt`. Time complexity is `O(n)` because each pair is processed once with constant-time operations. Space complexity is `O(1)` auxiliary, aside from the input vector itself.
