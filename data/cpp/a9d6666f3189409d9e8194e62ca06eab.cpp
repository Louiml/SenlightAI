// Write a C++ function that, given a valid 1-based position where a new disk block is added to a file, calculates the number of disk I/O operations required for contiguous, linked, and indexed file allocation strategies. The constants `BLOCK_SIZE = 1` and `FILE_SIZE = 100` are fixed, and the file after insertion will have exactly `FILE_SIZE + 1` blocks. For contiguous allocation, the cost is the number of disk blocks that must be shifted on disk (i.e., `BLOCK_SIZE * (number_of_blocks - position + 1)` if we consider the full block being written plus shifting, but the exact formula is: `(FILE_SIZE + 1 - position) * BLOCK_SIZE`). For linked allocation, the cost is the number of pointers to traverse from the start to the insertion point (i.e., `position - 1` pointer reads) plus 1 pointer read and 1 disk write for the new block (total `position - 1 + 2`). For indexed allocation, the cost is reading the index block (1 I/O) and writing the new data block (1 I/O), totaling 2. If the position is less than 1 or greater than `FILE_SIZE + 1`, the function should return a negative value (e.g., -1) to indicate an invalid position. Write a single free function `int calculateTotalIO(int position, int allocationType)` where `allocationType` is 0 for contiguous, 1 for linked, and 2 for indexed, that returns the number of I/O operations or -1 for invalid input. The function must be self-contained, use only standard headers, and not rely on any external data structures or global variables beyond the defined constants.
The solution requires simulating the three allocation strategies. The key is to first validate the position: it must be between 1 and `FILE_SIZE + 1` inclusive, otherwise return -1. For contiguous allocation, when adding a block at position `p`, all blocks from `p` to the end (inclusive of the new block but excluding the last block that remains) must be shifted right on disk. Since `BLOCK_SIZE` is 1, the number of blocks that need to be read and written (assuming a simple copy) is `(numBlocks - p)`, where `numBlocks = FILE_SIZE + 1`. The original snippet uses `(numBlocks - position) * BLOCK_SIZE`, which matches this count. For linked allocation, to insert at position `p`, you must traverse `p - 1` pointers to find the predecessor node, then write one pointer to the new node and write the new node's pointer to the next node; the snippet sums `position - 1` (traversal) + 2 (one pointer update for predecessor, one for new node's next pointer). For indexed allocation, you always read the index block once (1 I/O) and write the new data block once (1 I/O), giving a constant 2 regardless of position. Edge cases: position 1 (no traversal needed for linked, but still 2 writes), position `numBlocks` (contiguous requires 0 shifts, indexed still 2, linked requires traversing all blocks). The time complexity is O(1) for all cases because the computation is direct arithmetic with no loops or file operations simulated. Space complexity is O(1).
#include <cstddef>  // for size_t, if needed, but not required

// Constants for the simulated file system
constexpr int BLOCK_SIZE = 1;  
constexpr int FILE_SIZE = 100; 
constexpr int INVALID_POSITION = -1;

// Calculate disk I/O for adding a block at 1-based position.
// allocationType: 0 = contiguous, 1 = linked, 2 = indexed.
// Returns -1 if position is invalid, otherwise non-negative I/O count.
int calculateTotalIO(int position, int allocationType) {
    const int numBlocks = FILE_SIZE + 1; // after insertion, there are 101 blocks

    // Validate position: must be 1..numBlocks (inclusive)
    if (position < 1 || position > numBlocks) {
        return INVALID_POSITION;
    }

    int ioOperations = 0;
    switch (allocationType) {
        case 0: { // Contiguous: shift all blocks after insertion point
            ioOperations = (numBlocks - position) * BLOCK_SIZE;
            break;
        }
        case 1: { // Linked: traverse to predecessor + write two pointers
            ioOperations = (position - 1) + 2; // traversal + new block's pointer + update predecessor
            break;
        }
        case 2: { // Indexed: read index + write data block
            ioOperations = 2; // 1 read index, 1 write data block
            break;
        }
        default:
            return INVALID_POSITION; // unspecified allocation type
    }
    return ioOperations;
}
#include <cassert>

// Assume the solution function is declared above (or included here)
int main() {
    // Test invalid positions
    assert(calculateTotalIO(0, 0) == -1);
    assert(calculateTotalIO(102, 0) == -1);
    assert(calculateTotalIO(1, 3) == -1); // invalid allocation type

    // Contiguous allocation (allocationType = 0)
    assert(calculateTotalIO(1, 0) == (100) * 1); // shifting 100 blocks
    assert(calculateTotalIO(50, 0) == (51) * 1); // shifting 51 blocks
    assert(calculateTotalIO(101, 0) == 0);       // at the end, no shifts

    // Linked allocation (allocationType = 1)
    assert(calculateTotalIO(1, 1) == 2);   // no traversal, 2 pointer writes
    assert(calculateTotalIO(50, 1) == 51); // 49 traversal + 2
    assert(calculateTotalIO(101, 1) == 101); // 100 traversal + 2

    // Indexed allocation (allocationType = 2)
    assert(calculateTotalIO(1, 2) == 2);
    assert(calculateTotalIO(50, 2) == 2);
    assert(calculateTotalIO(101, 2) == 2);

    return 0;
}
