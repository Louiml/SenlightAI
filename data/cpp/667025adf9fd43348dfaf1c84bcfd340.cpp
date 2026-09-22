Write a C++ function `long long compactDiskChecksum(const std::string& input, bool wholeFileMode)` that takes a string of digits representing a disk map (the digit at even index i is the length of file i/2, and the digit at odd index i is the length of free space) and returns the resulting checksum after compacting the disk according to the specified mode. In whole-file mode (`wholeFileMode = true`), each file (identified by its ID number) is moved as a contiguous block to the leftmost free interval that can fit it, processing files from highest ID to lowest ID, without fragmenting any file. In block mode (`wholeFileMode = false`), individual blocks from the right end are moved one by one to the leftmost free slot until no free slot lies to the left of any file block. After compaction, the checksum is the sum over each block position `i` (0-indexed) of `i * fileID` for blocks occupied by files (free space contributes 0). For example, for input "12345", block mode yields checksum 60, and whole-file mode yields 132. The input consists only of digits and has at least one character; the output fits within a `long long`. The function must not use global variables, must be self-contained, and must be efficient for inputs up to length 100,000.
// The solution first expands the disk map string into a vector of `long long` where each position holds either a file ID (non-negative) or -1 for free space, with lengths taken directly from the digits. For block mode (`wholeFileMode = false`), we use a two-pointer/filling approach: we maintain an index `left` scanning from the beginning to find the first free slot (-1), and an index `right` scanning from the end to find the last non-free block. While `left < right`, we move the block at `right` into the free slot at `left` (setting `result[left] = result[right]` and `result[right] = -1`), then advance `left` past any consecutive non-free slots and decrement `right` past any free slots. This is essentially the provided `compress` function but can be implemented more cleanly with two pointers. Edge cases: all free spaces, no free spaces, or single file—these are naturally handled. Time complexity O(n) where n is the total number of blocks (sum of digits), space O(n). For whole-file mode (`wholeFileMode = true`), we need to move whole files left without fragmentation. First, build a map of free intervals (start index to length) from the expanded representation. Then process files from high ID to low ID (working right to left). For each file, determine its contiguous length by scanning left from the current position. Find the leftmost free interval with start index less than the file's start and length at least the file's length. If found, move the entire file block by copying its ID into that interval's start positions, set the original positions to -1, update the free interval map (remove the used interval and insert the remainder if any). After moving, continue processing the next file to the left. Important edge cases: no free interval fits the file (leave it), overlapping when moving—since we only move left, and we process from rightmost files first, we avoid moving a file into a position that already holds a file we haven't processed yet. Also, after moving a file, we must adjust the loop to skip the original file positions to avoid re-processing. Time complexity is O(m * f) in worst case, where m is the number of free intervals and f is the number of distinct files, but since we iterate through positions and each file is processed once and each free interval is touched a constant number of times, in practice it is O(n * log n) due to map operations, with n being total blocks. Space O(n). We implement both modes in one function with a conditional branch, sharing the expansion step.
#include <vector>
#include <string>
#include <map>
#include <cstddef>

// Computes checksum after compacting disk according to given mode.
// wholeFileMode=true: move whole files (contiguous blocks) left to leftmost fitting free interval.
// wholeFileMode=false: move individual blocks from right to leftmost free slot.
long long compactDiskChecksum(const std::string& s, bool wholeFileMode) {
    // Expand the disk map into a vector of block IDs, -1 for free.
    std::vector<long long> disk;
    for (std::size_t i = 0; i < s.size(); ++i) {
        long long id = (i % 2 == 0) ? static_cast<long long>(i / 2) : -1;
        int len = s[i] - '0';
        disk.insert(disk.end(), len, id);
    }
    long long n = static_cast<long long>(disk.size());

    if (!wholeFileMode) {
        // Block mode: two-pointer compaction.
        long long left = 0;
        long long right = n - 1;
        while (left < right) {
            // Find first free slot from left.
            while (left < n && disk[left] != -1) ++left;
            // Find last non-free block from right.
            while (right >= 0 && disk[right] == -1) --right;
            if (left < right) {
                disk[left] = disk[right];
                disk[right] = -1;
                ++left;
                --right;
            }
        }
    } else {
        // Whole-file mode.
        // Build free intervals map: start index -> length.
        std::map<long long, long long> freeIntervals;
        long long i = 0;
        while (i < n) {
            if (disk[i] == -1) {
                long long j = i;
                while (j < n && disk[j] == -1) ++j;
                freeIntervals[i] = j - i;
                i = j;
            } else {
                ++i;
            }
        }

        // Process files from right to left, skipping already processed (larger IDs).
        long long lastConsidered = -1; // last ID we processed (higher IDs are already handled)
        long long pos = n - 1;
        while (pos >= 0) {
            if (disk[pos] == -1 || disk[pos] <= lastConsidered) {
                --pos;
                continue;
            }
            // pos points to the rightmost block of a file with ID = disk[pos]
            long long fileID = disk[pos];
            // Find its leftmost block.
            long long start = pos;
            while (start >= 0 && disk[start] == fileID) --start;
            ++start; // now start = first index of this file
            long long fileLen = pos - start + 1;
            lastConsidered = fileID; // mark this ID as processed

            // Find leftmost free interval that can fit this file and lies to the left of start.
            long long chosenStart = -1;
            for (auto it = freeIntervals.begin(); it != freeIntervals.end(); ++it) {
                if (it->first >= start) break; // no free space to the left
                if (it->second >= fileLen) {
                    chosenStart = it->first;
                    // Move the file to the free interval.
                    for (long long j = 0; j < fileLen; ++j) {
                        disk[start + j] = -1;
                        disk[chosenStart + j] = fileID;
                    }
                    // Update free intervals: remove used interval, insert remainder if any.
                    long long remaining = it->second - fileLen;
                    freeIntervals.erase(it);
                    if (remaining > 0) {
                        freeIntervals[chosenStart + fileLen] = remaining;
                    }
                    break;
                }
            }
            // Move to the next file to the left (skip the file we just processed).
            pos = start - 1;
        }
    }

    // Compute checksum.
    long long checksum = 0;
    for (long long i = 0; i < n; ++i) {
        if (disk[i] != -1) {
            checksum += i * disk[i];
        }
    }
    return checksum;
}
#include <cassert>
#include <string>

// Declare the function under test.
long long compactDiskChecksum(const std::string& s, bool wholeFileMode);

int main() {
    // Example from the problem statement: "12345"
    // Block mode: compact [0, -1, -1, 1, 1, 1, -1, -1, -1, -1, 2, 2, ...] 
    // Actually expand: 0, -1, -1, 1,1,1, -1,-1,-1,-1, 2,2,2,2,2
    // Move blocks: 2->pos1, 2->pos2, 1->pos5? Let's trust known checksum 60.
    assert(compactDiskChecksum("12345", false) == 60);
    // Whole-file mode: move file 2 (len 5) into free space at index 2 (len 2? no) 
    // Actually file 2 cannot fit, file 1 moves to index 2, checksum=132.
    assert(compactDiskChecksum("12345", true) == 132);

    // Simple single file, no free space: checksum = 0*len? For "3" => file0 len3 => positions 0,1,2 => checksum 0*0+1*0+2*0=0
    assert(compactDiskChecksum("3", false) == 0);
    assert(compactDiskChecksum("3", true) == 0);

    // All free space (odd digit only, e.g., "5" meaning no files): checksum 0.
    assert(compactDiskChecksum("5", false) == 0);
    assert(compactDiskChecksum("5", true) == 0);

    // Mixed: "101" -> file0 len1, free1, file1 len1. Expanded: [0, -1, 1]
    // Block mode: move 1 into free slot -> [0,1,-1] checksum = 0*0 +1*1 =1
    assert(compactDiskChecksum("101", false) == 1);
    // Whole-file mode: file1 len1 can move into free interval at index1 -> [0,1,-1] checksum=1
    assert(compactDiskChecksum("101", true) == 1);

    // "909" -> file0 len9, free0, file1 len9. Expanded: [0x9, -1x0, 1x9] = 0..8, then 9..17 are 1.
    // Block mode: no free space, checksum = sum_{i=0}^{8} i*0 + sum_{i=9}^{17} i*1 = 9+10+...+17 = (9+17)*9/2=117
    assert(compactDiskChecksum("909", false) == 117);
    // Whole-file: same, no movement, checksum 117.
    assert(compactDiskChecksum("909", true) == 117);

    // "2333133121414131402" is a known example from Advent of Code 2024 Day 9.
    // Expected: part1 (block) = 1928, part2 (whole) = 2858.
    assert(compactDiskChecksum("2333133121414131402", false) == 1928);
    assert(compactDiskChecksum("2333133121414131402", true) == 2858);

    // Edge: leading free space "01" -> file0 len0, free1, file1 len1? Actually string "01": i=0 -> file0 len0, i=1 -> free len1. So disk is [-1]. No files. checksum 0.
    assert(compactDiskChecksum("01", false) == 0);
    assert(compactDiskChecksum("01", true) == 0);
    return 0;
}
