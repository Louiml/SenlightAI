Write a C++ function named `printMultiplicationTable` that takes no parameters and returns a `std::string` containing the complete 1×1 through 9×9 multiplication table, exactly formatted as shown: each multiplication line uses `printf`-style formatting with `%2d *%2d = %2d` (two-character right-aligned numbers for all three fields), each row (for a fixed multiplier) is followed by a newline, and after the last row, there is also a trailing newline (i.e., a blank line between each multiplier block, including after the 9×9 block). The output must match the exact spacing and newline placement produced by the original code snippet. The function should be pure (no side effects, no console output) and should build and return the entire table as a single string. Assume the environment supports standard C++ I/O and string formatting.

// The solution must reproduce the exact output of the nested loop in the snippet. The outer loop iterates `dan` from 1 to 9, and the inner loop iterates `su` from 1 to 9. For each pair, we format a line as `"%2d *%2d = %2d\n"` – note the single space after `*` and before `=`, and no leading space before `*` (so it becomes `" *"` after the first number formatted with `%2d`). After the inner loop completes for a given `dan`, we append a newline `"\n"` to create a blank line. This means for each multiplier block, there are 9 lines followed by an extra newline, resulting in 10 newlines per block, and 90 lines total. We can use `snprintf` or `std::ostringstream` with `std::setw` and `std::right` to format two-character-width numbers. Simpler: use `std::ostringstream` and manually pad with spaces if needed, but using `snprintf` into a temporary buffer is straightforward and matches the exact `printf` behavior. Build the string by appending each formatted line. Edge cases: numbers exactly 1–9 are always two digits when right-aligned; no overflow concerns. Time complexity is \(O(81)\) = O(1) constant, space complexity is \(O(\text{output size})\) which is a constant ~ 81 lines × ~15 bytes ≈ 1.2 KB, also effectively O(1).

#include <string>
#include <cstdio>

// Returns the full 9x9 multiplication table as a string, formatted exactly
// like the original printf loop: each line "%2d *%2d = %2d\n", with an
// extra newline after each row block.
std::string printMultiplicationTable() {
    std::string result;
    char buffer[32]; // Enough for a single formatted line

    for (int dan = 1; dan < 10; ++dan) {
        for (int su = 1; su < 10; ++su) {
            int len = std::snprintf(buffer, sizeof(buffer), "%2d *%2d = %2d\n", dan, su, dan * su);
            result.append(buffer, static_cast<size_t>(len));
        }
        result.push_back('\n'); // blank line after each row (including last)
    }
    return result;
}

#include <cassert>
#include <string>

// The function under test (include the solution above or link it)
std::string printMultiplicationTable();

int main() {
    std::string table = printMultiplicationTable();
    
    // Check the first line of the table
    assert(table.compare(0, 19, " 1 * 1 =  1\n") == 0);
    
    // Check the end of the first row block (after 1*9, then newline, then blank)
    // Position of " 1 * 9 =  9\n" in the first block: 9 lines * 19 chars = 171, then extra newline at 171
    assert(table.compare(152, 19, " 1 * 9 =  9\n") == 0);
    assert(table[171] == '\n'); // blank line after first row
    
    // Check a middle row: 5*5
    // Row 5 starts at (dan=5, so 4 previous blocks * (9*19 + 1) = 4*172 = 688)
    // Inside row 5, line 5 (su=5) is at offset 688 + 4*19 + 0 = 764
    assert(table.compare(764, 19, " 5 * 5 = 25\n") == 0);
    
    // Check the last line of the table (9*9)
    // Total length: 9 blocks * 172 = 1548
    assert(table.size() == 1548);
    assert(table.compare(1529, 19, " 9 * 9 = 81\n") == 0);
    
    // Check that the final character is a newline (blank line after last row)
    assert(table.back() == '\n');
}
