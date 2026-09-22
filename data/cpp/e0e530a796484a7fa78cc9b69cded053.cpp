Write a C++ function `int chooseColumn(const std::vector<int>& dispenseOrder, std::vector<int>& dispenseCount, int numDispensed)` that simulates selecting a column number (1 to 7 inclusive) from a dispenser. If `dispenseOrder[numDispensed]` is non-zero, that column is chosen directly. If it is zero, choose a random column between 1 and 7, but skip any column whose count in `dispenseCount` (indexed 0–6 for columns 1–7) is already at or above the capacity of 5. After selection, increment the corresponding count in `dispenseCount` and return the chosen column. Assume `dispenseOrder` is large enough to index `numDispensed`, `dispenseCount` has exactly 7 elements, and no column can exceed capacity 5 (so the function must ensure it never returns a column at capacity when picking randomly). Use a uniform random distribution for the random selection.
The solution must handle two cases: a forced column (when `dispenseOrder[numDispensed]` is non-zero) and a random column (when it is zero). For the forced case, we just use that value directly, but we must increment the count and return it. For the random case, we need to randomly select from columns that are not yet at capacity (i.e., those with `dispenseCount[col-1] < 5`). A straightforward rejection sampling approach works: repeatedly generate a random integer from 1 to 7, check if that column is available, and if not, try again. Since there are at most 7 possible columns and at any time at least one column must be available (because we never exceed 5 per column and total capacity is 7*5=35, but numDispensed is conceptually less than that), rejection sampling will terminate quickly, in expected constant time. Edge cases: when `dispenseOrder` specifies a column that is already at capacity (this is possible if the test data is inconsistent) — the specification does not forbid this, so we should still honor it (increment and return, even if it exceeds capacity? Better to follow spec exactly: it says choose that column directly, so we increment without checking capacity; the test will avoid such invalid inputs). Time complexity is O(1) expected for random selection, O(1) for forced, and space is O(1) beyond the input vectors. For a loop of n selections, total O(n).
#include <vector>
#include <random>

// Choose a column (1..7) based on dispenseOrder; if zero, pick a random column not at capacity.
// dispenseCount tracks how many items each column has dispensed (size 7, indices 0-6 for columns 1-7).
// numDispensed is the index into dispenseOrder for the current selection.
int chooseColumn(const std::vector<int>& dispenseOrder, std::vector<int>& dispenseCount, int numDispensed) {
    constexpr int kNumColumns = 7;
    constexpr int kMaxPerColumn = 5;

    int chosen = dispenseOrder[numDispensed];
    if (chosen == 0) {
        // Randomly select a column that still has capacity (count < 5).
        static std::mt19937 rng(42); // fixed seed for reproducibility
        std::uniform_int_distribution<int> dist(1, kNumColumns);
        do {
            chosen = dist(rng);
        } while (dispenseCount[chosen - 1] >= kMaxPerColumn);
    }

    // Increment the count for the chosen column.
    dispenseCount[chosen - 1]++;
    return chosen;
}
#include <cassert>
#include <vector>

// The solution function is included here for completeness (already defined above).
// Test cases for chooseColumn.

int main() {
    // Test forced columns.
    std::vector<int> order = {3, 0, 5, 0, 1};
    std::vector<int> counts(7, 0);
    assert(chooseColumn(order, counts, 0) == 3);
    assert(counts[2] == 1);

    // Random selection when order is 0: all columns empty, should return a valid 1..7 and increment.
    int result = chooseColumn(order, counts, 1);
    assert(result >= 1 && result <= 7);
    assert(counts[result-1] == 1);

    // Force another column.
    assert(chooseColumn(order, counts, 2) == 5);
    assert(counts[4] == 1);

    // Random selection again, still all columns under capacity.
    result = chooseColumn(order, counts, 3);
    assert(result >= 1 && result <= 7);
    assert(counts[result-1] >= 1);

    // Force column 1.
    assert(chooseColumn(order, counts, 4) == 1);
    assert(counts[0] == 1);

    // Test capacity enforcement: fill column 2 to 5, then force a random selection that skips it.
    std::vector<int> order2 = {0, 0, 0};
    std::vector<int> counts2(7, 0);
    // Manually fill column 2 to capacity.
    for (int i = 0; i < 5; ++i) counts2[1] = i+1; // just set to 5
    counts2[1] = 5;
    // First random selection should not pick column 2.
    int r = chooseColumn(order2, counts2, 0);
    assert(r != 2);
    assert(counts2[r-1] == 1);
    // Second random selection still shouldn't pick column 2 unless forced (it won't be).
    r = chooseColumn(order2, counts2, 1);
    assert(r != 2);
    assert(counts2[r-1] == 2 || counts2[r-1] == 1); // Either same column twice or different
}
