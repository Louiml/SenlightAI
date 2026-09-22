You are given an integer `N` (the number of computers, each at a distinct position from 0 to N-1 in order) and an integer `D` (a maximum allowed distance between any two monitored computers of the same country), followed by an array `countries` of length `N` where `countries[i]` is the country code of the computer at position `i`. A subset of computers is considered "valid" if for every country, it monitors at most 2 computers from that country, and if exactly 2 computers are monitored from the same country, the absolute difference of their positions must be ≤ `D`. The empty subset is not allowed. Write a standalone C++ function named `countValidSubsets` that takes `N`, `D`, and a vector of country codes as parameters and returns the total number of valid non-empty subsets modulo `1,000,000,007`. The function must not use global variables or an entry point, and should be self-contained with appropriate includes.

#include <cassert>
#include <vector>

int main() {
    // Single computer, any subset except empty -> 1
    assert(countValidSubsets(1, 5, {42}) == 1);

    // Two computers, same country, D=0 -> can't pick both, so subsets: {0}, {1} -> 2
    assert(countValidSubsets(2, 0, {1,1}) == 2);

    // Two computers, same country, D=1 -> can pick both, so subsets: {0},{1},{0,1} -> 3
    assert(countValidSubsets(2, 1, {1,1}) == 3);

    // Two computers, different countries, any D -> all 3 non-empty subsets valid
    assert(countValidSubsets(2, 0, {1,2}) == 3);

    // Three computers, same country, D=1 -> valid subsets: any single (3), any pair with distance ≤1: possible pairs (0,1) and (1,2) -> total 5; cannot pick all three.
    assert(countValidSubsets(3, 1, {5,5,5}) == 5);

    // Three computers, same country, D=2 -> valid: singles (3), pairs (3 pairs all distance ≤2), triple invalid (3>2) -> total 6
    assert(countValidSubsets(3, 2, {5,5,5}) == 6);

    // Example: N=4, D=1, countries: [1,2,1,2] 
    // Singles: 4. Pairs: (0,1) diff countries ok, (0,2) same country distance 2>1 invalid, (0,3) diff, (1,2) diff, (1,3) same country distance 2>1 invalid, (2,3) diff => 4 valid pairs. Triples: any triple with at most 2 per country: (0,1,2) has country1 at 0,2 distance2>1 invalid; (0,1,3) country2 at 1,3 distance2>1 invalid; (0,2,3) country1 at 0,2 distance2>1 invalid; (1,2,3) country2 at 1,3 distance2>1 invalid. Quad all invalid. Total 8.
    assert(countValidSubsets(4, 1, {1,2,1,2}) == 8);

    // Large D makes all subsets valid as long as each country ≤2: N=4, same country all, D=100 -> singles 4, pairs 6, triples invalid -> total 10
    assert(countValidSubsets(4, 100, {7,7,7,7}) == 10);

    // Zero D but different countries: any subset valid -> all 2^N -1
    assert(countValidSubsets(3, 0, {10,20,30}) == 7);

    return 0;
}

#include <vector>
#include <unordered_map>

const int MOD = 1000000007;

// Count valid non-empty subsets of monitored computers.
int countValidSubsets(int N, int D, const std::vector<int>& countries) {
    int totalMasks = 1 << N;
    int ans = 0;

    for (int mask = 1; mask < totalMasks; ++mask) {
        std::unordered_map<int, int> count;
        std::unordered_map<int, int> lastPos;
        bool valid = true;

        for (int i = 0; i < N; ++i) {
            if (mask & (1 << i)) {
                int c = countries[i];
                int newCount = count[c] + 1;
                if (newCount > 2) {
                    valid = false;
                    break;
                }
                if (newCount == 2) {
                    if (std::abs(i - lastPos[c]) > D) {
                        valid = false;
                        break;
                    }
                }
                count[c] = newCount;
                lastPos[c] = i;
            }
        }

        if (valid) {
            ans = (ans + 1) % MOD;
        }
    }

    return ans;
}

// The problem reduces to counting subsets of indices (from 0 to N-1) that satisfy two constraints per country: (1) at most 2 selected computers from that country, and (2) if 2 are selected, their positional distance must be ≤ D. Since N is small (implied by the original snippet using bitmask enumeration), we can enumerate all non-empty subsets via bitmask from 1 to (1<<N)-1. For each mask, iterate over all bits; for each selected index, maintain two arrays: `count[country]` (number of selected computers from that country) and `lastPos[country]` (position of the most recently seen selected computer for that country in index order). For each selected index with country `c`, increment count; if count becomes >2, invalid. If count becomes exactly 2, check if `abs(i - lastPos[c]) > D`, then invalid; otherwise update lastPos to i. If the mask passes all checks, increment the answer modulo MOD. Edge cases: only one computer from a country is always allowed; two computers from different countries are independent; D can be 0 (then only adjacent same-country pairs are allowed if distance 0, but note positions are distinct integers, so distance 0 never happens between different indices, so only exactly equal positions would be distance 0, but indices are distinct, so D=0 means two from same country are only allowed if they are same index, which is impossible, so any country with ≥2 selected becomes invalid). Also the empty subset (mask=0) is excluded. Time complexity: O(2^N * N) per call, space O(M) where M is the maximum country code (but we can use unordered_map to avoid huge arrays), but for simplicity we can use arrays sized to the max country code +1. Given N is small (likely ≤ 20), this is feasible.
