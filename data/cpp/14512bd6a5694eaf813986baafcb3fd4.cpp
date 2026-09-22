// Write a C++ function `countDigestBlocks` that simulates the block-counting logic of the `IteratedHashBase::Update` method from a cryptographic hash framework. Given a `uint64_t` current bit count (the total number of bits hashed so far) and a `size_t` input length in bytes, the function must compute and return the number of whole hash blocks of size 64 bytes that would be fully processed by the update routine, considering that the routine first fills any partial block from the current bit count’s block offset before processing full blocks. The function must handle the case where the current bit count is not block-aligned (i.e., there is leftover data in the current block) and the new input may or may not complete that block. The input length is guaranteed to be non-negative and the total bit count will not overflow 64 bits. Return the count as a `size_t`. Do not modify the input parameters.

The core logic mirrors the iterative hash update: the hash maintains an internal buffer of `BlockSize` (64 bytes). The current bit count determines how many bytes are already buffered in the current block: `num = currentBitCount % 512` (since 1 byte = 8 bits, 64 bytes = 512 bits). Then, the input bytes are processed in three phases:  
1. If there is leftover data (`num != 0`), we first try to fill the current block. If `num + len >= 64`, we fill the remaining `64 - num` bytes to complete one block (so that block is counted), then we advance the input pointer by `64 - num` and reduce `len` accordingly. If `num + len < 64`, no block is completed and the function returns 0.  
2. After handling the leftover, we process full blocks: `len / 64` full blocks are counted, and the remainder is left as new leftover.  
3. If after step 2 there is still leftover data (len becomes `len % 64`), that leftover does not complete a block.  

Edge cases:  
- When `num == 0` (block-aligned), we skip phase 1 and directly count `len / 64` blocks.  
- When `num + len` is exactly 64, exactly one block is completed and we set `num = 0`, then `len` becomes 0.  
- When `len` is zero, the function returns 0 regardless of `num`.  

The total number of blocks processed is the number of times the hash’s compression function (`HashBlock`) is invoked during the update. For a given `currentBitCount` and `len`, the number of completed blocks is:  
- If `num == 0`: `len / 64`  
- Else if `len < 64 - num`: `0`  
- Else: `1 + (len - (64 - num)) / 64`  

Time complexity is O(1) (constant arithmetic operations). Space complexity is O(1) as no extra data structures are used. The function must be careful with `size_t` arithmetic to avoid overflow, but given the guaranteed non-overflowing total bit count, simple arithmetic suffices. The input length is in bytes, but the current bit count is in bits; convert `currentBitCount` to bytes by dividing by 8 (integer division) for block offset calculation.

#include <cstddef>
#include <cstdint>

// Simulate the block-counting logic of IteratedHashBase::Update.
// Given the current total bit count and an input length in bytes,
// return the number of complete 64-byte blocks that would be hashed.
size_t countDigestBlocks(uint64_t currentBitCount, size_t inputLength) {
    const size_t blockSize = 64; // bytes
    const size_t num = static_cast<size_t>((currentBitCount / 8) % blockSize); // bytes in current block

    if (num == 0) {
        // Block-aligned: every full block is processed directly.
        return inputLength / blockSize;
    }

    const size_t available = blockSize - num; // bytes needed to complete current block
    if (inputLength < available) {
        // Not enough input to complete the current block; no full block processed.
        return 0;
    }

    // Complete the current block (count 1), then process remaining full blocks.
    size_t remaining = inputLength - available;
    return 1 + (remaining / blockSize);
}

#include <cassert>

int main() {
    // Block-aligned current bit count (0 bits -> 0 bytes offset)
    assert(countDigestBlocks(0, 0) == 0);
    assert(countDigestBlocks(0, 64) == 1);
    assert(countDigestBlocks(0, 128) == 2);
    assert(countDigestBlocks(0, 63) == 0);

    // Current bit count not block-aligned (e.g., 100 bits = 12 bytes, offset 12)
    assert(countDigestBlocks(100, 0) == 0);
    assert(countDigestBlocks(100, 51) == 0);  // need 52 to complete block
    assert(countDigestBlocks(100, 52) == 1);  // exactly completes one block
    assert(countDigestBlocks(100, 53) == 1);  // 1 block + 1 leftover
    assert(countDigestBlocks(100, 116) == 2); // fill block + one more full block

    // Boundary: offset 63 (currentBitCount = 504 bits)
    assert(countDigestBlocks(504, 1) == 1);   // exactly completes the block
    assert(countDigestBlocks(504, 2) == 1);
    assert(countDigestBlocks(504, 65) == 2);  // complete 63-byte block + 1 full block

    // Large input length
    assert(countDigestBlocks(8, 1000) == 15); // offset 1, need 63, then (1000-63)/64=14, total 15
}
