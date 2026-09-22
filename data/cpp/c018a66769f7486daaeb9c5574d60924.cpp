/*
Write a C++ function that simulates a simple memory allocation system with three operations: allocate a block of contiguous free memory of a given size, erase a previously allocated block by its identifier, and defragment the memory by compacting all allocated blocks to the front. The function should take the total memory size `m` and a vector of command strings (in the format `"alloc <size>"`, `"erase <id>"`, or `"defragment"`) and return a vector of strings containing the output for each command: for `alloc`, output the assigned block ID if successful, or `"NULL"` if no suitable contiguous block exists; for `erase`, output nothing if successful, or `"ILLEGAL_ERASE_ARGUMENT"` if the ID does not exist; for `defragment`, output nothing. Block IDs start at 1 and increase with each successful allocation. Memory is 1-indexed internally, and free blocks are zeros. The function must handle multiple commands in order, maintain state, and return results in the same order as the commands.
*/
#include <vector>
#include <string>
#include <sstream>

// Simulate a memory allocator with alloc, erase, and defragment commands.
// memorySize: total number of cells (1-indexed internally).
// commands: list of strings like "alloc 5", "erase 3", "defragment".
// Returns: vector of output lines for each command (empty string if no output).
std::vector<std::string> runAllocator(int memorySize, const std::vector<std::string>& commands) {
    std::vector<int> mem(memorySize + 1, 0); // mem[1..memorySize], 0 = free
    int nextId = 1;
    std::vector<std::string> results;
    
    for (const std::string& cmd : commands) {
        std::istringstream iss(cmd);
        std::string op;
        iss >> op;
        
        if (op == "alloc") {
            int size;
            iss >> size;
            if (size <= 0 || size > memorySize) {
                results.push_back("NULL");
                continue;
            }
            // Find first contiguous free block of given size.
            int run = 0;
            int start = -1;
            for (int i = 1; i <= memorySize; ++i) {
                if (mem[i] == 0) {
                    ++run;
                    if (run >= size) {
                        start = i - size + 1;
                        break;
                    }
                } else {
                    run = 0;
                }
            }
            if (start == -1) {
                results.push_back("NULL");
            } else {
                int id = nextId++;
                for (int i = start; i < start + size; ++i) {
                    mem[i] = id;
                }
                results.push_back(std::to_string(id));
            }
        } else if (op == "erase") {
            int id;
            iss >> id;
            bool found = false;
            for (int i = 1; i <= memorySize; ++i) {
                if (mem[i] == id) {
                    found = true;
                    mem[i] = 0;
                }
            }
            if (!found) {
                results.push_back("ILLEGAL_ERASE_ARGUMENT");
            } else {
                results.push_back(""); // no output on success
            }
        } else if (op == "defragment") {
            int write = 1;
            for (int read = 1; read <= memorySize; ++read) {
                if (mem[read] != 0) {
                    mem[write++] = mem[read];
                }
            }
            while (write <= memorySize) {
                mem[write++] = 0;
            }
            results.push_back(""); // no output
        } else {
            results.push_back(""); // ignore unknown commands
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration from the solution.
std::vector<std::string> runAllocator(int memorySize, const std::vector<std::string>& commands);

int main() {
    // Test 1: Basic alloc and erase.
    std::vector<std::string> cmds1 = {"alloc 3", "alloc 2", "erase 1", "alloc 1"};
    auto res1 = runAllocator(5, cmds1);
    std::vector<std::string> expected1 = {"1", "2", "", "1"};
    assert(res1 == expected1);

    // Test 2: Alloc fails when no contiguous space.
    std::vector<std::string> cmds2 = {"alloc 4", "alloc 2"};
    auto res2 = runAllocator(5, cmds2);
    std::vector<std::string> expected2 = {"1", "NULL"};
    assert(res2 == expected2);

    // Test 3: Erase illegal argument.
    std::vector<std::string> cmds3 = {"erase 5"};
    auto res3 = runAllocator(4, cmds3);
    std::vector<std::string> expected3 = {"ILLEGAL_ERASE_ARGUMENT"};
    assert(res3 == expected3);

    // Test 4: Defragment compacts memory.
    std::vector<std::string> cmds4 = {"alloc 1", "alloc 1", "alloc 1", "erase 2", "defragment", "alloc 2"};
    auto res4 = runAllocator(3, cmds4);
    std::vector<std::string> expected4 = {"1", "2", "3", "", "", "4"};
    assert(res4 == expected4);

    // Test 5: Defragment after multiple erases creates larger block.
    std::vector<std::string> cmds5 = {"alloc 2", "alloc 2", "erase 1", "erase 2", "defragment", "alloc 2"};
    auto res5 = runAllocator(4, cmds5);
    std::vector<std::string> expected5 = {"1", "2", "", "", "", "3"};
    assert(res5 == expected5);

    // Test 6: Alloc size larger than memory.
    std::vector<std::string> cmds6 = {"alloc 10"};
    auto res6 = runAllocator(3, cmds6);
    std::vector<std::string> expected6 = {"NULL"};
    assert(res6 == expected6);

    // Test 7: Alloc size zero or negative (treated as NULL).
    std::vector<std::string> cmds7 = {"alloc 0", "alloc -1"};
    auto res7 = runAllocator(3, cmds7);
    std::vector<std::string> expected7 = {"NULL", "NULL"};
    assert(res7 == expected7);

    // Test 8: Erase non-existent ID after defrag.
    std::vector<std::string> cmds8 = {"alloc 1", "defragment", "erase 1", "erase 1"};
    auto res8 = runAllocator(2, cmds8);
    std::vector<std::string> expected8 = {"1", "", "", "ILLEGAL_ERASE_ARGUMENT"};
    assert(res8 == expected8);

    // Test 9: Fragmentation prevents alloc before defrag, but works after.
    std::vector<std::string> cmds9 = {"alloc 1", "alloc 1", "erase 1", "alloc 2"};
    auto res9 = runAllocator(3, cmds9);
    std::vector<std::string> expected9 = {"1", "2", "", "NULL"};
    assert(res9 == expected9);

    // Test 10: Mixed sequence with defrag.
    std::vector<std::string> cmds10 = {"alloc 2", "alloc 1", "erase 1", "defragment", "alloc 2", "alloc 1"};
    auto res10 = runAllocator(4, cmds10);
    std::vector<std::string> expected10 = {"1", "2", "", "", "3", "4"};
    assert(res10 == expected10);

    return 0;
}
// The solution simulates memory as a vector of integers, where 0 means free and nonzero means occupied by a block with that ID. For each command:
// - `alloc`: Scan from index 1 to `m` tracking the current run length of consecutive zeros. When the run reaches the requested size, record the start position (`i - size + 1`) and immediately stop. If found, assign a new unique ID to each cell in that range and output the ID. If not found, output `"NULL"`. The scan is O(m) per alloc.
// - `erase`: Search the entire memory for the given ID. If found, set all matching cells to 0 and output nothing; else output `"ILLEGAL_ERASE_ARGUMENT"`. This is O(m) per erase.
// - `defragment`: Use two pointers, a write index `j` starting at 1, and a read index `i` from 1 to `m`. For each non-zero memory cell, copy its value to `mem[j]` and increment `j`. After the scan, fill the rest from `j` to `m` with zeros. This is O(m) per defragmentation.
// Edge cases: `alloc` with size larger than `m` always fails; `erase` with ID 0 or a non-existent ID triggers illegal; repeated `erase` on already-freed ID is illegal; defragmentation does not change IDs, only compacts them. The total time complexity is O(C × m) for C commands, and space O(m) for memory plus O(number of commands) for output.
