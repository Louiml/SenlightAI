Write a C++ function `constructTwoPermutations` that takes a vector of integers `a` of length `n` (with values in `[1, n]`) and attempts to construct two permutations `p1` and `p2` of `{1, 2, ..., n}` such that for every position `i`, `a[i]` equals either `p1[i]` or `p2[i]` (i.e., `a[i]` must appear at least once in the same index across the two permutations). If such two permutations exist, the function should return `true` and fill the vectors `p1` and `p2` with valid permutations (both permutations must contain each number from `1` to `n` exactly once, and for each index `i`, `a[i]` must be present in `p1[i]` or `p2[i]`, possibly both). If no such pair exists, return `false` and leave both vectors empty. The input vector `a` may contain duplicates, but any value appearing more than twice in `a` makes the task impossible. Also, if the value `1` appears more than once, it is impossible. A value equal to its index is always assignable, but values smaller than their position cause impossibility. Implement a greedy algorithm: first assign each value to one of the two permutations (distributing duplicates across different permutations), then fill the remaining positions with the largest unused numbers to satisfy permutation constraints. Ensure the function correctly detects impossible cases and constructs valid outputs when possible.
#include <cassert>
#include <vector>

// The solution function is declared before main, so we include it here.
// (Actually we would #include the file, but for this standalone test we assume it's in scope.)

int main() {
    // Test 1: Example from typical usage
    std::vector<int> a1 = {2, 1, 3};
    std::vector<int> p1, p2;
    assert(constructTwoPermutations(a1, p1, p2) == true);
    assert(p1.size() == 3 && p2.size() == 3);
    // Verify each is a permutation and matches condition
    assert(p1[0] == 2 || p2[0] == 2);
    assert(p1[1] == 1 || p2[1] == 1);
    assert(p1[2] == 3 || p2[2] == 3);

    // Test 2: Duplicate value twice is okay
    std::vector<int> a2 = {2, 2, 1};
    assert(constructTwoPermutations(a2, p1, p2) == true);
    assert(p1[0] == 2 || p2[0] == 2);
    assert(p1[1] == 2 || p2[1] == 2);

    // Test 3: Value appears three times -> impossible
    std::vector<int> a3 = {2, 2, 2};
    assert(constructTwoPermutations(a3, p1, p2) == false);
    assert(p1.empty() && p2.empty());

    // Test 4: 1 appears twice -> impossible
    std::vector<int> a4 = {1, 1, 2};
    assert(constructTwoPermutations(a4, p1, p2) == false);

    // Test 5: Small value too early -> impossible
    std::vector<int> a5 = {1, 1, 2};  // already covered
    // This case: a = [1, 3, 2] is valid
    std::vector<int> a5b = {1, 3, 2};
    assert(constructTwoPermutations(a5b, p1, p2) == true);
    // a = [2, 3, 1] -> 1 at position 3, value 1 < 3? Actually check: sorted values [1,2,3], indices [2,0,1], at i=0 value=1 >=1, i=1 value=2>=2, i=2 value=3>=3 -> valid
    std::vector<int> a5c = {2, 3, 1};
    assert(constructTwoPermutations(a5c, p1, p2) == true);

    // Test 6: Large n simple valid case
    std::vector<int> a6 = {1, 2, 3, 4, 5};
    assert(constructTwoPermutations(a6, p1, p2) == true);
    // Check each is a permutation
    std::vector<bool> seen(6, false);
    for (int x : p1) { assert(x >= 1 && x <= 5); assert(!seen[x]); seen[x] = true; }
    std::fill(seen.begin(), seen.end(), false);
    for (int x : p2) { assert(x >= 1 && x <= 5); assert(!seen[x]); seen[x] = true; }

    // Test 7: Case where we need to fill with largest numbers
    std::vector<int> a7 = {3, 3, 1, 2};
    assert(constructTwoPermutations(a7, p1, p2) == true);
    // Verify a7[0]=3 appears in p1 or p2 at index 0, etc.

    // Test 8: Impossible due to position constraint (5,5,1,2,3) - check sorted: values [1,2,3,5,5] indices sorted: [2,3,4,0,1], at i=0 val=1>=1, i=1 val=2>=2, i=2 val=3>=3, i=3 val=5>=4, i=4 val=5>=5 -> valid? but duplicates: 5 twice, 1 once, 2 once, 3 once, okay. Actually valid.
    std::vector<int> a8 = {5, 5, 1, 2, 3};
    assert(constructTwoPermutations(a8, p1, p2) == true);

    // Test 9: Impossible: a=[2,1] - sorted [1,2], indices [1,0], i=0 val=1>=1, i=1 val=2>=2, valid. 1 once, 2 once, okay.
    std::vector<int> a9 = {2, 1};
    assert(constructTwoPermutations(a9, p1, p2) == true);
    // p1 = [2,1], p2 = [1,2] or similar.

    // Test 10: Impossible case: a=[1,2,2,4] -> 2 appears twice, okay; check sorted: [1,2,2,4], indices [0,1,2,3], i=0 val=1>=1, i=1 val=2>=2, i=2 val=2>=3? No, 2 < 3 -> impossible
    std::vector<int> a10 = {1, 2, 2, 4};
    assert(constructTwoPermutations(a10, p1, p2) == false);

    return 0;
}
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>

// Attempt to construct two permutations p1 and p2 from the given array a.
// Returns true on success, false otherwise. On success, p1 and p2 are filled.
bool constructTwoPermutations(const std::vector<int>& a,
                              std::vector<int>& p1,
                              std::vector<int>& p2) {
    int n = static_cast<int>(a.size());
    p1.clear();
    p2.clear();
    
    // Frequency check and feasibility checks
    std::unordered_map<int, int> freq;
    for (int x : a) {
        freq[x]++;
        if (freq[x] > 2) return false;
    }
    if (freq[1] > 1) return false;
    
    // Check that at each sorted position value >= index
    std::vector<int> sorted_idx(n);
    for (int i = 0; i < n; ++i) sorted_idx[i] = i;
    std::sort(sorted_idx.begin(), sorted_idx.end(),
              [&](int i, int j) { return a[i] < a[j]; });
    for (int i = 0; i < n; ++i) {
        if (a[sorted_idx[i]] < i + 1) return false;
    }
    
    // Initialize permutations with -1 (unassigned)
    p1.assign(n, -1);
    p2.assign(n, -1);
    
    // Sets of unused numbers (1..n)
    std::set<int> unused1, unused2;
    for (int v = 1; v <= n; ++v) {
        unused1.insert(v);
        unused2.insert(v);
    }
    
    // Assign given values to p1 first, then p2 for duplicates
    std::unordered_map<int, bool> used1, used2;
    for (int i = n - 1; i >= 0; --i) {
        int idx = sorted_idx[i];
        int val = a[idx];
        if (used1[val]) {
            p2[idx] = val;
            used2[val] = true;
            unused2.erase(val);
        } else {
            p1[idx] = val;
            used1[val] = true;
            unused1.erase(val);
        }
    }
    
    // Fill remaining gaps in p1 with largest unused numbers
    for (int i = n - 1; i >= 0; --i) {
        int idx = sorted_idx[i];
        if (p1[idx] == -1) {
            int largest = *unused1.rbegin();
            p1[idx] = largest;
            unused1.erase(largest);
        } else {
            int largest = *unused2.rbegin();
            p2[idx] = largest;
            unused2.erase(largest);
        }
    }
    
    // Final verification (optional, but ensures correctness)
    std::vector<bool> seen1(n + 1, false), seen2(n + 1, false);
    for (int i = 0; i < n; ++i) {
        if (p1[i] < 1 || p1[i] > n || seen1[p1[i]]) return false;
        seen1[p1[i]] = true;
        if (p2[i] < 1 || p2[i] > n || seen2[p2[i]]) return false;
        seen2[p2[i]] = true;
        if (a[i] != p1[i] && a[i] != p2[i]) return false;
    }
    for (int v = 1; v <= n; ++v) {
        if (!seen1[v] || !seen2[v]) return false;
    }
    
    return true;
}
// The problem reduces to checking feasibility and then constructing two permutations. Observations: each value `x` in `a` must appear in at least one of the two permutations at its given position. Since each permutation is a permutation of `1..n`, each value `x` can appear at most twice in `a` (one in each permutation) and at most once in each permutation. Thus, any value with frequency >2 makes it impossible. Also, value `1` has only one possible position in any permutation (as the smallest number cannot be placed after a smaller index because positions start at 1, so `1` must appear at index 1; if `1` appears at two different indices, it's impossible). Additionally, for a sorted order of values, if at some position `i` (1-indexed) the value is less than `i+1`, that means too many small numbers have appeared earlier, making it impossible to place them in later positions. The main algorithm: sort indices by their `a` values. Traverse from largest to smallest. For each value, if it hasn't been assigned to `p1`, assign it to `p1`; if it's already used in `p1` (duplicate), assign it to `p2`. This ensures every value that appears once goes to `p1`, and duplicates go to `p2`. After that, there will be gaps (positions where `p1` or `p2` has -1). Fill each gap in `p1` with the largest unused number from the set of numbers not yet placed in `p1`, and similarly for `p2`. Because we process in descending order of values and assign the largest possible to the first open spot, this is guaranteed to work if the feasibility checks pass. Edge cases: when a value appears twice, ensure it appears at different indices (already by position). The time complexity is O(n log n) due to sorting and using sets/maps, and space is O(n).
