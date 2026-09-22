You are given a sequence of `n` intervals `[l_i, r_i]` with associated costs `c_i`. After processing each interval in order, you must output the minimum total cost required to cover the entire range from the current global minimum left endpoint to the current global maximum right endpoint. A cover consists of a set of intervals whose union equals the continuous range `[min_l, max_r]`. You may select any subset of the intervals seen so far, and each selected interval contributes its cost. Intervals may overlap, and you may select the same interval at most once per query. In particular, a single interval that exactly spans the whole current range is sufficient. For each prefix of the input (after each interval), print one integer: the minimum cost to cover the current global range. If it is impossible to cover the range with the intervals seen so far, print a sentinel value of `(1LL<<60)`? No—the original problem guarantees feasibility? Actually the snippet always prints `min(cc, c2 + c1)` where `cc` is the cost of an interval spanning exactly the whole range, and `c1` is the minimum cost among intervals with left endpoint equal to the global minimum `cl`, and `c2` is the minimum cost among intervals with right endpoint equal to the global maximum `cr`. This works because we can always cover the range by taking one interval starting at `cl` (the cheapest such) and one interval ending at `cr` (the cheapest such), and if an interval itself spans the full range, that is also a candidate. Prove that this gives the optimal answer: any covering set must include at least one interval whose left endpoint is exactly `cl` (otherwise `cl` would not be covered) and at least one interval whose right endpoint is exactly `cr` (otherwise `cr` would not be covered). Therefore any valid cover costs at least the sum of the cheapest such left-interval and the cheapest such right-interval. If a single interval covers both endpoints, its cost is another valid cover, so we take the minimum between that single-interval cost and the sum of the two cheapest endpoint intervals. This is optimal because we never need more than two intervals: take the cheapest interval that covers `cl` and the cheapest interval that covers `cr`; their union indeed covers the whole range because any point between `cl` and `cr` is either inside the first interval (if it starts at `cl`) or inside the second interval (if it ends at `cr`), and since the first interval starts at `cl` and the second ends at `cr`, and they are both within the global range, the union is continuous? Careful: if the first interval starts at `cl` but ends before `cr`, and the second ends at `cr` but starts after `cl`, there could be a gap. However, the problem's original snippet assumes that the two intervals always cover the whole range because the first has left endpoint `cl` and the second has right endpoint `cr`, and the first's right endpoint is at most `cr`, the second's left endpoint is at least `cl`, but they might not overlap. For eight? In the original context (AtCoder ABC 376 E?), the assumption is that the intervals are guaranteed to form a connected cover? Actually the snippet prints `c1 + c2` without checking overlap, which implies the actual problem guarantees that the union of the cheapest left-covering and cheapest right-covering intervals is always continuous. That is true if we choose the cheapest left-covering interval with the maximal right endpoint? The snippet does not do that; it only stores the minimum cost per left endpoint and per right endpoint. But wait, the original problem (AtCoder ABC 376 E?) is actually "Max/Min Query"? Let me recall: This appears to be from AtCoder ABC 376 "E - Max/Min"? Actually no. The snippet's logic suggests the problem is: for each prefix, find the minimum cost to cover the interval from the minimum L to the maximum R using intervals seen so far. Since all intervals are given as `[l, r]`, and we can choose any subset, the answer is indeed min( cost of an interval that exactly equals [cl, cr], min cost of an interval starting at cl + min cost of an interval ending at cr ). This is correct because any interval that covers the global minimum point must have left endpoint <= cl, but since cl is the minimum left endpoint among all intervals, no interval has left endpoint < cl, so the only way to cover cl is to use an interval whose left endpoint equals cl. Similarly, to cover cr, we need an interval whose right endpoint equals cr. Therefore any cover must contain at least one interval with left endpoint cl and at least one with right endpoint cr. The cheapest such two intervals can be chosen independently; their union covers everything between cl and cr because the left interval starts at cl and ends at some r1 >= cl, and the right interval ends at cr and starts at some l2 <= cr. But is the union continuous? If r1 < l2, there is a gap. However, the problem might guarantee that there is always an interval spanning the gap? No, the snippet does not check that. Actually, re-examine: the snippet also prints `cc`, which is the minimum cost among intervals that exactly have l == cl and r == cr (i.e., span the whole range). And the final answer is `min(cc, c1 + c2)`. If the two chosen intervals do not overlap, then the union is not continuous and the answer would be invalid. But the original problem (I recognize this from AtCoder ABC 376 F? maybe "E - Max/Min"? Actually I recall a problem "Interval Cost"?) might have the property that intervals are guaranteed to be such that the minimum cost cover is always achievable by either one full-span interval or by two intervals (one starting at cl and one ending at cr) that automatically overlap? In many problems, the intervals are sorted or have some property. Actually, I realize the snippet is from a known AtCoder problem "ABC 376 D"? No. Let me not overcomplicate: The task as given should be self-contained and correct. We need to create an independent task inspired by this snippet. We can simplify: The task is to process each prefix and output the minimum cost to cover the entire current range [min L, max R] given that you can choose any intervals seen so far. The correct solution as proven in the snippet is: maintain `cl` (minimum L), `cr` (maximum R), a map `lmp` from L to min cost among intervals with that L, a map `rmp` from R to min cost among intervals with that R, and `cc` (min cost among intervals that cover the whole current range). But note: `cc` in the snippet is updated only when an interval has l == cl and r == cr exactly, not any interval covering the whole range. Actually the snippet does: `if (l == cl and r == cr) chmin(cc, c);` after updating cl and cr. So `cc` is the cost of an interval that exactly equals the current global [cl, cr]. But what if an interval covers the whole range but extends beyond? That's impossible because cl is the minimum L, so no L is less than cl, and cr is the maximum R, so no R is greater than cr. Thus any interval covering the whole range must have L == cl and R == cr. So `cc` is correct. Then the answer is min(cc, cheapest L==cl + cheapest R==cr). Indeed, this is always a valid cover: the interval with L==cl starts at the leftmost point, and the interval with R==cr ends at the rightmost point. Their union might have a gap in the middle, but wait: consider intervals [0,2] cost 100, [3,5] cost 100, and cl=0, cr=5. The cheapest left-covering interval is [0,2] cost 100, cheapest right-covering is [3,5] cost 100, sum 200. But the union {0..2} ∪ {3..5} leaves 2.5 uncovered. So the answer cannot be 200. However, perhaps the problem guarantees that the intervals always form a connected cover? Actually the original snippet would output 200, which would be wrong. So the original problem must have a different interpretation: maybe the goal is to cover the range [L_min, R_max] by selecting intervals such that every point is covered, and the answer is actually the minimum cost to cover both endpoints? No. Let me search memory: This snippet is from AtCoder ABC 389 E? Actually I recall a problem "Minimum Cost to Cover Range" where the trick is exactly that you only need to cover the two endpoints, because any interval that covers the left endpoint and any interval that covers the right endpoint, if you take the one with the maximum right endpoint among those covering left, and the one with the minimum left endpoint among those covering right, then they might still have a gap. So the correct solution should maintain the cheapest interval covering cl with the maximum r, and the cheapest interval covering cr with the minimum l, and then check overlap. But the snippet does not do that. So perhaps the original problem has the property that intervals are such that if you take the cheapest interval starting at cl and the cheapest ending at cr, they always cover the whole range because intervals are "nice"? Actally, I think I recognize this problem: It is AtCoder ABC 376 E? No. It is AtCoder Beginner Contest 371 F? No. Actually, let me parse the snippet: It uses `map<int, long long> lmp` and `rmp`, and after each query it prints `min(cc, lmp.begin()->second + rmp.rbegin()->second)`. This is a known problem "Range Cover Query" from Codeforces? Or maybe it's from AtCoder ABC 384 E? I recall a problem where intervals are given, and for each prefix you need the minimum cost to cover the interval from min L to max R, and the trick is exactly as in the snippet because you only need to cover the two endpoints: any covering set must include an interval with L = min L and an interval with R = max R, and taking the cheapest such two intervals always covers the whole range because the interval with L = min L extends to some R1, and the interval with R = max R starts at some L2. But is it guaranteed that L2 <= R1? Not necessarily. However, the problem might have a constraint that intervals are sorted by L, or that the given intervals are such that the range is always connected? The snippet just outputs the sum without checking, so either the original problem guarantees it, or the intended solution is indeed this simple because the intervals are specifically designed such that the two cheapest endpoint intervals always overlap. For the purpose of creating a standalone task, we can make that assumption explicit: we guarantee that for the given input, the cheapest interval starting at the global minimum L and the cheapest interval ending at the global maximum R always overlap (i.e., their union is continuous). Or we can modify the task to make it correct: we can ask for the minimum cost to cover both the leftmost and rightmost points, not the entire range? But the snippet clearly intends covering the entire range. Actually, re-reading the snippet: it never checks overlap. So the original problem must have that property. Let me look up: This appears to be from AtCoder ABC 400? No. I think it's from AtCoder Regular Contest 175 A? No. Actually, I recall a problem "ABC 384 E - Max/Min"? Not.

To be safe, for the standalone task I will define the problem exactly as the snippet solves it, and state that the intervals are guaranteed such that the minimum cost cover is always either a single interval spanning the whole range, or the sum of the cheapest interval starting at the minimum L and the cheapest interval ending at the maximum R. I can make that a property of the test data. Or better, I can design the task so that the correct answer is indeed that sum, by explicitly asking: "Find the minimum cost to cover the current global range, given that you may select any two intervals, one with left endpoint equal to the global minimum L and one with right endpoint equal to the global maximum R, and you may also select a single interval that spans the whole range." Actually that's not a natural problem. Let me think of a cleaner formulation inspired by the snippet but self-contained and correct.

Let me define the task as follows: You are given a sequence of intervals with costs. For each prefix, you need to output the minimum total cost of selecting a set of intervals such that the union of selected intervals covers both the smallest left endpoint and the largest right endpoint among all intervals in the prefix. That is, you need to cover the two extreme points, not necessarily the entire range. But then the answer would just be min( cheapest interval with L == minL, cheapest interval with R == maxR, cheapest interval covering both endpoints )? Actually to cover both minL and maxR, you need at least one interval containing minL and at least one containing maxR. Since minL is the smallest L, any interval containing minL must have L == minL (because L cannot be smaller). Similarly, any interval containing maxR must have R == maxR. So the answer is min( cost of an interval with L==minL and R==maxR, cheapest interval with L==minL + cheapest interval with R==maxR ). This is exactly the snippet's computation, and it is always correct for the problem of covering the two endpoints. But covering the endpoints does not imply covering the whole range, so the problem statement must be "cover the two extreme points" not "cover the whole range". However, the snippet prints `min(cc, c1 + c2)` which matches covering the two endpoints. So let's define the task as: After each prefix, output the minimum cost to select a set of intervals such that both the global minimum left endpoint and the global maximum right endpoint are included in at least one selected interval. This is a well-defined problem and the snippet's solution is correct.

Thus, the task: Given a sequence of intervals `[l, r]` with cost `c`, after each interval, output the minimum total cost required to choose a subset of intervals seen so far such that the point `min_l` (the smallest `l` among all seen intervals) is covered by some chosen interval, and the point `max_r` (the largest `r` among all seen intervals) is covered by some chosen interval. You may choose any number of intervals, and each chosen interval contributes its cost. If a single interval covers both extremes, that is allowed. The answer is the minimum over all valid subsets.

Solution approach: Maintain `cl` = current minimum L, `cr` = current maximum R. Maintain `lmp` mapping each L to the minimum cost among intervals with that L. Maintain `rmp` mapping each R to the minimum cost among intervals with that R. Also maintain `cc` = minimum cost among intervals that have L == cl and R == cr (i.e., an interval that exactly spans the current extremes). After reading each interval, update cl, cr, lmp, rmp, and cc. If the new interval has L == cl and R == cr, update cc with min(cc, c). The answer for the current prefix is `min(cc, lmp[cl] + rmp[cr])` because to cover `cl`, you must use an interval with L == cl (since no interval has L < cl), and the cheapest such is `lmp[cl]`; similarly to cover `cr`, you must use an interval with R == cr, cheapest is `rmp[cr]`. These two intervals together cover both extreme points. If a single interval covers both, that is captured by `cc`.

Edge cases: When only one interval is seen, cl = l, cr = r, lmp[cl] = c, rmp[cr] = c, cc = c, answer = min(c, c+c) = c. When an interval exactly matches both extremes, cc is updated. When multiple intervals share the same L, keep the minimum cost. Same for R. Complexity: O(n log n) due to map operations, O(n) memory.

Now produce the task, analysis, solution function (without main), and test with asserts.
We process intervals one by one while maintaining four pieces of state: `cl` (the smallest left endpoint seen so far), `cr` (the largest right endpoint seen so far), a dictionary `leftCost` mapping each left endpoint to the minimum cost of any interval with that left endpoint, and `rightCost` mapping each right endpoint to the minimum cost of any interval with that right endpoint. Additionally, we keep `bothCost` as the minimum cost among intervals that exactly have left endpoint equal to `cl` and right endpoint equal to `cr`. After each insertion, the optimum cost to cover both extreme points is the minimum of two candidates: (1) the cost of a single interval that spans both extremes (i.e., `bothCost`), and (2) the sum of the cheapest interval that starts at `cl` and the cheapest that ends at `cr`. This works because covering `cl` forces us to choose at least one interval with left endpoint exactly `cl` (since no interval can have left endpoint less than `cl`), and covering `cr` forces us to choose at least one interval with right endpoint exactly `cr`. The cheapest such intervals provide a valid cover of the two extremes, and any other valid cover must cost at least that sum. When an interval simultaneously spans both extremes, it is a separate single-interval candidate. We update the dictionaries by taking the minimum when a duplicate endpoint appears. The algorithm runs in O(n log n) time and O(n) space, dominated by the two maps.
#include <bits/stdc++.h>
using namespace std;

// Process a sequence of intervals [l, r] with cost c.
// For each prefix, returns the minimum total cost to cover both the current
// global minimum left endpoint and the current global maximum right endpoint.
vector<long long> minCostToCoverExtremes(
    const vector<tuple<int,int,long long>>& intervals
) {
    const int n = (int)intervals.size();
    vector<long long> answers;
    answers.reserve(n);

    int cl = INT_MAX;
    int cr = INT_MIN;
    long long cc = LLONG_MAX; // min cost of an interval exactly spanning [cl, cr]

    map<int, long long> leftCost;  // min cost for each left endpoint
    map<int, long long> rightCost; // min cost for each right endpoint

    for (const auto& [l, r, c] : intervals) {
        // Update cheapest cost for this left endpoint
        auto itL = leftCost.find(l);
        if (itL == leftCost.end()) leftCost[l] = c;
        else itL->second = min(itL->second, c);

        // Update cheapest cost for this right endpoint
        auto itR = rightCost.find(r);
        if (itR == rightCost.end()) rightCost[r] = c;
        else itR->second = min(itR->second, c);

        // Update global extremes
        if (l < cl || r > cr) {
            cc = LLONG_MAX; // previous exact-span interval no longer spans new range
        }
        cl = min(cl, l);
        cr = max(cr, r);

        // If this interval exactly spans the new [cl, cr], update cc
        if (l == cl && r == cr) {
            cc = min(cc, c);
        }

        long long single = cc;
        long long pairCost = leftCost[cl] + rightCost[cr];
        answers.push_back(min(single, pairCost));
    }
    return answers;
}
#include <bits/stdc++.h>
using namespace std;

// Prototype of the function under test
vector<long long> minCostToCoverExtremes(const vector<tuple<int,int,long long>>& intervals);

int main() {
    vector<tuple<int,int,long long>> case1 = {
        {0, 5, 10},
        {2, 7, 20},
    };
    // prefix1: only [0,5] -> cl=0, cr=5, cc=10, leftCost[0]=10, rightCost[5]=10 -> min(10,20)=10
    // prefix2: cl=0, cr=7, no interval spans [0,7], leftCost[0]=10, rightCost[7]=20 -> 30
    vector<long long> exp1 = {10, 30};
    assert(minCostToCoverExtremes(case1) == exp1);

    vector<tuple<int,int,long long>> case2 = {
        {1, 3, 5},
        {2, 4, 7},
        {1, 4, 100},
    };
    // prefix1: cover [1,3] -> 5
    // prefix2: cl=1, cr=4, left[1]=5, right[4]=7, sum=12, no exact span -> 12
    // prefix3: cl=1, cr=4, left[1]=5, right[4]=7, also exact span cost 100 -> min(100,12)=12
    vector<long long> exp2 = {5, 12, 12};
    assert(minCostToCoverExtremes(case2) == exp2);

    vector<tuple<int,int,long long>> case3 = {
        {0, 0, 3},
        {0, 0, 5},
    };
    // prefix1: cl=0, cr=0, cc=3, left[0]=3, right[0]=3 -> min(3,6)=3
    // prefix2: cc stays 3, left[0]=3, right[0]=3 -> 3
    vector<long long> exp3 = {3, 3};
    assert(minCostToCoverExtremes(case3) == exp3);

    vector<tuple<int,int,long long>> case4 = {
        {5, 1, 10}, // invalid? l > r, but assume valid input l <= r
        {2, 8, 15},
    };
    // Actually l <= r must hold, so adjust:
    vector<tuple<int,int,long long>> case4b = {
        {5, 6, 10},
        {2, 8, 15},
    };
    // prefix1: cl=5, cr=6, cc=10, left[5]=10, right[6]=10 -> 10
    // prefix2: cl=2, cr=8, cc=LLONG_MAX (no exact span), left[2]=15, right[8]=15 -> 30
    vector<long long> exp4 = {10, 30};
    assert(minCostToCoverExtremes(case4b) == exp4);

    // Duplicate left endpoints with different costs
    vector<tuple<int,int,long long>> case5 = {
        {0, 3, 8},
        {0, 10, 20},
        {4, 10, 12},
    };
    // prefix1: cl=0, cr=3, cc=8, left[0]=8, right[3]=8 -> 8
    // prefix2: cl=0, cr=10, no exact span, left[0]=8, right[10]=20 -> 28
    // prefix3: cl=0, cr=10, left[0]=8, right[10]=min(20,12)=12 -> 20
    vector<long long> exp5 = {8, 28, 20};
    assert(minCostToCoverExtremes(case5) == exp5);

    // Case where a later interval gives exact span cheaper than pair
    vector<tuple<int,int,long long>> case6 = {
        {0, 10, 100},
        {0, 5, 1},
        {5, 10, 1},
    };
    // prefix1: 100
    // prefix2: cl=0, cr=10, left[0]=1, right[10]=100 -> min(100,101)=100
    // prefix3: cl=0, cr=10, left[0]=1, right[10]=1 -> 2, but also exact span? none. Answer 2
    vector<long long> exp6 = {100, 100, 2};
    assert(minCostToCoverExtremes(case6) == exp6);

    // Edge: n=0
    vector<tuple<int,int,long long>> case7;
    vector<long long> exp7;
    assert(minCostToCoverExtremes(case7) == exp7);

    // Large costs
    vector<tuple<int,int,long long>> case8 = {
        {0, 1000000000, 1000000000000LL},
        {0, 1, 1},
        {1, 1000000000, 1},
    };
    // prefix1: 1e12
    // prefix2: cl=0, cr=1e9, left[0]=1, right[1e9]=1e12 -> min(1e12, 1+1e12) = 1e12
    // prefix3: left[0]=1, right[1e9]=1 -> 2
    vector<long long> exp8 = {1000000000000LL, 1000000000000LL, 2};
    assert(minCostToCoverExtremes(case8) == exp8);

    // Single interval repeated
    vector<tuple<int,int,long long>> case9 = {
        {3, 7, 5},
        {3, 7, 4},
        {3, 7, 6},
    };
    // Always cl=3, cr=7, cc becomes 4 after second, left[3]=4, right[7]=4 -> answer 4
    vector<long long> exp9 = {5, 4, 4};
    assert(minCostToCoverExtremes(case9) == exp9);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
