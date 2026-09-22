// Write a C++ function named `simulate_card_placement` that takes three integers: `n` (the number of positions ranging from 1 to \(n\)), `m` (the number of turns to simulate), and `c` (an even positive integer representing a threshold), along with a vector `queries` of length `m` where each query is an integer in the inclusive range \([1, c]\). The function must simulate an interactive greedy placement algorithm (without actual I/O) and return a vector of integers representing the position (1-indexed) chosen for each query in order. The rules are: maintain an initially empty array `d` of size `n`. For each query `x`, if `x < c/2`, then place it into the leftmost available position such that it is strictly smaller than the value already there, scanning from index 0 upward; if no such existing value is less than `x` among the already-used positions on the left, place it at the leftmost empty position (tracked by a pointer `l`). If `x >= c/2`, do the symmetric: scan from the rightmost used position downward to find a position where the existing value is strictly greater than `x`, and if found place there; otherwise place at the rightmost empty position (tracked by pointer `r`) and decrement `r`. After each placement, if the entire array becomes filled (`l == r`), stop processing further queries (ignore any remaining queries). The function must return only the positions chosen, in order, up to the point where the array is filled (inclusive of the last placement that fills it). If `m` is zero or `n` is zero, return an empty vector. Edge cases: duplicates, values equal to `c/2` (treated as the "right" branch), and queries exceeding `c` need not be handled—assume inputs are valid.
// The task is a direct simulation of the given snippet's logic but encapsulated into a pure function. The main algorithm maintains two pointers: `l` (number of positions filled from the left) and `r` (number of positions filled from the right, equivalently the next empty index from the right). The array `d` of size `n` is initialized to some sentinel (e.g., -1) or we can just keep track of filled positions. For each query `x` (converted to 0-index inside function), if `x < c/2` (using integer arithmetic; note `c` is even so `c/2` is exact), we scan indices `0` to `l-1` to find the first index `i` such that `d[i]` is already filled and `x < d[i]`. If found, place there and output position `i+1`. If not found, place at index `l` (assuming `l < n`), then increment `l`. For `x >= c/2`, we scan indices `n-1` down to `r` to find the first index `i` (from right) such that `d[i]` is filled and `d[i] < x`. If found, place there. Otherwise place at index `r-1` (since `r` is the boundary meaning the rightmost empty index is `r-1` after decrement), then decrement `r`. The process stops when `l == r` (meaning all positions from `0` to `l-1` and from `r` to `n-1` fill the whole array). Important edge cases: (1) If `x` equals `c/2`, it goes to the right branch. (2) If no empty position is available (`l == r` initially when `n==0`), we return empty. (3) After placing at the boundary, we must check if the array is full before continuing; if so, break. (4) If the query vector is longer than needed, we ignore the rest. (5) For scanning, we must ensure we only look at filled positions: for left branch, positions `0..l-1` are always filled because we only increment `l` after placing at `l`; for right branch, positions `r..n-1` are filled, because we decrement `r` after placing at `r-1`. Time complexity: For each query, the scan in the worst case is \(O(n)\), and there are at most `m` queries, but the loop stops after at most `n` placements, so worst-case \(O(n^2)\) if `m` is large but the array fills slowly. Space complexity: \(O(n)\) for the array `d` and \(O(m)\) for the result (which could be up to `n`). This is acceptable and matches the reference algorithm.
#include <vector>
#include <algorithm>

// Simulates the card placement algorithm.
// n: number of positions, m: max number of queries to consider,
// c: even threshold, queries: list of integers in [1, c] (length >= m).
// Returns the 1-indexed positions chosen for each query until the array is full.
std::vector<int> simulate_card_placement(int n, int m, int c, const std::vector<int>& queries) {
    std::vector<int> result;
    if (n <= 0 || m <= 0 || queries.empty()) {
        return result;
    }

    // d[i] stores the value placed at position i; -1 means empty.
    std::vector<int> d(n, -1);
    int l = 0;          // left boundary: positions [0, l-1] are filled.
    int r = n;          // right boundary: positions [r, n-1] are filled.

    int processed = 0;
    while (processed < m && processed < static_cast<int>(queries.size())) {
        if (l == r) {
            break; // array is full
        }
        int x = queries[processed] - 1; // convert to 0-indexed value
        if (x < c / 2) {
            // Left branch: find first filled position with value > x
            int place_index = -1;
            for (int i = 0; i < l; ++i) {
                if (d[i] > x) { // note: we compare with existing value
                    place_index = i;
                    break;
                }
            }
            if (place_index == -1) {
                // Place at leftmost empty position
                place_index = l;
                d[place_index] = x;
                ++l;
            } else {
                d[place_index] = x;
            }
            result.push_back(place_index + 1);
        } else {
            // Right branch: find first filled position from right with value < x
            int place_index = -1;
            for (int i = n - 1; i >= r; --i) {
                if (d[i] < x) {
                    place_index = i;
                    break;
                }
            }
            if (place_index == -1) {
                // Place at rightmost empty position
                place_index = r - 1;
                d[place_index] = x;
                --r;
            } else {
                d[place_index] = x;
            }
            result.push_back(place_index + 1);
        }
        ++processed;
    }
    return result;
}
#include <cassert>
#include <vector>

// Function under test (include the solution above in the same file)
// simulate_card_placement as defined.

int main() {
    // Basic case: n=3, m=3, c=4, queries [1,2,3]
    // c/2 = 2. x=1 (left), x=2 (right? since 2>=2), x=3 (right)
    {
        std::vector<int> q = {1, 2, 3};
        std::vector<int> res = simulate_card_placement(3, 3, 4, q);
        // Expected:
        // x=1 (left): no filled left, place at index0 -> pos1
        // d=[1,-1,-1], l=1, r=3
        // x=2 (right): scan from n-1=2 down to r=3? none, place at r-1=2 -> pos3
        // d=[1,-1,2], l=1, r=2
        // x=3 (right): scan from n-1=2 down to r=2: d[2]=2 < 3, so place at index2 -> pos3 (overwrite)
        // Array not full? l=1, r=2 -> not equal. Continue, but m exhausted.
        // Result: [1,3,3]
        assert(res == std::vector<int>({1, 3, 3}));
    }

    // Fill to exactly n placements
    {
        std::vector<int> q = {1, 1, 1};
        // c=4, c/2=2, all x=1 <2 => left branch.
        // n=3, m=3
        // x=1 (0-index) left: no filled, place at l=0 -> pos1, l=1
        // x=1 again: left branch, scan i=0: d[0]=1 >1? no, place at l=1 -> pos2, l=2
        // x=1 again: scan i=0,1: d[0]=1>1? no, d[1]=1>1? no, place at l=2 -> pos3, l=3
        // now l=3, r=3 => full, stop. Result [1,2,3]
        std::vector<int> res = simulate_card_placement(3, 3, 4, q);
        assert(res == std::vector<int>({1, 2, 3}));
    }

    // More queries than needed, stop early.
    {
        std::vector<int> q = {2, 2, 2, 2}; // c=4, c/2=2, all are right branch
        // n=2, m=4
        // x=2 (0-index) right: scan from n-1=1 down to r=2? none, place at r-1=1 -> pos2, r=1
        // x=2 again: scan from 1 down to 1: d[1]=2 <2? no, place at r-1=0 -> pos1, r=0
        // now l=0, r=0 => full. Result [2,1]
        std::vector<int> res = simulate_card_placement(2, 4, 4, q);
        assert(res == std::vector<int>({2, 1}));
    }

    // n=0
    {
        std::vector<int> q = {1};
        assert(simulate_card_placement(0, 1, 4, q).empty());
    }

    // m=0
    {
        std::vector<int> q = {1, 2};
        assert(simulate_card_placement(3, 0, 4, q).empty());
    }

    // Scenario with overwrite in left branch
    {
        std::vector<int> q = {3, 1}; // c=6, c/2=3, so x=3 goes right, x=1 goes left
        // n=3, m=2
        // x=3 (right): scan from 2 down to 3? none, place at r-1=2 -> pos3, r=2
        // d=[-1,-1,3], l=0, r=2
        // x=1 (left): scan i=0 to l-1= -1? none, place at l=0 -> pos1, l=1
        // result [3,1]
        std::vector<int> res = simulate_card_placement(3, 2, 6, q);
        assert(res == std::vector<int>({3, 1}));
    }

    // Full array with mixed branches and overwrite on right
    {
        std::vector<int> q = {2, 3, 2}; // c=4, c/2=2
        // n=3, m=3
        // x=2 (right): scan from 2 down to 3? none, place at 2 -> pos3, r=2
        // d=[-1,-1,2], l=0, r=2
        // x=3 (right): scan from 2 down to 2: d[2]=2 <3, place at index2 -> pos3, overwrite
        // d=[-1,-1,3], l=0, r=2
        // x=2 (right): scan from 2 down to 2: d[2]=3 <2? no, place at r-1=1 -> pos2, r=1
        // d=[-1,2,3], l=0, r=1
        // not full (l=0,r=1), but m exhausted. result [3,3,2]
        std::vector<int> res = simulate_card_placement(3, 3, 4, q);
        assert(res == std::vector<int>({3, 3, 2}));
    }

    return 0;
}
