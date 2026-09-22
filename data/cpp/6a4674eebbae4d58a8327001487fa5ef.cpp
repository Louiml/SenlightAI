/*
Write a C++ function that simulates a simple 36-bit bitmask program execution. The function takes a vector of strings, where each string is either a mask assignment (starting with `"mask = "` followed by a 36-character string of `'0'`, `'1'`, and `'X'`) or a memory write (starting with `"mem["` followed by a decimal address and `"] = "` and a decimal value). For each mask, the function must process all memory writes that appear after that mask (until the next mask or end of input) in a deferred manner: when a new mask appears, apply the current mask to all pending writes and store the results in a map from address to value. At the end, apply the last mask to any remaining pending writes. The mask application rule for values is: if the mask bit is `'0'` or `'1'`, overwrite the corresponding bit of the 36-bit binary representation of the value; if the mask bit is `'X'`, keep the original value bit unchanged. The function should return the sum of all values stored in the final map, as an `unsigned long long`.
*/
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <cstdint>

// Simulates a 36-bit mask and memory writes, returns sum of final values.
// Each string in 'instructions' is either "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
// or "mem[<addr>] = <value>".
unsigned long long simulateMaskProgram(const std::vector<std::string>& instructions) {
    constexpr int MASK_SIZE = 36;
    std::bitset<MASK_SIZE> current_mask;
    bool has_mask = false;
    std::map<unsigned long long, unsigned long long> final_memory;
    // Pending writes for current mask: addr -> val
    std::map<unsigned long long, unsigned long long> pending;

    auto apply_mask_to_value = [](unsigned long long value, const std::bitset<MASK_SIZE>& mask) -> unsigned long long {
        // Convert mask bitset to two masks: ones_mask and zeros_mask (bits to set/clear)
        unsigned long long ones_mask = 0, zeros_mask = 0;
        for (int i = 0; i < MASK_SIZE; ++i) {
            // mask string leftmost is bitset index MASK_SIZE-1 (most significant)
            // In our bitset, index 0 is LSB. mask bit at string position i corresponds to bitset index MASK_SIZE-1-i.
            if (mask[MASK_SIZE - 1 - i]) {
                ones_mask |= (1ULL << i);
            } else {
                // '0' or 'X'? In mask bitset, we only set to 1 when mask char is '1'.
                // For '0' we need to clear, but we do not have that info in bitset.
                // So we use a different approach: store mask string directly.
            }
        }
        // The lambda above is simplistic; better to handle string directly.
        // We re-implement below using the actual mask string.
        return value; // placeholder, real implementation in function body
    };

    // Helper to apply a mask string to a value.
    auto apply_mask_string = [](unsigned long long value, const std::string& mask) -> unsigned long long {
        std::bitset<MASK_SIZE> bits(value);
        for (int i = 0; i < MASK_SIZE; ++i) {
            char c = mask[i];
            if (c == '0') {
                bits[MASK_SIZE - 1 - i] = 0;
            } else if (c == '1') {
                bits[MASK_SIZE - 1 - i] = 1;
            }
            // 'X' leaves bit unchanged
        }
        return bits.to_ullong();
    };

    auto flush_pending = [&]() {
        if (!has_mask) return;
        for (const auto& kv : pending) {
            final_memory[kv.first] = apply_mask_string(kv.second, current_mask_str);
        }
        pending.clear();
    };

    std::string current_mask_str;
    for (const auto& line : instructions) {
        if (line.rfind("mask", 0) == 0) {
            // Process previous pending
            flush_pending();
            // Parse new mask: line is "mask = ..."
            size_t eq = line.find('=');
            current_mask_str = line.substr(eq + 2); // skip "= "
            has_mask = true;
        } else if (line.rfind("mem", 0) == 0) {
            size_t open_bracket = line.find('[');
            size_t close_bracket = line.find(']');
            unsigned long long addr = std::stoull(line.substr(open_bracket + 1, close_bracket - open_bracket - 1));
            size_t eq = line.find('=');
            unsigned long long val = std::stoull(line.substr(eq + 2));
            pending[addr] = val;
        }
    }
    // Flush remaining pending after last mask
    flush_pending();

    // Sum all values in final_memory
    unsigned long long sum = 0;
    for (const auto& kv : final_memory) {
        sum += kv.second;
    }
    return sum;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// The solution function is assumed to be declared above.
// Test cases adapted from common examples.

int main() {
    // Basic test: apply mask 'X' all → values unchanged
    std::vector<std::string> instructions1 = {
        "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "mem[8] = 11",
        "mem[7] = 101",
        "mem[8] = 0"
    };
    // Values: all masked as 'X' so unchanged. Final: mem[8]=0, mem[7]=101 → sum 101
    assert(simulateMaskProgram(instructions1) == 101ULL);

    // Test with '0' and '1' bits
    std::vector<std::string> instructions2 = {
        "mask = 000000000000000000000000000000000001",
        "mem[0] = 2", // binary ...0010 → mask sets bit0 to 1 → becomes 3
        "mem[1] = 3"  // binary ...0011 → mask sets bit0 to 1 → becomes 3
        // final: mem[0]=3, mem[1]=3 → sum 6
    };
    assert(simulateMaskProgram(instructions2) == 6ULL);

    // Test multiple masks with overwriting
    std::vector<std::string> instructions3 = {
        "mask = 0X0000000000000000000000000000000000", // clear bit35, keep others
        "mem[0] = 10", // binary ...1010 → after mask: bit35=0, all others same → 10
        "mask = 1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", // set bit35 to 1
        "mem[0] = 10" // now value becomes 10 + 2^35 = 34359738378
    };
    // Compute: 2^35 = 34359738368; 10 + that = 34359738378
    assert(simulateMaskProgram(instructions3) == 34359738378ULL);

    // Test with pending without mask initially (unlikely but if first line is mem)
    std::vector<std::string> instructions4 = {
        "mem[5] = 42",
        "mask = 1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", // set bit35 to 1
        // pending mem[5]=42 gets processed with mask → 42+2^35
    };
    assert(simulateMaskProgram(instructions4) == 34359738410ULL);

    // Test empty input
    std::vector<std::string> instructions5;
    assert(simulateMaskProgram(instructions5) == 0ULL);

    // Test consecutive masks with no writes between
    std::vector<std::string> instructions6 = {
        "mask = 000000000000000000000000000000000001",
        "mask = 1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "mem[1] = 1" // final mask sets bit35, so value = 1 + 2^35
    };
    assert(simulateMaskProgram(instructions6) == 34359738369ULL);

    // Test duplicates within same segment: later wins
    std::vector<std::string> instructions7 = {
        "mask = 0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", // clear bit35
        "mem[3] = 5",
        "mem[3] = 7",
    };
    // Only mem[3]=7 stored, mask clears bit35 (already 0), final sum 7
    assert(simulateMaskProgram(instructions7) == 7ULL);

    std::cout << "All tests passed!\n";
    return 0;
}
// The main idea is to delay applying the current mask until a new mask is encountered or the input ends. For each mask segment, we store pending writes in a temporary map (or two parallel vectors) of address→value. When a new mask is read, iterate over all pending writes and apply the current mask to each value using bitwise operations: for mask position `i` (from leftmost, most significant bit at index 0), if mask bit is `'0'`, clear that bit; if `'1'`, set that bit; if `'X'`, leave alone. This can be done efficiently by converting the 36-bit mask into two 36-bit masks: one for ones (`mask1`) and one for zeros (`mask0`), then applying `(value | mask1) & ~mask0` (with care about bit order, but because we treat the mask string left-to-right as most significant first, we can align by shifting). Simpler: use `std::bitset<36>` for both value and mask, but since bitset index 0 is least significant, we map the mask string position 0 (leftmost) to bitset index 35. For a value, apply: for each position, if mask char is '0' set bitset bit to 0, if '1' set to 1, else keep. After processing all pending writes, clear the pending map. At the end, process remaining pending writes. Use a `std::map<unsigned long long, unsigned long long>` to accumulate final results (overwrites are natural). Complexity: for each mask segment, each pending write is processed once; worst-case O(N*36) where N is number of writes, and O(N) memory for pending writes plus O(N) for final map.
// Edge cases: mask strings always exactly 36 characters; addresses and values are non-negative integers that fit in 36 bits (but we can use `unsigned long long`); writes may have duplicate addresses, later ones overwrite earlier ones in the same segment; masks can appear consecutively with no writes; empty input returns 0.
