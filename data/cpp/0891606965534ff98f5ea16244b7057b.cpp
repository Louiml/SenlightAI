Write a C++ function named `calculateMenuOperation` that reads two floating-point numbers (`a` and `b`) and an integer choice (`d`) from standard input, performs the corresponding arithmetic operation (1: addition, 2: subtraction, 3: multiplication, 4: division, 5: exit), and returns the result as a `double`. For division, if the divisor is zero, return `NAN` (from `<cmath>`). For any invalid choice (not 1–5), return `NAN` as well. The function must handle input that may contain extra whitespace or newlines, and it must not print anything to the console. The caller is responsible for displaying the result. Ensure the function uses `const` where appropriate for parameters, though here parameters are read from input, not passed—so the function should read from `std::cin` directly and return a `double`. The function should be self-contained and not rely on any global state.

The solution reads three values from standard input: two floats and one integer, in the order specified by the original menu flow (the user selects an option first, then enters the two numbers). However, since the function is called per operation, we first read the integer `d`, then read `a` and `b` as floats. To mimic the original behavior, the function should read `d` first, then `a` and `b` (except for exit or invalid choices where no numbers are needed, but for simplicity we read them anyway or conditionally). For a clean design, read `d`, then if `d` is between 1 and 4, read `a` and `b`. If `d` is 5, return `NAN` (or could return 0, but the spec says invalid choices return NAN; exit is a valid choice but not an arithmetic operation, so treat it as a special case that returns `NAN` as well). Division by zero returns `NAN`. Invalid `d` returns `NAN`. Use `float` for input but cast to `double` for computation to maintain precision. Time complexity is O(1) as it performs a constant number of operations. Space complexity is O(1). Edge cases: zero divisor, invalid menu choice, non-numeric input (though the task assumes valid numeric input for simplicity). Use `std::isnan` from `<cmath>` to check results in tests.

#include <cmath>   // for NAN, isnan
#include <iostream> // for std::cin

/**
 * Reads a menu option and two float numbers from std::cin.
 * Returns the result of the selected operation as a double.
 * For division by zero, invalid choices, or exit, returns NAN.
 */
double calculateMenuOperation() {
    int d = 0;
    float a = 0.0f, b = 0.0f;

    // Read the menu choice
    std::cin >> d;

    // For valid arithmetic options, read the two numbers
    if (d >= 1 && d <= 4) {
        std::cin >> a >> b;
    }

    // Perform the selected operation
    switch (d) {
        case 1:
            return static_cast<double>(a) + static_cast<double>(b);
        case 2:
            return static_cast<double>(a) - static_cast<double>(b);
        case 3:
            return static_cast<double>(a) * static_cast<double>(b);
        case 4:
            if (b == 0.0f) {
                return NAN;
            }
            return static_cast<double>(a) / static_cast<double>(b);
        default:
            return NAN; // exit (5) or any invalid choice
    }
}

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

// Declare the solution function (assumed to be included via header or same file)
double calculateMenuOperation();

int main() {
    // Test addition: choose 1, numbers 3.5 and 2.2 → 5.7
    {
        std::istringstream input("1 3.5 2.2\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 5.7) < 1e-6);
    }

    // Test subtraction: choose 2, numbers 10 and 4.5 → 5.5
    {
        std::istringstream input("2 10 4.5\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 5.5) < 1e-6);
    }

    // Test multiplication: choose 3, numbers -2 and 6 → -12
    {
        std::istringstream input("3 -2 6\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result + 12.0) < 1e-6);
    }

    // Test division: choose 4, numbers 9 and 2 → 4.5
    {
        std::istringstream input("4 9 2\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 4.5) < 1e-6);
    }

    // Test division by zero: choose 4, numbers 5 and 0 → NAN
    {
        std::istringstream input("4 5 0\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::isnan(result));
    }

    // Test invalid choice: choose 9 → NAN (no numbers read)
    {
        std::istringstream input("9\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::isnan(result));
    }

    // Test exit choice: choose 5 → NAN
    {
        std::istringstream input("5\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::isnan(result));
    }

    // Test negative numbers with multiplication: choose 3, numbers -1.5 and -2 → 3
    {
        std::istringstream input("3 -1.5 -2\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 3.0) < 1e-6);
    }

    // Test extra whitespace: choose 1, numbers with leading/trailing spaces
    {
        std::istringstream input("   1   2.5  3.5   \n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 6.0) < 1e-6);
    }

    // Test division with negative divisor: choose 4, numbers -7 and -2 → 3.5
    {
        std::istringstream input("4 -7 -2\n");
        std::cin.rdbuf(input.rdbuf());
        double result = calculateMenuOperation();
        assert(std::abs(result - 3.5) < 1e-6);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
