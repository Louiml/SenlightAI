// Write a C++ function that accepts a string containing whitespace-separated integers representing building heights, where the buildings are listed from east to west. The function should return a `std::vector<int>` containing the heights of the buildings that have a sunset view, meaning they are visible from the west (the left side when reading the input). A building is visible if there is no other building to its west (i.e., appearing later in the input when processed from east to west) that is strictly taller than it. Buildings of equal height block each other (a building of equal height to the west hides the eastern one). The function must process the input in a single east-to-west pass, simulating an online stream, and return the visible buildings in east-to-west order (the order they appear in the input). The input may contain leading/trailing spaces and multiple spaces between numbers, and may have zero or more buildings. Assume all heights are positive integers (≥ 1). For example, given `"22 5 11 5 9 4 6 2"`, the output should be `{22, 11, 9, 6, 2}`. The function signature should be `std::vector<int> sunsetViewBuildings(const std::string& input)`.
// The core idea is to maintain a stack of buildings that are currently visible from the west while scanning the input from east to west. When we encounter a new building height, it is to the west of all previously processed buildings. If the new building is strictly taller than the top of the stack (the westernmost visible building so far), it will block that building and all other visible buildings behind it that are shorter or equal. Thus, we pop from the stack while the top is less than or equal to the new height, because the new building hides those. After popping, we push the new height onto the stack. This stack is maintained so that it is strictly decreasing from bottom (east, oldest) to top (west, newest). At the end, the stack contains the visible buildings in east-to-west order, which is exactly the order of input. Edge cases: an empty input yields an empty vector; a single building always has a view; equal heights are handled by popping (since a western equal-height building hides the eastern one). The algorithm runs in O(n) time because each building is pushed and popped at most once. Space complexity is O(n) in the worst case for the stack (which is also the output size).
#include <vector>
#include <string>
#include <sstream>

// Returns the heights of buildings that have a sunset view from the west,
// given input as whitespace-separated building heights listed east to west.
// The result is in east-to-west order (the same order as input that has a view).
std::vector<int> sunsetViewBuildings(const std::string& input) {
    std::vector<int> visible;
    std::istringstream stream(input);
    int height;
    while (stream >> height) {
        // Remove all buildings that are shorter or equal, as the new western building hides them.
        while (!visible.empty() && visible.back() <= height) {
            visible.pop_back();
        }
        visible.push_back(height);
    }
    return visible;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here or link it appropriately.

int main() {
    // Example from the problem statement.
    assert(sunsetViewBuildings("22 5 11 5 9 4 6 2") == std::vector<int>({22, 11, 9, 6, 2}));

    // Single building.
    assert(sunsetViewBuildings("7") == std::vector<int>({7}));

    // Empty input (no numbers).
    assert(sunsetViewBuildings("   ") == std::vector<int>());

    // Increasing heights from east to west -> only the westernmost is visible.
    assert(sunsetViewBuildings("1 2 3") == std::vector<int>({3}));

    // Decreasing heights from east to west -> all are visible.
    assert(sunsetViewBuildings("5 4 3 2 1") == std::vector<int>({5, 4, 3, 2, 1}));

    // Duplicate heights – the western one wins.
    assert(sunsetViewBuildings("3 3 3") == std::vector<int>({3}));

    // Mixed with extra whitespace.
    assert(sunsetViewBuildings("  10   5  10  ") == std::vector<int>({10}));

    // Multiple values with equal peaks.
    assert(sunsetViewBuildings("4 7 4 7 4") == std::vector<int>({7, 4}));

    // Negative heights are not expected but test handling of integers.
    // Here we assume positive, but the function works with any ints.
    assert(sunsetViewBuildings("-1 -2 -3") == std::vector<int>({-1, -2, -3})); // all visible because increasing westward

    // Large numbers.
    assert(sunsetViewBuildings("1000000 1 999999") == std::vector<int>({1000000, 999999}));
}
