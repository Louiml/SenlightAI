/*
Write a C++ function template named `addValues` that accepts two parameters of the same type and returns their sum. The function must work for fundamental numeric types such as `int`, `double`, `float`, and `long long`, as well as for user-defined types that overload the `+` operator. Additionally, ensure the function is `const`-correct: it should not modify the input parameters, and it should be callable with `const` objects. The template must be defined in a header-style manner so it can be used across multiple translation units. Finally, provide test cases demonstrating the function’s behavior with two different types (e.g., `int` and `double`) and with `const` arguments.
*/
#include <type_traits>

// Return the sum of two values of the same type.
// Works for any type that supports operator+.
template <typename T>
constexpr T addValues(const T a, const T b) {
    return a + b;
}
#include <cassert>
#include <string>

// Include the solution (assuming it's in the same file or included)
template <typename T>
constexpr T addValues(const T a, const T b) {
    return a + b;
}

// Define a simple user-defined type to test the template with operator+ overload
struct Point {
    int x, y;
    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

int main() {
    // Test with int
    const int a = 1, b = 2;
    assert(addValues(a, b) == 3);
    
    // Test with double
    const double c = 1.5, d = 2.5;
    assert(addValues(c, d) == 4.0);
    
    // Test with float
    const float e = 1.25f, f = 2.75f;
    assert(addValues(e, f) == 4.0f);
    
    // Test with long long
    const long long g = 10000000000LL, h = 20000000000LL;
    assert(addValues(g, h) == 30000000000LL);
    
    // Test with user-defined type
    Point p1{1, 2}, p2{3, 4};
    Point result = addValues(p1, p2);
    assert(result.x == 4 && result.y == 6);
    
    // Test with const arguments (implicitly covered above)
    // Compile-time evaluation check
    static_assert(addValues(2, 3) == 5, "Compile-time sum works");
    
    return 0;
}
// The solution is a straightforward function template that uses the `+` operator on two parameters of the same type. Since the template is generic, it will work for any type that supports `operator+`. The parameters are passed by value, which is fine for small types like integers and floating-point numbers, but for user-defined types, pass-by-value may involve copying; this is acceptable for the task’s scope. The function is marked `constexpr` where possible to allow compile-time evaluation and to be `const`-correct by not modifying the inputs. The main algorithm is simply returning `a + b`. Edge cases: for floating-point types, addition follows IEEE 754 rules (e.g., `NaN`, infinity) but no special handling is needed. For integer overflow, the behavior is undefined in C++, but the task does not require overflow handling. The time complexity is O(1) for built-in types, and space complexity is O(1). The template must be placed in a header (or a single file with the test code) because templates are instantiated at compile time. The provided solution will be a free function template with a `constexpr` specifier, and parameters are taken by value (which is const-correct because no modifications occur).
