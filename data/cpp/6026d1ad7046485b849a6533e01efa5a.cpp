Write a C++ function that takes a positive integer `n` and returns a `std::string` representing a triangular multiplication table. The table has `n` rows. For row `i` (1-indexed from 1 to n), the row contains exactly `n - i + 1` entries. Each entry is formatted as `"i * j = i*j"` where `j` runs from `1` to `n - i + 1`. Entries within a row are separated by the literal string `" / "` (space, slash, space). After the last entry of each row, a newline character `'\n'` is appended. For example, for `n = 3`, the string should be exactly: `"1 * 1 = 1 / 1 * 2 = 2 / 1 * 3 = 3\n2 * 1 = 2 / 2 * 2 = 4\n3 * 1 = 3\n"`. The function must be named `generateMultiplicationTable`, accept the parameter as `int n`, and return the result as `std::string`. Assume `n ≥ 1` (no need to handle non-positive input). Ensure the output uses `std::to_string` for numeric conversion to avoid formatting issues.

// The problem is a straightforward nested-loop generation of a string. The outer loop iterates `i` from 1 to `n` inclusive. For each row, the inner loop iterates `j` from 1 to `n - i + 1` inclusive. For each pair, we build a substring `"i * j = (i*j)"` using `std::to_string` for each integer conversion (i, j, and product). After each substring except the last in a row, append `" / "`. After the final entry of a row (when `j == n - i + 1`), append `'\n'`. This exactly mirrors the given snippet's output structure but captures it into a string instead of printing. The main edge case is `n = 1`, which produces a single row `"1 * 1 = 1\n"` — the loop conditions must handle this correctly (inner loop runs exactly once). No other special cases exist because input is guaranteed positive. Time complexity is O(n²) because the total number of entries is the sum from i=1 to n of (n-i+1) = n(n+1)/2. Space complexity is O(n²) as well because the resulting string size is proportional to the number of entries (each entry has a few dozen characters, plus separators). The solution uses only a stringstream-free approach, accumulating into a `std::string` with `+=` for efficiency.

#include <string>

// Generates a triangular multiplication table as a string.
// For each row i (1..n), prints j from 1 to n-i+1, separated by " / ".
// Rows are separated by a newline character.
std::string generateMultiplicationTable(int n) {
    std::string result;
    for (int i = 1; i <= n; ++i) {
        int limit = n - i + 1;
        for (int j = 1; j <= limit; ++j) {
            result += std::to_string(i);
            result += " * ";
            result += std::to_string(j);
            result += " = ";
            result += std::to_string(i * j);
            if (j == limit) {
                result += '\n';
            } else {
                result += " / ";
            }
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function under test.
std::string generateMultiplicationTable(int n);

int main() {
    // n = 1: single row, single entry.
    assert(generateMultiplicationTable(1) == "1 * 1 = 1\n");

    // n = 2: two rows, second row has one entry.
    assert(generateMultiplicationTable(2) == "1 * 1 = 1 / 1 * 2 = 2\n2 * 1 = 2\n");

    // n = 3: example from the problem statement.
    assert(generateMultiplicationTable(3) == "1 * 1 = 1 / 1 * 2 = 2 / 1 * 3 = 3\n2 * 1 = 2 / 2 * 2 = 4\n3 * 1 = 3\n");

    // n = 4: check last row and number of rows.
    std::string table4 = generateMultiplicationTable(4);
    assert(table4.rfind("4 * 1 = 4\n") == table4.length() - 9); // last row is "4 * 1 = 4\n"
    assert(table4.find("1 * 1 = 1 / 1 * 2 = 2 / 1 * 3 = 3 / 1 * 4 = 4\n") == 0);

    // n = 5: check product and row lengths by counting newlines.
    std::string table5 = generateMultiplicationTable(5);
    int newline_count = 0;
    for (char c : table5) {
        if (c == '\n') ++newline_count;
    }
    assert(newline_count == 5); // 5 rows

    // Verify that total entries count matches n(n+1)/2.
    int entry_count = 0;
    for (size_t pos = table5.find(" = "); pos != std::string::npos; pos = table5.find(" = ", pos + 1)) {
        ++entry_count;
    }
    assert(entry_count == 15); // 5*6/2

    // Additional spot check: for n=5, row 2 should contain "2 * 4 = 8".
    assert(table5.find("2 * 4 = 8") != std::string::npos);

    // Edge: ensure no leading/trailing spaces except separators.
    assert(table5[0] == '1');
    assert(table5.back() == '\n');
}
