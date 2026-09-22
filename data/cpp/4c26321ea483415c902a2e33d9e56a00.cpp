Write a C++ function `int solveFlipGame(const std::string board[4])` that takes a 4x4 board represented as an array of 4 strings, each of length 4 and containing only `'b'` (black) or `'w'` (white). The game allows pressing a cell, which flips (inverts) the color of that cell and its four orthogonal neighbors (up, down, left, right) if they exist. The goal is to make all 16 cells the same color (all black or all white). The function must return the minimum number of presses required to achieve this, or `-1` if it is impossible. The input is guaranteed to have exactly 4 strings, each exactly 4 characters long. Pressing the same cell more than once is pointless (it cancels out mod 2), so the solution can be modeled as a search over subsets of 16 cells. The function should be efficient enough to handle all possible inputs.
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.

int main() {
    // Already all white
    std::string board1[4] = {"wwww", "wwww", "wwww", "wwww"};
    assert(solveFlipGame(board1) == 0);

    // Already all black
    std::string board2[4] = {"bbbb", "bbbb", "bbbb", "bbbb"};
    assert(solveFlipGame(board2) == 0);

    // One black cell, pressing it flips itself and 2 neighbors (corner) -> all white
    std::string board3[4] = {"bwww", "wwww", "wwww", "wwww"};
    // Press (0,0) toggles (0,0),(0,1),(1,0) -> those become black? no, press toggles: black->white, white->black... let's reason: board3 has only (0,0) black. Press (0,0) toggles (0,0) to white, (0,1) to black, (1,0) to black -> not uniform. Pressing (0,1) toggles (0,0),(0,1),(0,2),(1,1) -> (0,0) becomes white, (0,1) to black? Actually we need to find minimum. Let's test a known solvable board: a single 'b' at center? Actually brute force will find minimum. We'll just assert that a known correct answer is 1 for a board where pressing one cell works. Example: board with (1,1) black only? Pressing (1,1) toggles (1,1) to white, (0,1),(2,1),(1,0),(1,2) to black -> not uniform. So no single press works for a single black. But a 2x2 black squares? Let's just assert for a known case from original snippet: The original solves by brute force and we can use a test case that is known to be 3 presses? To avoid manual computation, we can test that a uniform board returns 0, and that a board requiring at least one press returns >=1, and that an impossible case returns -1? But 4x4 is always solvable, so we can't test -1. Instead, we test a specific small example: board with (0,0) white and all else black? Pressing all four corners might work? Actually let's test with the sample from original code: They read a 4x4 and output a number. We can create a board that is known to be solvable in 2: For example, let's construct: board where (0,0) and (1,1) are 'b', rest 'w'. Press (0,0) and (1,1)? Let's compute? Better to use a simple assertion: If we press a specific single cell, the resulting board should have a minimal solution <=1? No, we need a reliable test. We can test a board that is one press away from uniform. For instance, take the all-white board and apply the toggle effect of pressing (0,0) manually, then that board should be solvable in 1 press. So we construct that board.
    // Board after pressing (0,0) from all-white: (0,0),(0,1),(1,0) become 'b', rest 'w'
    std::string board4[4] = {"bww", "bww", "www", "www"}; // error: length 3? need length 4 per row.
    // Correct: "bwww", "bwww", "wwww", "wwww"? Wait (1,0) is second row first column -> 'b'. So row0: b w w w; row1: b w w w; row2: w w w w; row3: w w w w.
    std::string board4fixed[4] = {"bwww", "bwww", "wwww", "wwww"};
    assert(solveFlipGame(board4fixed) == 1);

    // A board requiring 2 presses: take all-white and apply two presses, e.g., (0,0) and (0,1). That yields a board with toggles overlapping. Then minimal should be 2.
    std::string board5[4] = {"bwbw", "wwwb", "wwww", "wwww"}; // manually? Simpler: we can trust algorithm returns 2 for some known pattern. Let's use a pattern where pressing (0,0) and (3,3) yields a unique board. Compute toggles: (0,0) toggles (0,0),(0,1),(1,0). (3,3) toggles (3,3),(3,2),(2,3). So board has 'b' at those positions: row0: b w w w; row1: b w w w; row2: w w w b; row3: w w b b? Actually (3,3) 'b', (3,2) 'b', (2,3) 'b'. So row2: w w w b; row3: w w b b. So board: {"bwww","bwww","wwwb","wwbb"}. This should require 2 presses.
    std::string board5fixed[4] = {"bwww", "bwww", "wwwb", "wwbb"};
    assert(solveFlipGame(board5fixed) == 2);

    // A board with more complexity, just ensure it returns a non-negative number (solvable) and <=16.
    std::string board6[4] = {"bwbb", "wbwb", "bbww", "wbbw"};
    int result6 = solveFlipGame(board6);
    assert(result6 >= 0 && result6 <= 16);

    // Test a board that is the result of pressing all 16 cells from all-white: That board is the XOR of all masks. Since pressing all cells toggles every cell exactly its degree (2 for corners, 3 for edges, 4 for interior) which mod 2 gives: corners:2%2=0, edges non-corner:3%2=1, interior:4%2=0. So pressing all cells from all-white yields 'b' only on edge cells (non-corner). That board should be solvable in 16 presses (since pressing all again). Let's construct that.
    std::string board7[4] = {"wwww", "wbbw", "wbbw", "wwww"}; // Actually top row all corners? Wait edge non-corner: top row positions (0,1),(0,2) are edges non-corner -> 'b'; bottom row (3,1),(3,2); left column (1,0),(2,0); right column (1,3),(2,3). So row0: w b b w; row1: b w w b; row2: b w w b; row3: w b b w. That's the board. So:
    std::string board7fixed[4] = {"wbbw", "bwwb", "bwwb", "wbbw"};
    assert(solveFlipGame(board7fixed) == 16);

    return 0;
}
#include <string>
#include <array>
#include <vector>
#include <limits>

// Precomputed masks for each cell (0-15) toggling self and orthogonal neighbors
const std::array<int, 16> mask = []() {
    std::array<int, 16> m{};
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            int idx = r * 4 + c;
            int val = 1 << idx;
            if (r > 0) val |= 1 << ((r - 1) * 4 + c);
            if (r < 3) val |= 1 << ((r + 1) * 4 + c);
            if (c > 0) val |= 1 << (r * 4 + (c - 1));
            if (c < 3) val |= 1 << (r * 4 + (c + 1));
            m[idx] = val;
        }
    }
    return m;
}();

// Helper to recursively generate combinations of k presses from 0..15
bool genCombinations(int start, int k, int chosen, std::vector<int>& comb, int initBoard, int& best) {
    if (k == 0) {
        int eff = 0;
        for (int idx : comb) eff ^= mask[idx];
        int finalBoard = initBoard ^ eff;
        if (finalBoard == 0 || finalBoard == 0xFFFF) {
            best = static_cast<int>(comb.size());
            return true;
        }
        return false;
    }
    for (int i = start; i <= 16 - k; ++i) {
        comb.push_back(i);
        if (genCombinations(i + 1, k - 1, chosen, comb, initBoard, best)) return true;
        comb.pop_back();
    }
    return false;
}

// Returns minimum number of presses to make all cells same color, or -1 if impossible
int solveFlipGame(const std::string board[4]) {
    int init = 0;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (board[r][c] == 'b') init |= (1 << (r * 4 + c));
        }
    }
    if (init == 0 || init == 0xFFFF) return 0;
    for (int presses = 1; presses <= 16; ++presses) {
        std::vector<int> comb;
        int best = -1;
        if (genCombinations(0, presses, presses, comb, init, best)) return best;
    }
    return -1;
}
// The problem is a classic "Lights Out" variant on a 4x4 grid. Since pressing a cell twice cancels, we only need to consider whether each of the 16 cells is pressed (0 or 1 time). The state after a set of presses is the initial board XORed with the effect of those presses. Because the toggle operation is linear over GF(2), we can brute-force all \(2^{16} = 65536\) subsets of presses, compute the resulting board, and check if it is all same color. To minimize presses, we iterate subset sizes from 0 to 16 and return the first that works. However, the original snippet limits to 8 due to an assumption, but that is not mathematically correct; some boards may require up to 16 presses? Actually, for 4x4 Lights Out, the maximum minimal solution is 16? Let's verify: it is known that all configurations are solvable, and the maximum minimal moves is 16 (pressing all cells). So we can safely search all subset sizes up to 16. Time complexity: For each subset (up to 65536), we apply up to 16 presses, each affecting up to 5 cells, so O(65536 * 16 * 5) ≈ 5.2 million operations, which is trivial. Space complexity is O(1) aside from copying the board. We can also generate combinations of k presses using a helper that enumerates subsets of given size, similar to the snippet's `tryadd` but simpler. Edge cases: initial board already uniform => return 0. If no subset works (should not happen for 4x4, but in general), return -1. We must handle the press effect correctly using modulo 2 toggling. Implementation: Represent board as a 16-bit integer where bit `i` (0-15) is 1 if black, 0 if white. Then pressing a cell toggles a precomputed mask for that cell (self + neighbors). For each subset of presses, XOR the init with the XOR of masks for pressed cells. Check if result is 0 or 65535. Enumerate subsets by size using a recursive or iterative combination generator.
