/*
Given a vector of unsigned 32-bit integers representing a program in the Universal Machine (UM) virtual machine, write a C++ function `void runUM(const std::vector<uint32_t>& program, std::vector<uint32_t>& output)` that executes the program starting at instruction 0 and appends each value produced by the `Output` instruction (opcode 10, register C) to the `output` vector in the order they are emitted. The function must implement all 14 opcodes exactly as described in the snippet: conditional move (0), array get (1), array set (2), add (3), multiply (4), divide (5), bitwise NAND (6), halt (7), allocation (8), abandonment (9), output (10), input (11), load program (12), and load immediate (13). The machine has 8 registers (R0–R7) initially zero, an initial array (array 0) containing the given program, and supports dynamic memory allocation for new arrays. For `Input` (opcode 11), treat EOF as `0xFFFFFFFF` (i.e., the value returned by `getchar` on EOF cast to unsigned). For division by zero, set the result register to 0. For invalid opcode (including halt opcode 7) or when the instruction pointer exceeds the current array length, terminate execution. Ensure that after `LoadProg` (opcode 12) with a non-zero register B, the instruction stream switches to a copy of the array whose index is in register B, and the instruction pointer is set to register C (but for our function, we only need to continue execution, not produce any output about it). The function must not print anything to stdout or stderr; all output is collected in the `output` vector. The original snippet's final printing and file I/O are omitted; the function is purely a simulation.
*/
#include <cstdint>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

// Execute a UM program, appending all output values to the provided vector.
void runUM(const std::vector<uint32_t>& program, std::vector<uint32_t>& output) {
    // Registers R0-R7, initially zero.
    uint32_t reg[8] = {0};
    // Instruction pointer.
    uint32_t ip = 0;
    // Memory: vector of pointers to arrays (each array is vector<uint32_t>).
    std::vector<std::vector<uint32_t>*> memory;
    // Array 0 contains a copy of the program.
    memory.push_back(new std::vector<uint32_t>(program));
    // Free list for abandoned array indices.
    std::list<uint32_t> free_indices;

    // Helper to extract fields.
    auto getA = [](uint32_t c) -> uint32_t { return (c >> 6) & 0x7; };
    auto getB = [](uint32_t c) -> uint32_t { return (c >> 3) & 0x7; };
    auto getC = [](uint32_t c) -> uint32_t { return c & 0x7; };
    auto getR = [](uint32_t c) -> uint32_t { return (c >> 25) & 0x7; };
    auto getV = [](uint32_t c) -> uint32_t { return c & 0x1FFFFFF; };

    // Execute until halt or invalid state.
    while (true) {
        // Bounds check: if ip is beyond current array 0, halt.
        if (ip >= memory[0]->size()) {
            break;
        }
        uint32_t code = (*memory[0])[ip];
        uint32_t opcode = code >> 28;
        switch (opcode) {
            case 0: { // Conditional move
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                if (reg[c] != 0) reg[a] = reg[b];
                ++ip;
                break;
            }
            case 1: { // Array get
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                reg[a] = (*memory[reg[b]])[reg[c]];
                ++ip;
                break;
            }
            case 2: { // Array set
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                (*memory[reg[a]])[reg[b]] = reg[c];
                ++ip;
                break;
            }
            case 3: { // Add
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                reg[a] = reg[b] + reg[c];
                ++ip;
                break;
            }
            case 4: { // Multiply
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                reg[a] = reg[b] * reg[c];
                ++ip;
                break;
            }
            case 5: { // Divide
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                if (reg[c] != 0) {
                    reg[a] = reg[b] / reg[c];
                } else {
                    reg[a] = 0; // Undefined behavior; set to 0.
                }
                ++ip;
                break;
            }
            case 6: { // NAND
                uint32_t a = getA(code), b = getB(code), c = getC(code);
                reg[a] = ~(reg[b] & reg[c]);
                ++ip;
                break;
            }
            case 7: { // Halt
                return;
            }
            case 8: { // Allocate
                uint32_t b = getB(code), c = getC(code);
                uint32_t idx;
                if (!free_indices.empty()) {
                    idx = free_indices.front();
                    free_indices.pop_front();
                    delete memory[idx];
                    memory[idx] = new std::vector<uint32_t>(reg[c], 0);
                } else {
                    idx = static_cast<uint32_t>(memory.size());
                    memory.push_back(new std::vector<uint32_t>(reg[c], 0));
                }
                reg[b] = idx;
                ++ip;
                break;
            }
            case 9: { // Abandon
                uint32_t c = getC(code);
                uint32_t idx = reg[c];
                delete memory[idx];
                memory[idx] = nullptr;
                free_indices.push_back(idx);
                ++ip;
                break;
            }
            case 10: { // Output
                uint32_t c = getC(code);
                output.push_back(reg[c]);
                ++ip;
                break;
            }
            case 11: { // Input
                uint32_t c = getC(code);
                int ch = std::cin.get();
                if (ch == EOF) {
                    reg[c] = 0xFFFFFFFF;
                } else {
                    reg[c] = static_cast<uint32_t>(ch);
                }
                ++ip;
                break;
            }
            case 12: { // Load program
                uint32_t b = getB(code), c = getC(code);
                if (reg[b] != 0) {
                    // Copy array at reg[b] into a new vector.
                    std::vector<uint32_t>* old0 = memory[0];
                    memory[0] = new std::vector<uint32_t>(*memory[reg[b]]);
                    delete old0;
                }
                ip = reg[c];
                break;
            }
            case 13: { // Load immediate
                uint32_t r = getR(code), v = getV(code);
                reg[r] = v;
                ++ip;
                break;
            }
            default: { // Invalid opcode
                return;
            }
        }
    }

    // Clean up memory.
    for (auto* vec : memory) {
        delete vec;
    }
    memory.clear();
}
#include <cassert>
#include <cstdint>
#include <vector>

// Forward declaration of the function under test.
void runUM(const std::vector<uint32_t>& program, std::vector<uint32_t>& output);

int main() {
    // Test 1: Simple program that outputs 42 and halts.
    // Opcode 13 (load imm) with R=0, V=42: (13<<28) | (0<<25) | 42
    // Opcode 10 (output) with C=0: (10<<28) | 0
    // Opcode 7 (halt): (7<<28)
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 42u,
            (10u << 28) | 0u,
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1);
        assert(out[0] == 42u);
    }

    // Test 2: Program that adds two numbers (R0=2, R1=3, R2=R0+R1, output R2).
    // Load R0=2: (13<<28)|(0<<25)|2
    // Load R1=3: (13<<28)|(1<<25)|3
    // Add R2=R0+R1: opcode 3, A=2, B=0, C=1 => (3<<28) | (2<<6) | (0<<3) | 1
    // Output R2: (10<<28) | (2) [C=2]
    // Halt
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 2u,
            (13u << 28) | (1u << 25) | 3u,
            (3u << 28) | (2u << 6) | (0u << 3) | 1u,
            (10u << 28) | 2u,
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1);
        assert(out[0] == 5u);
    }

    // Test 3: Program that outputs two values from two independent runs? 
    // Instead, test that the function is re-entrant: call it twice with same program.
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 7u,
            (10u << 28) | 0u,
            (7u << 28)
        };
        std::vector<uint32_t> out1, out2;
        runUM(prog, out1);
        runUM(prog, out2);
        assert(out1 == out2);
        assert(out1.size() == 1 && out1[0] == 7u);
    }

    // Test 4: Program that uses array allocation and array get/set.
    // Allocate array of size 2 into R0 (opcode 8, B=0, C=1? Actually B=0, C=1 means size 1? Let's set size 1 for simplicity.
    //   Alloc: opcode 8, B=0, C=2 -> size 2? Use C=1 for size 1.
    //   Store 99 into array[0]: set R1=99, then ArraySet A=R0, B=0, C=R1.
    //   Get from array[0] into R2: ArrayGet A=2, B=0, C=0.
    //   Output R2.
    //   Halt.
    {
        std::vector<uint32_t> prog = {
            (8u << 28) | (0u << 3) | 1u,            // Alloc size 1 into R0
            (13u << 28) | (1u << 25) | 99u,         // R1=99
            (2u << 28) | (0u << 6) | (0u << 3) | 1u, // ArraySet A=0 (R0), B=0, C=1 (R1)
            (1u << 28) | (2u << 6) | (0u << 3) | 0u, // ArrayGet A=2, B=0, C=0
            (10u << 28) | 2u,                       // Output R2
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1);
        assert(out[0] == 99u);
    }

    // Test 5: Program with division by zero sets result to 0.
    // R0=5, R1=0, R2=R0/R1, output R2.
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 5u,
            (13u << 28) | (1u << 25) | 0u,
            (5u << 28) | (2u << 6) | (0u << 3) | 1u,
            (10u << 28) | 2u,
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1);
        assert(out[0] == 0u);
    }

    // Test 6: Conditional move: if R1 != 0, copy R1 to R0.
    // R0=10, R1=0, CMov R0=R1 doesn't happen, output R0=10.
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 10u,
            (13u << 28) | (1u << 25) | 0u,
            (0u << 28) | (0u << 6) | (1u << 3) | 1u, // CMov A=0, B=1, C=1 (check R1)
            (10u << 28) | 0u,
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1 && out[0] == 10u);
    }

    // Test 7: NAND: R0=0x0F, R1=0xF0, NAND => ~(0x0F & 0xF0) = ~0x00 = 0xFFFFFFFF.
    // Output R2.
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (0u << 25) | 0x0Fu,
            (13u << 28) | (1u << 25) | 0xF0u,
            (6u << 28) | (2u << 6) | (0u << 3) | 1u,
            (10u << 28) | 2u,
            (7u << 28)
        };
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.size() == 1);
        assert(out[0] == 0xFFFFFFFFu);
    }

    // Test 8: Program that terminates when instruction pointer runs off the end without halt.
    // One NOP-like instruction then ends.
    {
        std::vector<uint32_t> prog = { (3u << 28) | (0u << 6) | (0u << 3) | 0u }; // Add R0=R0+R0
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.empty());
    }

    // Test 9: Load program (opcode 12) changes instruction pointer and array 0.
    // R0 = 0 (default), R1 = 1 (index of array 1? We need array 1 to exist).
    // First allocate array size 2 into R0, then set array[0] to an instruction sequence.
    // But to keep simple: We'll manually construct memory with array 1 as a small program.
    // This is complex; instead test opcode 12 with B=0 (no change) and C as jump.
    // R0=0, R1=2, then load program with B=0, C=2 (set ip=2) then output something from R1.
    // Actually opcode 12 with B=0 just sets ip=reg[C].
    {
        std::vector<uint32_t> prog = {
            (13u << 28) | (1u << 25) | 123u,     // R1=123
            (12u << 28) | (0u << 3) | 1u,        // LoadProg B=0, C=1 -> jump to ip=1? But that would loop? 
            // To avoid infinite, we'll jump to ip=3 (skip the halt) and output.
            // Let's craft: R1=42, then LoadProg with C=3, then at index 3 output R1, halt.
        };
        // Build a longer program:
        std::vector<uint32_t> p2 = {
            (13u << 28) | (1u << 25) | 42u,      // 0: R1=42
            (12u << 28) | (0u << 3) | 3u,        // 1: LoadProg B=0, C=3 -> ip=3
            (7u << 28),                          // 2: Halt (not executed)
            (10u << 28) | 1u,                    // 3: Output R1
            (7u << 28)                           // 4: Halt
        };
        std::vector<uint32_t> out;
        runUM(p2, out);
        assert(out.size() == 1 && out[0] == 42u);
    }

    // Test 10: Empty program yields no output.
    {
        std::vector<uint32_t> prog;
        std::vector<uint32_t> out;
        runUM(prog, out);
        assert(out.empty());
    }

    return 0;
}
// The core challenge is to faithfully implement the UM instruction set with careful attention to memory management, array aliasing, and the dynamic nature of arrays. The solution initializes an `std::vector<std::vector<uint32_t>*>` to store pointers to arrays; index 0 holds a copy of the input program. A free-list (`std::list<uint32_t>`) tracks abandoned array indices for reuse. Registers are an array of 8 `uint32_t`. The main loop reads a 32-bit instruction from the currently active array (index 0, but possibly replaced by `LoadProg`). Opcode extraction uses `code >> 28`; operand fields are extracted via masks: A = `(code>>6)&0x7`, B = `(code>>3)&0x7`, C = `code&0x7`, and for load-immediate: R = `(code>>25)&0x7`, V = `code&0x1FFFFFF`. For each opcode, we implement the corresponding operation using `std::vector` methods, ensuring bounds access is safe (since the VM's semantics expect valid indices; we assume valid programs). For division, guard against zero denominator by setting result to 0. For input, read a character from `std::cin`; if EOF, store `0xFFFFFFFF`. Output appends the register value to the output vector. For allocation, either reuse a freed index or push a new vector of size `reg[C]` (all elements initialized to 0). For abandonment, delete the vector and add index to free-list. For load-program, if `reg[B] != 0`, copy the array at that index into a freshly allocated vector, delete the old array 0, and set array 0 to the new copy; then set instruction pointer to `reg[C]`. Halt occurs on opcode 7 (since default case returns false in snippet) or if instruction pointer goes out of bounds. The loop continues until halt or invalid state. Time complexity is O(N) where N is the total number of executed instructions, with O(M) space for allocated arrays, where M is the total memory allocated during execution. Edge cases include: empty program (executes zero times), program ending without explicit halt (terminates when IP out of bounds), division by zero, input at EOF, and memory reuse after abandonment.
