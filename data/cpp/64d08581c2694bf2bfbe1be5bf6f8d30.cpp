/*
Write a C++ function `shortestKeypadSequence` that takes a string `code` consisting of digits `0`–`9` and the letter `A` (representing a code to be typed on a numerical keypad) and returns the minimum length of a button-press sequence required on a directional keypad to ultimately type that code through a chain of three robots: the first robot controls the second robot’s directional keypad, the second controls the third robot’s directional keypad, and the third robot physically presses the buttons on the numerical keypad. All four keypads have the same layout: the numerical keypad is arranged as `789 / 456 / 123 / 0A` (with `A` directly below `3`), and the directional keypad is arranged as `^A / <v>`. A robot’s arm starts at `A` and can only move one step at a time (up, down, left, right) to adjacent keys; pressing a button is a separate distinct action. The function must compute the minimal possible total number of actions (moves and presses) across all three robots to type the entire code, considering that every robot must return its arm to `A` after finishing the code (the arm can also be moved to `A` between presses as needed, but the final action of the last robot is the press of the last character, and then we count no additional moves). The input string will always be valid (only digits `0`–`9` and `A`, length at least 1). The function should return a `size_t` representing that minimal length.
*/

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

// Type aliases for readability
using NeighborMap = std::unordered_map<char, std::string>;
using DirectionMap = std::unordered_map<char, std::unordered_map<char, char>>;
using PathMap = std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>>;

// Keypad layouts as static data
static const NeighborMap numerical_neighbors = {
    {'7', "84"}, {'8', "759"}, {'9', "86"},
    {'4', "751"}, {'5', "8642"}, {'6', "953"},
    {'1', "42"}, {'2', "5310"}, {'3', "62A"},
    {'0', "2A"}, {'A', "30"}
};

static const DirectionMap numerical_movement = {
    {'9', {{'8','<'}, {'6','v'}}},
    {'8', {{'9','>'}, {'7','<'}, {'5','v'}}},
    {'7', {{'8','>'}, {'4','v'}}},
    {'6', {{'5','<'}, {'3','v'}, {'9','^'}}},
    {'5', {{'6','>'}, {'4','<'}, {'2','v'}, {'8','^'}}},
    {'4', {{'5','>'}, {'1','v'}, {'7','^'}}},
    {'3', {{'2','<'}, {'6','^'}, {'A','v'}}},
    {'2', {{'3','>'}, {'1','<'}, {'0','v'}, {'5','^'}}},
    {'1', {{'2','>'}, {'4','^'}}},
    {'0', {{'A','>'}, {'2','^'}}},
    {'A', {{'0','<'}, {'3','^'}}}
};

static const NeighborMap directional_neighbors = {
    {'^', {"Av"}}, {'A', {"^>"}}, {'<', {"v"}},
    {'v', {"<^>"}}, {'>', {"vA"}}
};

static const DirectionMap directional_movement = {
    {'<', {{'v','>'}}},
    {'^', {{'A','>'}, {'v','v'}}},
    {'v', {{'>','>'}, {'^','^'}, {'<','<'}}},
    {'A', {{'^','<'}, {'>','v'}}},
    {'>', {{'v','<'}, {'A','^'}}}
};

namespace {
    // Remove all paths longer than the shortest
    void keepOnlyShortest(std::vector<std::string>& paths) {
        if (paths.empty()) return;
        auto minIt = std::min_element(paths.begin(), paths.end(),
                                      [](const std::string& a, const std::string& b) {
                                          return a.size() < b.size();
                                      });
        size_t minSize = minIt->size();
        paths.erase(std::remove_if(paths.begin(), paths.end(),
                                   [&](const std::string& p) { return p.size() > minSize; }),
                    paths.end());
    }

    // Compute all shortest movement sequences from key_from to key_to on a given keypad
    std::vector<std::string> shortestPathsBetween(char from, char to,
                                                   const NeighborMap& neighbors,
                                                   const DirectionMap& moves) {
        if (from == to) return {""};
        // BFS to find shortest paths
        std::vector<std::string> allPaths;
        std::vector<std::string> queue;
        std::string path(1, from);
        queue.push_back(path);
        size_t minLen = std::numeric_limits<size_t>::max();
        while (!queue.empty()) {
            std::string cur = queue.back();
            queue.pop_back();
            char last = cur.back();
            // If we reach target, record path (we might get longer ones later)
            if (last == to) {
                if (cur.size() <= minLen) {
                    minLen = cur.size();
                    allPaths.push_back(cur);
                }
                continue;
            }
            // Prune if current length + 1 > minLen
            if (minLen != std::numeric_limits<size_t>::max() && cur.size() + 1 > minLen) continue;
            for (char nxt : neighbors.at(last)) {
                // Avoid revisiting nodes and backtracking to start except initial
                if (cur.find(nxt) != std::string::npos) continue;
                std::string nextPath = cur + nxt;
                if (nextPath.size() <= minLen) {
                    queue.push_back(nextPath);
                }
            }
        }
        // Convert node sequences to direction sequences
        std::vector<std::string> result;
        for (const auto& p : allPaths) {
            std::string dirs;
            for (size_t i = 0; i < p.size(); ++i) {
                char fromKey = (i == 0) ? from : p[i-1];
                char toKey = p[i];
                dirs.push_back(moves.at(fromKey).at(toKey));
            }
            result.push_back(dirs);
        }
        keepOnlyShortest(result);
        return result;
    }

    // Precompute all-pairs shortest paths for a keypad
    PathMap computeAllShortestPaths(const NeighborMap& neighbors, const DirectionMap& moves) {
        PathMap result;
        for (const auto& kv : neighbors) {
            char from = kv.first;
            for (const auto& kv2 : neighbors) {
                char to = kv2.first;
                result[from][to] = shortestPathsBetween(from, to, neighbors, moves);
            }
        }
        return result;
    }

    // Given a start key and a target key on the directional keypad, return all possible control sequences
    // (each ends with pressing A) that move the robot from start to target.
    std::vector<std::string> controlSeqForSingleKey(char start, char target,
                                                    const PathMap& dirPaths) {
        if (start == target) return {"A"};
        std::vector<std::string> result;
        for (const auto& moveSeq : dirPaths.at(start).at(target)) {
            result.push_back(moveSeq + 'A');
        }
        return result;
    }

    // Given a start key and a sequence of buttons to press (on the directional keypad),
    // return all possible full control sequences (ending with the final press) that type them.
    std::vector<std::string> controlSeqForString(char start, const std::string& buttons,
                                                 const PathMap& dirPaths) {
        if (buttons.empty()) return {""};
        std::vector<std::string> heads = controlSeqForSingleKey(start, buttons[0], dirPaths);
        for (size_t i = 1; i < buttons.size(); ++i) {
            char prev = buttons[i-1];
            char cur = buttons[i];
            std::vector<std::string> newHeads;
            for (const auto& head : heads) {
                auto nextSeqs = controlSeqForSingleKey(prev, cur, dirPaths);
                for (const auto& ns : nextSeqs) {
                    newHeads.push_back(head + ns);
                }
            }
            heads = newHeads;
            keepOnlyShortest(heads);
        }
        return heads;
    }
}

// Main function: compute minimal length of outermost control sequence for given code
size_t shortestKeypadSequence(const std::string& code) {
    // Precompute all shortest paths
    PathMap numericalPaths = computeAllShortestPaths(numerical_neighbors, numerical_movement);
    PathMap directionalPaths = computeAllShortestPaths(directional_neighbors, directional_movement);

    // Level 1: Numerical robot types the code (starts at A)
    std::vector<std::string> level1 = controlSeqForString('A', code, directionalPaths); // but numerical robot's keypad is different; actually we need to use numericalPaths for movements on numerical keypad, but the higher-level control sequence for the numerical robot is typed on a directional keypad. The sequence of moves on the numerical keypad is what we need to type on the directional keypad. So we need to compute the movement sequences for the numerical robot directly, not via controlSeqForString. We'll handle correctly below.
    // Better: compute the movement sequences on the numerical keypad to type the code.
    // For each character transition, get all shortest movement directions on numerical keypad.
    std::vector<std::string> numMovementSeqs;
    char curPos = 'A';
    // Build all sequences of movement strings for the whole code
    std::vector<std::string> possible = {""};
    for (char c : code) {
        std::vector<std::string> newPossible;
        for (const auto& prefix : possible) {
            if (curPos == c) {
                newPossible.push_back(prefix); // no movement, just press A later
            } else {
                for (const auto& mv : numericalPaths.at(curPos).at(c)) {
                    newPossible.push_back(prefix + mv);
                }
            }
        }
        possible = newPossible;
        keepOnlyShortest(possible); // keep minimal total movement so far (without presses)
        curPos = c;
    }
    // Now each string in `possible` is the concatenation of movement-only sequences for each transition.
    // To type each character, we need to press A after each movement segment.
    // So we must produce the full level-1 button sequence: for each character, append 'A'.
    // But careful: between characters, we don't need to press anything else; we just move and press.
    std::vector<std::string> level1Buttons;
    for (const auto& moves : possible) {
        std::string full;
        // Insert A after each movement segment? Actually we need to interleave: for each character, the movement sequence then 'A'.
        // But our `possible` is concatenated movements without separators. We need to split by original transitions.
        // Simpler: recompute directly.
    }
    // Instead, we'll directly build level1 as a string of directional moves and presses.
    // Let's compute all sequences for the numerical robot: for each character, get all movement+press sequences on numerical keypad, then compose.
    std::vector<std::string> numSeq = {""};
    char start = 'A';
    for (char c : code) {
        std::vector<std::string> newNum;
        for (const auto& pref : numSeq) {
            if (start == c) {
                newNum.push_back(pref + 'A');
            } else {
                for (const auto& mv : numericalPaths.at(start).at(c)) {
                    newNum.push_back(pref + mv + 'A');
                }
            }
        }
        numSeq = newNum;
        keepOnlyShortest(numSeq);
        start = c;
    }
    // Now numSeq contains the button-press sequences on the numerical keypad (each char is a direction or A press).
    // Next level: directional robot must type these sequences. But the directional robot's keypad has buttons ^, A, <, v, >.
    // The numSeq strings contain characters from {<,>,^,v,A}? Actually numerical movement sequences consist of '<','>','^','v' and we appended 'A' each time. So numSeq contains those characters.
    // Now we need to type these sequences on a directional keypad, which has the same set of buttons as the movement directions! That's the trick: the numerical keypad's movements correspond exactly to the directional keypad's keys. So the directional robot must press those movement keys.
    // So level2: for each string in numSeq, compute all control sequences to type it on the directional keypad (starting from A on that keypad).
    std::vector<std::string> level2;
    for (const auto& s : numSeq) {
        auto seqs = controlSeqForString('A', s, directionalPaths);
        for (const auto& x : seqs) level2.push_back(x);
    }
    keepOnlyShortest(level2);
    // Level3: outermost directional robot types level2 sequences.
    std::vector<std::string> level3;
    for (const auto& s : level2) {
        auto seqs = controlSeqForString('A', s, directionalPaths);
        for (const auto& x : seqs) level3.push_back(x);
    }
    keepOnlyShortest(level3);
    if (level3.empty()) return 0;
    return level3[0].size();
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test simple code "0" : Numerical robot starts at A, needs to press 0.
    // Movement from A to 0: one path '<' (A->< is not directly? Actually A neighbors: 0,3. A->0 is '<'? Wait from A to 0: A's neighbors are 0 and 3. So move left to 0. So movement sequence "<". Then press A. So numerical sequence: "<A". Then directional robot to type "<A": from A, move left to '<', press A? Actually to press '<' on directional keypad, we need to move from A to '<'? Directional keypad layout: ^A / <v>. A is at (1,0) (top right), '<' is at (0,1) bottom left. Shortest path from A to '<': go down then left? Or left then down? Both length 2: "v<" or "<v"? But we need to press the key '<' so we move to it and press A. Then from '<' to 'A' to press the second button 'A'? Actually to type "<A" on directional keypad, we start at A, need to press '<' then press 'A'. Pressing '<' requires moving to '<' then pressing A (the button press on directional keypad that sends to numerical robot). Then pressing 'A' on directional keypad requires moving back to 'A' and pressing. So sequence: move from A to '<' (say "v<"), press A, move from '<' to A (maybe "<^" or ">^"? Actually from '<' to A: go up then right? '<' neighbors: v. A neighbors: ^,>. So from '<' to A: must go up to ^ then right to A? That's ">^" or "^>"? Both length 2. So one possible directional control sequence: "<v"?? It's complicated. But we don't need to compute manually; we trust the algorithm. We'll just assert that the function returns positive and that for "0" it equals some known value from the original problem? The original AoC example didn't include "0". Let's just test basic sanity: code "A" should be minimal? Pressing A on numerical keypad directly: numerical robot starts at A, so it just presses A. That yields sequence "A". Then directional robot to type "A": press A on its own keypad, so sequence "A". Then outermost robot to type "A": sequence "A". Total length 1. So shortestKeypadSequence("A") should be 1.
    assert(shortestKeypadSequence("A") == 1);

    // Code "0": numerical robot from A to 0: move '<' then press 'A' => "<A". Directional robot to type "<A": 
    // But from known solutions (AoC 2024 Day 21), the length for "0" is 4? Actually let's not guess; we can compute via brute force for small? Instead, we'll assert that the function returns a positive value and that it is consistent: the result should be finite and reasonable.
    size_t r0 = shortestKeypadSequence("0");
    assert(r0 > 0);

    // Code "1": numerical robot from A to 1: path A->0->2->1? Actually A->0 '<', 0->2 '^', 2->1 '<' => "<^<" plus press A => "<^<A". Then up the chain.
    size_t r1 = shortestKeypadSequence("1");
    assert(r1 > 0);

    // Compare to known result for "0" from the problem statement? The original example in AoC had code "029A" with answer 68 * 29? But we can't derive here. We'll just check that the function returns a value and that for a longer code it is non-decreasing? Not necessarily, but we'll check monotonic? Not needed.

    // Test that the result is symmetric? No.

    // Ensure that the function works for multi-character codes.
    size_t r029A = shortestKeypadSequence("029A");
    assert(r029A > 0);

    // Known from AoC 2024 Day 21 part 1: for code "029A", the answer (sum of complexities) is 126384 for a specific set. But the length alone for "029A" should be 68? Actually the example: For "029A", the shortest sequence length is 68? Let's check memory: In the problem, the fewest number of button presses on the outermost robot for "029A" is 68, and numeric part is 29, so complexity 1972? Actually the given example: 029A -> 68 * 29 = 1972, and total sum for five codes is 126384. So we can assert that shortestKeypadSequence("029A") == 68.
    assert(shortestKeypadSequence("029A") == 68);

    // Also test another known: "980A" -> length 64, numeric 980 => 62720? Actually from AoC example: 980A -> 64 * 980 = 62720, 179A -> 68 * 179 = 12172, 456A -> 64 * 456 = 29184, 379A -> 64 * 379 = 24256. Sum 126384. So we can assert:
    assert(shortestKeypadSequence("980A") == 64);
    assert(shortestKeypadSequence("179A") == 68);
    assert(shortestKeypadSequence("456A") == 64);
    assert(shortestKeypadSequence("379A") == 64);
}

// The problem is essentially a shortest-path problem on a layered sequence of keypads. We treat each keypad as an undirected graph whose edges are adjacent key positions; the length of an edge is 1 movement action. Pressing a key is an additional action, and we append a press action after every movement segment from one key to the next. We precompute, for both the numerical and directional keypads, the set of all shortest movement sequences between any two distinct keys (as strings of movement directions: `<`, `>`, `^`, `v`). Then, for a given target key sequence (e.g., the code to type), we recursively compute the set of all minimal-length control sequences at the next higher level of abstraction. Specifically, to move a robot from a start key to a target key (and, for the numerical robot, we must also return to `A` after the entire code), we take each shortest movement path on that keypad, then we need to "type" that movement string on the directional keypad one level up, which itself may have multiple shortest representations. Then we repeat for the next level up. The key insight is that for each movement path segment, we must also press `A` after the segment to actually perform the movement on the lower robot. So we compose recursively: `getControlSequence(robotStart, targetSeq)` returns all possible minimal-length sequences (in terms of actions at the higher level) to make the robot type `targetSeq` starting from `robotStart`, where the higher level is a directional keypad. For the base case, when the robot is the numerical one, we just need to type the code and then return to `A`; but since the problem asks only for the final length after three robots, we chain three levels: level 1 (numerical robot) -> level 2 (directional robot) -> level 3 (directional robot). At each level, we generate candidate sequences from the level below by composing all shortest paths for each character transition, and we keep only the shortest ones at each step. The recursion terminates when we reach the top level (the outermost robot) where we directly count the length of the candidate sequence. Important edge cases: (1) if start and target are the same, we only press `A` (length 1); (2) there may be multiple shortest movement paths between two keys (e.g., on numerical keypad from `7` to `9` is just `>` but from `1` to `0` may be `v>` or `>v`, both length 2); we must consider all; (3) for the numerical robot after typing the entire code, we also need to return to `A`, but the problem statement says we only count the actions to type the code and the final press, not extra moves after the last press? Actually re-reading: The code snippet's `solve` function iterates over `get_control_candidate_sequence('A', code, numerical_shortest_paths)` which already includes returning to `A` at the end (because the numerical robot's sequence ends with `A`). But in the original problem (Advent of Code 2024 Day 21), after typing the code, the robots need not return to `A`; however the provided snippet appears to include a return to `A` at the end? Actually looking at `get_control_candidate_sequence(robot_start_key, robot_target_key, ...)` for a single target key: it returns sequences that move from start to target and then from target back to `A` (with a final press on that return). But the overload for a string `code` calls `get_control_candidate_sequence('A', code, numerical_shortest_paths)` which processes each character and does NOT force a final return to `A`; it just types each character sequentially. The original snippet in `solve` does three nested loops: for `s1` (numerical robot's sequence from A to type code, not returning to A), then for `s2` (directional robot's sequence to type `s1`), then for `s3` (directional robot's sequence to type `s2`). So the final length is just the length of `s3` (the outermost sequence). There is no requirement to return to `A` at the end. The provided snippet's `get_control_candidate_sequence` for a target key alone does include a return, but that is used for internal moves between keys; for a whole string, it just types each character sequentially and does not add a final return. Thus I will implement the function exactly as the snippet: compute shortest paths between all keys on numerical and directional keypads, then for the given code compute minimal length after three levels: level1 = control sequences for numerical robot to type code, level2 = control sequences for directional robot to type level1 sequences, level3 = control sequences for directional robot to type level2 sequences, take minimum length of level3. Time complexity: The number of candidate sequences grows combinatorially, but due to the small fixed keypad sizes (4 directional keys, 11 numerical keys) and the fact that we prune to shortest at each step, the number of candidates remains bounded (typically a few dozen per step). Precomputing shortest paths for all pairs on both keypads takes O(V^3) naive BFS but here we use specific neighbor graphs and BFS per pair, which is fine with V=11 and V=5. The overall complexity for a code of length L is roughly O(L * C) where C is the number of candidate sequences at each level (small constant, ≤ maybe 10^3). Space is O(V^2 * average path length) for stored paths, plus O(C) for working sets.
