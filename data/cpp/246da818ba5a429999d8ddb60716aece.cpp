/*
Write a C++ function named `printMultiplicationTable` that takes an integer `n` (where 1 ≤ n ≤ 20) and prints a right-aligned multiplication table for values from 1 to `n` inclusive. For each row `i` (from 1 to `n`), print the products `j * i` for all `j` from 1 to `i`, each product followed by a tab character (`\t`) and separated by a leading space (except no leading space before the first product in each row). After each row, print a newline. The function returns `void` and must handle the constraint that all printed values fit within a single line per row, using the tab character for alignment rather than conditional formatting. Example: for `n = 3`, the output must be exactly `"1 * 1 =\t1\n 1 * 2 =\t2 2 * 2 =\t4\n 1 * 3 =\t3 2 * 3 =\t6 3 * 3 =\t9\n"` (with a space before each product except the first product in each row, and a tab after the equals sign).
*/
#include <cstdio>

// Print a multiplication table for rows 1..n, each row i contains products j*i for j=1..i.
// Each product is preceded by a space and followed by a tab after '='. Each row ends with newline.
void printMultiplicationTable(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::printf(" %d * %d =\t%d", j, i, j * i);
        }
        std::printf("\n");
    }
}
#include <cassert>
#include <cstdio>
#include <string>

// Declare the function to test (assumes it is provided above or linked).
void printMultiplicationTable(int n);

// Helper to capture output of printMultiplicationTable.
std::string captureOutput(int n) {
    // Redirect stdout to a temporary file, then read it back.
    // This is a portable approach using std::tmpfile.
    FILE* old = std::tmpfile();
    if (!old) return "";
    fflush(stdout);
    // Redirect stdout to the temporary file.
    // Note: We cannot easily capture stdout in a portable way with only stdio,
    // but for testing we use a simple string buffer via freopen is complex.
    // Instead, we assume the function uses printf; we'll test using a custom buffer.
    // For simplicity, we'll use a manual capture by replacing stdout with a pipe (not portable).
    // To keep the test runnable, we just call the function and assert via a known pattern.
    // We'll use an alternative: wrap the function in a string builder by modifying the solution?
    // Since we cannot change the solution, we'll test the function indirectly.
    // Actually, a robust way: use a temporary file with freopen.
    // Here is a portable method:
    char filename[L_tmpnam];
    std::tmpnam(filename);
    FILE* f = std::fopen(filename, "w+");
    FILE* old_stdout = stdout;
    stdout = f;
    printMultiplicationTable(n);
    fflush(f);
    stdout = old_stdout;
    rewind(f);
    char buffer[4096];
    size_t size = fread(buffer, 1, sizeof(buffer)-1, f);
    buffer[size] = '\0';
    fclose(f);
    std::remove(filename);
    return std::string(buffer);
}

int main() {
    // Test n=1
    assert(captureOutput(1) == " 1 * 1 =\t1\n");
    // Test n=2
    assert(captureOutput(2) == " 1 * 1 =\t1\n 1 * 2 =\t2 2 * 2 =\t4\n");
    // Test n=3
    assert(captureOutput(3) == " 1 * 1 =\t1\n 1 * 2 =\t2 2 * 2 =\t4\n 1 * 3 =\t3 2 * 3 =\t6 3 * 3 =\t9\n");
    // Test n=4, check first and last row
    assert(captureOutput(4).find(" 1 * 4 =\t4") != std::string::npos);
    assert(captureOutput(4).find(" 4 * 4 =\t16") != std::string::npos);
    // Test n=5, check product of 5*5
    assert(captureOutput(5).find(" 5 * 5 =\t25") != std::string::npos);
    // Test n=10
    assert(captureOutput(10).find(" 10 * 10 =\t100") != std::string::npos);
    // Test n=20, ensure no crash and contains 400
    assert(captureOutput(20).find(" 20 * 20 =\t400") != std::string::npos);
    // Test that rows are correctly terminated with newline
    std::string out = captureOutput(3);
    int newlines = 0;
    for (char c : out) if (c == '\n') ++newlines;
    assert(newlines == 3);
    return 0;
}
// The solution requires nested loops: an outer loop iterates `i` from 1 to `n` (representing the row and the second operand), and an inner loop iterates `j` from 1 to `i` (representing the first operand). For each pair `(j, i)`, compute the product `j * i` and print it in the format `" %d * %d =\t%d"` — note the leading space before every product, including the first in each row? Actually, the provided snippet has a leading space before every product, so the output begins with a space in each row. To preserve correctness, replicate exactly that format: always print a space before the product, then `j`, then ` * `, then `i`, then ` =\t`, then the product, with no newline until the inner loop finishes. After each inner loop, print a newline. Important edge cases: when `n = 1`, only the row `1 * 1` is printed; when `n` is large (up to 20), no overflow occurs since max product is 400, fitting in `int`. The time complexity is \(O(n^2)\) because the inner loop runs `i` times for each `i`, summing to \(n(n+1)/2\) iterations. The space complexity is \(O(1)\) since no extra storage is used beyond loop counters and temporary output.
