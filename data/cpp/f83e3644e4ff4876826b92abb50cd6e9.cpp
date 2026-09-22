/*
Write a C++ function that, given a vector of pairs of integers representing the home color and guest color of each team in a local soccer league, returns the total number of times a home team’s uniform color matches a guest team’s uniform color during a round-robin tournament (each team plays every other team exactly once, and in each match the first team is home and the second is guest). The function should count only matches where the home team’s first color equals the guest team’s second color. Teams are indexed from 0 to n-1, and the input vector has exactly n pairs. The function must handle n = 0 and n = 1 gracefully (returning 0 in both cases). No input validation is required; assume all integers are in the range [-10^9, 10^9].
*/

#include <vector>
#include <utility>

// Count ordered pairs (i, j) with i != j where home color of i equals guest color of j.
long long countHomeGuestMatches(const std::vector<std::pair<int, int>>& uniforms) {
    long long count = 0;
    const size_t n = uniforms.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i != j && uniforms[i].first == uniforms[j].second) {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the free function here or in a header above.

int main() {
    // Example from the snippet: n=3, uniforms = {(1,0),(0,1),(1,1)}
    // Matches: (0,1): 1==1? yes; (0,2): 1==1? yes; (1,0): 0==0? yes; others no → count=3
    std::vector<std::pair<int, int>> v1 = {{1,0},{0,1},{1,1}};
    assert(countHomeGuestMatches(v1) == 3);

    // n=0
    std::vector<std::pair<int, int>> v2;
    assert(countHomeGuestMatches(v2) == 0);

    // n=1
    std::vector<std::pair<int, int>> v3 = {{5,5}};
    assert(countHomeGuestMatches(v3) == 0);

    // n=2 with one match
    std::vector<std::pair<int, int>> v4 = {{2,3},{3,2}};
    // (0,1): 2==2? yes; (1,0): 3==3? yes → count=2
    assert(countHomeGuestMatches(v4) == 2);

    // n=2 no match
    std::vector<std::pair<int, int>> v5 = {{1,2},{3,4}};
    assert(countHomeGuestMatches(v5) == 0);

    // Duplicate colors and many matches
    std::vector<std::pair<int, int>> v6 = {{7,9},{7,1},{8,7}};
    // (0,1): 7==1? no; (0,2): 7==7? yes; (1,0): 7==9? no; (1,2): 7==7? yes; (2,0): 8==9? no; (2,1): 8==1? no → count=2
    assert(countHomeGuestMatches(v6) == 2);

    // Negative numbers
    std::vector<std::pair<int, int>> v7 = {{-1,-2},{-2,-1},{-1,-1}};
    // (0,1): -1==-1? yes; (0,2): -1==-1? yes; (1,0): -2==-2? yes; (1,2): -2==-1? no; (2,0): -1==-2? no; (2,1): -1==-1? yes → count=3
    assert(countHomeGuestMatches(v7) == 3);

    return 0;
}

// The problem requires counting ordered pairs (i, j) with i ≠ j such that uniforms[i].first == uniforms[j].second. This is a direct simulation of the double loop in the original snippet. The main algorithm: iterate over all i from 0 to n-1, and for each i iterate over all j from 0 to n-1, skipping when i == j, and increment a counter when the condition holds. Edge cases: when n = 0 or n = 1, the loops simply do not execute or the skip condition handles the single element, so the counter stays 0. Duplicate values are fine; each ordered pair is counted separately even if the same color appears multiple times. Time complexity is O(n^2) because of the nested loops; space complexity is O(1) auxiliary, not counting the input vector storage. A more efficient map-based approach could count frequencies of guest colors, but the straightforward nested loop matches the given snippet and is acceptable for small n.
