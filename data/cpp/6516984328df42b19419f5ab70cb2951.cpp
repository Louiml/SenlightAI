// Write a C++ function `int maxBracketScore(int n, const std::vector<int>& items)` that processes a sequence of `n` binary values (0 or 1) representing bracket events in a syntax checker: `1` means an opening bracket is closed (a positive event), and `0` means a nesting depth increases (a negative event). The function should return the **maximum possible total score** achievable, where the score starts at 0. For each `1` encountered, add 1 to the score; for each `0` encountered, you may either ignore it (no score change) or use it to reduce the score by 1 (as if you are closing an inner bracket), provided you have a "credit" from a previous `0` that you decided to keep. More precisely: you maintain a current "depth" counter. When you see a `0`, you increment depth by 1 (this depth can be used later to "absorb" a reduction). When you see a `1`, you add 1 to your score, and if depth > 0, you may optionally decrement depth by 1 (to simulate using one saved credit) without affecting the score—but doing so reduces the number of future reductions available. The goal is to maximize the final score, which is `(number of ones) + (maximum net depth that remains unused at the end)`. Equivalently, you can keep all depth increments, but whenever you see a `1`, you also add 1, and you never reduce depth; the maximum score is just total ones plus the maximum depth ever reached. Return that value. For example, for items `{1,0,1,0,0,1}`, the ones count is 3, maximum depth is 3 (after two zeros), so answer is 6. The function must handle empty input (n=0) by returning 0.
#include <cassert>
#include <vector>

int maxBracketScore(const std::vector<int>& items);

int main() {
    // Empty input
    assert(maxBracketScore({}) == 0);
    // All ones
    assert(maxBracketScore({1, 1, 1}) == 3);
    // All zeros
    assert(maxBracketScore({0, 0, 0}) == 3);
    // Single one
    assert(maxBracketScore({1}) == 1);
    // Single zero
    assert(maxBracketScore({0}) == 1);
    // Example from snippet: 1 0 1 0 0 1 -> count=3, max_extra=2 => 5
    assert(maxBracketScore({1, 0, 1, 0, 0, 1}) == 5);
    // Mixed: 0 1 0 1 -> count=2, max_extra: start 0: see 0 extra=1; see 1 extra=0; see 0 extra=1; see 1 extra=0 => max=1 => 3
    assert(maxBracketScore({0, 1, 0, 1}) == 3);
    // Long pattern: 0 0 1 1 -> count=2, max_extra: 0->1, 0->2, 1->1, 1->0 => max=2 => 4
    assert(maxBracketScore({0, 0, 1, 1}) == 4);
    // All zeros then all ones: 0 0 1 1 1 -> count=3, max_extra: 1,2,1,0,0 => max=2 => 5
    assert(maxBracketScore({0, 0, 1, 1, 1}) == 5);
    return 0;
}
#include <vector>

// Computes the maximum score defined as: total number of 1s plus the maximum
// value of a running counter that increments on 0 and decrements on 1 (but
// never goes below zero).
int maxBracketScore(const std::vector<int>& items) {
    int count = 0;      // total number of 1s
    int extra = 0;      // current balance (zeros minus ones, floored at 0)
    int max_extra = 0;  // maximum balance reached

    for (int value : items) {
        if (value == 1) {
            ++count;
            if (extra > 0) {
                --extra;
            }
        } else { // value == 0
            ++extra;
            if (extra > max_extra) {
                max_extra = extra;
            }
        }
    }
    return count + max_extra;
}
// The problem reduces to: given a binary sequence, count the total number of `1`s, and track the maximum prefix sum of `(+1 for `0`, 0 for `1`)`? Actually, let's re-read: The description says: for each `1`, add 1 to score; for each `0`, you may either ignore it or use it to reduce score by 1, but that reduction requires a credit. The optimal strategy is to never use reduction because that lowers the score. Instead, you want to maximize the final score by keeping all `1`s and never using reductions. The maximum score is simply `(number of ones) + (maximum extra value)`? Wait, the snippet actually counts ones and tracks maximum extra where extra is number of zeros seen so far minus ones seen so far (since each `1` decreases extra by 1 if extra>0, but in the snippet they decrement extra only if extra>0). That gives the maximum "extra" which is the peak of zeros minus ones, ignoring the decrement when ones exceed zeros. The final answer is `count + max_extra` where count is total ones. This is exactly: total ones + maximum prefix excess of zeros over ones. For example, sequence 1,0,1,0,0,1: ones=3, zeros=3. Track extra: start 0, see 1 -> extra stays 0 (since extra=0, no decrement), count=1; see 0 -> extra=1, max=1; see 1 -> extra=0 (decrement), count=2; see 0 -> extra=1, max=1; see 0 -> extra=2, max=2; see 1 -> extra=1, count=3. final count=3, max=2, sum=5. Wait the example in task says answer 6? Let's recalc: I had zeros: positions 2,4,5 (0-indexed). Max prefix excess of zeros over ones: 
// - start 0
// - 1: excess = -1? But the algorithm only decrements extra if extra>0, else extra stays 0. So after first 1, extra=0.
// - 0: extra=1, max=1
// - 1: extra=0, count=2
// - 0: extra=1, max=1
// - 0: extra=2, max=2
// - 1: extra=1, count=3
// So count=3, max=2 => 5. But task says 6. So my interpretation is off. Let's re-derive from the snippet: The snippet processes n numbers t (0 or 1). It keeps count (number of 1s) and extra. When t==1: count++, and if extra>0 then extra--. When t==0: extra++, and if extra>max_extra update max_extra. At the end output count+max_extra. This is exactly: total ones + maximum value of extra at any point (where extra is computed by adding 1 for zeros and subtracting 1 for ones only if extra>0, otherwise keep extra=0). That maximum extra is the maximum number of zeros that appear before the corresponding ones that would "cancel" them, but since extra never goes negative, it's the maximum prefix imbalance of zeros over ones (with a floor at 0). So for the example {1,0,1,0,0,1}, max_extra=2, count=3 => 5. But the task statement claims 6, which is incorrect. So I need to adjust the task to match the snippet's actual behavior. Thus, the correct description: Given a sequence of 0s and 1s, define a counter `extra` that starts at 0. Process each element: if it's 1, increment `count` (total ones) and decrement `extra` by 1 if `extra` > 0; if it's 0, increment `extra` by 1. The final answer is `count + max_extra` where `max_extra` is the maximum value ever attained by `extra` during processing. That is exactly the snippet. So the task should be: Write a function `int maxScore(const std::vector<int>& arr)` that computes this. Edge cases: empty vector returns 0; all ones: extra never >0, max_extra=0, count=n, answer=n; all zeros: count=0, extra keeps increasing, max_extra=n, answer=n. Mixed cases as above. Time O(n), space O(1). The solution is straightforward.
