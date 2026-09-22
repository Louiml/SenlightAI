// Write a C++ function `bool canPlaceFlowers(vector<int>& flowerbed, int n)` that determines whether at least `n` new flowers can be planted in a linear flowerbed represented by a vector of integers, where `0` indicates an empty plot and `1` indicates an occupied plot. A new flower may be planted in an empty plot only if the adjacent plots (left and right) are also empty or non-existent (i.e., at the boundaries). The function may modify the input vector by setting planted plots to `1` to reflect the placement, but this modification is only for internal logic and does not affect the return value. The vector length is at least 1, and `n` is a non-negative integer. Return `true` if it is possible to plant at least `n` flowers, otherwise `false`.
The solution iterates through the flowerbed from left to right, checking each position. For a position `i` to be plantable, the current plot must be `0`, and both neighbors must be `0` or out of bounds (for the first and last plots). When such a position is found, we immediately set it to `1` (simulating that a flower has been planted) and increment a counter. This greedy approach is optimal because planting at the leftmost available valid position never prevents more future plantings; since we only move forward, planting now only potentially blocks the immediate next plot, but we would not be able to plant there anyway because the current plot would have been `1`. The loop continues through the entire array. Edge cases include a single-plot bed, where the single plot is plantable if it is `0`, and beds with all `1`s, where no planting is possible. The algorithm stops early if the counter meets or exceeds `n`, but it is not necessary for correctness. Time complexity is O(m), where m is the length of the flowerbed, and space complexity is O(1) auxiliary, ignoring the input modification.
#include <vector>

// Determines if at least `needed` flowers can be planted in the flowerbed.
// Modifies the input vector by planting flowers where possible for internal logic.
bool canPlaceFlowers(std::vector<int>& flowerbed, int needed) {
    int count = 0;
    const int size = flowerbed.size();
    
    for (int i = 0; i < size; ++i) {
        if (flowerbed[i] == 0 &&
            (i == 0 || flowerbed[i - 1] == 0) &&
            (i == size - 1 || flowerbed[i + 1] == 0)) {
            flowerbed[i] = 1;
            ++count;
            if (count >= needed) {
                return true;
            }
        }
    }
    return count >= needed;
}
#include <cassert>
#include <vector>

// Function declaration is assumed from the solution above.

int main() {
    // Test 1: Basic case, can place 1 flower.
    std::vector<int> bed1 = {1, 0, 0, 0, 1};
    assert(canPlaceFlowers(bed1, 1) == true);
    assert(canPlaceFlowers(bed1, 2) == false); // Now bed1 is modified: [1,0,1,0,1]
    
    // Test 2: Single empty plot.
    std::vector<int> bed2 = {0};
    assert(canPlaceFlowers(bed2, 1) == true);
    assert(canPlaceFlowers(bed2, 0) == true);
    
    // Test 3: All occupied.
    std::vector<int> bed3 = {1, 1, 1};
    assert(canPlaceFlowers(bed3, 0) == true);
    assert(canPlaceFlowers(bed3, 1) == false);
    
    // Test 4: Empty beds with boundaries.
    std::vector<int> bed4 = {0, 0, 0};
    assert(canPlaceFlowers(bed4, 2) == true);
    assert(canPlaceFlowers(bed4, 3) == false); // Only two can be placed.
    
    // Test 5: Alternating pattern.
    std::vector<int> bed5 = {1, 0, 1, 0, 1};
    assert(canPlaceFlowers(bed5, 1) == false);
    
    // Test 6: Multiple consecutive zeros.
    std::vector<int> bed6 = {0, 0, 1, 0, 0};
    assert(canPlaceFlowers(bed6, 2) == true);
    
    // Test 7: Large needed value.
    std::vector<int> bed7 = {0, 0, 0, 0};
    assert(canPlaceFlowers(bed7, 1) == true);
    assert(canPlaceFlowers(bed7, 2) == true);
    assert(canPlaceFlowers(bed7, 3) == false);
    
    // Test 8: No flowers needed anyway.
    std::vector<int> bed8 = {0, 1, 0};
    assert(canPlaceFlowers(bed8, 0) == true);
    
    // Test 9: Two-element bed.
    std::vector<int> bed9 = {0, 0};
    assert(canPlaceFlowers(bed9, 1) == true);
    assert(canPlaceFlowers(bed9, 2) == false);
    
    // Test 10: Non-adjacent valid spots chosen greedily.
    std::vector<int> bed10 = {0, 1, 0, 0, 0, 1, 0};
    assert(canPlaceFlowers(bed10, 2) == true);
}
