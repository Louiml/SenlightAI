/*
Write a C++ function that, given a positive integer N and a list of N pairs of integers (a, b) where each a is a unique integer from 1 to N (representing the rank of a candidate in one evaluation round) and each b is a positive integer (representing the rank in another evaluation round), returns the maximum number of candidates that can be selected such that no selected candidate is strictly worse than another selected candidate in both ranks. In other words, a candidate can be selected if, when candidates are sorted by their first rank (a), there exists no previously selected candidate with a lower second rank (b). The input pairs are given in unsorted order, and the function should process them from a vector of pairs. Constraints: N can be up to 100,000, and b values are also integers from 1 to N but may have duplicates. The function should return an integer count.
*/
#include <vector>

// Given a list of pairs (first_rank, second_rank) where first_rank is unique from 1..N,
// returns the maximum number of candidates that can be selected so that no selected
// candidate is strictly worse than another in both ranks. first_rank is used as index.
int maximumSelectableCandidates(const std::vector<std::pair<int, int>>& candidates) {
    if (candidates.empty()) return 0;
    int N = candidates.size(); // first_rank values are 1..N
    // Initialize an array to store second_rank indexed by first_rank (1-based).
    std::vector<int> secondByFirst(N + 1, 0);
    for (const auto& p : candidates) {
        secondByFirst[p.first] = p.second;
    }
    // First candidate (first_rank=1) is always selectable.
    int count = 1;
    int minSecond = secondByFirst[1];
    for (int first = 2; first <= N; ++first) {
        if (secondByFirst[first] < minSecond) {
            ++count;
            minSecond = secondByFirst[first];
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// Include the solution function here (not repeated for brevity but assume it is above).

int main() {
    // Sample: N=5, pairs: (1,4), (2,3), (3,2), (4,1), (5,5)
    std::vector<std::pair<int, int>> c1 = {{1,4},{2,3},{3,2},{4,1},{5,5}};
    assert(maximumSelectableCandidates(c1) == 4); // select 1,2,3,4 (ranks b: 4,3,2,1) but not 5.

    // All increasing b: (1,1),(2,2),(3,3) => only first selected.
    std::vector<std::pair<int, int>> c2 = {{1,1},{2,2},{3,3}};
    assert(maximumSelectableCandidates(c2) == 1);

    // All decreasing b: (1,5),(2,4),(3,3),(4,2),(5,1) => all selected.
    std::vector<std::pair<int, int>> c3 = {{1,5},{2,4},{3,3},{4,2},{5,1}};
    assert(maximumSelectableCandidates(c3) == 5);

    // Single candidate.
    std::vector<std::pair<int, int>> c4 = {{1,1}};
    assert(maximumSelectableCandidates(c4) == 1);

    // Unsorted input: (2,3),(1,1),(3,2) => select 1 and 3 (b:1 then 2? Actually b:1 then 2 is not less, so only 1? Let's check: a=1 b=1 selected, min=1; a=2 b=3 not selected; a=3 b=2 not selected, so only 1).
    std::vector<std::pair<int, int>> c5 = {{2,3},{1,1},{3,2}};
    assert(maximumSelectableCandidates(c5) == 1);

    // Another: (1,5),(2,4),(3,6),(4,2) => select 1,2,4 (b:5,4,2) => 3.
    std::vector<std::pair<int, int>> c6 = {{1,5},{2,4},{3,6},{4,2}};
    assert(maximumSelectableCandidates(c6) == 3);

    return 0;
}
// The problem is equivalent to the "New Recruit" or "TV Game" style problem where we have two rankings and need to find the maximum number of employees that can be hired such that for any two hired employees, one is strictly better in at least one rank. Since each first-rank value a is unique and ranges from 1 to N, we can map the first rank directly to an array indexed by a, storing the second rank b. Then we iterate through a from 1 to N. The first candidate (with a=1) is always selected, because there is no one before him. As we iterate, we maintain the minimum second rank among selected candidates. A candidate can be selected only if his b is smaller than the current minimum, because if his b is larger than or equal to the current min, then there exists a previously selected candidate who is better or equal in both ranks (since previous candidates have smaller a). When we select a new candidate, we update the minimum to that candidate's b. Edge cases: N=1 returns 1; if all b values are decreasing, all candidates are selected; if all b values are increasing, only the first is selected. Time complexity: O(N) because we fill an array of size N and traverse it once. Space complexity: O(N) for the array.
