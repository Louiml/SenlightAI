/*
Write a C++ function `int numberOfAlternatingGroups(const std::vector<int>& colors, int k)` that counts the number of groups of exactly `k` consecutive tiles on a circular arrangement of tiles, where each tile is colored either red (`0`) or blue (`1`). A group is considered "alternating" if every adjacent pair within that group (including the wrap-around from the last tile of the circle to the first) has different colors. The circle is represented by a vector of integers, and the first and last elements are adjacent. The function should return the total number of distinct starting positions (0-indexed) such that the `k` tiles starting from that position form an alternating sequence. The input size `n` satisfies `3 <= n <= 10^5`, `k` satisfies `3 <= k <= n`, and each element is either `0` or `1`. Assume the input vector is valid and non-empty.
*/
#include <vector>

/**
 * Counts the number of circular groups of exactly k consecutive tiles
 * that alternate in color. Tiles are represented by 0 (red) and 1 (blue).
 * The first and last tiles are adjacent because the arrangement is circular.
 *
 * @param colors Vector of 0/1 values representing the tile colors.
 * @param k      The fixed group length (3 <= k <= colors.size()).
 * @return       The number of starting positions that form an alternating group.
 */
int numberOfAlternatingGroups(const std::vector<int>& colors, int k) {
    const int n = static_cast<int>(colors.size());
    int answer = 0;
    int run_length = 0;  // length of consecutive alternating tiles ending at current index

    // Iterate over the doubled circle to handle wrap-around.
    for (int i = 0; i < 2 * n; ++i) {
        // Check the edge between tile (i-1) and tile i (circularly).
        if (i > 0 && colors[i % n] == colors[(i - 1) % n]) {
            // Edge is invalid -> reset the alternating run to just the current tile.
            run_length = 1;
        } else {
            // Edge is valid -> extend the run.
            ++run_length;
        }

        // Only count a group when its ending index is in the second copy (i >= n),
        // so each circular starting position is counted exactly once.
        if (i >= n && run_length >= k) {
            ++answer;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

int main() {
    // Example 1 from the prompt
    std::vector<int> colors1 = {0, 1, 0, 1, 0};
    assert(numberOfAlternatingGroups(colors1, 3) == 3);

    // Example 2 from the prompt
    std::vector<int> colors2 = {0, 1, 0, 0, 1, 0, 1};
    assert(numberOfAlternatingGroups(colors2, 6) == 2);

    // Example 3 from the prompt
    std::vector<int> colors3 = {1, 1, 0, 1};
    assert(numberOfAlternatingGroups(colors3, 4) == 0);

    // Entire circle alternating, k = n -> exactly one group
    std::vector<int> colors4 = {0, 1, 0, 1, 0, 1};
    assert(numberOfAlternatingGroups(colors4, 6) == 1);

    // k = n but circle not alternating -> zero
    std::vector<int> colors5 = {0, 0, 1, 0, 1, 1};
    assert(numberOfAlternatingGroups(colors5, 6) == 0);

    // All same color, any k >= 3 -> zero
    std::vector<int> colors6 = {0, 0, 0, 0, 0};
    assert(numberOfAlternatingGroups(colors6, 3) == 0);

    // Short alternating run, only one valid starting position
    std::vector<int> colors7 = {0, 1, 0, 0, 0};
    // k=3: starting at 0 -> [0,1,0] valid; starting at 1 -> [1,0,0] invalid; etc.
    assert(numberOfAlternatingGroups(colors7, 3) == 1);

    // Large k covering almost whole circle, one break
    std::vector<int> colors8 = {0, 1, 0, 1, 1, 0, 1};
    // n=7, k=6: valid groups start at index 0? [0,1,0,1,1,0] has 0-1-0-1-1-0 -> edge 1-1 invalid -> no.
    // Start at 1: [1,0,1,1,0,1] -> 1-0-1-1 invalid -> no.
    // Start at 2: [0,1,1,0,1,0] -> 0-1-1 invalid -> no.
    // Start at 3: [1,1,0,1,0,1] -> 1-1 invalid -> no.
    // Start at 4: [1,0,1,0,1,0] -> all alternating? 1-0,0-1,1-0,0-1,1-0 -> yes! So one.
    // Start at 5: [0,1,0,1,0,1] -> all alternating? 0-1,1-0,0-1,1-0,0-1 -> yes! So two.
    // Start at 6: [1,0,1,0,1,0] -> all alternating? 1-0,0-1,1-0,0-1,1-0 -> yes! So three.
    // But note direction: we need to check all edges. Let's compute more systematically:
    // The circle has edges: 0-1,1-0,0-1,1-1(invalid),1-0,0-1,1-0 (last to first)
    // So the only invalid edge is between index 3 and 4.
    // A group of length 6 must have 5 consecutive valid edges. The longest run of valid edges is from index 4 to 3 (wrap): edges 4-5,5-6,6-0,0-1,1-2 = 5 edges, so group starting at 4 is valid. Also from index 5 to 4: edges 5-6,6-0,0-1,1-2,2-3 = 5 edges? Edge 2-3 is 0-1 valid, but that's 5 edges, so group starting at 5 is valid. From index 6 to 5: edges 6-0,0-1,1-2,2-3,3-4 (3-4 invalid) -> no. So only two valid groups? Let's check starting at 4: tiles 4,5,6,0,1,2 = [1,0,1,0,1,0] fully alternating? 1-0,0-1,1-0,0-1,1-0 yes. Starting at 5: tiles 5,6,0,1,2,3 = [0,1,0,1,0,1] yes. Starting at 6: tiles 6,0,1,2,3,4 = [1,0,1,0,1,1] has 1-1 at end? Actually edges: 6-0 (1-0),0-1,1-0,0-1,1-1 (invalid) -> no. So answer 2. Our function should return 2.
    assert(numberOfAlternatingGroups(colors8, 6) == 2);

    // Edge case: k equal to n and only one alternating group
    std::vector<int> colors9 = {1, 0, 1, 0, 1, 0, 1, 0}; // n=8, fully alternating
    assert(numberOfAlternatingGroups(colors9, 8) == 1);

    // Edge case: k=3, many positions but only few alternating
    std::vector<int> colors10 = {0, 1, 1, 0, 1, 0, 0};
    // n=7, k=3. Check all starts:
    // 0:[0,1,1] invalid (1-1)
    // 1:[1,1,0] invalid
    // 2:[1,0,1] valid
    // 3:[0,1,0] valid
    // 4:[1,0,0] invalid
    // 5:[0,0,1] invalid
    // 6:[0,1,0]? Actually 6,0,1 = [0,0,1]? colors[6]=0, colors[0]=0, colors[1]=1 -> [0,0,1] invalid
    // So answer 2.
    assert(numberOfAlternatingGroups(colors10, 3) == 2);

    return 0;
}
// The key observation is that for a fixed `k`, a group starting at position `i` (in the original array) is alternating if and only if for every consecutive pair within that group (considering circular adjacency), the colors differ. Instead of checking each group individually in O(n*k), we can precompute a "valid adjacency" property: define a boolean that tells whether the edge between tile `i` and `i+1` (mod n) is valid (i.e., the two colors differ). Then a group of length `k` is valid iff all `k-1` consecutive edges are valid. To handle circularity, we can duplicate the array conceptually by iterating over an extended index `j` from 0 to `2n-1`, and maintain a running count `cnt` of consecutive valid edges ending at the current tile. When we encounter an invalid edge (i.e., `colors[j] == colors[(j-1) % n]`), we reset `cnt` to 1 (since the current tile itself starts a new run). Otherwise, we increment `cnt`. The condition for a valid group ending at position `j` is that `cnt >= k` (meaning the last `k-1` edges are all valid, so the last `k` tiles form an alternating group). However, we must only count groups whose starting position is in the original range (0 to n-1), so we only add to the answer when `j >= n` and `cnt >= k`. Why `j >= n`? Because we iterate up to `2n-1`, and each valid group that starts at index `s` (0 <= s < n) will be counted exactly once when `j = s + k - 1` which is between `k-1` and `n + k - 2`. Since `k <= n`, the index `s + k - 1` can be as large as `2n-2` for `s = n-1` and `k = n`, but for `s` in `[0, n-1]`, the condition `j >= n` ensures we don't double-count groups that start in the duplicated part. Actually, careful: a group starting at `s` (0 <= s < n) will be counted when `j = s + k - 1`. The smallest `j` is `k-1` (when `s=0`), and the largest `j` is `(n-1) + k - 1 = n + k - 2`. Since `k >= 3`, `j` ranges from 2 to `n+k-2`. We only want to count each distinct starting position once, and we only want to count when `s` is in `[0, n-1]`. However, note that `s = (j - (k-1) + n) % n` for the original circle. For `j` from `k-1` to `n+k-2`, each `s` in `[0,n-1]` appears exactly once. If we count all `j` in this range, we get exactly `n` possible starting positions, but we must ensure that we count only those with `cnt >= k`. So the condition `j >= n` is a safe way to avoid counting duplicates? Let's verify: For `s` in `[0, n-1]`, the corresponding `j = s + k - 1`. For `s=0`, `j = k-1` which is `< n` if `k-1 < n`, i.e., always because `k <= n` and `k>=3` so `k-1 < n` unless `k=n` then `k-1 = n-1 < n`. So for `s=0`, `j` is less than `n`. If we require `j >= n`, we would miss that group. That's a problem. Let's re-examine the provided solution: It uses `ans += i >= n && cnt >= k ? 1 : 0;`. For `i` from 0 to `2n-1`. For `i` in `[0, n-1]`, it doesn't count. For `i` in `[n, 2n-1]`, it counts if `cnt >= k`. How does that cover all starting positions? Consider the original array with indices `0..n-1`. A group starting at `s` covers tiles `s, s+1, ..., s+k-1` (mod n). The last index (in the duplicated array) when we process this group is `i = s + k - 1` (using modulo for the actual tile colors). Since we process `i` from 0 to `2n-1`, and we use `colors[i % n]`, the group starting at `s` is fully processed when we reach `i = s + k - 1`. For `s=0`, that's `i = k-1`, which is `< n` (since `k <= n`). That would be missed if we only count when `i >= n`. But the provided solution works for the examples, so let's test: Example 1: `colors=[0,1,0,1,0]`, `n=5`, `k=3`. All adjacent edges are alternating (0-1,1-0,0-1,1-0,0-0? Wait last and first: colors[4]=0, colors[0]=0, so they are equal! So the edge between index 4 and 0 is not alternating. So the circle is not fully alternating. Let's compute the answer: groups of length 3: start at 0: [0,1,0] alternating? 0!=1,1!=0 yes. start at 1: [1,0,1] yes. start at 2: [0,1,0] yes? Actually indices 2,3,4 = [0,1,0] alternating yes. start at 3: [1,0,0]? indices 3,4,0 = [1,0,0] – 1!=0,0==0 not alternating. start at 4: [0,0,1]? indices 4,0,1 = [0,0,1] – 0==0 not alternating. So answer 3. The solution: for i=0 to 9. Let's simulate: i=0: i%5=0, previous? i=0 so skip condition. cnt initially? We need to initialize cnt. In the code, cnt starts as 0. At i=0, since i==0, the condition `if (i && colors[i%n] == colors[(i-1)%n])` is false because `i` is 0, so we go to else: ++cnt => cnt=1. Then `ans += i>=n && cnt>=k ? 1:0` => i=0 <5, so no. i=1: i%5=1, (i-1)%5=0, colors[1]=1, colors[0]=0, different, so else: ++cnt => cnt=2. i=1<5 no. i=2: colors[2]=0, colors[1]=1 different => cnt=3. i=2<5 no. i=3: colors[3]=1, colors[2]=0 different => cnt=4. i=3<5 no. i=4: colors[4]=0, colors[3]=1 different => cnt=5. i=4<5 no. i=5: i%5=0, (i-1)%5=4, colors[0]=0, colors[4]=0 equal => reset cnt=1 (since condition true, cnt=1). i=5 >=5 and cnt=1 <3 => no. i=6: i%5=1, (i-1)%5=0, colors[1]=1, colors[0]=0 diff => ++cnt => cnt=2. i=6>=5 but cnt<3 no. i=7: i%5=2, (i-1)%5=1, colors[2]=0, colors[1]=1 diff => cnt=3. i=7>=5 and cnt>=3 => ans++ (1). i=8: i%5=3, (i-1)%5=2, colors[3]=1, colors[2]=0 diff => cnt=4. i=8>=5 ans++ (2). i=9: i%5=4, (i-1)%5=3, colors[4]=0, colors[3]=1 diff => cnt=5. i=9>=5 ans++ (3). So answer 3. This matches. Why does it work? Because we count a group when the ending index `i` is at least `n`. But for a group starting at `s` (0<=s<n), the ending index in the doubled array is `s + k - 1`. For `s=0`, that's `k-1` which is < n (since k<=n, so k-1 <= n-1). So we would not count that group directly. However, notice that the same group also appears when we consider the wrap-around: the group starting at index `s` is identical to the group starting at index `s+n` in the doubled array (since it's circular). So when we process `i = s + k - 1 + n`, which is >= n, the `cnt` value at that point reflects the same run of consecutive alternating edges that started at `s`. Let's verify: For the group starting at `s`, the run of valid edges begins at the edge between `s` and `s+1`, and continues for `k-1` edges. In the doubled iteration, when we pass the point where we reset cnt due to an invalid edge, the cnt value at position `i` equals the length of the consecutive valid run ending at `i`. For a valid group of length `k`, the run of valid edges ending at its last tile has length at least `k-1`, so cnt >= k when we reach that last tile. For the original starting point `s`, the last tile is at index `s+k-1` in the original circle. In the doubled array, this index appears as `s+k-1` (if `s+k-1 < n`) and also as `s+k-1+n` (which is always >= n). The value of cnt at the latter index is the same as at the former (since the sequence repeats), because the edges are identical. So counting when `i >= n` effectively counts each valid group exactly once, from the second occurrence of its ending index. This avoids double-counting and ensures all starting positions are covered, because every valid group's ending index has a representation in `[n, 2n-1]` (since the maximum ending index is `n-1 + k - 1` which is `n+k-2`; adding `n` gives `2n+k-2` which is beyond `2n-1`? Wait, we iterate up to `2n-1` inclusive. For `s=n-1`, `k=n`, ending index is `n-1+n-1=2n-2`, which is < 2n-1? Actually 2n-2 <= 2n-1, so it's within range. But we need `i >= n`, and the second occurrence is `s+k-1+n`. For `s=0`, `k=n`, that's `0+n-1+n = 2n-1`, which is exactly the last index. So all valid groups have a corresponding `i` in `[n, 2n-1]` where `i = s + k - 1 + n` (which is >= n because s>=0,k-1>=0). However, note that `i` might exceed `2n-1` if `s+k-1+n > 2n-1`, i.e., `s+k-1 > n-1`. But `s <= n-1` and `k-1 <= n-1`, so `s+k-1` can be as large as `2n-2`, adding `n` gives `3n-2` which is > `2n-1`. That would be beyond our loop. So we only process up to `2n-1`, which is exactly the maximum needed. For `s=n-1, k=n`, `s+k-1 = 2n-2`, adding n gives `3n-2` which is out of range. But we don't need to consider that because the group starting at `n-1` with length `n` is the entire circle; its ending index in the original is `(n-1 + n-1) % n = n-2`? Wait, careful: For a circular group of length k, the tiles are `s, s+1, ..., s+k-1` with indices mod n. The "ending tile" is the last tile in the sequence, which is at index `(s+k-1) % n`. When we process the doubled array, we track the run of valid edges. The group is valid if the edges from `s` to `s+1`, ..., up to `s+k-2` to `s+k-1` are all valid. The last edge considered is between `s+k-2` and `s+k-1` (mod n). In the doubled array, the index `i` that corresponds to the last tile (i.e., the position where we have processed the last tile) is `s+k-1` (if we use linear indices without modulo when incrementing). But since we process `i` from 0 to 2n-1, and we access `colors[i%n]`, the run length `cnt` at `i` tells us how many consecutive tiles ending at `i` have alternating colors. For a valid group of length k, the run of consecutive alternating tiles ending at position `i` (where `i` is the last tile's index in the doubled array) must be at least k. However, the run might start before `s`. The condition `cnt >= k` means that the last k tiles (from `i-k+1` to `i`) form an alternating sequence. For that to correspond to a valid group starting at `s`, we need `i - k + 1` to be congruent to `s` modulo n, and also the entire k tiles must be within the circle. Since we iterate up to 2n-1, each starting position `s` in `[0,n-1]` will have its group ending at `i = s + k - 1` and also at `i' = s + k - 1 + n`. The latter is guaranteed to be >= n (since s>=0, k-1>=0, so i' >= n). But is i' always <= 2n-1? That requires `s + k - 1 + n <= 2n-1` => `s + k - 1 <= n-1` => `s + k <= n`. Since `k <= n` and `s <= n-1`, `s + k` can be as large as `n-1 + n = 2n-1` which is > n. So for some groups, i' exceeds 2n-1. For example, n=5, k=5, s=4: s+k-1 = 4+5-1=8, +n=13 > 9 (2n-1=9). So that group's ending at i=8 is the only one within range, and i=8 >= n (8>=5), so it is counted. So it's fine. In general, for any s, the ending index `s+k-1` in the original (mod n) is between 0 and n-1. When we process the doubled array, the same ending pattern appears at both `s+k-1` and `s+k-1+n`. But `s+k-1` is between `k-1` (>=2) and `n-1 + n-1 = 2n-2`? Wait, `s+k-1` without modulo can be as large as `n-1 + n-1 = 2n-2` if s=n-1 and k=n. That's greater than n-1. But in the doubled array, we process up to 2n-1, so `s+k-1` might be within [0,2n-1] for all s? For s=n-1,k=n, s+k-1 = 2n-2, which is <= 2n-1, so yes it's within the loop. So the run ending at i = s+k-1 might be >= n for some s. For example, n=5,k=5,s=4 gives i=8 which is >=5, so it's counted there. For s=0,k=5, i=4 which is <5, so it's not counted at i=4. But we have the duplicate at i=4+5=9 which is >=5 and <=9, so it's counted. So the condition `i >= n` simply ensures we only count each valid group from its "second half" occurrence if needed, avoiding double counting when i is already >=n for the first occurrence. But what about a group where s+k-1 >= n already? Then it would be counted twice if we didn't have the condition? Let's test: n=5,k=5,s=4 gives i=8 (>=5) and i'=8+5=13 (out of range). So only one count. For s=3,k=5: s+k-1=7 (>=5) and i'=12 (out of range). So only one count. For s=2,k=5: i=6 (>=5), i'=11 out. So only one count. For s=1,k=5: i=5 (>=5), i'=10 out. So only one count. For s=0,k=5: i=4 (<5), i'=9 (>=5) counted. So each s gives exactly one i >= n in the range [n, 2n-1]? Need to verify: For s such that s+k-1 >= n, then i = s+k-1 is in [n, 2n-2] (since s<=n-1, k<=n -> s+k-1 <= 2n-2). And i' = s+k-1+n >= n+n = 2n > 2n-1, so out of range. So only i is counted. For s such that s+k-1 < n, then i' = s+k-1+n is in [n, 2n-2]? Actually s+k-1 < n implies s+k-1 <= n-1. Then i' = s+k-1+n <= n-1+n = 2n-1, so it's in range. So exactly one of i or i' falls into [n, 2n-1]? Let's check: if s+k-1 < n, then i is <n, i' is in [n, 2n-1]; if s+k-1 >= n, then i is >=n and <=2n-2, i' is >2n-1. So yes, each starting position s yields exactly one i in [n, 2n-1] that corresponds to the end of the group. Therefore, counting only when i >= n and cnt >= k gives the correct count. The algorithm works because when we process i, `cnt` is the length of the longest run of alternating tiles ending at i. If cnt >= k, then the k tiles ending at i form an alternating group, and we count it if i is in the designated "counting zone" to avoid duplicates. This is O(n) time and O(1) extra space. Edge cases: k=n (entire circle) – need to check if the whole circle is alternating. The loop runs 2n times, and we count only one if the entire circle is alternating. Because there will be exactly one i where cnt>=k in the zone. Also, if the array has no alternating edges (e.g., all same color), then cnt never reaches k except maybe after a long run? Actually if all same, every edge is invalid, so cnt resets to 1 every step, never >=k unless k=1 but k>=3. So answer 0. If there is a run of alternating edges of length L, then cnt will be L+1 after that run. For a group of length k to be valid, we need a run of at least k consecutive alternating tiles, which is equivalent to at least k-1 consecutive valid edges. The algorithm correctly counts each such run. Complexity: O(n) time, O(1) space.
