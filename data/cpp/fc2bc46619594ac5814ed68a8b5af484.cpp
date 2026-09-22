Write a C++ function `long long countValidTeams(const std::vector<int>& ratings)` that returns the total number of ways to choose a team of exactly 3 soldiers from the given ratings list, where a team is valid if the ratings are strictly increasing OR strictly decreasing in the order of their positions in the array. Each soldier can be used at most once per team, and teams are distinguished only by the set of indices (not values). Handle arrays of length less than 3 by returning 0. The function should be efficient for input sizes up to 10,000 and must not use recursion or exponential-time enumeration. You may use dynamic programming with memoization and must ensure the result fits in a 64-bit signed integer.
The problem is a classic counting of triplets with monotonic ordering. The key observation is that for any middle element at index `j`, the number of increasing teams with `j` as the middle is the number of elements before `j` with smaller rating multiplied by the number of elements after `j` with larger rating. Similarly, for decreasing teams, it's elements before with larger rating times elements after with smaller rating. Summing this over all `j` gives the total count. This is O(n²) in the worst case using prefix/suffix counts computed naively, but we can do it in O(n²) with simple nested loops (for n up to 10,000 that's 100 million operations, acceptable). However, a more efficient O(n log n) approach using Fenwick trees exists, but for simplicity we use the O(n²) approach. Edge cases: n < 3 returns 0. Duplicate ratings are allowed but a team must be strictly increasing/decreasing, so equal values cannot be part of the same team. Time complexity O(n²), space O(1) auxiliary if we compute on the fly (no extra arrays besides input). We'll implement a direct triple loop counting all increasing and decreasing triplets: for each pair (i,j) with i<j, count how many k>j satisfy the condition. That's O(n³) which is too slow. Instead, we use the middle-element method: for each j, count left smaller/larger and right larger/smaller. To compute left counts efficiently we can precompute prefix counts using a frequency array if ratings are in a limited range, but since ratings can be arbitrary integers we use a nested loop per j (O(n) per j, total O(n²)). That's acceptable for n=10,000 (10^8 operations, borderline but fine in C++ with optimizations). We'll implement that.
#include <vector>

// Count teams of 3 with strictly increasing or strictly decreasing ratings.
// Time: O(n^2), Space: O(1) auxiliary (excluding input).
long long countValidTeams(const std::vector<int>& ratings) {
    const std::size_t n = ratings.size();
    if (n < 3) return 0;

    long long totalTeams = 0;

    // For each possible middle element at index j.
    for (std::size_t j = 1; j + 1 < n; ++j) {
        int leftSmaller = 0;
        int leftLarger = 0;
        for (std::size_t i = 0; i < j; ++i) {
            if (ratings[i] < ratings[j]) ++leftSmaller;
            else if (ratings[i] > ratings[j]) ++leftLarger;
        }

        int rightSmaller = 0;
        int rightLarger = 0;
        for (std::size_t k = j + 1; k < n; ++k) {
            if (ratings[k] < ratings[j]) ++rightSmaller;
            else if (ratings[k] > ratings[j]) ++rightLarger;
        }

        // Increasing: left smaller * right larger
        totalTeams += static_cast<long long>(leftSmaller) * rightLarger;
        // Decreasing: left larger * right smaller
        totalTeams += static_cast<long long>(leftLarger) * rightSmaller;
    }

    return totalTeams;
}
#include <cassert>
#include <vector>

// The solution function is declared above (include the code from solution section).

int main() {
    // Example from typical LeetCode problem: [2,5,3,4,1] -> 3 teams
    // Increasing: (2,3,4), (2,5,3? no), (2,5,4? no), (2,3,4) yes, (2,4? wait)
    // Let's list: indices (0,2,3) ratings 2<3<4; (0,1? 2<5>4 no); actually (0,1,? ) 2<5>... no
    // Valid increasing: (0,2,3) = 2,3,4; (0,? actually 2<5? no for triple)
    // Decreasing: (2,5,4) indices (0,1,3); (2,5,1) indices (0,1,4); (2,4,1) indices (0,3,4); (5,3,1) indices (1,2,4); (5,4,1) indices (1,3,4); (3,? actually 3>4? no) 
    // Let's trust the known answer: For [2,5,3,4,1] the answer is 3.
    std::vector<int> r1 = {2,5,3,4,1};
    assert(countValidTeams(r1) == 3);

    // All increasing
    std::vector<int> r2 = {1,2,3,4};
    // Increasing triples: (0,1,2), (0,1,3), (0,2,3), (1,2,3) = 4; decreasing = 0 → total 4
    assert(countValidTeams(r2) == 4);

    // All decreasing
    std::vector<int> r3 = {4,3,2,1};
    // Decreasing triples: (0,1,2), (0,1,3), (0,2,3), (1,2,3) = 4; increasing = 0 → total 4
    assert(countValidTeams(r3) == 4);

    // Less than 3
    assert(countValidTeams({1,2}) == 0);
    assert(countValidTeams({5}) == 0);
    assert(countValidTeams({}) == 0);

    // Duplicates: [1,1,1] should be 0 because strictly increasing/decreasing
    assert(countValidTeams({1,1,1}) == 0);

    // Mixed: [1,3,2] 
    // Increasing: (0,1? 1<3>2 no), (0,2? 1<2 yes but need middle? Actually triple only 3 elements, so middle is index1=3: left smaller=1 (index0=1), right larger= none, right smaller=index2=2 → increasing teams=1*0=0; decreasing teams: left larger none, right smaller=1 →0. So total 0? Let's check manually: all triples of size 3: only (1,3,2) not increasing (1<3>2) not decreasing (1<3>2) so 0.
    assert(countValidTeams({1,3,2}) == 0);

    // Larger case with known count: [5,4,3,2,1] total 10 (choose any 3 decreasing)
    std::vector<int> r7 = {5,4,3,2,1};
    assert(countValidTeams(r7) == 10);

    // [1,2,3] -> exactly 1 increasing
    assert(countValidTeams({1,2,3}) == 1);

    // [3,2,1] -> exactly 1 decreasing
    assert(countValidTeams({3,2,1}) == 1);

    return 0;
}
