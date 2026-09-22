// Given integers K, C, and S, write a C++ function `std::string solveFractiles(int K, int C, int S)` that returns a space-separated list of exactly S positive integers (each between 1 and K inclusive) representing the positions of tiles to check in the fractal artwork, in a format suitable for the "Fractiles" problem. The function must return `"IMPOSSIBLE"` if no valid set of S positions exists given the constraints (where a valid solution requires at least S positions such that S*C >= K). If a solution exists, return the positions as `1 2 3 ... S` (the first S integers starting from 1), space-separated, with no trailing space. The function must handle K up to 200, C up to 200, and S up to 200, and must be deterministic.

The given snippet is a brute-force placeholder that just prints positions 1 through K regardless of constraints, which is incorrect. The real "Fractiles" problem asks: given K (original tile count), C (complexity/levels of recursion), and S (number of tiles you can inspect), determine if you can identify whether any original tile is gold. The known solution: you need to check at least `ceil(K/C)` tiles. If S >= ceil(K/C), a solution exists. The simplest valid set of positions is the first S integers from 1 to K (since the snippet shows that pattern, and any set of S distinct positions works as long as S*C >= K). Actually, the standard solution constructs specific positions, but the problem in this task is simplified: we only need to output `1 2 ... S` if `S*C >= K`, otherwise `"IMPOSSIBLE"`. The edge case is when C or S could be 0? But constraints say positive. If S*C >= K, then output positions 1 through S. If not, output "IMPOSSIBLE". Complexity is O(S) time and O(S) space for the output string. We also need to handle formatting carefully: no trailing space, and use stringstream or manual concatenation.

#include <string>
#include <sstream>

// Returns a string with the required tile positions, or "IMPOSSIBLE" if not enough tiles can be checked.
// Given K original tiles, C complexity, and S maximum checks, we can succeed iff S*C >= K.
// If possible, we output positions 1 through S (the simplest valid choice).
std::string solveFractiles(int K, int C, int S) {
    // Check if we can cover all K original tiles with S checks of depth C.
    // Each check can reveal up to C original positions (in the recursive fractal),
    // so total coverage is S*C. Need S*C >= K.
    if (static_cast<long long>(S) * C < K) {
        return "IMPOSSIBLE";
    }
    
    // Build the output string: positions 1 through S, space-separated.
    std::ostringstream output;
    for (int i = 1; i <= S; ++i) {
        if (i > 1) {
            output << " ";
        }
        output << i;
    }
    
    return output.str();
}

#include <cassert>
#include <string>

// Declaration of the function to test (assume it's included from the solution)
std::string solveFractiles(int K, int C, int S);

int main() {
    // Basic cases where solution exists
    assert(solveFractiles(1, 1, 1) == "1");
    assert(solveFractiles(2, 1, 2) == "1 2");
    assert(solveFractiles(3, 2, 2) == "1 2");  // S*C = 4 >= 3
    assert(solveFractiles(5, 3, 2) == "1 2");  // S*C = 6 >= 5
    
    // Edge case where S*C == K exactly
    assert(solveFractiles(4, 2, 2) == "1 2");
    
    // Impossible cases
    assert(solveFractiles(3, 1, 2) == "IMPOSSIBLE");  // 2*1=2 < 3
    assert(solveFractiles(5, 2, 2) == "IMPOSSIBLE");  // 4 < 5
    assert(solveFractiles(10, 3, 3) == "IMPOSSIBLE"); // 9 < 10
    
    // Larger case, S > K but still valid? Here K=2, C=1, S=3 -> coverage 3 >= 2, output "1 2 3"
    assert(solveFractiles(2, 1, 3) == "1 2 3");
    
    // Maximum boundary: K=200, C=200, S=1 -> 200 >= 200, valid
    assert(solveFractiles(200, 200, 1) == "1");
    
    // K=200, C=1, S=199 -> 199 < 200, impossible
    assert(solveFractiles(200, 1, 199) == "IMPOSSIBLE");
    
    return 0;
}
