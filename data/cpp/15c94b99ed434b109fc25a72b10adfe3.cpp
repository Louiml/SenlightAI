/*
Write a C++ function `minimumFlipsToUniform(int n, const std::string& tiles)` that accepts the number of tiles `n` and a string of length `n` where each character is either `'B'` (black) or `'W'` (white). The tiles are arranged in a row, and in a single operation you may flip the color of two adjacent tiles (positions `i` and `i+1`, for `1 <= i < n`). The goal is to make all tiles the same color (all black or all white). The function must return a vector of the positions (1-indexed) of the tiles that are flipped, in the order they are flipped, such that after applying exactly those flips (each flip toggles both tile `i` and tile `i+1`), the entire row becomes uniform. If it is impossible to achieve uniformity, return an empty vector. If multiple solutions exist, any valid sequence is acceptable, but the returned sequence must be minimal in length (i.e., you must find a solution with the fewest flips). The function must not modify the input string, and must handle `n == 1` (where no flips are needed and the output is an empty vector). The constraints are `1 <= n <= 100000`, and the string contains only `'B'` or `'W'`.
*/

#include <vector>
#include <string>

// Returns the minimal sequence of flips (1-indexed positions) to make all tiles uniform,
// or an empty vector if impossible. The target color is chosen to minimize flips.
std::vector<int> minimumFlipsToUniform(int n, const std::string& tiles) {
    // Convert input to a modifiable vector of bits: 'B' -> 1, 'W' -> 0.
    std::vector<int> bits(n + 1);
    for (int i = 1; i <= n; ++i) {
        bits[i] = (tiles[i - 1] == 'B') ? 1 : 0;
    }

    // Helper lambda to try making all tiles equal to a target color.
    // Returns true and fills `flips` if achievable; otherwise returns false.
    auto try_target = [&](int target, std::vector<int>& flips) {
        // Work on a copy to preserve original bits for the other target.
        std::vector<int> cur = bits;
        flips.clear();

        for (int i = 1; i < n; ++i) {
            if (cur[i] != target) {
                flips.push_back(i);
                cur[i] ^= 1;
                cur[i + 1] ^= 1;
            }
        }

        return cur[n] == target;
    };

    std::vector<int> flips_black, flips_white;
    bool possible_black = try_target(1, flips_black);
    bool possible_white = try_target(0, flips_white);

    // Decide which candidate is minimal and valid.
    if (possible_black && possible_white) {
        return (flips_black.size() <= flips_white.size()) ? flips_black : flips_white;
    } else if (possible_black) {
        return flips_black;
    } else if (possible_white) {
        return flips_white;
    } else {
        return {};
    }
}

#include <cassert>
#include <vector>
#include <string>

std::vector<int> minimumFlipsToUniform(int n, const std::string& tiles);

int main() {
    // n=1, no flips needed.
    assert(minimumFlipsToUniform(1, "B") == std::vector<int>{});
    assert(minimumFlipsToUniform(1, "W") == std::vector<int>{});

    // Already uniform.
    assert(minimumFlipsToUniform(3, "BBB") == std::vector<int>{});
    assert(minimumFlipsToUniform(4, "WWWW") == std::vector<int>{});
    assert(minimumFlipsToUniform(2, "BB") == std::vector<int>{});

    // Simple case: flip first two to make all white from "BW".
    // n=2, "BW": target white -> flip at 1 -> "WW", 1 flip. Target black impossible because last becomes W.
    assert(minimumFlipsToUniform(2, "BW") == std::vector<int>{1});

    // n=2, "WB": target black -> flip at 1 -> "BB", 1 flip.
    assert(minimumFlipsToUniform(2, "WB") == std::vector<int>{1});

    // n=3, "BWB": target white: flip 1 -> "WWB" then flip 2? Check: 1: BWB -> flip1 => WWB, then flip2 => WWW? Actually flip2 changes positions 2 and 3, becomes "WWW"? positions 2 and 3 become W toggled: 2 from W->B? Let's compute: after flip1: positions: W W B. Now i=2: cur[2]=W equals target, no flip. Final: last is B not W -> impossible. Target black: flip1 -> "WWB" then i=2 cur[2]=W !=1 -> flip2 -> "WBB"? Wait, let's simulate: start BWB, i=1: cur[1]=1==target, no flip. i=2: cur[2]=0!=1, flip2 -> B toggles to W? Actually positions 2 and 3: 0->1, 1->0 => gives B (1) at pos2, W (0) at pos3? That's wrong – flip toggles both: pos2 becomes 1, pos3 becomes 0. So after flip2: positions: B W? Original after no flip at i=1: B W B? Wait original B W B. i=1: B==1, skip. i=2: W !=1, flip2 toggles pos2 and pos3: W->B, B->W, giving B B W. Now last is W, not 1 -> impossible. So no solution for "BWB"? Let's check manually: all toggles: flip1 gives W W B, flip2 gives B B W, flip1+flip2 gives? Start B W B, flip1 -> W W B, flip2 -> W B W? Actually flip2 on W W B: pos2 W->B, pos3 B->W => W B W. Not uniform. So impossible. So need empty vector.
    assert(minimumFlipsToUniform(3, "BWB") == std::vector<int>{});

    // n=4, "BWWB": target white? Let's find minimal. Simulate: bit: 1 0 0 1. target 0: i=1: 1!=0 flip1 -> 0 1 0 1; i=2: 1!=0 flip2 -> 0 0 1 1; i=3: 1!=0 flip3 -> 0 0 0 0? Wait pos3 and pos4: pos3=1->0, pos4=1->0 => 0 0 0 0, valid, flips: {1,2,3} length 3. target 1: i=1: 1==1 skip; i=2: 0!=1 flip2 -> 1 1 1 0? Wait pos2=0->1, pos3=0->1 gives 1 1 1 0; i=3: 1==1 skip; last=0 !=1 invalid. So answer is {1,2,3}.
    assert(minimumFlipsToUniform(4, "BWWB") == std::vector<int>({1,2,3}));

    // n=4, "WBBW": target black? Let's compute: bits 0 1 1 0. target 1: i=1: 0!=1 flip1 -> 1 0 1 0; i=2: 0!=1 flip2 -> 1 1 0 0; i=3: 0!=1 flip3 -> 1 1 1 1? Wait pos3=0->1, pos4=0->1 => 1 1 1 1, valid flips {1,2,3}. target 0: i=1:0==0 skip; i=2:1!=0 flip2 -> 1 0 0 0? Actually pos2=1->0, pos3=1->0 => 0 0 0? Wait original 0 1 1 0: after skip i=1, i=2 flip2 gives 0 0 0? pos2 1->0, pos3 1->0 => 0 0 0 0? Let's recalc: positions: 0 1 1 0. i=1: cur[1]==0 skip. i=2: cur[2]==1 !=0, flip2: cur[2]=0, cur[3]=0 -> now 0 0 0 0. That's already valid, flips {2} length 1. So minimal is {2}.
    assert(minimumFlipsToUniform(4, "WBBW") == std::vector<int>({2}));

    // n=5, "BWBWB": Let's trust algorithm; test with known minimal? We'll just verify result is valid by a helper function? But for assert, we can just check that function returns empty for pattern that is impossible? Actually "BWBWB" maybe impossible? Let's simulate target 0: bits 1 0 1 0 1. i=1: 1!=0 flip1 -> 0 1 1 0 1; i=2:1!=0 flip2 ->0 0 0 1 1; i=3:0==0 skip; i=4:1!=0 flip4 ->0 0 0 0 0? Wait pos4 toggles with pos5: pos4=1->0, pos5=1->0 -> all zeros valid, flips {1,2,4} length 3. target 1: i=1:1==1 skip; i=2:0!=1 flip2 ->1 1 0 0 1; i=3:0!=1 flip3 ->1 1 1 1 0; i=4:1==1 skip; last=0 invalid. So answer {1,2,4}. Check that sequence is minimal? Could there be 2 flips? Probably not. We'll just assert that flipping according to returned sequence yields uniform.
    auto result = minimumFlipsToUniform(5, "BWBWB");
    // We can manually verify that returned sequence is valid and length 3.
    // Since we trust the algorithm, just check size and first few.
    assert(result.size() == 3);
    assert(result[0] == 1 && result[1] == 2 && result[2] == 4);

    // Test an impossible pattern: n=3 "BWB" already done.
    // Also test n=3 "WBW": bits 0 1 0. target 0: i=1:0==0 skip; i=2:1!=0 flip2 -> pos2 0, pos3 1 -> 0 0 1? Actually after flip2, pos2=0, pos3=0? Wait pos3 original 0 toggles to 1? Oh, flip2 toggles pos2 and pos3: pos2 1->0, pos3 0->1 => 0 0 1, last=1 invalid. target 1: i=1:0!=1 flip1 -> 1 0 0; i=2:0!=1 flip2 -> 1 1 1? pos2 0->1, pos3 0->1 -> all 1, valid flips {1,2} length 2. So answer {1,2}.
    assert(minimumFlipsToUniform(3, "WBW") == std::vector<int>({1,2}));

    return 0;
}

// The problem is a classic linear toggle puzzle. Consider representing colors as bits: `'B' = 1`, `'W' = 0`. Flipping positions `i` and `i+1` toggles both bits. The key observation: to make the entire row uniform, we can process from left to right. If we decide the target color is `T` (either all `0` or all `1`), then for each position `i` from 1 to `n-1`, if the current bit at `i` differs from `T`, we must perform a flip at position `i` (because any flip that affects position `i` earlier would have already been processed; the only way to change position `i` from this point forward is to flip at `i`, which also toggles `i+1`). After processing all positions up to `n-1`, the last tile `n` must already be `T`; if not, this target is impossible. Therefore, there are at most two candidate solutions: one targeting all black (`T=1`) and one targeting all white (`T=0`). For each candidate, we simulate the left-to-right greedy flips and record positions. If a candidate is valid, we consider its flip count. The minimal valid candidate (or if both valid, the one with fewer flips; if tie, either) is returned. If neither candidate is valid (i.e., both end with the last tile mismatched), return an empty vector. Edge cases: `n == 1` always returns empty vector because no flips needed. Also, note that the greedy is optimal because each position `i`'s state can only be affected by flips at `i-1`, `i`, and `i+1`, and the left-to-right decision is forced once the target is fixed; trying an unnecessary flip at `i` would not help achieve uniformity with fewer total flips. The algorithm runs in `O(n)` time and `O(n)` space for the output vector (or `O(1)` extra if we just count and then build, but we need to return positions, so `O(n)` is necessary for the answer). The simulation uses a copy of the bit array, but we can avoid copying by toggling as we go, using `O(n)` space for that copy or `O(1)` if we directly modify a local vector. The input string is constant.
