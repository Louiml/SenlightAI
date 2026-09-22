Write a C++ function `simulatePhotonCollisions` that takes three integers `n`, `m`, `k` and two vectors of length `n`: `positions` (0-indexed starting positions of photons on a circle of length `m`) and `directions` (a string of `n` characters, each `'R'` or `'L'`, indicating initial clockwise or counterclockwise motion). Each photon moves at 1 unit per second for `k` seconds. Photons pass through each other without interacting (no collisions), but when two photons would occupy the same point at the same time, they are considered indistinguishable and simply swap identities (we can treat them as passing through). After exactly `k` seconds, return a vector of `n` integers where the `i`-th element is the final 1-indexed position (from 0 to `m-1`, then add 1) of the photon that started at index `i`. The photon identities are tracked by their original indices even though they pass through each other. The input positions are given as 1-indexed values initially (convert to 0-indexed internally). The order of photons in the output is by original index.
The key insight is that when photons pass through each other (which is equivalent to swapping identities), the set of final positions (as a multiset) is the same as if they never interacted — each photon independently moves `k` steps in its direction modulo `m`. So we first compute the final position for each photon as if it were alone: `finalPos[i] = (pos[i] + dir[i]*k) mod m` (with `dir[i]=+1` for `'R'`, `-1` for `'L'`), handling negative modulo with `((x % m) + m) % m`. These positions form a multiset that must be assigned to the original indices. The order of assignments is determined by the relative order of starting positions. Because photons pass through each other, the cyclic order of photons on the circle is preserved (up to rotation). We sort photons by their starting position, and also sort the list of final positions. The sorted final positions correspond to the sorted starting positions in the same cyclic order, but the whole sequence may be rotated by a shift `p` that counts how many times a photon crosses the 0-point in a direction that advances its rank. Specifically, for each photon, if it moves right, it wraps `floor((k + pos)/m)` times, each wrap advances its index by 1 in the cyclic order; if it moves left, it wraps `floor((k + m-1 - pos)/m)` times, each wrap decreases its index by 1. The net shift `p = (sum of these counts) mod n` tells how many positions the sorted final list is rotated relative to the sorted starting list. Then we assign: the photon at sorted starting index `j` gets the final position at sorted index `(j + p) mod n`. Edge cases: `k` can be large (up to 10^9), `n` and `m` up to 10^5; positions are 1-indexed input, convert to 0-indexed. If `n=0`, return empty. Time complexity O(n log n) due to sorting, space O(n).
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// Simulate photons moving on a circle of length m for k seconds.
// positions are 1-indexed input, directions are 'R' or 'L'.
// Returns final 1-indexed positions for each original index.
std::vector<int> simulatePhotonCollisions(int n, long long m, long long k,
                                         const std::vector<int>& positions,
                                         const std::string& directions) {
    if (n == 0) return {};

    // Struct to hold each photon's data.
    struct Photon {
        int id;            // original index
        long long pos;     // 0-indexed starting position
        long long dir;     // +1 for R, -1 for L
    };

    std::vector<Photon> photons(n);
    for (int i = 0; i < n; ++i) {
        photons[i].id = i;
        photons[i].pos = static_cast<long long>(positions[i]) - 1; // to 0-indexed
        photons[i].dir = (directions[i] == 'R') ? 1LL : -1LL;
    }

    // Compute the net cyclic shift p (mod n).
    long long shift = 0;
    std::vector<long long> final_positions(n); // final positions (0-indexed) as if independent

    for (int i = 0; i < n; ++i) {
        long long pos = photons[i].pos;
        long long dir = photons[i].dir;
        // Number of times this photon wraps around the circle.
        long long wraps;
        if (dir == 1) {
            wraps = (k + pos) / m;
        } else {
            wraps = (k + m - 1 - pos) / m;
        }
        shift = (shift + dir * wraps) % m;
        shift %= n;
        // Final position ignoring passing through (just independent movement).
        long long final_pos = (pos + dir * k) % m;
        if (final_pos < 0) final_pos += m;
        final_positions[i] = final_pos;
    }

    // Sort photons by starting position.
    std::sort(photons.begin(), photons.end(), [](const Photon& a, const Photon& b) {
        return a.pos < b.pos;
    });

    // Sort all final positions.
    std::sort(final_positions.begin(), final_positions.end());

    // Assign final positions to original ids with rotation.
    std::vector<int> result(n, 0);
    for (int j = 0; j < n; ++j) {
        int rotated_index = static_cast<int>((j + shift) % n);
        if (rotated_index < 0) rotated_index += n; // ensure non-negative
        int original_id = photons[j].id;
        result[original_id] = static_cast<int>(final_positions[rotated_index] + 1); // to 1-indexed
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (same file). This is the test driver.
int main() {
    // Example 1: Simple no wrap
    {
        int n = 3;
        long long m = 10, k = 1;
        std::vector<int> positions = {1, 4, 7}; // 0->0, 4->3, 7->6 (0-indexed)
        std::string dirs = "RRL";
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        // Photon0: pos0->1 (R), Photon1: pos3->2 (R? directions[1] is 'R'? Actually string "RRL": idx0='R', idx1='R', idx2='L')
        // So photon0: 0->1, photon1:3->4, photon2:6->5. No interaction, no wrap. So result = {2,5,6}
        assert(result == std::vector<int>({2,5,6}));
    }

    // Example 2: Wrap around with multiple photons
    {
        int n = 2;
        long long m = 5, k = 7;
        std::vector<int> positions = {1, 5}; // 0-indexed: 0,4
        std::string dirs = "RL";
        // Photon0: pos0, dir1: final = (0+7)%5=2 -> 1-indexed 3
        // Photon1: pos4, dir-1: final = (4-7)%5 = (-3)%5=2 -> 1-indexed 3
        // They both end at same position 2 (0-indexed) -> both should get 3.
        // Since they pass through, identities swap? Actually they meet at some time, but final positions same, so both get same.
        // Let's compute shift: photon0 wraps: (7+0)/5=1 -> shift +=1; photon1 wraps: (7+5-1-4)/5 = (7)/5=1 -> shift -=1; net shift=0.
        // Sorted positions: 0,4. Sorted finals: 2,2. Assign: index0->2, index1->2. So result: {3,3}
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        assert(result == std::vector<int>({3,3}));
    }

    // Example 3: Larger test with known result from brute force simulation
    {
        int n = 4;
        long long m = 8, k = 3;
        std::vector<int> positions = {2, 4, 6, 8}; // 0-indexed: 1,3,5,7
        std::string dirs = "RLRL";
        // Brute manual: photons move independently:
        // p0:1+3=4, p1:3-3=0, p2:5+3=0 (wrap), p3:7-3=4
        // Final positions: {4,0,0,4} sorted: {0,0,4,4}
        // Sorted starting pos: 1,3,5,7
        // Shift calculation: p0 R: wraps=(3+1)/8=0, p1 L: (3+8-1-3)/8=(7)/8=0, p2 R: (3+5)/8=1 (since 8/8=1), p3 L:(3+8-1-7)/8=(3)/8=0 => net shift=+1 mod 4 =1
        // Assign sorted index j gets sorted final at (j+1)%4:
        // j=0 (pos1) gets final[1]=0 -> 1-indexed 1
        // j=1 (pos3) gets final[2]=4 -> 5
        // j=2 (pos5) gets final[3]=4 -> 5
        // j=3 (pos7) gets final[0]=0 -> 1
        // So result (by original id): id0 (pos1) ->1, id1(pos3)->5, id2(pos5)->5, id3(pos7)->1 => {1,5,5,1}
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        assert(result == std::vector<int>({1,5,5,1}));
    }

    // Example 4: Single photon
    {
        int n = 1;
        long long m = 10, k = 25;
        std::vector<int> positions = {3}; // 0-indexed 2
        std::string dirs = "R";
        // Final pos = (2+25)%10 = 7 -> 8
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        assert(result == std::vector<int>({8}));
    }

    // Example 5: Empty input
    {
        std::vector<int> result = simulatePhotonCollisions(0, 10, 5, {}, "");
        assert(result.empty());
    }

    // Example 6: All photons same starting position (degenerate)
    {
        int n = 3;
        long long m = 10, k = 5;
        std::vector<int> positions = {5,5,5}; // all 0-indexed 4
        std::string dirs = "RLL";
        // All final positions: (4+5)%10=9 for R, (4-5)%10=9 for L (both same) => all 9 -> 1-indexed 10
        // Sort positions all same, finals all same, shift: R wraps (5+4)/10=0, L wraps (5+10-1-4)/10=(10)/10=1 each -> net shift = -2 mod 3 = 1
        // But all finals same, so any rotation gives same result. All should be 10.
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        assert(result == std::vector<int>({10,10,10}));
    }

    // Example 7: Large k causing many wraps
    {
        int n = 2;
        long long m = 3, k = 10;
        std::vector<int> positions = {1,2}; // 0-indexed 0,1
        std::string dirs = "RR";
        // p0: (0+10)%3 = 1 -> 2, p1: (1+10)%3 = 2 -> 3
        // shift: both R: wraps (10+0)/3=3, (10+1)/3=3 => sum=6 mod 2 =0
        // Sorted positions: 0,1; sorted finals: 1,2; no rotation -> result {2,3}
        std::vector<int> result = simulatePhotonCollisions(n, m, k, positions, dirs);
        assert(result == std::vector<int>({2,3}));
    }

    return 0;
}
