// You are given `n` boxes, each with a fixed capacity and a value per unit capacity. You have a truck with total capacity `N`. Write a C++ function `maxValueFromBoxes(int truckCapacity, const std::vector<std::pair<int,int>>& boxes)` that takes the truck capacity and a vector of pairs where each pair is `(boxCapacity, valuePerUnit)`. You may take any amount (including fractional amounts) from each box, but you cannot exceed a box's own capacity. Your goal is to fill the truck with units to maximize total value. Return the maximum total value as an integer (since all inputs are integers, the result can be computed without floating-point loss). The boxes vector may be unsorted, may contain duplicate capacities or values, and may have zero or negative values (which you should avoid taking if possible). If the truck capacity is zero or less, return zero. Assume `truckCapacity` is non-negative.
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (from solution)
long long maxValueFromBoxes(int truckCapacity, const std::vector<std::pair<int,int>>& boxes);

int main() {
    // Basic example from typical problem: capacity 10, boxes: (5,10) and (3,5) sorted appropriately
    std::vector<std::pair<int,int>> boxes1 = {{5,10}, {3,5}};
    // Take full (5*10=50), remaining 5, take (3*5=15), remaining 2, but no more boxes -> total 65
    assert(maxValueFromBoxes(10, boxes1) == 65LL);

    // Unsorted input
    std::vector<std::pair<int,int>> boxes2 = {{3,5}, {5,10}, {2,7}};
    // Sort by value: (5,10) value=10, (2,7) value=7, (3,5) value=5. Capacity 5:
    // take (5*10=50) full, remaining 0 -> total 50
    assert(maxValueFromBoxes(5, boxes2) == 50LL);

    // Capacity larger than total capacities
    std::vector<std::pair<int,int>> boxes3 = {{2,3}, {4,1}};
    // Sort: (2,3) value=3, (4,1) value=1. Capacity 10: take both full -> 2*3 + 4*1 = 10
    assert(maxValueFromBoxes(10, boxes3) == 10LL);

    // Negative values: all non-positive so result 0
    std::vector<std::pair<int,int>> boxes4 = {{5,-2}, {3,-1}};
    assert(maxValueFromBoxes(10, boxes4) == 0LL);

    // Mixed positive and zero/negative: take only positives
    std::vector<std::pair<int,int>> boxes5 = {{4,3}, {2,0}, {1,-5}};
    // Sort: (4,3) value=3, (2,0) value=0, (1,-5) value=-5. Capacity 3:
    // take (3*3=9) from the first box (partial) -> total 9
    assert(maxValueFromBoxes(3, boxes5) == 9LL);

    // Edge cases: capacity 0, empty vector
    assert(maxValueFromBoxes(0, {{1,1}}) == 0LL);
    assert(maxValueFromBoxes(5, {}) == 0LL);

    // Duplicate capacities and values
    std::vector<std::pair<int,int>> boxes6 = {{2,10}, {2,10}, {1,5}};
    // Sort: all value 10 then 5. Capacity 4:
    // take 2*10=20, remaining 2, take 2*10=20 -> total 40
    assert(maxValueFromBoxes(4, boxes6) == 40LL);

    // Large numbers to check long long usage
    std::vector<std::pair<int,int>> boxes7 = {{1000000, 1000000}};
    assert(maxValueFromBoxes(500000, boxes7) == 500000LL * 1000000LL);

    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum total value obtainable by filling a truck of given capacity
// with fractional amounts from boxes, each with a capacity and value per unit.
// If no positive-value boxes exist, the result is 0.
long long maxValueFromBoxes(int truckCapacity, const std::vector<std::pair<int,int>>& boxes) {
    if (truckCapacity <= 0 || boxes.empty()) {
        return 0LL;
    }

    // Copy boxes to sort, preserving original input.
    std::vector<std::pair<int,int>> sortedBoxes = boxes;

    // Sort descending by value per unit (second element).
    std::sort(sortedBoxes.begin(), sortedBoxes.end(),
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  return a.second > b.second;
              });

    long long totalValue = 0LL;
    int remaining = truckCapacity;

    for (const auto& box : sortedBoxes) {
        if (box.second <= 0) {
            // Non-positive value per unit: never take these.
            break;
        }
        if (remaining == 0) {
            break;
        }
        if (box.first <= remaining) {
            // Take entire box.
            totalValue += static_cast<long long>(box.first) * box.second;
            remaining -= box.first;
        } else {
            // Take only what fits.
            totalValue += static_cast<long long>(remaining) * box.second;
            remaining = 0;
        }
    }

    return totalValue;
}
// This is a classic fractional knapsack problem where each item (box) has a weight (box capacity) and a value per unit weight (value per unit). To maximize total value, we sort boxes in descending order of value per unit. Then we iterate through sorted boxes: if the remaining truck capacity is greater than or equal to the box's capacity, we take the entire box, add `boxCapacity * valuePerUnit` to the result, and reduce remaining capacity by `boxCapacity`. Otherwise, we take only the remaining capacity fraction, add `remaining * valuePerUnit` to the result, and break because the truck is full. If a box has non-positive value per unit, it will be at the end after sorting descending, and we will skip it unless all positive-valued boxes are exhausted and we still have capacity (which would not happen because we break when full, and if capacity remains, the remaining boxes have non-positive values so we take none—actually we must handle that: if we still have capacity after iterating all boxes, we do not add negative values, because we can choose to take nothing from those boxes). Edge cases: empty vector, capacity zero, all boxes with negative value—we return zero because we take nothing. Sorting by value descending ensures greedy optimality for fractional items. Time complexity is O(m log m) for sorting, O(m) for iteration. Space complexity O(1) auxiliary (excluding the storage of the input vector).
