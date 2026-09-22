Write a C++ function that takes an integer `n` and two vectors of integers `a` and `b`, each of length `n`, representing a sequence of `n` tram stops where at each stop `i` (0-indexed) exactly `a[i]` passengers get off and `b[i]` passengers get on the tram. The tram starts empty before the first stop and cannot have a negative number of passengers at any point (the given inputs guarantee this). The function must simulate the trip and return the maximum number of passengers ever present on the tram during the entire journey (including just after boarding at each stop). For clarity, just before stop `i`, the tram has `c` passengers; after letting `a[i]` off and boarding `b[i]`, the new passenger count is `d = c - a[i] + b[i]`. The initial passenger count is 0. The function should return the highest value of `d` observed after any stop.
The algorithm is a straightforward simulation. Initialize a variable `current` to 0 (passengers on the tram before the first stop). Also initialize `max_passengers` to 0. Iterate over all stops from `i = 0` to `n-1`: update `current = current - a[i] + b[i]` (since `a[i]` get off and `b[i]` get on). Then if `current > max_passengers`, update `max_passengers`. At the end, return `max_passengers`. Edge cases include: n can be 0 (return 0), or if the tram is always empty (e.g., each stop has equal on/off counts), the maximum is 0. The problem guarantees that `current` never becomes negative, but if inputs were invalid, the logic would still work (it would just go negative, but the maximum would still be computed correctly). The time complexity is O(n) because we process each stop once, and the space complexity is O(1) auxiliary beyond the input vectors (which are passed by const reference to avoid copying). We should use `const std::vector<int>&` for parameters and mark the function as `const`-correct.
#include <vector>
#include <algorithm>

// Simulates the tram journey and returns the maximum number of passengers on board.
// a[i] passengers get off at stop i, b[i] passengers get on at stop i.
// Tram starts empty. Assumes inputs never cause negative passengers.
int maxPassengers(const std::vector<int>& a, const std::vector<int>& b) {
    int current = 0;          // passengers on tram before first stop
    int max_count = 0;        // highest passenger count observed

    for (std::size_t i = 0; i < a.size(); ++i) {
        current = current - a[i] + b[i];   // after stop i
        max_count = std::max(max_count, current);
    }

    return max_count;
}
#include <cassert>
#include <vector>

// Free function prototype (as defined in the solution)
int maxPassengers(const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Example from typical tram problems
    assert(maxPassengers({0, 2, 3}, {3, 1, 1}) == 3);
    // All on, none off
    assert(maxPassengers({0, 0, 0}, {5, 2, 1}) == 8);
    // All off, none on
    assert(maxPassengers({3, 2, 4}, {0, 0, 0}) == 0);
    // Mixed, peak in middle
    assert(maxPassengers({1, 2, 1}, {3, 4, 0}) == 4);
    // Empty input
    assert(maxPassengers({}, {}) == 0);
    // Single stop
    assert(maxPassengers({3}, {5}) == 2);
    // Peak at last stop
    assert(maxPassengers({1, 1, 1}, {1, 1, 5}) == 4);
    // Long journey with decreasing then increasing
    assert(maxPassengers({0, 2, 4, 1, 0}, {3, 2, 1, 5, 2}) == 6);
    // Constant passenger count
    assert(maxPassengers({0, 1, 2, 1}, {3, 3, 3, 3}) == 3);
    // All zeros
    assert(maxPassengers({0, 0, 0}, {0, 0, 0}) == 0);
    return 0;
}
