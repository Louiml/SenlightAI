Write a C++ function named `champagneTowerVolume` that simulates pouring `poured` cups of champagne into a tower of glasses arranged in rows, where row 0 has 1 glass, row 1 has 2 glasses, row 2 has 3 glasses, and so on. When champagne is poured into the top glass, each glass holds exactly 1 cup; any excess over 1 cup flows equally to the two glasses directly below it (left and right). Given integers `poured` (total cups poured from the top, non-negative), `query_row` (0-indexed row number, non-negative), and `query_glass` (0-indexed glass position in that row, must be valid: 0 ≤ `query_glass` ≤ `query_row`), return the amount of champagne (a `double`) that ends up in that specific glass, capped at 1.0 if the glass is full. The function must handle `poured` up to 10^9 and `query_row` up to 100.

The problem can be solved by simulating the flow of champagne row by row. We maintain a one-dimensional array `current` representing the amount of champagne in each glass of the current row. Initially, the only glass at row 0 has `poured` cups. For each row from 0 up to `query_row - 1`, we create a new array `next` for the next row (of size one larger) initialized to zero. For each glass `j` in the current row, if its amount exceeds 1.0, the excess `(current[j] - 1.0)` is split equally: half goes to `next[j]` and half goes to `next[j+1]` (the left and right glasses below). After processing all glasses in the current row, we replace `current` with `next` and continue. This works because champagne can only flow downward, and each glass's overflow depends only on glasses above it. Important edge cases: if `poured` is 0, the top glass and all below are empty; if a glass receives less than or equal to 1 cup, no overflow occurs (use `max(0.0, ...)` to avoid negative overflow values due to floating‑point precision); and if `query_row` is 0, we return `min(poured, 1.0)` directly. Time complexity is O(query_row^2) because we iterate over all glasses up to the queried row, which is at most 100 rows, so this is effectively O(1) given constraints. Space complexity is O(query_row) for the arrays (bounded by 101 elements).

#include <vector>
#include <algorithm>

// Simulate pouring champagne and return the amount in the specified glass.
double champagneTowerVolume(int poured, int query_row, int query_glass) {
    if (query_row == 0) {
        return std::min(static_cast<double>(poured), 1.0);
    }
    
    // current row amounts (row 0 initially)
    std::vector<double> current(1, static_cast<double>(poured));
    
    for (int row = 0; row < query_row; ++row) {
        // next row has one more glass
        std::vector<double> next(row + 2, 0.0);
        for (int j = 0; j < static_cast<int>(current.size()); ++j) {
            if (current[j] > 1.0) {
                double overflow = (current[j] - 1.0) / 2.0;
                next[j] += overflow;
                next[j + 1] += overflow;
            }
        }
        current = std::move(next);
    }
    
    double amount = current[query_glass];
    return std::min(amount, 1.0);
}

#include <cassert>
#include <cmath>

int main() {
    // Basic cases
    assert(std::fabs(champagneTowerVolume(1, 0, 0) - 1.0) < 1e-9);
    assert(std::fabs(champagneTowerVolume(1, 1, 0) - 0.0) < 1e-9);
    assert(std::fabs(champagneTowerVolume(2, 1, 0) - 0.5) < 1e-9);
    assert(std::fabs(champagneTowerVolume(2, 1, 1) - 0.5) < 1e-9);
    
    // Full glass at top, overflow to second row
    assert(std::fabs(champagneTowerVolume(3, 1, 1) - 1.0) < 1e-9);
    assert(std::fabs(champagneTowerVolume(3, 2, 0) - 0.0) < 1e-9);
    assert(std::fabs(champagneTowerVolume(3, 2, 1) - 0.25) < 1e-9);
    
    // Large pour, middle glasses full
    assert(std::fabs(champagneTowerVolume(10, 3, 1) - 1.0) < 1e-9);
    assert(std::fabs(champagneTowerVolume(10, 3, 2) - 1.0) < 1e-9);
    
    // Edge: zero poured
    assert(std::fabs(champagneTowerVolume(0, 5, 2) - 0.0) < 1e-9);
    
    // Edge: deep row with small pour
    assert(std::fabs(champagneTowerVolume(4, 2, 1) - 0.5) < 1e-9);
    
    // Edge: query_row 0 with poured > 1
    assert(std::fabs(champagneTowerVolume(100, 0, 0) - 1.0) < 1e-9);
    
    return 0;
}
