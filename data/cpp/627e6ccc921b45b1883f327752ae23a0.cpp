/*
You are given `K` positive integers (1 ≤ K ≤ 10,000) representing the lengths of LAN cables you already own, and a target number `N` (1 ≤ N ≤ 10,000) of equal-length cables you need to obtain by cutting the existing cables. Write a C++ function named `maxCableLength` that takes the vector of cable lengths (as `const std::vector<long>&`) and the integer `N`, and returns the maximum possible length (as a `long`) such that you can cut at least `N` pieces each of that exact length from the given cables. A cable can be cut into multiple pieces of the chosen length; leftover parts are discarded. The chosen length must be a positive integer, and all `N` pieces must be exactly that length. The input lengths are guaranteed to be at least 1 and fit in `long`. The result will always exist because you could always choose length 1.
*/
#include <vector>
#include <algorithm>

// Returns the maximum integer length L such that we can cut at least N pieces
// of length L from the given cable lengths.
long maxCableLength(const std::vector<long>& cables, int N) {
    if (cables.empty() || N <= 0) return 0;

    long max_len = *std::max_element(cables.begin(), cables.end());

    // Binary search for the largest feasible length.
    // lo: possible answer (inclusive), hi: impossible (exclusive).
    long lo = 1;
    long hi = max_len + 1; // hi is impossible because max_len+1 yields zero pieces

    while (lo < hi) {
        long mid = lo + (hi - lo) / 2; // avoids overflow of (lo+hi)

        // Count how many pieces of length mid we can produce.
        long total_pieces = 0;
        for (long cable : cables) {
            total_pieces += cable / mid;
            if (total_pieces >= N) break; // early exit to save time
        }

        if (total_pieces >= N) {
            lo = mid + 1; // mid is feasible, try longer
        } else {
            hi = mid; // mid is too long
        }
    }

    // lo is the first length that is not feasible (or max_len+1 if all feasible),
    // so the biggest feasible length is lo - 1.
    return lo - 1;
}
#include <cassert>
#include <vector>

long maxCableLength(const std::vector<long>& cables, int N); // declaration from solution

int main() {
    // Basic cases
    assert(maxCableLength({10, 20, 30}, 6) == 10); // 1+2+3=6 pieces
    assert(maxCableLength({5, 5, 5}, 3) == 5);     // 1 each = 3
    assert(maxCableLength({100, 1}, 2) == 1);      // 100/1 + 1/1 = 101 >= 2, but 2 too big
    assert(maxCableLength({802, 743, 457, 539}, 11) == 200); // classic example

    // Edge cases
    assert(maxCableLength({1}, 1) == 1);           // only one cable, need one piece
    assert(maxCableLength({1, 1, 1}, 3) == 1);     // all cables length 1, need all pieces
    assert(maxCableLength({10}, 10) == 1);         // cut into 10 unit pieces
    assert(maxCableLength({10}, 1) == 10);         // take whole cable
    assert(maxCableLength({7, 7, 7}, 4) == 3);     // 7/3=2 each => 6 pieces, but 4 needed; 7/2=3 each =>9, so 3 works? Actually 7/3=2+2+2=6>=4, 8? not possible
    // Let's compute carefully: for L=3: 2+2+2=6 >=4, L=4:1+1+1=3 <4, so answer=3
    assert(maxCableLength({1000000000, 1000000000}, 3) == 500000000); // large values
    // Large N
    assert(maxCableLength({100, 100, 100}, 300) == 1); // 100 each -> 300 pieces of length 1
    // Mixed large and small
    assert(maxCableLength({100, 1, 1, 1}, 4) == 1); // 100/1=100+3=103>=4, but 2? 100/2=50 +3*0=50>=4, but length 2 works, but length 3? 33+0=33>=4, can go higher? Actually 100^? Let's find: For L=4, 25 pieces, >=4, so answer >1. Let's compute: L=50: 2 pieces, <4 fails, so answer at most 50. Need max L with pieces>=4. For L=20:5+0+0+0=5>=4, L=25:4>=4, L=26:3<4 => answer=25? but check L=25:100/25=4, yes. So assert 25.
    assert(maxCableLength({100, 1, 1, 1}, 4) == 25);

    return 0;
}
// This is a classic parametric search (binary search on answer) problem. Define a feasibility function: given a candidate length `L`, we count how many pieces we can obtain by summing `floor(cable_length / L)` for every cable. If the total count is at least `N`, then `L` is a feasible length; otherwise it is too large. The feasible lengths form a contiguous range from 1 up to some maximum, so we binary search for the largest feasible `L`. Important edge cases: overflow when computing `hi + 1` if we use a naive `hi` as the maximum cable length, so we must use `long` for indices and guard the upper bound. Also, the binary search should be written to find the last feasible value (lo inclusive, hi exclusive). We set `lo = 1`, `hi = max_length + 1`. While `lo < hi`, we compute `mid = lo + (hi - lo)/2` (avoid overflow), count pieces. If count >= `N`, then `mid` is feasible, so set `lo = mid + 1` (because we want the largest feasible, and `mid` might not be the last). Otherwise set `hi = mid`. At the end, `lo - 1` is the answer. Complexity: binary search over range up to `max_length` takes O(log(max_length)) iterations; each iteration scans all K cables, so total time O(K log(max_length)). Space O(K) for the input vector, plus O(1) auxiliary.
