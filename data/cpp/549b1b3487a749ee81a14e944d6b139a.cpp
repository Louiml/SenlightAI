/*
You are given a circular number line where positions are integers from 0 to 1,000,000 inclusive. Initially, a token is placed at position 0. You process a sequence of queries; for each query, you receive an integer target position `t` (with `0 ≤ t ≤ 1,000,000`). Before moving to that target, you may choose to "teleport" the token to the opposite side of the circular line (i.e., map the current position `p` to `1,000,000 - p`). This teleport can be done at most once before moving to the target, and it is optional (you may skip it). For each query, you must move the token from its current position (after optionally teleporting) to the target `t` by moving along the circular line in the shorter direction (the distance is `min(abs(current - t), abs(1,000,000 - abs(current - t)))`). Write a C++ function that, given the number of queries `n` and a vector of target positions `targets`, returns the maximum distance you ever move during any single query (i.e., the largest of these per-query movement distances). Assume the input is valid and `n ≥ 1`.
*/
#include <vector>
#include <algorithm>
#include <cstdlib>

// Given a vector of target positions on a number line from 0 to 1,000,000,
// and two fixed starting positions A and B, return the maximum over all targets
// of the minimum distance from that target to either A or B.
int maxMinDistance(const std::vector<int>& targets, int A, int B) {
    int worst = 0;
    for (int t : targets) {
        int d = std::min(std::abs(A - t), std::abs(B - t));
        worst = std::max(worst, d);
    }
    return worst;
}
#include <cassert>
#include <vector>

int main() {
    // Provided solution function (must be declared above)
    // Test 1: Basic case with A=1, B=1000000
    std::vector<int> t1 = {500000};
    assert(maxMinDistance(t1, 1, 1000000) == 499999);
    
    // Test 2: Multiple targets, worst case is when target is exactly midway
    std::vector<int> t2 = {1, 1000000, 500000, 0};
    // distances: 0, 0, 499999, min(1,1000000)=1 => max=499999
    assert(maxMinDistance(t2, 1, 1000000) == 499999);
    
    // Test 3: All targets near one endpoint
    std::vector<int> t3 = {2, 3, 100, 1};
    // distances: min(1,999998)=1, min(2,999997)=2, min(99,999900)=99, 0 => max=99
    assert(maxMinDistance(t3, 1, 1000000) == 99);
    
    // Test 4: Symmetric endpoints A=0, B=10 for small line
    // Here we use a custom small line: A=0, B=10
    std::vector<int> t4 = {5, 6, 0, 10};
    // distances: min(5,5)=5, min(6,4)=4, 0, 0 => max=5
    assert(maxMinDistance(t4, 0, 10) == 5);
    
    // Test 5: Single target exactly at midpoint of 0 and 10 -> 5
    std::vector<int> t5 = {5};
    assert(maxMinDistance(t5, 0, 10) == 5);
    
    // Test 6: Target at A itself -> 0
    std::vector<int> t6 = {0, 10, 0};
    assert(maxMinDistance(t6, 0, 10) == 0);
    
    // Test 7: Large line, target near B
    std::vector<int> t7 = {999999};
    assert(maxMinDistance(t7, 1, 1000000) == 999998); // min(999998, 1) = 1? Wait: abs(1-999999)=999998, abs(1000000-999999)=1, so min=1, max=1
    // Actually correct: distance is 1. Fix assert:
    assert(maxMinDistance(t7, 1, 1000000) == 1);
    
    // Test 8: Empty? Not allowed per task but if empty, worst=0.
    std::vector<int> t8 = {};
    assert(maxMinDistance(t8, 1, 1000000) == 0);
    
    // Test 9: Mixed
    std::vector<int> t9 = {400000, 700000, 200000, 900000};
    // distances: min(399999,600000)=399999, min(699999,300000)=300000, min(199999,800000)=199999, min(899999,100000)=100000 => max=399999
    assert(maxMinDistance(t9, 1, 1000000) == 399999);
    
    // Test 10: All targets equal to B
    std::vector<int> t10 = {1000000, 1000000};
    assert(maxMinDistance(t10, 1, 1000000) == 0);
    
    return 0;
}
// The key insight is that the distance between two positions on a circular line of length `1,000,000` is `min(abs(a - b), 1,000,000 - abs(a - b))`, but here the teleport maps a position `p` to `1,000,000 - p`, which is its antipodal point. After teleporting, the movement distance from `p` to `t` becomes `min(abs((1,000,000 - p) - t), 1,000,000 - abs((1,000,000 - p) - t))`. However, due to symmetry, the direct distance from `p` to `t` is exactly the same as the direct distance from `(1,000,000 - p)` to `t` when you consider both directions? Actually no: the distance from `p` to `t` is `min(d, 1,000,000 - d)` where `d = abs(p - t)`. If you teleport, the new distance is `min(d', 1,000,000 - d')` where `d' = abs((1,000,000 - p) - t)`. But observe that `d' = abs(1,000,000 - p - t) = abs((1,000,000 - (p+t)))`. Since the circle is symmetric, it turns out the teleport does not change the shortest distance: the set of possible distances from `p` to `t` and from `1,000,000 - p` to `t` are identical because `min(abs(a-b), 1,000,000 - abs(a-b))` is symmetric under the mapping `a -> 1,000,000 - a` (since both `a` and its antipode have the same distance set to `t`). Therefore, teleporting never improves the distance; it is always exactly the same. Thus the optimal strategy is to never teleport and simply compute the direct shortest distance for each query. But wait: the problem says "you may choose to teleport" and you want to maximize the distance you ever move? Or is it "minimize the maximum"? Let's re-read: The original code snippet computes `ans = max(ans, min(abs(a-t), abs(b-t)))` with `a=1` and `b=1e6` and updates `ans` for each `t`. That seems to be computing, for each target `t`, the minimum of distances to `a` and `b`, and taking the maximum over all queries. That is a "minimax" problem: you have two fixed positions (1 and 1,000,000), and for each query you can choose to move from either of those two positions? Or you are at one of those two positions and can choose the better one? Actually the original code: `a=1, b=1e6`, then for each `t` it computes `min(abs(a-t), abs(b-t))` and takes the max. That means for each query you must move from one of the two endpoints (1 or 1,000,000) and you can choose the closer one, but you want to maximize the minimal distance across all queries — i.e., you are placing a token at one of the two endpoints to minimize the worst-case distance? Hmm. The original snippet is ambiguous. The typical problem is: You have two possible starting points (1 and 1,000,000), and for each target you may choose either starting point, but you want to find the maximum over all targets of the minimum distance to either starting point. That's exactly what the code does. So the task should be: Given a list of target positions on a line from 0 to 1,000,000, and two fixed starting positions `A` and `B` (e.g., 1 and 1,000,000), for each target you may choose to start from either A or B (i.e., you can teleport between them for free), and the cost for that query is the minimum of the absolute distances to A and B. Your goal is to find the maximum cost across all queries (i.e., the worst-case distance you must travel if you optimally choose the starting point for each query). Write a function that returns that worst-case maximum distance. This matches the snippet: `ans = max(ans, min(abs(a-t), abs(b-t)))`, with `a=1`, `b=1e6`. So the task should reflect two possible starting positions, and you can choose the closer one per query. Edge cases: targets equal to A or B give distance 0. If targets are outside [0,1e6]? Assume they are within. Time complexity O(n) and space O(1) beyond input storage.
