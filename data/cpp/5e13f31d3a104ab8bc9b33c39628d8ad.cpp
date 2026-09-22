// Write a C++ function `longestGoodSegment(const std::vector<int>& values, int left, int right)` that takes a 1-indexed array of integers (represented as a 0-indexed vector) and two 1-indexed positions `left` and `right` (inclusive), and returns the length of the longest contiguous subsegment within `[left, right]` that does **not** contain any triple of consecutive elements that are in non-increasing order (i.e., there is no index `i` such that `values[i] >= values[i+1] >= values[i+2]` within that subsegment). The function should handle the case where the segment length is less than 3 (then the whole segment is trivially valid). The input vector is fixed and you can precompute any auxiliary data inside the function. The function must return an integer (the maximum valid subsegment length) for the given query range. The array may contain duplicate values, and negative numbers, and the range `[left, right]` is guaranteed to be within `1 ≤ left ≤ right ≤ n`, where `n` is the size of the vector.
// The key observation is that a "bad" triple is any three consecutive positions where the sequence is non-increasing. The problem becomes finding the longest subarray inside `[left, right]` that avoids such triples. Since the entire original array is fixed, we can precompute a prefix array `pref` where `pref[i]` counts the number of bad triples whose **starting index** is strictly less than `i` (i.e., triples starting at positions `0` to `i-1`). For each index `i` from `0` to `n-3`, if `values[i] >= values[i+1] && values[i+1] >= values[i+2]`, then there is a bad triple starting at `i`. We store `pref[i+1] = pref[i] + 1` in that case, otherwise `pref[i+1] = pref[i]`. For indices `i` beyond `n-3`, `pref[i+1] = pref[i]`.
//
// For a query `[left, right]` (1-indexed), the number of bad triples that lie entirely inside the segment is the count of bad triple starting indices `s` such that `left ≤ s ≤ right-2`. In prefix terms, bad triple start indices `< right-1` (because we count starts up to `right-2`, so the count of starts `< right-1` minus the count of starts `< left-1`). Since `pref[x]` counts bad starts `< x`, the number of bad triples inside is `pref[right-1] - pref[left-1]`. However, to find the longest valid subsegment, we need to remove the minimal number of elements to break all bad triples. Each bad triple must have at least one element removed; the minimal removal to break all overlapping bad triples is exactly the number of such triples (since each triple is independent? Not exactly—overlapping triples share elements, but the optimal way is to remove the middle element of each triple? Actually, the snippet computes `rem = pref[r-2] - pref[l-1]` and then returns `(r - l + 1) - rem`. Let's analyze: For a segment of length `len`, the maximum valid subsegment length is `len - k`, where `k` is the minimum number of elements to delete to eliminate all bad triples. For a sequence of consecutive bad triples (like a strictly decreasing run of length 4), removing one element (e.g., the second) breaks all overlapping triples. But the snippet's formula uses `pref[r-2] - pref[l-1]` which counts bad triples starting at positions from `l-1` to `r-3` (0-indexed start). That count equals the number of bad triples whose **start index** is within `[l-1, r-3]`. In a strictly decreasing run of length `m`, there are `m-2` bad triples. To eliminate all of them, you need to remove `m-2` elements? Actually no—removing every second element? Let's test: decreasing `[5,4,3,2]` has triples (5,4,3) and (4,3,2) at starts 0 and 1. Remove index 1 (value 4) and the remaining `[5,3,2]` has triple (5,3,2) which is not non-increasing because 5>=3 but 3>=2? 5>=3 true, 3>=2 true, so still bad. Remove index 2 (value 3) leaves `[5,4,2]` which is also bad. Remove index 1 and 2? That's 2 removals. But the count of triples is 2, so removing 2 elements works. In general, for a decreasing run of length `m`, the minimal removals is `m-2` (remove all but two endpoints). That equals the number of triples in that run. So the total number of bad triples across disjoint runs gives the exact number of removals needed, because triples from different runs don't overlap, and within a run, each triple requires one unique removal (the middle element of each triple, but careful: overlapping triples share middle elements? For consecutive triples, each triple's middle element is distinct? In a decreasing run, triple0 middle is index1, triple1 middle is index2, etc. Removing all middle elements (indices 1,2,...,m-2) removes `m-2` elements and eliminates all triples. So yes, removal count equals the number of bad triples. Thus the maximum valid subsegment length is original length minus the count of bad triples entirely inside the segment. Since any bad triple must be fully inside, and we remove exactly one element per triple, the result is correct. Edge cases: if segment length < 3, no triples exist, so the entire segment is valid. For a segment of length L ≥ 3, we compute `rem = number of bad triples with start index between left-1 and right-3` (0-indexed start). That is `pref[right-2] - pref[left-1]` because `pref[x]` counts starts `< x`, so starts up to `right-3` are `< right-2`. The final answer is `L - rem`. Time complexity: O(n) preprocessing + O(1) per query (but the task only asks for one query, still O(n) overall). Space: O(n) for prefix array. The function must be self-contained and not rely on global state.
#include <vector>

// Returns the length of the longest contiguous subsegment within [left, right] (1-indexed)
// that does not contain any non-increasing triple of consecutive elements.
// left and right are 1-indexed positions in the original array.
int longestGoodSegment(const std::vector<int>& values, int left, int right) {
    int n = static_cast<int>(values.size());
    // Prefix count of bad triples: pref[i] = number of bad triples whose starting index < i.
    std::vector<int> pref(n + 1, 0);
    for (int i = 0; i + 2 < n; ++i) {
        pref[i + 1] = pref[i];
        if (values[i] >= values[i + 1] && values[i + 1] >= values[i + 2]) {
            ++pref[i + 1];
        }
    }
    // Fill remaining positions after the last possible triple.
    for (int i = std::max(0, n - 2); i < n; ++i) {
        pref[i + 1] = pref[i];
    }

    int segmentLength = right - left + 1;
    if (segmentLength < 3) {
        return segmentLength;
    }

    // Number of bad triples entirely inside [left, right]:
    // bad triple starts at s (0-indexed) where left-1 <= s <= right-3.
    int badTriples = pref[right - 2] - pref[left - 1];
    return segmentLength - badTriples;
}
#include <cassert>
#include <vector>

int longestGoodSegment(const std::vector<int>& values, int left, int right);

int main() {
    // No bad triples in the whole array
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(longestGoodSegment(v1, 1, 5) == 5);
    assert(longestGoodSegment(v1, 2, 4) == 3);

    // Single bad triple in the middle
    std::vector<int> v2 = {1, 5, 4, 3, 2, 6};
    // Indices (0-based): 1,2,3 forms bad triple (5>=4>=3)
    // Query [2,5] (1-indexed) => values 5,4,3,2 => length 4, one bad triple => answer 3
    assert(longestGoodSegment(v2, 2, 5) == 3);
    // Query [1,4] => values 1,5,4,3 => has triple (5,4,3) => length 4, answer 3
    assert(longestGoodSegment(v2, 1, 4) == 3);
    // Query [3,5] => values 4,3,2 => one triple => length 3, answer 2
    assert(longestGoodSegment(v2, 3, 5) == 2);
    // Query [4,6] => values 3,2,6 => no bad triple => length 3
    assert(longestGoodSegment(v2, 4, 6) == 3);

    // Consecutive decreasing run of length 4
    std::vector<int> v3 = {10, 9, 8, 7, 100};
    // Query [1,4] => decreasing run length 4 => bad triples: starts 0,1 => 2 triples => answer 4-2=2
    assert(longestGoodSegment(v3, 1, 4) == 2);
    // Query [2,5] => values 9,8,7,100 => one bad triple (9,8,7) => answer 4-1=3
    assert(longestGoodSegment(v3, 2, 5) == 3);

    // Duplicate values
    std::vector<int> v4 = {5, 5, 5, 1, 2};
    // Query [1,3] => all equal => bad triple at start 0 => length 3, answer 2
    assert(longestGoodSegment(v4, 1, 3) == 2);
    // Query [3,5] => values 5,1,2 => no bad triple => length 3
    assert(longestGoodSegment(v4, 3, 5) == 3);

    // Short segment less than 3
    std::vector<int> v5 = {7, 2, 9, 1, 0};
    assert(longestGoodSegment(v5, 2, 2) == 1);
    assert(longestGoodSegment(v5, 3, 4) == 2);

    // Entire array no bad triples except at end
    std::vector<int> v6 = {3, 1, 2, 4, 0, -1};
    // Query [4,6] => values 4,0,-1 => one bad triple => length 3, answer 2
    assert(longestGoodSegment(v6, 4, 6) == 2);
    // Query [1,5] => all except last => no bad triples? Check: 3>=1? false; 1>=2? false; 2>=4? false; 4>=0? true but 0>=? no next. So no bad triple => length 5
    assert(longestGoodSegment(v6, 1, 5) == 5);

    return 0;
}
