You are given a recipe string `s` consisting only of the characters 'B', 'S', and 'C', which denote the required quantities of bread, sausage, and cheese for making one burger. You currently have `nb` breads, `ns` sausages, and `nc` cheeses in your kitchen. The prices for buying one bread, one sausage, and one cheese are `pb`, `ps`, and `pc` respectively. You have `r` rubles to spend. You can buy any number of additional ingredients at these prices, and you can also use the ingredients you already have. You may only make whole burgers, and each burger requires exactly the number of each ingredient specified by the recipe string. Write a C++ function `maxBurgers` that takes the recipe string `s`, the integers `nb, ns, nc, pb, ps, pc, r` (all non‑negative, with `r` and the initial ingredient counts possibly up to `10^14` and the budget `r` up to `10^12`), and returns the maximum number of burgers you can make. The recipe string length is at most 100. Note that if a recipe does not use an ingredient (e.g., 'B' only), you do not need to buy that ingredient at all. Your solution must handle large numbers and must not use floating‑point arithmetic.
The problem asks for the maximum integer `x` such that you can make `x` burgers. For a given `x`, the total required amount of each ingredient is `count * x`, where `count` is the number of times that ingredient appears in the recipe. The amount you need to buy is `max(0, required - current)`. The total cost to buy the shortfall is `cost(x) = pb * max(0, rb*x - nb) + ps * max(0, rs*x - ns) + pc * max(0, rc*x - nc)`. This cost is a non‑decreasing function of `x` (since the required amounts increase linearly). Therefore, we can binary search on `x`. The lower bound is 0 (always possible), and an upper bound can be set to a very large number such as `10^14 + 10^12` (or simply `1e15` as in the original code) because the recipe length is at most 100, so even if you buy all ingredients, the cost per burger is at most `(pb+ps+pc)*100 ≤ 3e14` and with a budget of `1e12` you cannot make more than `1e12` burgers if you need to buy everything, but the safe upper bound `1e15` works because binary search runs in O(log N) where N is the upper bound. For each `mid`, compute `cost(mid)` using 64‑bit integers to avoid overflow: each term is at most `pb * (100 * 1e15)` which is `1e5 * 1e17 = 1e22` which overflows 64‑bit. However, we can cap the cost check early: if cost exceeds `r` (which is at most `1e12`), we can stop computing and return false. To avoid overflow, compute each term using `long long` and check if the term exceeds `r` before adding. Since `r ≤ 1e12`, we can safely do: if `rb > 0` and `mid > (nb + r / pb) / rb` then the cost already exceeds `r` (since even buying one of each needed ingredient would exceed the budget), but a simpler robust approach is to use `__int128` for the intermediate cost calculation, which easily handles up to 1e22. The binary search converges in about 50 iterations. Edge cases: if the recipe uses no ingredient of a type, the count is zero, and the corresponding term becomes zero because `max(0, 0*mid - nb) = 0`. Also, if you already have enough of all ingredients for a large `mid`, cost becomes zero and you can increase the answer. The time complexity is O(|s| + log(1e15) * constant) = O(|s| + 60) and space O(1). We recommend using `__int128` for the cost calculation to avoid overflow, and the upper bound `1e15` is safe because the maximum burgers you can make is bounded by `(nb + r/pb + ns + r/ps + nc + r/pc)` which is at most `3e14/1 + 3e12` if prices are 1, but definitely less than `1e15`.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

// Returns the maximum number of burgers that can be made given the recipe and available resources.
ll maxBurgers(const string& s, ll nb, ll ns, ll nc, ll pb, ll ps, ll pc, ll r) {
    // Count required ingredients per burger
    ll rb = 0, rs = 0, rc = 0;
    for (char c : s) {
        if (c == 'B') rb++;
        else if (c == 'S') rs++;
        else if (c == 'C') rc++;
    }

    // Check if making x burgers is affordable
    auto canMake = [&](ll x) -> bool {
        i128 cost = 0;
        if (rb > 0 && x > 0) {
            i128 need = (i128)rb * x;
            i128 have = nb;
            if (need > have) {
                i128 buy = need - have;
                cost += (i128)buy * pb;
            }
        }
        if (rs > 0 && x > 0) {
            i128 need = (i128)rs * x;
            i128 have = ns;
            if (need > have) {
                i128 buy = need - have;
                cost += (i128)buy * ps;
            }
        }
        if (rc > 0 && x > 0) {
            i128 need = (i128)rc * x;
            i128 have = nc;
            if (need > have) {
                i128 buy = need - have;
                cost += (i128)buy * pc;
            }
        }
        return cost <= (i128)r;
    };

    ll lo = 0, hi = 1e15; // safe upper bound
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2; // avoid overflow
        if (canMake(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Assume the solution is defined above (in a real test, include it)

int main() {
    // Basic examples
    assert(maxBurgers("B", 1, 0, 0, 10, 0, 0, 100) == 11); // need 1 bread each, have 1, buy 10 more
    assert(maxBurgers("BS", 2, 1, 0, 1, 2, 0, 10) == 3); // each burger costs 1+2=3, have 2 breads 1 sausage: make 1 with current, buy 2 breads+2 sausages = 2+4=6, total 3 burgers
    assert(maxBurgers("BSC", 0, 0, 0, 1, 1, 1, 5) == 1); // one burger costs 3, can make 1
    assert(maxBurgers("BSC", 10, 10, 10, 1, 1, 1, 1000) == 10); // have enough for 10, no need to buy
    assert(maxBurgers("BSC", 0, 0, 0, 1, 1, 1, 2) == 0); // can't afford even one

    // Large numbers to check overflow
    assert(maxBurgers("B", 0LL, 0LL, 0LL, 1000000000000000LL, 0LL, 0LL, 1000000000000000LL) == 1);
    assert(maxBurgers("B", 1000000000000000LL, 0LL, 0LL, 1LL, 0LL, 0LL, 1000000000000000LL) == 2000000000000000LL);

    // Recipe with zero usage of some ingredient
    assert(maxBurgers("C", 0, 0, 5, 0, 0, 2, 10) == 7); // need 1 cheese, have 5, can buy 5 more => 10 total, but budget 10 buys 5, so 5+5=10

    // Edge: no budget
    assert(maxBurgers("B", 0, 0, 0, 5, 0, 0, 0) == 0);

    // Test that solution handles recipe length > 1
    assert(maxBurgers("BBB", 0, 0, 0, 2, 0, 0, 6) == 1); // each burger needs 3 breads, cost 6 => 1 burger

    cout << "All tests passed!" << endl;
    return 0;
}
