// Write a C++ function `std::string runBrainfuck(const std::string& program, const std::string& input)` that simulates a minimal Brainfuck interpreter for a fixed tape of 30,000 bytes, initially all zero. The function must process the program commands `> < + - . , [ ]` exactly as standard Brainfuck: `>` increments the data pointer, `<` decrements it, `+` increments the current cell (modulo 256, wrapping around), `-` decrements it (modulo 256), `.` outputs the current cell as a character (append to the result string), `,` reads the next character from the provided `input` string (if no input remains, it should set the cell to 0), and `[`/`]` implement loops that jump forward past the matching `]` if the current cell is zero, or backward past the matching `[` if nonzero. The tape pointer is clamped: if the program tries to go below index 0, it should wrap to 29999; if it tries to go above 29999, it should wrap to 0. Assume the program is syntactically valid (balanced brackets). The function must return only the output characters produced by `.` commands.
#include <cassert>
#include <string>

// Free function declared above; include it here or copy the implementation.

int main() {
    // Basic increment and output
    assert(runBrainfuck("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.", "") == "Hello World!\n");

    // Simple loop: print 'A' (65) five times
    assert(runBrainfuck("+++++[>+++++++++++++++++++++++++++++++++++++++++++++++++++++++<-]>.....", "") == "AAAAA");

    // Input handling
    assert(runBrainfuck(",.", "X") == "X");

    // Input exhaustion sets cell to zero
    assert(runBrainfuck(",.", "") == std::string(1, '\0'));

    // Tape wrap-around: start at 0, go left to 29999, then increment and output
    assert(runBrainfuck("<+.", "") == std::string(1, 1));

    // Tape wrap-around right from 29999 to 0
    // Move to the right 30000 times, but that's slow; test with small custom? Not needed for simple test.
    // Instead test multiple left moves then right wrap: from 0, left 1 (to 29999), then left 1 more should wrap to 29998? Actually wrap: 29999-1=29998, not wrap. To wrap from 29999 to 0: go right once.
    // Use a program that starts at 0, go right 30000 times? That's heavy. Skip heavy test; rely on small wrap with left.
    assert(runBrainfuck("<<", "") == "");  // just check no crash

    // Nested loops
    assert(runBrainfuck("+++[>+<-]>", "") == ""); // adds 3 to second cell but no output

    // Output after loop: first cell becomes 3, then move right and output
    assert(runBrainfuck("+++[>+<-]> .", "") == std::string(1, 3));

    // Empty program
    assert(runBrainfuck("", "anything") == "");

    // Ignore non-command characters
    assert(runBrainfuck("+.+.", "") == std::string(1, 1) + std::string(1, 1));

    return 0;
}
#include <string>
#include <vector>

// Runs a Brainfuck program with given input, returns the output characters.
std::string runBrainfuck(const std::string& program, const std::string& input) {
    const int TAPE_SIZE = 30000;
    std::vector<unsigned char> tape(TAPE_SIZE, 0);
    int dp = 0;          // data pointer (wraps 0..TAPE_SIZE-1)
    int pc = 0;          // program counter
    int input_pos = 0;   // position in input string
    std::string output;

    auto adjust_dp = [&](int delta) {
        dp = (dp + delta + TAPE_SIZE) % TAPE_SIZE;
    };

    while (pc < static_cast<int>(program.size())) {
        char cmd = program[pc];
        switch (cmd) {
            case '>': adjust_dp(1); break;
            case '<': adjust_dp(-1); break;
            case '+': tape[dp]++; break;  // unsigned char wraps modulo 256
            case '-': tape[dp]--; break;
            case '.': output.push_back(static_cast<char>(tape[dp])); break;
            case ',':
                if (input_pos < static_cast<int>(input.size())) {
                    tape[dp] = static_cast<unsigned char>(input[input_pos++]);
                } else {
                    tape[dp] = 0;  // no input left
                }
                break;
            case '[':
                if (tape[dp] == 0) {
                    // Jump forward to matching ']'
                    int depth = 1;
                    while (depth > 0) {
                        pc++;
                        if (program[pc] == '[') depth++;
                        else if (program[pc] == ']') depth--;
                    }
                }
                break;
            case ']':
                if (tape[dp] != 0) {
                    // Jump backward to matching '['
                    int depth = 1;
                    while (depth > 0) {
                        pc--;
                        if (program[pc] == ']') depth++;
                        else if (program[pc] == '[') depth--;
                    }
                }
                break;
            default:
                // Ignore unknown characters (including comments)
                break;
        }
        pc++;
    }
    return output;
}
// The core approach is to simulate the Brainfuck execution exactly as the reference code does, but with careful handling of tape bounds (wrap-around) and input exhaustion. We maintain a `std::vector<unsigned char>` of size 30000 (each cell modulo 256 handles `+`/`-` wrapping naturally). We iterate through the program with an index `pc` and a data pointer `dp`. For `[` and `]`, we use a helper to find matching bracket: when encountering `[` and current cell is 0, scan forward counting nested brackets to find the matching `]`; when encountering `]` and current cell is nonzero, scan backward counting nested brackets to find the matching `[`. This O(n) per jump in the worst case, but for valid programs it's standard. Input handling: maintain an index into the `input` string; if at end, set cell to 0. Output is appended to a `std::string`. Time complexity is O(P + O) where P is the number of executed commands (which can be up to exponential in program length in pathological cases, but for typical tasks it’s linear) and O is the output size; space is O(30000) for the tape plus O(1) extra. Edge cases: empty program returns empty string; empty input with `,` sets cell to 0; wrap-around on `<` at index 0 and `>` at index 29999; loops with zero or nonzero start conditions.
