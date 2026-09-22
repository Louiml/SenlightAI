Write a C++ function `int longestUniquePath(const std::vector<std::string>& board)` that takes a rectangular grid of uppercase letters (each row a string, at least 1x1, at most 20x20) and returns the length of the longest path you can walk starting from the top-left cell (0,0). You may move up, down, left, or right, but you cannot visit a cell whose letter has already appeared earlier in the path (i.e., all letters in the path must be distinct). The path length is the number of cells visited (including the start). The function must handle grids where the start cell itself makes a valid one‑cell path. If a move leads to a duplicate letter, that move is not allowed (you stop before it). The function must not modify the input.
The problem is a classic DFS with backtracking through a state representing the set of letters already used on the current path. Since the alphabet is only 26 letters, we can track used letters with a boolean array or bitmask. Start DFS from (0,0) with an initial count of 1, marking the starting letter as used. At each step, try all four directions; if the neighbor is inside bounds and its letter is not yet used, recurse with count+1 and mark that letter temporarily. After the recursion, unmark the letter for backtracking. Return the maximum count encountered. Edge cases: a 1x1 grid returns 1; grids where all letters are the same return 1; grids where a long winding path exists but revisits letters must backtrack. Time complexity is O(4^L) in the worst case where L is the number of cells, but with the distinct‑letter constraint the practical limit is bounded by the number of distinct letters (≤26) visited in a path, so worst‑case is O(26 * 4^26) theoretically, but for a 20x20 grid the search prunes heavily; typical DFS with backtracking runs fast. Auxiliary space is O(26) for the used array plus O(L) for recursion stack depth, at most 26.
#include <vector>
#include <string>
#include <algorithm>

// Returns the length of the longest path from (0,0) where all visited letters are distinct.
int longestUniquePath(const std::vector<std::string>& board) {
    if (board.empty() || board[0].empty()) return 0;
    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());
    bool used[26] = {false};
    int best = 0;
    
    // Depth-first search with backtracking.
    // r, c: current position; depth: number of cells visited so far (including current)
    void dfs(int r, int c, int depth) {
        int idx = board[r][c] - 'A';
        used[idx] = true;
        best = std::max(best, depth);
        
        // Explore four directions
        const int dr[4] = {-1, 1, 0, 0};
        const int dc[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                int nidx = board[nr][nc] - 'A';
                if (!used[nidx]) {
                    dfs(nr, nc, depth + 1);
                }
            }
        }
        used[idx] = false; // backtrack
    }
    
    dfs(0, 0, 1);
    return best;
}
#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be included above)

int main() {
    // 1x1 grid
    assert(longestUniquePath({"A"}) == 1);
    // All same letter, only start cell possible
    assert(longestUniquePath({"AAA", "AAA", "AAA"}) == 1);
    // 1x2 with distinct letters: A->B
    assert(longestUniquePath({"AB"}) == 2);
    // 1x2 with same letters
    assert(longestUniquePath({"AA"}) == 1);
    // 2x2 all distinct: A B / C D -> path A->B->D->C (4)
    assert(longestUniquePath({"AB", "CD"}) == 4);
    // 2x2 with a repeated letter: A B / B C -> max path A->B->C is 3
    assert(longestUniquePath({"AB", "BC"}) == 3);
    // Larger grid where a winding path exists but is blocked by duplicates
    // Example: A B C / B C D / C D E -> B and C appear multiple times, but longest distinct path is A->B->C->D->E (5)
    assert(longestUniquePath({"ABC", "BCD", "CDE"}) == 5);
    // Test with a dead end: A B / C B -> from A can go to B or C, but B repeats, so path A->B (2) or A->C->B? C then B is distinct? A,C,B: A and C and B are distinct, that's 3.
    assert(longestUniquePath({"AB", "CB"}) == 3);
    // Test wrap-around: all 26 distinct letters in 5x5 grid with one extra? Not needed; basic checks suffice.
    // Test width 1
    assert(longestUniquePath({"A", "B", "C"}) == 3);
    // Test where best path requires backtracking: A B / A C -> from A can go right to B, or down to A (invalid), so only A->B (2) or A->C (2), max 2
    assert(longestUniquePath({"AB", "AC"}) == 2);

    return 0;
}
