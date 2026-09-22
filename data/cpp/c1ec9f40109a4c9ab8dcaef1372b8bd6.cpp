// Write a C++ function named `findMinimumHeatingRadius` that takes two vectors of integers, `houses` and `heaters`, representing positions along a number line (not necessarily sorted, and possibly containing duplicates). The function must return the minimum possible radius `r` such that every house is within distance `r` of at least one heater. More precisely, for every house position `h`, there must exist at least one heater position `ht` with `|h - ht| <= r`. Assume both vectors are non-empty. Your solution must be efficient even for large inputs (up to ~10^5 elements each) and must handle negative positions and non-unique values.

// The core idea is to sort both arrays, then for each house, find the nearest heater using a two-pointer or binary search approach. Since heating radius must cover all houses, the answer is the maximum, over all houses, of the distance from that house to its closest heater.  
// **Algorithm:** Sort `houses` and `heaters` in ascending order. Maintain a pointer `j` that advances through `heaters` while `heaters[j] <= houses[i]`, so that `heaters[j]` is the first heater not strictly less than the current house. Then the two candidate nearest heaters are `heaters[j]` (right side) and `heaters[j-1]` if `j>0` (left side). Compute `leftDist = abs(houses[i] - previousHeater)` and `rightDist = abs(heaters[j] - houses[i])`. The closest distance for that house is `min(leftDist, rightDist)`. Update the global answer as `max(answer, closestDistance)`.  
// **Edge cases:** If `j == 0`, there is no left heater; use the first heater for both sides (or treat left as infinity, but min with right still works if we set left to a large value). If `j` reaches `heaters.size() - 1` and the house is beyond all heaters, the while loop stops correctly; the right heater is the last one and left is `heaters[j-1]`. Duplicate positions in houses or heaters do not affect correctness. If all houses are left of the smallest heater, `j` remains 0 and left is computed from `heaters[0]`, right also from `heaters[0]` (which are equal), so min is that distance.  
// **Complexities:** Sorting takes O(H log H + T log T) where H=number of houses, T=number of heaters. The single pass over houses with a moving pointer for heaters takes O(H + T) time, so overall O(H log H + T log T). Auxiliary space is O(1) beyond input storage.

#include <vector>
#include <algorithm>
#include <cstdlib> // for abs(int)

// Returns the minimum radius such that every house is covered by at least one heater.
int findMinimumHeatingRadius(std::vector<int>& houses, std::vector<int>& heaters) {
    // Sort both containers to enable efficient nearest-neighbor search.
    std::sort(houses.begin(), houses.end());
    std::sort(heaters.begin(), heaters.end());

    // Pointer into the heaters array. We advance it while heaters[j] <= current house.
    int j = 0;
    int radius = 0;

    for (int house : houses) {
        // Move to the first heater that is > house (or the last one if all are <=).
        while (j < static_cast<int>(heaters.size()) - 1 && heaters[j] <= house) {
            ++j;
        }

        // Distance to the heater on the right (which is heaters[j]).
        int rightDist = std::abs(heaters[j] - house);

        // Distance to the heater on the left (previous heater), if any.
        int leftDist = (j > 0) ? std::abs(heaters[j - 1] - house) : rightDist;

        // The house needs the smaller of the two distances.
        int needed = std::min(leftDist, rightDist);

        // The global radius must cover the worst-case house.
        radius = std::max(radius, needed);
    }

    return radius;
}

#include <cassert>
#include <vector>

// The solution function is declared above, but for testing we include it here.
// In a real test file, you would include the header or paste the function.

int main() {
    // Example from the original snippet: houses [1,2,3], heaters [2]
    {
        std::vector<int> houses = {1, 2, 3};
        std::vector<int> heaters = {2};
        assert(findMinimumHeatingRadius(houses, heaters) == 1);
    }

    // Houses on both sides, heaters in middle
    {
        std::vector<int> houses = {1, 2, 3, 4};
        std::vector<int> heaters = {1, 4};
        assert(findMinimumHeatingRadius(houses, heaters) == 0);
    }

    // Single house, single heater same position
    {
        std::vector<int> houses = {5};
        std::vector<int> heaters = {5};
        assert(findMinimumHeatingRadius(houses, heaters) == 0);
    }

    // Single house far from single heater
    {
        std::vector<int> houses = {0};
        std::vector<int> heaters = {10};
        assert(findMinimumHeatingRadius(houses, heaters) == 10);
    }

    // Unsorted input with negative positions and duplicates
    {
        std::vector<int> houses = {-3, -1, 0, 2, 2, 4};
        std::vector<int> heaters = {-2, 3, 3};
        assert(findMinimumHeatingRadius(houses, heaters) == 1);
    }

    // All houses to the right of all heaters
    {
        std::vector<int> houses = {10, 12, 15};
        std::vector<int> heaters = {1, 2, 3};
        assert(findMinimumHeatingRadius(houses, heaters) == 12);
    }

    // All houses to the left of all heaters
    {
        std::vector<int> houses = {-5, -3};
        std::vector<int> heaters = {0, 1};
        assert(findMinimumHeatingRadius(houses, heaters) == 5);
    }

    // Large gap case with multiple heaters
    {
        std::vector<int> houses = {1, 100, 200};
        std::vector<int> heaters = {50, 150, 300};
        // House 1 -> nearest 50, dist 49; house 100 -> nearest 50 dist 50 or 150 dist 50; house 200 -> nearest 150 dist 50. Max = 50?
        // Actually house 1 to 50 = 49, house 100 to 50/150 = 50, house 200 to 150 = 50, so answer 50.
        assert(findMinimumHeatingRadius(houses, heaters) == 50);
    }

    // Empty? Not allowed per task, but test with both size 1
    {
        std::vector<int> houses = {7};
        std::vector<int> heaters = {-1};
        assert(findMinimumHeatingRadius(houses, heaters) == 8);
    }

    return 0;
}
