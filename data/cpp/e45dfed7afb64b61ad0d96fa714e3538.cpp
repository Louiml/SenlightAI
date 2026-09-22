A frog is attempting to jump out of a well. The frog can jump exactly `forwardSteps` meters forward, or exactly `backwardSteps` meters backward. The well has a top edge at position `forwardDitch` (a positive integer) and a bottom edge at position `-backwardDitch` (where `backwardDitch` is given as a positive integer, so the bottom is at `-backwardDitch`). The frog starts at position 0. Each jump takes exactly `time` seconds per meter of horizontal movement (so a jump of `d` meters takes `d * time` seconds). The frog performs jumps in a fixed alternating order: first a forward jump, then a backward jump, then forward, and so on. The frog "escapes" if, after completing a jump, it lands at or beyond the top edge (position ≥ `forwardDitch`) or at or below the bottom edge (position ≤ `-backwardDitch`). At the moment of escape, the frog stops immediately; the total time is the sum of all full jumps completed before escape plus the time for the partial distance covered in the escape jump (since the jump is considered successful as soon as it reaches the boundary, and the remaining distance of that jump is not traveled). Write a C++ function that takes five integers: `forwardSteps`, `backwardSteps`, `forwardDitch`, `backwardDitch`, and `time`, and returns a string describing the escape. If the frog escapes on a forward jump, return "F " followed by the total time in seconds (as an integer). If it escapes on a backward jump, return "B " followed by the total time. If the frog never escapes (because `forwardSteps == backwardSteps` and it cannot reach either boundary from any starting position within the well), return "NO". Assume all inputs are positive integers (except possibly `time` can be 0, but it is non-negative). The function must handle the case where the frog escapes on the first jump, and also handle the case where it oscillates forever without escape. The inputs are such that the total time fits in a 64-bit signed integer. The function signature is `std::string frogEscape(long long forwardSteps, long long backwardSteps, long long forwardDitch, long long backwardDitch, long long time)`. Note: the original snippet had a bug where `backwardDitch` was negated internally; your solution must correctly interpret the bottom boundary as `-backwardDitch`.

#include <cassert>
#include <string>
#include <iostream>

// Declare the function (in a real project, this would be in a header)
std::string frogEscape(long long forwardSteps, long long backwardSteps,
                       long long forwardDitch, long long backwardDitch,
                       long long time);

int main() {
    // Case 1: First forward jump escapes immediately.
    assert(frogEscape(10, 5, 10, 100, 2) == "F 20"); // distance 10, time 2

    // Case 2: Forward jump escapes after a backward jump (steps unequal).
    // Start 0, forward to 5 (can't reach top 8), back to 0, forward to 5 again? Actually:
    // forward=5, back=3, top=8, bottom=10. Jump1: 0->5, not >=8; back to 2. Jump2: 2->7, not >=8; back to 4. Jump3: 4->9, escape. Distance for third jump = 8-4=4. Total distance = 5+3+5+3+4=20, time=1 => "F 20"
    assert(frogEscape(5, 3, 8, 10, 1) == "F 20");

    // Case 3: Backward escape.
    // forward=2, back=5, top=100, bottom=10. Start 0->2, not escape; back to -3, not <=-10; forward to -1; back to -6; forward to -4; back to -9; forward to -7; back to -12, escape. Distance for last backward = -7 - (-10) = 3? Actually position before backward is -7, need to reach -10, distance = 3. Total distance = 2+5+2+5+2+5+2+5+2+5+2+5+2+5+2+3? Let's compute: jumps: F2, B5, F2, B5, F2, B5, F2, B5, F2, B5, F2, B5, F2, B5, F2? Wait, we need to simulate carefully. But test with a known simple case: forward=1, back=2, top=100, bottom=1. Start 0->1, not escape; back -1, not <=-1? Actually -1 <= -1 is true, escape on backward with distance 1 (from 1 to 0? Wait, position=1, target -1, distance = 1 - (-1) = 2? Actually need to travel from 1 down to -1, distance 2. First forward distance 1, second backward distance 2, total distance 3, time=1 => "B 3". Test that.
    assert(frogEscape(1, 2, 100, 1, 1) == "B 3");

    // Case 4: Equal steps, forward escape.
    assert(frogEscape(5, 5, 5, 10, 1) == "F 5");

    // Case 5: Equal steps, no escape.
    assert(frogEscape(3, 3, 10, 10, 1) == "NO");

    // Case 6: Time is zero, escape immediately.
    assert(frogEscape(10, 5, 10, 100, 0) == "F 0");

    // Case 7: Large numbers, forward escape after multiple cycles.
    // forward=10, back=1, top=25, bottom=100. Simulate: 0->10 (not>=25), back to 9, 9->19 (not), back to 18, 18->28 escape. Distance for last = 25-18=7. Total distance = 10+1+10+1+7=29, time=2 => "F 58"
    assert(frogEscape(10, 1, 25, 100, 2) == "F 58");

    // Case 8: Backward escape on second jump (immediate after first forward).
    // forward=2, back=3, top=100, bottom=1. Start 0->2, not; back from 2 to -1, escape (target -1). Distance backward = 2 - (-1) = 3. Total distance = 2 + 3 = 5, time=1 => "B 5"
    assert(frogEscape(2, 3, 100, 1, 1) == "B 5");

    // Case 9: Escape exactly on boundary.
    assert(frogEscape(5, 2, 5, 10, 1) == "F 5"); // first jump exactly to top

    // Case 10: Escape exactly on backward boundary.
    // forward=1, back=4, top=100, bottom=4. 0->1, back to -3, not <=-4; forward to -2, back to -6, escape. Position before last backward = -2, target -4, distance = 2. Total distance = 1+4+1+2=8, time=3 => "B 24"
    assert(frogEscape(1, 4, 100, 4, 3) == "B 24");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <string>
#include <sstream>

// Determine the escape direction and total time for a frog jumping in a well.
// forwardSteps: distance of each forward jump
// backwardSteps: distance of each backward jump
// forwardDitch: top edge position (escape if position >= forwardDitch)
// backwardDitch: bottom edge magnitude (escape if position <= -backwardDitch)
// time: seconds per meter traveled
// Returns "F <time>" or "B <time>" on escape, or "NO" if escape never occurs.
std::string frogEscape(long long forwardSteps, long long backwardSteps,
                       long long forwardDitch, long long backwardDitch,
                       long long time) {
    // Special case: if steps are equal, the frog oscillates between 0 and forwardSteps.
    if (forwardSteps == backwardSteps) {
        if (forwardSteps >= forwardDitch) {
            // First forward jump reaches or exceeds the top edge.
            long long distance = forwardDitch; // from 0 to forwardDitch
            return "F " + std::to_string(distance * time);
        } else {
            // Never escapes because backward jump returns to 0, never below -backwardDitch.
            return "NO";
        }
    }

    long long position = 0;
    long long elapsed = 0; // total distance traveled so far (in meters)

    while (true) {
        // Forward jump attempt
        if (position + forwardSteps >= forwardDitch) {
            // Need to travel (forwardDitch - position) meters to reach top.
            long long distance = forwardDitch - position;
            return "F " + std::to_string((elapsed + distance) * time);
        } else {
            // Complete the full forward jump.
            position += forwardSteps;
            elapsed += forwardSteps;
        }

        // Backward jump attempt
        if (position - backwardSteps <= -backwardDitch) {
            // Need to travel (position - (-backwardDitch)) = (position + backwardDitch) meters down.
            long long distance = position + backwardDitch;
            return "B " + std::to_string((elapsed + distance) * time);
        } else {
            // Complete the full backward jump.
            position -= backwardSteps;
            elapsed += backwardSteps;
        }
    }
}

// The problem is a simulation of alternating jumps with early termination upon reaching either boundary. The key observation is that after each forward+backward pair, the frog returns to the same position `p` (since it moves `forwardSteps` then `backwardSteps`). Thus, the only positions ever visited are multiples of the net movement per cycle, but because of the alternating order, the frog visits `p + forwardSteps` then `p` again, then `p + forwardSteps` again, etc. The algorithm is to simulate the sequence: start at position `p = 0`, time `t = 0`. In each iteration, check the forward jump: if `p + forwardSteps >= forwardDitch`, escape on forward with total time `(t + (forwardDitch - p)) * time` because the frog only needs to travel `forwardDitch - p` meters of that jump to reach the top. Otherwise, add the full forward jump: `p += forwardSteps`, `t += forwardSteps`. Then check backward jump: if `p - backwardSteps <= -backwardDitch`, escape on backward with total time `(t + (p - (-backwardDitch))) * time` = `(t + p + backwardDitch) * time`, because from position `p` it needs to travel `p - (-backwardDitch) = p + backwardDitch` meters downward. Otherwise, add the full backward jump: `p -= backwardSteps`, `t += backwardSteps`. The loop continues. However, there is a special case: when `forwardSteps == backwardSteps`, the frog alternates between positions `0` and `forwardSteps` forever, so it can only escape if `forwardSteps >= forwardDitch` (first forward jump reaches top) or if `forwardSteps <= backwardDitch`? Wait, the backward jump from position `forwardSteps` goes to `0`, which is not below `-backwardDitch` unless `backwardDitch` is 0, but it's positive. So escape on backward is impossible when steps are equal because after a forward jump it is at `forwardSteps`, and then a backward jump returns to `0`, never reaching negative. Thus only forward escape is possible, and only on the first jump (or any forward jump, but all are the same), so if `forwardSteps >= forwardDitch`, escape immediately, else no escape. For the general case `forwardSteps != backwardSteps`, the simulation will eventually terminate because each iteration moves the position by `forwardSteps - backwardSteps` net per cycle? Actually, after a full forward+backward pair, the net change is `forwardSteps - backwardSteps`, but the frog's positions during the forward jump are increasing, and during backward jump decreasing. Since the boundaries are finite, the frog must either hit a boundary or eventually? For example, if `forwardSteps < backwardSteps`, then after a backward jump the position decreases by `backwardSteps - forwardSteps` relative to previous cycle, so it will go more and more negative, eventually hitting the bottom. If `forwardSteps > backwardSteps`, it goes more positive, eventually hitting the top. Thus the simulation always terminates when steps are unequal. The loop is guaranteed to break in finite steps because the position is monotonic during each forward phase (increasing) and each backward phase (decreasing), but the overall trend is monotonic after accounting for net change; specifically, consider the sequence of positions after each full pair: `0, forwardSteps - backwardSteps, 2*(forwardSteps - backwardSteps), ...` and also the intermediate positions. If `forwardSteps > backwardSteps`, the sequence increases without bound, eventually exceeding `forwardDitch`. If `forwardSteps < backwardSteps`, the sequence decreases without bound, eventually going below `-backwardDitch`. Since jumps are integer steps, the number of iterations is at most something like `max(forwardDitch, backwardDitch) / |forwardSteps - backwardSteps|` plus a constant, so it terminates. Time complexity is O(number of jumps), which is at most O(max(forwardDitch, backwardDitch) / |forwardSteps - backwardSteps|). In the worst case, if `forwardSteps - backwardSteps` is 1 and boundaries are large, could be O(max(forwardDitch, backwardDitch)). Space is O(1). Edge cases: first jump escape, immediate backward escape (but that can't happen because backward jump first is after forward, so first backward jump is second jump). Also, handle the case where `forwardSteps` is larger than `forwardDitch` and `backwardSteps` is large, but the first forward escapes. Also handle case where `time` is 0, then total time is 0. The function should return a string with "F " or "B " followed by the computed total time as an integer (using `long long`). Note that total time = (distance * time), and distance is an integer, time is integer, product fits in `long long` as guaranteed.
