Write a standalone C++ function `extractAndPrintSortedNumbers(int n, const std::vector<std::string>& lines)` that processes `n` strings, where each string contains non-negative integers possibly separated by arbitrary non-digit characters (e.g., letters, spaces, punctuation). The function must extract every integer (a contiguous sequence of digit characters) from these strings, collect all numbers across all strings, sort them in non-decreasing order, and return a `std::vector<int>` containing these sorted numbers. Input strings may be empty or contain no digits; duplicate numbers must appear multiple times in the output. The function must be const-correct and handle arbitrarily large integer values (up to the limit of `int`). Your implementation should not use any global state or external priority queues; all processing must be local to the function.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single string with mixed separators
    std::vector<std::string> t1 = {"abc123def456ghi"};
    assert(extractAndPrintSortedNumbers(1, t1) == std::vector<int>({123, 456}));

    // Multiple strings, duplicates, leading zeros
    std::vector<std::string> t2 = {"12x34", "x1y2", "003", "42"};
    assert(extractAndPrintSortedNumbers(4, t2) == std::vector<int>({1, 2, 3, 12, 34, 42}));

    // Empty strings and no digits
    std::vector<std::string> t3 = {"", "no digits here", "!!!", "   "};
    assert(extractAndPrintSortedNumbers(4, t3).empty());

    // Large numbers within int range
    std::vector<std::string> t4 = {"2147483647", "0", "999999999"};
    assert(extractAndPrintSortedNumbers(3, t4) == std::vector<int>({0, 999999999, 2147483647}));

    // All numbers already sorted descending, ensure sort works
    std::vector<std::string> t5 = {"5", "4", "3", "2", "1"};
    assert(extractAndPrintSortedNumbers(5, t5) == std::vector<int>({1, 2, 3, 4, 5}));

    // Consecutive digits and mixed with punctuation
    std::vector<std::string> t6 = {"a10b20c30", "40,50;60"};
    assert(extractAndPrintSortedNumbers(2, t6) == std::vector<int>({10, 20, 30, 40, 50, 60}));
}
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// Extract all non-negative integers from each string (sequences of digits),
// collect them across all strings, sort them in ascending order, and return.
std::vector<int> extractAndPrintSortedNumbers(int n, const std::vector<std::string>& lines) {
    std::vector<int> numbers;
    numbers.reserve(1000); // heuristic, reallocation is fine

    for (int i = 0; i < n; ++i) {
        const std::string& str = lines[i];
        int pos = 0;
        while (pos < static_cast<int>(str.size())) {
            // Skip non-digits
            while (pos < static_cast<int>(str.size()) && !std::isdigit(static_cast<unsigned char>(str[pos]))) {
                ++pos;
            }
            // Extract a single integer
            int value = 0;
            bool found = false;
            while (pos < static_cast<int>(str.size()) && std::isdigit(static_cast<unsigned char>(str[pos]))) {
                value = value * 10 + (str[pos] - '0');
                ++pos;
                found = true;
            }
            if (found) {
                numbers.push_back(value);
            }
        }
    }

    std::sort(numbers.begin(), numbers.end());
    return numbers;
}
// The solution iterates over each string in the input vector, scanning character by character. For each character, if it is a digit, we accumulate consecutive digits into a running integer value, resetting it whenever a non-digit or the end of the string is reached. After finishing each string, all extracted numbers are collected into a temporary list. After processing all strings, we sort that list using `std::sort` (which is efficient and stable for `int`). Edge cases include empty strings, strings with no digits, leading/trailing non-digit characters, multiple consecutive non-digits, and large numbers that fit in `int` (we assume input won't overflow `int`; if `long long` were needed, the type could be adjusted). Time complexity is \(O(N \cdot M + K \log K)\), where \(N\) is the number of strings, \(M\) is the average length of a string, and \(K\) is the total number of extracted integers. Space complexity is \(O(K)\) for storing the extracted numbers plus \(O(1)\) auxiliary for scanning.
