/*
Write a C++ function `std::vector<std::vector<int>> sudokuDomainsFromClues(int p, int q, const std::vector<std::vector<int>>& clues)` that takes Sudoku dimensions `p` (block rows) and `q` (block columns) and a partially filled board `clues` of size `N = p*q` where `0` represents an empty cell. The function must return a board of the same dimensions where each cell contains a sorted vector of all possible values (1 to N) that the cell could take, subject to standard Sudoku constraints (no duplicate values in any row, column, or `p x q` block). For cells with a given clue (non-zero), the domain must contain only that single value. Cells with duplicate clues within the same row, column, or block, or with a value outside 1..N, must have an empty domain (indicating an unsolvable puzzle). The output is a `std::vector<std::vector<int>>` where each inner vector is the domain in ascending order.
*/

#include <vector>
#include <algorithm>
#include <cassert>

// Returns a board of domains for a Sudoku puzzle.
// p = block rows, q = block columns, board size = p*q.
// clues[r][c] = 0 means empty cell, otherwise fixed value 1..N.
// Domain list is sorted ascending; empty vector means no possible value.
std::vector<std::vector<int>> sudokuDomainsFromClues(
    int p, int q, const std::vector<std::vector<int>>& clues) {
    int N = p * q;
    // Validate board dimensions
    if (clues.empty() || clues.size() != static_cast<size_t>(N)) {
        return {};
    }
    for (const auto& row : clues) {
        if (row.size() != static_cast<size_t>(N)) {
            return {};
        }
    }

    // Initialize domains: full set for 0, singleton for valid clue, empty for invalid clue value
    std::vector<std::vector<int>> domains(N, std::vector<int>(N));
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int val = clues[r][c];
            if (val == 0) {
                domains[r][c].resize(N);
                for (int v = 1; v <= N; ++v) {
                    domains[r][c][v-1] = v;
                }
            } else if (val >= 1 && val <= N) {
                domains[r][c].push_back(val);
            } else {
                domains[r][c].clear(); // invalid clue value -> empty domain
            }
        }
    }

    // For each given clue, remove that value from peers in same row, column, and block
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int val = clues[r][c];
            if (val < 1 || val > N) continue; // skip invalid clues

            // Row elimination (except self)
            for (int cc = 0; cc < N; ++cc) {
                if (cc == c) continue;
                if (clues[r][cc] == val) {
                    domains[r][c].clear(); // duplicate clue in row -> conflict
                    domains[r][cc].clear();
                }
                // Remove val from peer domain if not fixed as clue
                if (clues[r][cc] == 0) {
                    auto& d = domains[r][cc];
                    auto it = std::find(d.begin(), d.end(), val);
                    if (it != d.end()) d.erase(it);
                }
            }

            // Column elimination (except self)
            for (int rr = 0; rr < N; ++rr) {
                if (rr == r) continue;
                if (clues[rr][c] == val) {
                    domains[r][c].clear();
                    domains[rr][c].clear();
                }
                if (clues[rr][c] == 0) {
                    auto& d = domains[rr][c];
                    auto it = std::find(d.begin(), d.end(), val);
                    if (it != d.end()) d.erase(it);
                }
            }

            // Block elimination (except self)
            int br = (r / p) * p;
            int bc = (c / q) * q;
            for (int rr = br; rr < br + p; ++rr) {
                for (int cc = bc; cc < bc + q; ++cc) {
                    if (rr == r && cc == c) continue;
                    if (clues[rr][cc] == val) {
                        domains[r][c].clear();
                        domains[rr][cc].clear();
                    }
                    if (clues[rr][cc] == 0) {
                        auto& d = domains[rr][cc];
                        auto it = std::find(d.begin(), d.end(), val);
                        if (it != d.end()) d.erase(it);
                    }
                }
            }
        }
    }

    // Ensure domains are sorted (they remain sorted after erasures, but just in case)
    for (auto& row : domains) {
        for (auto& d : row) {
            std::sort(d.begin(), d.end());
        }
    }

    return domains;
}

#include <cassert>
#include <vector>

// main function with test asserts
int main() {
    // Test 1: simple 2x2 Sudoku (p=1, q=2) with one clue
    std::vector<std::vector<int>> clues1 = {{1, 0}, {0, 0}};
    auto dom1 = sudokuDomainsFromClues(1, 2, clues1);
    // N=2, domains: (0,0)={1}, (0,1)={2}, (1,0)={2}, (1,1)={1,2}? Wait row/col constraints:
    // Actually for 1x2 blocks, N=2, valid puzzle: all rows/cols must contain 1,2.
    // Domain of (0,0)={1}, (0,1)={2} (row/col eliminations), (1,0)={2}, (1,1)={1}
    assert(dom1[0][0] == std::vector<int>({1}));
    assert(dom1[0][1] == std::vector<int>({2}));
    assert(dom1[1][0] == std::vector<int>({2}));
    assert(dom1[1][1] == std::vector<int>({1}));

    // Test 2: invalid duplicate in row leads to empty domain for the conflicting cell
    std::vector<std::vector<int>> clues2 = {{1, 1}, {0, 0}};
    auto dom2 = sudokuDomainsFromClues(1, 2, clues2);
    assert(dom2[0][0].empty());
    assert(dom2[0][1].empty());
    // Other cells should still have full or reduced domains, but for 1x2 with two 1s in row impossible
    // So we just check empty at the duplicate cells.

    // Test 3: clue value out of range -> treated as empty? Actually we return empty domain for that cell
    std::vector<std::vector<int>> clues3 = {{3, 0}, {0, 0}}; // N=2, 3 invalid
    auto dom3 = sudokuDomainsFromClues(1, 2, clues3);
    assert(dom3[0][0].empty()); // invalid value gives empty domain

    // Test 4: standard 3x3 Sudoku (p=3,q=3) with a few clues
    std::vector<std::vector<int>> clues4 = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    auto dom4 = sudokuDomainsFromClues(3, 3, clues4);
    // Check that given clues have singleton domain
    assert(dom4[0][0] == std::vector<int>({5}));
    assert(dom4[0][1] == std::vector<int>({3}));
    // Cell (0,2) is empty: valid values? Row 0 missing {1,2,4,6,8,9}, col 2 missing {1,2,3,4,5,7,8}, block (0-2,0-2) missing {1,2,4,5,6,7,8,9}
    // Intersection includes {1,2,4,8} (common), but also block constraints? Actually we compute by elimination from clues only, not full propagation.
    // After eliminating 5,3,7 from row, 6,9,8 from col, and block clues 5,3,6,9,8 etc. 
    // Let's just check it doesn't contain 5 or 3 or 7 or 6 or 9 or 8.
    auto& d02 = dom4[0][2];
    assert(std::find(d02.begin(), d02.end(), 5) == d02.end());
    assert(std::find(d02.begin(), d02.end(), 3) == d02.end());
    assert(std::find(d02.begin(), d02.end(), 6) == d02.end());
    assert(std::find(d02.begin(), d02.end(), 8) == d02.end());

    // Test 5: dimension mismatch returns empty board
    auto dom5 = sudokuDomainsFromClues(2, 2, {{1,0,0},{0,0,0}});
    assert(dom5.empty());

    // Test 6: all empty board returns full domains
    std::vector<std::vector<int>> clues6(4, std::vector<int>(4, 0));
    auto dom6 = sudokuDomainsFromClues(2, 2, clues6);
    for (int r=0; r<4; ++r) {
        for (int c=0; c<4; ++c) {
            assert(dom6[r][c] == std::vector<int>({1,2,3,4}));
        }
    }

    return 0;
}

// The approach builds the domain for each cell by starting with a full domain of {1, 2, ..., N} for empty cells and a singleton for given clues. Then, for each given clue value `v` at position (r, c), eliminate `v` from the domains of all other cells in the same row, column, and block that do not already have that value fixed. This repeated elimination across all clues ensures that any cell conflicting with a clue gets an empty domain if no value remains. The algorithm runs in O(N^2 * N) = O(N^3) because for each clue (at most N^2 cells) we iterate over up to 3*N other cells and remove at most one value. Space complexity is O(N^2 * N) = O(N^3) to store all domains, though typically each domain is size O(N). Edge cases: puzzle already invalid due to duplicate clues in a unit (row/col/block) → that cell's domain becomes empty after elimination; clue value out of range → treat as zero (empty) but the function still checks consistency; if p*q mismatch with board size, assert or handle gracefully by returning empty board. The final domains maintain sorted order naturally because we remove values from a sorted list.
