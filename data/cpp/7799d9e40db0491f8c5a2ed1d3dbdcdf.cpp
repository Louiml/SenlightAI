// Write a C++ function `powerfulIntegers(int x, int y, int bound)` that returns a `std::vector<int>` containing all distinct integers of the form `x^i + y^j` that are **less than or equal to** `bound`, where `i` and `j` are non‑negative integers (0, 1, 2, …). The returned vector may be in any order and must not contain duplicates. The function should correctly handle edge cases where `x == 1` or `y == 1`, as powers of 1 repeat forever (1^0 = 1, 1^1 = 1, …). The inputs `x` and `y` are positive integers, and `bound` is a non‑negative integer. For example, for `x = 2, y = 3, bound = 10`, the valid sums are: 1+1=2, 1+3=4, 1+9=10, 2+1=3, 2+3=5, 2+9=11 (too large), 4+1=5, 4+3=7, 8+1=9 → distinct: {2,3,4,5,7,9,10}.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic example
    auto v1 = powerfulIntegers(2, 3, 10);
    std::vector<int> expected1 = {2, 3, 4, 5, 7, 9, 10};
    std::sort(v1.begin(), v1.end());
    assert(v1 == expected1);

    // x = 1, y = 1: only 1+1=2 if bound >= 2
    auto v2 = powerfulIntegers(1, 1, 2);
    assert(v2 == std::vector<int>({2}));

    // x = 1, y = 2, bound = 5: powers of 1 are 1, powers of 2 are 1,2,4
    // sums: 1+1=2, 1+2=3, 1+4=5
    auto v3 = powerfulIntegers(1, 2, 5);
    std::sort(v3.begin(), v3.end());
    assert(v3 == std::vector<int>({2, 3, 5}));

    // bound = 0: no sums
    auto v4 = powerfulIntegers(2, 2, 0);
    assert(v4.empty());

    // Large x, small bound
    auto v5 = powerfulIntegers(100, 2, 100);
    std::sort(v5.begin(), v5.end());
    assert(v5 == std::vector<int>({2, 3, 5, 9, 17, 33, 65, 101})); // 101 > bound so not included? actually 100+1=101 > 100, check: 1+1=2, 1+2=3, 1+4=5, 1+8=9, 1+16=17, 1+32=33, 1+64=65, 100+1=101 too large, so only 7? Let's recalc: max xi=1 (since 100>bound), so xi=1 only. y powers: 1,2,4,8,16,32,64. sums: 2,3,5,9,17,33,65. Actually not 101. So expected: {2,3,5,9,17,33,65}
    std::vector<int> expected5 = {2,3,5,9,17,33,65};
    std::sort(v5.begin(), v5.end());
    assert(v5 == expected5);

    // Duplicate sums: x=2, y=2, bound=10: sums: 1+1=2, 1+2=3, 1+4=5, 1+8=9, 2+1=3 (dup), 2+2=4, 2+4=6, 2+8=10, 4+1=5 (dup), 4+2=6 (dup), 4+4=8, 8+1=9 (dup), 8+2=10 (dup) → distinct {2,3,4,5,6,8,9,10}
    auto v6 = powerfulIntegers(2, 2, 10);
    std::sort(v6.begin(), v6.end());
    assert(v6 == std::vector<int>({2,3,4,5,6,8,9,10}));
}

#include <vector>
#include <unordered_set>

// Return all distinct integers of the form x^i + y^j <= bound.
std::vector<int> powerfulIntegers(int x, int y, int bound) {
    std::unordered_set<int> result;
    
    for (int xi = 1; xi <= bound; xi *= x) {
        for (int yj = 1; xi + yj <= bound; yj *= y) {
            result.insert(xi + yj);
            if (y == 1) {
                break;
            }
        }
        if (x == 1) {
            break;
        }
    }
    
    return std::vector<int>(result.begin(), result.end());
}

// The solution iterates over all possible powers of `x` (starting from 1, i.e., x^0) up to `bound`, and for each such power `xi`, iterates over all possible powers of `y` (starting from 1) while `xi + yj <= bound`. Each sum is inserted into an `unordered_set` to automatically remove duplicates. A key edge case is `x == 1`: since 1^i is always 1, we must break the outer loop after the first iteration (i=0) to avoid infinite looping. Similarly, if `y == 1`, then 1^j is always 1, so we break the inner loop after the first iteration. The loops multiply the current power by `x` (or `y`) each time; for `x > 1`, the number of iterations is `O(log_x(bound))`, and similarly for `y`. Because `bound` is small (typically ≤ 10^6), this is efficient. Time complexity is `O(log_x(bound) * log_y(bound))` in the worst case, and space complexity is `O(number of distinct sums)`.
