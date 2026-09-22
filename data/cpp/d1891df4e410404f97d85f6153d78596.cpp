/*
Write a C++ function `countSetBitsPerRow` that takes a vector of bytes (each byte represented as `uint8_t`), where the bytes are stored in a row‑major layout exactly as in the provided Rcpp snippet: the input is a flat vector `data` of size `rows * cols` (where `rows` is the number of splits and `cols` is the number of bins). For each row `i` (0 ≤ i < rows), the function must compute the total number of set bits (i.e., bits equal to 1) across all `cols` bytes of that row, using the built‑in `__builtin_popcount` for each byte, and return a `std::vector<int>` of length `rows` containing these counts. Your function must handle `rows` and `cols` being zero (returning an empty vector in that case), and must use `const` appropriately on inputs. Assume the input vector size is always exactly `rows * cols`; you do not need to validate that. The function signature must be: `std::vector<int> countSetBitsPerRow(const std::vector<uint8_t>& data, std::size_t rows, std::size_t cols)`.
*/
#include <cstdint>
#include <vector>

// Count set bits in each row of a row-major byte matrix.
// data has size rows * cols; returns a vector of length rows.
std::vector<int> countSetBitsPerRow(const std::vector<uint8_t>& data,
                                    std::size_t rows,
                                    std::size_t cols) {
    std::vector<int> result(rows, 0);

    for (std::size_t i = 0; i < rows; ++i) {
        const std::size_t row_start = i * cols;
        int count = 0;
        for (std::size_t j = 0; j < cols; ++j) {
            count += __builtin_popcount(static_cast<unsigned int>(data[row_start + j]));
        }
        result[i] = count;
    }

    return result;
}
#include <cassert>
#include <cstdint>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Empty input: no rows.
    {
        std::vector<uint8_t> data;  // empty
        std::vector<int> result = countSetBitsPerRow(data, 0, 0);
        assert(result.empty());
    }

    // One row, one byte with value 0 (no bits) and 0xFF (8 bits).
    {
        std::vector<uint8_t> data = {0x00};
        assert(countSetBitsPerRow(data, 1, 1) == std::vector<int>{0});

        data[0] = 0xFF;
        assert(countSetBitsPerRow(data, 1, 1) == std::vector<int>{8});
    }

    // Two rows, one byte each: row0=0x0F (4 bits), row1=0xAA (4 bits)
    {
        std::vector<uint8_t> data = {0x0F, 0xAA};
        assert(countSetBitsPerRow(data, 2, 1) == std::vector<int>{4, 4});
    }

    // One row, multiple bytes: 0x01 (1 bit), 0x80 (1 bit), 0x00 (0 bits)
    {
        std::vector<uint8_t> data = {0x01, 0x80, 0x00};
        assert(countSetBitsPerRow(data, 1, 3) == std::vector<int>{2});
    }

    // Multiple rows and columns: row0 = {0xFF,0x00} → 8+0=8; row1 = {0x0F,0xF0} → 4+4=8; row2 = {0x00,0xFF} → 0+8=8
    {
        std::vector<uint8_t> data = {0xFF,0x00, 0x0F,0xF0, 0x00,0xFF};
        assert(countSetBitsPerRow(data, 3, 2) == std::vector<int>{8, 8, 8});
    }

    // Zero columns but non‑zero rows: each row has zero bytes -> all counts are 0
    {
        std::vector<uint8_t> data; // empty, size = rows*cols = 3*0 = 0
        assert(countSetBitsPerRow(data, 3, 0) == std::vector<int>{0,0,0});
    }

    // Large test with mixed bytes: row0 = 2 bytes (0xA5=5 bits, 0x5A=6 bits) -> 11; row1 = 2 bytes (0x00=0, 0xFF=8) -> 8
    {
        std::vector<uint8_t> data = {0xA5, 0x5A, 0x00, 0xFF};
        assert(countSetBitsPerRow(data, 2, 2) == std::vector<int>{5+6, 0+8});
    }

    // Single row, single column with value 0x55 (4 bits)
    {
        std::vector<uint8_t> data = {0x55};
        assert(countSetBitsPerRow(data, 1, 1) == std::vector<int>{4});
    }

    // Edge: rows=1, cols=0 (empty data) returns one zero
    {
        std::vector<uint8_t> data;
        assert(countSetBitsPerRow(data, 1, 0) == std::vector<int>{0});
    }

    // Edge: rows=0, cols=5 (empty data) returns empty
    {
        std::vector<uint8_t> data;
        assert(countSetBitsPerRow(data, 0, 5).empty());
    }

    return 0;
}
// The problem is a direct translation of the provided Rcpp function to standard C++ with a simpler interface. The core idea is to iterate row by row (from 0 to rows‑1) and for each row, iterate over all its `cols` bytes. The bytes of row `i` are located at indices `i * cols + 0`, `i * cols + 1`, …, `i * cols + (cols-1)`. For each byte, we use `__builtin_popcount(byte)` to count how many bits are set in that byte. Summing these counts over all `cols` bytes gives the total for that row. Important edge cases: (1) If `rows == 0` or `cols == 0`, there are no rows or no data to process, but we must still return a vector of size `rows` (which is zero if `rows == 0`); if `rows > 0` but `cols == 0`, each row has zero bytes, so each count is zero. (2) The data vector is guaranteed to be of size `rows * cols`, so no out‑of‑bounds access occurs if we iterate exactly up to those bounds. (3) `__builtin_popcount` works on `unsigned int`; `uint8_t` is promoted to `int`, but since the value is 0–255, it is safe. Time complexity is O(rows × cols) because we visit every byte exactly once, and each `__builtin_popcount` runs in constant time (usually a single CPU instruction). Space complexity is O(rows) for the output vector, in addition to the input (which we do not copy). The solution is straightforward and mirrors the original logic but uses a clearer interface.
