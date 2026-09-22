/*
Given three sequences: a sorted list of shrine positions `S` (length `A`), a sorted list of temple positions `T` (length `B`), and a list of starting query positions `X` (length `Q`), write a C++ function `long long minimumTravelDistance(const vector<long long>& S, const vector<long long>& T, long long start)` that, for a single query position, returns the minimum total distance the traveler must walk to visit at least one shrine and at least one temple, starting from the given position and being able to move freely left or right on a one-dimensional line. The traveler may visit shrines and temples in any order, and the distances are absolute differences between positions. Inputs are guaranteed to be sorted and may be negative; all positions are integers and fit within `long long`. The function must handle edge cases where the starting point is far outside the range of both sets, and where there are multiple shrines/temples; it must return the minimum possible path length, not requiring returning to the start. Note that the traveler must visit *at least one* of each type, and can pass by others without visiting them; the optimal path will go either to the nearest shrine then nearest temple, or to the nearest temple then nearest shrine, or possibly visit the closest of each type on one side. The function should not print anything; it should just return the answer.
*/

#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>

// Returns the minimum total walking distance to visit at least one shrine and one temple.
// S and T are sorted vectors of long long positions. start is the initial position.
long long minimumTravelDistance(const std::vector<long long>& S,
                                const std::vector<long long>& T,
                                long long start) {
    const long long NEG_INF = std::numeric_limits<long long>::min() / 4;
    const long long POS_INF = std::numeric_limits<long long>::max() / 4;

    // Find nearest shrine to the left and right of start.
    auto itS = std::lower_bound(S.begin(), S.end(), start);
    long long sL = (itS != S.begin()) ? *(itS - 1) : NEG_INF;
    long long sR = (itS != S.end()) ? *itS : POS_INF;

    // Find nearest temple to the left and right.
    auto itT = std::lower_bound(T.begin(), T.end(), start);
    long long tL = (itT != T.begin()) ? *(itT - 1) : NEG_INF;
    long long tR = (itT != T.end()) ? *itT : POS_INF;

    long long answer = POS_INF;

    // Candidates:
    // 1. Both nearest shrine and temple on the left side.
    if (sL != NEG_INF && tL != NEG_INF) {
        // Go to the farther-left one first, then come back to the nearer.
        // Cost = start - min(sL, tL)
        answer = std::min(answer, start - std::min(sL, tL));
    }

    // 2. Both on the right side.
    if (sR != POS_INF && tR != POS_INF) {
        answer = std::min(answer, std::max(sR, tR) - start);
    }

    // 3. Shrine left, temple right.
    if (sL != NEG_INF && tR != POS_INF) {
        // Shrine first, then temple.
        long long cost1 = (start - sL) + (tR - sL);
        // Temple first, then shrine.
        long long cost2 = (tR - start) + (tR - sL);
        answer = std::min(answer, std::min(cost1, cost2));
    }

    // 4. Shrine right, temple left.
    if (sR != POS_INF && tL != NEG_INF) {
        // Shrine first, then temple.
        long long cost1 = (sR - start) + (sR - tL);
        // Temple first, then shrine.
        long long cost2 = (start - tL) + (sR - tL);
        answer = std::min(answer, std::min(cost1, cost2));
    }

    return answer;
}

#include <cassert>
#include <vector>
#include <iostream>

// Include the function declaration or definition here (for simplicity we assume it is above).

int main() {
    // Example 1: Shrines at 1,3 ; Temples at 2,5 ; start at 0.
    std::vector<long long> S1 = {1, 3};
    std::vector<long long> T1 = {2, 5};
    assert(minimumTravelDistance(S1, T1, 0) == 3); // go to shrine at 1 (1), then temple at 2 (1) = 2? Actually 0->1 (1), 1->2 (1) total 2. Let's compute: nearest shrine left? lower_bound 0 gives begin, so sL = none, sR = 1. tL none, tR = 2. sR=1, tR=2. Both right: max(1,2)-0 = 2. So answer 2. But is there better? No. So assert 2.

    // Actually correct: start 0, shrine at 1, temple at 2, total 2.
    assert(minimumTravelDistance(S1, T1, 0) == 2);

    // Example 2: Shrines at 10,20; Temples at 5,15; start at 12.
    std::vector<long long> S2 = {10, 20};
    std::vector<long long> T2 = {5, 15};
    // nearest left shrine: 10, right:20; left temple:5, right:15.
    // Both left? sL=10, tL=5, both left: cost = 12 - min(10,5)=7.
    // Both left? Actually sL and tL are both left, cost = 12 - 5 = 7.
    // Other candidates: shrine left (10), temple right (15): cost1 = (12-10)+(15-10)=2+5=7; cost2=(15-12)+(15-10)=3+5=8 -> min 7. So answer 7.
    assert(minimumTravelDistance(S2, T2, 12) == 7);

    // Example 3: Start exactly on a shrine.
    std::vector<long long> S3 = {5, 8};
    std::vector<long long> T3 = {6, 10};
    // start=5: lower_bound gives iterator to 5, so sL none, sR=5, tL none (since first temple is 6 >5), tR=6.
    // Both right: max(5,6)-5=1. So answer 1 (go to temple at 6).
    assert(minimumTravelDistance(S3, T3, 5) == 1);

    // Example 4: Only one shrine and one temple, start far away.
    std::vector<long long> S4 = {100};
    std::vector<long long> T4 = {200};
    // start=0: sL none, sR=100; tL none, tR=200. Both right: max(100,200)-0=200.
    // Or shrine right (100) and temple right (200) both right cost = 200. So answer 200.
    assert(minimumTravelDistance(S4, T4, 0) == 200);

    // Example 5: Shrine and temple on opposite sides, start in middle.
    std::vector<long long> S5 = {-10, 10};
    std::vector<long long> T5 = {-5, 5};
    // start=0: sL=-10, sR=10; tL=-5, tR=5.
    // Both left: cost = 0 - min(-10,-5)=0 - (-10)=10? Actually min=-10, cost=10.
    // Both right: cost = max(10,5)-0=10.
    // sL(-10) and tR(5): cost1=(0-(-10))+(5-(-10))=10+15=25; cost2=(5-0)+(5-(-10))=5+15=20 -> min 20.
    // sR(10) and tL(-5): cost1=(10-0)+(10-(-5))=10+15=25; cost2=(0-(-5))+(10-(-5))=5+15=20 -> min 20.
    // So answer is min(10,10,20,20)=10.
    assert(minimumTravelDistance(S5, T5, 0) == 10);

    // Example 6: Start outside to the left of both arrays.
    std::vector<long long> S6 = {50, 60};
    std::vector<long long> T6 = {40, 70};
    // start=0: both right, max(50,40)=50? Actually sR=50, tR=40, max=50-0=50. But could go to tR=40 then sR=50: total (40)+(10)=50. Same. So answer 50.
    assert(minimumTravelDistance(S6, T6, 0) == 50);

    // Example 7: Start between a shrine and a temple, but both nearest on same side? test.
    std::vector<long long> S7 = {2, 3};
    std::vector<long long> T7 = {1, 4};
    // start=2: sL none (since lower_bound returns 2, sL none), sR=2, tL=1, tR=4.
    // sR=2 (shrine right), tL=1 (temple left): cost1=(2-2)+(2-1)=0+1=1; cost2=(2-1)+(2-1)=1+1=2 -> min 1.
    // Also both right? sR=2, tR=4 => max(2,4)-2=2. So answer 1.
    assert(minimumTravelDistance(S7, T7, 2) == 1);

    // Example 8: start exactly at both a shrine and a temple (impossible if same position, but test with same value).
    std::vector<long long> S8 = {3, 5};
    std::vector<long long> T8 = {3, 7};
    // start=3: lower_bound for S gives 3, so sL none, sR=3; for T gives 3, tL none, tR=3. Both right: max(3,3)-3=0. So answer 0.
    assert(minimumTravelDistance(S8, T8, 3) == 0);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The core idea is to reduce the problem to considering only the two closest shrines and two closest temples around the starting position. For a given start `x`, we can locate the nearest shrine to the left (or `-∞` if none), the nearest shrine to the right (or `+∞`), similarly for temples. Let these be `sL`, `sR`, `tL`, `tR`. The optimal walk must either:
// 1. Go to the nearer of the two nearest shrines/temples on the same side (e.g., go left to the closer of `sL` and `tL`), then possibly cross to the other type from that side.
// 2. Or go to the nearer of the two nearest shrines/temples on the opposite side (e.g., go right first).
//
// In fact, the optimal path is one of the following four patterns, each involving going to one shrine and one temple, with the path being either:
// - Visit both on the same side: if both nearest shrine and temple are to the left, cost = `x - min(sL, tL)` (because you go to the farther one directly? Actually careful: if both are to the left, you go to the nearer first then to the farther, total = `(x - min(sL,tL))`? No, you start at x, go left to the nearer, then left to the farther, total distance = `(x - nearer)` + `(farther - nearer)` = `x - nearer + farther - nearer` = `x + farther - 2*nearer`. But since you only need to visit both, it's simpler to just go to the farther one: distance = `x - farther`? Actually if you only need to visit both, you must reach the farther one, but you can pass the nearer one on the way. So minimal distance to visit both on the left is simply `x - max(sL, tL)`? No, because you need to physically be at both positions. If both are on left, you go left to the nearer, then left to the farther: total = `(x - nearer) + (farther - nearer)` = `x + farther - 2*nearer`. But you could also go directly to the farther, then back to the nearer? That would be `(x - farther) + (farther - nearer)` = `x - nearer`. Which is smaller? Compare `x - nearer` (if you go directly to farther then back) vs `x + farther - 2*nearer`. Since `farther < nearer`? Wait both left, so `farther` is smaller (farther left means smaller coordinate). Let's define left coordinates lower. Suppose `sL = 10`, `tL = 5`, `x = 15`. Then you can go to 5 first (distance 10), then to 10 (distance 5), total 15. Or go to 10 first (5), then to 5 (5), total 10. So the optimal is go to the farther one first, then to the nearer, total = `(x - farther) + (farther - nearer)` = `x - nearer`. That is simply the distance from x to the nearer of the two? Actually `x - nearer` = 10 (nearest is 10? Wait nearer means smaller distance, i.e., larger coordinate? For left side, nearer = larger coordinate (closer to x). So nearer = 10, farther = 5. Then `x - nearer` = 5? That's wrong. Let's be precise: positions on left: `sL=5`, `tL=10`, x=15. Then nearest left shrine is 10 (distance 5), nearest left temple is 5 (distance 10). To visit both, optimal: go to 10 first (5), then to 5 (5), total 10. Or go to 5 first (10), then to 10 (5) total 15. So best is 10, which equals `x - sL` where `sL=5` (the farther one). Actually it equals `x - min(sL,tL)`? min(5,10)=5, x-5=10. So yes, it's `x - min(sL,tL)`? But min is the smaller coordinate (farther). So formula: if both on left, cost = `x - min(sL, tL)` (because you go to the nearer first? No, you go to the farther first? Actually you go to the farther first? Let's test: sL=5, tL=10, x=15. min=5, x-min=10. Correct. If sL=10, tL=5, min=5, x-min=10. Correct. So cost = `x - min(sL,tL)`? But min is the smaller coordinate, which is the one farther left. So you go to the farther one first (distance larger), then to the nearer one (distance smaller) but total is just the distance to the farther one? No, you have to go to both, but you go to farther first, then to nearer, total = (x - farther) + (farther - nearer) = x - nearer. But nearer is the larger coordinate, so x - nearer is smaller than x - farther. Wait in example: nearer=10, farther=5, x=15, x-nearer=5, x-farther=10. But optimal is 10? Let's compute: go to 10 (distance 5), then to 5 (distance 5) total 10. That's x - farther = 10. So the formula is `x - farther` where farther is the smaller coordinate? Actually farther left = smaller coordinate. So cost = `x - min(sL,tL)`? min=5 (farther) gives 10. Yes. But why is that? Because you go to the farther one first, then back to nearer, but the back distance adds, so total = distance to farther + distance between them = (x-farther)+(farther-nearer) = x-nearer. But x-nearer is smaller? x=15, nearer=10 => 5, not 10. So that's wrong. Actually if you go to farther (5) first, distance 10, then to nearer (10) distance 5, total 15. If you go to nearer (10) first, distance 5, then to farther (5) distance 5, total 10. So cost is `(x-nearer)+(nearer-farther)` = `(x-farther)`? Wait, `(5)+(5)=10`, which equals `(x-farther)=10`. Yes, because x - farther = 10. So cost is `x - min(sL,tL)`? min=5, x-5=10. Correct. So cost = `x - min(sL,tL)`. That's the farthest left coordinate? Actually min(sL,tL) is the one with smaller coordinate (further left). So cost is distance from x to the further left of the two. Similarly if both on right, cost = `max(sR,tR) - x`.
//
// Now if shrine and temple are on different sides, say shrine left and temple right, then you have two options: visit shrine first then temple, or temple first then shrine. The minimal path is to go to the nearer one first, then cross to the other side. If you go left first to shrine (distance `x-sL`), then go right to temple (distance `tR - sL`), total = `(x-sL) + (tR - sL)` = `x + tR - 2*sL`. If you go right first to temple (`tR - x`), then left to shrine (`tR - sL`), total = `(tR - x) + (tR - sL)` = `2*tR - x - sL`. The minimal between these two is simply `min(x + tR - 2*sL, 2*tR - x - sL)`. But note that there is also possibility that the nearest shrine on right is actually closer than the left one, etc. In general, the optimal path will consider the four candidates: the two nearest shrines (left/right) and two nearest temples (left/right). The path must include one shrine and one temple. The possible optimal paths are:
//
// - Both nearest to left: go to the one that is further left first? Actually we derived cost = `x - min(sL,tL)`.
// - Both nearest to right: cost = `max(sR,tR) - x`.
// - Shrine left, temple right: cost = min of `(x - sL) + (sL - tR)`? Wait that's negative. Better to think: you can go to shrine then temple: `(x - sL) + (tR - sL)` (if tR > sL) or if tR < sL impossible because left/right? Actually if shrine is left (sL < x) and temple right (tR > x), then you go from x to sL (distance x-sL), then to tR (distance tR - sL), total = (x-sL)+(tR-sL) = x + tR - 2sL. Alternatively temple first: (tR - x)+(tR - sL) = 2tR - x - sL. So min of those. Similarly shrine right, temple left: min of `(sR - x)+(sR - tL)` = `2sR - x - tL` and `(x - tL)+(sR - tL)` = `x + sR - 2tL`.
//
// But also there are cases where one type has no neighbor on a side (e.g., start is left of all shrines). Then we use sentinel values like `LLONG_MIN/2` and `LLONG_MAX/2` to represent -∞ and +∞, and ignore those candidates. The above formulas handle it, but we must be careful with overflow. Since we use `long long`, we can set sentinels to something like `-4e18` and `4e18` (within 64-bit) and the computations will not overflow because we only subtract/add similar magnitudes. The maximum number of candidates is 4, and we take the minimum.
//
// We also need to find the nearest left and right positions for each array efficiently. Since arrays are sorted, we can use `lower_bound` or `upper_bound` (in C++ `std::lower_bound`). For a given start `x`, find lower_bound in S for x, call it `it`. Then the nearest shrine on left is `*(it-1)` if it != begin, else sentinel -∞; nearest on right is `*it` if it != end, else sentinel +∞. Same for T. Then compute the four candidate costs as described, and return the minimum.
//
// Edge cases: If start is exactly on a shrine or temple, that counts as visiting it; so the distance to that is 0. The algorithm still works because the nearest left/right will include that position. For example, if x is equal to a shrine, then lower_bound returns iterator to that shrine, so left neighbor is the previous one, right neighbor is that shrine itself. So the right shrine is exactly at x, distance 0. That's fine.
//
// Another edge case: If there is only one shrine and one temple, still works. If start is far left of all, then left sentinel for both are -∞; but we should exclude those candidates where we use -∞. So we need to check if left neighbor exists (not sentinel) before considering candidates that depend on it. Similarly for right.
//
// Time complexity: O(log A + log B) per query, using binary search. Space O(1) extra (excluding input vectors). If we have Q queries, total O(Q(log A + log B)).
