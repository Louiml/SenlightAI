Write a C++ function named `countFundableDepartments` that takes a vector of non-negative integers `requests` (each representing the cost, in dollars, needed to fund a particular department) and a non-negative integer `budget` (the total amount of money available). The function must return the maximum number of departments that can be fully funded, assuming you may choose any subset of departments, but you must fund each chosen department completely. To maximize the count, always fund the cheapest departments first. If the budget is zero, the result must be zero, even if some requests are also zero. The function should not modify the input vector, and it must handle an empty vector by returning zero.
#include <cassert>
#include <vector>

// Function declaration (or include the solution header)
int countFundableDepartments(const std::vector<int>& requests, int budget);

int main() {
    // Basic cases
    assert(countFundableDepartments({1, 2, 3, 4}, 5) == 2); // 1+2=3, 1+2+3>5
    assert(countFundableDepartments({2, 2, 2}, 5) == 2);   // 2+2=4, 2+2+2>5
    assert(countFundableDepartments({5}, 5) == 1);
    assert(countFundableDepartments({10}, 5) == 0);

    // Empty vector
    assert(countFundableDepartments({}, 10) == 0);

    // Zero budget
    assert(countFundableDepartments({1, 2, 3}, 0) == 0);
    assert(countFundableDepartments({0, 1, 2}, 0) == 0); // Even with zero-cost requests, must return 0

    // Edge cases with zeros and budget > 0
    assert(countFundableDepartments({0, 0, 5}, 2) == 2); // Funding two zero-cost depts is possible, but 5 doesn't fit
    assert(countFundableDepartments({0, 1, 2}, 1) == 2); // 0 and 1 fit, 2 doesn't

    // Duplicates and unsorted input
    assert(countFundableDepartments({3, 1, 2, 1}, 4) == 3); // 1+1+2=4
    assert(countFundableDepartments({4, 4, 4}, 12) == 3);
    assert(countFundableDepartments({4, 4, 4}, 11) == 2);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum number of departments that can be fully funded
// given a fixed budget, by always selecting the cheapest departments first.
int countFundableDepartments(const std::vector<int>& requests, int budget) {
    if (budget == 0) {
        return 0;
    }
    if (requests.empty()) {
        return 0;
    }

    // Work on a sorted copy to avoid modifying the input
    std::vector<int> sortedRequests = requests;
    std::sort(sortedRequests.begin(), sortedRequests.end());

    int totalSpent = 0;
    int count = 0;

    for (int cost : sortedRequests) {
        if (totalSpent + cost <= budget) {
            totalSpent += cost;
            ++count;
        } else {
            break; // Costs are sorted; no further costs can fit
        }
    }

    return count;
}
// The core idea is a greedy approach: to fund the maximum number of departments with a fixed budget, you should always select the departments with the smallest costs. This is because any funded department consumes its full cost from the budget, and choosing a cheaper department leaves more budget for additional ones, which cannot reduce the count. First, sort a copy of the request costs in ascending order (do not modify the original). Then iterate through the sorted list, maintaining a running total of spent money and a count. For each cost, check if the current total plus this cost is less than or equal to the budget; if yes, add the cost to the total and increment the count; if no, break because all subsequent costs are larger (since it's sorted). Edge cases: an empty vector returns 0; a budget of 0 returns 0 (even if there are zero-cost requests, funding one would still be possible but the problem statement requires returning 0, so we must explicitly return 0 when budget is 0); duplicate costs are handled naturally by sorting and iterating. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the copy (or O(1) if the original vector could be modified, but we are required not to modify it).
