/*
Write a C++ function `int minimumCost(int n, int one, int two)` that computes the minimum total cost to buy exactly `n` items when a single item costs `one` units and a bundle of two items costs `two` units. You may purchase any combination of single items and two-item bundles, and the order of purchases does not matter. The function must return the lowest possible total cost. For example, if `n = 5`, `one = 3`, and `two = 5`, the cheapest way is to buy two bundles (4 items for 10) plus one single (3) for a total of 13, because buying one bundle plus three singles costs 5+9=14. Handle all non-negative integers, including `n = 0` (cost 0), and ensure the function works efficiently for large `n` (up to 10^9).
*/
#include <algorithm>

// Returns the minimum total cost to buy exactly n items
// when a single item costs one and a two-item bundle costs two.
int minimumCost(int n, int one, int two) {
    if (n <= 0) {
        return 0;
    }
    if (2 * one >= two) {
        // Prefer bundles, buy as many as possible, then pay for a single if odd.
        int cost = (n / 2) * two;
        if (n % 2 != 0) {
            cost += one;
        }
        return cost;
    } else {
        // Two singles are cheaper than a bundle, so buy all singles.
        return n * one;
    }
}
#include <cassert>

int minimumCost(int n, int one, int two); // declaration from solution

int main() {
    assert(minimumCost(0, 5, 10) == 0);
    assert(minimumCost(1, 5, 10) == 5);
    assert(minimumCost(2, 5, 10) == 10); // 2 singles = 10, bundle = 10, same
    assert(minimumCost(3, 5, 10) == 15); // bundle + single = 15
    assert(minimumCost(5, 3, 5) == 13); // two bundles + one single
    assert(minimumCost(4, 3, 5) == 10); // two bundles
    assert(minimumCost(4, 2, 100) == 8); // all singles
    assert(minimumCost(7, 10, 15) == 55); // 3 bundles + one single = 45+10
    assert(minimumCost(2, 1, 1) == 1); // one bundle beats two singles
    assert(minimumCost(1, 0, 5) == 0); // free single
    return 0;
}
// The optimal strategy is to prefer two-item bundles whenever buying two single items would cost at least as much as one bundle. Compare the cost of two single items (`2 * one`) against the bundle price (`two`). If `2 * one >= two`, buying bundles is at least as good as two singles, so purchase as many full bundles as possible: `(n / 2)` bundles, costing `(n / 2) * two`. If there is an odd remaining item (`n % 2 == 1`), buy one single item costing `one`. If `2 * one < two`, then buying two singles is cheaper than one bundle, so never use bundles; instead, buy all items as singles, costing `n * one`. However, note: when `2 * one >= two`, the single leftover for an odd `n` must still be bought as one single (because you cannot buy half a bundle). Edge cases: `n = 0` returns 0; `one = 0` or `two = 0` work naturally; if `two` is much larger than `2 * one`, the no-bundle path is chosen. This is a greedy constant-time solution: main algorithm is a few arithmetic operations and conditionals. Time complexity: O(1). Space complexity: O(1).
