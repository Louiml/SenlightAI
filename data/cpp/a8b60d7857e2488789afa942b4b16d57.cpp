Write a C++ function `string simulate8085(const string& instruction, bool* flag, string* registers, map<string,string>& memory, string pc)` that processes a single 8085 assembly instruction in a simplified emulator. The input instruction is a string with operands separated by spaces or commas (e.g., `"ADD B"`, `"MVI A, 05H"`, `"JMP 2000H"`). The function must parse the instruction into tokens and dispatch to the appropriate handler based on the opcode. Supported operations: `ADD`, `ADI`, `SUB`, `SUI`, `INR`, `DCR`, `INX`, `DCX`, `DAD`, `JMP`, `JC`, `JNC`, `JZ`, `JNZ`, `MOV`, `MVI`, `LXI`, `LDA`, `STA`, `SHLD`, `LHLD`, `STAX`, `XCHG`, `SET`, `CMP`, `CMA`. For arithmetic, logical, and data-transfer instructions (except jumps), the function calls `allocate_memory(opcode, pc)` and returns that value. For jumps, return the result of the corresponding jump handler function (e.g., `JMP(address)`). If the opcode is unknown, print `Instruction doesn't exist` and return an empty string. The function must not modify its inputs except through the provided pointer/reference parameters (registers, memory, flags) as done by the handlers. Assume all helper functions (`ADD`, `MOV`, `JMP`, `allocate_memory`, etc.) are already declared and defined elsewhere.
The core approach is tokenization: split the input string by spaces and commas, storing each token in a vector. The first token is the opcode; remaining tokens are operands. Then, use a series of `if`/`else if` comparisons to match the opcode and call the appropriate handler function with the exact arguments as shown in the snippet. For arithmetic/data-transfer/jump-conditional instructions, the return value is `allocate_memory(opcode, pc)`, except for unconditional and conditional jumps (`JMP`, `JC`, etc.) where the return value comes from the dedicated jump function. The function must handle edge cases: instructions with 1 or 2 operands (e.g., `CMA` has no operands, `MOV` has two), and ensure that parsing uses `strtok` correctly with `const_cast` since the input is a `const string` (a safer alternative would be `istringstream`, but the snippet uses `strtok`; we replicate that behavior). The time complexity is O(n) where n is the instruction length (due to tokenization), and each dispatch is O(1). Space complexity is O(k) where k is the number of tokens (small constant). Edge cases: empty input or malformed instruction (fewer tokens than needed) may cause undefined behavior in the handlers, so we assume valid input per the task spec.
#include <bits/stdc++.h>
using namespace std;

// External declarations (assumed provided elsewhere)
string allocate_memory(const string& opcode, const string& pc);
void ADD(const string& operand, string* regs, bool* flag, map<string,string>& mem);
void ADI(const string& operand, string* regs, bool* flag);
void SUB(const string& operand, string* regs, bool* flag, map<string,string>& mem);
void SUI(const string& operand, string* regs, bool* flag);
void INR(const string& operand, string* regs, bool* flag, map<string,string>& mem);
void DCR(const string& operand, string* regs, bool* flag, map<string,string>& mem);
void INX(const string& operand, string* regs, bool* flag);
void DCX(const string& operand, string* regs, bool* flag);
void DAD(const string& operand, string* regs, bool* flag);
string JMP(const string& operand, string* regs, bool* flag);
string JC(const string& operand, const string& pc, string* regs, bool* flag);
string JNC(const string& operand, const string& pc, string* regs, bool* flag);
string JZ(const string& operand, const string& pc, string* regs, bool* flag);
string JNZ(const string& operand, const string& pc, string* regs, bool* flag);
void MOV(const string& dest, const string& src, string* regs, bool* flag, map<string,string>& mem);
void MVI(const string& dest, const string& val, string* regs, bool* flag, map<string,string>& mem);
void LXI(const string& pair, const string& val, string* regs, bool* flag, map<string,string>& mem);
void LDA(const string& addr, string* regs, bool* flag, map<string,string>& mem);
void STA(const string& addr, string* regs, bool* flag, map<string,string>& mem);
void SHLD(const string& addr, string* regs, bool* flag, map<string,string>& mem);
void LHLD(const string& addr, string* regs, bool* flag, map<string,string>& mem);
void STAX(const string& pair, string* regs, bool* flag, map<string,string>& mem);
void XCHG(string* regs, bool* flag);
void SET(const string& addr, const string& val, map<string,string>& mem);
void CMP(const string& operand, string* regs, bool* flag, map<string,string>& mem);
void CMA(const string& opcode, string* regs, bool* flag);

// Simulates one 8085 instruction, returns next PC update.
string execute_statement(const string& input, bool* flag, string* registers, map<string,string>& memory, const string& pc) {
    // Tokenize input by spaces and commas
    vector<string> tokens;
    string current;
    for (char ch : input) {
        if (ch == ' ' || ch == ',') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
    }
    if (!current.empty()) tokens.push_back(current);
    
    if (tokens.empty()) return "";
    const string& opcode = tokens[0];
    
    // Dispatch based on opcode
    if (opcode == "ADD") {
        ADD(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "ADI") {
        ADI(tokens[1], registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "SUB") {
        SUB(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "SUI") {
        SUI(tokens[1], registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "INR") {
        INR(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "DCR") {
        DCR(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "INX") {
        INX(tokens[1], registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "DCX") {
        DCX(tokens[1], registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "DAD") {
        DAD(tokens[1], registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "JMP") {
        return JMP(tokens[1], registers, flag);
    } else if (opcode == "JC") {
        return JC(tokens[1], pc, registers, flag);
    } else if (opcode == "JNC") {
        return JNC(tokens[1], pc, registers, flag);
    } else if (opcode == "JZ") {
        return JZ(tokens[1], pc, registers, flag);
    } else if (opcode == "JNZ") {
        return JNZ(tokens[1], pc, registers, flag);
    } else if (opcode == "MOV") {
        MOV(tokens[1], tokens[2], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "MVI") {
        MVI(tokens[1], tokens[2], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "LXI") {
        LXI(tokens[1], tokens[2], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "LDA") {
        LDA(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "STA") {
        STA(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "SHLD") {
        SHLD(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "LHLD") {
        LHLD(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "STAX") {
        STAX(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "XCHG") {
        XCHG(registers, flag);
        return allocate_memory(opcode, pc);
    } else if (opcode == "SET") {
        SET(tokens[1], tokens[2], memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "CMP") {
        CMP(tokens[1], registers, flag, memory);
        return allocate_memory(opcode, pc);
    } else if (opcode == "CMA") {
        CMA(opcode, registers, flag);
        return allocate_memory(opcode, pc);
    } else {
        cout << "Instruction doesn't exist\n";
        return "";
    }
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Minimal stub implementations for testing
string allocate_memory(const string& opcode, const string& pc) { return "PC+" + opcode + "+" + pc; }
string last_operand;
void ADD(const string& op, string* r, bool* f, map<string,string>& m) { last_operand = op; }
void ADI(const string& op, string* r, bool* f) { last_operand = op; }
void SUB(const string& op, string* r, bool* f, map<string,string>& m) { last_operand = op; }
void SUI(const string& op, string* r, bool* f) { last_operand = op; }
void INR(const string& op, string* r, bool* f, map<string,string>& m) { last_operand = op; }
void DCR(const string& op, string* r, bool* f, map<string,string>& m) { last_operand = op; }
void INX(const string& op, string* r, bool* f) { last_operand = op; }
void DCX(const string& op, string* r, bool* f) { last_operand = op; }
void DAD(const string& op, string* r, bool* f) { last_operand = op; }
string JMP(const string& op, string* r, bool* f) { return "JUMP:" + op; }
string JC(const string& op, const string& pc, string* r, bool* f) { return "JC:" + op; }
string JNC(const string& op, const string& pc, string* r, bool* f) { return "JNC:" + op; }
string JZ(const string& op, const string& pc, string* r, bool* f) { return "JZ:" + op; }
string JNZ(const string& op, const string& pc, string* r, bool* f) { return "JNZ:" + op; }
void MOV(const string& d, const string& s, string* r, bool* f, map<string,string>& m) { last_operand = d + "," + s; }
void MVI(const string& d, const string& v, string* r, bool* f, map<string,string>& m) { last_operand = d + "," + v; }
void LXI(const string& p, const string& v, string* r, bool* f, map<string,string>& m) { last_operand = p + "," + v; }
void LDA(const string& a, string* r, bool* f, map<string,string>& m) { last_operand = a; }
void STA(const string& a, string* r, bool* f, map<string,string>& m) { last_operand = a; }
void SHLD(const string& a, string* r, bool* f, map<string,string>& m) { last_operand = a; }
void LHLD(const string& a, string* r, bool* f, map<string,string>& m) { last_operand = a; }
void STAX(const string& p, string* r, bool* f, map<string,string>& m) { last_operand = p; }
void XCHG(string* r, bool* f) { last_operand = "XCHG"; }
void SET(const string& a, const string& v, map<string,string>& m) { last_operand = a + "," + v; }
void CMP(const string& op, string* r, bool* f, map<string,string>& m) { last_operand = op; }
void CMA(const string& o, string* r, bool* f) { last_operand = "CMA"; }

// Include the solution function (copy here for test)
// ... (place the solution function code above)

int main() {
    bool flag[5] = {false, false, false, false, false};
    string regs[8] = {"00","01","02","03","04","05","06","07"};
    map<string,string> memory;
    
    // Test ADD with comma
    string res1 = execute_statement("ADD B,", flag, regs, memory, "3000");
    assert(res1 == "PC+ADD+3000");
    assert(last_operand == "B");
    
    // Test MOV with space
    string res2 = execute_statement("MOV A,B", flag, regs, memory, "3001");
    assert(res2 == "PC+MOV+3001");
    assert(last_operand == "A,B");
    
    // Test JMP (no allocate_memory)
    string res3 = execute_statement("JMP 2000H", flag, regs, memory, "3002");
    assert(res3 == "JUMP:2000H");
    
    // Test JC with comma
    string res4 = execute_statement("JC,2001H", flag, regs, memory, "3003");
    assert(res4 == "JC:2001H");
    
    // Test MVI with data
    string res5 = execute_statement("MVI A, 05H", flag, regs, memory, "3004");
    assert(res5 == "PC+MVI+3004");
    assert(last_operand == "A,05H");
    
    // Test unknown instruction
    string res6 = execute_statement("HLT", flag, regs, memory, "3005");
    assert(res6 == "");
    
    // Test XCHG no operands
    string res7 = execute_statement("XCHG", flag, regs, memory, "3006");
    assert(res7 == "PC+XCHG+3006");
    assert(last_operand == "XCHG");
    
    // Test LXI multiple tokens with comma
    string res8 = execute_statement("LXI H, 2500H", flag, regs, memory, "3007");
    assert(res8 == "PC+LXI+3007");
    assert(last_operand == "H,2500H");
    
    // Test JNZ with pc still returned
    string res9 = execute_statement("JNZ 4000H", flag, regs, memory, "3008");
    assert(res9 == "JNZ:4000H");
    
    // Test CMA
    string res10 = execute_statement("CMA", flag, regs, memory, "3009");
    assert(res10 == "PC+CMA+3009");
    
    cout << "All tests passed!\n";
    return 0;
}
