Write a C++ function that, given four contiguous blocks of 16-bit signed integers (each block of the same length `length`, representing four separate input arrays stored consecutively in a single buffer), produces an interleaved output buffer where the output is formed by alternating one value from each input in round-robin order (all four inputs' first values, then all four inputs' second values, etc.). The function must process the data efficiently using SSE4.2 intrinsics when available: it should process 16 bytes (8 values) at a time using SIMD unpacking operations, and handle any remaining bytes with a scalar fallback loop. The input and output buffers are both byte arrays with the given `length` measured in bytes; each 16-bit value is stored in little-endian order. Your solution must be self-contained, include appropriate headers, and be guarded by a preprocessor check for `__SSE4_2__` (with a portable scalar fallback if not defined). The function signature should be: `void interleave_4_sse4(uint8_t* dst, const uint8_t* src, int32_t length)`, where `src` points to the four concatenated blocks (block 1 at offset 0, block 2 at offset `length`, etc.) and `dst` points to the interleaved output of size `4 * length` bytes. Note that the `length` parameter is the byte length of each individual input block, not the total size. The function must correctly handle lengths that are not multiples of 16 bytes (the SIMD chunk size) by falling back to scalar processing for the final partial chunk.

The core problem is to interleave four arrays of 16-bit integers in a round-robin pattern: output[0..7] = src1[0..7], src2[0..7], src3[0..7], src4[0..7] (actually output[0]=src1[0], output[1]=src2[0], output[2]=src3[0], output[3]=src4[0], output[4]=src1[1], etc.). The given snippet uses SIMD to process 8 elements (16 bytes) per iteration. The algorithm: load 16 bytes from each input pointer, then use `_mm_unpacklo_epi8` and `_mm_unpackhi_epi8` to combine pairs of inputs into 32-byte results (though each unpack produces 16 bytes). The sequence of unpacks (`epi8`, then `epi16`, then `epi16` again) effectively transposes the 4x8 matrix of 16-bit values into the interleaved order. Specifically, after loading four 16-byte chunks (each containing 8 16-bit values), the first unpack combines the low 8 bytes of input1 and input2 into a 16-byte vector, and similarly for input3/4. Then `epi16` unpacks combine these to produce the correct ordering. Each iteration writes 4*16 = 64 bytes to the output. Edge cases: `length` may not be a multiple of 16 bytes (8 values per block); the scalar fallback processes remaining elements one 16-bit value at a time. Also, if `__SSE4_2__` is not defined, we must provide a pure scalar version. The time complexity is O(length) (processing each byte once), and space complexity is O(1) beyond the input/output buffers. The solution must be careful with pointer arithmetic: `src` points to four consecutive blocks, each of `length` bytes; we advance each source pointer by 16 bytes per SIMD iteration. After the SIMD loop, we handle the tail with scalar reads/writes. All reads/writes use unaligned loads/stores since buffers may not be aligned.

#include <cstdint>
#include <cstddef>

#ifdef __SSE4_2__
#include <emmintrin.h>
#include <smmintrin.h>
#endif

// Interleave four arrays of 16-bit signed integers (each of 'length' bytes)
// from 'src' (containing four consecutive blocks) into 'dst' in round-robin order.
// 'length' is the byte size of each input block; output size is 4*length bytes.
// The function is safe for any alignment, handles lengths not multiple of 16 bytes.
void interleave_4_sse4(uint8_t* dst, const uint8_t* src, int32_t length)
{
    // Number of whole 16-byte chunks (each chunk holds 8 int16 values per input).
    int32_t chunks = length / 16;

    // Pointers to the four input blocks.
    const uint8_t* pSrc1 = src;
    const uint8_t* pSrc2 = src + length;
    const uint8_t* pSrc3 = src + 2 * length;
    const uint8_t* pSrc4 = src + 3 * length;
    uint8_t* pDst = dst;

#ifdef __SSE4_2__
    // SIMD path: process 16 bytes (8 elements) per iteration.
    for (int32_t i = 0; i < chunks; ++i) {
        // Load 16 bytes (8 int16) from each input.
        __m128i one0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSrc1));
        __m128i one1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSrc2));
        __m128i one2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSrc3));
        __m128i one3 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSrc4));

        // First stage: interleave low/high bytes of pairs.
        __m128i two0 = _mm_unpacklo_epi8(one0, one1);
        __m128i two1 = _mm_unpackhi_epi8(one0, one1);
        __m128i two2 = _mm_unpacklo_epi8(one2, one3);
        __m128i two3 = _mm_unpackhi_epi8(one2, one3);

        // Second stage: interleave 16-bit values to get final order.
        __m128i three0 = _mm_unpacklo_epi16(two0, two2);
        __m128i three1 = _mm_unpackhi_epi16(two0, two2);
        __m128i three2 = _mm_unpacklo_epi16(two1, two3);
        __m128i three3 = _mm_unpackhi_epi16(two1, two3);

        // Store four 16-byte results.
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDst), three0);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDst + 16), three1);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDst + 32), three2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDst + 48), three3);

        pSrc1 += 16;
        pSrc2 += 16;
        pSrc3 += 16;
        pSrc4 += 16;
        pDst += 64;
    }
#endif

    // Scalar fallback for remaining elements (also used entirely if no SSE4.2).
    int32_t remaining_start = chunks * 16;
    for (int32_t i = remaining_start; i < length; i += 2) { // each iteration processes one int16 per input
        // Read two bytes per input (little-endian int16).
        int16_t v1 = static_cast<int16_t>(pSrc1[0] | (pSrc1[1] << 8));
        int16_t v2 = static_cast<int16_t>(pSrc2[0] | (pSrc2[1] << 8));
        int16_t v3 = static_cast<int16_t>(pSrc3[0] | (pSrc3[1] << 8));
        int16_t v4 = static_cast<int16_t>(pSrc4[0] | (pSrc4[1] << 8));

        // Write interleaved values.
        pDst[0] = static_cast<uint8_t>(v1 & 0xFF);
        pDst[1] = static_cast<uint8_t>((v1 >> 8) & 0xFF);
        pDst[2] = static_cast<uint8_t>(v2 & 0xFF);
        pDst[3] = static_cast<uint8_t>((v2 >> 8) & 0xFF);
        pDst[4] = static_cast<uint8_t>(v3 & 0xFF);
        pDst[5] = static_cast<uint8_t>((v3 >> 8) & 0xFF);
        pDst[6] = static_cast<uint8_t>(v4 & 0xFF);
        pDst[7] = static_cast<uint8_t>((v4 >> 8) & 0xFF);

        pSrc1 += 2;
        pSrc2 += 2;
        pSrc3 += 2;
        pSrc4 += 2;
        pDst += 8;
    }
}

#include <cassert>
#include <cstdint>
#include <vector>
#include <cstring>

// The function under test.
void interleave_4_sse4(uint8_t* dst, const uint8_t* src, int32_t length);

int main() {
    // Test 1: length = 16 bytes (exactly one SIMD chunk, 8 values per input).
    {
        const int32_t length = 16;
        std::vector<uint8_t> src(4 * length);
        // Fill block 1 with values 100..107 (as little-endian int16)
        for (int i = 0; i < 8; ++i) {
            int16_t v = static_cast<int16_t>(100 + i);
            src[i * 2] = static_cast<uint8_t>(v & 0xFF);
            src[i * 2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        }
        // Block 2: values 200..207
        for (int i = 0; i < 8; ++i) {
            int16_t v = static_cast<int16_t>(200 + i);
            src[length + i * 2] = static_cast<uint8_t>(v & 0xFF);
            src[length + i * 2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        }
        // Block 3: values 300..307
        for (int i = 0; i < 8; ++i) {
            int16_t v = static_cast<int16_t>(300 + i);
            src[2 * length + i * 2] = static_cast<uint8_t>(v & 0xFF);
            src[2 * length + i * 2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        }
        // Block 4: values 400..407
        for (int i = 0; i < 8; ++i) {
            int16_t v = static_cast<int16_t>(400 + i);
            src[3 * length + i * 2] = static_cast<uint8_t>(v & 0xFF);
            src[3 * length + i * 2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        }
        std::vector<uint8_t> dst(4 * length);
        interleave_4_sse4(dst.data(), src.data(), length);
        // Expected output: 100,200,300,400,101,201,301,401,... in little-endian.
        for (int i = 0; i < 8; ++i) {
            int16_t expected[4] = {static_cast<int16_t>(100+i), static_cast<int16_t>(200+i),
                                   static_cast<int16_t>(300+i), static_cast<int16_t>(400+i)};
            for (int j = 0; j < 4; ++j) {
                int16_t got = static_cast<int16_t>(dst[(i*4+j)*2] | (dst[(i*4+j)*2+1] << 8));
                assert(got == expected[j]);
            }
        }
    }

    // Test 2: length = 20 bytes (not multiple of 16; 10 values per input, triggers scalar tail).
    {
        const int32_t length = 20;
        std::vector<uint8_t> src(4 * length);
        for (int b = 0; b < 4; ++b) {
            for (int i = 0; i < 10; ++i) {
                int16_t v = static_cast<int16_t>(b * 1000 + i);
                src[b * length + i * 2] = static_cast<uint8_t>(v & 0xFF);
                src[b * length + i * 2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
            }
        }
        std::vector<uint8_t> dst(4 * length);
        interleave_4_sse4(dst.data(), src.data(), length);
        for (int i = 0; i < 10; ++i) {
            int16_t expected[4] = {static_cast<int16_t>(i), static_cast<int16_t>(1000+i),
                                   static_cast<int16_t>(2000+i), static_cast<int16_t>(3000+i)};
            for (int j = 0; j < 4; ++j) {
                int16_t got = static_cast<int16_t>(dst[(i*4+j)*2] | (dst[(i*4+j)*2+1] << 8));
                assert(got == expected[j]);
            }
        }
    }

    // Test 3: length = 0 (empty input).
    {
        const int32_t length = 0;
        std::vector<uint8_t> src(0), dst(0);
        interleave_4_sse4(dst.data(), src.data(), length);
        assert(dst.empty());
    }

    // Test 4: length = 2 (one int16 per input, only scalar path).
    {
        const int32_t length = 2;
        uint8_t src[4*2] = {1,0, 2,0, 3,0, 4,0}; // values 1,2,3,4
        uint8_t dst[8] = {0};
        interleave_4_sse4(dst, src, length);
        // Expected interleaved: 1,2,3,4
        assert(dst[0]==1 && dst[1]==0 && dst[2]==2 && dst[3]==0 &&
               dst[4]==3 && dst[5]==0 && dst[6]==4 && dst[7]==0);
    }

    // Test 5: large length (1024 bytes) to stress SIMD loop heavily.
    {
        const int32_t length = 1024;
        std::vector<uint8_t> src(4 * length);
        // Deterministic pattern: each byte equals its position modulo 251.
        for (size_t i = 0; i < src.size(); ++i) src[i] = static_cast<uint8_t>((i % 251));
        std::vector<uint8_t> dst(4 * length);
        interleave_4_sse4(dst.data(), src.data(), length);
        // Build expected output manually.
        std::vector<uint8_t> expected(4 * length);
        size_t out = 0;
        for (int i = 0; i < length / 2; ++i) { // each int16
            for (int b = 0; b < 4; ++b) {
                int val = (b * length + i * 2) % 251;
                expected[out++] = static_cast<uint8_t>(val);
                expected[out++] = 0; // actually the second byte is not from modulo pattern? let's fix: we need full 16-bit
            }
        }
        // Simpler: just compare against a direct scalar reference implementation.
        // Build reference.
        std::vector<uint8_t> ref(4 * length);
        uint8_t* ps1 = src.data();
        uint8_t* ps2 = src.data() + length;
        uint8_t* ps3 = src.data() + 2*length;
        uint8_t* ps4 = src.data() + 3*length;
        uint8_t* pd = ref.data();
        for (int i = 0; i < length; i += 2) {
            *pd++ = *ps1++; *pd++ = *ps1++;
            *pd++ = *ps2++; *pd++ = *ps2++;
            *pd++ = *ps3++; *pd++ = *ps3++;
            *pd++ = *ps4++; *pd++ = *ps4++;
        }
        assert(std::memcmp(dst.data(), ref.data(), ref.size()) == 0);
    }

    return 0;
}
