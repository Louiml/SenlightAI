// Create a C++ function that simulates the core computational behavior of the given SystemC module, but as a pure, synchronous function. The function should take a vector of integer input values (representing successive values of `in_value`) and produce a vector of integer outputs (representing the values written to `result` at each clock cycle). The function must replicate the exact sequence of arithmetic operations and control flow: first, it performs a loop of 5 iterations where each iteration reads the next input value, adds the current loop index `i` (1 through 5), then subtracts that input value 4 times (once per inner loop iteration `j` from 1 to 4), and outputs the result. After this, it consumes the next input value, then performs a nested loop where `i` goes from 1 to 3 and `j` goes from 1 to 2, adding `i` and subtracting `j` in the innermost body, and outputs the final accumulated result. The function should consume inputs strictly in the order they appear, and if there are not enough inputs to complete both phases, it should stop early at the point where an input is needed but unavailable, returning only the outputs produced so far.
#include <cassert>
#include <vector>

// Declare the function being tested (assumed defined elsewhere in the same translation unit)
std::vector<int> simulate_for_nest(const std::vector<int>& inputs);

int main() {
    // Test 1: Full six inputs, both phases complete
    std::vector<int> out1 = simulate_for_nest({10, 20, 30, 40, 50, 100});
    // Phase1: for i=1..5, acc = input + i - 4*input = i - 3*input
    // i=1: 1 - 30 = -29; i=2: 2 - 60 = -58; i=3: 3 - 90 = -87; i=4: 4 - 120 = -116; i=5: 5 - 150 = -145
    // Phase2: acc=100; i=1: +1 then -1-2 => -2; acc=99; i=2: +2 =>101 then -1-2=>98; i=3: +3=>101 then -1-2=>98
    assert((out1 == std::vector<int>{-29, -58, -87, -116, -145, 98}));

    // Test 2: Only phase1 inputs (5 inputs) – phase2 not executed because no extra input
    std::vector<int> out2 = simulate_for_nest({5, 5, 5, 5, 5});
    // For each: acc = 5 + i - 20 = i -15 => i=1:-14, i=2:-13, i=3:-12, i=4:-11, i=5:-10
    assert((out2 == std::vector<int>{-14, -13, -12, -11, -10}));

    // Test 3: Fewer than 5 inputs – stop after available
    std::vector<int> out3 = simulate_for_nest({1, 2, 3});
    // i=1: 1 +1 -4 = -2; i=2: 2+2-8 = -4; i=3: 3+3-12 = -6; then break
    assert((out3 == std::vector<int>{-2, -4, -6}));

    // Test 4: Exactly 6 inputs but with different values
    std::vector<int> out4 = simulate_for_nest({0, -1, 2, 7, 3, 9});
    // Phase1: i=1: 0+1-0=1; i=2: -1+2-(-4)=? compute: -1+2=1 then -(-1)*4 = 1+4=5? Wait: acc = input + i then subtract input 4 times => (-1+2) -4*(-1) = 1+4=5
    // i=3: 2+3=5 -8=-3; i=4: 7+4=11 -28=-17; i=5: 3+5=8 -12=-4
    // Phase2: acc=9; i=1:+1 -3 =>7; i=2:+2 -3 =>6; i=3:+3 -3 =>6
    assert((out4 == std::vector<int>{1, 5, -3, -17, -4, 6}));

    // Test 5: Empty input – no outputs
    std::vector<int> out5 = simulate_for_nest({});
    assert(out5.empty());

    // Test 6: Negative inputs test
    std::vector<int> out6 = simulate_for_nest({-2, -2, -2, -2, -2, -2});
    // Phase1: each: acc = -2 + i - 4*(-2) = i + 6 => i=1:7, i=2:8, i=3:9, i=4:10, i=5:11
    // Phase2: acc=-2; i=1: +1 -3 = -4; i=2:+2 -3 = -5; i=3:+3 -3 = -5
    assert((out6 == std::vector<int>{7, 8, 9, 10, 11, -5}));

    return 0;
}
#include <vector>

// Simulate the arithmetic sequence of the SystemC module for a list of inputs.
// Returns the outputs produced until inputs are exhausted or all steps complete.
std::vector<int> simulate_for_nest(const std::vector<int>& inputs) {
    std::vector<int> outputs;
    size_t idx = 0;  // current input index

    // Phase 1: Unrolled loop inside rolled loop (5 outer iterations, 4 inner subractions)
    for (int i = 1; i <= 5; ++i) {
        if (idx >= inputs.size()) break;  // no more inputs
        int input_value = inputs[idx++];
        int acc = input_value + i;       // tmp2 = in_value + i
        for (int j = 1; j <= 4; ++j) {
            acc -= input_value;          // subtract original input 4 times
        }
        outputs.push_back(acc);          // result.write(inp_tmp)
    }

    // Phase 2: Unrolled loop inside unrolled loop (needs one more input as accumulator)
    if (idx < inputs.size()) {
        int acc = inputs[idx++];         // inp_tmp = in_value.read()
        for (int i = 1; i <= 3; ++i) {
            acc += i;                    // add outer loop index
            for (int j = 1; j <= 2; ++j) {
                acc -= j;                // subtract inner loop index
            }
        }
        outputs.push_back(acc);          // result.write(inp_tmp)
    }

    return outputs;
}
// The main algorithm is a direct translation of the synchronous `while(1)` body into a single-pass function traversing the input vector. It uses two distinct phases: Phase 1 processes five outer iterations (`i = 1..5`). For each outer iteration, it reads the next input value into a temporary `int`, adds `i` to it, then runs an inner loop of four iterations that subtracts the original input value (not the incremented one) from the accumulator. After the inner loop, the result is pushed to the output vector. Phase 2 begins by reading one more input value as the initial accumulator, then runs an outer loop of three iterations (`i = 1..3`) with an inner loop of two iterations (`j = 1..2`) that adds `i` and subtracts `j` from the accumulator. After the loops, the final accumulator is pushed to the output. Key edge cases: input vector may be shorter than needed, so the function must check the input index before every read and return immediately if exhausted. The subtraction in Phase 1 uses the original input value, not the incremented temporary. Time complexity is O(N) where N is the number of inputs consumed (at most 6), and space complexity is O(1) auxiliary plus the output vector size (at most 6).
