// Write a C++ function named `countUniqueElements` that takes a `std::vector<int>` and returns the number of distinct integers in the vector after removing all duplicates. However, the function must use a `std::multiset` internally to solve the problem, specifically leveraging the fact that `multiset` stores duplicate elements. The task is to: (1) insert all elements of the vector into a `multiset`, (2) then count how many unique values remain after removing all but one occurrence of each duplicate. The function should return the count of unique elements. For example, for input `{1,1,2,2,3}`, the output should be `3`. The vector may be empty, in which case return `0`. Do not use any other container (like `set` or `unordered_set`) for the main logic—only `multiset` and basic iteration are allowed.
#include <cassert>
#include <vector>

int main() {
    assert(countUniqueElements({}) == 0);
    assert(countUniqueElements({5}) == 1);
    assert(countUniqueElements({1,1,1,1}) == 1);
    assert(countUniqueElements({1,2,3,4,5}) == 5);
    assert(countUniqueElements({3,1,2,3,2,1}) == 3);
    assert(countUniqueElements({-1,-1,0,0,2,2,2}) == 3);
    assert(countUniqueElements({7,7,8,9,8,7}) == 3);
    assert(countUniqueElements({100,200,100,200,300}) == 3);
    assert(countUniqueElements({1,1,2,2,3}) == 3);
    assert(countUniqueElements({5,4,3,2,1,1,2,3,4,5}) == 5);
}
#include <vector>
#include <set>

// Count distinct integers in a vector using a multiset.
int countUniqueElements(const std::vector<int>& input) {
    std::multiset<int> ms(input.begin(), input.end());
    
    if (ms.empty()) {
        return 0;
    }
    
    int uniqueCount = 1;
    auto it = ms.begin();
    int prev = *it;
    ++it;
    
    for (; it != ms.end(); ++it) {
        if (*it != prev) {
            ++uniqueCount;
            prev = *it;
        }
    }
    
    return uniqueCount;
}
// The solution involves placing every element of the input vector into a `std::multiset<int>`. Since a multiset stores elements in sorted order and allows duplicates, we can then iterate through the multiset using its iterator. The key observation is that consecutive equal elements appear adjacent in the multiset's sorted order. We can count unique values by traversing the multiset and incrementing the counter only when the current element differs from the previous one (or at the very first element). Alternatively, we can use `std::unique_copy` or simply compare each element with its predecessor. Edge cases: empty vector returns 0; a vector with all same elements returns 1. Time complexity is O(n log n) due to insertion into multiset, and space complexity is O(n) because the multiset stores all elements. No other data structures are needed for counting—only a single integer counter and a loop.
