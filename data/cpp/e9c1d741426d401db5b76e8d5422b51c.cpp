// You are given an 8x8 chessboard represented as a grid, and the coordinates of a knight piece (x, y) where both coordinates are integers from 0 to 7 inclusive. Write a standalone C++ function that takes the knight's current position and a set of occupied squares (each represented as a pair of integers) from the same team (i.e., squares blocked by friendly pieces) and returns a sorted list (in ascending order first by x, then by y) of all valid legal moves the knight can make. A move is legal if the destination square is within the 8x8 board boundaries and is not occupied by a friendly piece. The knight moves in an L-shape: two squares in one direction and one square perpendicular (8 possible moves total). The function should be independent and not rely on any external game state—just the provided positions.
// The solution approach is straightforward: for each of the 8 possible knight offsets [(1,2), (2,1), (2,-1), (1,-2), (-1,-2), (-2,-1), (-2,1), (-1,2)] (or equivalent), compute the candidate destination (new_x, new_y). For each candidate, check if it is within the 0-7 boundary for both coordinates. If it is inside the board, check against the set of occupied friendly squares; if no friendly piece occupies that exact square, add it to the result list. After collecting all valid moves, sort the list of pairs (first by x, then by y) to produce deterministic output. Edge cases include positions near board edges where many candidates become invalid due to out-of-bound coordinates, and overlapping occupied squares which simply cause that candidate to be skipped. Time complexity is O(1) for generating the 8 candidates plus O(k log k) for sorting where k ≤ 8, effectively O(1) since k is constant. Space complexity is O(1) for the result (max 8 moves) plus O(n) to store the occupied set where n is the number of friendly pieces, if we store them in a set for quick lookup. Using a set of pairs gives O(1) average lookup per candidate.
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

// Return all legal knight moves from (x,y) given a set of occupied friendly squares.
// Occupied squares are provided as a set of (x,y) pairs.
std::vector<std::pair<int,int>> knightMoves(int x, int y, const std::set<std::pair<int,int>>& occupied) {
    // All 8 possible L-shaped offsets for a knight
    const int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    const int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};
    
    std::vector<std::pair<int,int>> result;
    
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // Check board boundaries (0..7 inclusive)
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            // Check not occupied by friendly piece
            if (occupied.find({nx, ny}) == occupied.end()) {
                result.emplace_back(nx, ny);
            }
        }
    }
    
    // Sort by x first, then by y (default pair comparison does this)
    std::sort(result.begin(), result.end());
    
    return result;
}
#include <cassert>
#include <vector>
#include <set>
#include <utility>

// Declare the function (already defined above in solution)
std::vector<std::pair<int,int>> knightMoves(int x, int y, const std::set<std::pair<int,int>>& occupied);

int main() {
    // Test 1: Corner position (0,0) with no occupied squares — should have 2 moves
    {
        std::set<std::pair<int,int>> empty;
        auto moves = knightMoves(0, 0, empty);
        std::vector<std::pair<int,int>> expected = {{1,2}, {2,1}};
        assert(moves == expected);
    }
    
    // Test 2: Center position (3,3) with no occupied — should have 8 moves
    {
        std::set<std::pair<int,int>> empty;
        auto moves = knightMoves(3, 3, empty);
        assert(moves.size() == 8);
        // Check a few known moves exist
        assert(moves[0] == std::make_pair(1,2));
        assert(moves[7] == std::make_pair(5,4)); // sorted by x, last is (5,4)
    }
    
    // Test 3: Blocked square — (4,4) with friend at (5,6) — that move should be excluded
    {
        std::set<std::pair<int,int>> occupied = {{5,6}};
        auto moves = knightMoves(4, 4, occupied);
        // From (4,4) the 8 moves are: (2,3),(2,5),(3,2),(3,6),(5,2),(5,6),(6,3),(6,5)
        // Excluding (5,6) leaves 7 moves
        assert(moves.size() == 7);
        for (const auto& m : moves) {
            assert(m != std::make_pair(5,6));
        }
    }
    
    // Test 4: Edge near top-right — (7,7) no occupied — only (5,6) and (6,5)
    {
        std::set<std::pair<int,int>> empty;
        auto moves = knightMoves(7, 7, empty);
        std::vector<std::pair<int,int>> expected = {{5,6}, {6,5}};
        assert(moves == expected);
    }
    
    // Test 5: Multiple occupied squares, ensure they are all excluded
    {
        std::set<std::pair<int,int>> occupied = {{2,3}, {2,5}, {3,2}, {3,6}, {5,2}, {5,6}, {6,3}, {6,5}};
        auto moves = knightMoves(4, 4, occupied);
        assert(moves.empty());
    }
    
    // Test 6: Sorted order verification — (0,3) no occupied
    {
        std::set<std::pair<int,int>> empty;
        auto moves = knightMoves(0, 3, empty);
        // From (0,3): (1,1),(1,5),(2,2),(2,4)
        std::vector<std::pair<int,int>> expected = {{1,1},{1,5},{2,2},{2,4}};
        assert(moves == expected);
    }
    
    return 0;
}
