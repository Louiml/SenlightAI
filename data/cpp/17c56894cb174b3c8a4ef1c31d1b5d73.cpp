/*
Write a C++ function that, given a positive integer `rows` and a positive integer `cols`, returns a string containing a rectangular "star grid" pattern with exactly `rows` lines. Each line must start with the 0-based line index (i.e., `0`, `1`, `2`, ...), followed by `" --> "`, then exactly `cols` repetitions of `" * "` (including the spaces between stars and after the last star), and end with a newline character `'\n'`. For example, with `rows=3` and `cols=2`, the returned string should be exactly `"0 -->  *  * \n1 -->  *  * \n2 -->  *  * \n"`. The function must not print anything to the console; it must return the full string.
*/
#include <string>

// Builds a rectangular star grid as a string. For each row i (0-based),
// appends "i --> " followed by 'cols' copies of " * " and a newline.
std::string buildStarGrid(int rows, int cols) {
    std::string result;
    result.reserve(static_cast<size_t>(rows) * (cols * 3 + 8)); // rough reservation
    for (int i = 0; i < rows; ++i) {
        result += std::to_string(i);
        result += " --> ";
        for (int j = 0; j < cols; ++j) {
            result += " * ";
        }
        result += '\n';
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the solution function (normally in a header)
std::string buildStarGrid(int rows, int cols);

int main() {
    // Test with rows=3, cols=2 matching the example
    assert(buildStarGrid(3, 2) == "0 -->  *  * \n1 -->  *  * \n2 -->  *  * \n");

    // Test single row
    assert(buildStarGrid(1, 1) == "0 -->  * \n");

    // Test multiple rows, single column
    assert(buildStarGrid(2, 1) == "0 -->  * \n1 -->  * \n");

    // Test larger numbers
    assert(buildStarGrid(2, 4) == "0 -->  *  *  *  * \n1 -->  *  *  *  * \n");

    // Test with rows=4, cols=3 to check indexing continues correctly
    assert(buildStarGrid(4, 3) ==
           "0 -->  *  *  * \n"
           "1 -->  *  *  * \n"
           "2 -->  *  *  * \n"
           "3 -->  *  *  * \n");

    return 0;
}
// The solution builds the output string step by step using a loop over each row index from 0 to `rows-1`. For each row, append the row index as a decimal number using `std::to_string`, then append the literal `" --> "`, then loop `cols` times to append `" * "` (note the leading space, asterisk, and trailing space, so the total pattern per star is 3 characters). Finally, append a newline `'\n'` after each row. Edge cases: `rows` and `cols` are positive, so no need to handle zero or negative values; however, if `rows` or `cols` is 0, the function would return an empty string (but per problem statement, assume positive inputs). Time complexity is \(O(rows \times cols)\) because we append a fixed-length pattern `cols` times for each row. Space complexity is \(O(rows \times cols)\) for the output string, plus negligible auxiliary space.
