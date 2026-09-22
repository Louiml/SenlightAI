// Given two arrays of equal length \(N\), representing customer arrival and departure times in 24-hour clock format (e.g., 900 means 09:00, 1430 means 14:30), write a C++ function `int minimumChairs(const std::vector<int>& arrivals, const std::vector<int>& departures)` that returns the minimum number of chairs needed so that no customer ever has to stand. Assume that at any exact time, a customer arriving and a customer departing happen simultaneously; to be safe, treat the arriving customer as being present at that moment (i.e., increment before decrement for simultaneous events). The function should handle \(1 \le N \le 100\), and all times are in the range [0000, 2359]. The solution must be efficient and work for any valid input, including cases where multiple arrivals or departures occur at the same time. The function should not read from standard input or print to standard output; it should only compute and return the integer result.
#include <cassert>
#include <vector>

int minimumChairs(const std::vector<int>& arrivals, const std::vector<int>& departures);

int main() {
    // Sample test
    std::vector<int> a1 = {900, 1000, 1100, 1030, 1600};
    std::vector<int> d1 = {1900, 1300, 1130, 1130, 1800};
    assert(minimumChairs(a1, d1) == 4);

    // Single customer
    std::vector<int> a2 = {1000};
    std::vector<int> d2 = {1100};
    assert(minimumChairs(a2, d2) == 1);

    // All overlap at the same time
    std::vector<int> a3 = {900, 900, 900};
    std::vector<int> d3 = {1000, 1000, 1000};
    assert(minimumChairs(a3, d3) == 3);

    // Simultaneous arrival and departure: arrival should count first
    std::vector<int> a4 = {1000, 1000};
    std::vector<int> d4 = {1000, 1100};
    assert(minimumChairs(a4, d4) == 2);

    // No overlap
    std::vector<int> a5 = {100, 200, 300};
    std::vector<int> d5 = {150, 250, 350};
    assert(minimumChairs(a5, d5) == 1);

    // Reverse order input
    std::vector<int> a6 = {1600, 1100, 1000, 1030, 900};
    std::vector<int> d6 = {1800, 1130, 1300, 1130, 1900};
    assert(minimumChairs(a6, d6) == 4);

    // Multiple departures at same time as arrivals
    std::vector<int> a7 = {900, 1000, 1100};
    std::vector<int> d7 = {1000, 1100, 1200};
    assert(minimumChairs(a7, d7) == 2); // at 1000: arrivals 900,1000 present? Actually 900 departs at 1000, 1000 arrives → still 2

    // Large time gap
    std::vector<int> a8 = {0, 2359};
    std::vector<int> d8 = {1, 2400}; // Note 2400 is out of range but still works over midnight; allowed? We assume valid input (<=2359) but test with 2400 might be invalid; better use valid:
    // Correct valid test:
    std::vector<int> a9 = {0, 2358};
    std::vector<int> d9 = {1, 2359};
    assert(minimumChairs(a9, d9) == 1);

    // All arrive, none depart until end
    std::vector<int> a10 = {1, 2, 3, 4};
    std::vector<int> d10 = {5, 5, 5, 5};
    assert(minimumChairs(a10, d10) == 4);

    return 0;
}
#include <vector>
#include <algorithm>

// Return the minimum number of chairs needed so no customer stands.
// Arrivals and departures are given in 24-hour clock format (e.g., 900 = 09:00).
// At the same timestamp, arrivals are counted before departures to be safe.
int minimumChairs(const std::vector<int>& arrivals, const std::vector<int>& departures) {
    std::vector<std::pair<int, int>> events; // first = time, second = +1 for arrival, -1 for departure

    const int n = static_cast<int>(arrivals.size());
    events.reserve(2 * n);

    for (int i = 0; i < n; ++i) {
        events.emplace_back(arrivals[i], 1);   // arrival
        events.emplace_back(departures[i], -1); // departure
    }

    // Sort by time; if times equal, arrival (1) comes before departure (-1)
    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second; // 1 > -1, so arrivals first
    });

    int current = 0;
    int max_needed = 0;

    for (const auto& event : events) {
        current += event.second;
        if (current > max_needed) {
            max_needed = current;
        }
    }

    return max_needed;
}
// The core idea is to treat each arrival and departure as an event with a timestamp and a type (arrival = +1, departure = -1). We collect all events into a single vector of pairs or a custom struct, sort them by time. For events with the same timestamp, arrivals must be processed before departures to correctly count the maximum simultaneous customers — because a customer who arrives at the exact moment another departs still needs a chair. Sorting by time and then by event type (arrival first) ensures this. Then we iterate through the sorted events, maintaining a running count `current` of customers present. For each arrival, increment `current`; for each departure, decrement `current`. The answer is the maximum value `current` reaches at any point. Edge cases include all customers arriving and departing at the same time, or overlapping intervals where many customers are present at a single instant. Complexity: sorting \(2N\) events takes \(O(N \log N)\) time, and the linear scan is \(O(N)\), so overall \(O(N \log N)\) time and \(O(N)\) auxiliary space for the event list.
