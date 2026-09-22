// Write a C++ function named `applyUnaryOperations` that takes a positive floating-point number as input and returns a string describing the results of two unary operations applied in sequence: first, the number is raised to its own power (i.e., `x^x`), and then the exponential function (`e^x`) is applied to that result. The function must validate that the input is positive and finite (not NaN or infinity). For valid input, compute `first = pow(number, number)` and `second = exp(first)`, and return a string in exactly this format: `"First (x^x): <first>\nSecond (e^first): <second>"` where the numeric values are printed with default precision (as `std::cout` would). For invalid input (non-positive or non-finite), return the string `"Invalid input"`. The function should not prompt for input or print to the console; it only performs the computation and returns the formatted string. You may use `<cmath>` and `<string>` and `<sstream>`.
The solution begins by checking whether the input is finite and greater than zero using `std::isfinite` and a comparison to 0.0. If invalid, return `"Invalid input"`. For valid input, compute `first = std::pow(number, number)`. Note that `pow` may return large values for large numbers, but since the input is positive and finite, `first` will also be positive and finite (potential overflow for extremely large numbers but not expected in tests). Then compute `second = std::exp(first)`. This could cause overflow for large `first`; however, the function does not need to catch that per specification, but for robustness, if `exp` overflows to infinity, the formatted output would contain `inf` which is acceptable. Build the result string using `std::ostringstream` to insert the numbers with default formatting (like `std::cout`). Finally, return the string. Edge cases: input exactly 0 or negative returns invalid; NaN or infinity returns invalid. The time complexity is O(1), since only a couple of math operations and string formatting are performed. Space complexity is O(1) for the computation, but O(L) where L is the length of the output string, which is typically constant for reasonable inputs.
#include <cmath>
#include <sstream>
#include <string>

// Applies x^x then e^(x^x) to a positive finite float and returns a formatted description.
std::string applyUnaryOperations(float number) {
    // Validate input: must be positive and finite (not NaN or infinity).
    if (!(number > 0.0f) || !std::isfinite(number)) {
        return "Invalid input";
    }

    // First unary operation: raise number to its own power.
    float first = std::pow(number, number);

    // Second unary operation: exponential of the first result.
    float second = std::exp(first);

    // Format the result using a string stream (default precision like std::cout).
    std::ostringstream oss;
    oss << "First (x^x): " << first << "\nSecond (e^first): " << second;
    return oss.str();
}
#include <cassert>
#include <string>

// Declaration of the solution function (assume it is defined above in the same translation unit).
std::string applyUnaryOperations(float number);

int main() {
    // Test valid non-integer input: 2.0 -> 2^2=4, e^4 ~ 54.5982
    std::string result1 = applyUnaryOperations(2.0f);
    assert(result1.find("First (x^x): 4") != std::string::npos);
    assert(result1.find("Second (e^first): 54.5982") != std::string::npos);

    // Test valid small input: 1.0 -> 1^1=1, e^1 ~ 2.71828
    std::string result2 = applyUnaryOperations(1.0f);
    assert(result2.find("First (x^x): 1") != std::string::npos);
    assert(result2.find("Second (e^first): 2.71828") != std::string::npos);

    // Test invalid zero
    assert(applyUnaryOperations(0.0f) == "Invalid input");

    // Test invalid negative
    assert(applyUnaryOperations(-3.5f) == "Invalid input");

    // Test invalid NaN
    assert(applyUnaryOperations(std::nanf("")) == "Invalid input");

    // Test invalid infinity
    assert(applyUnaryOperations(INFINITY) == "Invalid input");

    // Test valid very small positive (0.5 -> 0.5^0.5 ~ 0.707107, exp ~ 2.02811)
    std::string result3 = applyUnaryOperations(0.5f);
    assert(result3.find("First (x^x): 0.707107") != std::string::npos);
    assert(result3.find("Second (e^first): 2.02811") != std::string::npos);

    // Test valid large but finite (10.0 -> 10^10 = 1e10, exp extremely large, may be inf, but still valid)
    std::string result4 = applyUnaryOperations(10.0f);
    assert(result4.find("First (x^x): 1e+10") != std::string::npos);

    return 0;
}
