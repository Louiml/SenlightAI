/*
Write a C++ function that simulates the multi-step decompression dispatch logic found in the snippet: given a single byte representing a bitmask of compression flags, a list of decompression callback functions (each taking input/output buffers and lengths), and an input buffer, apply the decompression callbacks in a fixed order (from least significant flag bit upward) to transform the input into an output buffer. The function should accept `const` input data, use `out_length` as both the initial capacity of the output buffer and (upon return) the actual number of output bytes, and return `true` on success or `false` if unknown compression flags are set. If the input length equals the initial output length, simply copy input to output. When multiple decompressions are needed, chain them through alternating temporary buffers. Do not actually compress/decompress—instead, simulate each callback by either copying data unchanged or reversing it (for testing), but keep the general flow identical.
*/

#include <cstddef>
#include <cstring>
#include <vector>

// Callback type: decompress input buffer into output buffer, updating out_length.
using DecompressFn = void (*)(char* out, int* out_len, const char* in, int in_len);

// Simulated callbacks for testing: identity and reverse.
void identity_decompress(char* out, int* out_len, const char* in, int in_len) {
    std::memcpy(out, in, static_cast<size_t>(in_len));
    *out_len = in_len;
}

void reverse_decompress(char* out, int* out_len, const char* in, int in_len) {
    for (int i = 0; i < in_len; ++i) {
        out[i] = in[in_len - 1 - i];
    }
    *out_len = in_len;
}

// Structure describing one decompression type.
struct DecompressEntry {
    unsigned char mask;
    DecompressFn fn;
};

// Main dispatcher: applies a sequence of decompressions based on a flag byte.
// Returns true on success, false if unknown compression flags are present.
bool multiDecompress(char* out_buf, int* out_length, const char* in_buf, int in_length,
                     const std::vector<DecompressEntry>& decomp_table) {
    // If input length equals output capacity, copy directly (handling aliasing).
    if (in_length == *out_length) {
        if (in_buf != out_buf) {
            std::memcpy(out_buf, in_buf, static_cast<size_t>(in_length));
        }
        *out_length = in_length;
        return true;
    }

    // Read compression flags from the first byte of input.
    unsigned char flags = static_cast<unsigned char>(in_buf[0]);
    const char* data_start = in_buf + 1;
    int data_len = in_length - 1;  // Excluding the flag byte.

    // Check for unknown flags.
    unsigned char unknown = flags;
    for (const auto& entry : decomp_table) {
        unknown &= static_cast<unsigned char>(~entry.mask);
    }
    if (unknown != 0) {
        return false;
    }

    // Count how many decompressions are requested.
    int count = 0;
    for (const auto& entry : decomp_table) {
        if ((flags & entry.mask) != 0) {
            count++;
        }
    }

    // If no decompressions, just copy the data after the flag byte.
    if (count == 0) {
        if (data_len > *out_length) {
            // Should not happen if caller provided enough space.
            return false;
        }
        std::memcpy(out_buf, data_start, static_cast<size_t>(data_len));
        *out_length = data_len;
        return true;
    }

    // Allocate a temporary buffer if more than one decompression step.
    std::vector<char> temp;
    char* temp_ptr = nullptr;
    if (count >= 2) {
        temp.resize(static_cast<size_t>(*out_length));
        temp_ptr = temp.data();
    }

    // Apply decompressions in order of the table.
    const char* current_in = data_start;
    int current_len = data_len;
    char* current_out = out_buf;
    int step = 0;
    for (const auto& entry : decomp_table) {
        if ((flags & entry.mask) != 0) {
            // Alternate between output and temporary buffer.
            char* next_out = (step % 2 == 0) ? out_buf : temp_ptr;
            int next_out_len = *out_length;  // initial capacity for this step
            entry.fn(next_out, &next_out_len, current_in, current_len);
            // The next step uses this written data as input.
            current_in = next_out;
            current_len = next_out_len;
            step++;
        }
    }

    // If the last write went into temp, copy its contents to the final output.
    if (step % 2 == 1 && temp_ptr != nullptr) {
        std::memcpy(out_buf, temp_ptr, static_cast<size_t>(current_len));
    }

    *out_length = current_len;
    return true;
}

#include <cassert>
#include <cstring>
#include <vector>

// Assume solution code is above (multiDecompress and helpers).

int main() {
    // Scenario 1: no flags set, input flag byte + "abc" -> output "abc".
    char input1[] = {0x00, 'a', 'b', 'c'};
    char out1[16] = {0};
    int out_len1 = 16;
    std::vector<DecompressEntry> table1 = {
        {0x01, identity_decompress},
        {0x02, reverse_decompress}
    };
    bool ok1 = multiDecompress(out1, &out_len1, input1, 4, table1);
    assert(ok1);
    assert(out_len1 == 3);
    assert(std::memcmp(out1, "abc", 3) == 0);

    // Scenario 2: unknown flag bit 0x04 set -> false.
    char input2[] = {0x04, 'x'};
    char out2[16] = {0};
    int out_len2 = 16;
    bool ok2 = multiDecompress(out2, &out_len2, input2, 2, table1);
    assert(!ok2);

    // Scenario 3: one reverse flag (0x02) on "abc" -> "cba".
    char input3[] = {0x02, 'a', 'b', 'c'};
    char out3[16] = {0};
    int out_len3 = 16;
    bool ok3 = multiDecompress(out3, &out_len3, input3, 4, table1);
    assert(ok3);
    assert(out_len3 == 3);
    assert(std::memcmp(out3, "cba", 3) == 0);

    // Scenario 4: two flags (0x03) on "abc": first identity, then reverse -> "cba".
    char input4[] = {0x03, 'a', 'b', 'c'};
    char out4[16] = {0};
    int out_len4 = 16;
    bool ok4 = multiDecompress(out4, &out_len4, input4, 4, table1);
    assert(ok4);
    assert(out_len4 == 3);
    assert(std::memcmp(out4, "cba", 3) == 0);

    // Scenario 5: two flags but reversed order in table would still apply in table order.
    // With table order {0x02 reverse, 0x01 identity}, input "abc" -> reverse gives "cba", then identity gives "cba".
    std::vector<DecompressEntry> table2 = {
        {0x02, reverse_decompress},
        {0x01, identity_decompress}
    };
    char input5[] = {0x03, 'a', 'b', 'c'};
    char out5[16] = {0};
    int out_len5 = 16;
    bool ok5 = multiDecompress(out5, &out_len5, input5, 4, table2);
    assert(ok5);
    assert(out_len5 == 3);
    assert(std::memcmp(out5, "cba", 3) == 0);

    // Scenario 6: input length equals output capacity, no flag byte handling.
    // But our solution requires flag byte always; test equal lengths without flag? Actually our code checks in_length == *out_length first, so if sizes match, it copies whole input (including flag byte) – that’s a design choice. Let's test: input "abcd", out_len=4 -> copy "abcd".
    char input6[] = {'a', 'b', 'c', 'd'};
    char out6[4] = {0};
    int out_len6 = 4;
    bool ok6 = multiDecompress(out6, &out_len6, input6, 4, table1);
    assert(ok6);
    assert(out_len6 == 4);
    assert(std::memcmp(out6, "abcd", 4) == 0);

    // Scenario 7: multiple decompressions use temporary buffer; verify data intact.
    // Use three steps: identity, reverse, identity on "hello" -> still "hello" after all.
    std::vector<DecompressEntry> table3 = {
        {0x01, identity_decompress},
        {0x02, reverse_decompress},
        {0x04, identity_decompress}  // this bit is unknown in table1 but here it's known
    };
    char input7[] = {0x07, 'h', 'e', 'l', 'l', 'o'};
    char out7[32] = {0};
    int out_len7 = 32;
    bool ok7 = multiDecompress(out7, &out_len7, input7, 6, table3);
    assert(ok7);
    assert(out_len7 == 5);
    assert(std::memcmp(out7, "hello", 5) == 0);

    // Scenario 8: zero-length data after flag byte.
    char input8[] = {0x01};  // identity on empty data
    char out8[1] = {0};
    int out_len8 = 1;
    bool ok8 = multiDecompress(out8, &out_len8, input8, 1, table1);
    assert(ok8);
    assert(out_len8 == 0);

    return 0;
}

// The core is a dispatcher that examines a flag byte, identifies which decompression methods are requested, validates that all bits are known, and then applies the methods sequentially. The algorithm: first, if input length equals output capacity, copy or return directly (handling aliasing). Next, read the flag byte from the start of the input and advance past it, reducing the length accordingly. Build a table of known masks, each associated with a function pointer that takes `(char* out, int* out_len, const char* in, int in_len)`. Count how many flags are set. If more than one, allocate a temporary buffer of `out_length` size. Then iterate through the table in order, and for each set bit, call the corresponding function with either the main output buffer or the temporary buffer alternating by an index. After each call, treat the produced output as the new input for the next step. Finally, if the last write went into temp instead of the final output, copy it over. Edge cases: unknown flag bits → return false; zero flags → no decompression, copy input to output? (Follow snippet: if flags are zero, the loop does nothing and the input buffer (started at original in_buf? Actually in_buf was incremented) becomes the output? In the snippet, if no flags are set, in_buf points past the flag byte, and they copy that to out_buf with out_length unchanged—but that would be wrong. However, in the original, the flag byte is mandated, so we must handle it. For our task, we can specify that a zero flag byte still means "no compression applied", but the input buffer still includes the flag byte, so we must copy the remaining input after the flag byte to output, with length reduced by one. To keep it simple, we can say the input always includes the flag byte, and even for zero flags, output is `in_length - 1` bytes copied from after the flag byte. But the snippet does not do that; it would copy `in_length` bytes from `in_buf` after incrementing, which is wrong. Since we're designing a standalone task, we can define behavior explicitly: The flag byte is consumed, and only the remaining bytes represent compressed data. The callbacks receive the remaining data. The total input length passed to the dispatcher includes the flag byte. So after reading the flag and decrementing `in_length`, if no flags are set, we must copy `in_length` bytes (the data after the flag) to output and set `*pout_length = in_length`. This is a sensible fix. Time complexity: O(k * n) where k is number of compression steps and n is data size, due to copying. Space: O(out_length) for temporary buffer when more than one step.
