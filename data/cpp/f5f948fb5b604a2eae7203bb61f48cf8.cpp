/*
Write a standalone C++ function that takes exactly 10 integers (each in the range [1, 10000]) and returns the minimum possible absolute difference between the total sums of two disjoint groups of 5 players each. The function should not read from standard input; instead, it should accept the 10 scores as a `std::array<int, 10>` (or a `std::vector<int>` of size 10) and return an `int`. The solution must compute the smallest possible |team1_sum - team2_sum|, where team1 and team2 each contain exactly 5 distinct players.
*/

#include <array>
#include <algorithm>
#include <cstdlib>

// Compute the minimum absolute difference between the sums of two disjoint groups of 5 numbers.
int minTeamDifference(const std::array<int, 10>& scores) {
    int total = 0;
    for (int s : scores) total += s;

    int best = total; // initialize with a large number (maximum possible difference)
    int current_sum = 0;

    // Recursive lambda to select 5 players starting from index 'start'
    // 'selected' is how many we've picked so far.
    std::function<void(int, int)> backtrack = [&](int start, int selected) {
        if (selected == 5) {
            int diff = std::abs(total - 2 * current_sum);
            best = std::min(best, diff);
            return;
        }
        // We need at least (5 - selected) more players, so we can only go up to 10 - (5 - selected)
        for (int i = start; i <= 10 - (5 - selected); ++i) {
            current_sum += scores[i];
            backtrack(i + 1, selected + 1);
            current_sum -= scores[i];
        }
    };

    backtrack(0, 0);
    return best;
}

**Note:** The solution uses `<functional>` for `std::function`; include it. The code above uses a recursive lambda, which requires `#include <functional>`. The final implementation should include all necessary headers. The provided code is self-contained but without a `main`. The following test code will provide a `main` for verification.

#include <array>
#include <cassert>
#include <functional>
#include <cstdlib>

// Function declaration (or include the solution above)
int minTeamDifference(const std::array<int, 10>& scores);

int main() {
    // Example from the description
    std::array<int, 10> scores1 = {5, 1, 8, 3, 4, 6, 7, 10, 9, 2};
    assert(minTeamDifference(scores1) == 1);

    // Example from the sample input
    std::array<int, 10> scores2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(minTeamDifference(scores2) == 1);

    // All equal values
    std::array<int, 10> scores3 = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    assert(minTeamDifference(scores3) == 0);

    // Extreme values: one very large, rest small
    std::array<int, 10> scores4 = {10000, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    // Total = 10009. Best split: one team has 10000+1+1+1+1 = 10004, other has 5*1=5, diff = 9999.
    // But can we do better? Try to put the large with 4 smalls (10004) vs 5 smalls (5) diff 9999.
    // Alternative: put large with 3 smalls (10003) vs 6 smalls? No, must be 5 each. So best is 9999.
    assert(minTeamDifference(scores4) == 9999);

    // Random-like case: 1..10 except missing 5? Already tested. Another:
    std::array<int, 10> scores5 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    // Total = 550. Best split: 5 numbers sum to 275? Try 100+90+50+30+5 (no 5). Let's compute manually: 
    // The closest to half (275) is: 100+90+80+5? We have no 5. Let's brute: Actually, we trust the function.
    assert(minTeamDifference(scores5) >= 0); // Just ensure it runs
    assert(minTeamDifference(scores5) == 10); // Let's verify: 100+90+80+70+50=390, other=160 diff 230? That's not right.
    // Let's not guess; just check it returns a number between 0 and total.
    int diff = minTeamDifference(scores5);
    assert(diff >= 0 && diff <= 550);

    // Smallest possible values
    std::array<int, 10> scores6 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    assert(minTeamDifference(scores6) == 0);

    // Max values: 10000 each, total 100000, half 50000, any 5 players sum to 50000, diff 0
    std::array<int, 10> scores7 = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
    assert(minTeamDifference(scores7) == 0);

    // A known case: 1,2,3,4,5,6,7,8,9,100 -> total 145, half 72.5. 
    // Try 100+1+2+3+4=110, other=35 diff 75. Try 100+5+6+7+8=126, other=19 diff 107. 
    // Best: pick 100+1+2+3+4=110 diff 75? Or without 100: 9+8+7+6+5=35, other=110 diff 75. So 75.
    std::array<int, 10> scores8 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    assert(minTeamDifference(scores8) == 75);

    // Another: 1,1,2,2,3,3,4,4,5,5 -> total 30, half 15. Can we get 15? 
    // 5+4+3+2+1=15, other 5+4+3+2+1=15, diff 0.
    std::array<int, 10> scores9 = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    assert(minTeamDifference(scores9) == 0);

    // Large spread: 1,2,3,4,5,6,7,8,9,10 but we already did. 
    // Another: 5,10,15,20,25,30,35,40,45,50 -> total 275, half 137.5. 
    // Best: 50+45+40+20+2? No 2. Try 50+45+40+25+20=180, other=95 diff 85. 
    // Let's trust the function to compute.
    std::array<int, 10> scores10 = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    assert(minTeamDifference(scores10) == 5); // Verified by brute force: 50+45+35+30+25=185, other=90 diff 95? No. 
    // Let's not guess; but the function should return the correct min.

    return 0;
}

**Note:** The test code above includes some comments speculating on values but the actual assert for `scores5` and `scores10` may require accurate computation. For a robust test, either compute expected values via a brute-force in the test (not shown) or choose cases with known answers. For `scores5` (10,20,...,100) the correct minimum is 10 (since total 550, half 275, you can get 270+280? Actually 100+90+80+70+50=390, other=160 diff 230; but you can get 100+90+80+60+40=370, other=180 diff 190; best is to get as close to 275: 100+90+80+5? no 5. So the min is 10? Let me think: total 550, half 275. You can have 100+90+80+70+60=400, other=150 diff 250. Alternatively 100+90+80+70+50=390 diff 230. Actually, the closest sum to 275 you can make with 5 numbers from that set: 100+90+80+70+60=400, 100+90+80+70+50=390, 100+90+80+60+50=380, ..., so the sum will be quite far from 275. The difference is at least 400-150=250? That seems large. But the minimum difference is when one team sum is as close to 275 as possible. The possible sums of 5 numbers: we need to find the subset sum closest to 275. The numbers are 10,20,30,40,50,60,70,80,90,100. The total is 550. You can get 275? 100+90+80+5? no. 100+90+70+60+50=370, 100+90+80+60+40=370, etc. The closest to 275 might be 100+90+80+70+60=400 (diff 125) or 100+90+80+70+50=390 (diff 115) or 100+90+80+60+50=380 (diff 105) or 100+90+70+60+50=370 (diff 95) or 100+80+70+60+50=360 (diff 85) or 90+80+70+60+50=350 (diff 75) or 100+90+80+70+40=380 (diff 105) ... The minimum diff might be 75? Actually, I should not guess in the test. The test code should either be verified by hand or the asserts should be for cases with known answers. For safety, I'll modify the test to only include cases with clear answers: the first four and the all-equal cases, plus a known hand-computed case like {1,2,3,4,5,6,7,8,9,100} which we computed as 75. For the others, I'll remove the speculative asserts or compute them properly. Since the test code is part of the response, it must be correct. I'll provide a corrected test below.

**Corrected Test** (with verified expected values):
#include <array>
#include <cassert>
#include <functional>
#include <cstdlib>

int minTeamDifference(const std::array<int, 10>& scores);

int main() {
    // Example from description
    std::array<int, 10> s1 = {5, 1, 8, 3, 4, 6, 7, 10, 9, 2};
    assert(minTeamDifference(s1) == 1);

    // Sample input
    std::array<int, 10> s2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(minTeamDifference(s2) == 1);

    // All equal
    std::array<int, 10> s3 = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    assert(minTeamDifference(s3) == 0);

    // One large, rest small
    std::array<int, 10> s4 = {10000, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    assert(minTeamDifference(s4) == 9999); // 10000+4*1=10004 vs 5*1=5, diff=9999

    // Hand-computed case: 1,2,3,4,5,6,7,8,9,100
    // Total 145. Best split: 100+1+2+3+4=110, other=35 diff=75. Or 100+5+6+7+8=126 diff=107. So min is 75.
    std::array<int, 10> s5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    assert(minTeamDifference(s5) == 75);

    // All minimum values
    std::array<int, 10> s6 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    assert(minTeamDifference(s6) == 0);

    // All maximum values
    std::array<int, 10> s7 = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
    assert(minTeamDifference(s7) == 0);

    // Pair symmetric: 1,1,2,2,3,3,4,4,5,5 -> total 30, half 15, exact split possible
    std::array<int, 10> s8 = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    assert(minTeamDifference(s8) == 0);

    // Another known: 2,2,2,2,2,2,2,2,2,2 -> all 2, total 20, any 5 sum 10, diff 0
    std::array<int, 10> s9 = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    assert(minTeamDifference(s9) == 0);

    // Random but verified: 1,3,5,7,9,11,13,15,17,100 -> total 181, half 90.5. 
    // Try 100+1+3+5+7=116, other=65 diff=51. Try 100+1+3+5+9=118 diff=55. Try 100+1+3+7+9=120 diff=59. 
    // Without 100: 17+15+13+11+9=65, other=116 diff=51. So min 51.
    std::array<int, 10> s10 = {1, 3, 5, 7, 9, 11, 13, 15, 17, 100};
    assert(minTeamDifference(s10) == 51);

    return 0;
}

// The core problem is to choose any 5 players from the 10 to form the first team; the remaining 5 automatically form the second team. Since the total sum of all 10 scores is fixed, if the first team’s sum is `cur`, the second team’s sum is `total - cur`, and the difference is `abs(total - 2*cur)`. To minimize this, we explore all combinations of 5 players. The number of combinations is C(10,5) = 252, which is very small, so a recursive backtracking DFS over indices is efficient. To avoid duplicate combinations (e.g., choosing players in different orders), we restrict the DFS to only consider indices in increasing order. This is done by passing a `start` parameter to the recursion. Edge cases: all scores equal (difference 0), scores with large values (up to 10000), and the fact that the teams are unordered (so we only need to consider one team). Time complexity is O(C(10,5) * 5) ≈ O(1260) operations, which is constant. Space complexity is O(5) for recursion stack depth (or O(1) if we track depth), plus O(1) for the array.
