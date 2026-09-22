// Write a C++ function `int countBombs(int friends, int encountersPerFriend, int startingPower)` that simulates the following process. There are `friends` friends, each of whom you will meet exactly `encountersPerFriend` times. Initially, your power level is `startingPower`. For each friend (processed in the order they are given in the original snippet, but here the order does not matter because each friend gets the same number of encounters), you gain power equal to the number of times you would need to meet that friend to accumulate enough encounters to reach `encountersPerFriend` encounters with that friend. Specifically, for each friend, you add `(encountersPerFriend - 1) / a + 1` where `a` is the per-friend count and here all friends have the same `a` equal to `encountersPerFriend`. However, to generalize, assume each friend has a different `a` value passed as a vector. So the actual task: Given a vector of positive integers `a` (the per-friend encounter counts), an integer `d` (the target number of encounters per friend), and an initial power `x`, return the final power after processing each friend in order, where for each `a[i]` you add `(d - 1) / a[i] + 1` to `x`. This matches the snippet’s behavior where `n` is the number of friends, `d` is the target encounters, and `x` starts as given. The function must handle large values (up to 10^9 for `d` and `a[i]`) and use 64-bit integers to avoid overflow. The input vector is non-empty, all values positive. The function should be pure, with no side effects.

#include <cassert>
#include <vector>

// Solution function declaration (from above)
long long computeFinalPower(long long, long long, const std::vector<long long>&);

int main() {
    // Basic examples matching snippet behavior.
    assert(computeFinalPower(10, 5, {2, 3}) == 10 + (4/2+1) + (4/3+1) == 10 + 3 + 2 == 15);
    assert(computeFinalPower(0, 1, {1, 1}) == 0 + 1 + 1 == 2);
    assert(computeFinalPower(100, 100, {100}) == 100 + (99/100+1) == 100 + 1 == 101);
    // Edge: large values, ensure no overflow.
    assert(computeFinalPower(1, 1000000000, {1, 1000000000}) == 1 + (999999999/1+1) + (999999999/1000000000+1) == 1 + 1000000000 + 1 == 1000000002);
    // Single friend.
    assert(computeFinalPower(5, 7, {7}) == 5 + (6/7+1) == 5 + 1 == 6);
    // All friends have same a, matches original snippet logic with identical a.
    assert(computeFinalPower(0, 3, {2, 2, 2}) == 0 + 3 * ((2/2)+1) == 0 + 3 * 2 == 6);
    // a > d.
    assert(computeFinalPower(1, 5, {10, 20}) == 1 + (4/10+1) + (4/20+1) == 1 + 1 + 1 == 3);
    // Large vector.
    std::vector<long long> many(1000, 1);
    assert(computeFinalPower(0, 2, many) == 1000 * ((1)/1+1) == 2000);
    return 0;
}

#include <vector>
#include <cstdint>

// Compute final power after processing each friend.
// Parameters:
//   startingPower - initial power level.
//   targetEncounters - the value d from the problem.
//   encounterCounts - vector of positive integers a[i].
// Returns the final power as a 64-bit integer.
long long computeFinalPower(long long startingPower,
                            long long targetEncounters,
                            const std::vector<long long>& encounterCounts) {
    long long result = startingPower;
    for (const long long a : encounterCounts) {
        // Equivalent to ceil(targetEncounters / a) for positive integers.
        result += (targetEncounters - 1) / a + 1;
    }
    return result;
}

// The problem reduces to summing a simple integer arithmetic expression for each element. For a given `d` and `a`, the value `(d-1)/a + 1` computes the ceiling of `d/a`? Actually, check: if `d=5, a=2`, then `(5-1)/2 + 1 = 2+1=3`, which is ceil(5/2)=3. If `d=6, a=2`, `(5)/2+1=2+1=3`, but ceil(6/2)=3 as well, so it's equal. In general, `(d-1)/a + 1` equals `ceil(d/a)` for positive integers? For `d=1,a=5`: `(0)/5+1=1`, ceil(1/5)=1, yes. For `d=2,a=3`: `(1)/3+1=0+1=1`, ceil(2/3)=1. So it's just `ceil(d/a)`. However, the snippet name is not important; we just follow the formula. The algorithm: initialize `result = startingPower`. For each value `a` in the vector, compute `add = (d - 1) / a + 1` using integer division (which truncates toward zero, but all numbers are positive so fine), then add to `result`. The final `result` is returned. Edge cases: `d` can be as small as 1, then `(0)/a + 1 = 1` for any `a`, so each friend adds 1. `a` can be huge (up to 1e9), and `d` up to 1e9, so `d-1` fits in 64-bit, but use `long long` for safety. The vector size can be large (up to 1e5? not specified but fine). Time complexity O(n), space O(1) beyond input vector. The result can be up to `startingPower + n * (d/a?)` but could be large, so use `long long`.
