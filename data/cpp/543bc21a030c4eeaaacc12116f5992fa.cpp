// Write a C++ function `long long simulateCarWash(int n, int m, const std::vector<int>& washTime, const std::vector<int>& carWeight, const std::vector<int>& events)` that simulates a car wash center with `n` wash bays and `m` cars. Each bay `i` (1-indexed) has a fixed time `washTime[i]` per unit of car weight. Each car `j` (1-indexed) has weight `carWeight[j]`. The events list has exactly `2*m` integers: each positive integer `x` (1 ≤ x ≤ m) represents car `x` arriving (placed in line), and each negative integer `-x` represents car `x` finishing/waiting and being processed (releasing its bay). Initially all bays are free. When a car arrives, if a bay is free, it is immediately assigned to the bay with the smallest index; otherwise it waits in FIFO queue. When a car finishes (negative event), its bay is freed, and if any car is waiting, the first waiting car is assigned to that freed bay. The total cost is the sum over all cars of `washTime[bay] * carWeight[car]` for the bay that car uses. Return the total cost as a `long long`. The input guarantees every positive event is a distinct car, every negative event refers to a car that has already arrived and not yet finished, and after processing all events all bays are free. Events are given in chronological order.
#include <cassert>
#include <vector>

// Function declared above is assumed to be included here.

int main() {
    // Basic: 1 bay, 2 cars, events: car1 arrives, car1 departs, car2 arrives, car2 departs
    {
        int n = 1, m = 2;
        std::vector<int> washTime = {0, 5};       // bay 1 time=5
        std::vector<int> carWeight = {0, 3, 4};   // car1 weight=3, car2 weight=4
        std::vector<int> events = {1, -1, 2, -2};
        assert(simulateCarWash(n, m, washTime, carWeight, events) == 5*3 + 5*4);
    }

    // Two bays, multiple cars waiting, ensure smallest index bay used
    {
        int n = 2, m = 4;
        std::vector<int> washTime = {0, 2, 3};
        std::vector<int> carWeight = {0, 10, 10, 10, 10};
        // All cars arrive before any departure
        std::vector<int> events = {1, 2, 3, 4, -1, -2, -3, -4};
        // Car1 -> bay1 (2*10=20), car2 -> bay2 (3*10=30), car3 waits,
        // car4 waits, then car1 departs -> car3 gets bay1 (2*10=20),
        // car2 departs -> car4 gets bay2 (3*10=30)
        long long expected = 20 + 30 + 20 + 30;
        assert(simulateCarWash(n, m, washTime, carWeight, events) == expected);
    }

    // Car departs while queue empty, then later car arrives
    {
        int n = 1, m = 3;
        std::vector<int> washTime = {0, 1};
        std::vector<int> carWeight = {0, 5, 7, 2};
        std::vector<int> events = {1, -1, 2, -2, 3, -3};
        assert(simulateCarWash(n, m, washTime, carWeight, events) == 5 + 7 + 2);
    }

    // Multiple departures before new arrivals, reuse bay correctly
    {
        int n = 2, m = 5;
        std::vector<int> washTime = {0, 4, 6};
        std::vector<int> carWeight = {0, 1, 1, 1, 1, 1};
        // Arrivals: 1,2 ; deps 1,2 ; arrivals 3,4,5 ; deps 3,4,5
        std::vector<int> events = {1, 2, -1, -2, 3, 4, 5, -3, -4, -5};
        // Car1 bay1 cost=4, car2 bay2 cost=6, after deps both free.
        // car3 bay1 cost=4, car4 bay2 cost=6, car5 waits then gets bay1 after car3 departs cost=4
        long long expected = 4 + 6 + 4 + 6 + 4;
        assert(simulateCarWash(n, m, washTime, carWeight, events) == expected);
    }

    // Large cost using long long
    {
        int n = 1, m = 2;
        std::vector<int> washTime = {0, 1000000};
        std::vector<int> carWeight = {0, 1000000, 1000000};
        std::vector<int> events = {1, -1, 2, -2};
        long long expected = 1000000LL * 1000000LL + 1000000LL * 1000000LL;
        assert(simulateCarWash(n, m, washTime, carWeight, events) == expected);
    }

    // Empty events (m=0) returns 0
    {
        int n = 3, m = 0;
        std::vector<int> washTime = {0, 1, 2, 3};
        std::vector<int> carWeight = {0};
        std::vector<int> events = {};
        assert(simulateCarWash(n, m, washTime, carWeight, events) == 0);
    }

    return 0;
}
#include <vector>
#include <queue>
#include <cstdlib>

// Simulates car wash with n bays, m cars.
// washTime[i] is time per weight unit for bay i (1-indexed in vector, index 0 unused).
// carWeight[i] is weight of car i (1-indexed in vector, index 0 unused).
// events: positive values are arrivals, negative values are departures.
// Returns total cost as long long.
long long simulateCarWash(int n, int m,
                          const std::vector<int>& washTime,
                          const std::vector<int>& carWeight,
                          const std::vector<int>& events) {
    // Available bay indices (min-heap, smallest index first)
    std::priority_queue<int, std::vector<int>, std::greater<int>> availableBays;
    for (int i = 1; i <= n; ++i) {
        availableBays.push(i);
    }

    // FIFO queue of waiting cars (car id)
    std::queue<int> waitingCars;

    // assignedBay[car] = bay index currently assigned, 0 if not assigned
    std::vector<int> assignedBay(m + 1, 0);

    long long totalCost = 0;

    for (int event : events) {
        if (event > 0) {
            // Arrival
            int car = event;
            waitingCars.push(car);
        } else {
            // Departure: free the bay of car abs(event)
            int car = std::abs(event);
            int bay = assignedBay[car];
            assignedBay[car] = 0; // clear assignment
            availableBays.push(bay); // bay becomes free
        }

        // Process as many waiting cars as possible
        while (!waitingCars.empty() && !availableBays.empty()) {
            int car = waitingCars.front();
            waitingCars.pop();

            int bay = availableBays.top();
            availableBays.pop();

            assignedBay[car] = bay;
            totalCost += static_cast<long long>(washTime[bay]) * carWeight[car];
        }
    }

    return totalCost;
}
// The problem is a classic simulation of a multi-server queue with priority assignment to free servers. We maintain: (1) a min-heap of available bay indices (initialized with all 1..n), (2) a FIFO queue of waiting cars (by car id), (3) an array `assignedBay` of size m+1 recording which bay a car is currently assigned to (or 0 if not). For each event: if positive `x`, push car x into the queue, then attempt to process: while both the queue and heap are non-empty, pop the smallest bay from heap, assign it to the front car, add its cost (`washTime[bay] * carWeight[car]`), update `assignedBay[car]`, and pop the car from queue. If negative `x`, the car `-x` must have an assigned bay; push that bay back into the heap, clear `assignedBay[abs(x)]=0`, then again attempt to process waiting cars. Edge cases: multiple free bays appear simultaneously (heap handles smallest index), waiting cars are served in arrival order (queue), and a negative event may trigger assignment of multiple waiting cars if several bays free up over time (but after each negative only one bay frees, so at most one new assignment occurs per negative, unless more events process later). Time complexity: each car enters the queue once and leaves once, each bay is pushed/popped from heap at most once, so O((n+m) log n) time, O(n+m) space.
