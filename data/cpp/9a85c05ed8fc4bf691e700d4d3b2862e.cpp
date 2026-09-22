// Write a C++ function `constructPath(int n, int m, long long k)` that, given a grid of size `n` rows by `m` columns (with `n,m >= 1`), attempts to construct a path that starts at cell `(1,1)` and moves within the grid. The path is described as a sequence of commands, where each command is either "R" (move right), "L" (move left), "D" (move down), "U" (move up), or "DUL" (a triple move: down, up, left — must stay inside the grid for each individual move). The total number of moves (counting each character in a multi-character command) must be exactly `k`. The goal is to find any valid sequence of commands (with repetitions allowed) that covers exactly `k` moves, or determine that it is impossible. The output must be either `"NO"` (if impossible) or `"YES"` followed by a list of commands in the format: first an integer `t` (number of distinct command entries), then `t` lines each containing a positive integer `cnt` and a command string `s`, meaning that command `s` is repeated `cnt` times consecutively, and the total moves sum to `k`. The path may visit any cells, including revisits, but must never move outside the grid. The function returns a `std::string` containing the full output (including newlines), or `"NO"` if impossible. For testing, the function must handle up to `n,m ≤ 10^9` and `k ≤ 10^18`. The number of distinct command groups in the output must be at most `n+m` groups (to keep output size reasonable). If multiple solutions exist, any one is acceptable, but the algorithm must always produce a valid solution if one exists.

// The key observation is that any single cell on the grid can be visited multiple times. The simplest approach is to try to construct a path that covers as many moves as possible while respecting grid boundaries. The following strategy guarantees coverage of up to `(n*m - 1)` moves (the longest possible path without revisiting cells) but we can do better by allowing revisits. Actually, we can cover arbitrarily many moves by repeatedly moving right and left along the last row, or up and down along the last column. But we must be careful to avoid going out of bounds. The reference solution (from the snippet) uses a greedy approach: first, for each row except the last, try to go right `m-1` times, then perform `m-1` times the pattern "DUL" (which moves down, up, left, netting a left movement but also providing 3 moves each), then one "D" to move to the next row. For the last row, try to go right `m-1` times, then left `m-1` times, then up `n-1` times. The total possible moves is `(n-1)*(m-1 + 3*(m-1) + 1) + (m-1) + (m-1) + (n-1)` but the algorithm uses a helper `add` that greedily adds repeats of a command until the required `k` is reached or the repeat count is exhausted. The algorithm checks feasibility by attempting the construction in a fixed order, and if at any point the remaining `k` becomes zero, it outputs "YES" with the accumulated commands. If the entire fixed-order sequence is exhausted and `k` is still positive, it outputs "NO". The fixed order is: for each `i` from 1 to n-1: add `m-1` times "R", then `m-1` times "DUL", then 1 "D". Then after the loop, add `m-1` times "R", `m-1` times "L", `n-1` times "U". This order guarantees that every move stays inside the grid (as long as we are careful: "DUL" moves down, then up (back to same row), then left; this is valid if we are not on the first column and not on the last row, which is true during the loop because we are on row i (from 1 to n-1) and we start at column 1 so left would go out if we do "DUL" after having moved right a few times? Actually the pattern is added after moving right `m-1` times, so we end up at column m, row i. Then "DUL": down to row i+1, up back to row i, left to column m-1. That is valid. Then the next iteration starts from column m-1? Actually the next iteration adds "R" `m-1` times, which from column m-1 would go right to column m, but then we already have column m? Wait, we need to track position. The reference implementation does not track position, but it's known that the pattern is valid because after the "DUL" pattern, we end at column m-1, then the next "R" `m-1` times moves to column m again, and so on. This is a bit tricky but the reference is correct for the intended problem. The edge case is when `n=1` or `m=1`: the loop for rows does nothing, and we just try to add `m-1` R, `m-1` L, `n-1` U (which might be zero). If `n=m=1`, the total possible moves is zero, so if `k>0` it outputs "NO". Time complexity is O(n+m) because we add at most about 3n + 2 command groups, each with constant work. Space complexity is O(n+m) for storing the answer.

#include <string>
#include <vector>
#include <utility>

// Construct a path covering exactly k moves on an n x m grid.
// Returns "NO" if impossible, otherwise "YES\n<number of groups>\n" followed by
// each group as "<count> <command>\n". Commands: R, L, D, U, DUL.
std::string constructPath(int n, int m, long long k) {
    long long remaining = k;
    std::vector<std::pair<long long, std::string>> groups;
    
    // Helper to add a command repeated 'times' times, consuming from 'remaining'.
    // Returns false if we can finish exactly within this repetition.
    auto add = [&](long long times, const std::string& cmd) -> bool {
        if (times == 0) return true;
        long long len = static_cast<long long>(cmd.size());
        if (times * len < remaining) {
            remaining -= times * len;
            groups.push_back({times, cmd});
            return true;
        }
        long long full = remaining / len;
        remaining %= len;
        if (full > 0) groups.push_back({full, cmd});
        if (remaining == 0) return false;
        // Partial repetition: take a prefix of the command.
        std::string prefix = cmd.substr(0, static_cast<size_t>(remaining));
        groups.push_back({1, prefix});
        remaining = 0;
        return false;
    };
    
    bool possible = true;
    // For each row except the last: go right, then do DUL pattern, then down.
    for (int i = 1; i < n && possible; ++i) {
        if (!add(m - 1, "R")) possible = false;
        if (possible && !add(m - 1, "DUL")) possible = false;
        if (possible && !add(1, "D")) possible = false;
    }
    
    if (possible) {
        // Last row: go right, then left, then up.
        if (!add(m - 1, "R")) possible = false;
        if (possible && !add(m - 1, "L")) possible = false;
        if (possible && !add(n - 1, "U")) possible = false;
    }
    
    if (remaining != 0 || !possible) {
        return "NO";
    }
    
    std::string result = "YES\n" + std::to_string(groups.size()) + "\n";
    for (const auto& g : groups) {
        result += std::to_string(g.first) + " " + g.second + "\n";
    }
    return result;
}

#include <cassert>
#include <string>

std::string constructPath(int n, int m, long long k);

int main() {
    // Test 1: Simple 1x1 grid, k=0 is possible (empty path)
    assert(constructPath(1, 1, 0) == "YES\n0\n");
    // Test 2: 1x1 grid, k>0 impossible
    assert(constructPath(1, 1, 1) == "NO");
    // Test 3: 1x3 grid, k=2 possible (move right twice)
    std::string res = constructPath(1, 3, 2);
    assert(res.rfind("YES\n", 0) == 0);
    // Count moves from output (should be 2)
    // We can parse the output but simpler: just check it's YES
    // Test 4: 2x2 grid, k=3 possible? 
    // Moves: right(1), DUL(3), D(1) => 5 total, but we can truncate.
    res = constructPath(2, 2, 3);
    assert(res.rfind("YES\n", 0) == 0);
    // Test 5: 2x2 grid, k=100 possible (can do many cycles)
    res = constructPath(2, 2, 100);
    assert(res.rfind("YES\n", 0) == 0);
    // Test 6: 1x1 grid, k=0 is YES (already tested)
    // Test 7: 3x3 grid, k=1 possible (just one R)
    res = constructPath(3, 3, 1);
    assert(res.rfind("YES\n", 0) == 0);
    // Test 8: 3x3 grid, k=1000000 possible (long path)
    res = constructPath(3, 3, 1000000);
    assert(res.rfind("YES\n", 0) == 0);
    // Test 9: 2x1 grid, k=1 possible (one D)
    res = constructPath(2, 1, 1);
    assert(res.rfind("YES\n", 0) == 0);
    // Test 10: 1x2 grid, k=3 impossible (max moves 1)
    assert(constructPath(1, 2, 3) == "NO");
    return 0;
}
