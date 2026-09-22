// Write a C++ function named `hanoiMoves` that takes three integer arguments: `n` (the number of discs), `source` (the starting peg number), and `destination` (the target peg number), and returns a `std::vector<std::string>` containing the sequence of moves required to solve the Towers of Hanoi puzzle, with each move formatted as `"Move a disc from peg X to peg Y"` (where X and Y are the peg numbers). The function must assume that a third peg (the auxiliary peg) is the peg that is neither `source` nor `destination` among pegs 1, 2, and 3. The input `n` is guaranteed to be a non-negative integer. If `n` is 0, the function should return an empty vector. The function must handle any valid assignment of source and destination (i.e., source and destination must be distinct integers between 1 and 3 inclusive). The returned move order must follow the classic recursive Towers of Hanoi algorithm.

The solution uses the standard recursive Towers of Hanoi algorithm. The key insight is that to move `n` discs from `source` to `destination` using a temporary peg, you first recursively move `n-1` discs from `source` to the temporary peg (which is the peg not equal to `source` or `destination`), then move the largest disc directly from `source` to `destination`, and finally recursively move the `n-1` discs from the temporary peg to `destination`. The auxiliary peg for each recursive call is determined as the peg that is neither the current source nor the current destination. Since pegs are numbered 1, 2, 3, the auxiliary can be computed as `6 - source - destination` (since 1+2+3=6). Base case: when `n == 0`, return an empty vector. Edge cases: `n=0` returns empty; `source` and `destination` are distinct (guaranteed by specification), and the algorithm automatically handles reversed source/destination (e.g., source=3, destination=1) because the temporary peg is correctly computed. Time complexity: The number of moves is \(2^n - 1\), so the algorithm runs in \(O(2^n)\) time and uses \(O(n)\) call-stack space, but the returned vector itself holds \(O(2^n)\) strings, so total space is \(O(2^n)\) for the output. For each recursive call, we append to the vector, which is efficient if we pass the vector by reference.

#include <vector>
#include <string>

// Returns a vector of move strings for the Towers of Hanoi puzzle.
// Moves n discs from 'source' to 'destination' using the remaining peg as auxiliary.
std::vector<std::string> hanoiMoves(int n, int source, int destination) {
    std::vector<std::string> moves;
    // Recursive helper that appends moves to the vector.
    // Uses a lambda to avoid global state.
    std::function<void(int, int, int)> helper = [&](int num, int src, int dst) {
        if (num <= 0) return;
        int aux = 6 - src - dst;  // Peg number not equal to src or dst.
        helper(num - 1, src, aux);
        moves.push_back("Move a disc from peg " + std::to_string(src) + " to peg " + std::to_string(dst));
        helper(num - 1, aux, dst);
    };
    helper(n, source, destination);
    return moves;
}
Note: The code uses `std::function` which requires including `<functional>`. For a cleaner solution without `std::function`, one can define a private recursive function or an inner lambda with an explicit capture of `moves`. The above uses a lambda with `std::function` for self-containment. For completeness, include `<functional>` as well. The final output code below includes all necessary headers.

#include <cassert>
#include <vector>
#include <string>
#include <functional>

// The solution function (copy from above but with full headers)
std::vector<std::string> hanoiMoves(int n, int source, int destination) {
    std::vector<std::string> moves;
    std::function<void(int, int, int)> helper = [&](int num, int src, int dst) {
        if (num <= 0) return;
        int aux = 6 - src - dst;
        helper(num - 1, src, aux);
        moves.push_back("Move a disc from peg " + std::to_string(src) + " to peg " + std::to_string(dst));
        helper(num - 1, aux, dst);
    };
    helper(n, source, destination);
    return moves;
}

int main() {
    // Test n=0: empty vector.
    assert(hanoiMoves(0, 1, 3).empty());

    // Test n=1: single move.
    auto m1 = hanoiMoves(1, 1, 3);
    assert(m1.size() == 1);
    assert(m1[0] == "Move a disc from peg 1 to peg 3");

    // Test n=2 with source=1, dest=2 (aux=3).
    auto m2 = hanoiMoves(2, 1, 2);
    assert(m2.size() == 3);
    assert(m2[0] == "Move a disc from peg 1 to peg 3");
    assert(m2[1] == "Move a disc from peg 1 to peg 2");
    assert(m2[2] == "Move a disc from peg 3 to peg 2");

    // Test n=3 with source=1, dest=3 (classic).
    auto m3 = hanoiMoves(3, 1, 3);
    assert(m3.size() == 7);
    assert(m3[0] == "Move a disc from peg 1 to peg 3");
    assert(m3[1] == "Move a disc from peg 1 to peg 2");
    assert(m3[2] == "Move a disc from peg 3 to peg 2");
    assert(m3[3] == "Move a disc from peg 1 to peg 3");
    assert(m3[4] == "Move a disc from peg 2 to peg 1");
    assert(m3[5] == "Move a disc from peg 2 to peg 3");
    assert(m3[6] == "Move a disc from peg 1 to peg 3");

    // Test reversed source/destination: n=2, source=3, dest=1 (aux=2).
    auto m4 = hanoiMoves(2, 3, 1);
    assert(m4.size() == 3);
    assert(m4[0] == "Move a disc from peg 3 to peg 2");
    assert(m4[1] == "Move a disc from peg 3 to peg 1");
    assert(m4[2] == "Move a disc from peg 2 to peg 1");

    // Test n=3 with source=2, dest=1 (aux=3).
    auto m5 = hanoiMoves(3, 2, 1);
    assert(m5.size() == 7);
    assert(m5[0] == "Move a disc from peg 2 to peg 1");
    assert(m5[6] == "Move a disc from peg 2 to peg 1"); // Last move is from 2 to 1

    return 0;
}
