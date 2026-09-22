Write a standalone C++ function `shipment_possible` that takes the length of a 1D cargo train \(n\), the minimum number of loaded carriages \(k\), the maximum length of a contiguous empty block allowed \(a\), a sequence of \(m\) queries \(p_1, p_2, \dots, p_m\) where each query loads one carriage at position \(p_i\), and returns the earliest query index \(i\) such that after processing queries \(1\) through \(i\) (inclusive), it becomes impossible to have at least \(k\) loaded carriages, given that each loaded carriage must be separated by at most \(a\) empty carriages (i.e., any contiguous block of empty carriages must have length at most \(a\)). If this never happens during all \(m\) queries, return \(-1\). Positions are 1-indexed from left to right. Assume \(1 \le k \le n\), \(1 \le a \le n\), \(1 \le m \le 10^5\), and all positions are distinct integers between 1 and \(n\). The function should simulate the queries efficiently using an ordered set to track already loaded positions and maintain a count of how many valid loaded carriages are possible as a "game" where cargo is only possible at positions that are not in an overlong empty block; effectively, the maximum number of loads you can place given the empty-block constraint is the sum over each gap between consecutive loaded positions (including sentinels at 0 and \(n+1\)) of \(\lfloor (gap\_length) / (a+1) \rfloor\). Return the earliest index where this total drops below \(k\), otherwise \(-1\).

The key observation is that the condition "any contiguous empty block must have length ≤ a" means that between two loaded carriages (or between the train ends and the nearest loaded carriage), the maximum number of additional loads you could place is \(\lfloor (gap)/(a+1) \rfloor\), because in a gap of length \(L\) (number of empty carriage positions between two loaded ones), the pattern for maximal packing is to place a loaded carriage at positions separated by exactly \(a+1\) from the left boundary, then repeat; the floor division gives how many full groups of \(a+1\) positions fit. Initially, there are no loaded carriages, so the entire train from 1 to \(n\) is a single gap of length \(n+2\)? Actually we treat sentinels at 0 and \(n+1\) as loaded, so the initial gap length between them is \(n+1\) positions from 1 to n inclusive, so the initial total is \(f(0, n+1) = f(left=0, right=n+1)\) where the gap length is \(right - left - 1\)? In the snippet, `f(l,r)` returns `(r-l)/(a+1)` which when positions are integers, and the gap between sentinels at `l` and `r` has `r-l-1` empty positions; but `(r-l)/(a+1)` actually counts how many loads can fit in the interval of integer positions strictly between `l` and `r` when we can place loads such that each load is at least `a+1` apart and adjacent to the boundaries. This works for integer division because the number of interior positions that can be occupied is floor((r-l-1+1)/(a+1)) = floor((r-l)/(a+1)). So we maintain a sorted set of loaded positions (with sentinels 0 and n+1). When a new position `p` is loaded, we find its neighbors `L` (the previous loaded position) and `R` (the next), and we subtract the previous contribution `f(L, R)` and add `f(L, p) + f(p, R)`. We keep a running total `ready`. Initially `ready = f(0, n+1)`. After each insertion, if `ready < k`, we return that query index. If we finish all queries without going below `k`, return -1. Edge cases: positions are distinct, so no duplicates; sentinels are never queried; `a` can be 0? But given constraints `a≥1`, so fine. The complexity is O(m log m) because each insertion and neighbor lookup is O(log m) with an ordered set, and each update is O(1). Space is O(m) for the set.

#include <set>
#include <algorithm>

// Returns the maximum number of cargo loads that can be placed in the
// contiguous interval (l, r) given that empty blocks can have length at most a.
// l and r are sentinel positions (0 and n+1) or actual loaded positions.
static int max_loads_between(int l, int r, int a) {
    // Number of empty positions between l and r is (r - l - 1).
    // Each load requires one position and at least a empty after it.
    // Equivalent to floor((r - l) / (a + 1)).
    return (r - l) / (a + 1);
}

// Simulates loading carriages one by one and finds the earliest query index
// (1-based) after which it becomes impossible to have at least k loads.
// Returns that index, or -1 if it never happens.
int shipment_possible(int n, int k, int a, const std::vector<int>& queries) {
    std::set<int> loaded;
    loaded.insert(0);
    loaded.insert(n + 1);
    
    int current_total = max_loads_between(0, n + 1, a);
    
    for (size_t i = 0; i < queries.size(); ++i) {
        const int pos = queries[i];
        auto it = loaded.lower_bound(pos);
        const int right = *it;
        --it;
        const int left = *it;
        
        // Remove the old gap's contribution, add the two new gaps'.
        current_total -= max_loads_between(left, right, a);
        current_total += max_loads_between(left, pos, a);
        current_total += max_loads_between(pos, right, a);
        
        if (current_total < k) {
            return static_cast<int>(i + 1); // 1-based index
        }
        
        loaded.insert(pos);
    }
    
    return -1;
}

#include <cassert>
#include <vector>

// Forward declaration of the function being tested.
int shipment_possible(int n, int k, int a, const std::vector<int>& queries);

int main() {
    // Example: n=5, k=3, a=1, queries load positions 1,3,5.
    // Initially can place 3 loads (positions 1,3,5). After loading 1,3,5, still 3 loads.
    // So never below 3, return -1.
    assert(shipment_possible(5, 3, 1, {1, 3, 5}) == -1);
    
    // n=5, k=3, a=1, queries 2,4. After loading 2, we still can place loads at 1,3,5? 
    // Gap (0,2) length 2: floor(2/2)=1 -> position 1. Gap (2,6) length 4: floor(4/2)=2 -> 3 and 5. Total 3 -> ok.
    // After loading 4, gaps: (0,2)=1, (2,4)=1, (4,6)=1? Actually (4,6) length 2 -> floor(2/2)=1 -> 5. Total 1+1+1=3 still ok. So -1.
    assert(shipment_possible(5, 3, 1, {2, 4}) == -1);
    
    // n=5, k=3, a=1, queries 2,3. 
    // After 2: total 3. After 3: gaps (0,2)=1, (2,3)=0, (3,6)= floor(3/2)=1 -> total 2 <3, so return 2.
    assert(shipment_possible(5, 3, 1, {2, 3}) == 2);
    
    // n=1, k=1, a=1, query {1} -> initial total = floor(2/2)=1. After loading 1, gaps (0,1)=0, (1,2)=0 total=0 <1 -> return 1.
    assert(shipment_possible(1, 1, 1, {1}) == 1);
    
    // n=10, k=5, a=0? a must be >=1, so use a=1. 
    // Load positions 2,4,6,8,10. Check early.
    // With a=1, max loads in entire train: floor(11/2)=5. After loading 2: gaps (0,2)=1, (2,11)= floor(9/2)=4 total=5 ok.
    // After 4: gaps (0,2)=1, (2,4)=1, (4,11)= floor(7/2)=3 total=5 ok.
    // After 6: (0,2)=1, (2,4)=1, (4,6)=1, (6,11)= floor(5/2)=2 total=5 ok.
    // After 8: (0,2)=1, (2,4)=1, (4,6)=1, (6,8)=1, (8,11)= floor(3/2)=1 total=5 ok.
    // After 10: (0,2)=1, (2,4)=1, (4,6)=1, (6,8)=1, (8,10)=1, (10,11)=0 total=5 ok. So -1.
    assert(shipment_possible(10, 5, 1, {2,4,6,8,10}) == -1);
    
    // Same but k=6. After loading 10 total=5 <6 -> return 5.
    assert(shipment_possible(10, 6, 1, {2,4,6,8,10}) == 5);
    
    // n=3, k=2, a=1, queries: 2. 
    // Initial total = floor(4/2)=2. After loading 2: gaps (0,2)=1, (2,4)=1 total=2 ok. -1.
    // Then if query 2,1: after 2 ok, then after 1: gaps (0,1)=0, (1,2)=0, (2,4)=1 total=1 <2 -> return 2.
    assert(shipment_possible(3, 2, 1, {2, 1}) == 2);
    
    // Edge case: a large, e.g., n=100, k=1, a=100. Any load will reduce total below 1? 
    // Initial total = floor(101/101)=1. After loading any pos, gaps sum to less than 1? 
    // Actually if a>=n, total= floor((n+1)/(a+1)) could be 1 if n+1 >= a+1. 
    // Load pos=1: gaps (0,1)=0, (1,101)= floor(100/101)=0 total=0 <1 -> returns 1.
    assert(shipment_possible(100, 1, 100, {1}) == 1);
    
    // Large number of queries, ensure no crash.
    std::vector<int> many;
    for (int i = 1; i <= 50; ++i) many.push_back(i);
    int result = shipment_possible(50, 50, 1, many);
    // With a=1, max possible loads in 50 is floor(51/2)=25 <50, so it should fail at first insertion.
    assert(result == 1);
    
    return 0;
}
