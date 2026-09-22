/*
Write a C++ function that reads two decimal numbers from standard input (each on its own line or separated by whitespace), computes their product as an integer by truncating the decimal part (i.e., discarding any fractional portion of the intermediate double product, not rounding), and returns that integer value as an `int`. The function should handle negative numbers correctly (e.g., the product of `-2.5` and `3.0` is `-7`, not `-7.5`), and should not perform any explicit rounding or type conversion beyond what the original snippet does. The function must be self-contained and must not rely on any global state.
*/

#include <iostream>

// Reads two decimal numbers from standard input and returns their product truncated to an int.
// The function assumes the product fits within the range of int and that input is valid.
int truncatedProduct() {
    double x = 0.0;
    double y = 0.0;
    std::cin >> x >> y;
    // Truncates the double product toward zero when converting to int.
    int product = x * y;
    return product;
}

#include <cassert>

// Forward declaration of the solution function (assumes the solution is included above).
int truncatedProduct();

int main() {
    // To test, we simulate stdin using simple redirection by creating a helper lambda
    // that sets cin's buffer to a stringstream and calls the function.
    // Because we cannot redefine cin easily in a portable way, we test the logic
    // by extracting the core computation into a testable helper? Instead,
    // we will directly test the truncation logic with inline doubles.
    // Since the function reads from stdin, we use a small helper to feed input.
    // For demonstration, we test the core logic separately and also test the function
    // with a custom input stream replacement.
    
    // Test the truncation behavior directly (equivalent to the function's core).
    auto truncateProduct = [](double a, double b) -> int {
        // Replicate the exact expression from the solution.
        int result = a * b;
        return result;
    };
    
    // Basic cases
    assert(truncateProduct(2.0, 3.0) == 6);
    assert(truncateProduct(-2.5, 3.0) == -7); // -7.5 truncated to -7
    assert(truncateProduct(0.0, 5.0) == 0);
    assert(truncateProduct(-1.2, -2.0) == 2); // 2.4 truncated to 2
    assert(truncateProduct(1.9, 1.0) == 1);   // 1.9 truncated to 1
    assert(truncateProduct(-0.5, 0.5) == 0);  // -0.25 truncated to 0
    
    // Edge: product is exactly integer
    assert(truncateProduct(2.5, 4.0) == 10);  // 10.0 → 10
    assert(truncateProduct(-3.0, 2.0) == -6); // -6.0 → -6
    
    // Edge: values that produce a product just below zero
    assert(truncateProduct(-0.1, 0.1) == 0);  // -0.01 truncated to 0
    
    // Test the actual function using a temporary replacement of cin's buffer.
    // This is a common trick using <sstream> and <iostream> but requires including them.
    // For simplicity we instead directly test the function via a helper that feeds a string.
    // Since the solution function reads from std::cin, we simulate by using a stringstream
    // and redirecting cin's rdbuf. This is valid in a test context.
    std::streambuf* original = std::cin.rdbuf();
    {
        std::istringstream input("2.5 3.0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(truncatedProduct() == 7); // 7.5 truncated to 7
    }
    {
        std::istringstream input("-2.5 3.0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(truncatedProduct() == -7);
    }
    {
        std::istringstream input("0.5 -0.5\n");
        std::cin.rdbuf(input.rdbuf());
        assert(truncatedProduct() == 0);
    }
    std::cin.rdbuf(original);
    
    return 0;
}

// The core problem is to reproduce the behavior of the snippet: read two doubles, multiply them, assign the result to an `int` variable (which truncates toward zero in C++), and return that `int`. Because the input values are doubles, the multiplication may produce a non-integer result; assigning to `int` discards the fractional part. Important edge cases include: (1) zero inputs, (2) negative products where truncation moves toward zero (e.g., `-0.5 * 2 = -1.0` → `-1`, but `-2.5 * 0.4 = -1.0` → `-1`; note that for values like `-2.9 * 1 = -2.9` → `-2`), (3) values that are already integers, and (4) extremely large or small products that could overflow if stored in an `int` — but the task specification assumes the product fits within `int` bounds. The algorithm simply reads two doubles using `std::cin`, multiplies them into a `double` intermediate, then casts to `int` via direct assignment (which truncates). Time complexity is O(1) and space complexity is O(1).
