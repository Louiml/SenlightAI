Write a standalone C++ function `countBitwiseTriples()` that counts and prints the number of ordered quadruples `(a, b, c, d)` where each value is an integer from `0` to `3` inclusive, such that the bitwise AND of all four numbers `(a & b & c & d)` is greater than or equal to the bitwise XOR of all four numbers `(a ^ b ^ c ^ d)`. The function should output each qualifying quadruple on a separate line in the format `a b c d`, and after all quadruples are printed, output the total count on its own line. The function should return nothing (void). The input is fixed—there is no user input; the function iterates over the full search space. Ensure the code is self-contained and can be compiled as a standalone program.
The solution approach is straightforward: use four nested loops to iterate over all possible values of `a`, `b`, `c`, and `d`, each ranging from `0` to `3`. For each combination, compute `x = a & b & c & d` and `y = a ^ b ^ c ^ d` (where `^` is bitwise XOR). If `x >= y`, print the quadruple and increment a counter. After all loops finish, print the counter. The number of quadruples is `4^4 = 256`, so the time complexity is `O(4^4) = O(256)`, which is constant—trivially fast. Space complexity is `O(1)` apart from the output stream. There are no special edge cases because the limits are fixed small integers; the comparison works directly on integer values. The function must print exactly as specified, with spaces between numbers and a space after the last number before the newline (as in the original snippet), but for a clean output, we can print `a << ' ' << b << ' ' << c << ' ' << d << '\n'`. The count should be printed after the loop with a trailing newline.
#include <iostream>

// Count and print ordered quadruples (a,b,c,d) with 0<=each<=3
// such that (a & b & c & d) >= (a ^ b ^ c ^ d).
void countBitwiseTriples() {
    int ans = 0;
    for (int a = 0; a < 4; ++a) {
        for (int b = 0; b < 4; ++b) {
            for (int c = 0; c < 4; ++c) {
                for (int d = 0; d < 4; ++d) {
                    const int x = a & b & c & d;
                    const int y = a ^ b ^ c ^ d;
                    if (x >= y) {
                        std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
                        ++ans;
                    }
                }
            }
        }
    }
    std::cout << ans << '\n';
}
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// Declare the function to test (normally in a header or before main)
void countBitwiseTriples();

// We capture output by redirecting cout to a stringstream inside the test.
int main() {
    // Redirect cout to a stringstream to capture output.
    std::ostringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    countBitwiseTriples();

    // Restore cout.
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();

    // Count the number of printed quadruples (each line has 4 numbers + newline).
    std::istringstream input(output);
    std::string line;
    int quadruple_count = 0;
    while (std::getline(input, line)) {
        if (!line.empty()) {
            ++quadruple_count;
        }
    }

    // The last line is the total count, so quadruple lines = total lines - 1.
    int total_lines = 0;
    std::istringstream count_lines(output);
    std::string dummy;
    while (std::getline(count_lines, dummy)) {
        ++total_lines;
    }
    int quad_lines = total_lines - 1;

    // Extract the final count from the last line.
    std::istringstream last_line(output);
    std::string token;
    int final_count = -1;
    while (std::getline(last_line, token)) {
        if (!token.empty()) {
            final_count = std::stoi(token);
        }
    }

    // Assert that the number of quadruple lines equals the printed count.
    assert(quad_lines == final_count);

    // Assert that the count is correct by brute-force recomputation.
    int expected_count = 0;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d)
                    if ((a & b & c & d) >= (a ^ b ^ c ^ d))
                        ++expected_count;
    assert(final_count == expected_count);

    // Also verify that the output contains exactly the expected quadruples.
    // For simplicity, we check that the first and last lines are as expected.
    std::istringstream first_last(output);
    std::string first_line, last_line_str;
    std::vector<std::string> all_lines;
    std::string cur;
    while (std::getline(first_last, cur)) {
        all_lines.push_back(cur);
    }
    assert(all_lines.front() == "0 0 0 0");
    assert(all_lines.back() == std::to_string(expected_count));

    // If any assertion fails, the program aborts with an error message.
    // If all pass, print a success message.
    std::cout << "All test assertions passed.\n";
    return 0;
}
