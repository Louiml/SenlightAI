Write a C++ function `long long maxNonAdjacentPairs(const std::string& a, const std::string& b)` that takes two binary strings `a` and `b` of equal length `n` (1 ≤ n ≤ 10^5) and computes the maximum total score obtainable by selecting positions from 1 to n. For each position `i`, define `value(i) = (a[i]-'0') + (b[i]-'0')`. The score from selecting a position `i` is `value(i)`. Additionally, if two selected positions are adjacent (i and i+1), you get a bonus of 1. The function must return the maximum possible total score (sum of selected `value(i)` + bonuses for adjacent selected pairs). Positions can be selected or skipped independently.

We process the positions left to right while tracking whether the previous position was selected. This is a dynamic programming / greedy problem because the bonus is only +1 for adjacency, which is small compared to potential values (0,1,2). The key observation is that selecting a position with value 2 is always beneficial; selecting with value 1 is also always beneficial because even if it causes a missed +1 bonus with a neighbor, the gain is still at least 1 vs. possibly skipping to get at most +2 from both neighbors (but that would require selecting both neighbors, which is also possible). A simpler greedy approach: iterate each position, decide based on current value and previous selection state.  
We maintain `last` = 1 if previous position was selected, else 0. For each position `i` (from 1 to n):  
- If current value == 2: always select it. Add 2, and if `last==1`, also add 1 bonus (since adjacent). Set `last=1`.  
- If current value == 0: never select it, because selecting gives 0 and may block future bonuses. Set `last=0`.  
- If current value == 1: we need to be careful. If `last==1`, we can select it (gain 1 + 1 bonus = 2) which is at least as good as skipping (gain 0). So select it. If `last==0`, we should select it, because it gives +1 now; skipping would give 0 now, and selecting it also doesn't prevent any future bonus because we can always select the next one regardless. So select it. Thus for value 1, always select it, and if `last==1`, get bonus +1, then set `last=1`.  
This greedy works because value 0 never selected, value 1 and 2 always selected. But careful: what if value is 0 and previous was selected? Skipping is fine. Also, what if value is 1 and previous was not selected but selecting it blocks a potential future bonus with next? Actually selecting it gives +1; if we skip it, we might later select next (value maybe 2) and get +1 bonus with previous (if previous selected) but that previous would have been 0? Actually skipping a 1 gives 0, and then next position might be 2, but we could have selected that 1 and also selected the next 2, getting +1 bonus (since 1 and 2 are adjacent) = 1+2+1 = 4, which is better than skipping the 1 to get 2+? So always selecting 1 is optimal.  
Thus the algorithm is: initialize `last=0`, `ans=0`. For i from 1 to n: compute `val = (a[i-1]-'0') + (b[i-1]-'0')`. If `val==0`: `last=0`. Else (val==1 or 2): `ans += val`; if `last==1`, `ans += 1`; `last=1`.  
Edge cases: n=1; all zeros; alternating patterns. Time O(n), space O(1).  
This matches the provided snippet's logic, which adds sentinel characters '1' and '0' at ends and then subtracts 2 at the end (because boundary adjustments). But we can implement directly without sentinels, ensuring correct handling of boundaries.

#include <string>

// Returns the maximum total score by selecting positions in binary strings a and b.
// value(i) = (a[i]-'0') + (b[i]-'0'), bonus +1 for each adjacent selected pair.
long long maxNonAdjacentPairs(const std::string& a, const std::string& b) {
    long long ans = 0;
    bool last_selected = false; // whether position i-1 was selected

    for (std::size_t i = 0; i < a.size(); ++i) {
        int val = (a[i] - '0') + (b[i] - '0');
        if (val == 0) {
            last_selected = false; // not selected, no contribution
        } else {
            ans += val; // always select positions with value 1 or 2
            if (last_selected) {
                ++ans; // bonus for adjacency with previous selected
            }
            last_selected = true;
        }
    }
    return ans;
}

#include <cassert>
#include <string>

// The solution function is declared above.
// Test cases:
int main() {
    // Single position, value 0
    assert(maxNonAdjacentPairs("0", "0") == 0);
    // Single position, value 1
    assert(maxNonAdjacentPairs("1", "0") == 1);
    // Single position, value 2
    assert(maxNonAdjacentPairs("1", "1") == 2);

    // All zeros: no selection
    assert(maxNonAdjacentPairs("000", "000") == 0);

    // All ones: values all 1, select all, bonuses between each pair
    // n=3: values 1+1+1=3, bonuses for (1,2) and (2,3) => +2, total=5
    assert(maxNonAdjacentPairs("111", "000") == 5);

    // Alternating 2 and 0: only select the 2s, no adjacency
    assert(maxNonAdjacentPairs("101", "101") == 2 + 2); // values 2,0,2 => 4

    // Mixed: "10" "01" gives values 1+1? Actually a="10", b="01": pos0: 1+0=1, pos1:0+1=1 => both selected, bonus +1 => total=1+1+1=3
    assert(maxNonAdjacentPairs("10", "01") == 3);

    // Edge case n=2, values 2 and 2: both selected, bonus =1 => 5
    assert(maxNonAdjacentPairs("11", "11") == 5);

    // Longer case: a="10101", b="01010" => values: 1,1,1,1,1 all ones, all selected, bonuses 4 => total 5+4=9
    assert(maxNonAdjacentPairs("10101", "01010") == 9);

    // Test with zeros and ones: a="1001", b="0000" => values 1,0,0,1 => select positions 0 and 3, no bonus => total 2
    assert(maxNonAdjacentPairs("1001", "0000") == 2);

    return 0;
}
