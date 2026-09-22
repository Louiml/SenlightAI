// Given a positive integer `T` representing the number of test cases, followed by `T` pairs of positive integers `(a, b)`, write a C++ function that computes for each pair an intermediate cumulative score: after reading the first pair, the cumulative score for player 1 is `a + a = 2a` and for player 2 is `b + b = 2b`. For each subsequent pair, the cumulative scores are updated by adding the new `a` to the previous cumulative score for player 1 and the new `b` to the previous cumulative score for player 2. Then, among all test cases (starting from the first), determine the maximum absolute difference between the two cumulative scores at any point, and return a string that indicates which player had the lead at that moment: output `"1 <diff>"` if player 1’s cumulative score was greater, or `"2 <diff>"` if player 2’s cumulative score was greater, where `<diff>` is the maximum absolute difference. If there is a tie at the maximum difference, the first occurrence (earliest test index) wins. The function should take two vectors (or arrays) of equal length containing the `a` and `b` values for each test case, and return the result string.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example: T=5, pairs: (1,0), (2,1), (3,2), (4,3), (5,4)
    // Cumulative sums: player1: 1,3,6,10,15; player2:0,1,3,6,10
    // Diffs: 1,2,3,4,5 -> max is 5 at last case, winner 1
    assert(maxLead({1,2,3,4,5}, {0,1,2,3,4}) == "1 5");

    // Player 2 takes a big lead: (0,1), (0,2), (0,3)
    // Cumulative: p1=0,0,0; p2=1,3,6; diffs=-1,-3,-6 -> max 6, winner 2
    assert(maxLead({0,0,0}, {1,2,3}) == "2 6");

    // Single test case, tie at 0: (5,5)
    assert(maxLead({5}, {5}) == "1 0"); // diff=0, abs=0, first wins, winner=1

    // Mixed leads: (10,1), (0,100) 
    // After first: p1=10,p2=1 diff=9 -> max=9, winner=1
    // After second: p1=10,p2=101 diff=-91 -> abs=91, winner=2
    assert(maxLead({10,0}, {1,100}) == "2 91");

    // Equal maximum differences: (5,0), (0,5), (5,0)
    // Cum sums: (5,0) diff=5; (5,5) diff=0; (10,5) diff=5 -> max=5 at first occurrence, winner=1
    assert(maxLead({5,0,5}, {0,5,0}) == "1 5");

    // All zeros: (0,0),(0,0) -> maxDiff=0 at first, winner=1
    assert(maxLead({0,0}, {0,0}) == "1 0");

    // Negative differences but larger later: (1,2),(3,0)
    // After first: p1=1,p2=2 diff=-1 -> max=1, winner=2
    // After second: p1=4,p2=2 diff=2 -> max=2, winner=1
    assert(maxLead({1,3}, {2,0}) == "1 2");
    
    return 0;
}
#include <string>
#include <vector>
#include <cstdlib> // for std::abs

// Given vectors of a and b values for each test case, return a string
// indicating which player had the maximum lead and the lead value.
std::string maxLead(const std::vector<int>& a, const std::vector<int>& b) {
    int n = a.size();
    int sumA = 0, sumB = 0;
    int maxDiff = -1;
    int winner = 1; // default, will be updated
    int lead = 0;

    for (int i = 0; i < n; ++i) {
        sumA += a[i];
        sumB += b[i];
        int diff = sumA - sumB;
        int absDiff = std::abs(diff);
        if (absDiff > maxDiff) {
            maxDiff = absDiff;
            winner = (diff > 0) ? 1 : 2;
            lead = absDiff;
        }
    }
    return std::to_string(winner) + " " + std::to_string(lead);
}
// The solution processes the input sequentially, maintaining cumulative sums `sumA` and `sumB` for player 1 and player 2 respectively. For each test case index `i`, update `sumA += a[i]` and `sumB += b[i]`. Compute the difference `diff = sumA - sumB`. If `diff > 0`, the leader is player 1 with lead `diff`; if `diff < 0`, the leader is player 2 with lead `-diff`. Track the maximum absolute difference seen so far, `maxDiff`. On a tie (i.e., a new difference equals the current max), we keep the earlier occurrence because we only update when strictly greater. The initial value of `maxDiff` should be set to `-1` so that the first test case always becomes the maximum. Time complexity is O(T) since we scan the arrays once. Space complexity is O(1) auxiliary, ignoring the input storage. Edge cases: T=1 works fine; negative differences are handled by absolute value; all zeros would lead to a difference of 0, which is correctly handled—since maxDiff starts at -1, the first case (even with 0) becomes the answer, and the leader is determined by the sign of the difference (if 0, both are equal, but the problem implies one must be chosen; we arbitrarily output "1 0" or "2 0"? The original code compares strictly greater, so for equal cumulative sums it would not update, but since maxDiff starts from the first difference, we can choose to output "1" if diff >= 0 else "2", or handle ties by preferring player 1. To be robust, if diff == 0 and maxDiff == 0, we keep the earlier occurrence. In our implementation, we update only if `abs(diff) > maxDiff`, so for equal differences we don’t change. That naturally preserves the first occurrence. For diff=0, `abs(diff)=0` is greater than initial `maxDiff=-1`, so first case wins. For subsequent zeros, they don’t update, preserving the first. Thus the function returns the first occurrence of the maximum absolute difference.
