Write a C++ function `maxPassengersInTram` that simulates a tram making multiple stops. The function takes an integer `n` (the number of stops) and a vector of pairs of integers, where each pair `{a, b}` represents the number of passengers exiting and entering at that stop, respectively. The tram starts empty at the first stop. For each stop, passengers exit first, then new passengers enter. The capacity of the tram is the maximum number of passengers onboard at any point during the entire journey. The function should return this maximum capacity. The input is guaranteed to be valid: at no stop will more passengers exit than are currently onboard, and all counts are non-negative. Handle the edge case where `n` could be 0 (return 0) and where the maximum occurs at the very beginning or after the last stop.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (provided externally)
int maxPassengersInTram(int n, const std::vector<std::pair<int, int>>& stops);

int main() {
    // Example from the snippet: 3 stops
    std::vector<std::pair<int, int>> stops1 = {{0, 3}, {2, 5}, {4, 2}};
    assert(maxPassengersInTram(3, stops1) == 6); // sequence: 3, 6, 4

    // No stops -> 0
    assert(maxPassengersInTram(0, {}) == 0);

    // Single stop, only passengers board
    std::vector<std::pair<int, int>> stops2 = {{0, 10}};
    assert(maxPassengersInTram(1, stops2) == 10);

    // Single stop, passengers exit (tram starts empty, so exiting must be 0)
    std::vector<std::pair<int, int>> stops3 = {{0, 5}};
    assert(maxPassengersInTram(1, stops3) == 5);

    // Multiple stops, maximum at the end
    std::vector<std::pair<int, int>> stops4 = {{0, 2}, {1, 3}, {2, 4}};
    // Start:0 -> after stop1:2, after stop2: (2-1)+3=4, after stop3: (4-2)+4=6
    assert(maxPassengersInTram(3, stops4) == 6);

    // Passengers exit more than enter, but never below zero
    std::vector<std::pair<int, int>> stops5 = {{0, 5}, {3, 1}, {2, 2}};
    // after stop1:5, after stop2: (5-3)+1=3, after stop3: (3-2)+2=3
    assert(maxPassengersInTram(3, stops5) == 5);

    // All zeros
    std::vector<std::pair<int, int>> stops6 = {{0, 0}, {0, 0}, {0, 0}};
    assert(maxPassengersInTram(3, stops6) == 0);

    return 0;
}

#include <vector>
#include <utility>
#include <algorithm>

// Computes the maximum number of passengers onboard at any point.
// The tram starts empty. Each stop provides (exiting, entering) counts.
int maxPassengersInTram(int n, const std::vector<std::pair<int, int>>& stops) {
    int current = 0;
    int maxCapacity = 0;

    for (int i = 0; i < n; ++i) {
        int exiting = stops[i].first;
        int entering = stops[i].second;
        current -= exiting;   // passengers exit first
        current += entering;  // then new passengers enter
        maxCapacity = std::max(maxCapacity, current);
    }

    return maxCapacity;
}

// The solution uses a simple simulation. Initialize `currentPassengers` to 0 and `maxCapacity` to 0. Iterate through each stop in order. For each stop, subtract `a` (exiting) from `currentPassengers`, then add `b` (entering). After updating, update `maxCapacity` to the maximum of its previous value and `currentPassengers`. Since the tram starts empty and passengers can only increase or decrease as specified, this directly tracks the onboard count. Key edge cases: if `n` is 0, the loop doesn't execute and we return 0 (the initial max). If all stops have no net change, max stays 0. If the maximum occurs at the start of a stop after exits but before entries, that value is captured because we update after both operations within the same iteration—but note the problem states "at any point," so we should consider the value after exits but before entries if that could be larger. However, since exits reduce and entries increase, the maximum will always occur after entries (or at the very start for the first stop with no exits). To be safe, we could also check after the subtraction and before addition, but the problem's typical interpretation (as in the original snippet) only checks after the full stop update. For correctness, the maximum cannot exceed a value right after entries because exits only decrease. So tracking after the full update is sufficient. Time complexity is O(n), space O(1) extra (excluding input storage). The function should take a `const std::vector<std::pair<int,int>>&` to avoid copying and use `const` where appropriate.
