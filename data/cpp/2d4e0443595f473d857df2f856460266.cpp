Write a standalone C++ function named `encodeIntraPCMBlock` that simulates the core algorithm of the given `EncodeIntraPCM` function from an H.264/AVC encoder. The function must accept: (1) a reference to a 2D `std::vector<std::uint8_t>` representing the luma plane of a current macroblock's input image (16x16 samples), (2) a reference to a 2D `std::vector<std::uint8_t>` representing the Cb chroma plane (8x8 samples), (3) a reference to a 2D `std::vector<std::uint8_t>` representing the Cr chroma plane (8x8 samples), and (4) a reference to a `std::vector<std::uint32_t>` output bit buffer that will receive the packed bitstream. The function must first write the fixed Exp-Golomb code for `25` (which is `0000011001` in binary) as a single unsigned integer (the original code writes it via `ue_v`, but here you must compute the bits manually: 25 in Exp-Golomb order = prefix of 4 zeros, then 1, then 4-bit suffix of 25+1=26 (binary `11010`), so the bits are `000011010`). After writing that, the function must pad the bitstream to a byte boundary (i.e., if the number of bits written so far is not a multiple of 8, append zero bits until it is). Then, the function must copy the 16x16 luma block, 8x8 Cb block, and 8x8 Cr block into the output bit buffer using the same word-based packing scheme as the original: for luma, process 16 rows of 16 bytes, reading and writing 4 bytes at a time (treating the input row as contiguous in memory) for 4 iterations per row; for each chroma plane, process 8 rows of 8 bytes, reading and writing 4 bytes at a time for 2 iterations per row. The bits must be appended to the output vector in big-endian bit order (i.e., the first bit of the first byte of the first plane is the most significant bit of the first 32‑bit word written). The output bit buffer must be a `std::vector<std::uint32_t>` where each element holds 32 bits in sequential order, and the total number of valid bits must be a multiple of 32 after processing all planes (since each plane's data size is a multiple of 32 bits). The function must return a `bool` indicating success (always `true` unless the input dimensions are invalid, in which case return `false`). You may assume the input vectors are valid sizes and that the output vector is empty initially.

The solution mirrors the original `EncodeIntraPCM` logic but simplifies the interface to pure data processing, removing encoder state and error handling. The main algorithm has three parts: (1) Exp-Golomb code for 25, (2) byte alignment, (3) sequential bit copying of luma, Cb, and Cr planes using 32‑bit word reads/writes. For the Exp-Golomb code, `codeNum = 25` maps to `M = floor(log2(codeNum+1)) = 4` (since 25+1=26, log2 is between 4 and 5), so we write `M` zeros, then a `1`, then the `M`-bit binary of `codeNum+1` minus `2^M` (i.e., 26-16=10, which is `1010` in 4 bits), yielding total bits `0000 1 1010` = `000011010` (9 bits). Then we pad with 7 zeros to reach 16 bits total (since 9 bits not multiple of 8), so the first 32-bit word contains `0000110100000000` in the high 16 bits and `0000000000000000` in the low 16 bits? Actually careful: after writing 9 bits, we pad to 16 bits (not 32) because we only need the next byte boundary: 9 bits → 16 bits (2 bytes). So we append 7 zero bits, now we have 16 bits total. Then we process luma: each row of 16 bytes = 4 words of 32 bits. Since we are already byte-aligned, we just append each 32-bit word directly to the output vector. The bit order within each word is the natural order of the bytes copied from the input (i.e., first byte of the row goes into the most significant byte of the word, as if we read a 32-bit integer from memory in little-endian? In the original code, they use `*((uint*)pSrc)` which on a little-endian machine reads the 4 bytes in reverse order, but for our abstraction we will define that the input vector is stored in row-major order and we read 4 bytes sequentially and pack them as a 32-bit big-endian integer (i.e., first byte becomes bits 31–24, second becomes bits 23–16, etc.). This is a simplification that makes the test deterministic. We process all rows: luma 16 rows, each row 4 words → 64 words = 2048 bits. Then Cb: 8 rows, each row 2 words (since 8 bytes) → 16 words = 512 bits. Cr same. Total bits after padding: 16 (header) + 2048 + 512 + 512 = 3088 bits, which is not a multiple of 32? Actually 3088/32 = 96.5, so we must pad the final output to a multiple of 32 bits? The original function does not pad after the last plane, but the bitstream is naturally byte-aligned (since each plane size is multiple of 8 bytes? Luma 16*16=256 bytes = 2048 bits, chroma 8*8=64 bytes=512 bits, so all are byte-aligned). Our total bits before final padding: header 16 bits + 2048 + 512 + 512 = 3088, which is not a multiple of 8? Actually 3088/8=386 bytes, so it is byte-aligned but not word-aligned. The output vector expects 32-bit words; to avoid partial words, we must pad to a multiple of 32 bits. The original code writes bits continuously and does not care about word alignment. To simplify, we will append zero bits until the total number of bits is a multiple of 32. That means we add 32 - (3088 % 32) = 32 - 16 = 16 zero bits. This results in 3104 bits = 97 words exactly. For the implementation, we maintain a bit position counter and a current 32-bit buffer; we write bits one by one into the buffer, and when the buffer is full (32 bits), we push it to the vector. Edge case: the header is 9 bits, we pad to 16 bits, then write luma rows. Important: the input vectors are 2D, so we need to read row by row; for each row we fetch 4 bytes at a time and treat them as a big-endian 32-bit value. We must copy luma, then Cb, then Cr in that order. The function should also handle the possibility that the input vectors have incorrect dimensions (e.g., luma not 16x16) by returning `false`. Time complexity is O(16*16 + 8*8 + 8*8) = O(384) operations, essentially linear in the number of samples. Space complexity is O(1) auxiliary aside from the output vector which grows to O(N) where N is the number of bits (proportional to input size). The algorithm is deterministic and does not require any external libraries beyond standard headers.

#include <cstdint>
#include <vector>
#include <stdexcept>

// Helper: append a single bit to the bit buffer.
static void writeBit(std::vector<std::uint32_t>& out, std::uint32_t& cur, int& bitPos, bool bit) {
    if (bitPos == 0) {
        cur = 0;
    }
    cur = (cur << 1) | (bit ? 1u : 0u);
    bitPos++;
    if (bitPos == 32) {
        out.push_back(cur);
        cur = 0;
        bitPos = 0;
    }
}

// Helper: append a sequence of bits given as a 32-bit value with a length.
static void writeBits(std::vector<std::uint32_t>& out, std::uint32_t& cur, int& bitPos,
                      std::uint32_t value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) {
        writeBit(out, cur, bitPos, ((value >> i) & 1u) != 0);
    }
}

// Helper: write a 32-bit word directly (ensuring we are in a fresh word).
static void write32(std::vector<std::uint32_t>& out, std::uint32_t& cur, int& bitPos, std::uint32_t word) {
    // Must be called only when bitPos == 0 (byte-aligned and word-aligned).
    out.push_back(word);
    // cur and bitPos remain as they are (bitPos should be 0).
}

// Simulates EncodeIntraPCM for luma, Cb, and Cr planes.
bool encodeIntraPCMBlock(
    const std::vector<std::vector<std::uint8_t>>& luma,
    const std::vector<std::vector<std::uint8_t>>& cb,
    const std::vector<std::vector<std::uint8_t>>& cr,
    std::vector<std::uint32_t>& out) {

    // Validate dimensions.
    if (luma.size() != 16 || cb.size() != 8 || cr.size() != 8) return false;
    for (auto& row : luma) if (row.size() != 16) return false;
    for (auto& row : cb) if (row.size() != 8) return false;
    for (auto& row : cr) if (row.size() != 8) return false;

    out.clear();

    // Current bit accumulator and bit position (0-31).
    std::uint32_t cur = 0;
    int bitPos = 0;

    // 1. Write Exp-Golomb code for 25 (codeNum = 25).
    // M = floor(log2(25+1)) = 4.
    // Write M zeros, then a single 1, then the M-bit suffix.
    // Suffix value = (codeNum+1) - (1<<M) = 26 - 16 = 10 (binary 1010).
    for (int i = 0; i < 4; ++i) writeBit(out, cur, bitPos, false);
    writeBit(out, cur, bitPos, true);
    writeBits(out, cur, bitPos, 10, 4); // writes 1010

    // 2. Align to byte boundary (8 bits).
    while (bitPos % 8 != 0) writeBit(out, cur, bitPos, false);

    // 3. Write luma (16x16). Process row by row, each row 16 bytes = 4 words.
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; col += 4) {
            // Build a 32-bit big-endian word from 4 bytes.
            std::uint32_t word = (static_cast<std::uint32_t>(luma[row][col]) << 24) |
                                 (static_cast<std::uint32_t>(luma[row][col+1]) << 16) |
                                 (static_cast<std::uint32_t>(luma[row][col+2]) << 8) |
                                 (static_cast<std::uint32_t>(luma[row][col+3]));
            // Must be word-aligned now (bitPos==0).
            write32(out, cur, bitPos, word);
        }
    }

    // 4. Write Cb (8x8). Each row 8 bytes = 2 words.
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; col += 4) {
            std::uint32_t word = (static_cast<std::uint32_t>(cb[row][col]) << 24) |
                                 (static_cast<std::uint32_t>(cb[row][col+1]) << 16) |
                                 (static_cast<std::uint32_t>(cb[row][col+2]) << 8) |
                                 (static_cast<std::uint32_t>(cb[row][col+3]));
            write32(out, cur, bitPos, word);
        }
    }

    // 5. Write Cr (8x8). Same pattern.
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; col += 4) {
            std::uint32_t word = (static_cast<std::uint32_t>(cr[row][col]) << 24) |
                                 (static_cast<std::uint32_t>(cr[row][col+1]) << 16) |
                                 (static_cast<std::uint32_t>(cr[row][col+2]) << 8) |
                                 (static_cast<std::uint32_t>(cr[row][col+3]));
            write32(out, cur, bitPos, word);
        }
    }

    // 6. Align the entire output to a multiple of 32 bits (full words).
    while (bitPos != 0) writeBit(out, cur, bitPos, false);

    return true;
}

#include <cassert>
#include <vector>
#include <cstdint>

// (The solution function is already defined above; this test file just calls it.)

int main() {
    // Helper to create a 16x16 luma with a simple pattern.
    auto makeLuma = []() {
        std::vector<std::vector<std::uint8_t>> l(16, std::vector<std::uint8_t>(16));
        for (int r = 0; r < 16; ++r)
            for (int c = 0; c < 16; ++c)
                l[r][c] = static_cast<std::uint8_t>(r * 16 + c);
        return l;
    };
    auto makeChroma = []() {
        std::vector<std::vector<std::uint8_t>> c(8, std::vector<std::uint8_t>(8));
        for (int r = 0; r < 8; ++r)
            for (int col = 0; col < 8; ++col)
                c[r][col] = static_cast<std::uint8_t>(r * 8 + col);
        return c;
    };

    auto luma = makeLuma();
    auto cb = makeChroma();
    auto cr = makeChroma();
    std::vector<std::uint32_t> out;

    // Test 1: successful encoding.
    assert(encodeIntraPCMBlock(luma, cb, cr, out) == true);
    // Number of words: header (2 bytes = 16 bits) + luma (256 bytes = 256*8 = 2048 bits) + cb (64*8=512) + cr (512) = 16+2048+512+512=3088 bits, padded to 3104 bits = 97 words.
    assert(out.size() == 97);

    // Test 2: first word contains the header (bits 0..15: 0000110100000000) plus the first 16 bits of luma row0.
    // Header is 9 bits '000011010' (binary), then 7 zero padding -> 16 bits: 0b0000110100000000 = 0x0D00.
    // First 16 bits of luma row0: bytes 0..3 = 0,1,2,3 -> big-endian word 0x00010203.
    // So first 32 bits = (0x0D00 << 16) | (0x00010203) = 0x0D00010203? Actually it's a 32-bit value: high 16 bits from header, low 16 bits from first half of luma word.
    // But our writer writes bits continuously: after header 16 bits, we write luma row0 word (32 bits) directly. The first word of out contains bits 0..31: bits 0..15 are header (0x0D00), bits 16..31 are the first 16 bits of luma row0 (bytes 0 and 1). That is 0x0000 (byte0=0) and 0x0001 (byte1=1) as a 16-bit value = 0x0001? Actually big-endian: byte0 (0) then byte1 (1) in the high byte and low byte? Let's be precise: we build the word as (byte0<<24)|(byte1<<16)|(byte2<<8)|byte3. For row0 bytes 0,1,2,3 we get 0x00010203. The first 16 bits of that word are the high 16 bits = 0x0001. So out[0] = (0x0D00 << 16) | 0x0001 = 0x0D000001.
    assert(out[0] == 0x0D000001u);

    // Test 3: last word should be the last 32 bits of Cr plane. Cr row7 bytes 56..59? Actually last row (index 7) of cr: bytes 56,57,58,59? But cr is 8x8 so last row indices 56..63. For simplicity just check that the last word is not zero and matches expected pattern.
    // Since cr is 8x8 with values r*8+col, last row (row 7) has values 56,57,58,59,60,61,62,63. The last word covers bytes 60..63, so word = (60<<24)|(61<<16)|(62<<8)|63 = 0x3C3D3E3F.
    assert(out.back() == 0x3C3D3E3Fu);

    // Test 4: invalid dimensions.
    std::vector<std::vector<std::uint8_t>> badLuma(15, std::vector<std::uint8_t>(16));
    assert(encodeIntraPCMBlock(badLuma, cb, cr, out) == false);

    // Test 5: all zeros planes.
    std::vector<std::vector<std::uint8_t>> zeroLuma(16, std::vector<std::uint8_t>(16, 0));
    std::vector<std::vector<std::uint8_t>> zeroChroma(8, std::vector<std::uint8_t>(8, 0));
    std::vector<std::uint32_t> out2;
    assert(encodeIntraPCMBlock(zeroLuma, zeroChroma, zeroChroma, out2) == true);
    assert(out2.size() == 97);
    // After header and padding, all sample bits are zero, so only header appears in first word.
    assert(out2[0] == 0x0D000000u);

    // Test 6: all 0xFF planes.
    std::vector<std::vector<std::uint8_t>> ffLuma(16, std::vector<std::uint8_t>(16, 0xFF));
    std::vector<std::vector<std::uint8_t>> ffChroma(8, std::vector<std::uint8_t>(8, 0xFF));
    std::vector<std::uint32_t> out3;
    assert(encodeIntraPCMBlock(ffLuma, ffChroma, ffChroma, out3) == true);
    // First word: header + first 16 bits of all-ones luma row0 = (0x0D00 << 16) | 0xFFFF = 0x0D00FFFF.
    assert(out3[0] == 0x0D00FFFFu);
    // All remaining words should be 0xFFFFFFFF.
    for (size_t i = 1; i < out3.size(); ++i) {
        assert(out3[i] == 0xFFFFFFFFu);
    }

    return 0;
}
