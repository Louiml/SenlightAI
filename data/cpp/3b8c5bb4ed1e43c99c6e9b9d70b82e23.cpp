/*
Write a C++ function named `maxRoadTripSouvenirs` that takes two positive integers `N` and `M`, followed by an array of `N` prices for souvenirs available in city A and an array of `M` prices for souvenirs available in city B, and returns the maximum total price of exactly two souvenirs one from each city such that the sum is maximized. The function should return that maximum sum as an integer. The input arrays are 1-indexed conceptually (the first element corresponds to index 1). All prices are positive integers. If no valid pair exists (e.g., either array empty), return 0.
*/
#include <vector>
#include <algorithm>

// Returns the maximum possible sum of exactly one souvenir from city A and one from city B.
// If either vector is empty, returns 0.
int maxRoadTripSouvenirs(const std::vector<int>& pricesA, const std::vector<int>& pricesB) {
    if (pricesA.empty() || pricesB.empty()) {
        return 0;
    }
    
    int maxA = *std::max_element(pricesA.begin(), pricesA.end());
    int maxB = *std::max_element(pricesB.begin(), pricesB.end());
    
    return maxA + maxB;
}
#include <cassert>
#include <vector>

int maxRoadTripSouvenirs(const std::vector<int>& pricesA, const std::vector<int>& pricesB);

int main() {
    // Normal case
    assert(maxRoadTripSouvenirs({1, 5, 3}, {4, 2, 6}) == 11); // 5 + 6
    // Single element each
    assert(maxRoadTripSouvenirs({10}, {20}) == 30);
    // Duplicates
    assert(maxRoadTripSouvenirs({7, 7, 1}, {9, 9}) == 16);
    // All same prices
    assert(maxRoadTripSouvenirs({3, 3, 3}, {3, 3}) == 6);
    // One empty vector
    assert(maxRoadTripSouvenirs({}, {1, 2}) == 0);
    assert(maxRoadTripSouvenirs({5}, {}) == 0);
    // Both empty
    assert(maxRoadTripSouvenirs({}, {}) == 0);
    // Larger numbers
    assert(maxRoadTripSouvenirs({1000, 500, 999}, {2, 1}) == 1001);
    // Order independence (max in first position vs last)
    assert(maxRoadTripSouvenirs({3, 1, 9}, {8, 0}) == 17);
}
// The problem reduces to finding the maximum element from each of the two arrays and summing them. Since we need exactly one souvenir from each city and we want the maximum total price, the optimal answer is simply the maximum price in city A plus the maximum price in city B. This works because there are no constraints such as budget or ordering—only the sum needs to be maximized, and the independent maxima combine optimally. Edge cases include when one or both arrays are empty (return 0), and when arrays contain duplicate maxima (still fine). The time complexity is O(N + M) to scan both arrays once, and the space complexity is O(1) auxiliary extra space (excluding the input storage). The function should be `const`-correct by taking the vectors by `const` reference. Since the original problem snippet uses 1-indexed arrays, we will assume the vectors passed are 0-indexed but represent the same data; we only need to find the maximums.
