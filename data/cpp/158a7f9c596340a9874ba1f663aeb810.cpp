/*
Write a C++ function `biscuitsEaten` that takes three integers: `ovenTime` (the time in minutes required to bake one batch of biscuits), `batchSize` (the number of biscuits produced per batch), and `totalTime` (the total available baking time in minutes). The function must return the total number of biscuits that can be produced during the entire available time, assuming batches are baked only when a full batch’s required time has elapsed (no partial batches) and you may bake as many consecutive batches as fit within the time. If the available time is less than the time needed for one batch, the result should be zero. Implement the solution in a free function with a descriptive name, and ensure it is const-correct and uses only standard headers.
*/

#include <cstdint>

// Returns the total number of biscuits produced given the time per batch,
// biscuits per batch, and total available time. Only full batches are counted.
int64_t biscuitsEaten(int ovenTime, int batchSize, int totalTime) {
    // Integer division floors automatically, giving the number of complete batches.
    int64_t totalBatches = totalTime / ovenTime;
    return totalBatches * static_cast<int64_t>(batchSize);
}

#include <cassert>

int main() {
    // Basic case: 2 minutes per batch, 3 biscuits per batch, 10 minutes total -> 5 batches * 3 = 15
    assert(biscuitsEaten(2, 3, 10) == 15);
    // Exact multiple: 5 min per batch, 4 biscuits, 15 minutes -> 3 batches * 4 = 12
    assert(biscuitsEaten(5, 4, 15) == 12);
    // Not enough time for one batch: 10 min per batch, 5 biscuits, 9 minutes -> 0
    assert(biscuitsEaten(10, 5, 9) == 0);
    // Exactly one batch fits: 7 min per batch, 2 biscuits, 7 minutes -> 2
    assert(biscuitsEaten(7, 2, 7) == 2);
    // Extra time does not produce partial batches: 3 min per batch, 1 biscuit, 8 minutes -> 2 batches, 2 biscuits
    assert(biscuitsEaten(3, 1, 8) == 2);
    // Large numbers to check overflow safety (using int64_t internally)
    assert(biscuitsEaten(1, 1000000000, 1000000000) == 1000000000000000000LL);
    // Zero total time
    assert(biscuitsEaten(4, 7, 0) == 0);
    // Batch size zero (edge case, but should return 0)
    assert(biscuitsEaten(2, 0, 10) == 0);
    return 0;
}

// The problem is a straightforward integer division and multiplication. The maximum number of complete batches that can be baked in `totalTime` minutes is `totalTime / ovenTime`. Each batch produces `batchSize` biscuits, so the total biscuits are `(totalTime / ovenTime) * batchSize`. Since we only count complete batches, when `totalTime` is less than `ovenTime`, the integer division yields zero, automatically satisfying the requirement to return zero. Edge cases include: `ovenTime` equal to `totalTime` (one batch), `totalTime` being a multiple of `ovenTime` (exact batches), and very large values that could overflow—though the problem uses `int`, we can note that using `long long` internally would avoid overflow for moderate inputs. The algorithm runs in O(1) time and uses O(1) auxiliary space.
