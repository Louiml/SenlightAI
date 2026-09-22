You are given `n` bus companies, each operating on a route between two cities. Every company charges a fixed rate per kilometer, but they all travel the same distance `m` kilometers. For company `i`, you know its rate per kilometer `a` (in some currency) and the number of kilometers it can travel per liter of fuel `b`. The total cost for company `i` is `(a / b) * m` currency units. Write a C++ function `double minBusCost(int n, int m, const std::vector<std::pair<int,int>>& rates)` that, given the number of companies `n`, the common distance `m`, and a vector of pairs where each pair contains `a` (rate per km in currency) and `b` (km per liter), returns the minimum total cost across all companies as a double. The input is guaranteed to have at least one company, and all `a` and `b` are positive integers.

// The problem is a straightforward minimization over a set of computed values. For each company `i`, compute the total cost as `(double)a / b * m`. Since `a`, `b`, and `m` are integers, dividing `a` by `b` as integers would truncate toward zero, so we must cast one operand to `double` before division to preserve precision. Initialize the answer to a very large value (e.g., `1e100`) and update it with the minimum of the current answer and each computed cost. Because all inputs are positive, division is well-defined (no division by zero), and the result is always positive. Edge cases: only one company, then the answer is simply that company's cost; identical costs across companies does not affect the minimum. Complexity: `O(n)` time (one pass over the vector) and `O(1)` extra space (excluding input storage). The output should be formatted with 12 decimal places when printed, but the function returns a `double` directly.

#include <vector>
#include <algorithm>
#include <cstddef>

// Return the minimum total cost for the bus companies.
// Each company has (rate per km, km per liter). Distance is common.
double minBusCost(int n, int m, const std::vector<std::pair<int,int>>& rates) {
    double best = 1e100; // large initial value
    for (int i = 0; i < n; ++i) {
        // Use double division to avoid integer truncation
        double cost = static_cast<double>(rates[i].first) / rates[i].second * m;
        best = std::min(best, cost);
    }
    return best;
}

#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

int main() {
    // Simple case: two companies, second is cheaper
    std::vector<std::pair<int,int>> r1 = {{10, 5}, {3, 2}};
    assert(std::fabs(minBusCost(2, 10, r1) - 15.0) < 1e-9); // 10/5*10=20, 3/2*10=15

    // Single company
    std::vector<std::pair<int,int>> r2 = {{7, 4}};
    assert(std::fabs(minBusCost(1, 8, r2) - 14.0) < 1e-9); // 7/4*8=14

    // All same cost
    std::vector<std::pair<int,int>> r3 = {{5, 1}, {10, 2}, {15, 3}};
    assert(std::fabs(minBusCost(3, 4, r3) - 20.0) < 1e-9); // each gives 20

    // First is cheapest
    std::vector<std::pair<int,int>> r4 = {{1, 10}, {2, 5}, {4, 2}};
    assert(std::fabs(minBusCost(3, 100, r4) - 10.0) < 1e-9); // 1/10*100=10, others bigger

    // Non-divisible result
    std::vector<std::pair<int,int>> r5 = {{1, 3}, {2, 3}};
    assert(std::fabs(minBusCost(2, 5, r5) - (5.0/3.0)) < 1e-9); // 1/3*5 ≈ 1.6667

    // Large values
    std::vector<std::pair<int,int>> r6 = {{1000000, 1}, {1, 1000000}};
    assert(std::fabs(minBusCost(2, 1000000, r6) - 1.0) < 1e-9); // 1/1e6*1e6=1

    // n=0 not allowed, but test with n=1 already done
    // Extra check: ensure no integer overflow in intermediate double
    std::vector<std::pair<int,int>> r7 = {{1000000000, 1}, {1, 1000000000}};
    double result = minBusCost(2, 1000000000, r7);
    assert(result > 0 && result < 1e18); // sanity

    // Multiple companies with same minimum
    std::vector<std::pair<int,int>> r8 = {{4, 2}, {6, 3}, {8, 4}};
    assert(std::fabs(minBusCost(3, 10, r8) - 20.0) < 1e-9); // all 20

    // Test with n=4 and a clear minimum
    std::vector<std::pair<int,int>> r9 = {{1, 1}, {2, 1}, {3, 1}, {4, 1}};
    assert(std::fabs(minBusCost(4, 7, r9) - 7.0) < 1e-9); // first is cheapest

    // Test with no exact double representation issue
    std::vector<std::pair<int,int>> r10 = {{3, 7}, {5, 11}};
    double cost1 = 3.0/7.0 * 100;
    double cost2 = 5.0/11.0 * 100;
    double expected = cost1 < cost2 ? cost1 : cost2;
    assert(std::fabs(minBusCost(2, 100, r10) - expected) < 1e-9);

    return 0;
}
