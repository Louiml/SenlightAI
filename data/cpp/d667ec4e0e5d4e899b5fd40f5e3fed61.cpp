// Write a C++ function `int towerOfHanoiSteps(int n, int source, int destination, vector<pair<int,int>>& moves)` that, given the number of rings `n`, a source peg number, and a destination peg number (both from 1 to 3), appends to `moves` the sequence of moves (each move as a pair of peg numbers: from, to) needed to solve the Tower of Hanoi puzzle. The function must return the total number of moves (which is `2^n - 1`). The pegs are numbered 1, 2, 3, and only one ring may be moved at a time, with larger rings never placed on top of smaller ones. The function should work for any `n ≥ 1` and any valid source/destination pegs (they may be the same, in which case no moves should be generated and the return value should be 0). Use recursion to implement the classic algorithm.

#include <cassert>
#include <vector>
#include <utility>

int towerOfHanoiSteps(int, int, int, std::vector<std::pair<int,int>>&);

int main() {
    // Test n=1 simple move
    std::vector<std::pair<int,int>> moves;
    assert(towerOfHanoiSteps(1, 1, 3, moves) == 1);
    assert(moves.size() == 1);
    assert(moves[0] == std::make_pair(1, 3));

    // Test n=2 standard solution
    moves.clear();
    assert(towerOfHanoiSteps(2, 1, 3, moves) == 3);
    assert(moves.size() == 3);
    assert(moves[0] == std::make_pair(1, 2));
    assert(moves[1] == std::make_pair(1, 3));
    assert(moves[2] == std::make_pair(2, 3));

    // Test n=3 classic 7 moves
    moves.clear();
    assert(towerOfHanoiSteps(3, 1, 3, moves) == 7);
    assert(moves.size() == 7);
    // Check first, middle, last
    assert(moves[0] == std::make_pair(1, 3));
    assert(moves[3] == std::make_pair(1, 3));
    assert(moves[6] == std::make_pair(1, 3));

    // Test n=0
    moves.clear();
    assert(towerOfHanoiSteps(0, 1, 3, moves) == 0);
    assert(moves.empty());

    // Test source == destination
    moves.clear();
    assert(towerOfHanoiSteps(5, 2, 2, moves) == 0);
    assert(moves.empty());

    // Test different source/destination (1->2 with n=3)
    moves.clear();
    assert(towerOfHanoiSteps(3, 1, 2, moves) == 7);
    assert(moves[0] == std::make_pair(1, 3));
    assert(moves[6] == std::make_pair(3, 2));

    // Test n=4 total moves count
    moves.clear();
    assert(towerOfHanoiSteps(4, 1, 3, moves) == 15);
    assert(moves.size() == 15);

    // Test n=5, verify first move always goes to auxiliary peg (3) when dest=3
    moves.clear();
    towerOfHanoiSteps(5, 1, 3, moves);
    assert(moves.front() == std::make_pair(1, 3));
    assert(moves.back() == std::make_pair(1, 3));

    return 0;
}

#include <vector>
#include <utility>

// Appends moves for Tower of Hanoi with n rings from source to destination.
// Returns the number of moves appended (0 if n == 0 or source == destination).
int towerOfHanoiSteps(int n, int source, int destination, std::vector<std::pair<int,int>>& moves) {
    if (n == 0 || source == destination) {
        return 0;
    }
    if (n == 1) {
        moves.push_back({source, destination});
        return 1;
    }
    int other = 6 - (source + destination); // third peg
    int count = towerOfHanoiSteps(n - 1, source, other, moves);
    moves.push_back({source, destination});
    count += 1;
    count += towerOfHanoiSteps(n - 1, other, destination, moves);
    return count;
}

// The classic recursive solution for Tower of Hanoi: to move `n` rings from `st` to `ed`, first move the top `n-1` rings from `st` to the auxiliary peg (the third peg, computed as `6 - st - ed`), then move the largest ring directly from `st` to `ed`, then move the `n-1` rings from the auxiliary peg to `ed`. Base case: if `n == 0` or `st == ed`, do nothing. The function appends each move (as a `pair<int,int>`) to the vector and returns `2^n - 1` when `st != ed` and `n ≥ 1`. Edge cases: `n = 0` returns 0 moves; `st == ed` returns 0 moves; duplicate moves should not be generated when source equals destination. Time complexity is \(O(2^n)\) because each move is generated exactly once; space complexity is \(O(n)\) for recursion depth plus \(O(2^n)\) for the output vector (unavoidable). The algorithm handles all valid inputs and avoids unnecessary recursion when source equals destination.
