/*
You are given a list of `n` people, each with a weight `w[i]` and a binary string `s[i]` where `'0'` means the person is classified as an adult by a preliminary model and `'1'` means classified as a child. You need to choose a real number `X` (the decision boundary) such that for every person with `w[i] < X` you reclassify them as a child, and for every person with `w[i] >= X` you reclassify them as an adult. Your goal is to maximize the number of correct classifications, where the correct label for each person is the complement of their given `s[i]` (i.e., if `s[i] == '0'`, the true label is `'1'`, and if `s[i] == '1'`, the true label is `'0'`). Write a function `maxCorrect` that takes `n`, the string `s`, and a vector of weights `w`, and returns the maximum possible number of correct classifications you can achieve by choosing any real number `X`. Note that `X` can be any real number, not necessarily equal to any of the weights, and the classification rule is strict inequality for the child side.
*/

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <cstdint>

// Returns the maximum number of correct classifications achievable by choosing a threshold X.
// For w[i] < X, we classify as child (true label is '1' if s[i]=='0'? no, true label is opposite).
// Let's clarify: s[i] is preliminary, true is opposite. So:
// - If s[i]=='0', true label is '1' (child), correct if w[i] < X.
// - If s[i]=='1', true label is '0' (adult), correct if w[i] >= X.
int maxCorrect(int n, const std::string& s, const std::vector<long long>& w) {
    // Total number of people with preliminary label '1' (true adults).
    int trueAdults = 0;
    for (char ch : s) {
        if (ch == '1') ++trueAdults;
    }
    int trueChildren = n - trueAdults;

    // Pair each weight with the delta: for s[i]=='0', crossing weight increases correct by +1;
    // for s[i]=='1', crossing weight decreases correct by -1.
    std::vector<std::pair<long long, int>> events;
    events.reserve(n);
    for (int i = 0; i < n; ++i) {
        int delta = (s[i] == '0') ? +1 : -1;
        events.emplace_back(w[i], delta);
    }
    std::sort(events.begin(), events.end());

    // Start with X less than all weights: everyone considered adult, correct = trueAdults.
    int current = trueAdults;
    int best = current; // also handle X greater than all weights later.

    // Sweep through sorted events. For each distinct weight, apply all deltas for that weight
    // only after moving X beyond that weight. Since the boundary is strict <, all people with
    // weight equal to X are not children, so they remain adults. Thus we must process all
    // people with the same weight together *after* moving past that weight.
    int idx = 0;
    while (idx < n) {
        long long currentWeight = events[idx].first;
        // Apply all deltas for this weight.
        while (idx < n && events[idx].first == currentWeight) {
            current += events[idx].second;
            ++idx;
        }
        // Now X is just above currentWeight, so all people with weight <= currentWeight are children.
        best = std::max(best, current);
    }

    // Also consider X greater than all weights: everyone becomes child.
    best = std::max(best, trueChildren);
    return best;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above; here we test it.

int maxCorrect(int n, const std::string& s, const std::vector<long long>& w); // forward declaration

int main() {
    // Test 1: Example from snippet with n=5, "01010", w=[1,2,3,4,5]? Actually let's construct.
    // s = "0011", w = [1, 2, 3, 4]
    // True labels: complementary: s0='0' -> true child, s1='0' -> true child, s2='1' -> true adult, s3='1' -> true adult.
    // Choose X = 2.5: children with w<2.5: indices 0,1 (both correct). adults w>=2.5: indices 2,3 (both correct) => 4 correct.
    // Can we get 4? choose X=0 => all adult: only adults correct: 2. choose X=5 => all child: 2. Best is 4.
    assert(maxCorrect(4, "0011", {1,2,3,4}) == 4);

    // Test 2: All same weight. s="010", w=[5,5,5].
    // True labels: '0'->child, '1'->adult, '0'->child. Any X: if X>5 all children => 2 correct; if X<=5 all adults => 1 correct. Best = 2.
    assert(maxCorrect(3, "010", {5,5,5}) == 2);

    // Test 3: Single person, s="1", w=[10]. True label adult. Choose X>10 => child => 0 correct; X<=10 => adult => 1 correct. Best=1.
    assert(maxCorrect(1, "1", {10}) == 1);

    // Test 4: Single person, s="0", w=[-3]. True label child. Choose X>-3 => child => 1 correct; X<=-3 => adult => 0. Best=1.
    assert(maxCorrect(1, "0", {-3}) == 1);

    // Test 5: Mixed negatives and duplicates.
    // s="1010", w=[3,3,4,4]. True: '1'->adult, '0'->child, '1'->adult, '0'->child.
    // Try X=3.5: children w<3.5: indices 0?(w=3, s='1' true adult, so not child), wait classify w<X as child. So w=3 are children (indices 0,1): index0 true adult => wrong, index1 true child => correct. w>=3.5 are adults (indices 2,3): index2 true adult => correct, index3 true child => wrong. Total correct=2.
    // Try X=4.5: children w<4.5: indices 0,1,2,3 all children: index0 true adult wrong, index1 child correct, index2 adult wrong, index3 child correct => 2. Adults none => 0. Total=2.
    // Try X=2.5: all adult: adults correct: indices 0,2 => 2 correct. So best=2.
    assert(maxCorrect(4, "1010", {3,3,4,4}) == 2);

    // Test 6: Large weights, all children.
    // n=3, s="000", w=[10,20,30]. True adults? No, true children all. Choose X>30 => all children correct => 3.
    assert(maxCorrect(3, "000", {10,20,30}) == 3);

    // Test 7: All adults.
    // n=3, s="111", w=[1,2,3]. Choose X<=1 => all adult correct => 3.
    assert(maxCorrect(3, "111", {1,2,3}) == 3);

    // Test 8: Boundary equal to weight must be careful.
    // s="01", w=[5,5]. True: '0' child, '1' adult. If X=5, then w<5? both are not <5, so both adult: adult correct for index1 => 1. If X=5.1, both child: child correct for index0 => 1. If X=4.9, both adult -> same. So best=1.
    assert(maxCorrect(2, "01", {5,5}) == 1);

    // Test 9: Mixed where boundary between distinct weights.
    // s="0101", w=[1,2,3,4]. True: child, adult, child, adult. Try X=2.5: children w<2.5: indices 0 (child correct),1 (adult wrong). Adults w>=2.5: indices 2(child wrong),3(adult correct). Total 2. Try X=1.5: children: index0 correct, others adult wrong. Adults: indices 1,2,3: index1 adult correct, others wrong => total 2. Try X=3.5: children: 0,1,2: index0 correct, index1 wrong, index2 correct => 2. adults: index3 correct => total 3. So best 3.
    assert(maxCorrect(4, "0101", {1,2,3,4}) == 3);

    // Test 10: Large n random? Not needed, but we can do one more edge: all same weight and mixed labels.
    // s="01", w=[7,7] already done. Let's do s="010", w=[7,7,7] => true child, adult, child. Any X: if X>7 all children => 2 correct; if X<=7 all adult => 1 correct. Best=2.
    assert(maxCorrect(3, "010", {7,7,7}) == 2);
}

// The problem is equivalent to choosing a threshold `X` on the weight axis. For each person, the preliminary label is `s[i]`, but the true label is the opposite. So a person with `s[i]=='0'` (true child) is correctly classified if you place `X` so that `w[i] < X` (i.e., you reclassify them as child). A person with `s[i]=='1'` (true adult) is correctly classified if `w[i] >= X` (i.e., you reclassify them as adult). Thus, for a fixed `X`, the correct count is:
// - number of true children (s[i]=='0') with weight < X
// - plus number of true adults (s[i]=='1') with weight >= X.
//
// We can precompute the total number of true adults (`tmp` = count of s[i]=='1') and true children (`n - tmp`). Start with `X` very small (less than all weights): then all people are considered adults, so correct count = `tmp`. As we sweep `X` across the sorted weights in increasing order, when we cross a weight of a person with s[i]=='0', that person becomes correctly classified (was previously incorrectly classified as adult, now correctly child), so count increases by 1. When we cross a weight of a person with s[i]=='1', that person becomes incorrectly classified (was previously correctly adult, now child), so count decreases by 1. The answer is the maximum count over all possible positions of `X`. We only need to consider positions between consecutive distinct weights, because within a group of equal weights, the boundary cannot split them (since the condition is strict `<` for child; the boundary `X` either includes all equal weights on one side or the other). So we sort the pairs `(w[i], s[i])` by weight, and after processing each equal-weight group, update the count and take the maximum. Edge case: we also consider `X` greater than all weights (all classified as children), which gives count = `n - tmp`. That is captured by the initial `ans = tmp` and the final `chmax(ans, n - tmp)` after the loop, but we must be careful with the loop’s final group: when the last group has equal weights, after processing them we should compare the count after that group, which corresponds to `X` just above that weight. The implementation in the snippet uses `if(p[i].first != p[i+1].first)` which correctly checks group boundaries only if `p` is sorted and we update the count before comparing. But note that when `i == n-1`, `p[i+1]` is out of bounds; the snippet does not handle that, which is a bug. In our reference solution, we’ll handle the last group separately. Time complexity is O(n log n) due to sorting, space O(n). The edge cases: n=1, all same weights, and weights with duplicates must be handled correctly by updating counts only at group boundaries.
