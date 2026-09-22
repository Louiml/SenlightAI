/*
Write a C++ function `splitBlocks` that takes an input vector of integers representing raw codewords (each in the range 0–255) and two parameters: the number of error correction codewords per block and a vector of block descriptions, where each description is a pair `{count, numDataCodewords}`. The function must partition the raw codewords into consecutive data blocks, exactly as the given DataBlock::getDataBlocks algorithm does: fill data codewords for all blocks (shorter blocks first), then add the extra data byte to longer blocks, then fill error correction codewords interleaved block-by-block. The input is guaranteed valid (no underflow/overflow), but the function should throw `std::invalid_argument` if block sizes differ by more than 1 or if the total number of consumed codewords does not match the input size. Return a vector of vectors, where each inner vector represents a block’s codewords (data + error correction). Assume the input order of block descriptions is as in the original: for each description, create `count` blocks, and blocks are grouped by description. The first block created defines the "shorter" block length if any; longer blocks are those at the end of the result list that have one extra codeword.
*/
#include <vector>
#include <stdexcept>
#include <utility>

using BlockSpec = std::pair<int, int>; // {count, numDataCodewords}

// Partition rawCodewords into data blocks as described.
std::vector<std::vector<int>> splitBlocks(
    const std::vector<int>& rawCodewords,
    int ecCodewords,
    const std::vector<BlockSpec>& blockSpecs) {
    
    // Count total number of blocks
    int totalBlocks = 0;
    for (const auto& spec : blockSpecs) {
        totalBlocks += spec.first;
    }
    
    // Create block vectors with appropriate sizes
    std::vector<std::vector<int>> result(totalBlocks);
    int numResultBlocks = 0;
    for (const auto& spec : blockSpecs) {
        int count = spec.first;
        int numDataCodewords = spec.second;
        for (int i = 0; i < count; ++i) {
            int blockSize = ecCodewords + numDataCodewords;
            result[numResultBlocks++] = std::vector<int>(blockSize);
        }
    }
    
    // Determine which blocks are longer (by 1 codeword)
    int shorterBlocksTotalCodewords = result[0].size();
    int longerBlocksStartAt = static_cast<int>(result.size()) - 1;
    while (longerBlocksStartAt >= 0) {
        int numCodewords = static_cast<int>(result[longerBlocksStartAt].size());
        if (numCodewords == shorterBlocksTotalCodewords) {
            break;
        }
        if (numCodewords != shorterBlocksTotalCodewords + 1) {
            throw std::invalid_argument("Data block sizes differ by more than 1");
        }
        longerBlocksStartAt--;
    }
    longerBlocksStartAt++;
    
    int shorterBlocksNumDataCodewords = shorterBlocksTotalCodewords - ecCodewords;
    int rawOffset = 0;
    
    // Fill data codewords common to all blocks
    for (int i = 0; i < shorterBlocksNumDataCodewords; ++i) {
        for (int j = 0; j < numResultBlocks; ++j) {
            result[j][i] = rawCodewords[rawOffset++];
        }
    }
    
    // Fill the extra data codeword for longer blocks
    for (int j = longerBlocksStartAt; j < numResultBlocks; ++j) {
        result[j][shorterBlocksNumDataCodewords] = rawCodewords[rawOffset++];
    }
    
    // Fill error correction codewords
    int max = static_cast<int>(result[0].size());
    for (int i = shorterBlocksNumDataCodewords; i < max; ++i) {
        for (int j = 0; j < numResultBlocks; ++j) {
            int iOffset = (j < longerBlocksStartAt) ? i : (i + 1);
            result[j][iOffset] = rawCodewords[rawOffset++];
        }
    }
    
    if (rawOffset != static_cast<int>(rawCodewords.size())) {
        throw std::invalid_argument("rawCodewordsOffset != rawCodewords.length");
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <stdexcept>

int main() {
    // Example: 1 block, 4 data + 2 EC = 6 codewords
    std::vector<int> raw1 = {10, 20, 30, 40, 50, 60};
    auto blocks1 = splitBlocks(raw1, 2, {{1, 4}});
    assert(blocks1.size() == 1);
    assert(blocks1[0] == raw1);

    // Two blocks: both same size, 2 data + 1 EC = 3 each
    std::vector<int> raw2 = {1, 2, 3, 4, 5, 6};
    auto blocks2 = splitBlocks(raw2, 1, {{2, 2}});
    assert(blocks2.size() == 2);
    assert(blocks2[0] == std::vector<int>({1, 2, 3}));
    assert(blocks2[1] == std::vector<int>({4, 5, 6}));

    // Three blocks: first two are shorter (2 data), last is longer (3 data), 1 EC each
    // Shorter block total = 3, longer total = 4
    std::vector<int> raw3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Data phase: for i in 0..1: j=0,1,2 -> consume 1,4,7; 2,5,8
    // then extra data for longer (j=2): consume 3
    // EC phase: i=2 to max=3? Actually shorterBlocksTotal=3, shorterData=2, max=3
    // for i=2: j=0->block0[2]=6, j=1->block1[2]=9, j=2->iOffset=3, block2[3]=10
    auto blocks3 = splitBlocks(raw3, 1, {{2, 2}, {1, 3}});
    assert(blocks3.size() == 3);
    assert(blocks3[0] == std::vector<int>({1, 2, 6}));
    assert(blocks3[1] == std::vector<int>({4, 5, 9}));
    assert(blocks3[2] == std::vector<int>({7, 3, 8, 10}));

    // Edge: all blocks same size, no longer blocks
    std::vector<int> raw4 = {0, 1, 2, 3};
    auto blocks4 = splitBlocks(raw4, 0, {{2, 2}});
    assert(blocks4.size() == 2);
    assert(blocks4[0] == std::vector<int>({0, 1}));
    assert(blocks4[1] == std::vector<int>({2, 3}));

    // Edge: zero EC codewords, mixed sizes (should throw if differ by 1? yes valid)
    std::vector<int> raw5 = {1, 2, 3, 4};
    auto blocks5 = splitBlocks(raw5, 0, {{1, 1}, {1, 3}});
    assert(blocks5.size() == 2);
    assert(blocks5[0] == std::vector<int>({1, 4}));
    assert(blocks5[1] == std::vector<int>({2, 3, 5}) ); // actually data: first fills 1,2; then longer extra 3; no EC

    // Throw on invalid size difference
    bool threw = false;
    try {
        splitBlocks({1, 2, 3, 4, 5}, 0, {{1, 1}, {1, 2}});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Throw on length mismatch
    threw = false;
    try {
        splitBlocks({1, 2}, 0, {{1, 1}});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
// The algorithm reconstructs the exact interleaving used in Data Matrix error correction. First, compute the total number of blocks by summing the counts of each description. For each block, allocate a buffer of size `numDataCodewords + ecCodewords`. Then identify which blocks are "longer" (size `shorterLength + 1`) by scanning from the end of the block list; if any block differs by more than 1, throw. The shorter data length is `shorterBlockTotal - ecCodewords`. Fill in three phases: (1) for each data index from 0 to shorterDataLen-1, iterate over all blocks in order and copy one codeword from the raw input; (2) for blocks starting at longerBlocksStartAt, copy one extra data codeword at position shorterDataLen; (3) for each error correction index from shorterDataLen to maxBlockSize-1, iterate all blocks, but for longer blocks use index+1 within that block, copying from raw input. Finally, verify that exactly as many raw codewords were consumed as the input size; otherwise throw. Time complexity is O(totalCodewords) where totalCodewords is the sum of all block sizes, since each codeword is copied exactly once; space complexity is O(totalCodewords) for the result. Edge cases include blocks all the same size (longerBlocksStartAt equals result size, so no extra data fill) and descriptions with count 0 (skip). Also handle single-block scenarios where the only block may be shorter or longer relative to nothing—the scan ensures consistency.
