Write a C++ function that takes a positive integer `n` and a positive integer `limit` as parameters, and returns a string containing the multiplication table of `n` from 1 through `limit`, formatted as `"n x i = result"` on each line, with lines separated by newline characters. If `limit` is 0 or negative, return an empty string. The function must not print anything to standard output; it must only return the constructed string. The function signature should be `std::string multiplicationTable(int n, int limit)`, and you must apply `const` correctness where appropriate (e.g., parameters passed by value are fine, but mark them const inside the function).

The solution constructs a string by iterating from 1 to `limit` (inclusive), appending each formatted line using `std::to_string` for the integers and concatenation with `+=`. We check early for `limit <= 0` and return an empty string to handle invalid inputs. The main edge case is when `limit` is 0 or negative, where no table is produced. For a positive `limit`, there are exactly `limit` lines, each of constant length (assuming `n` and `i` fit within typical integer ranges), so the time complexity is O(limit) because we loop `limit` times and each append operation is amortized O(1). The auxiliary space is O(limit * line_length) due to the returned string size; no other significant memory is used. We do not use `std::endl` to avoid unnecessary flushing; newline characters are explicitly added.

#include <string>

// Return a formatted multiplication table for n from 1 to limit as a string.
// If limit <= 0, return an empty string.
std::string multiplicationTable(const int n, const int limit) {
    if (limit <= 0) {
        return std::string();
    }
    
    std::string table;
    for (int i = 1; i <= limit; ++i) {
        table += std::to_string(n) + " * " + std::to_string(i) + " = " + std::to_string(n * i);
        if (i != limit) {
            table += '\n';
        }
    }
    return table;
}

#include <cassert>
#include <string>

// The solution function (declared here for testing; in practice, include the header)
std::string multiplicationTable(const int n, const int limit);

int main() {
    // Test basic multiplication table for n=4, limit=10
    std::string expected = "4 * 1 = 4\n4 * 2 = 8\n4 * 3 = 12\n4 * 4 = 16\n4 * 5 = 20\n4 * 6 = 24\n4 * 7 = 28\n4 * 8 = 32\n4 * 9 = 36\n4 * 10 = 40";
    assert(multiplicationTable(4, 10) == expected);

    // Test limit = 1
    assert(multiplicationTable(7, 1) == "7 * 1 = 7");

    // Test limit = 0 (should return empty string)
    assert(multiplicationTable(3, 0) == "");

    // Test negative limit (should return empty string)
    assert(multiplicationTable(2, -5) == "");

    // Test n=0 (works fine with positive limit)
    assert(multiplicationTable(0, 3) == "0 * 1 = 0\n0 * 2 = 0\n0 * 3 = 0");

    // Test n=1 and limit=5
    assert(multiplicationTable(1, 5) == "1 * 1 = 1\n1 * 2 = 2\n1 * 3 = 3\n1 * 4 = 4\n1 * 5 = 5");

    // Test larger limit to ensure correctness of loop termination
    assert(multiplicationTable(9, 2) == "9 * 1 = 9\n9 * 2 = 18");

    // Test that there is no trailing newline
    std::string small = multiplicationTable(2, 1);
    assert(small == "2 * 1 = 2");
    assert(small.empty() || small.back() != '\n');

    // Test negative n (still works)
    assert(multiplicationTable(-3, 2) == "-3 * 1 = -3\n-3 * 2 = -6");
    return 0;
}
