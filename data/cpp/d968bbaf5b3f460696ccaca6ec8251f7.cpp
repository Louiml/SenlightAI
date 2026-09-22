/*
Write a standalone C++ function `simulateHazardNoopInsertion` that takes a vector of integers representing a sequence of instruction issue counts (non-negative integers, each indicating how many issue slots an instruction consumes) and a hazard-threshold value `hazardLimit` (positive integer). The function must simulate a simplified post-register-allocation hazard recognizer: before each instruction, if the number of consecutive issue cycles already advanced since the last "noop-free" point equals or exceeds `hazardLimit`, insert noops (represented as `-1` in the output sequence) into the operation sequence. Specifically, maintain a current issue-cycle counter that increments by 1 for each instruction's issue count (i.e., `advance by the instruction's count after emitting it`). Before emitting an instruction, check: if the current cycle counter is greater than or equal to `hazardLimit`, then insert exactly one noop (append `-1` to the output sequence), reset the cycle counter to 0, and do not advance the counter for the noop. Then emit the instruction by appending its value, and afterwards increment the cycle counter by the instruction's value. Additionally, at the very beginning of the sequence, reset the cycle counter to 0 (do not insert noops before the first instruction if the cycle counter is initially 0). Return the modified sequence as a `std::vector<int>`. If the input sequence is empty, return an empty vector. The function must be `const`-correct and operate in a single pass.
*/

#include <vector>

// Simulates a hazard recognizer that inserts noops (represented as -1) before
// an instruction when the current issue-cycle counter reaches or exceeds hazardLimit.
// Each instruction's value is the number of issue cycles it consumes.
std::vector<int> simulateHazardNoopInsertion(const std::vector<int>& instructions, int hazardLimit) {
    std::vector<int> result;
    result.reserve(instructions.size() * 2); // at most one noop per instruction
    int cycles = 0;

    for (int val : instructions) {
        if (cycles >= hazardLimit) {
            result.push_back(-1); // insert a noop
            cycles = 0;           // reset cycle counter
        }
        result.push_back(val);
        cycles += val;
    }

    return result;
}

#include <cassert>
#include <vector>

// Function under test (same as solution, for completeness)
std::vector<int> simulateHazardNoopInsertion(const std::vector<int>& instructions, int hazardLimit) {
    std::vector<int> result;
    result.reserve(instructions.size() * 2);
    int cycles = 0;
    for (int val : instructions) {
        if (cycles >= hazardLimit) {
            result.push_back(-1);
            cycles = 0;
        }
        result.push_back(val);
        cycles += val;
    }
    return result;
}

int main() {
    // Basic case: no hazard triggers
    assert(simulateHazardNoopInsertion({1, 1, 1}, 5) == std::vector<int>({1, 1, 1}));

    // Trigger at cycle count reaching limit
    assert(simulateHazardNoopInsertion({3, 2, 1}, 3) == std::vector<int>({3, 2, -1, 1})); // after 3+2=5>=3, insert noop before 1

    // Multiple triggers
    assert(simulateHazardNoopInsertion({2, 2, 2, 2}, 4) == std::vector<int>({2, 2, -1, 2, 2, -1, 2})); // cycles:0->2->4 (trigger noop before 3rd), then reset->2->4 (trigger noop before 4th), then 2

    // Noop on first instruction if hazardLimit is 0? But hazardLimit positive, so skip
    assert(simulateHazardNoopInsertion({1}, 1) == std::vector<int>({1})); // first instruction cycles=0, no noop, then cycles=1

    // Empty input
    assert(simulateHazardNoopInsertion({}, 1) == std::vector<int>());

    // Zero-cycle instructions never trigger
    assert(simulateHazardNoopInsertion({0, 0, 0}, 1) == std::vector<int>({0, 0, 0}));

    // Trigger exactly at limit and then reset
    assert(simulateHazardNoopInsertion({2, 1, 1, 2}, 3) == std::vector<int>({2, 1, -1, 1, 2})); // cycles:0->2->3 (trigger noop before third), reset->1->3 (trigger noop before fourth?), wait: after noop cycles=0, emit 1 -> cycles=1, then emit 2 but before emitting 2 cycles=1<3, so no noop. Actually output: {2,1} cycles=3 -> noop, reset, emit 1 cycles=1, emit 2 cycles=3, no noop. So result {2,1,-1,1,2} correct.

    return 0;
}

// The algorithm is a straightforward single-pass simulation. Initialize a cycle counter `cycles = 0`. For each instruction value `val` in the input vector: first, if `cycles >= hazardLimit`, append a noop (`-1`) to the output and set `cycles = 0`. Then append `val` to the output. Then increment `cycles` by `val`. This correctly models the behavior described: a noop is inserted before any instruction when the current cycle count has reached the hazard threshold, and the noop resets the cycle count. Edge cases: empty input returns empty output. If `hazardLimit` is 1, then any instruction with value ≥1 triggers a noop before it? Wait—if `hazardLimit=1`, after the first instruction with value 1, `cycles=1`, so before the next instruction we insert a noop. That is fine. The very first instruction always has `cycles=0`, so it never triggers a noop initially. Also, if an instruction has value 0, it does not advance the counter, so it may never trigger a noop unless previous instructions accumulate. Complexity: O(n) time and O(1) auxiliary space besides the output vector (which is O(n) in size). No additional data structures needed.
