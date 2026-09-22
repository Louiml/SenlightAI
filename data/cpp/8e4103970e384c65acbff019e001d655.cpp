/*
Write a C++ function `writePeakValue` that simulates the core behavior of the provided `LVPSA_QPD_WritePeak_Float` function, but in a simplified, standalone manner. The function must take a resizable buffer (e.g., `std::vector<uint8_t>&`) that represents a circular spectrum data buffer organized as `numBands` contiguous bands per time slot, with `bufferLength` time slots total. The function also takes a `bandIndex` (0-based), a floating-point `value` in the range [0.0, 1.0], and an in/out parameter `writeIndex` that gives the current slot position (0-based) where the band value should be written. It must write `static_cast<uint8_t>(value * 256.0f)` (clamped to 255 if needed) at position `writeIndex * numBands + bandIndex`. Then it must advance `writeIndex` to the next slot, wrapping around to 0 if it reaches `bufferLength`. The function must not return anything and must modify the buffer and the `writeIndex` reference accordingly. Assume the buffer always has the exact size `numBands * bufferLength`.
*/

#include <cstdint>
#include <vector>
#include <algorithm>

// Write a peak level value (0.0 to 1.0) into a circular spectral data buffer.
// The buffer is organized as numBands values per time slot, with bufferLength slots.
// writeIndex is updated to point to the next slot, wrapping circularly.
void writePeakValue(std::vector<uint8_t>& buffer, size_t numBands, size_t bufferLength,
                    size_t bandIndex, float value, size_t& writeIndex) {
    // Convert float value (assumed 0.0..1.0) to 8-bit unsigned integer.
    // Multiply by 256 and clamp to 255 to avoid overflow from rounding.
    uint8_t byteValue = static_cast<uint8_t>(std::min(255.0f, value * 256.0f));

    // Compute the linear index and write the value.
    size_t targetIndex = writeIndex * numBands + bandIndex;
    buffer[targetIndex] = byteValue;

    // Advance the write index, wrapping around to the beginning.
    ++writeIndex;
    if (writeIndex == bufferLength) {
        writeIndex = 0;
    }
}

#include <cassert>
#include <vector>
#include <cstddef>

// (Solution function is assumed to be included above)
int main() {
    // Test 1: Basic write and advance with 2 bands, 3 slots.
    std::vector<uint8_t> buf1(6, 0);
    size_t idx1 = 0;
    writePeakValue(buf1, 2, 3, 0, 0.5f, idx1);
    assert(buf1[0] == 128);        // 0.5*256 = 128
    assert(idx1 == 1);
    writePeakValue(buf1, 2, 3, 1, 1.0f, idx1);
    assert(buf1[1 * 2 + 1] == 255); // clamped to 255
    assert(idx1 == 2);

    // Test 2: Wrap around after reaching bufferLength.
    std::vector<uint8_t> buf2(2 * 2, 0); // 2 bands, 2 slots
    size_t idx2 = 0;
    writePeakValue(buf2, 2, 2, 0, 0.0f, idx2);
    writePeakValue(buf2, 2, 2, 1, 0.0f, idx2);
    assert(idx2 == 0); // wrapped around
    // Overwrite slot 0 with new values.
    writePeakValue(buf2, 2, 2, 0, 0.25f, idx2);
    assert(buf2[0] == 64);
    assert(idx2 == 1);

    // Test 3: Single band, buffer length 1.
    std::vector<uint8_t> buf3(1, 0);
    size_t idx3 = 0;
    writePeakValue(buf3, 1, 1, 0, 0.75f, idx3);
    assert(buf3[0] == 192);
    assert(idx3 == 0); // wraps immediately

    // Test 4: Ensure writes to different bands within the same slot do not overlap.
    std::vector<uint8_t> buf4(4, 0); // 2 bands, 2 slots
    size_t idx4 = 0;
    writePeakValue(buf4, 2, 2, 0, 1.0f, idx4); // writes to index 0
    writePeakValue(buf4, 2, 2, 1, 0.5f, idx4); // writes to index 1 (same slot since idx4 still 0)
    assert(buf4[0] == 255);
    assert(buf4[1] == 128);
    assert(idx4 == 1);

    // Test 5: Many cycles to verify consistent wrapping.
    std::vector<uint8_t> buf5(3 * 4, 0); // 3 bands, 4 slots
    size_t idx5 = 0;
    for (int i = 0; i < 10; ++i) {
        writePeakValue(buf5, 3, 4, i % 3, static_cast<float>(i) / 10.0f, idx5);
    }
    // After 10 writes, idx5 = 10 % 4 = 2 (since each write advances by 1).
    assert(idx5 == 2);
    // Check the slot written at the 9th write (i=9) had band 0 and value 0.9.
    // That write happened at slot index 9 % 4 = 1, then idx advanced to 2.
    assert(buf5[1 * 3 + 0] == static_cast<uint8_t>(0.9f * 256.0f));

    return 0;
}

// The main algorithm is straightforward: compute the target vector index as `writeIndex * numBands + bandIndex`, assign the byte value (careful to clamp float multiplication to 255 because `value` might be exactly 1.0 and `256.0f` could produce 256 after rounding). Then increment `writeIndex` by 1, and if it equals `bufferLength`, reset to 0. Edge cases include: a single band and buffer length 1 (should wrap immediately after one write), values at exactly 0.0 or 1.0, and a `bandIndex` that is the last band in a slot (which still writes correctly). Since the buffer is circular, repeated calls will overwrite old data after a full cycle. Time complexity is O(1) per call, and space complexity is O(1) extra (ignoring the vector itself). The function must be `noexcept` and use `const` for read-only parameters only.
