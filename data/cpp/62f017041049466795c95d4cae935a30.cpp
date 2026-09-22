Write a standalone C++ function `encodeDeflateBlock` that takes a `std::vector<uint8_t>` representing uncompressed data (up to 65535 bytes) and a `bool finalBlock` flag, and returns a `std::vector<uint8_t>` containing the raw DEFLATE fixed-Huffman compressed representation of that data. The function must implement the DEFLATE fixed-Huffman block format exactly: output 1 bit for BFINAL (the provided flag), 2 bits for BTYPE=01 (fixed Huffman), then encode each input byte as a literal using the fixed Huffman code tables (with 8-bit codes for literals 0-143, 9-bit codes for literals 144-255), and finally emit the end-of-block symbol (code 256 is 7 bits: `0000000`). The output must be bit-packed MSB-first per DEFLATE specification. If the input is empty, the function should output only the block header bits (BFINAL and BTYPE) followed by the end-of-block symbol. The function must not use any external compression libraries; only standard headers such as `<vector>`, `<cstdint>`, `<cassert>` are allowed.

#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is defined above; here we test it.

int main() {
    // Helper to extract bits from a byte stream MSB-first and compare.
    auto decodeFixedHuffman = [](const std::vector<uint8_t>& encoded) {
        // Simple decoder for fixed Huffman blocks (only literals + EOB).
        std::vector<uint8_t> decoded;
        size_t bitPos = 0;
        auto readBit = [&]() -> int {
            if (bitPos >= encoded.size() * 8) return -1; // out of data
            size_t byteIndex = bitPos / 8;
            int bitIndex = 7 - (bitPos % 8);
            ++bitPos;
            return (encoded[byteIndex] >> bitIndex) & 1;
        };
        auto readBits = [&](int n) -> int {
            int value = 0;
            for (int i = 0; i < n; ++i) {
                int bit = readBit();
                if (bit < 0) return -1;
                value = (value << 1) | bit;
            }
            return value;
        };
        // Skip BFINAL and BTYPE.
        int bfinal = readBit();
        int btype = readBits(2);
        assert(bfinal == 0 || bfinal == 1);
        assert(btype == 1); // fixed Huffman

        // Decode symbols until EOB (symbol 256).
        while (true) {
            // Try 8-bit code first.
            int code8 = readBits(8);
            if (code8 < 0) break;
            if (code8 == 0x00) { // EOB? Actually EOB is 7-bit 0000000, but we only get here if 8 bits read; handle below
            }
            // Since codes are prefix-free, we need to parse properly:
            // Reset bitPos to before the 8-bit read attempt.
            bitPos -= 8;
            // Read first 7 bits to check for EOB.
            int code7 = readBits(7);
            if (code7 == 0) {
                // EOB symbol 256.
                break;
            }
            // Re-read 7 bits? Wait we consumed them. We need the full code.
            // Better approach: read bit by bit until a symbol matches.
            // Reset to before the 8-bit read.
            bitPos -= 0; // we've already moved bitPos back 8 then advanced 7, so now we're at start of symbol.
            // Actually simpler: re-implement reading using a state machine.
            // For brevity in testing, we'll just check the output structure.
            break;
        }
        // For the test, we validate by checking the encoded bit pattern directly.
        return decoded;
    };

    // Test 1: Empty input, non-final block.
    {
        std::vector<uint8_t> input;
        auto out = encodeDeflateBlock(input, false);
        // Expected bits: BFINAL=0, BTYPE=01, EOB=0000000.
        // Combined: 0 01 0000000 -> bits: 00100000 00 (padding).
        // Bit sequence: 0,0,1,0,0,0,0,0,0,0 => first byte: 0x20, second byte: 0x00 (padding).
        assert(out.size() == 2);
        assert(out[0] == 0x20);
        assert(out[1] == 0x00);
    }

    // Test 2: Empty input, final block.
    {
        std::vector<uint8_t> input;
        auto out = encodeDeflateBlock(input, true);
        // BFINAL=1, BTYPE=01, EOB=0000000.
        // Bits: 1 01 0000000 -> 101000000 -> first byte 0xA0, second padding 0x00.
        assert(out.size() == 2);
        assert(out[0] == 0xA0);
        assert(out[1] == 0x00);
    }

    // Test 3: Single byte literal 0x00, non-final.
    {
        std::vector<uint8_t> input = {0x00};
        auto out = encodeDeflateBlock(input, false);
        // BFINAL=0, BTYPE=01, literal 0x00 code (8 bits): 0x30, EOB 0000000.
        // Bits: 0 01 00110000 0000000 -> let's compute:
        // Sequence: 0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 (17 bits)
        // Group into bytes: first 8: 00100110 = 0x26; next 8: 00000000 = 0x00; last bit: 0 -> padded 00000000.
        assert(out.size() == 3);
        assert(out[0] == 0x26);
        assert(out[1] == 0x00);
        assert(out[2] == 0x00);
    }

    // Test 4: Literal 'A' (0x41), final block.
    {
        std::vector<uint8_t> input = {0x41};
        auto out = encodeDeflateBlock(input, true);
        // BFINAL=1, BTYPE=01, literal 0x41 code (8 bits): 0x30+0x41=0x71, EOB 0000000.
        // Bits: 1 01 01110001 0000000 -> 10101110 00100000 00...
        // First byte: 10101110 = 0xAE; second byte: 00100000 = 0x20; third byte: 00000000 = 0x00.
        assert(out.size() == 3);
        assert(out[0] == 0xAE);
        assert(out[1] == 0x20);
        assert(out[2] == 0x00);
    }

    // Test 5: Two bytes: 0x00 and 0xFF, final.
    {
        std::vector<uint8_t> input = {0x00, 0xFF};
        auto out = encodeDeflateBlock(input, true);
        // BFINAL=1, BTYPE=01, literal 0x00 code 0x30, literal 0xFF code (9 bits) 0x190+(255-144)=0x190+111=0x1FF, EOB 0000000.
        // Compute bit sequence manually: 1 01 00110000 111111111 0000000
        // Total bits: 1+2+8+9+7=27 bits.
        // First byte: 1 0 1 0 0 1 1 0 = 0xA6
        // Second byte: 0 0 1 1 1 1 1 1 = 0x3F
        // Third byte: 1 1 1 0 0 0 0 0 = 0xE0
        // Fourth byte: 0 0 0 0 0 0 0 0 = 0x00 (pad)
        assert(out.size() == 4);
        assert(out[0] == 0xA6);
        assert(out[1] == 0x3F);
        assert(out[2] == 0xE0);
        assert(out[3] == 0x00);
    }

    // Test 6: Maximum length input (65535) should not crash and output size is proportional.
    {
        std::vector<uint8_t> input(65535, 0x41); // all 'A'
        auto out = encodeDeflateBlock(input, true);
        // Each byte is 8-bit literal so compressed size is roughly (3 + 8*65535 + 7)/8 bytes plus padding.
        size_t expectedMin = (3 + 8 * input.size() + 7) / 8;
        assert(out.size() >= expectedMin && out.size() <= expectedMin + 1);
    }

    return 0;
}

#include <vector>
#include <cstdint>
#include <cassert>

// Encode a single input stream as a DEFLATE fixed-Huffman block.
// The block contains only literal symbols (no matches) and ends with end-of-block.
std::vector<uint8_t> encodeDeflateBlock(const std::vector<uint8_t>& input, bool finalBlock) {
    // Fixed Huffman code tables (literal values and end-of-block).
    // Values are the canonical codes as per RFC 1951.
    struct CodeEntry { uint16_t code; uint8_t length; };

    // Literals 0-143: 8-bit codes, value = 0x30 + literal.
    // Literals 144-255: 9-bit codes, value = 0x190 + (literal - 144).
    // End-of-block (symbol 256): 7-bit code 0b0000000.
    auto getCode = [](uint16_t symbol) -> CodeEntry {
        assert(symbol <= 256);
        if (symbol <= 143) {
            return { static_cast<uint16_t>(0x30 + symbol), 8 };
        } else if (symbol <= 255) {
            return { static_cast<uint16_t>(0x190 + (symbol - 144)), 9 };
        } else { // symbol == 256 (end of block)
            return { 0x00, 7 };
        }
    };

    // Bit writer that packs bits MSB-first into bytes.
    class BitWriter {
    public:
        std::vector<uint8_t>& out;
        uint32_t bitBuffer = 0;
        int bitCount = 0;

        explicit BitWriter(std::vector<uint8_t>& outStream) : out(outStream) {}

        void writeBits(uint32_t value, int numBits) {
            assert(numBits > 0 && numBits <= 16);
            // Write bits from most significant to least significant of the specified length.
            for (int i = numBits - 1; i >= 0; --i) {
                bitBuffer = (bitBuffer << 1) | ((value >> i) & 1);
                ++bitCount;
                if (bitCount == 8) {
                    out.push_back(static_cast<uint8_t>(bitBuffer));
                    bitBuffer = 0;
                    bitCount = 0;
                }
            }
        }

        void flush() {
            if (bitCount > 0) {
                // Pad remaining bits with zeros.
                bitBuffer <<= (8 - bitCount);
                out.push_back(static_cast<uint8_t>(bitBuffer));
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    };

    std::vector<uint8_t> output;
    output.reserve(input.size() * 8 / 8 + 8); // at most 9 bits per byte plus overhead
    BitWriter writer(output);

    // Write block header: BFINAL (1 bit) and BTYPE (2 bits, fixed = 01).
    writer.writeBits(finalBlock ? 1 : 0, 1);
    writer.writeBits(0b01, 2);

    // Encode each input byte as a literal.
    for (uint8_t byte : input) {
        CodeEntry entry = getCode(byte);
        writer.writeBits(entry.code, entry.length);
    }

    // End-of-block symbol 256.
    CodeEntry eob = getCode(256);
    writer.writeBits(eob.code, eob.length);

    // Flush any remaining bits.
    writer.flush();

    return output;
}

// The core challenge is implementing DEFLATE’s fixed Huffman coding with correct bit ordering (MSB-first within each byte) and bit packing. The fixed Huffman tables are defined by the DEFLATE spec: literals 0-143 have codes of length 8 bits (the code value is `0x30 + literal`, i.e., the literal value plus 0x30, using the first 8 bits of the canonical codes), literals 144-255 have codes of length 9 bits (code value `0x190 + (literal - 144)`), and the end-of-block symbol 256 has a 7-bit code `0000000`. Because these codes are already in the order they appear in the canonical Huffman tree, we can directly use them without additional tree construction. The bit writer must accumulate bits into a byte buffer, writing the most significant bit of each code first, and when the buffer fills 8 bits, output that byte. The order of field emission is: BFINAL (1 bit), BTYPE (2 bits), then a sequence of literal codes, then end-of-block. The input is processed byte-by-byte, with each byte becoming a literal symbol (no match/length-distance encoding is performed, adhering to fixed-Huffman with only literals). Edge cases include empty input, input length at the maximum 65535, and ensuring correct handling of bit alignment — the output may have a partial final byte that should be padded with zeros in the remaining low bits. Time complexity is O(n) where n is the number of input bytes, since each byte requires a constant number of bit-writing operations; space complexity is O(1) auxiliary besides the output vector.
