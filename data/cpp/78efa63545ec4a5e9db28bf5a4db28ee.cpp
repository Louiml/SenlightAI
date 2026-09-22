// Given a list of `n` positive integers (1 ≤ n ≤ 100), write a C++ function `minimumGroupCount` that returns the minimum number of groups into which all numbers can be partitioned. Each group is defined by a "representative" integer: once a representative is chosen, every number in the entire list that is divisible by that representative is assigned to that group and cannot be assigned to another group. A number can only be assigned to a group whose representative divides it, and every number must be assigned to exactly one group. The function should take a `std::vector<int>` and return the minimal possible number of groups. For example, for `{2, 3, 4, 6}`, one optimal grouping is groups with representatives 2 and 3: 2 divides 2, 4, 6; 3 divides 3, 6 (with 6 already taken by 2's group, so it doesn't matter), thus 2 groups. The solution must avoid using global variables and must be implemented as a single free function.

// The problem is essentially a greedy interval/cover problem. Since the numbers are positive integers and we can sort them, the key observation is that if we process numbers in ascending order, the smallest unassigned number must be chosen as a representative for a new group. Why? Because any larger number cannot divide a smaller number (since they are positive and sorted ascending), so the smallest unassigned number cannot be assigned to any group whose representative is larger. It could only be assigned to a group whose representative is a proper divisor, but all divisors smaller than it are already processed and either used as representatives or covered; if it's unassigned, no smaller divisor exists as a representative that covers it. Thus, we must start a new group with that smallest unassigned number. Once chosen, we mark it and all multiples of it as assigned. This greedy choice is optimal because any valid solution must have at least one group containing this smallest unassigned number, and using it as the representative covers the most possible numbers (all its multiples) among all choices that could cover it. The algorithm: sort the vector, iterate through each element, if not used, increment group count, then for all subsequent elements that are multiples of the chosen representative, mark them used. Edge cases: duplicate numbers—if a duplicate is not used, it becomes a representative, but duplicates of an already used number are already marked used when the first occurrence was processed. Single element yields one group. Time complexity is O(n^2) due to the inner loop over all elements for each new representative, but n ≤ 100 so fine. Space complexity is O(n) for the usage flag.

#include <vector>
#include <algorithm>

// Returns the minimum number of groups needed so that each group has a
// representative that divides all numbers assigned to that group.
int minimumGroupCount(std::vector<int> values) {
    std::sort(values.begin(), values.end());
    const int n = static_cast<int>(values.size());
    std::vector<bool> used(n, false);

    int groups = 0;
    for (int i = 0; i < n; ++i) {
        if (used[i]) continue;
        ++groups;
        const int rep = values[i];
        for (int j = i; j < n; ++j) {
            if (values[j] % rep == 0) {
                used[j] = true;
            }
        }
    }
    return groups;
}

#include <cassert>
#include <vector>

// Declare the function being tested
int minimumGroupCount(std::vector<int> values);

int main() {
    // Single element
    assert(minimumGroupCount({7}) == 1);
    // All divisible by 1
    assert(minimumGroupCount({1, 2, 3, 4, 5}) == 1);
    // Two coprime numbers
    assert(minimumGroupCount({2, 3}) == 2);
    // Example from description
    assert(minimumGroupCount({2, 3, 4, 6}) == 2);
    // Duplicates
    assert(minimumGroupCount({2, 2, 4}) == 1);
    // Larger coprime set
    assert(minimumGroupCount({5, 7, 11, 13}) == 4);
    // Mixed divisibility
    assert(minimumGroupCount({6, 2, 3, 9, 18}) == 2); // groups: 2 (covers 2,6,18), 3 (covers 3,9)
    // Already sorted input
    assert(minimumGroupCount({2, 4, 8, 16}) == 1);
    // Unsorted input
    assert(minimumGroupCount({10, 5, 2}) == 2); // 2 covers 2,10; 5 covers 5
    // Max values
    assert(minimumGroupCount({100, 50, 25, 5}) == 1);
    return 0;
}
