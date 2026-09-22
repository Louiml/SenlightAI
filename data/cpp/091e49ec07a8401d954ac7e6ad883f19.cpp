Write a C++ function `analyzeThreeValues` that takes three `int` parameters and returns a `std::string` summarizing four properties in a fixed format: the average (as a floating-point value rounded to two decimal places), the minimum, the maximum, and whether a user-provided search value exists among the three inputs. The function must read the search value from standard input (using `cin`). The returned string must follow exactly: `"Avg: X.XX; Min: A; Max: B; Found: YES/NO"`, where X, A, B are the actual numbers, and YES/NO indicates if the search value appears among the three inputs. The function must handle duplicates correctly (e.g., if two inputs equal the minimum, still report the correct value). The inputs may be negative, positive, or zero. Do not modify the input parameters; treat them as read-only.
The solution computes the arithmetic mean as a `double` by summing the three integers and dividing by 3.0, then formats it to two decimal places using `std::fixed` and `std::setprecision(2)`. For the minimum and maximum, initialize both from the first value, then compare with the other two values sequentially; duplicates are handled naturally because comparisons use `<=` and `>=` thresholds where necessary (though simple comparisons suffice). For the search, read an `int` from standard input, then check equality against each of the three input values; if any matches, set a boolean flag. The final string is built using `std::ostringstream` to ensure proper formatting. Edge cases: all equal values, negative numbers, and large values (within `int` range). Time complexity is O(1) – constant time operations regardless of input; space complexity is O(1) for the variables, plus the returned string length O(1).
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

// Returns a formatted summary: average (2 decimal places), min, max, and whether a user-entered value is present.
std::string analyzeThreeValues(int a, int b, int c) {
    double average = (a + b + c) / 3.0;

    int minVal = a;
    if (b < minVal) minVal = b;
    if (c < minVal) minVal = c;

    int maxVal = a;
    if (b > maxVal) maxVal = b;
    if (c > maxVal) maxVal = c;

    int searchVal;
    std::cin >> searchVal;

    bool found = (a == searchVal || b == searchVal || c == searchVal);

    std::ostringstream output;
    output << "Avg: " << std::fixed << std::setprecision(2) << average
           << "; Min: " << minVal
           << "; Max: " << maxVal
           << "; Found: " << (found ? "YES" : "NO");
    return output.str();
}
#include <iostream>
#include <sstream>
#include <cassert>

// Declare the function under test (exact signature from solution)
std::string analyzeThreeValues(int a, int b, int c);

int main() {
    // Helper to redirect cin for each test
    auto test = [](int a, int b, int c, int search, const std::string& expected) {
        std::istringstream input(std::to_string(search) + "\n");
        std::cin.rdbuf(input.rdbuf());  // redirect cin to our stream
        assert(analyzeThreeValues(a, b, c) == expected);
        std::cin.clear();
    };

    test(10, 20, 30, 20, "Avg: 20.00; Min: 10; Max: 30; Found: YES");
    test(-5, -10, 0, -5, "Avg: -5.00; Min: -10; Max: 0; Found: YES");
    test(7, 7, 7, 8, "Avg: 7.00; Min: 7; Max: 7; Found: NO");
    test(1, 2, 3, 4, "Avg: 2.00; Min: 1; Max: 3; Found: NO");
    test(100, -100, 0, 0, "Avg: 0.00; Min: -100; Max: 100; Found: YES");
    test(2, 2, 3, 4, "Avg: 2.33; Min: 2; Max: 3; Found: NO");
    test(-1, -2, -3, -3, "Avg: -2.00; Min: -3; Max: -1; Found: YES");
    test(0, 0, 0, 0, "Avg: 0.00; Min: 0; Max: 0; Found: YES");
    test(1000, 2000, 3000, 1500, "Avg: 2000.00; Min: 1000; Max: 3000; Found: NO");
    test(42, 17, 99, 17, "Avg: 52.67; Min: 17; Max: 99; Found: YES");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
