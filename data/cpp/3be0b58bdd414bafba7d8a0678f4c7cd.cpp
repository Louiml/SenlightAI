/*
Write a C++ function `std::string averageQuotientOperation(int n, int k, int p, const std::vector<int>& heights)` that simulates the core computation from the given snippet but in a simplified, self-contained form. You are given `n` numbers, `k` operations, and a precision parameter `p`. The first number is a "base" value. From the remaining numbers, filter out any that are ≤ the base, then sort the remaining ascending. If fewer than `k` remain, set `k` to the count. You may perform at most `k` operations, where each operation takes a contiguous block of the sorted remaining numbers (block length ≥ 1) and replaces the entire block by the average of its elements (computed with high precision). The goal is to maximize the final value of the first element after applying operations left-to-right, where each operation also averages the current first element with the block’s average (i.e., the first element becomes `(first + block_average)/2`). However, note that a block can be of length 1, which simply averages the first with that single number. You must output the final first element as a string with exactly `2*p` digits after the decimal point, rounded? Actually, the original prints with `p*2` after decimal point, but for simplicity we’ll use `to_string(int)` from a provided Decimal-like implementation. Since implementing full Decimal is heavy, for this task we require using a custom fixed‑point representation with arbitrary precision? To keep it testable, we’ll restrict to integer inputs and ensure the final value can be computed exactly as a rational number with denominator being a power of 2? Actually the operations are averaging, so denominator becomes product of powers of 2 and lengths. To avoid floating point, we can use a simple 64‑bit integer numerator/denominator? But lengths up to 8000 and up to 14 operations could overflow. For a standalone task, we can simplify: you are given `n` (≤10), `k` (≤5), `p` (≤3), and the heights are integers ≤1000. Write a function that returns the maximum achievable value of the first element after applying exactly `k` operations (each operation averages the current first with the average of a chosen contiguous subarray of the current sorted remaining numbers; after an operation, the chosen block is removed from the remaining list). The operations are applied sequentially. Return the result as a string with `p` digits after the decimal point, rounded to nearest. Use double precision for computation; the test cases will have tolerances. The function should handle edge cases: if the filtered list is empty, return the base as a string. If `k` is larger than the filtered list size, cap `k` accordingly. The order of operations matters: you can choose blocks adaptively. This is a small search problem.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>

// Compute maximum achievable first value after at most k averaging operations.
// heights[0] is base, others are filtered/sorted.
std::string averageQuotientOperation(int n, int k, int p, const std::vector<int>& heights) {
    if (n == 0) return "0." + std::string(p, '0');
    double base = static_cast<double>(heights[0]);

    std::vector<int> filtered;
    for (int i = 1; i < n; ++i) {
        if (heights[i] > heights[0]) filtered.push_back(heights[i]);
    }
    std::sort(filtered.begin(), filtered.end());
    int m = static_cast<int>(filtered.size());
    if (k > m) k = m;

    int bestMask = (1 << m) - 1; // all selected
    double bestVal = base;

    // Recursive function: current value, current list as a vector of indices (in original filtered array)
    // but we can pass a bitmask of remaining indices.
    std::function<void(double, int, int)> dfs = [&](double cur, int mask, int opsLeft) {
        // Update best
        if (cur > bestVal) bestVal = cur;
        if (opsLeft == 0) return;

        // Build the current list in original order
        std::vector<int> list;
        for (int i = 0; i < m; ++i) if (mask & (1 << i)) list.push_back(i);

        int sz = static_cast<int>(list.size());
        // Try every contiguous subarray of the current list
        for (int L = 0; L < sz; ++L) {
            double sum = 0;
            for (int R = L; R < sz; ++R) {
                sum += filtered[list[R]];
                int blockLen = R - L + 1;
                double avg = sum / blockLen;
                double newCur = (cur + avg) / 2.0;
                // Remove this block from the mask
                int newMask = mask;
                for (int idx = L; idx <= R; ++idx) {
                    newMask &= ~(1 << list[idx]);
                }
                dfs(newCur, newMask, opsLeft - 1);
            }
        }
    };

    dfs(base, bestMask, k);

    // Format to p decimal places, rounding
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(p) << bestVal;
    return oss.str();
}

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

// Forward declaration of the function being tested
std::string averageQuotientOperation(int n, int k, int p, const std::vector<int>& heights);

// Helper to parse a fixed-point string to double for comparison
double parseDouble(const std::string& s) {
    return std::stod(s);
}

int main() {
    // Case 1: base=5, filtered [6,7], k=1, best to average with 7 -> (5+7)/2=6.0
    assert(parseDouble(averageQuotientOperation(3, 1, 2, {5, 6, 7})) == 6.0);

    // Case 2: base=10, filtered [11,12,13], k=2. Best: average with 13 first -> (10+13)/2=11.5, then with 12 -> (11.5+12)/2=11.75
    assert(std::abs(parseDouble(averageQuotientOperation(4, 2, 3, {10, 11, 12, 13})) - 11.75) < 1e-6);

    // Case 3: base=1, filtered [] -> returns 1.000
    assert(averageQuotientOperation(1, 3, 3, {1}) == "1.000");

    // Case 4: base=2, filtered [3], k=5 (capped to 1) -> (2+3)/2=2.5
    assert(averageQuotientOperation(2, 5, 1, {2, 3}) == "2.5");

    // Case 5: base=4, filtered [5,6,7], k=0 -> 4.0
    assert(averageQuotientOperation(4, 0, 1, {4, 5, 6, 7}) == "4.0");

    // Case 6: base=8, filtered [9,10,11,12], k=2. Optimal: average with 12 -> (8+12)/2=10, then with 11 -> (10+11)/2=10.5
    assert(std::abs(parseDouble(averageQuotientOperation(5, 2, 2, {8, 9, 10, 11, 12})) - 10.5) < 1e-6);

    // Case 7: base=3, filtered [4], k=1 -> 3.5
    assert(averageQuotientOperation(2, 1, 1, {3, 4}) == "3.5");

    // Case 8: base=1, filtered [2,2,2], k=3 -> after each average with 2, final is (1+2)/2=1.5 then (1.5+2)/2=1.75 then (1.75+2)/2=1.875
    assert(std::abs(parseDouble(averageQuotientOperation(4, 3, 4, {1, 2, 2, 2})) - 1.875) < 1e-6);

    return 0;
}

// This is a combinatorial optimization. We have a base value `base = heights[0]`. From the remaining values, we filter out those ≤ base, sort ascending. Let the resulting array be `a` of length `m`. We need to apply exactly `k` operations (with `k` capped to `m` if necessary). Each operation: choose a contiguous subarray of the current `a` (which is a subset of the original, but since we remove blocks, the remaining elements preserve their original relative order after deletions? Actually we remove the chosen block, so the remaining list shrinks without reordering). The operation computes `avg = mean(block)` and updates the current value `cur` of the first element (which is the base initially) to `(cur + avg)/2`. The block is then removed from the list. We can choose blocks adaptively after each removal. The goal is to maximize the final `cur`.
//
// Because `m` ≤ 9 and `k` ≤ 5 in our test constraints, we can brute-force all ways to choose a sequence of blocks. However, a block’s removal changes the adjacency. But since we remove blocks, the remaining elements are still in original order; any contiguous subarray in the current list is a contiguous subarray of the original list after some deletions. This is equivalent to partitioning the original sorted list into at most `k` blocks (some possibly empty? No, each block must be non-empty) that we process in some order. But the order of processing matters because the current value changes. So we need to decide the sequence of blocks and their order. Since `m` ≤ 9, we can use recursion: at each step, choose a contiguous subarray from the current list (which is a subset of indices of original, but we can track the current list). The number of states is limited. For each state (current value as double, current list as a bitmask of original indices), we try all possible contiguous segments in the current list left-to-right order. The complexity is exponential but fine for small sizes.
//
// Precision: Use double, and round to `p` digits after decimal when formatting. Since inputs are small integers and operations are just averages, double is enough for tolerance. Edge cases: if `m == 0`, return `base` formatted to `p` decimals. If `k == 0`, return base. Also, it's always optimal to use all `k` operations? Possibly not, because averaging with a block could lower the value if the block’s average is lower than current. So we should consider using fewer than `k` operations as well. So we try all possible `j` from 0 to `k` operations, and take the maximum final value. In the recursion, we allow stopping early. The output string should be formatted with `p` digits after decimal, using rounding (e.g., `std::fixed << std::setprecision(p)`). Time complexity is exponential in `m` and `k`, but with `m≤9` and `k≤5`, worst-case states are manageable; space O(m) for recursion.
