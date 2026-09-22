Given two positive integers H and W, write a C++ function `solve` that prints (to standard output) a grid of size H × W consisting only of the characters `'#'` and `'.'`, such that every row and every column contains exactly one more `'#'` than `'.'`  — that is, the number of `#` in each row is `(W+1)/2` and in each column is `(H+1)/2` — with the additional constraint that no two `#` are adjacent horizontally, vertically, or diagonally. If such a grid does not exist for the given H and W, print `"Impossible"`. The function should not return a value; it outputs the grid or the word `"Impossible"` followed by a newline.
The problem is equivalent to placing a set of cells so that each row and column has a fixed count of marked cells (the majority count) while no two marked cells are adjacent even diagonally. This is a known combinatorial pattern: the only possible grids are those where H and W are both odd, because the required number of `#` per row is `(W+1)/2` and per column `(H+1)/2`. If both are odd, we can place `#` exactly on cells where (r + c) is even (checkerboard pattern). This pattern automatically gives each row and column the correct count because in an odd-length row, the pattern has one more even-indexed cell than odd-indexed, and similarly for columns. Adjacency is avoided because diagonal, vertical, and horizontal neighbors of an even-sum cell have odd sums, hence are all `.`. For example, for H=3, W=3: `#.#`, `.#.`, `#.#` works. If H or W is even, it is impossible to have the required counts while maintaining non-adjacency, because the checkerboard pattern would give equal counts in even-length dimensions, not majority. The algorithm simply checks if H and W are both odd; if yes, prints the checkerboard, else prints "Impossible". Time complexity O(HW) for output, space O(1) aside from output buffer.
#include <bits/stdc++.h>
using namespace std;

// Prints a valid HxW grid or "Impossible" if no such grid exists.
void solve(long long H, long long W) {
    if (H % 2 == 0 || W % 2 == 0) {
        cout << "Impossible\n";
        return;
    }
    for (long long r = 0; r < H; ++r) {
        for (long long c = 0; c < W; ++c) {
            // Place '#' when row+col is even (checkerboard).
            cout << (((r + c) % 2 == 0) ? '#' : '.');
        }
        cout << '\n';
    }
}
#include <bits/stdc++.h>
using namespace std;

// Declare the solution function (here for testing).
void solve(long long H, long long W);

// Helper to capture output of solve into a string for testing.
string captureSolve(long long H, long long W) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    solve(H, W);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Odd dimensions: valid checkerboard.
    assert(captureSolve(1, 1) == "#\n");
    assert(captureSolve(3, 3) == "#.#\n.#.\n#.#\n");
    assert(captureSolve(5, 1) == "#\n.\n#\n.\n#\n");
    assert(captureSolve(1, 5) == "#.#.#\n");
    
    // Even dimension: impossible.
    assert(captureSolve(2, 2) == "Impossible\n");
    assert(captureSolve(3, 4) == "Impossible\n");
    assert(captureSolve(4, 3) == "Impossible\n");
    assert(captureSolve(2, 5) == "Impossible\n");
    
    // Large odd dimension: just check first few lines pattern.
    string large = captureSolve(7, 7);
    assert(large.substr(0, 13) == "#.#.#.#\n.#.#.#.\n");
    
    return 0;
}
