/*
Write a C++ function `double averageWaitingTime(const std::vector<std::vector<int>>& customers)` that simulates a single chef cooking dishes in the order customers arrive at a restaurant. Each customer is represented by a pair `[arrivalTime, cookingTime]` where the chef starts preparing their dish as soon as the chef is free and the customer has arrived (whichever comes later), and the waiting time for a customer is the difference between the time their dish is completed and their arrival time. The function must return the average waiting time across all customers as a double. The input vector is non-empty, and all arrival times and cooking times are positive integers. Customers are given in non-decreasing order of arrival time. Assume the chef starts idle at time 0.
*/

#include <vector>
#include <algorithm> // for std::max (though not strictly needed if using if/else)

// Computes the average waiting time for all customers.
// customers[i] = {arrivalTime, cookingTime}, sorted by arrival time.
double averageWaitingTime(const std::vector<std::vector<int>>& customers) {
    double total_wait = 0.0;
    int chef_free_time = 0; // time when the chef finishes the previous dish
    
    for (const auto& customer : customers) {
        int arrival = customer[0];
        int cooking = customer[1];
        
        // The chef starts when both the previous dish is done and the customer has arrived.
        int start_time = (chef_free_time > arrival) ? chef_free_time : arrival;
        chef_free_time = start_time + cooking;
        
        // Waiting time is completion time minus arrival time.
        total_wait += chef_free_time - arrival;
    }
    
    return total_wait / static_cast<double>(customers.size());
}

#include <cassert>
#include <vector>

// Declaration of the function (assume it's included from above)
double averageWaitingTime(const std::vector<std::vector<int>>& customers);

int main() {
    // Example 1: from typical problem
    std::vector<std::vector<int>> customers1 = {{1,2},{2,5},{4,3}};
    // Customer 1: start at 1, finish at 3, wait=2
    // Customer 2: start at 3, finish at 8, wait=6
    // Customer 3: start at 8, finish at 11, wait=7 -> avg=(2+6+7)/3=5.0
    assert(averageWaitingTime(customers1) == 5.0);

    // Example 2: chef idle between customers
    std::vector<std::vector<int>> customers2 = {{5,1},{10,4}};
    // C1: start 5, finish 6, wait=1
    // C2: start 10, finish 14, wait=4 -> avg=2.5
    assert(averageWaitingTime(customers2) == 2.5);

    // Single customer, no idle
    std::vector<std::vector<int>> customers3 = {{0,10}};
    assert(averageWaitingTime(customers3) == 10.0);

    // Customers arriving while chef is busy (all consecutive, no idle)
    std::vector<std::vector<int>> customers4 = {{0,1},{1,1},{2,1}};
    // waits: 1, 2, 2 -> avg=5/3 ~1.6667
    double result4 = averageWaitingTime(customers4);
    assert(result4 > 1.66666 && result4 < 1.66667); // compare within tolerance

    // Customers with arrival times that are not consecutive but chef stays busy
    std::vector<std::vector<int>> customers5 = {{0,3},{2,2},{3,1}};
    // C1: start0 finish3 wait3
    // C2: start3 finish5 wait3
    // C3: start5 finish6 wait3 -> avg=3.0
    assert(averageWaitingTime(customers5) == 3.0);

    // Large cooking times causing long waits
    std::vector<std::vector<int>> customers6 = {{1,100},{2,1}};
    // C1: start1 finish101 wait100
    // C2: start101 finish102 wait100 -> avg=100.0
    assert(averageWaitingTime(customers6) == 100.0);

    // Customers with same arrival time (though problem says non-decreasing, duplicates allowed)
    std::vector<std::vector<int>> customers7 = {{3,5},{3,2}};
    // C1: start3 finish8 wait5
    // C2: start8 finish10 wait7 -> avg=6.0
    assert(averageWaitingTime(customers7) == 6.0);

    // Arrival at time 0
    std::vector<std::vector<int>> customers8 = {{0,4},{0,2}};
    // C1: start0 finish4 wait4
    // C2: start4 finish6 wait6 -> avg=5.0
    assert(averageWaitingTime(customers8) == 5.0);

    // Mix of idle and busy phases
    std::vector<std::vector<int>> customers9 = {{2,3},{5,1},{10,2}};
    // C1: start2 finish5 wait3
    // C2: start5 finish6 wait1
    // C3: start10 finish12 wait2 -> avg=2.0
    assert(averageWaitingTime(customers9) == 2.0);

    // Single customer with large cooking time
    std::vector<std::vector<int>> customers10 = {{7,500}};
    assert(averageWaitingTime(customers10) == 500.0);

    return 0;
}

// The solution tracks the time when the chef finishes the previous dish (`end`), initialized to 0. For each customer with arrival time `a` and cooking time `ct`:
// - The chef can start cooking only when both the previous dish is done and the customer has arrived. So the start time is `max(end, a)`.
// - The new finish time becomes `start + ct`, and the customer's waiting time is `(start + ct) - a`.
// - Accumulate this waiting time into a running sum, then divide by the number of customers at the end.
// Edge cases: if the chef is idle when a customer arrives (`end < a`), the start time is the arrival time; if the chef is still busy (`end >= a`), the start time is `end`. Since arrivals are sorted, no customer can "overtake" another. The handling of both cases is unified by the `max` formula. For `n` customers, time complexity is `O(n)` and space complexity is `O(1)` beyond the input storage.
