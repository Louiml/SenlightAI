// Given two integers `n` and `k`, construct a permutation of the numbers `1` through `n` that is the `k`-th lexicographically smallest permutation when considering only the number of inversions. Specifically, the permutation must have exactly `k-1` permutations (in lexicographic order) that are lexicographically smaller but also have the same total number of inversions as the target permutation. The order of comparison is the standard lexicographic order on integer sequences. More precisely, the target permutation must be the `k`-th permutation (1-indexed) in the sorted list of all permutations of `1..n` that have the **minimum possible number of inversions** among all permutations of `1..n`? No—that is not the case. Instead, the task is to find the `k`-th lexicographically smallest permutation of `1..n` such that the **total number of inversions** is exactly some fixed value, where the fixed value is the number of inversions that the permutation constructed by the algorithm in the snippet would produce. This is ambiguous. Upon closer inspection, the snippet's logic builds a permutation by deciding at each step whether to place the next smallest number at the front or the back, based on a precomputed power-of-two sequence (`calc`). The permutation it generates has exactly `k` "large-to-small" choices that determine its position in the set of permutations with a specific inversion count. The task is this: Given `n` and `k`, output the permutation that results from this exact greedy inversion-count-based construction. If `n` is large (≥41) and `k` exceeds `2^(n-1)` (which overflows), the answer is `-1` (impossible) because the number of possible permutations with that inversion count is bounded. More precisely, the number of permutations of `1..n` with a given inversion count is at most `2^(n-1)` for the maximal count, so if `k` exceeds that bound, output `-1`. Otherwise, output the permutation of size `n` built by repeatedly taking the next smallest unused number and deciding whether to put it at the front (if the remaining count `calc(idx)` of permutations with the remaining choices is at least `k`) or at the back (and then subtract that count from `k`).
// The provided snippet is a solution to a specific combinatorial problem: generate the `k`-th lexicographically smallest permutation among those that have the same "inversion profile" determined by a binary decision tree. The algorithm works as follows: For a given `n`, the maximum possible number of permutations that can be obtained by this construction is `2^(n-1)` (since at each of the `n-1` steps after the first, we choose between placing the current smallest number at the front or back). The function `calc(g)` returns `2^g` for `g` between 0 and 40, returns `-1` for `g>40` (indicating overflow beyond 64-bit), and returns 0 for negative `g`. If `n-1 > 40` (i.e., `n >= 42`), `calc(n-1)` returns `-1`, and the check `k > calc(n-1)` fails because comparing an integer with `-1` is not meaningful; actually in the snippet, they guard with `n <= 41 && k > calc(n-1)`. For large `n`, if `k` is huge, it might be impossible. But the logic is: if `n <= 41` and `k > 2^(n-1)`, output `-1`. If `n > 41`, then `2^(n-1)` is astronomically large, and any `k` that fits in the 64-bit integer is certainly ≤ that bound, so we proceed. The construction: Start with `s=0`, `e=n-1`, `num=1`, `idx=n-2`. For each `num` from 1 to `n`, compute `val = calc(idx)`. If `val == -1` (meaning `idx > 40`), then the remaining number of permutations is so large that the current smallest number goes to the front (since `k` is small relative to that). Otherwise, if `val >= k`, place `num` at the front (`perm[s++] = num`). Else, place `num` at the back (`perm[e--] = num`) and subtract `val` from `k`. Then decrement `idx` and increment `num`. This algorithm effectively constructs the `k`-th permutation in the set of permutations that have a specific "stack-sortable" property (the number of inversions is exactly the number of times we placed a number at the back). The time complexity is O(n) because we make a single pass over `n` numbers, and each `calc` call is O(1) (since it's capped at 40 recursion). Space complexity is O(n) for the permutation vector. The edge cases include: `n=1` (then `idx=-1`, `calc(-1)` returns 0, and since `val=0`, `k` must be 1 to avoid `val>=k`? Actually if `n=1`, the loop runs once, `idx=-1`, `val=0`, and if `k` is 1, `val >= k` is false, so it places at back, but `s==e==0`, so after `perm[e--]`, it's fine. For `n=1` any `k` other than 1? The initial check: `n <= 41 && k > calc(n-1)` where `calc(0)=1`, so if `k>1`, output -1. So only `k=1` works, and the permutation is `[1]`. Another edge: when `k` exactly equals `val`, the algorithm places `num` at back, so it handles the boundary correctly. Also, for `n>41`, `calc(idx)` returns -1 when `idx>40`, so it always places at front because `val==-1` triggers the front placement. That is correct because the number of permutations is enormous. The reference solution must replicate this exact behavior.
#include <vector>
#include <cstdint>

// Compute 2^g for g in [0,40], return -1 for g>40, 0 for g<0.
int64_t power_of_two_capped(int g) {
    if (g < 0) return 0;
    if (g > 40) return -1;
    return static_cast<int64_t>(1) << g; // 2^g fits in int64_t for g<=40
}

// Construct the k-th permutation (1-indexed) of size n according to the greedy inversion-count algorithm.
// Returns an empty vector if impossible (i.e., n<=41 and k > 2^(n-1)).
std::vector<int> kth_permutation_by_inversion(int n, int64_t k) {
    if (n <= 41) {
        int64_t max_count = power_of_two_capped(n - 1);
        if (k > max_count) {
            return {}; // impossible
        }
    }
    std::vector<int> result(n);
    int left = 0, right = n - 1;
    int current_num = 1;
    int idx = n - 2;
    while (current_num <= n) {
        int64_t val = power_of_two_capped(idx);
        if (val == -1) {
            result[left++] = current_num;
        } else {
            if (val >= k) {
                result[left++] = current_num;
            } else {
                result[right--] = current_num;
                k -= val;
            }
        }
        --idx;
        ++current_num;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared above (not repeated here).

int main() {
    // Basic valid cases
    assert((kth_permutation_by_inversion(1, 1) == std::vector<int>{1}));
    assert((kth_permutation_by_inversion(2, 1) == std::vector<int>{1, 2}));
    assert((kth_permutation_by_inversion(2, 2) == std::vector<int>{2, 1}));
    assert((kth_permutation_by_inversion(3, 1) == std::vector<int>{1, 2, 3}));
    assert((kth_permutation_by_inversion(3, 2) == std::vector<int>{1, 3, 2}));
    assert((kth_permutation_by_inversion(3, 3) == std::vector<int>{2, 1, 3}));
    assert((kth_permutation_by_inversion(3, 4) == std::vector<int>{3, 2, 1}));

    // Edge: k exactly at maximum
    assert((kth_permutation_by_inversion(4, 8) == std::vector<int>{4, 3, 2, 1}));

    // Impossible case: n=3, k=5 (max is 4)
    assert(kth_permutation_by_inversion(3, 5).empty());

    // Large n (n=42) with large k, should succeed
    auto res = kth_permutation_by_inversion(42, static_cast<int64_t>(1) << 40);
    assert(!res.empty());
    assert(res.size() == 42);
    // The first element must be 1 because val=-1 for idx=40
    assert(res[0] == 1);

    // Another large n test with k=1 (should be 1..n)
    auto res2 = kth_permutation_by_inversion(50, 1);
    for (int i = 0; i < 50; ++i) {
        assert(res2[i] == i + 1);
    }

    // Edge: n=41, k = 2^40 (the maximum), should give reversed permutation
    auto res3 = kth_permutation_by_inversion(41, static_cast<int64_t>(1) << 40);
    assert(res3.size() == 41);
    for (int i = 0; i < 41; ++i) {
        assert(res3[i] == 41 - i);
    }

    // Edge: n=41, k = 2^40 + 1, should be impossible
    assert(kth_permutation_by_inversion(41, (static_cast<int64_t>(1) << 40) + 1).empty());

    return 0;
}
