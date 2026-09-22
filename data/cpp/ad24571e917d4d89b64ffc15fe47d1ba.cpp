Given a 4-bit register virtual machine program encoded as a byte vector, write a C++ function `bool successfulProgram(const std::vector<uint8_t>& program)` that decodes and executes instructions in the given format: each instruction consists of a 1-byte opcode followed by an 8-byte little-endian encoding. Valid opcodes are: 1=halt, 2=add, 3=mul, 4=cmp, 5=jmp. For `add`, `mul`, the encoding’s low 4 bits select a register index (0-14), and the next 4 bits are a sign-extended 4-bit immediate added/multiplied into that register (also sign-extended to 64 bits). For `cmp`, the low 4 bits are left register, next 4 bits are right register, and the next 4 bits must be zero; if `reg[left] == reg[right]`, set the zero flag (bit 1). For `jmp`, the low 4 bits are a condition (0=unconditional jump, 1=jump if zero flag set), and the next 4 bits are a sign-extended 4-bit offset added to the instruction pointer. `halt` must have encoding equal to `0xdeadbeef` and stops execution. The program header is not included in the vector; the vector begins directly with instructions. Execution starts at IP=0, and after each instruction (except jump which may modify IP before increment) increments IP by 1 (i.e., each instruction occupies 9 bytes). If IP goes out of bounds of the vector, the program fails. The VM has 15 64-bit registers initialized to zero, and flags initially 0. The function returns `true` if execution reaches halt and the winning flag bit (bit 4) is set—however note: the winning flag is never set by any instruction, so the function should simply return `true` if a valid `halt` is reached; return `false` on any error (invalid register index >14, invalid cmp encoding upper bits nonzero, invalid halt encoding, invalid jump condition, out-of-bounds IP, or program empty). All operations use unsigned 64-bit wraparound arithmetic. The byte vector length is guaranteed to be a multiple of 9 bytes per instruction; there is no separate header. Write the function to be robust and not modify any global state.
The solution decodes each instruction from the byte vector with a position `pos` starting at 0. For each step, if `pos` is not a valid multiple-of-9 offset, the program is malformed. Read opcode byte at `pos`, then read 8 bytes as a little-endian `uint64_t` using bit shifts or `memcpy`. Based on opcode, parse encoding: for add/mul, extract low 4 bits as register index (validate ≤14), shift right 4, sign-extend the low 4 bits of the remainder to 64 bits using arithmetic shift (`(int64_t)(encoding << 60) >> 60` or manually). Apply operation to register array, using unsigned wraparound (no overflow checks). For cmp, extract registers from low 4 and next 4 bits, shift right 8, require the rest be zero; if left==right, set flag bit 1. For jmp, condition from low 4 bits (must be 0 or 1), shift right 4, sign-extend next 4 bits; if condition==0 or (condition==1 and zero flag set), add offset to IP (using signed addition on `int64_t` but ensure IP stays non-negative; if it becomes negative, treat as out-of-bounds). For halt, check encoding == 0xdeadbeef, then set `halted` flag. After decoding and executing, if not a jump that modified IP, increment IP by 9 (the size of one instruction). If IP becomes exactly equal to vector size, that is out-of-bounds unless halt was reached—so after halt, break. Return true only if halted was reached and no error occurred. Edge cases: empty program returns false; register index 15 is invalid; sign extension for immediate uses 4 bits; jump offset can be negative; when jump modifies IP, the increment by 9 is skipped. Time complexity O(number of instructions), space O(1) besides the register array.
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>

// Simulates the VM program and returns true if it halts successfully.
bool successfulProgram(const std::vector<uint8_t>& program) {
    const size_t INSTR_SIZE = 9; // 1 byte opcode + 8 byte encoding
    const size_t NUM_REGS = 15;
    const uint64_t ZERO_FLAG = 1;
    const uint64_t WIN_FLAG = 4; // not used to set, just keep

    if (program.empty() || program.size() % INSTR_SIZE != 0) {
        return false;
    }

    uint64_t regs[NUM_REGS] = {0};
    uint64_t flags = 0;
    size_t ip = 0; // instruction pointer in bytes
    bool stopped = false;

    auto fetch = [&](size_t offset) -> bool {
        // no actual fetch needed beyond reading bytes
        return offset + INSTR_SIZE <= program.size();
    };

    while (!stopped) {
        if (ip + INSTR_SIZE > program.size()) {
            return false; // out-of-bounds
        }

        uint8_t opcode = program[ip];
        uint64_t encoding = 0;
        // read 8 bytes little-endian
        for (size_t i = 0; i < 8; ++i) {
            encoding |= static_cast<uint64_t>(program[ip + 1 + i]) << (8 * i);
        }

        size_t next_ip = ip + INSTR_SIZE; // default advance

        switch (opcode) {
            case 1: { // halt
                if (encoding != 0xdeadbeefULL) {
                    return false;
                }
                stopped = true;
                break;
            }
            case 2: { // add
                size_t reg = encoding & 0xF;
                if (reg >= NUM_REGS) return false;
                int64_t imm = (encoding >> 4) & 0xF;
                // sign extend 4-bit to 64
                imm = (imm << 60) >> 60;
                regs[reg] = static_cast<uint64_t>(static_cast<int64_t>(regs[reg]) + imm); // wraparound
                break;
            }
            case 3: { // mul
                size_t reg = encoding & 0xF;
                if (reg >= NUM_REGS) return false;
                int64_t imm = (encoding >> 4) & 0xF;
                imm = (imm << 60) >> 60;
                regs[reg] = static_cast<uint64_t>(static_cast<int64_t>(regs[reg]) * imm);
                break;
            }
            case 4: { // cmp
                size_t left = encoding & 0xF;
                size_t right = (encoding >> 4) & 0xF;
                if (left >= NUM_REGS || right >= NUM_REGS) return false;
                uint64_t rest = encoding >> 8;
                if (rest != 0) return false;
                if (regs[left] == regs[right]) {
                    flags |= ZERO_FLAG;
                }
                break;
            }
            case 5: { // jmp
                uint64_t cond = encoding & 0xF;
                if (cond > 1) return false;
                int64_t offset = (encoding >> 4) & 0xF;
                offset = (offset << 60) >> 60;
                bool jump = (cond == 0) || (cond == 1 && (flags & ZERO_FLAG));
                if (jump) {
                    // signed addition might go negative
                    int64_t new_ip = static_cast<int64_t>(ip) + offset * INSTR_SIZE; // offset is in instructions
                    if (new_ip < 0) return false;
                    ip = static_cast<size_t>(new_ip);
                    next_ip = ip; // no extra increment
                    if (ip % INSTR_SIZE != 0 || ip >= program.size()) return false; // must align
                }
                break;
            }
            default:
                return false;
        }

        ip = next_ip;
    }

    // halt reached successfully
    return true;
}
#include <cassert>
#include <vector>
#include <cstdint>

bool successfulProgram(const std::vector<uint8_t>& program);

int main() {
    // Simple halt
    std::vector<uint8_t> halt = {1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0};
    assert(successfulProgram(halt));

    // Add then halt
    std::vector<uint8_t> add = {
        2, 0, 0, 0, 0, 0, 0, 0, 0,  // add reg0 += 0 (imm 0)
        1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0
    };
    assert(successfulProgram(add));

    // Mul with sign-extended negative imm: reg0 = 5, imm = -1 => reg0 becomes -5
    std::vector<uint8_t> mul_neg = {
        2, 0, 0, 0, 0, 0, 0, 0, 0,  // reg0 = 0
        2, 0, 0, 0, 0, 0, 0, 0, 0,  // unused add
        // To test mul, set reg0 via add with imm=5
        2, 5, 0, 0, 0, 0, 0, 0, 0,  // add reg0 += 5
        3, 0, 0xF, 0, 0, 0, 0, 0, 0,  // mul reg0 *= -1 (encoding low 4=0, next 4=15 -> -1)
        1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0
    };
    assert(successfulProgram(mul_neg));

    // Invalid register index
    std::vector<uint8_t> bad_reg = {2, 0xF, 0, 0, 0, 0, 0, 0, 0};
    assert(!successfulProgram(bad_reg));

    // Cmp with non-zero upper bits
    std::vector<uint8_t> bad_cmp = {4, 0, 0, 1, 0, 0, 0, 0, 0};
    assert(!successfulProgram(bad_cmp));

    // Jump valid: unconditional jump to halt at ip=9 (offset=1)
    std::vector<uint8_t> jmp = {
        5, 0, 0, 0, 0, 0, 0, 0, 0,  // jmp offset 0 (to itself) but let's do offset 1
        1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0
    };
    // Modify encoding: jmp instruction at ip=0, offset=1 => encoding low 4=0, next 4=1 => 0x10
    jmp[1] = 0x10; // little-endian: byte0 = low 8 bits of encoding, so 0x10
    assert(successfulProgram(jmp));

    // Jump with negative offset: need loop but could cause infinite; test with jump to halt from second instruction
    std::vector<uint8_t> jmp_neg = {
        5, 0x0F, 0, 0, 0, 0, 0, 0, 0,  // jmp offset -1 (encoding 0x0F)
        1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0
    };
    // At ip=0, jmp offset -1 -> new ip = 0 + (-1)*9 = -9 invalid, so false
    assert(!successfulProgram(jmp_neg));

    // JZ with zero flag set via cmp: set two regs equal, then jump
    std::vector<uint8_t> jz = {
        2, 0, 0, 0, 0, 0, 0, 0, 0,  // add reg0 +=0
        4, 0, 0, 0, 0, 0, 0, 0, 0,  // cmp reg0, reg0 -> zero flag set (both 0)
        5, 0x11, 0, 0, 0, 0, 0, 0, 0,  // jz offset 1 (condition=1, offset=1)
        1, 0xef, 0xbe, 0xad, 0xde, 0, 0, 0, 0  // halt
    };
    // encoding: low 4 = 1 (JZ), next 4 = 1 (offset) => 0x11
    assert(successfulProgram(jz));

    // Out-of-bounds without halt
    std::vector<uint8_t> no_halt = {2, 0, 0, 0, 0, 0, 0, 0, 0};
    assert(!successfulProgram(no_halt));

    // Empty program
    std::vector<uint8_t> empty;
    assert(!successfulProgram(empty));

    return 0;
}
