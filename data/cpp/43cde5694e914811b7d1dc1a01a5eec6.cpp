Write a C++ function `minimumMovement` that takes a non-empty vector of integers and returns a pair containing: (1) the minimum total distance traveled by a "pointer" starting at position 0 that must visit each number in the input array exactly once (in any order), and (2) a vector<int> of positions (including the starting position 0 at index 0) representing the order in which the pointer visits the array elements to achieve that minimum total distance. The pointer moves along an integer line; visiting the i-th array element (0-indexed) requires the pointer to be at position `(i+1)` (or equivalently, at coordinate `i+1` on the line), and the cost to move from position `a` to position `b` is `|a-b|`. The pointer starts at position 0 (which is not an array element, just the starting point). You must output the minimum possible total move cost, and a valid visit order. Note: multiple orderings can give the same minimum cost; any valid one is acceptable, but the positions you output must correspond to exactly the order in which you visit the array elements (including the starting 0). The input numbers are distinct? No—they can have duplicates, but we don't care about values' relative order—only their positions (indices+1). So effectively we have a set of target positions: 1,2,...,n. The pointer starts at 0. The problem reduces to: given positions 1..n, find a permutation that minimizes sum_{k=0}^{n-1} |pos_{k+1} - pos_k| where pos_0=0 and pos_1..pos_n are a permutation of 1..n. (Note: the original code snippet processes the numbers in descending order by value, assigning alternating signs to positions, but for our task we ignore values and just consider the positions 1..n; we want to produce the same kind of optimal ordering as the snippet does—the snippet effectively sorts by descending value, then alternates left/right from center, giving a "zigzag" that minimizes total movement from 0. For our task, since all positions 1..n are present exactly once regardless of input numbers, the optimal cost depends only on n. But we must output a valid order. In the snippet, they assign position f=1, then -1, then 2, then -2, etc., based on sorted descending values. That yields an order like 1, -1, 2, -2,... but positions are indices+1? Actually they treat ans[p.second+1]=f, so they assign positions to array indices. That is different. To keep the task simple, we ask: given an array of n integers (values irrelevant), we need to assign each array element a position from the set {1,2,...,n} (each used exactly once) such that we minimize sum_{k=1}^{n} |assigned_pos[k] - assigned_pos[k-1]| with assigned_pos[0]=0, and also we need to output the assigned positions in array order (including a leading 0). The minimum cost is: for n=1: cost=1 (from 0 to 1); for n=2: cost=3 (e.g., positions 1,2 from 0->1->2 total 1+1=2? Actually 0->1 cost1, 1->2 cost1 total2? Wait min? Let's compute: we can do 0->1->2 cost2, but we have two elements so positions 1,2 in some order; 0->1 cost1, 1->2 cost1 total2; or 0->2 cost2, 2->1 cost1 total3. So min=2. For n=3: possible orders 0->1->3->2? 0 to1 cost1, 1 to3 cost2, 3 to2 cost1 total4; 0->2->1->3: 0-2 cost2, 2-1 cost1, 1-3 cost2 total5; optimal is 0->1->2->3 cost3? Wait that visits all three: positions 1,2,3 in order: 0->1 cost1, 1->2 cost1, 2->3 cost1 total3. Is there better? No because each step at least 1. So min cost = n (the path 0,1,2,...,n). But that's trivial. So my task is too trivial. Let's re-read the snippet: It reads n numbers, sorts them by value in descending order using a max-heap, then assigns positions f=1, -1, 2, -2,... to indices based on that sorted order. Then computes val = sum( x_i * 2 * abs(f) )? Actually val += p.first * 2 * abs(f) where f is the assigned "coordinate" (like -1,1,-2,2...). That's a different problem: it's like placing points on a line at integer coordinates symmetric around 0, and minimizing sum of weighted distances? But the code snippet is convoluted. For simplicity, I'll design a new task: Given an array of n distinct integers, sort them in descending order, then assign each element a "position" from the set {1, -1, 2, -2, ..., floor(n/2), -floor(n/2)} but only for n elements, using the pattern: the largest gets position 1, next largest gets -1, next gets 2, next gets -2, and so on. If n is odd, the last one gets the next positive (e.g., n=5: positions 1,-1,2,-2,3). Return the vector of assigned positions (in the same order as the original input array) and also compute the sum of (element value * 2 * absolute position). That sum is what the code computes. That is an independent task: given a vector of distinct integers, sort them descending, assign coordinates as described, and return the weighted sum and the coordinate per original index.
// The main idea is to sort the input values in descending order. Then assign positions from a pattern: first largest gets +1, second gets -1, third gets +2, fourth gets -2, etc. This is equivalent to interleaving positive integers 1,2,3,... and their negatives. Specifically, for index i (0-based in sorted order): if i is even, position = (i/2 + 1); if i is odd, position = -( (i+1)/2 ). After assigning positions, we need to map these positions back to the original input order. Store each element as a pair (value, original index), sort by value descending, then compute the position for each according to its rank. Then place the position into an answer array at the original index. Finally, compute the sum: for each element, add (value * 2 * abs(position)). Return the sum and the position array. The minimum possible sum is achieved by this greedy assignment because we pair the largest values with the smallest absolute positions (1, 2, ...) to minimize the weighted sum, since the position sign doesn't affect absolute value. The signs alternate but the absolute values are 1,1,2,2,... which is optimal by rearrangement inequality: the largest values go with the smallest multipliers. Edge cases: n=0? Task says non-empty. n=1: only position +1. Time complexity: O(n log n) due to sorting. Space: O(n) for storing pairs and answer.
#include <bits/stdc++.h>
using namespace std;

// Given a vector of distinct integers, sort descending, assign positions
// 1, -1, 2, -2, ... to them, and return:
// pair: first = sum of (value * 2 * abs(position)), second = vector of positions per original index.
pair<long long, vector<int>> minimumWeightedMovement(const vector<int>& input) {
    int n = input.size();
    vector<pair<int,int>> items; // (value, original index)
    for (int i = 0; i < n; ++i) {
        items.push_back({input[i], i});
    }
    sort(items.begin(), items.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        return a.first > b.first; // descending
    });
    
    vector<int> positions(n);
    long long total = 0;
    for (int rank = 0; rank < n; ++rank) {
        int pos;
        if (rank % 2 == 0) {
            pos = rank / 2 + 1;       // 1, 2, 3, ...
        } else {
            pos = -( (rank + 1) / 2 ); // -1, -2, -3, ...
        }
        int original_idx = items[rank].second;
        int value = items[rank].first;
        positions[original_idx] = pos;
        total += static_cast<long long>(value) * 2LL * std::abs(pos);
    }
    return {total, positions};
}
#include <bits/stdc++.h>
int main() {
    // Test 1: single element
    auto r1 = minimumWeightedMovement({5});
    assert(r1.first == 10); // 5 * 2 * 1 = 10
    assert(r1.second == vector<int>{1}); // position at index 0

    // Test 2: two elements descending [10,1]
    auto r2 = minimumWeightedMovement({10,1});
    // sorted desc: 10 (pos 1), 1 (pos -1)
    // sum = 10*2*1 + 1*2*1 = 20+2=22
    // positions original: index0 (10) gets 1, index1 (1) gets -1
    assert(r2.first == 22);
    assert(r2.second == vector<int>{1, -1});

    // Test 3: three elements [3, 1, 2] -> sorted desc: 3(pos1), 2(pos-1), 1(pos2)
    auto r3 = minimumWeightedMovement({3,1,2});
    // sum = 3*2*1 + 2*2*1 + 1*2*2 = 6+4+4=14
    // original indices: 3 at idx0->pos1, 1 at idx1->pos2, 2 at idx2->pos-1
    assert(r3.first == 14);
    assert(r3.second == vector<int>{1, 2, -1});

    // Test 4: four elements [4,2,9,5] -> sorted desc: 9(pos1),5(pos-1),4(pos2),2(pos-2)
    auto r4 = minimumWeightedMovement({4,2,9,5});
    // sum = 9*2*1 + 5*2*1 + 4*2*2 + 2*2*2 = 18+10+16+8=52
    // original indices: idx3(4?) Actually input: [4(idx0),2(idx1),9(idx2),5(idx3)]
    // sorted: 9(idx2,pos1),5(idx3,pos-1),4(idx0,pos2),2(idx1,pos-2)
    assert(r4.first == 52);
    assert(r4.second == vector<int>{2, -2, 1, -1});

    // Test 5: negative values
    auto r5 = minimumWeightedMovement({-3, -7, -1});
    // sorted desc: -1(pos1), -3(pos-1), -7(pos2)
    // sum = -1*2*1 + -3*2*1 + -7*2*2 = -2 -6 -28 = -36
    // original idx0:-3 gets -1, idx1:-7 gets 2, idx2:-1 gets 1
    assert(r5.first == -36);
    assert(r5.second == vector<int>{-1, 2, 1});

    // Test 6: all same? Task says distinct, but test duplicates anyway (should still work by stable sort? but we assume distinct)
    // Provide a case with n=0? Not allowed, but we can test n=1 again.
    
    return 0;
}
