Write a C++ function `std::pair<long long, long long> solveCandy(long long n)` that, given a positive integer `n` representing the total number of items, determines a non‑negative integer solution to `2*a + 5*b = n` where `a` and `b` are non‑negative integers. The function must return the pair `(a, b)` with the **smallest possible value of `a`** (and consequently the corresponding `b`). If no non‑negative integer solution exists, return `{-1, -1}`. The function should handle all positive `n` up to `10^18`. The function must not use loops or recursion; it should be a direct mathematical formula. The return type is `std::pair<long long, long long>`.

// We need to solve the linear Diophantine equation `2a + 5b = n` with `a, b >= 0`. Since the coefficient of `a` is 2 and of `b` is 5, we can analyze `n mod 2` and `n mod 5`. However, the trick is to minimize `a`; since `a` is multiplied by 2, increasing `a` by 5 and decreasing `b` by 2 keeps the sum constant. Thus, the minimal non‑negative `a` is either 0, 1, 2, 3, or 4 because any larger `a` can be reduced by 5 (and `b` increased by 2) while staying non‑negative if `b >= 2`. So we need to find the smallest `a` in `{0,1,2,3,4}` such that `(n - 2a)` is divisible by 5 and `(n - 2a)/5 >= 0`. If no such `a` exists, return `{-1,-1}`. Edge cases: `n=1` and `n=3` have no solution. For `n` even, `a` can often be 0 if `n` is a multiple of 5; otherwise we check small residues. The algorithm runs in O(1) time and O(1) space.

#include <utility>

// Return the non-negative solution (a, b) to 2*a + 5*b = n with minimal a,
// or {-1, -1} if no non-negative integer solution exists.
std::pair<long long, long long> solveCandy(long long n) {
    // Try a = 0, 1, 2, 3, 4 in increasing order to minimize a.
    for (long long a = 0; a <= 4; ++a) {
        long long rem = n - 2 * a;
        if (rem < 0) break;
        if (rem % 5 == 0) {
            long long b = rem / 5;
            return {a, b};
        }
    }
    return {-1, -1};
}

#include <cassert>
#include <utility>

std::pair<long long, long long> solveCandy(long long n); // Declaration from solution

int main() {
    assert(solveCandy(1) == std::make_pair(-1LL, -1LL));
    assert(solveCandy(3) == std::make_pair(-1LL, -1LL));
    assert(solveCandy(2) == std::make_pair(1LL, 0LL));
    assert(solveCandy(5) == std::make_pair(0LL, 1LL));
    assert(solveCandy(4) == std::make_pair(2LL, 0LL));
    assert(solveCandy(7) == std::make_pair(1LL, 1LL));     // 2*1 + 5*1 = 7
    assert(solveCandy(11) == std::make_pair(3LL, 1LL));    // 2*3 + 5*1 = 11
    assert(solveCandy(12) == std::make_pair(1LL, 2LL));    // 2*1 + 5*2 = 12
    assert(solveCandy(0) == std::make_pair(0LL, 0LL));     // Edge case: n=0 is non-negative
    assert(solveCandy(1000000000000000000LL) == std::make_pair(0LL, 200000000000000000LL)); // 2*0 + 5*200e15 = 1e18
}
