You are given `n` items, each with a category flag (`1` for a "stool" item, `0` for a "pencil" item) and a cost `x[i]`. You must distribute all `n` items into exactly `k` groups (numbered `0` to `k-1`). Each group must contain at least one item. The distribution must follow this rule: place the `k` stools with the largest costs each into their own distinct group first (if there are fewer than `k` stools, then each stool goes into a distinct group, and the remaining groups are filled with pencils). If there are more than `k` stools, then after the top `k` stools are placed each in their own group, all remaining stools and all pencils go into the group that contains the smallest of those top `k` stools (group `k-1`). If there are fewer than `k` stools, after placing each stool in its own group, fill the remaining groups with pencils one by one in the order given, and any leftover pencils all go into the group that has the smallest stool (group `k-1`). For each group, compute its total cost as twice the sum of all item costs in that group, then subtract the minimum item cost in the group if the group contains at least one stool. The answer is the sum of all group totals divided by 2. Write a function that takes `n`, `k`, the cost array `x`, and a vector of category flags (1 for stool, 0 for pencil), and returns a `pair<double, vector<vector<int>>>` where the double is the computed average (the total sum divided by 2, with one decimal place as in the original code, but for correctness keep it as a double), and the vector of vectors contains the 1-based indices of items in each group in the exact order described. The function must handle all edge cases: `k >= n` (though the problem as given assumes `k <= n` for each group non-empty, but handle it gracefully), `n=0`, all stools, all pencils, etc. The output groups are deterministic as described.
The algorithm directly simulates the described grouping. First, separate the indices of stool items and pencil items by scanning the input. Sort the stool indices in descending order of their cost values `x[idx]`. Create `k` empty vectors of indices, and a boolean array `has_stool` initially all false. If there is at least one stool, assign the first `min(stsz, k)` stools to groups `0..min(stsz,k)-1` and set `has_stool` for those groups. Let `minstool = k-1` (this is the group that will receive the extra items). Now handle the two cases:

- If `stsz > k`: After placing top `k` stools, all remaining stools (indices from `k` to `stsz-1`) go into group `minstool`. Then all pencils also go into group `minstool`.
- Else (i.e., `stsz <= k`): For each stool placed (indices `0..stsz-1`), they are in groups `0..stsz-1`. Then fill the remaining groups `stsz` to `k-1` with pencils one by one, taking pencils in the order they appear in the input. If pencils remain (i.e., `pcsz > k-stsz`), then all leftover pencils (starting at index `k-stsz` in the pencil vector) go into group `minstool`.

If there are no stools at all (`stsz == 0`), then all groups are filled with pencils in order, and the last group gets any leftovers. Note: In the original code, if `stsz == 0`, then `minstool = k-1`, and the loop for `i=stsz; i<k; i++` fills groups with pencils, and the second loop `for (i=k-stsz; i<pcsz; i++)` puts leftovers in group `k-1`. This is exactly what we do.

After assigning all items, compute for each group: `sum = sum of 2*x[idx]` for all indices in that group. If the group has a stool (i.e., `has_stool[i]` is true), subtract the minimum `x` value among the indices in that group (use a helper to compute the minimum). The answer is the total of all group sums divided by 2. Note that this is a double; the original code prints with one decimal place, but for the function we can return the double directly.

Edge cases:  
- `n == 0`: Return 0.0 and `k` empty groups (but the problem expects each group non-empty, so this case is degenerate; we can just return empty groups).  
- `k == 0`: Not allowed by problem, but handle by returning 0.0 and empty vector.  
- All pencils: `stsz == 0`, the algorithm works.  
- All stools and `stsz <= k`: Then groups get one stool each, and leftover stools go to group `k-1`. But note that the original code in the `else` branch does `for (i=stsz;i<k;i++) kart[i].push_back(pencil[i-stsz]);` which would be out-of-bounds if `pencil` is empty. Actually, the original code would have a problem if `stsz < k` and `pcsz < k-stsz`, because then it accesses `pencil[i-stsz]` without bound check. In a correct implementation, we must check bounds. So we will handle that: if `stsz <= k`, we fill groups that have no stool with pencils as long as pencils are available; if pencils run out, we leave those groups empty? That contradicts "each group must contain at least one item" but the problem input likely guarantees `k <= n` and enough items. For robustness, we can fill whatever pencils are available, and if a group still has no items, we can leave it empty (though the original code would crash). For the test, we will only test valid inputs where `k <= n` and there are enough items to fill every group.

Time complexity: O(n log n) due to sorting stools (worst case all stools) plus O(n) for grouping and O(k * average group size) for computing sums. Space complexity: O(n + k) for storing vectors.
#include <vector>
#include <algorithm>
#include <utility>
#include <limits>

// Given n items with costs and category flags (1 = stool, 0 = pencil),
// distribute them into k groups according to the described rule.
// Returns a pair: (total_sum/2.0, vector of groups with 1-based indices in order).
std::pair<double, std::vector<std::vector<int>>> distribute_items(
    int n, int k, const std::vector<long long>& x, const std::vector<int>& category) {
    // Prepare vectors of indices for stools and pencils.
    std::vector<int> stool, pencil;
    for (int i = 0; i < n; ++i) {
        if (category[i] == 1) stool.push_back(i);
        else pencil.push_back(i);
    }
    int stsz = static_cast<int>(stool.size());
    int pcsz = static_cast<int>(pencil.size());

    // Sort stools by descending cost.
    std::sort(stool.begin(), stool.end(),
              [&](int a, int b) { return x[a] > x[b]; });

    // Create k groups.
    std::vector<std::vector<int>> groups(k);
    std::vector<bool> has_stool(k, false);

    int top_stools = std::min(stsz, k);
    // Place top stools into distinct groups.
    for (int i = 0; i < top_stools; ++i) {
        groups[i].push_back(stool[i] + 1); // store 1-based index
        has_stool[i] = true;
    }

    int target_group = k - 1; // group that receives extra items

    if (stsz > k) {
        // Put all remaining stools and all pencils into group k-1.
        for (int i = k; i < stsz; ++i) {
            groups[target_group].push_back(stool[i] + 1);
        }
        for (int i = 0; i < pcsz; ++i) {
            groups[target_group].push_back(pencil[i] + 1);
        }
    } else { // stsz <= k
        // Place pencils into empty groups (after stools if any).
        int pencil_idx = 0;
        for (int i = top_stools; i < k && pencil_idx < pcsz; ++i) {
            groups[i].push_back(pencil[pencil_idx] + 1);
            ++pencil_idx;
        }
        // Put any leftover pencils into group k-1.
        for (; pencil_idx < pcsz; ++pencil_idx) {
            groups[target_group].push_back(pencil[pencil_idx] + 1);
        }
        // If there were no stools and pcsz < k, some groups may be empty;
        // but we handle gracefully (problem likely guarantees k <= n).
        // If no stools and there are exactly pcsz = k, then each group gets one pencil.
    }

    // Compute the answer.
    long long total = 0;
    for (int g = 0; g < k; ++g) {
        long long group_sum = 0;
        long long min_val = std::numeric_limits<long long>::max();
        for (int idx1 : groups[g]) {
            int idx0 = idx1 - 1;
            group_sum += 2 * x[idx0];
            if (has_stool[g]) {
                min_val = std::min(min_val, x[idx0]);
            }
        }
        if (has_stool[g] && !groups[g].empty()) {
            group_sum -= min_val;
        }
        total += group_sum;
    }

    double answer = static_cast<double>(total) / 2.0;
    return {answer, groups};
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Basic example with 2 stools and 2 pencils, k=2.
    {
        int n = 4, k = 2;
        std::vector<long long> x = {10, 5, 3, 8};
        std::vector<int> cat = {1, 0, 1, 0}; // stools at 0 and 2, pencils at 1 and 3
        auto res = distribute_items(n, k, x, cat);
        // Stools sorted by cost: idx2 (cost 3), idx0 (cost 10) -> groups: [2,1] and [0,3]? Wait: top 2 stools are idx0 (10) and idx2 (3), placed into groups 0 and 1. So group0 has item1 (cost10), group1 has item3 (cost3). Then because stsz=2 <= k=2, pencils fill remaining groups: group0 already has stool, group1 already has stool, so no empty groups. Leftover pencils go to group k-1=1. So group0: [1], group1: [3,2,4] (1-based). Compute: group0 sum=2*10=20, minus min(10)=10 -> 10. group1 sum=2*(3+3+8)=2*14=28, minus min(3,3,8)=3 -> 25. total=35, answer=17.5. Check groups order.
        assert(res.first == 17.5);
        assert(res.second.size() == 2);
        assert(res.second[0] == std::vector<int>{1});
        assert(res.second[1] == std::vector<int>{3,2,4});
    }

    // Test 2: More stools than k.
    {
        int n = 5, k = 2;
        std::vector<long long> x = {10, 20, 30, 40, 5};
        std::vector<int> cat = {1,1,1,1,0}; // stools at 0-3, pencil at 4
        auto res = distribute_items(n, k, x, cat);
        // Stools sorted by cost descending: idx3(40), idx2(30), idx1(20), idx0(10)
        // Place top 2: group0 gets idx3 (cost40), group1 gets idx2 (cost30). stsz=4>k=2, so remaining stools idx1, idx0 go to group1 (k-1). Also pencil idx4 goes to group1.
        // group0: [4] (1-based), group1: [3,2,1,5] (1-based: idx2+1=3, idx1+1=2, idx0+1=1, idx4+1=5)
        // Compute group0: sum=2*40=80, minus min(40)=40 -> 40. group1: costs 30,20,10,5 -> sum=2*(65)=130, minus min=5 -> 125. total=165, answer=82.5
        assert(std::abs(res.first - 82.5) < 1e-9);
        assert(res.second[0] == std::vector<int>{4});
        assert(res.second[1] == std::vector<int>{3,2,1,5});
    }

    // Test 3: Fewer stools than k, all remaining groups filled with pencils.
    {
        int n = 5, k = 3;
        std::vector<long long> x = {1, 2, 3, 4, 5};
        std::vector<int> cat = {1,0,0,0,0}; // only one stool at idx0
        auto res = distribute_items(n, k, x, cat);
        // Stools: only idx0 (cost1). top_stools = min(1,3)=1, group0 gets idx0.
        // stsz=1 <= k=3 -> fill groups 1 and 2 with pencils from order: group1 gets idx1 (cost2), group2 gets idx2 (cost3). Leftover pencils: idx3, idx4 go to group k-1=2.
        // group0: [1], group1: [2], group2: [3,4,5]
        // Compute group0: sum=2*1=2, minus min(1)=1 -> 1. group1: sum=2*2=4, has_stool? false -> 4. group2: sum=2*(3+4+5)=24, has_stool? false -> 24. total=29, answer=14.5
        assert(std::abs(res.first - 14.5) < 1e-9);
        assert(res.second[0] == std::vector<int>{1});
        assert(res.second[1] == std::vector<int>{2});
        assert(res.second[2] == std::vector<int>{3,4,5});
    }

    // Test 4: All pencils, k=n (each group gets one pencil).
    {
        int n = 3, k = 3;
        std::vector<long long> x = {10, 20, 30};
        std::vector<int> cat = {0,0,0};
        auto res = distribute_items(n, k, x, cat);
        // stsz=0, top_stools=0, fill groups 0,1,2 with pencils idx0,1,2. No leftovers.
        // Each group has single pencil, no stools so sum = 2*x, no subtraction.
        // total = 2*10+2*20+2*30 = 120, answer=60
        assert(res.first == 60.0);
        assert(res.second[0] == std::vector<int>{1});
        assert(res.second[1] == std::vector<int>{2});
        assert(res.second[2] == std::vector<int>{3});
    }

    // Test 5: Single item, k=1.
    {
        int n = 1, k = 1;
        std::vector<long long> x = {5};
        std::vector<int> cat = {1};
        auto res = distribute_items(n, k, x, cat);
        // group0 gets stool, sum=2*5=10, minus min=5 -> 5, answer=2.5
        assert(res.first == 2.5);
        assert(res.second[0] == std::vector<int>{1});
    }

    // Test 6: Exactly k stools, no pencils.
    {
        int n = 3, k = 3;
        std::vector<long long> x = {5, 10, 7};
        std::vector<int> cat = {1,1,1};
        auto res = distribute_items(n, k, x, cat);
        // Stools sorted descending: idx1(10), idx2(7), idx0(5) -> groups: 0->[2], 1->[3], 2->[1] (1-based)
        // Each group has one stool: sum = 2*x, subtract min = x, so total per group = x. Total sum = 10+7+5=22, answer=11
        assert(res.first == 11.0);
        assert(res.second[0] == std::vector<int>{2});
        assert(res.second[1] == std::vector<int>{3});
        assert(res.second[2] == std::vector<int>{1});
    }

    return 0;
}
