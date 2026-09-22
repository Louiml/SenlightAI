Write a C++ function named `swapAndPrintSum` that takes two references to variables of a generic numeric type `T` (where `T` supports `operator+` and `operator<<`), swaps their values, and then prints the sum of the swapped values to standard output in the format `sum: <value>`. The function must not return anything. The task requires using templates to support any numeric type (e.g., `int`, `double`, `float`, etc.), and the function must correctly swap the original variables' values (observable by the caller) and output the sum after the swap. Provide appropriate `const` correctness where applicable, though note that the parameters must be non-const references since swapping modifies them.
The solution involves a template function that takes two non-const references of type `T`. The swap is straightforward: store one value in a temporary, assign the second to the first, assign the temporary to the second. After swapping, compute the sum of the two new values using `a + b` and print it to `std::cout`. Important edge cases: The types must support copy construction and assignment, and `operator+` must be defined. For numeric types like `int`, `double`, this works fine. Since the parameters are references, the swap affects the original variables in the caller's scope. Time complexity is O(1) for both swap and sum operations. Space complexity is O(1) for the temporary variable and the sum variable, though the sum variable uses a local `T` which may have its own overhead depending on type (but for primitives it's constant). No dynamic memory or containers are used.
#include <iostream>

// Swaps two values of generic type T and prints their sum after swapping.
// The swap modifies the original variables because they are passed by reference.
template <typename T>
void swapAndPrintSum(T& a, T& b) {
    // Swap using a temporary variable
    T temp = a;
    a = b;
    b = temp;

    // Compute the sum after swapping
    T sum = a + b;
    std::cout << "sum: " << sum << std::endl;
}
#include <cassert>
#include <iostream>

// Declare the function template (declared in solution, but we need it here)
template <typename T>
void swapAndPrintSum(T& a, T& b);

int main() {
    // Test with integers
    int x = 5, y = 10;
    swapAndPrintSum(x, y);
    assert(x == 10 && y == 5); // swapped correctly
    
    // Test with doubles
    double p = 1.5, q = 2.5;
    swapAndPrintSum(p, q);
    assert(p == 2.5 && q == 1.5); // swapped correctly
    
    // Test with floats
    float m = 3.3f, n = 4.4f;
    swapAndPrintSum(m, n);
    assert(m == 4.4f && n == 3.3f); // swapped correctly
    
    // Test with negative numbers
    int a = -5, b = 5;
    swapAndPrintSum(a, b);
    assert(a == 5 && b == -5);
    
    // Test with identical values
    int c = 7, d = 7;
    swapAndPrintSum(c, d);
    assert(c == 7 && d == 7); // swap leaves same values, sum is 14
    
    // Test with large numbers (int max and 0)
    int big = 2000000000, zero = 0;
    swapAndPrintSum(big, zero);
    assert(big == 0 && zero == 2000000000);
    
    // Test with long long
    long long l1 = 10000000000LL, l2 = -10000000000LL;
    swapAndPrintSum(l1, l2);
    assert(l1 == -10000000000LL && l2 == 10000000000LL);
    
    // Test with unsigned int (must be careful with sum overflow, but swap works)
    unsigned int u1 = 3, u2 = 4;
    swapAndPrintSum(u1, u2);
    assert(u1 == 4 && u2 == 3);
    
    // Test with char (treated as numeric type)
    char ch1 = 'A', ch2 = 'B';
    swapAndPrintSum(ch1, ch2);
    assert(ch1 == 'B' && ch2 == 'A');
    
    // Test with bool (works, though sum is bool arithmetic)
    bool b1 = true, b2 = false;
    swapAndPrintSum(b1, b2);
    assert(b1 == false && b2 == true);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
