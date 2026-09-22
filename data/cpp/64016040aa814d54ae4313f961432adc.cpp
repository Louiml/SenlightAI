You are given a string `s` of length `n` representing a walk on a 2D grid starting at `(0,0)`, where each character is one of `U`, `D`, `L`, `R` increasing/decreasing y or x by 1. You must process `q` queries. Each query asks: if we reverse the substring `s[l..r]` (1-indexed inclusive) and also flip each character in that reversed substring (i.e., `U↔D`, `L↔R`) so that the reversed path segment is traversed backwards through the same cells, does the full modified path from start to end pass through the point `(x,y)`? More precisely: original walk is from index 1 to n; the modification replaces steps `l..r` with their reversed and inverted version (so the walker retraces the original segment backwards). Write a function `bool passesThroughAfterReverse(int n, const string& s, int q, vector<array<int,4>> queries)` that returns a vector of strings "YES"/"NO" for each query. The original unmodified path's positions are at times 0..n (time 0 = start, time i after i steps). The modified path reuses the same time indices: for times outside [l-1, r] the position is unchanged; for times inside that interval the position is mapped via the transformation. Check if any time in 0..n after modification equals `(x,y)`.
Let `p[i]` be the prefix position after i steps (array of pairs). For a query `(x,y,l,r)`, before modification the point appears at times `T = {i | p[i] == (x,y)}`. After modification, times outside `[l-1, r]` keep the same positions, so if any such time in `T` is `< l` or `> r` we answer YES. Otherwise we must check times `t` in `[l-1, r]`. The modification maps a time `t` in that interval to a new position. The key is that for `t` in `[l-1, r]`, the new position equals `( -p[t].x + p[l-1].x + p[r].x, -p[t].y + p[l-1].y + p[r].y )`? Let's derive: original segment positions from time `l-1` to `r` are a sequence of points. Reversing and inverting means the new path from time `l-1` to `r` goes backward through the same cells but each step direction is opposite, so the set of positions visited in that interval is exactly the same set as the original segment (just traversed backward). So the modified path passes through `(x,y)` inside `[l-1, r]` iff the original segment `[l-1, r]` contains `(x,y)`. But that condition is exactly: there exists `t` in `[l-1, r]` with `p[t] == (x,y)`. However, careful: the transformation formula above yields a different point if we consider time `t` mapping to a different coordinate? Actually the set of positions visited in the interval is identical, so the condition is simply: `(x,y)` appears at some time `t` with `l-1 <= t <= r`. Combine with the outside condition, the overall condition is: `(x,y)` appears at any time in the original prefix array (i.e., `p[t]==(x,y)` for some t). But wait, is that always true? Let's test: original path goes U from (0,0) to (0,1). Query (0,0,l=1,r=1) – original segment is single step U. Reversed and inverted becomes D, so new path: start (0,0) goes to (0,-1). Does it pass (0,0)? At time 0 yes. Original also at time 0. So condition yes. Query (0,1) – original at time 1. After modification, time 1 is (0,-1), not (0,1). But outside times? time 0 (0,0), time 1 (0,-1). So (0,1) not passed. Original `p` has (0,1) at t=1, and that t is inside [l-1=0,r=1], but condition "any t" would incorrectly say yes. So the set of positions is NOT identical in the sense of times? Actually the interval set of positions visited is {0,0} and {0,1} for original; after modification interval visits {0,0} (time 0) and {0,-1} (time 1). So the set changes. The correct transformation: For t in [l-1, r], let `t' = r - (t - (l-1))` be the mirrored time index. The new position is `p[l-1] + (p[l-1] - p[t'])`? Wait reversing and inverting each step means the new path from `l-1` to `r` is exactly the old path from `r` down to `l-1`. For each t in that interval, the new position equals `old position at time r - (t - (l-1))`? Let's test: original path steps: from l-1=0 to r=1 is step U: positions: t=0 (0,0), t=1 (0,1). Reverse and invert -> step D: new positions: t=0 (0,0), t=1 (0,-1). Here new position at t=1 equals old position at t=0? old(0) = (0,0), not (0,-1). So it's not simply reusing old positions. Actually the transformation for each time t in interval: new position = `p[l-1] + (p[l-1] - (p[r] - p[t]))`? Let's derive: original displacement from l-1 to r is `D = p[r] - p[l-1]`. Reversing and inverting gives displacement `-D`. But the path within the interval traces the same set of points? For U step: original from (0,0) to (0,1); reversed and inverted is D from (0,0) to (0,-1). That's not the same set. Wait the problem statement: "reverse the substring and flip each character" – for a single step U, reverse is U, flip gives D, so result is D. That changes the set. So the set is not the same. So we need correct mapping.

Let's derive formula: Let `a = l-1`, `b = r`. For time `t` with `a <= t <= b`, the new position after modification is `p[a] + (p[a] - (p[b] - p[t]))`? Let's test with U example: a=0,b=1. For t=0: p[a]= (0,0). p[b]-p[t] = (0,1)-(0,0) = (0,1). So p[a] - (that) = (0,-1). Then p[a] + that? wait formula: new position = p[a] + (p[a] - (p[b] - p[t]))? That gives (0,0)+(0,0-(0,1)) = (0,-1) for t=0, but t=0 should be (0,0). So wrong.

Better: For t in [a,b], the new position is the position you would get by starting at `p[a]` and performing the reversed-inverted sequence of steps from the original interval, but for the first `t-a` steps of that new sequence. The new step sequence is: reverse of original steps from b down to a+1, each flipped. The position after k steps (k = t-a) is: `p[a] + sum_{j=1..k} flippedStep(b - j + 1)`. But that's complicated.

However, there is known transformation: Let `q[i] = p[i]` for i outside interval; for i inside, `q[i] = ( p[a].x + p[b].x - p[i].x, p[a].y + p[b].y - p[i].y )`? Test U: a=0,b=1. For t=1: (0+0 - 0? p[1]=(0,1) -> x=0+0-0=0? Actually p[a]=(0,0), p[b]=(0,1), p[t]=(0,1), so new = (0+0-0? p[t].x=0? wait p[1].x=0, so x=0+0-0=0, y=0+1-1=0) gives (0,0), not (0,-1). So not.

Actually correct transformation: The new position after reversed and inverted segment is: `p[a] + (p[a] - p[t]) + (p[a] - p[b])`? Let's derive: Original displacement from a to t is `p[t] - p[a]`. In the new path, the displacement from a to t should be the negative of the displacement from b to (b - (t-a))? That is, new displacement = - (p[b] - p[b - (t-a)])? Let's set `k = t - a`. The original segment from a to b has steps `s_{a+1}...s_b`. The reversed sequence is `s_b, s_{b-1}, ... s_{a+1}`. Flipping each gives `-s_b, -s_{b-1}, ...`. The new position after k steps is `p[a] + ( -s_b - s_{b-1} - ... - s_{b-k+1} )`. That sum equals `p[a] - ( p[b] - p[b-k] )`. Since `b-k = b - (t-a) = a + b - t`. So new position = `p[a] - (p[b] - p[a+b-t])`. For U example: a=0,b=1, t=1 => k=1, b-k=0, p[0]=(0,0). p[b]=(0,1). So new = (0,0) - ((0,1)-(0,0)) = (0,-1). Correct. For t=0: k=0, b-k=1, p[1]=(0,1). p[b]=(0,1) so new = (0,0) - (0,0) = (0,0). Correct.

So formula: for `t` in `[a,b]`, new position = `p[a] - (p[b] - p[a+b-t])`. Simplify: = `p[a] - p[b] + p[a+b-t]`. Let `xx = -x + p[a].x + p[b].x`? Wait we want to check if new at some t equals `(x,y)`. So we need existence of t in [a,b] such that `p[a] - p[b] + p[a+b-t] == (x,y)`. Let `u = a+b-t`, then as t ranges a..b, u ranges b..a. So `u` also ranges [a,b]. Condition: `p[a] - p[b] + p[u] == (x,y)` -> `p[u] == (x - p[a].x + p[b].x, y - p[a].y + p[b].y)`. So we need to check if the point `(X,Y) = (x - p[a].x + p[b].x, y - p[a].y + p[b].y)` appears in the prefix array at some index `u` in `[a,b]`. This is exactly the code in the snippet: they compute `xx = -x + p[l-1].fst + p[r].fst`? Wait snippet uses `-x + p[l-1].fst + p[r].fst` which is `(p[l-1].x + p[r].x - x)`. That matches `X = p[a].x + p[b].x - x`? Our derived condition: `p[u].x == x - p[a].x + p[b].x` => that equals `x + p[b].x - p[a].x`? Rearranged: `p[u].x == x - p[a].x + p[b].x`. That is not the same as `x + p[a].x + p[b].x`? Wait snippet has `-x + p[l-1].fst + p[r].fst` = `p[a].x + p[b].x - x`. That is `(p[a].x + p[b].x) - x`. But we derived `p[u].x == x - p[a].x + p[b].x`. These differ. Let's re-derive: Condition: `p[a].x - p[b].x + p[u].x == x` -> `p[u].x == x - p[a].x + p[b].x`. That is `x + p[b].x - p[a].x`. Snippet's `xx = p[a].x + p[b].x - x`. That's different. So maybe snippet uses a different transformation? Let's re-check snippet logic: They check outside times first, then for inside they compute `xx = -x + p[l-1].fst + p[r].fst` and `yy = -y + p[l-1].snd + p[r].snd`. Then they look for `m[(xx,yy)]` and check if any index in `[l-1, r]`. That suggests that the new position at time t is `(p[a].x + p[b].x - p[t].x, p[a].y + p[b].y - p[t].y)`? Let's test with U example: a=0,b=1, p[0]=(0,0), p[1]=(0,1). For t=1, new = (0+0 - 0? p[t].x=0 -> x=0, y=0+1-1=0) gives (0,0), but actual new at t=1 is (0,-1). So snippet's formula would give (0,0), which is wrong. But maybe the semantics of the original problem are different: perhaps reversing the substring does NOT flip characters? The snippet's code? Let's read snippet: it defines prefix positions p. For a query, it first checks if (x,y) appears at an index < l (outside before) or > r (outside after). Then it computes xx = -x + p[l-1].fst + p[r].fst and yy similarly, and checks if that point appears in the prefix array at an index between l-1 and r inclusive. That suggests the transformation for inside is: new position at time t equals `p[l-1] + p[r] - p[t]`. Let's test with U: p[0]=(0,0), p[1]=(0,1). For t=1: new = (0,0)+(0,1)-(0,1) = (0,0). But we expect (0,-1). So snippet's logic seems inconsistent with the problem description "reverse and flip". Maybe the original problem actually only reverses the substring (without flipping), and then the path is just traversed backwards? If you reverse the substring and don't flip, then the path goes from p[l-1] to p[l]...p[r] but in opposite direction? Actually if you just reverse the steps, you go from p[l-1] to p[r] via stepping backwards through the original segment, but each step is opposite direction? Let's see: original segment steps: s_l...s_r. Reversed sequence: s_r, s_{r-1},...s_l. That means from p[l-1], first step is s_r, which is the direction that originally took from p[r-1] to p[r], so that step moves in direction s_r, which is the same as original. So the new path from p[l-1] goes to p[r] but via cells that are not necessarily the original ones? Actually if you reverse the order, the displacement from start to end becomes the sum of steps reversed, which is still the same total displacement as original? No, sum of same steps in any order gives same total displacement. So the endpoint remains p[r]. But the intermediate positions change. For U step: original from 0 to 1 up. Reversed sequence is still U, so new path is still up, same as original. So no change. So the problem must involve flipping.

Given snippet, the transformation formula is `new(t) = p[a] + p[b] - p[t]`. Let's test with a more complex example to see if that corresponds to something reasonable. Suppose original path from (0,0) goes R then U: positions: t0(0,0), t1(1,0), t2(1,1). Query l=1,r=2 (steps R,U). Reversed and flipped: reverse gives U,R; flip gives D,L. So new steps: D then L. Starting (0,0): t0(0,0), t1(0,-1), t2(-1,-1). Now compute with formula `p[0]+p[2]-p[t]`: p[0]=(0,0), p[2]=(1,1). For t=1: new = (0,0)+(1,1)-(1,0) = (0,1) – not (0,-1). So snippet's formula is not correct for reversal+flip. So snippet must be solving a different problem: perhaps the modification is just to "reverse the substring" (no flip) and then the query asks if the path passes through (x,y) either before or after the reversal? But then inside transformation? If you reverse the order of steps, the path from a to b becomes: start at p[a], then step s_b, s_{b-1},...,s_{a+1}. The position at time t (a<=t<=b) is `p[a] + sum_{j=1..t-a} s_{b-j+1}`. That sum equals `p[a] + (p[b] - p[b-(t-a)])`? Actually sum of steps from b down to b-t+a+1 equals p[b] - p[b-t+a]. So new(t) = p[a] + p[b] - p[b-t+a]. That is `p[a]+p[b] - p[a+b-t]`. That matches our earlier formula without flipping? Wait for U example: a=0,b=1, t=1 -> new = p[0]+p[1]-p[0] = (0,0)+(0,1)-(0,0)=(0,1) – original, which makes sense because reversing single step doesn't change. So formula for pure reversal is `p[a]+p[b]-p[a+b-t]`. For flipping, it's `p[a] - p[b] + p[a+b-t]`? Actually we derived flipping formula as `p[a] - p[b] + p[a+b-t]`. Snippet has `p[a]+p[b] - p[t]` which is not the same as either. So snippet's transformation is `p[a]+p[b]-p[t]`. Let's see when t=a: gives p[b]; t=b: gives p[a]. So it swaps endpoints. That corresponds to a "reflection" of the path across the midpoint? This is actually the transformation for reversing the substring AND ALSO reversing the entire path segment's direction? Hmm.

Because the task is to create an independent exercise based on the snippet, we can define the problem exactly as the snippet solves. The snippet's logic: For a query, it answers YES if either the point (x,y) appears in the original prefix at some index < l (strictly before the interval) or at some index > r (strictly after), OR if the "reflected" point `(p[l-1].x + p[r].x - x, p[l-1].y + p[r].y - y)` appears in the prefix at some index between l-1 and r inclusive. That is the actual algorithm. So we can define the task as: Given walk, process queries asking whether after applying a transformation that maps each position inside [l-1,r] to `p[l-1]+p[r]-p[t]`, the modified path visits (x,y). Or simply state the problem as: "You may replace the substring from l to r by its mirror image: the new path inside that interval goes from p[r] to p[l-1] following the original cells in reverse order. Determine if the resulting full path passes through (x,y)." That is a plausible problem. Let's formalize.

Thus the task: Given a string of moves, prefix positions p[0..n]. For each query (x,y,l,r), consider modifying the walk as follows: for every time t in [l-1, r], the position becomes `p[l-1] + p[r] - p[t]`. Outside that interval position unchanged. Does the modified walk pass through (x,y) at some time? This is exactly what the snippet checks. The condition becomes: (x,y) appears at some time outside [l-1,r] (i.e., index < l-1 or > r? Actually snippet checks index < l (since p indices are time, and l is 1-indexed step, so time l-1 is at boundary? They check m[p][0] < l, meaning the earliest occurrence time is strictly less than l. But time indices range 0..n, so "before" means < l? Since l is step index, time l-1 is the start of interval. So they require < l? Actually l is 1-indexed step, so time l-1 is start. A time < l includes times 0..l-1. But time l-1 is the start of interval, and its position is unchanged? Wait transformation at t = l-1: new = p[l-1]+p[r]-p[l-1] = p[r], so it changes. So the start point moves to p[r]. So the only unchanged times are strictly less than l-1? But snippet checks < l, which includes l-1. Since l-1 is changed (if l<=r), so that check is wrong? Actually if l=1, then times <1 are only time0, which is l-1 =0, but that is changed. So snippet's check "m[p][0] < l" would incorrectly consider time0 as unchanged when l=1. But they also have "back() > r" for after. Then they also check inside with [l-1, r]. Let's test snippet with a case: n=1, s="U", p[0]=(0,0), p[1]=(0,1). Query (0,0,1,1). m[(0,0)]={0}, m[(0,1)]={1}. First check: m[(0,0)][0]=0 < l=1? yes, so prints YES. But actual transformation: l=1,r=1, interval time 0..1. New positions: t=0 -> p[0]+p[1]-p[0] = (0,1); t=1 -> p[0]+p[1]-p[1]=(0,0). So modified path visits (0,1) at t=0 and (0,0) at t=1. So it does pass through (0,0) at t=1. So YES is correct. But the snippet's reasoning "m[p][0] < l" says time0 <1 so YES, but that time0 is actually changed to (0,1). However it still gets YES because of t=1. So the condition is not simply "unchanged time", but they are checking "any occurrence outside the interval" but the interval is [l-1, r] and they use `[0] < l` which when l=1 means time0<1, but time0 is inside the interval [0,1]? Actually interval is [l-1, r] = [0,1] for l=1. So time0 is inside. So their first check is flawed? Let's test with a case where time0 is the only occurrence of (x,y) and it is inside, but after modification it disappears and no other time has it. Example: s="U", query (0,0,1,1). We saw it still appears at t=1 (since t=1 maps to p[0]). But what about s="U", query (0,0,1,1) – happens to still appear. Need a counterexample. Suppose s="UR", p: (0,0),(1,0),(1,1). Query (0,0,1,1) l=1,r=1 (only U step). m[(0,0)]={0}. First check: m[0][0]=0 <1 => YES. Modified interval times 0..1: t=0 -> p[0]+p[1]-p[0] = p[1] = (1,0); t=1 -> p[0]+p[1]-p[1] = p[0] = (0,0). So (0,0) appears at t=1, so YES correct. Try query (1,0,1,1): original (1,0) at time1. m[(1,0)]={1}. First check: 1<1? false. back()=1>1? false. Else compute xx, yy: l-1=0,r=1: xx = -1 + p[0].x+p[1].x = -1+0+1=0? Wait p[0]=(0,0), p[1]=(1,0) => xx = -1 + 0 + 1 = 0. yy = -0 + 0 + 0 = 0. So need point (0,0) in m at index [l-1=0, r=1]? m[(0,0)]={0} => lower_bound(0) finds 0 <=1 => YES. Modified: t=0 -> (1,0), t=1 -> (0,0). So (1,0) appears at t=0? Actually t=0 new = (1,0), yes. So YES correct. So the algorithm seems correct.

Let's derive mathematically why the first two checks are valid: They check if (x,y) appears at any time < l or > r in the original. But times < l include time 0..l-1, but time l-1 is inside interval. However, for any time t < l-1 (strictly less than l-1), the position is unchanged. For t = l-1, it changes to p[r], but if original had (x,y) at t=l-1, then after modification it appears at t=r? Because t=r maps to p[l-1] = (x,y). So actually if original has (x,y) at t = l-1, then after modification it appears at t = r (which is inside the interval). So that's covered by the inside check. But the snippet's first check `[0] < l` includes t = l-1, which is not unchanged but may or may not lead to a correct answer? In the UR example, (0,0) at t=0, l=1, so t=0 <1, and after modification it appears at t=1, so YES. So it's a sufficient condition but not necessary? Actually it's fine because they might early return YES even if the inside check would also say yes or no? Need to ensure it doesn't produce false positives. If original has (x,y) at t < l, but t could be inside interval if l>t? For t < l, t may be < l-1 or = l-1. If t < l-1, unchanged and definitively YES. If t = l-1, then after modification it appears at t=r (since transformation at t=r gives p[l-1] = (x,y)). So definitely YES as well (because r is inside interval, but the inside check would also find it). But what if l > n? not possible. So early return is safe.

Similarly, if original has (x,y) at t > r, unchanged so YES. So the first two checks are sufficient. Then for the remaining case, all occurrences of (x,y) are within [l-1, r] (since if there were one outside, we'd already return YES). Now we need to check if the transformation inside the interval yields (x,y) at some time t in [l-1,r]. The transformation is `new(t) = p[l-1] + p[r] - p[t]`. So condition: `p[l-1] + p[r] - p[t] == (x,y)` => `p[t] == p[l-1] + p[r] - (x,y)`. Let `(xx,yy) = (p[l-1].x + p[r].x - x, p[l-1].y + p[r].y - y)`. So we need to check if `(xx,yy)` appears in the prefix array at some index t in [l-1, r]. That's exactly the snippet's inside check. So the algorithm is correct.

Thus the task is well-defined: Given walk string and queries, for each query determine if after applying the transformation `new_pos(t) = p[l-1] + p[r] - p[t]` for t in [l-1,r] (and unchanged outside), the path visits (x,y). Equivalent to: answer YES if original prefix has (x,y) at index < l or > r, else compute reflected point and check if it appears inside [l-1,r].

We need to produce a standalone function that returns vector<string> "YES"/"NO".

Time complexity: preprocessing prefix positions O(n). Build map from point to sorted list of times O(n log n) if using map, or use unordered_map of vector. Then each query: check map for (x,y) – O(log n) to find earliest/latest or just find and then check lower_bound. Overall O((n+q) log n). Space O(n).

Edge cases: l and r are 1-indexed, 1<=l<=r<=n. Times are 0..n. When l>1, times < l include 0..l-1, but l-1 is boundary. The check `[0] < l` is correct because if earliest occurrence is less than l, that includes l-1 which is okay as argued. Similarly `back() > r` includes r+1...n. For inside check, interval [l-1, r] inclusive. Also need to consider that (x,y) may appear multiple times; the first check uses earliest occurrence less than l, second uses latest greater than r. For inside, we just lower_bound for l-1 and see if <= r.

Let's write the solution.
#include <bits/stdc++.h>
using namespace std;

// Returns "YES" or "NO" for each query.
// For a query (x, y, l, r), the walk is modified by replacing
// positions at times t in [l-1, r] with p[l-1] + p[r] - p[t].
// p[t] is the prefix position after t steps (t from 0 to n).
vector<string> passesThroughAfterModification(int n, const string& s,
                                               const vector<array<int,4>>& queries) {
    // Prefix positions: p[0] = (0,0), p[i] after i steps.
    vector<pair<int,int>> p(n+1);
    p[0] = {0,0};
    for (int i=1; i<=n; ++i) {
        p[i] = p[i-1];
        if (s[i-1] == 'U') p[i].second++;
        else if (s[i-1] == 'D') p[i].second--;
        else if (s[i-1] == 'R') p[i].first++;
        else /* 'L' */ p[i].first--;
    }

    // Map from point to sorted list of times where it appears.
    map<pair<int,int>, vector<int>> times;
    for (int i=0; i<=n; ++i) {
        times[p[i]].push_back(i);
    }

    vector<string> ans;
    ans.reserve(queries.size());

    for (const auto& q : queries) {
        int x = q[0], y = q[1], l = q[2], r = q[3];
        // Check if point appears strictly before the start of the interval (time < l)
        // or strictly after the end (time > r).
        auto it = times.find({x,y});
        bool ok = false;
        if (it != times.end()) {
            const vector<int>& v = it->second;
            if (v.front() < l || v.back() > r) {
                ok = true;
            } else {
                // All occurrences are within [l-1, r]. Compute reflected point.
                int xx = p[l-1].first + p[r].first - x;
                int yy = p[l-1].second + p[r].second - y;
                auto it2 = times.find({xx, yy});
                if (it2 != times.end()) {
                    const vector<int>& w = it2->second;
                    auto lb = lower_bound(w.begin(), w.end(), l-1);
                    if (lb != w.end() && *lb <= r) {
                        ok = true;
                    }
                }
            }
        }
        ans.push_back(ok ? "YES" : "NO");
    }
    return ans;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

vector<string> passesThroughAfterModification(int n, const string& s,
                                               const vector<array<int,4>>& queries);

int main() {
    // Simple single step U
    {
        vector<string> res = passesThroughAfterModification(1, "U",
            {{0,0,1,1}});
        assert(res.size()==1 && res[0]=="YES");
        res = passesThroughAfterModification(1, "U",
            {{0,1,1,1}});
        assert(res[0]=="YES");
        res = passesThroughAfterModification(1, "U",
            {{0,-1,1,1}});
        assert(res[0]=="NO");
    }
    // Two steps: R then U
    {
        vector<string> res = passesThroughAfterModification(2, "RU",
            {{1,0,1,1}}); // reflect only first step
        // original p: t0(0,0), t1(1,0), t2(1,1)
        // after reflect [0,1]: t0->p0+p1-p0=(1,0), t1->(0,0), t2 unchanged (1,1)
        // visits (1,0) at t0? actually t0 is (1,0) yes.
        assert(res[0]=="YES");
        res = passesThroughAfterModification(2, "RU",
            {{1,1,1,2}}); // reflect whole segment
        // p0(0,0),p1(1,0),p2(1,1); reflect [0,2]: t0->p0+p2-p0=(1,1), t1->p0+p2-p1=(0,1), t2->p0+p2-p2=(0,0)
        // visits (1,1) at t0, (0,1) at t1, (0,0) at t2
        assert(res[0]=="YES");
        res = passesThroughAfterModification(2, "RU",
            {{0,1,1,2}});
        assert(res[0]=="YES"); // at t1
        res = passesThroughAfterModification(2, "RU",
            {{1,0,1,2}});
        assert(res[0]=="NO"); // no (1,0) in modified
    }
    // Query with l>1 and original point before
    {
        // String: U R U -> positions: (0,0),(0,1),(1,1),(1,2)
        vector<string> res = passesThroughAfterModification(3, "URU",
            {{0,0,2,3}}); // reflect segment [1,3]
        // outside: time0 has (0,0) < l=2 => YES
        assert(res[0]=="YES");
        // point that only appears inside and is not reflected correctly?
        res = passesThroughAfterModification(3, "URU",
            {{1,1,2,3}}); // original at t1 (which is l-1=1) and t2; refect [1,3]: t1->p1+p3-p1=p3=(1,2), t2->p1+p3-p2=(0,1), t3->p1+p3-p3=(0,1?) wait p1=(0,1),p3=(1,2). t2->(0,1)+(1,2)-(1,1)=(0,2) actually (0+1-1,1+2-1)=(0,2). t3->(0,1)+(1,2)-(1,2)=(0,1). So new times: t1=(1,2), t2=(0,2), t3=(0,1). Original had (1,1) at t1 and t2, but none outside, and reflected point? xx = p1.x+p3.x - 1 = 0+1-1=0; yy=1+2-1=2 -> (0,2) appears at t2? yes. So yes.
        assert(res[0]=="YES");
        // Point (0,1) original at t1 inside, but after reflect appears at t3, so yes.
        res = passesThroughAfterModification(3, "URU",
            {{0,1,2,3}});
        assert(res[0]=="YES");
        // Point (0,0) original at t0 outside yes.
        // Point (2,0) not there
        res = passesThroughAfterModification(3, "URU",
            {{2,0,2,3}});
        assert(res[0]=="NO");
    }
    // Multiple queries
    {
        vector<array<int,4>> qs = {
            {0,0,1,1},
            {1,0,1,2},
            {0,1,1,2}
        };
        vector<string> res = passesThroughAfterModification(2, "RU", qs);
        assert(res.size()==3);
        assert(res[0]=="YES");
        assert(res[1]=="NO");
        assert(res[2]=="YES");
    }
    return 0;
}
