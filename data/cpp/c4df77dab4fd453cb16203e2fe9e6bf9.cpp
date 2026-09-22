You are given a list of box types, where each box type is represented by a pair `[numberOfBoxes, unitsPerBox]`, and a truck that can carry a fixed total number of boxes. Write a C++ function `maximumUnits` that takes a `vector<vector<int>>& boxTypes` and an integer `truckSize`, and returns the maximum total number of units that can be loaded into the truck. You may take any number of boxes from any box type, but you cannot exceed the truck's capacity in total boxes. You should prioritize box types with the highest units per box. The input can contain duplicate box types, and the truck size may be larger than the total number of available boxes (in which case all boxes are taken). The function must handle an empty box list and a truck size of zero correctly.

#include <cassert>
#include <vector>

int main() {
    // Basic case: sort and take highest units first
    std::vector<std::vector<int>> boxes1 = {{1, 3}, {2, 2}, {3, 1}};
    assert(maximumUnits(boxes1, 4) == 8); // take 1 box of 3, 2 boxes of 2, 1 box of 1 = 3+4+1=8

    // Truck larger than total boxes: take all
    std::vector<std::vector<int>> boxes2 = {{5, 10}, {2, 7}};
    assert(maximumUnits(boxes2, 10) == 5*10 + 2*7); // 64

    // Exact fit: all boxes fit exactly
    std::vector<std::vector<int>> boxes3 = {{2, 5}, {2, 3}};
    assert(maximumUnits(boxes3, 4) == 2*5 + 2*3); // 16

    // Truck size zero: no boxes taken
    std::vector<std::vector<int>> boxes4 = {{3, 9}, {1, 4}};
    assert(maximumUnits(boxes4, 0) == 0);

    // Empty box list
    std::vector<std::vector<int>> boxes5 = {};
    assert(maximumUnits(boxes5, 5) == 0);

    // Partial fill from a high-unit type, then stop
    std::vector<std::vector<int>> boxes6 = {{2, 100}, {3, 1}};
    assert(maximumUnits(boxes6, 1) == 100); // take only 1 box from first type

    // Duplicate types with same units per box
    std::vector<std::vector<int>> boxes7 = {{2, 4}, {3, 4}, {1, 2}};
    assert(maximumUnits(boxes7, 5) == 2*4 + 3*4 + 0*2); // 20

    // Partial fill across multiple types without filling completely
    std::vector<std::vector<int>> boxes8 = {{3, 6}, {2, 5}, {4, 2}};
    assert(maximumUnits(boxes8, 4) == 3*6 + 1*5); // 23

    // Large truck but not enough boxes
    std::vector<std::vector<int>> boxes9 = {{1, 1}, {1, 1}};
    assert(maximumUnits(boxes9, 100) == 2); // all taken

    return 0;
}

#include <vector>
#include <algorithm>
#include <functional>

// Returns the maximum total units that can be loaded into the truck.
// boxTypes[i] = {numberOfBoxes, unitsPerBox}
// truckSize = maximum number of boxes the truck can carry.
int maximumUnits(std::vector<std::vector<int>>& boxTypes, int truckSize) {
    // Sort in descending order of units per box (second element).
    std::sort(boxTypes.begin(), boxTypes.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[1] > b[1];
              });

    int totalUnits = 0;
    int loadedBoxes = 0;

    for (const auto& boxType : boxTypes) {
        int numBoxes = boxType[0];
        int unitsPerBox = boxType[1];

        // If all boxes of this type fit, take them all.
        if (loadedBoxes + numBoxes <= truckSize) {
            totalUnits += numBoxes * unitsPerBox;
            loadedBoxes += numBoxes;
        } else {
            // Otherwise, take only the remaining space, then stop.
            int remaining = truckSize - loadedBoxes;
            if (remaining > 0) {
                totalUnits += remaining * unitsPerBox;
                loadedBoxes = truckSize;
            }
            break; // No more space left
        }
    }

    return totalUnits;
}

// The optimal strategy is to greedily load boxes with the highest units per box first. Sort the `boxTypes` vector in descending order of the units per box (the second element of each pair). Then iterate through the sorted list, maintaining a running count of loaded boxes and the accumulated total units. For each box type, if the entire batch of boxes fits within the remaining truck capacity, take all of them. Otherwise, if there is still space in the truck, take only the remaining capacity’s worth of boxes from this type (filling the truck completely) and then stop, since all subsequent types have lower or equal units per box. Edge cases: if `boxTypes` is empty, return 0; if `truckSize` is zero, return 0; if the total boxes are less than `truckSize`, simply sum all units. The sorting step dominates the time complexity: `O(n log n)` where `n` is the number of box types. Space complexity is `O(1)` auxiliary (ignoring the input vector and sorting overhead).
