// Write a C++ function `long long minTotalCost(const std::vector<long long>& available, const std::vector<long long>& requirements)` that, given a multiset of available item costs and a list of required minimum costs, determines the minimum total cost such that each requirement is satisfied by selecting a distinct available item whose cost is at least the requirement. If it is impossible to satisfy all requirements, return `-1`. The function should work for up to 10^5 items and requirements, with costs up to 10^9. The requirements may be in any order, and duplicate costs are allowed in both the available multiset and the requirements list.

#include <cassert>
#include <vector>

// The solution function above is included here for completeness in the test.
// In a real setup, the function definition would be above this main.
long long minTotalCost(const std::vector<long long>& available,
                       const std::vector<long long>& requirements);

int main() {
    // Basic example: choose 3,5,6 for requirements 1,4,5 -> 14
    assert(minTotalCost({3, 5, 6, 8}, {1, 4, 5}) == 14);

    // Duplicate items: need two items >= 2, choose 2 and 3 -> 5
    assert(minTotalCost({2, 3, 4}, {2, 2}) == 5);

    // Impossible: cannot satisfy requirement 100
    assert(minTotalCost({1, 2, 3}, {2, 100}) == -1);

    // Empty requirements -> 0
    assert(minTotalCost({5, 6}, {}) == 0);

    // Requirements unsorted should still work correctly
    assert(minTotalCost({10, 20, 30}, {30, 10, 20}) == 60);

    // All identical items and requirements
    assert(minTotalCost({7, 7, 7}, {7, 7, 7}) == 21);

    // Exactly enough items, must use all
    assert(minTotalCost({5, 8, 12}, {4, 5, 10}) == 25);

    // Items with large values
    assert(minTotalCost({1000000000, 999999999}, {900000000, 999999999}) == 1999999999);

    // Duplicate requirements and items
    assert(minTotalCost({4, 4, 1, 1}, {1, 1, 4, 4}) == 10);

    // Requirement larger than any item -> -1
    assert(minTotalCost({1, 2}, {2, 3}) == -1);

    return 0;
}

#include <vector>
#include <set>
#include <algorithm>

// Given available item costs and required minimum costs, return the minimum total cost
// to satisfy every requirement using a distinct available item, or -1 if impossible.
long long minTotalCost(const std::vector<long long>& available,
                       const std::vector<long long>& requirements) {
    if (requirements.empty()) return 0;

    std::multiset<long long> items(available.begin(), available.end());
    std::vector<long long> reqs = requirements;
    std::sort(reqs.begin(), reqs.end()); // Process smaller requirements first

    long long total = 0;
    for (long long r : reqs) {
        auto it = items.lower_bound(r);
        if (it == items.end()) {
            return -1; // No item large enough for this requirement
        }
        total += *it;
        items.erase(it);
    }
    return total;
}

// The optimal strategy is to sort the requirements in non-decreasing order and process them in that order. For each requirement, we need the smallest available item that is at least as large as the requirement. Using a `std::multiset` (or `std::set` with counts) to store the available costs, we can perform a `lower_bound` query for each requirement. If no such item exists, the task is impossible and we return `-1`. Otherwise, we erase that item (one occurrence) and add its cost to the total. Sorting the requirements ensures that we always match the smallest possible item to the smallest needed requirement first, which is optimal because any valid assignment must assign distinct items, and processing in ascending order minimizes the cost by keeping larger items for later, more demanding requirements. The time complexity is \(O((n + m) \log n)\) due to sorting the requirements (\(O(m \log m)\)) and performing \(m\) binary-search-like operations on the multiset. The space complexity is \(O(n)\) for the multiset. Edge cases include an empty requirement list (returns 0), insufficient available items (returns -1), and duplicate costs where each must be used exactly once.
