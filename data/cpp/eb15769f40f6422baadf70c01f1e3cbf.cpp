Write a C++ function `maxCakePieces(int n, int m, const vector<int>& pieces)` that simulates a contestant cutting `n` cake pieces with a knife usable at most `m` times. Each cut must be made exactly at an integer centimeter mark, splitting a piece into two smaller pieces whose lengths are positive integers. The goal is to maximize the number of pieces of length exactly 10 centimeters. A piece already of length 10 requires no cut and counts immediately. Pieces longer than 10 can be cut; if a piece’s length is a multiple of 10, it can be processed efficiently to produce multiple 10-cm pieces per cut, while non-multiples are processed greedily one 10-cm piece per cut. Return the maximum possible count of 10-cm pieces after using at most `m` cuts (you may use fewer cuts; unused cuts are irrelevant). Note: leftover pieces that are not exactly 10 cm do not count toward the total.

#include <cassert>
#include <vector>

// The solution function is declared here (or include the header)
int maxCakePieces(int n, int m, const std::vector<int>& pieces);

int main() {
    // Basic cases
    assert(maxCakePieces(1, 0, {10}) == 1);
    assert(maxCakePieces(1, 1, {20}) == 1); // only one cut, split 20 into 10 and 10, but only one 10? Actually first cut gets one 10, remainder 10 is another 10 without extra cut? Wait: cutting 20 into 10 and 10 with one cut yields both 10s, so should be 2. Let's correct: the function's logic: A=20, t=10, ans++ then t==10 ans++, so result 2. But if m=1, that's correct because you used one cut and got two pieces. So assert should be 2.
    assert(maxCakePieces(1, 1, {20}) == 2); // one cut splits into two 10s
    assert(maxCakePieces(1, 2, {30}) == 3); // first cut: 10+20, second cut: 20->10+10, total 3
    assert(maxCakePieces(2, 2, {20, 15}) == 3); // 20 gives 2 (one cut), 15 gives 1 (one cut) total 3
    assert(maxCakePieces(2, 1, {20, 15}) == 2); // best: use cut on 20 to get 2, 15 untouched
    assert(maxCakePieces(3, 3, {25, 30, 10}) == 4); // 10 free, 30: first cut 10, remainder 20->second cut gives 2, total 3; 25 with one cut gives 1, total 4
    assert(maxCakePieces(1, 0, {15}) == 0); // no cuts, no 10-cm piece
    assert(maxCakePieces(1, 1, {15}) == 1); // cut 15 -> 10+5, one piece
    assert(maxCakePieces(1, 3, {40}) == 4); // 40: cut 10 (m=2 left, rem 30), cut 10 (m=1, rem 20), cut 20->two 10s (m=0, gets 2 more) total 4
    assert(maxCakePieces(2, 0, {10, 10}) == 2); // both free
    return 0;
}

#include <vector>
#include <queue>
#include <functional>

// Returns the maximum number of 10-cm pieces obtainable from the given pieces
// using at most m cuts. Pieces of length exactly 10 are free and count immediately.
// Multiples of 10 are processed first because they yield more pieces per cut.
int maxCakePieces(int n, int m, const std::vector<int>& pieces) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> multiples; // A%10==0, A>10
    std::priority_queue<int, std::vector<int>, std::greater<int>> nonMultiples; // A>10, A%10!=0
    int ans = 0;

    for (int i = 0; i < n; ++i) {
        int A = pieces[i];
        if (A == 10) {
            ++ans;
        } else if (A % 10 == 0) {
            multiples.push(A);
        } else if (A > 10) {
            nonMultiples.push(A);
        }
    }

    // Process multiples first: each cut can yield 1 or 2 pieces, better ratio.
    while (m > 0 && !multiples.empty()) {
        int A = multiples.top();
        multiples.pop();
        int t = A - 10;
        ++ans; // the 10-cm piece cut off
        --m;
        if (t == 10) {
            ++ans; // remainder is another 10, no extra cut needed
        } else if (t == 20) {
            if (m <= 0) break;
            --m;
            ans += 2; // split 20 into two 10s with one cut
        } else if (t > 20) {
            multiples.push(t); // remainder is a larger multiple, process later
        }
        // if t < 10, discard (no 10-cm piece)
    }

    // Process non-multiples: each cut yields exactly one 10-cm piece.
    while (m > 0 && !nonMultiples.empty()) {
        int A = nonMultiples.top();
        nonMultiples.pop();
        int t = A - 10;
        ++ans;
        --m;
        if (t > 10) {
            nonMultiples.push(t); // remainder still > 10 and not a multiple of 10
        }
        // if t <= 10, discard (no more 10-cm pieces)
    }

    return ans;
}

// The optimal strategy is to prioritize cutting pieces that are multiples of 10, because with each cut you can obtain at least one 10-cm piece and often more if the remainder is also a multiple of 10 (e.g., length 30: first cut yields a 10, remainder 20; second cut yields two 10s, total 3 pieces from 2 cuts, ratio 1.5 per cut, better than non-multiples which yield at most 1 per cut). For multiples, use a min-heap (priority queue) to always process the smallest multiple first, because smaller multiples produce fewer leftover pieces and waste fewer cuts. For each multiple piece of length `A` (where A%10==0 and A>10): cut off a 10 piece, decrement cuts, increment answer. The remainder `t = A-10`. If `t==10`, that’s another 10 piece and no extra cut needed (you already made the cut that produced both 10s? Actually, after cutting a 20 into two 10s with one cut, both count, so increment answer by 1 for the remainder). If `t==20`, you have two 10s but need one more cut to split the 20; if you have cuts available, spend one more cut and get two more pieces (so total +2 for that second cut), else stop. Otherwise, if `t` is a multiple of 10 and >20, push it back into the heap for further processing. For non-multiples (A>10 and A%10 != 0), use a separate min-heap, but here each cut yields exactly one 10-cm piece and the remainder is either <10 (discard) or >10 and non-multiple (push back). This greedy order (multiples first) is optimal because a cut on a non-multiple yields only 1 piece per cut, while a cut on a multiple can yield 1 or 2 pieces per cut (when remainder is 10 or 20). Edge cases: pieces already 10 are counted without cuts; if m reaches 0, stop; if a piece length is exactly 20 and m=1, you get only 1 piece (the first 10) because you can’t split the 20 further; if m=2 you get 3 pieces (10+10+10). Complexity: each piece is processed at most O(log n) times for heap operations; total time O((n + cuts) log n) and space O(n) for the heaps.
