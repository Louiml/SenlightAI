Write a C++ function `floorDifference(const std::string& floorA, const std::string& floorB)` that takes two optional parking-level strings in a special notation ("B1" through "B9" for basement levels 1–9, and "1F" through "9F" for above-ground floors 1–9) and returns the absolute difference in the number of vertical levels between them. For example, "B2" is one level below "B1", and "B1" is directly adjacent to "1F"; the distance between "B2" and "3F" is 5 levels. The function must handle valid inputs only (all strings are one of the 18 possible notations) and must be case-sensitive. The difference is the absolute value of subtracting the numeric mapping of floorA from that of floorB, where basements map to negative numbers (B1=0, B2=-1, ..., B9=-8) and above-ground floors map to positive numbers (1F=1, ..., 9F=9).
#include <cassert>

// The solution function is assumed to be declared above.
int main() {
    // Same floor returns 0
    assert(floorDifference("B1", "B1") == 0);
    assert(floorDifference("5F", "5F") == 0);

    // Adjacent levels: B1 to 1F, B2 to B1, 1F to 2F
    assert(floorDifference("B1", "1F") == 1);
    assert(floorDifference("B2", "B1") == 1);
    assert(floorDifference("1F", "2F") == 1);

    // Basement to above-ground
    assert(floorDifference("B2", "3F") == 5);
    assert(floorDifference("B9", "9F") == 17);

    // Above-ground to above-ground
    assert(floorDifference("3F", "7F") == 4);
    assert(floorDifference("8F", "1F") == 7);

    // Basement to basement
    assert(floorDifference("B5", "B8") == 3);
    assert(floorDifference("B9", "B1") == 8);

    // Order does not matter
    assert(floorDifference("3F", "B2") == 5);
    assert(floorDifference("9F", "B9") == 17);
}
#include <string>
#include <map>
#include <cstdlib>

// Returns the absolute vertical level difference between two parking floor notations.
// Valid notations: "B1" to "B9" (basement) and "1F" to "9F" (above ground).
int floorDifference(const std::string& floorA, const std::string& floorB) {
    // Build a static mapping from floor notation to numeric level index.
    static const std::map<std::string, int> levelMap = {
        {"B9", -8}, {"B8", -7}, {"B7", -6}, {"B6", -5}, {"B5", -4},
        {"B4", -3}, {"B3", -2}, {"B2", -1}, {"B1", 0},
        {"1F", 1}, {"2F", 2}, {"3F", 3}, {"4F", 4}, {"5F", 5},
        {"6F", 6}, {"7F", 7}, {"8F", 8}, {"9F", 9}
    };

    const int levelA = levelMap.at(floorA);
    const int levelB = levelMap.at(floorB);

    return std::abs(levelA - levelB);
}
// The solution approach is to define a mapping from each of the 18 valid floor strings to a numeric "level index" where the basement levels are assigned negative integers and above-ground floors positive integers, with "B1" at index 0 and "1F" at index 1 so that consecutive levels differ by exactly 1. The mapping can be implemented using a `std::map<std::string, int>` that is populated with all 18 possible entries. The main algorithm simply looks up both floor strings in the map and returns `std::abs(map[floorA] - map[floorB])`. Edge cases include when both floors are the same (difference 0), when one is the highest basement ("B1") and the other is the lowest above-ground floor ("1F") (difference 1), and the maximum possible difference between "B9" (index -8) and "9F" (index 9) is 17. The time complexity is O(1) after the map is built (since map lookup is logarithmic in the number of keys, but with only 18 keys it is effectively constant). Space complexity is O(18) for the map, which is constant. No special handling is needed for invalid inputs since the problem guarantees valid strings.
