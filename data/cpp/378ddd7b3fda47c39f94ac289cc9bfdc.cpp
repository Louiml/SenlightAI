Write a standalone C++ function that, given a rectangular grid with a start cell at coordinates `(srow, scol)` and a target cell at `(erow, ecol)` (using 1-based indexing, with rows increasing downward and columns increasing to the right), returns a vector of all unique paths from start to target, where each path is represented as a string consisting of only two characters: `'h'` for a move one column to the right and `'v'` for a move one row down. Paths are generated in a specific order: all horizontal-first (i.e., all paths that start with `'h'` when possible) then all vertical-first moves, recursively. The function must handle the edge case where start and target are the same cell by returning a single empty string `""` as the only path (representing zero moves). The function should be `const`-correct and work for any valid coordinates where `srow <= erow` and `scol <= ecol`. You do not need to validate inputs; assume they are always valid (non-negative differences).
#include <cassert>
#include <vector>
#include <string>

// Required function declaration (assume the solution function is defined above or in the same file)
std::vector<std::string> getAllMazePaths(int, int, int, int);

int main() {
    // Same start and end -> only empty path
    std::vector<std::string> p1 = getAllMazePaths(2, 3, 2, 3);
    assert(p1.size() == 1);
    assert(p1[0] == "");

    // One step right
    std::vector<std::string> p2 = getAllMazePaths(1, 1, 1, 2);
    assert(p2.size() == 1);
    assert(p2[0] == "h");

    // One step down
    std::vector<std::string> p3 = getAllMazePaths(1, 1, 2, 1);
    assert(p3.size() == 1);
    assert(p3[0] == "v");

    // 2x2 grid from (1,1) to (2,2): paths: h v, v h
    std::vector<std::string> p4 = getAllMazePaths(1, 1, 2, 2);
    assert(p4.size() == 2);
    assert(p4[0] == "hv");
    assert(p4[1] == "vh");

    // 3x3 grid from (1,1) to (3,3): exactly 6 paths, lexicographic order
    std::vector<std::string> p5 = getAllMazePaths(1, 1, 3, 3);
    assert(p5.size() == 6);
    std::vector<std::string> expected5 = {"hhvv", "hvhv", "hvvh", "vhhv", "vhvh", "vvhh"};
    for (size_t i = 0; i < expected5.size(); ++i) {
        assert(p5[i] == expected5[i]);
    }

    // Rectangular 2x3 grid from (1,1) to (2,3): paths = C(3+1,1)=4
    std::vector<std::string> p6 = getAllMazePaths(1, 1, 2, 3);
    assert(p6.size() == 4);
    std::vector<std::string> expected6 = {"hhv", "hvh", "vhh"};
    // Wait: actually from (1,1) to (2,3): down (1) and right (2) => length 3, number = 3!/(1!2!)=3
    assert(p6.size() == 3);
    // Order check: all starting with 'h' then 'v'
    assert(p6[0] == "hhv");
    assert(p6[1] == "hvh");
    assert(p6[2] == "vhh");

    // Larger 4x4 from (1,1) to (4,4): verifies count only
    std::vector<std::string> p7 = getAllMazePaths(1, 1, 4, 4);
    assert(p7.size() == 20); // C(6,3)=20

    return 0;
}
#include <vector>
#include <string>

// Returns all monotonic paths (only 'h' right and 'v' down moves) from (srow, scol) to (erow, ecol).
// Paths are produced in lexicographic order (all 'h' prefixed before 'v' prefixed, recursively).
std::vector<std::string> getAllMazePaths(int srow, int scol, int erow, int ecol) {
    // Base case: already at target, exactly one empty path
    if (srow == erow && scol == ecol) {
        return {""};
    }

    std::vector<std::string> totalPaths;

    // Move right (horizontal) if possible
    if (scol < ecol) {
        std::vector<std::string> rightPaths = getAllMazePaths(srow, scol + 1, erow, ecol);
        for (const std::string& path : rightPaths) {
            totalPaths.push_back("h" + path);
        }
    }

    // Move down (vertical) if possible
    if (srow < erow) {
        std::vector<std::string> downPaths = getAllMazePaths(srow + 1, scol, erow, ecol);
        for (const std::string& path : downPaths) {
            totalPaths.push_back("v" + path);
        }
    }

    return totalPaths;
}
// The problem is a classic enumeration of monotonic lattice paths from one grid point to another, where only right (h) and down (v) moves are allowed. The solution uses recursion:  
// - Base case: if the current cell is the target, return a vector containing a single empty string (indicating one valid path with no further moves).  
// - Recursive step: from the current cell `(r, c)`, we can move right to `(r, c+1)` if `c < ecol`, and we can move down to `(r+1, c)` if `r < erow`. For each possible move, we recursively compute all paths from the new cell to the target, and prepend the corresponding character (`'h'` or `'v'`) to each returned path.  
// - Ordering: because we process the right move first (and append its results before the down move’s results), the final vector is sorted in lexicographic order where all `'h'`-prefixed paths come before all `'v'`-prefixed paths, and the same ordering applies recursively inside each subtree.  
// Edge cases:  
// - If start equals target, the base case directly returns `{""}`.  
// - If only one move is possible (e.g., only right moves remain), recursion will handle it naturally.  
// Time complexity: The number of distinct paths is \(C((erow-srow)+(ecol-scol), erow-srow)\), which is exponential in the sum of distances. Each path of length \(L = (erow-srow)+(ecol-scol)\) requires \(O(L)\) time to construct (due to string concatenation), so total time is \(O(L \cdot \text{number of paths})\). Space complexity is also \(O(L \cdot \text{number of paths})\) to store the result vector, plus recursion stack depth \(O(L)\).  
// The implementation should use reference parameters to avoid unnecessary copying, but the return-by-value approach is fine for a standalone function. To achieve const-correctness, all parameters are passed by value (or const reference for the result vector if needed), and the function itself is not modifying any external state.
