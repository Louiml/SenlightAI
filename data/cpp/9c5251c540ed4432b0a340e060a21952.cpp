// Write a C++ function `int maxPassengers(int n, const std::vector<std::pair<int,int>>& stops)` that models a tram route. The tram starts empty. There are `n` stops total, and for each stop (except the final one) the input provides the number of passengers `a` who get off and the number `b` who get on at that stop. For the final stop, only the number `a` who get off is relevant (no one gets on). The function must return the maximum number of passengers that were ever on the tram at any point during the journey, assuming the tram capacity is unlimited and the counts are always non‑negative and valid (i.e., at each stop, the number getting off never exceeds the current occupancy). The first line gives `n`, followed by `n-1` pairs of `(a,b)` for intermediate stops, and then a single `a` for the last stop. The function must compute the running total correctly and return the peak value.
The solution simulates the tram stop by stop. Initialize `current` and `ans` to 0 (since the tram starts empty). For each of the `n-1` intermediate stops, subtract the number of passengers getting off, then add the number getting on, update `current`, and set `ans` to the maximum of `ans` and `current`. After processing all intermediate stops, handle the last stop: subtract the alighting passengers (since no one boards), update `current` (which should become 0 if input is correct), and again update `ans` with the maximum. The key is to correctly track the running total and record the peak at every change. Edge cases include: a single stop (then no intermediate stops, just the final `a` which is 0 because the tram starts empty – the answer is 0), and cases where the tram empties completely before the end (ans remains at the earlier peak). Time complexity is O(n) because each stop is processed once. Space complexity is O(1) aside from the input storage if passed as a vector; the algorithm itself uses constant extra space.
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum number of passengers on the tram at any point.
// stops contains pairs (off, on) for each intermediate stop, and finalOff is the number getting off at the last stop.
int maxPassengers(const std::vector<std::pair<int,int>>& stops, int finalOff) {
    int current = 0;
    int ans = 0;
    for (const auto& stop : stops) {
        current -= stop.first;   // passengers get off
        current += stop.second;  // passengers get on
        ans = std::max(ans, current);
    }
    current -= finalOff;         // no one gets on at the last stop
    ans = std::max(ans, current);
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

int maxPassengers(const std::vector<std::pair<int,int>>& stops, int finalOff);

int main() {
    // Example: 4 stops, intermediate: (0,3), (2,5), (4,1); finalOff=2
    std::vector<std::pair<int,int>> stops1 = {{0,3}, {2,5}, {4,1}};
    assert(maxPassengers(stops1, 2) == 7);

    // Single stop: no intermediate, finalOff=0 (empty tram)
    std::vector<std::pair<int,int>> stops2 = {};
    assert(maxPassengers(stops2, 0) == 0);

    // Tram gradually empties, peak at the beginning
    std::vector<std::pair<int,int>> stops3 = {{0,5}, {3,1}, {2,0}};
    assert(maxPassengers(stops3, 1) == 5);

    // All passengers get off at once at the final stop
    std::vector<std::pair<int,int>> stops4 = {{0,10}, {5,0}};
    assert(maxPassengers(stops4, 5) == 10);

    // Zero passengers throughout
    std::vector<std::pair<int,int>> stops5 = {{0,0}, {0,0}};
    assert(maxPassengers(stops5, 0) == 0);

    // Larger tram, peak in the middle
    std::vector<std::pair<int,int>> stops6 = {{0,2}, {1,3}, {4,0}, {0,1}};
    assert(maxPassengers(stops6, 2) == 5);
}
