// Write a C++ function `printDivisorsSorted(int64_t n)` that takes a positive integer `n` and returns a `std::string` containing all positive divisors of `n` in ascending order, separated by spaces, with exactly 10 divisors per line (the last line may have fewer). The output string should end with a newline character. If `n` has exactly one divisor, the string should contain just that divisor followed by a newline. The function must handle large values of `n` (up to \(10^{18}\)) using 64-bit integers, and it must not print anything to standard output itself; instead, it builds and returns the formatted string.
#include <cassert>
#include <string>

// The solution function is declared above; this main tests it.
int main() {
    // n = 1: only divisor 1, single line with newline
    assert(printDivisorsSorted(1) == "1\n");

    // n = 6: divisors 1 2 3 6 (one line, less than 10)
    assert(printDivisorsSorted(6) == "1 2 3 6\n");

    // n = 12: divisors 1 2 3 4 6 12
    assert(printDivisorsSorted(12) == "1 2 3 4 6 12\n");

    // n = 16 (perfect square): divisors 1 2 4 8 16 (no duplicates)
    assert(printDivisorsSorted(16) == "1 2 4 8 16\n");

    // n = 100: divisors 1 2 4 5 10 20 25 50 100
    assert(printDivisorsSorted(100) == "1 2 4 5 10 20 25 50 100\n");

    // n = 30: divisors 1 2 3 5 6 10 15 30
    assert(printDivisorsSorted(30) == "1 2 3 5 6 10 15 30\n");

    // n = 36: divisors 1 2 3 4 6 9 12 18 36
    assert(printDivisorsSorted(36) == "1 2 3 4 6 9 12 18 36\n");

    // n = 120: divisors 1 2 3 4 5 6 8 10 12 15 20 24 30 40 60 120
    // Here exactly 10 divisors on first line, then 6 on second.
    assert(printDivisorsSorted(120) == "1 2 3 4 5 6 8 10 12 15\n20 24 30 40 60 120\n");

    // n = 1e18 (a large perfect square): check it contains 1 and n, and size
    int64_t large = 1000000000000000000LL; // 10^18
    std::string largeResult = printDivisorsSorted(large);
    // Should start with "1 " and end with "1000000000000000000\n"
    assert(largeResult.find("1 ") == 0);
    assert(largeResult.substr(largeResult.size() - 22) == "1000000000000000000\n");
}
#include <string>
#include <set>
#include <cstdint>

// Return a string containing all positive divisors of n in ascending order,
// with up to 10 divisors per line, separated by spaces, ending with a newline.
std::string printDivisorsSorted(int64_t n) {
    std::set<int64_t> divisors;
    // Find all divisor pairs up to sqrt(n)
    for (int64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divisors.insert(i);
            divisors.insert(n / i);
        }
    }

    std::string result;
    result.reserve(divisors.size() * 20); // rough estimate for 64-bit numbers
    int count = 0;
    for (auto it = divisors.begin(); it != divisors.end(); ++it) {
        result += std::to_string(*it);
        ++count;
        if (count % 10 == 0) {
            result += '\n';
        } else if (it != std::prev(divisors.end())) {
            result += ' ';
        }
    }
    // Ensure the string ends with a newline if the last line wasn't complete
    if (divisors.size() % 10 != 0) {
        result += '\n';
    }
    return result;
}
// The core task is to find all divisors of `n` efficiently and format them. Instead of iterating from 1 to `n` (which is infeasible for large n), we only iterate `i` from 1 to \(\sqrt{n}\). For each `i` that divides `n`, we insert both `i` and `n/i` into a `std::set<int64_t>`. Using a set automatically sorts the divisors in ascending order and removes duplicates (when `i == n/i`, e.g., for a perfect square). After collecting all divisors, we iterate through the set and build a string, adding a space between numbers and inserting a line break every 10 numbers. The last line does not require a trailing space but must end with a newline character. Edge cases include `n = 1`, where only divisor 1 exists, and perfect squares where `i` and `n/i` are equal, but the set handles duplication. The time complexity is \(O(\sqrt{n})\) for finding divisors (due to the loop up to √n) plus \(O(d \log d)\) for set insertion and traversal, where `d` is the number of divisors. For a 64-bit integer, the maximum number of divisors is about 103680 (for highly composite numbers near \(10^{18}\)), but in practice it is much smaller. Overall, the algorithm runs in \(O(\sqrt{n} + d \log d)\) time and uses \(O(d)\) space for the set.
