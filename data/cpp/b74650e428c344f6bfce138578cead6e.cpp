// Write a C++ function `simulateProgramExecution` that takes a vector of strings representing instructions (each instruction is a single uppercase letter followed by an optional space and an integer argument, e.g., `"L 5"`, `"A 3"`, `"S 2"`, `"P"`, `"X filename"`, `"Z"`), a starting accumulator value, and a starting program counter index, and returns a struct `ProgramResult` containing the final accumulator value, the final program counter (index after the last executed instruction), and a boolean indicating whether the program finished normally (encountered `"Z"`) or terminated due to an invalid/out-of-range instruction. The function should simulate a simple machine with the following semantics: `"L n"` loads `n` into the accumulator, `"A n"` adds `n` to the accumulator, `"S n"` subtracts `n` from the accumulator, `"P"` does nothing (no-op) for simulation purposes, `"X filename"` is treated as a no-op (ignore filename), and `"Z"` ends the program. Execution proceeds sequentially from the starting PC; if at any point the PC is out of bounds (less than 0 or greater than or equal to the instruction count), the function returns a result with `finished = false`. The accumulator is a 32-bit integer, and you may assume no overflow occurs. The function should be const-correct and take the instruction vector by const reference.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Simple sequence: load 5, add 3, subtract 2, end
    {
        std::vector<std::string> prog = {"L 5", "A 3", "S 2", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 0, 0);
        assert(r.acc == 6);
        assert(r.pc == 4);
        assert(r.finished == true);
    }

    // No 'Z', runs off the end
    {
        std::vector<std::string> prog = {"A 1", "A 2"};
        ProgramResult r = simulateProgramExecution(prog, 10, 0);
        assert(r.acc == 13);
        assert(r.pc == 2);
        assert(r.finished == false);
    }

    // Starting PC out of bounds
    {
        std::vector<std::string> prog = {"L 1", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 0, 5);
        assert(r.acc == 0);
        assert(r.pc == 5);
        assert(r.finished == false);
    }

    // 'P' and 'X' are no-ops, 'Z' at end
    {
        std::vector<std::string> prog = {"P", "X file.txt", "L 7", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 1, 0);
        assert(r.acc == 7);
        assert(r.pc == 4);
        assert(r.finished == true);
    }

    // Empty program
    {
        std::vector<std::string> prog;
        ProgramResult r = simulateProgramExecution(prog, 42, 0);
        assert(r.acc == 42);
        assert(r.pc == 0);
        assert(r.finished == false);
    }

    // Negative values and subtraction
    {
        std::vector<std::string> prog = {"L -3", "S -2", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 0, 0);
        assert(r.acc == -1);
        assert(r.pc == 3);
        assert(r.finished == true);
    }

    // Start with non-zero accumulator
    {
        std::vector<std::string> prog = {"A 10", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 5, 0);
        assert(r.acc == 15);
        assert(r.pc == 2);
        assert(r.finished == true);
    }

    // No-op instruction with missing argument (just 'A')
    // Our handler treats value as 0, so acc unchanged
    {
        std::vector<std::string> prog = {"A", "Z"};
        ProgramResult r = simulateProgramExecution(prog, 3, 0);
        assert(r.acc == 3);
        assert(r.pc == 2);
        assert(r.finished == true);
    }

    // Run to out-of-bounds after several instructions
    {
        std::vector<std::string> prog = {"L 1", "A 2", "S 3"};
        ProgramResult r = simulateProgramExecution(prog, 0, 1);
        assert(r.acc == -1);
        assert(r.pc == 3);
        assert(r.finished == false);
    }
}

#include <string>
#include <vector>
#include <cstdint>

struct ProgramResult {
    int32_t acc;
    int pc;
    bool finished;
};

// Simulate a simple instruction set: L n, A n, S n, P, X filename, Z.
ProgramResult simulateProgramExecution(
    const std::vector<std::string>& instructions,
    int32_t startAcc,
    int startPC
) {
    int32_t acc = startAcc;
    int pc = startPC;
    int n = static_cast<int>(instructions.size());

    while (pc >= 0 && pc < n) {
        const std::string& instr = instructions[pc];
        char op = instr[0];
        int value = 0;
        if (instr.size() > 2) {
            value = std::stoi(instr.substr(2));
        }

        if (op == 'L') {
            acc = value;
        } else if (op == 'A') {
            acc += value;
        } else if (op == 'S') {
            acc -= value;
        } else if (op == 'Z') {
            // Program ends normally
            return {acc, pc, true};
        }
        // 'P' and 'X' are no-ops

        ++pc;
    }

    // Loop exited because PC went out of bounds
    return {acc, pc, false};
}

// The solution is a straightforward sequential simulation. We maintain two variables: `acc` (accumulator) and `pc` (program counter). We iterate while `pc` is within the bounds of the instruction vector and we haven't encountered `"Z"`. For each instruction, we parse the first character via `instruction[0]`, and if there is an argument (size > 2), extract it as a substring starting at index 2 and convert to integer using `std::stoi`. We then apply the corresponding operation: `'L'` sets acc to value, `'A'` adds, `'S'` subtracts, `'P'` and `'X'` do nothing, `'Z'` returns with finished=true. After each instruction, we increment `pc` by 1, and at the end we record the final acc and pc. Edge cases include: empty instruction list (should return finished=false because PC=0 is out of bounds), instruction with only a letter (e.g., `"S"` with no argument) — the `if` guard means value remains empty and `std::stoi` would be called only when operation expects an argument; to avoid crashes, for `L`/`A`/`S` we check if `command.size() > 2` before converting, else treat the value as 0. However, the simplest robust approach is to assume well-formed input per spec (though our reference handles missing argument by defaulting to 0). Time complexity is O(n) where n is the number of executed instructions, and space complexity is O(1) auxiliary (excluding the input vector and returned struct).
