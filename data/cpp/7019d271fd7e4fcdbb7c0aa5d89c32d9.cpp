/*
Implement a C++ function `encodeMixedTileIndex` that, given the length `L` of a mixed tile (a sequence of sites), a pointer to an array `part_indxs` of length `L` containing the current state index of each site (each in the range `[0, nst-1]`), the number of possible states per site `nst`, and a global offset `st_glind`, returns the single flattened index of the tile in a master state array using mixed-radix encoding. The encoding must follow the exact formula: for `l` from `0` to `L-1`, accumulate `ind = ind * nst + part_indxs[l]`, and finally add `st_glind`. The function must be `const`-correct (take the state array as `const int*`), handle `L == 0` by returning `st_glind`, and validate that `nst > 0` (if `nst <= 0`, return `-1` to indicate an error). The function should not modify the input array, should be self-contained with only `<cstddef>` and `<algorithm>` included, and must not use any global state. The test harness will call this function with various arrays and offsets to verify the mixed-radix encoding matches manual calculations, including edge cases with a single site, zero sites, and a large offset.
*/
#include <cstddef>

// Encode a sequence of site states into a single mixed-radix index.
// part_indxs: array of length L, each value in [0, nst-1]
// nst: number of states per site, must be > 0
// st_glind: global offset added to the final index
// Returns: the encoded index, or -1 if nst <= 0
long long encodeMixedTileIndex(int L, const int* part_indxs, int nst, int st_glind) {
    if (nst <= 0) {
        return -1;
    }
    long long ind = 0;
    for (int l = 0; l < L; ++l) {
        ind = ind * nst + part_indxs[l];
    }
    return ind + st_glind;
}
#include <cassert>

int main() {
    // Basic case: L=3, nst=2, states [1,0,1] => 1*4 + 0*2 + 1 = 5
    int states1[3] = {1, 0, 1};
    assert(encodeMixedTileIndex(3, states1, 2, 0) == 5);
    
    // With global offset
    assert(encodeMixedTileIndex(3, states1, 2, 10) == 15);
    
    // Single site: state 2, nst=3 => just 2 plus offset
    int state_single = 2;
    assert(encodeMixedTileIndex(1, &state_single, 3, 0) == 2);
    assert(encodeMixedTileIndex(1, &state_single, 3, 7) == 9);
    
    // Zero-length tile: returns only the offset
    assert(encodeMixedTileIndex(0, nullptr, 3, 42) == 42);
    
    // Larger nst: L=2, nst=4, states [3,2] => 3*4+2 = 14
    int states2[2] = {3, 2};
    assert(encodeMixedTileIndex(2, states2, 4, 0) == 14);
    
    // All zeros: L=4, nst=5 => 0
    int states3[4] = {0, 0, 0, 0};
    assert(encodeMixedTileIndex(4, states3, 5, 0) == 0);
    
    // Maximum values for small L: L=2, nst=3, states [2,2] => 2*3+2=8
    int states4[2] = {2, 2};
    assert(encodeMixedTileIndex(2, states4, 3, 0) == 8);
    
    // Error case: nst = 0
    int states5[2] = {0, 1};
    assert(encodeMixedTileIndex(2, states5, 0, 5) == -1);
    
    // Stress-like: L=6, nst=2, states [1,1,1,1,1,1] => 63, plus offset
    int states6[6] = {1, 1, 1, 1, 1, 1};
    assert(encodeMixedTileIndex(6, states6, 2, 0) == 63);
    assert(encodeMixedTileIndex(6, states6, 2, 100) == 163);
    
    // Check const-correctness with a const array
    const int const_states[3] = {0, 1, 2};
    assert(encodeMixedTileIndex(3, const_states, 3, 0) == 0*9 + 1*3 + 2);
    
    return 0;
}
// The core task is to implement mixed-radix encoding, which is exactly how the original code's `find_ind` function computes indices for site states in a lattice. The mixed-radix system uses a fixed base `nst` for each digit position, where each digit is the state of a site. Starting with `ind = 0`, we multiply the current accumulator by `nst` and add the next site's state. This is analogous to converting a number from base `nst` to decimal, but here the "digits" are already in the range `[0, nst-1]`, so we do not need to mod or divide—just accumulate. The final `st_glind` is an offset added after the loop, representing a global base index for all possible tile configurations (e.g., in the original code, it is `18` to skip over pair probabilities). Edge cases: if `L` is zero, the loop runs zero times, so `ind` remains `0`, and we return `st_glind`. If `nst` is non-positive, the encoding is ill-defined, so we return an error indicator `-1`. For positive `nst` and any `L`, the computation is deterministic and linear in `L`. The time complexity is O(L) because we iterate exactly once over the array. The space complexity is O(1) because we only use a single integer accumulator and no auxiliary data structures. Potential overflow is possible if `nst` and `L` are large, but for typical uses (e.g., `nst` small and `L` up to a few dozen) the accumulator fits in a 64-bit integer. The function should be `long long` to accommodate larger values, and the input states are assumed to be valid (no explicit validation for each state is required by the task, but we could add a debug assertion if needed). The reference solution below follows the exact logic of `find_ind` from the snippet, stripping away any domain-specific reactions and focusing solely on the index calculation. The test code verifies known values: for `L=3`, `nst=2`, states `[1,0,1]`, the encoding gives `1*2*2 + 0*2 + 1 = 5`, and with offset `10` the result is `15`. We also test zero-length, single-element, and a case with `nst=3`.
