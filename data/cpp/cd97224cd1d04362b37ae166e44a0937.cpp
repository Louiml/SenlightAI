// Write a C++ function that takes a string containing multiple blocks separated by double newlines (`\n\n`). Each block contains one or more lines of whitespace-separated integers. The function must compute, for each block, the sum of all integers in that block, then return the largest block sum found across all blocks. If the input is empty or contains no blocks with integers, return 0. The input may have arbitrary leading/trailing whitespace, and integers can be negative or positive. The function signature is `int maxBlockSum(const std::string& input)`.
The task is to parse a structured string where blocks are separated by exactly two newline characters. Within each block, integers are separated by whitespace (spaces, tabs, newlines). Use a string stream to parse integers line by line or directly from the whole block after splitting. First, split the input into blocks using `\n\n` as delimiter. For each block, extract all integers (using a stream or regex), sum them, and track the maximum sum. Edge cases: empty input returns 0; a block with no integers contributes 0 to the maximum (but should not incorrectly override if other blocks have positive sums); negative sums are allowed and should be considered. Since the input uses `\n\n`, and newlines within a block separate lines, but integers can be on the same line, it's easiest to treat the entire block as a whitespace-separated stream of integers. Time complexity is O(N) where N is the total number of characters in the input, because we parse each character once. Space complexity is O(K) where K is the size of the largest block (for storing its split lines or the substring), but we can do O(1) extra space by parsing directly without storing all blocks—simply iterate from the start, collecting numbers until we hit a double newline, then finalize the block sum. However, for simplicity, we can split into blocks, which is O(T) space for all blocks, but acceptable. To be efficient, use a manual scan: read characters, when we encounter a digit or minus sign, parse the integer, add to current block sum; when we see two consecutive newlines, finalize the current block sum and compare to maximum. This avoids extra storage.
#include <string>
#include <cctype>
#include <algorithm>

// Given a string with blocks separated by "\n\n", return the maximum sum of integers in any block.
int maxBlockSum(const std::string& input) {
    if (input.empty()) return 0;

    int max_sum = 0;          // best block sum seen so far
    int current_sum = 0;      // sum of current block
    bool in_block = false;    // whether we are inside a block
    bool has_number = false;  // whether current block has at least one number

    size_t i = 0;
    size_t n = input.size();

    while (i < n) {
        // Skip non-digit characters (except minus sign for negatives)
        if (std::isdigit(static_cast<unsigned char>(input[i])) || input[i] == '-') {
            // Parse the integer
            size_t start = i;
            if (input[i] == '-') {
                i++;
            }
            while (i < n && std::isdigit(static_cast<unsigned char>(input[i]))) {
                i++;
            }
            // Convert substring to int
            std::string num_str = input.substr(start, i - start);
            int num = std::stoi(num_str);
            current_sum += num;
            has_number = true;
            in_block = true;
            continue; // i is already at next character after number
        }
        
        // Check for double newline (block separator)
        if (input[i] == '\n' && i + 1 < n && input[i+1] == '\n') {
            // End of a block if we were inside one
            if (has_number) {
                max_sum = std::max(max_sum, current_sum);
                current_sum = 0;
                has_number = false;
                in_block = false;
            }
            i += 2; // skip both newlines
            continue;
        }
        
        // If we see a single newline or other whitespace, just move on
        i++;
    }

    // Handle the last block if there is one
    if (has_number) {
        max_sum = std::max(max_sum, current_sum);
    }

    return max_sum;
}
#include <cassert>
#include <string>
#include <iostream>

// Assume solution function is declared above
int maxBlockSum(const std::string& input);

int main() {
    // Basic case: two blocks
    assert(maxBlockSum("1 2\n\n3 4") == 7); // second block sums to 7
    // First block empty? Actually "1 2" sums to 3, second to 7 -> max 7

    // Single block with multiple lines
    assert(maxBlockSum("10\n20\n30") == 60);

    // Negative numbers
    assert(maxBlockSum("-5 -5\n\n-1 -2") == -3); // max is -3 (second block)

    // Empty input
    assert(maxBlockSum("") == 0);

    // Block with no numbers (just newlines) should be ignored
    assert(maxBlockSum("1 2\n\n   \n\n3 4") == 7); // middle empty block skipped

    // Leading/trailing whitespace
    assert(maxBlockSum("  \n 5 \n \n\n 10 ") == 10);

    // Large numbers
    assert(maxBlockSum("1000000 2000000\n\n500000") == 3000000);

    // Single number
    assert(maxBlockSum("42") == 42);

    // Negative only block
    assert(maxBlockSum("-10\n\n-5\n\n-1") == -1);

    std::cout << "All tests passed!\n";
    return 0;
}
