/*
Write a C++ function `vector<long long> findYogicNumbers(long long c)` that, given a positive integer `c`, finds all non-negative integers `x` such that when you take `x`, multiply it by 9, and add a single digit `y` (0 ≤ y ≤ 9) to the product, the result equals `c`. In other words, find all integers `x` for which there exists a digit `y` satisfying `9*x + y == c`. Return the list of such `x` values in ascending order. If no such `x` exists, return an empty vector. The function must handle `c` up to `10^18` safely. Note the relationship: for each valid `x`, the corresponding `y` is `c - 9*x`, and this `y` must be a single digit (0 to 9). Also, `c` is guaranteed to be positive, but you must handle the case where `c` is tiny (e.g., `c=1` yields no valid `x` because `9*0+1=1` gives `y=1`, so `x=0` is valid? Check: for `c=1`, `x=0` gives `9*0+0=0` not 1, so `x=0` not valid; `x` must be integer, try `x=0` -> `9*0=0`, `y=1` works? Actually `9*0+1=1`, so yes `y=1` is a digit, so `x=0` is valid). Ensure your function correctly identifies all such integers.
*/
#include <vector>
#include <algorithm>

// Given a positive integer c, return all non-negative integers x such that
// there exists a digit y (0..9) with 9*x + y == c. Sorted ascending.
std::vector<long long> findYogicNumbers(long long c) {
    std::vector<long long> result;
    // Try every possible digit y from 0 to 9.
    for (int y = 0; y <= 9; ++y) {
        long long remainder = c - y;
        // remainder must be non-negative and divisible by 9.
        if (remainder >= 0 && remainder % 9 == 0) {
            long long x = remainder / 9;
            result.push_back(x);
        }
    }
    // The loop processes y in increasing order, but x = (c-y)/9 is decreasing.
    // So sort to return in ascending order.
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Declaration of the function under test.
std::vector<long long> findYogicNumbers(long long c);

int main() {
    // Test 1: c=1 -> only x=0 (9*0+1=1)
    assert(findYogicNumbers(1) == std::vector<long long>{0});

    // Test 2: c=9 -> y=0 gives x=1, y=9 gives x=0 -> {0,1}
    assert(findYogicNumbers(9) == (std::vector<long long>{0, 1}));

    // Test 3: c=18 -> y=0 gives x=2, y=9 gives x=1 -> {1,2}
    assert(findYogicNumbers(18) == (std::vector<long long>{1, 2}));

    // Test 4: c=10 -> y=1 gives x=1 (9*1+1=10), y=10 not a digit, no others -> {1}
    assert(findYogicNumbers(10) == (std::vector<long long>{1}));

    // Test 5: c=2 -> no solution because no digit y makes 9*x+y=2 for non-negative x (x=0 gives y=2, but 9*0+2=2? Wait y=2 is a digit, but 9*0=0, c-y=2, not divisible by 9, so invalid) -> empty
    assert(findYogicNumbers(2).empty());

    // Test 6: c=100 -> solutions? For y=1, c-y=99, 99%9=0, x=11; y=10 invalid; y=0 gives 100 not divisible by 9; y=2..9? 100-y divisible by 9? 100-1=99 divisible, 100-0=100 no, 100-2=98 no, ... 100-9=91 no -> only x=11
    assert(findYogicNumbers(100) == (std::vector<long long>{11}));

    // Test 7: large c, c=999999999999999999 (10^18-1). y=0 gives x=111111111111111111; y=9 gives c-y=999999999999999990, /9=111111111111111110. So expected {111111111111111110, 111111111111111111} sorted.
    long long big = 999999999999999999LL;
    std::vector<long long> expected = {111111111111111110LL, 111111111111111111LL};
    assert(findYogicNumbers(big) == expected);

    // Test 8: c=0 (though positive per problem, test edge) -> y=0 gives x=0 -> {0}
    assert(findYogicNumbers(0) == (std::vector<long long>{0}));

    // Test 9: c=8 -> y=8 gives remainder 0, x=0 -> {0}. y=8 is valid digit.
    assert(findYogicNumbers(8) == (std::vector<long long>{0}));

    // Test 10: c=81 -> y=0 gives x=9, y=9 gives x=8 -> {8,9}
    assert(findYogicNumbers(81) == (std::vector<long long>{8, 9}));

    return 0;
}
// The problem reduces to solving `9*x + y = c` for integers `x >= 0` and digits `y` in `[0,9]`. For a given `c`, we can iterate over all possible `y` from 0 to 9. For each `y`, compute `x = (c - y) / 9` only if `(c - y) >= 0` and `(c - y) % 9 == 0`. If both hold, then `x` is a valid solution. This directly follows from the code snippet: it iterates `y` from 0 to 9, checks `x1 = c - y` positive and divisible by 9, then derives `x = x1 / 9`. The original snippet also appends `10*x + y` which is actually a different problem, but here we are asked to return `x` itself. Edge cases: when `c` is small (e.g., `c=0` but problem says positive, but handle anyway), `c=1` gives `y=1` and `x=0` valid. For `c` values where no divisibility fits, return empty. Since `y` has only 10 possibilities, the time complexity is O(1) (constant) because we iterate over at most 10 digits. Space complexity is O(number of solutions) which is at most 10 (but usually fewer). Sorting is trivial since we collect in increasing order of `y`, but since `x` increases as `y` decreases (because `x = (c-y)/9`), the natural iteration order is not sorted, so we sort the vector before returning. The maximum `c` is up to 10^18, so use `long long`. No overflow issues because `9*x` is at most `c` which fits in `long long`. The approach is straightforward and efficient.
