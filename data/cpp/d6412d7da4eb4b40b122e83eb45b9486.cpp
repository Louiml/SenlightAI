/*
Write a C++ function named `calculateProducts` that takes three integer parameters: `timePerItem` (the time needed to produce one item), `itemsPerBatch` (the number of items produced per completed time cycle), and `totalTime` (the total available time). The function must return the total number of items that can be produced if the production process can only produce `itemsPerBatch` items after every full `timePerItem` time units have elapsed, and the last partial cycle (if any) does not yield any items. All inputs are positive integers (≥1). For example, if it takes 4 minutes to produce 3 items, and you have 10 minutes, you can complete 2 full cycles (8 minutes) producing 6 items, but the remaining 2 minutes cannot start a new cycle. The function should handle cases where `totalTime` is less than `timePerItem`, returning 0.
*/
#include <cstdint>  // for int64_t

// Calculate total items produced given the time needed per cycle,
// items per cycle, and total available time.
// Returns 0 if fewer than one full cycle can be completed.
std::int64_t calculateProducts(int timePerItem, int itemsPerBatch, int totalTime) {
    // Compute number of complete cycles using integer division.
    const std::int64_t completeCycles = static_cast<std::int64_t>(totalTime) / timePerItem;
    // Multiply by items per batch to get total production.
    const std::int64_t totalItems = completeCycles * itemsPerBatch;
    return totalItems;
}
#include <cassert>
#include <cstdint>

int main() {
    // Basic case: 10 minutes, 4 per cycle, 3 items -> 2 cycles => 6 items
    assert(calculateProducts(4, 3, 10) == 6);
    // Exact multiple: 12 minutes, 4 per cycle, 3 items -> 3 cycles => 9 items
    assert(calculateProducts(4, 3, 12) == 9);
    // Less than one full cycle: 3 minutes, 4 per cycle, 1 item -> 0 cycles => 0
    assert(calculateProducts(4, 1, 3) == 0);
    // One item per cycle: 5 minutes, 1 per cycle, 7 minutes -> 1 cycle => 1
    assert(calculateProducts(5, 1, 7) == 1);
    // Large numbers: 1000 per cycle, 2 minutes, 1000000 minutes -> 500000 cycles => 500,000,000
    assert(calculateProducts(2, 1000, 1000000) == 500000000LL);
    // Boundary: time equals timePerItem exactly -> 1 cycle
    assert(calculateProducts(7, 5, 7) == 5);
    // All ones: 1 minute per cycle, 1 item, 1 minute -> 1
    assert(calculateProducts(1, 1, 1) == 1);
    // Long long overflow check: timePerItem=1, itemsPerBatch=2000000000, totalTime=2000000000 -> 4e18 fits in int64
    assert(calculateProducts(1, 2000000000, 2000000000) == 4000000000000000000LL);
    // No cycles with small total time
    assert(calculateProducts(100, 50, 99) == 0);
}
// The core logic is a simple integer division followed by multiplication: the number of complete production cycles is `totalTime / timePerItem` (integer division truncates any remainder), and each cycle yields `itemsPerBatch` items. Multiplying these gives the total output. Edge cases: (1) when `totalTime` is smaller than `timePerItem`, the division yields 0, so the result is 0 — this is naturally handled by integer division; (2) very large inputs may overflow the result if using `int`, so the function should use `long long` internally to be safe, though the parameters themselves can remain `int`. The time complexity is O(1) and space complexity is O(1) because only a single multiplication and division are performed.
