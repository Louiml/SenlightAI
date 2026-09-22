// Write a C++ function `int earliestCowPosition(int n, const vector<int>& hierarchy, const vector<pair<int,int>>& fixedPositions)` that solves the following problem: Farmer John has `n` cows labeled `1` through `n` to place in a line of `n` positions (also labeled `1` through `n`), one cow per position. He has a specific hierarchy list of `m` cows (a permutation of some subset of `1..n`) that must appear in the line in that exact relative order (not necessarily contiguously). Additionally, `k` cows have fixed positions: for each pair `(cow, pos)`, cow `cow` must be placed at exactly position `pos`. All fixed positions are distinct, and no fixed position is used twice. Cow `1` is not among the fixed cows (if it were, just return its fixed position). Your task is to find the earliest (smallest numbered) position where cow `1` can be placed while satisfying all constraints (the hierarchy order and the fixed positions). If it is impossible to place cow `1` anywhere, return `-1`. The function should assume that a valid placement of all cows exists somewhere (though not necessarily with cow 1), but if placing cow 1 in a particular way leads to impossibility, that candidate is invalid. Return the smallest valid position for cow `1`.

// We need to find the minimum possible position for cow `1` such that there exists a full assignment of cows to positions respecting: (a) the fixed positions given, (b) the hierarchy order (cows in `hierarchy` appear in increasing position order), and (c) each position used once. Since `n` is small (but not given, could be up to 10^5), we cannot brute-force all permutations. Instead, iterate over candidate positions `h` from 1 to `n` for cow `1`. For each candidate, simulate the placement greedily: create a map `fixed` from cow to position (starting from the given fixed positions), and a set `occupied` of positions already used. If cow `1` is already fixed, return that. Otherwise, tentatively place cow 1 at `h` if `h` is unused; if it is used, skip to the next `h`. Then we need to check if the hierarchy can be satisfied. Process the hierarchy list in order. Maintain a pointer `j` that is the smallest position we can assign the next hierarchy cow to. For each cow in the hierarchy: if that cow already has a fixed position, check that the fixed position is at least `j`; if not, break (invalid). If it is, update `j` to that position + 1 and continue. If the cow is not fixed, we must find the next unused position >= `j`. If none exists, break. If we successfully assign it, mark that position used, and update `j` to that position+1. If we finish processing all m hierarchy cows, then the candidate `h` is valid and we return `h`. If we finish the entire loop without success, return -1. Time complexity: O(n*m) in worst case if we naively scan for next free position; but we can use a balanced tree (`std::set<int>`) of unused positions to find the next free position in O(log n), giving O(n*m log n). Since m is typically ≤ n, this is O(n^2 log n) worst-case, acceptable for n up to ~2000 but might be tight for 10^5. However, given the problem context (USACO Bronze "Milk Order"), n ≤ 100, so it's fine. Space complexity: O(n+m+k). Edge cases: cow 1 already fixed (return that). If a hierarchy cow’s fixed position forces it to appear before an earlier hierarchy cow’s position, invalid. If a candidate position for cow 1 conflicts with a fixed position, skip. Also, if the hierarchy is empty, any unused position works; return the smallest unused position.

#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cassert>

/**
 * Returns the smallest position for cow 1 such that all constraints are satisfied,
 * or -1 if impossible.
 *
 * @param n Number of positions (1..n)
 * @param hierarchy The required relative order of certain cows
 * @param fixedPositions Pairs (cow, position) that must hold exactly
 * @return The smallest valid position for cow 1, or -1.
 */
int earliestCowPosition(int n, const std::vector<int>& hierarchy,
                        const std::vector<std::pair<int,int>>& fixedPositions) {
    // Build mapping from cow to fixed position (if any)
    std::unordered_map<int,int> fixed;
    for (const auto& p : fixedPositions) {
        fixed[p.first] = p.second;
    }

    // If cow 1 is already fixed, return that position
    auto it1 = fixed.find(1);
    if (it1 != fixed.end()) {
        return it1->second;
    }

    // Try every possible position h for cow 1 in increasing order
    for (int h = 1; h <= n; ++h) {
        // Check if h is already occupied by a fixed cow (other than cow 1, which isn't fixed)
        bool occupiedByFixed = false;
        for (const auto& p : fixedPositions) {
            if (p.second == h) {
                occupiedByFixed = true;
                break;
            }
        }
        if (occupiedByFixed) continue;

        // Create a copy of fixed positions for this candidate
        std::unordered_map<int,int> curFixed = fixed;
        curFixed[1] = h;

        // Track which positions are occupied
        std::set<int> occupied;
        for (const auto& p : curFixed) {
            occupied.insert(p.second);
        }

        // Simulate placing hierarchy cows in order
        int nextPos = 1; // smallest position we can use next
        bool valid = true;
        for (size_t i = 0; i < hierarchy.size(); ++i) {
            int cow = hierarchy[i];
            auto it = curFixed.find(cow);
            if (it != curFixed.end()) {
                // Cow already has a fixed position
                int pos = it->second;
                if (pos < nextPos) {
                    valid = false;
                    break;
                }
                nextPos = pos + 1;
            } else {
                // Cow not fixed: find the smallest free position >= nextPos
                auto it2 = occupied.lower_bound(nextPos);
                int freePos = -1;
                if (it2 == occupied.end()) {
                    // No occupied position at or after nextPos, so use nextPos if <= n
                    if (nextPos <= n) {
                        freePos = nextPos;
                    }
                } else {
                    int firstOccupied = *it2;
                    if (nextPos < firstOccupied) {
                        freePos = nextPos;
                    } else {
                        // Need to find a gap; scan forward from nextPos
                        int probe = nextPos;
                        while (probe <= n && occupied.count(probe)) {
                            ++probe;
                        }
                        if (probe <= n) freePos = probe;
                    }
                }
                if (freePos == -1) {
                    valid = false;
                    break;
                }
                curFixed[cow] = freePos;
                occupied.insert(freePos);
                nextPos = freePos + 1;
            }
        }

        if (valid) {
            return h; // smallest valid h found
        }
    }
    return -1;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (in the same translation unit).
// This main provides tests.

int main() {
    // Example 1: Simple case, cow 1 can go at position 1
    {
        int n = 3;
        std::vector<int> hierarchy = {2, 3};
        std::vector<std::pair<int,int>> fixed;
        assert(earliestCowPosition(n, hierarchy, fixed) == 1);
    }

    // Example 2: Cow 1 cannot go at position 1 because cow 2 is fixed there
    {
        int n = 3;
        std::vector<int> hierarchy = {2, 3};
        std::vector<std::pair<int,int>> fixed = {{2,1}};
        // cow 1 can go at 2? Check: place cow1 at 2, cow2 at 1, cow3 at 3 -> hierarchy 2 then 3 OK => valid
        assert(earliestCowPosition(n, hierarchy, fixed) == 2);
    }

    // Example 3: Cow 1 already fixed
    {
        int n = 4;
        std::vector<int> hierarchy = {};
        std::vector<std::pair<int,int>> fixed = {{1,3}};
        assert(earliestCowPosition(n, hierarchy, fixed) == 3);
    }

    // Example 4: Impossible for cow 1 to be placed (hierarchy forces conflict)
    {
        int n = 2;
        std::vector<int> hierarchy = {2, 1, 3}; // cow 1 appears after cow 2, but cow1 must also be placed early?
        std::vector<std::pair<int,int>> fixed = {{2,1}, {3,2}};
        // cow1 must be placed somewhere, but positions 1 and 2 are taken, and cow1 is not fixed.
        // So impossible, return -1 (though overall placement of all cows might be impossible too, but we return -1 for cow1)
        // Here hierarchy includes cow1? Actually hierarchy can include any cows. This test is a bit contrived.
        // Let's use a simpler impossible case:
    }

    // Example 4 (corrected): All positions forced by fixed cows, no room for cow1
    {
        int n = 2;
        std::vector<int> hierarchy = {2}; // just cow 2
        std::vector<std::pair<int,int>> fixed = {{2,1}, {3,2}}; // but cow 3 not in n=2, ignore
        // Actually fixed includes cow 3 invalid, so let's fix:
        std::vector<std::pair<int,int>> fixed2 = {{2,1}}; // cow2 at pos1, pos2 free
        // cow1 can go at 2, hierarchy cow2 at 1 then nothing else, valid => return 2
        assert(earliestCowPosition(2, {2}, {{2,1}}) == 2);
    }

    // Example 5: No hierarchy, all positions fixed except one
    {
        int n = 5;
        std::vector<int> hierarchy;
        std::vector<std::pair<int,int>> fixed = {{2,1}, {3,2}, {4,4}, {5,5}}; // pos3 free
        // cow1 can go at pos3
        assert(earliestCowPosition(n, hierarchy, fixed) == 3);
    }

    // Example 6: Hierarchy prevents early placement
    {
        int n = 4;
        std::vector<int> hierarchy = {2, 1, 3}; // cow1 must come after cow2
        std::vector<std::pair<int,int>> fixed = {{2,1}}; // cow2 at pos1
        // cow1 cannot be at pos1 (taken by cow2). Try pos2: cow2 at1, cow1 at2, cow3? hierarchy: 2 (1) ,1(2),3(need after 2) -> cow3 can go at 3 => valid. So answer 2.
        assert(earliestCowPosition(n, hierarchy, fixed) == 2);
    }

    // Example 7: Impossible because hierarchy requires cow1 to be before a fixed cow that is earlier
    {
        int n = 3;
        std::vector<int> hierarchy = {1, 2}; // cow1 before cow2
        std::vector<std::pair<int,int>> fixed = {{2,1}}; // cow2 at pos1, so cow1 must be <1 impossible
        // No valid position for cow1, return -1
        assert(earliestCowPosition(n, hierarchy, fixed) == -1);
    }

    return 0;
}
