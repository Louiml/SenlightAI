Write a C++ function that, given a positive integer `n`, returns a `std::vector<std::string>` where each string represents a row of the following pattern: for row `i` (1-indexed), the row contains `i` numbers, and the `j`-th number (1-indexed) in that row is the sum `1 + 2 + ... + j` (i.e., the triangular number `T(j)`). Each row's numbers must be separated by a single space, and there should be no trailing space at the end of any row. For example, if `n = 5`, the output rows should be:
```
1
1 3
1 3 6
1 3 6 10
1 3 6 10 15
```
The function must handle `n = 0` by returning an empty vector. Assume `n` is non-negative. Use appropriate `const` correctness and avoid any global state.

// The solution builds the result row by row. For a given row `i`, we compute a running sum starting from 0 and for each column `j` from 1 to `i`, we add `j` to that running sum and append the new total to the row’s string. This mirrors the original snippet’s inner loop but instead of printing, we accumulate into a string. Edge cases: `n = 0` should yield an empty vector; each row must not have trailing spaces (so we add spaces only between numbers). Time complexity is \(O(n^2)\) because the total number of numbers printed is \(\sum_{i=1}^n i = n(n+1)/2\), and each number is constructed in constant time (using `std::to_string`). Space complexity is \(O(n^2)\) for the output strings, plus \(O(n)\) for the temporary row string.

#include <vector>
#include <string>

// Builds a vector of strings, where row i (1-indexed) contains triangular numbers
// T(1), T(2), ..., T(i), space-separated. Returns empty vector if n == 0.
std::vector<std::string> buildTriangularRows(int n) {
    std::vector<std::string> rows;
    rows.reserve(n > 0 ? n : 0);

    for (int i = 1; i <= n; ++i) {
        std::string row;
        int sum = 0;
        for (int j = 1; j <= i; ++j) {
            sum += j;
            if (j > 1) {
                row += ' ';
            }
            row += std::to_string(sum);
        }
        rows.push_back(row);
    }

    return rows;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // n = 0 -> empty vector
    assert(buildTriangularRows(0).empty());

    // n = 1 -> single row "1"
    auto r1 = buildTriangularRows(1);
    assert(r1.size() == 1);
    assert(r1[0] == "1");

    // n = 2 -> rows "1" and "1 3"
    auto r2 = buildTriangularRows(2);
    assert(r2.size() == 2);
    assert(r2[0] == "1");
    assert(r2[1] == "1 3");

    // n = 3 -> rows "1", "1 3", "1 3 6"
    auto r3 = buildTriangularRows(3);
    assert(r3.size() == 3);
    assert(r3[0] == "1");
    assert(r3[1] == "1 3");
    assert(r3[2] == "1 3 6");

    // n = 5 -> full example, check final row
    auto r5 = buildTriangularRows(5);
    assert(r5.size() == 5);
    assert(r5[0] == "1");
    assert(r5[1] == "1 3");
    assert(r5[2] == "1 3 6");
    assert(r5[3] == "1 3 6 10");
    assert(r5[4] == "1 3 6 10 15");

    // Ensure no trailing spaces in any row (we can check last character is not space)
    for (const auto& row : r5) {
        assert(!row.empty());
        assert(row.back() != ' ');
    }
}
