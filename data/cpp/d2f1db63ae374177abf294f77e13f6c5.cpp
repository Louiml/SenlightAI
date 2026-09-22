/*
Write a C++ function that simulates the movement of a toy on a circular track of `n` positions, where each position has a direction indicator for the toy. Given an initial configuration described by `n` pairs of `(direction, name)` and a list of `m` movement commands `(direction, steps)`, return the name of the toy at the final position after applying all commands in order. The positions are indexed from 0 to n-1, and the toy starts at position 0. Each command has a `direction` (0 means move clockwise/increasing index, 1 means move counter‑clockwise/decreasing index) and a `steps` count (non‑negative). The circular track wraps around. The movement of the toy must account for the direction indicator at the current position: if the current position's indicator equals 1, the toy's movement direction is reversed before it moves. The final answer is the name at the final position. Both `n` and `m` are positive, names are non‑empty strings, and `steps` can be up to 10^9. For clarity, you must not use global variables; instead, pass all needed data as parameters to the function.
*/
#include <string>
#include <vector>

// Simulates toy movement on a circular track given initial configuration and commands.
// Returns the name of the toy at the final position.
// toys: vector of pairs (sta, name) where sta is 0/1 indicator and name is the toy's name.
// commands: vector of pairs (dir, step) where dir is 0 (clockwise) or 1 (counter‑clockwise), step is non‑negative.
std::string simulateToyMovement(const std::vector<std::pair<int, std::string>>& toys,
                                const std::vector<std::pair<int, int>>& commands) {
    int n = static_cast<int>(toys.size());
    int current = 0;  // start at position 0

    for (const auto& cmd : commands) {
        int dir = cmd.first;
        int step = cmd.second;
        int step_mod = step % n;  // circular movement reduction

        // If current position's indicator is 1, reverse the direction.
        int effective_dir = dir;
        if (toys[current].first == 1) {
            effective_dir = 1 - dir;  // 0 -> 1, 1 -> 0
        }

        if (effective_dir == 0) {  // clockwise (increasing index)
            current = (current + step_mod) % n;
        } else {  // counter‑clockwise (decreasing index)
            current = (current - step_mod + n) % n;
        }
    }

    return toys[current].second;
}
#include <cassert>
#include <string>
#include <vector>
#include <utility>

// Include the solution function here (or link it)

int main() {
    // Test 1: Single position, any moves stay at same spot.
    std::vector<std::pair<int, std::string>> toys1 = {{0, "A"}};
    std::vector<std::pair<int, int>> cmds1 = {{0, 5}, {1, 10}};
    assert(simulateToyMovement(toys1, cmds1) == "A");

    // Test 2: Simple forward movement with no indicator reversal.
    std::vector<std::pair<int, std::string>> toys2 = {{0, "X"}, {0, "Y"}, {0, "Z"}};
    std::vector<std::pair<int, int>> cmds2 = {{0, 2}};  // from 0 -> 2
    assert(simulateToyMovement(toys2, cmds2) == "Z");

    // Test 3: Backward movement.
    std::vector<std::pair<int, std::string>> toys3 = {{0, "X"}, {0, "Y"}, {0, "Z"}};
    std::vector<std::pair<int, int>> cmds3 = {{1, 1}};  // from 0 -> 2 (counter‑clockwise wraps)
    assert(simulateToyMovement(toys3, cmds3) == "Z");

    // Test 4: Indicator reversal at current position.
    // toys[0].sta = 1, command dir=0 (clockwise). Effective becomes counter‑clockwise.
    std::vector<std::pair<int, std::string>> toys4 = {{1, "A"}, {0, "B"}, {0, "C"}};
    std::vector<std::pair<int, int>> cmds4 = {{0, 1}};  // effective counter‑clockwise: from 0 -> 2
    assert(simulateToyMovement(toys4, cmds4) == "C");

    // Test 5: Multiple commands with reversal and big steps modulo.
    // n=4, toys: 0: sta=0,name="P", 1: sta=1,name="Q", 2: sta=0,name="R", 3: sta=0,name="S"
    // Command1: dir=0 step=6 (mod 4 =2) -> from 0 to 2 (no reversal because sta=0) -> R
    // At index 2, sta=0, so Command2: dir=1 step=9 (mod 4 =1) effective counter‑clockwise -> from 2 to 1 -> Q
    std::vector<std::pair<int, std::string>> toys5 = {{0, "P"}, {1, "Q"}, {0, "R"}, {0, "S"}};
    std::vector<std::pair<int, int>> cmds5 = {{0, 6}, {1, 9}};
    assert(simulateToyMovement(toys5, cmds5) == "Q");

    // Test 6: Zero steps and reversal combined.
    std::vector<std::pair<int, std::string>> toys6 = {{1, "A"}, {0, "B"}};
    std::vector<std::pair<int, int>> cmds6 = {{0, 0}, {1, 0}};  // stays at 0 always
    assert(simulateToyMovement(toys6, cmds6) == "A");

    // Test 7: Large step value (1e9) with n=3.
    std::vector<std::pair<int, std::string>> toys7 = {{0, "a"}, {0, "b"}, {0, "c"}};
    std::vector<std::pair<int, int>> cmds7 = {{0, 1000000000}};  // 1e9 % 3 = 1, so from 0 -> 1
    assert(simulateToyMovement(toys7, cmds7) == "b");

    // Test 8: Example from snippet (toy[1] after commands) but with proper input interpretation.
    // n=3, toys: 0: sta=0 name="A", 1: sta=1 name="B", 2: sta=0 name="C"
    // Commands: (0,1) then (1,2)
    // Start at 0: cmd1 dir=0, step=1, sta=0 -> move to 1 (B)
    // At 1: sta=1, cmd2 dir=1 step=2 -> reverse to dir=0, step_mod=2 -> move from 1 to 0 (A)
    std::vector<std::pair<int, std::string>> toys8 = {{0, "A"}, {1, "B"}, {0, "C"}};
    std::vector<std::pair<int, int>> cmds8 = {{0, 1}, {1, 2}};
    assert(simulateToyMovement(toys8, cmds8) == "A");

    return 0;
}
// The main algorithm is a direct simulation: keep a variable `current` (initialized to 0) representing the index of the toy. For each command `(dir, step)`, determine the effective movement direction as follows: if the current position's indicator `toy[current].sta` equals 1, reverse the command's direction (i.e., if `dir` is 0, effective becomes 1; if `dir` is 1, effective becomes 0). Then move `step` positions in that effective direction on a circular array. Because `step` can be huge, we can reduce it modulo `n` to avoid overflow and unnecessary loop iterations. For a move clockwise (effective=0), new index = `(current + step_mod) % n`; for counter‑clockwise (effective=1), new index = `(current - step_mod + n) % n`. Continue for all commands. The final answer is `toy[current].name`. Edge cases: `n=1` (any move stays at index 0), `step=0` (no movement), and large steps (modulo reduces them). Complexity is O(m) time because each command takes constant time (using modulo), and O(1) extra space beyond input storage. Total storage is O(n+m) for the arrays.
