/*
Write a C++ function named `analyzeInput` that reads integers from standard input (via `cin`) until a `0` is entered, and returns a `std::pair<double, double>` where the first element is the average (as a `double`) of all odd integers entered (excluding the terminating 0), and the second element is the average (as a `double`) of all integers in the inclusive range [7, 31] (excluding the terminating 0). The input guarantees at least one odd integer and at least one integer in [7, 31] will be entered before the terminating 0. The function must handle all integer inputs, including negative numbers and zeros (except the sentinel 0, which stops reading). It should not print anything; only compute and return the two averages.
*/

#include <iostream>
#include <utility>

// Reads integers from std::cin until 0 is entered.
// Returns a pair (average of odd inputs, average of inputs in [7,31]).
// Assumes at least one odd and one in-range integer are present before the 0.
std::pair<double, double> analyzeInput() {
    int num;
    int countOdd = 0;
    long long sumOdd = 0;
    int countRange = 0;
    long long sumRange = 0;

    while (std::cin >> num && num != 0) {
        // Check for odd number (using modulo, negative odd also true)
        if (num % 2 != 0) {
            countOdd++;
            sumOdd += num;
        }
        // Check for inclusive range [7, 31]
        if (num >= 7 && num <= 31) {
            countRange++;
            sumRange += num;
        }
    }

    // Guaranteed non-zero counts by problem statement
    double avgOdd = static_cast<double>(sumOdd) / countOdd;
    double avgRange = static_cast<double>(sumRange) / countRange;
    return {avgOdd, avgRange};
}

#include <cassert>
#include <iostream>
#include <sstream>
#include <utility>

// Declare the function from the solution (or assume it's included above)
std::pair<double, double> analyzeInput();

int main() {
    // Test 1: Simple odd and range numbers
    {
        std::istringstream input("3 5 7 9 11 0");
        std::streambuf* orig = std::cin.rdbuf(input.rdbuf());
        auto result = analyzeInput();
        std::cin.rdbuf(orig);
        assert(result.first == (3 + 5 + 7 + 9 + 11) / 5.0); // 7.0
        assert(result.second == (7 + 9 + 11) / 3.0);        // 9.0
    }

    // Test 2: Negative odd numbers and edge range values
    {
        std::istringstream input("-5 7 31 0");
        std::streambuf* orig = std::cin.rdbuf(input.rdbuf());
        auto result = analyzeInput();
        std::cin.rdbuf(orig);
        assert(result.first == (-5 + 7 + 31) / 3.0);       // 11.0
        assert(result.second == (7 + 31) / 2.0);            // 19.0
    }

    // Test 3: Only one odd and one range number
    {
        std::istringstream input("7 0");
        std::streambuf* orig = std::cin.rdbuf(input.rdbuf());
        auto result = analyzeInput();
        std::cin.rdbuf(orig);
        assert(result.first == 7.0);
        assert(result.second == 7.0);
    }

    // Test 4: Multiple zeros before sentinel (they are ignored because loop stops at first 0)
    {
        std::istringstream input("9 0 5 0");
        std::streambuf* orig = std::cin.rdbuf(input.rdbuf());
        auto result = analyzeInput();
        std::cin.rdbuf(orig);
        assert(result.first == 9.0);
        assert(result.second == 9.0);
    }

    // Test 5: Large numbers and many entries, ensure no overflow (use long long)
    {
        std::istringstream input("1000000000 1000000000 7 0");
        std::streambuf* orig = std::cin.rdbuf(input.rdbuf());
        auto result = analyzeInput();
        std::cin.rdbuf(orig);
        assert(result.first == 500000000.5); // (1e9+1e9+7)/3
        assert(result.second == (7.0 + 1e9 + 1e9) / 3.0);
    }

    std::cout << "All tests passed!\n";
    return 0;
}

// The solution uses a loop that repeatedly reads integers until the sentinel `0` is encountered. For each non-zero input, we check two independent conditions: (a) if the number is odd (`num % 2 != 0`), update a running count and sum for odd numbers; (b) if the number is between 7 and 31 inclusive, update a running count and sum for that range. The sentinel `0` is not processed for either condition because we test it after reading and break the loop (or check `num != 0` before processing). Edge cases: negative odd numbers are still odd (e.g., -3 % 2 == -1, which is non-zero, so we treat any remainder not equal to 0 as odd); the range check uses `<= 31 && >= 7` to include boundaries. Since the problem guarantees at least one valid odd and at least one valid in-range number, division by zero is avoided. The averages are computed as `double` by casting the sums to `double` before division. Time complexity is O(n) where n is the number of integers read until the sentinel; space complexity is O(1) as we only store four running totals and the input variable.
