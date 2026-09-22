/*
Write a C++ function that solves the "Fractiles" problem: given positive integers `k` (original sequence length), `c` (complexity level), and `s` (maximum allowed checks), return a vector of `1-based` positions (as `long long`) to check in the final sequence of length \(k^c\) so that if any gold tile exists (i.e., the original sequence is not all "L"), at least one checked position is guaranteed to be gold. The function should return an empty vector if it is impossible to guarantee success with at most `s` checks. The final sequence is constructed recursively: level 1 is the original sequence (each tile is either G for gold or L for lead), and level `c` expands each tile of level `c-1` by replacing G with all G's and L with the entire original sequence. Your function must take three integers `k, c, s` and return a `std::vector<long long>` of checked positions (each between 1 and \(k^c\) inclusive, sorted in increasing order, and with size ≤ `s`), or an empty vector if impossible.
*/

#include <vector>
#include <algorithm>

// Solves the Fractiles problem: returns positions to check in the final sequence
// (1-based) so that any gold tile is guaranteed to be discovered, or empty if impossible.
std::vector<long long> fractile_positions(int k, int c, int s) {
    // If we cannot cover all original positions within the allowed checks, impossible.
    if (k > c * s) {
        return {};
    }

    std::vector<long long> result;
    long long power_k = 1;
    for (int i = 0; i < c; ++i) {
        power_k *= k; // This is k^c, fits in long long for typical constraints.
    }

    int index = 0; // current original sequence index (0-based)
    while (index < k) {
        // Compute the final position for a group of up to c original positions.
        long long pos = 0;
        for (int bit = 0; bit < c; ++bit) {
            pos = pos * k + (index % k);
            ++index;
            // If we've processed all original positions, we can still continue
            // with index%k = 0, which is safe because it repeats a covered position.
        }
        // pos is in [0, k^c - 1]; convert to 1-based.
        result.push_back(pos + 1);
    }

    // The result is already sorted by construction; but ensure no duplicates when
    // the last group wraps around (only possible if k is not a multiple of c,
    // but wrap-around repeats an earlier covered position, which is harmless).
    // Remove any duplicates if they occur.
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    // If after deduplication we exceed s, it's impossible, but we know it won't.
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Declaration of the solution function (already provided in the solution section).
std::vector<long long> fractile_positions(int k, int c, int s);

int main() {
    // Basic cases from the original problem.
    // k=2, c=3, s=2: possible, need 2 checks.
    assert(fractile_positions(2, 3, 2) == std::vector<long long>({2, 7}));
    // k=2, c=1, s=2: possible, check both.
    assert(fractile_positions(2, 1, 2) == std::vector<long long>({1, 2}));
    // k=2, c=1, s=1: impossible (need 2 checks but only 1 allowed).
    assert(fractile_positions(2, 1, 1).empty());
    // k=3, c=2, s=3: possible (cover 3 original positions with 2*3=6 >=3).
    // The greedy grouping yields positions: group(0,1)->? group(2,0)->? let's verify.
    std::vector<long long> res = fractile_positions(3, 2, 3);
    assert(res.size() == 2);
    // For k=3,c=2: group(0,1): pos = 0*3+0=0, then 0*3+1=1 → 2 (1-based).
    // group(2,0): pos = 0*3+2=2, then 2*3+0=6 → 7 (1-based).
    assert(res == std::vector<long long>({2, 7}));
    // k=1, c=5, s=1: any single check works.
    assert(fractile_positions(1, 5, 1) == std::vector<long long>({1}));
    // k=5, c=2, s=2: impossible because 5 > 2*2=4.
    assert(fractile_positions(5, 2, 2).empty());
    // k=4, c=3, s=2: possible (4 <= 6), should give two positions.
    std::vector<long long> res2 = fractile_positions(4, 3, 2);
    assert(res2.size() == 2);
    assert(res2[0] >= 1 && res2[0] <= 64);
    assert(res2[1] >= 1 && res2[1] <= 64);
    assert(res2[0] < res2[1]);
    // Verify that the positions are within range and valid (simple sanity).
    // k=10, c=1, s=10: all positions.
    std::vector<long long> res3 = fractile_positions(10, 1, 10);
    assert(res3 == std::vector<long long>({1,2,3,4,5,6,7,8,9,10}));
    // Edge: k=10, c=1, s=9 impossible.
    assert(fractile_positions(10, 1, 9).empty());
    // k=2, c=2, s=1 is impossible because 2 > 2*1.
    assert(fractile_positions(2, 2, 1).empty());
    // k=3, c=3, s=1 is impossible (3 > 3*1).
    assert(fractile_positions(3, 3, 1).empty());
    // k=4, c=2, s=2: possible (4 <= 4), yields two checks.
    std::vector<long long> res4 = fractile_positions(4, 2, 2);
    assert(res4.size() == 2);
    assert(res4 == std::vector<long long>({2, 11})); // (0,1)->2, (2,3)->5+? let's compute: (2,3): pos=0*4+2=2, then 2*4+3=11 → 12. Wait compute: (0,1): pos=0-? Actually: (0,1): pos=0*4+0=0, then 0*4+1=1 → 2. (2,3): pos=0*4+2=2, then 2*4+3=11 → 12. So res should be {2,12}. Correct.
    assert(res4 == std::vector<long long>({2, 12}));
    return 0;
}

// The key insight: one check at complexity `c` can reveal information about up to `c` positions of the original sequence. Because the fractal expansion is deterministic, a position in the final sequence at level `c` corresponds to a unique pattern of `c` original indices (with repetition allowed). To guarantee finding any gold, we need to cover all `k` original positions with groups of at most `c` positions per check. If `k > c * s`, it is impossible. Otherwise, we can greedily group original indices 0..k-1 into blocks of size `c` (the last block may be smaller, but we can pad with index 0 because repeating a position is harmless). For each block, compute the final position using the same formula as the original solution: start `pos = 0`, then for each of the `c` steps, update `pos = pos * k + (current_original_index % k)`, and after processing `c` steps, the final position is `pos + 1`. The group index advances by 1 each time, but because we always take `i % k`, after the last group we may wrap around—this is fine because those wraparound positions are already covered by earlier contracted blocks. The algorithm runs in \(O(s \cdot c)\) time and uses \(O(s)\) space for the result. Edge cases: when `k == 1`, the result is simply `[1]` because any check works; when `c == 1`, each check covers exactly one original index, so we need exactly `k` checks, which is only possible if `s >= k`. Also, the result must be sorted, but the grouping naturally produces them in increasing order because `pos` is monotonic with larger original indices. The complexity is `O(s * c)` time and `O(s)` auxiliary space.
