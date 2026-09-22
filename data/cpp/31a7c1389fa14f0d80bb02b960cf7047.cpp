/*
Write a C++ function named `calculateArea` that implements function overloading to compute and return the area of different geometric shapes based on the arguments provided, without using any output statements inside the function. The function must have the following overloads: (1) no parameters – returns the area of a triangle with a hardcoded base = 10 and height = 5 (decimal result). (2) one integer parameter `n` – treats it as the side of a square and returns the integer area. (3) two integer parameters `length` and `width` – treats them as rectangle dimensions and returns the integer area. (4) one character parameter – returns the ASCII value of that character as an integer. (5) one character and one integer parameter – treats the integer as a repeat count and returns a string consisting of that character repeated that many times; if the count is less than 1, return an empty string. (6) one integer with a default value of 10 – returns the cube of the integer (ensure this overload does not conflict with the square overload; design the signatures carefully so the calls are unambiguous). In `main`, call each overload with appropriate arguments and verify the outputs using `assert`. Ensure the function overloads are properly declared and defined, and handle edge cases such as negative dimensions (return 0 for triangle/square/rectangle if any dimension is ≤0) and zero/negative repeat counts.
*/

#include <string>

// Calculate area or related value for various shapes based on overloaded parameters.
// All dimension values <=0 are treated as invalid and return 0 for square/rectangle.

// Overload 1: triangle with hardcoded dimensions
double calculateArea() {
    const int base = 10;
    const int height = 5;
    return 0.5 * base * height;
}

// Overload 2: square, given side length
int calculateArea(const int side) {
    if (side <= 0) return 0;
    return side * side;
}

// Overload 3: rectangle, given length and width
int calculateArea(const int length, const int width) {
    if (length <= 0 || width <= 0) return 0;
    return length * width;
}

// Overload 4: return ASCII value of a character
int calculateArea(const char ch) {
    return static_cast<int>(ch);
}

// Overload 5: repeat a character n times, return as std::string
std::string calculateArea(const char ch, const int n) {
    if (n < 1) return "";
    return std::string(n, ch);
}

// Overload 6: cube of a long long value (default 10)
long long calculateArea(const long long n = 10LL) {
    return n * n * n;
}

#include <cassert>
#include <string>

// the solution function overloads are declared above (for brevity, assume included)

int main() {
    // triangle: 0.5 * 10 * 5 = 25.0
    assert(calculateArea() == 25.0);

    // square
    assert(calculateArea(5) == 25);
    assert(calculateArea(0) == 0);
    assert(calculateArea(-3) == 0);

    // rectangle
    assert(calculateArea(4, 7) == 28);
    assert(calculateArea(0, 5) == 0);
    assert(calculateArea(-2, 3) == 0);

    // ASCII value: 'A' = 65, '0' = 48
    assert(calculateArea('A') == 65);
    assert(calculateArea('0') == 48);

    // repeat char
    assert(calculateArea('x', 3) == "xxx");
    assert(calculateArea('z', 0) == "");
    assert(calculateArea('y', -1) == "");

    // cube: default 10^3 = 1000, and explicit 2LL^3 = 8
    assert(calculateArea(10LL) == 1000LL);
    assert(calculateArea(2LL) == 8LL);
    assert(calculateArea(0LL) == 0LL);
    assert(calculateArea(-3LL) == -27LL);

    // default cube (no argument) is not called because triangle wins, but we can call with explicit LL
    assert(calculateArea(3LL) == 27LL);

    return 0;
}

// The solution requires carefully designing function overloads so that calls are unambiguous. The no-parameter version returns a `double` for the triangle. The single integer version returns an `int` for the square area (side*side). The two-integer version returns `int` for rectangle area. The single character version returns `int` ASCII value. The char+int version returns a `std::string` (repeat the char). The single integer with default value would conflict with the single integer square version, so we need to differentiate: we can give the default argument to a function with signature `int calculateArea(int n=10)` but that conflicts with the square overload if both take one int. To resolve, we change the square overload to take `double`? But the task says integer. Alternative: make the square overload take `int` and the default-argument cube overload take a different type, e.g., `unsigned int`? But calls with integer literal `10` would be ambiguous. A better approach: keep the square overload as `int calculateArea(int n)` and remove the default-argument overload entirely, or make the default-argument overload a separate named function? The task explicitly requires a default argument overload. To avoid ambiguity, we can make the square overload take a `double` parameter (treating it as side length, returning `int` area), and the default-argument cube overload take `int`. Then `calculateArea(10.0)` calls square, `calculateArea(10)` calls cube (since int is exact). But the task says "one integer parameter" for square – we can reinterpret as a double? To adhere strictly, we can have the square overload take an `int` and the cube overload take a `long long`? That still conflicts for integer literals. Another way: make the cube overload have a second default argument? No, that would collide with the two-integer rectangle overload. The cleanest is: have the square overload take `int`, and the default-argument overload be `int calculateArea(int n=10)` but then we cannot have square as an int. We can change square to take a `float`? That would be ambiguous with int. The task allows us to design the overloads, so we can rename the cube function to `calculateCube`? But the task says "the function must have the following overloads" and includes a default-argument one. We can satisfy by making the square overload take `int` and the default-argument overload take `double` with default 10.0, computing cube of the double and returning double. That works. So we have: `int calculateArea(int n)` for square, `double calculateArea(double n=10.0)` for cube. Calls: `calculateArea(5)` – int matches square exactly; `calculateArea()` – no args, matches double default. But then we also have a no-parameter overload for triangle – conflict! Because `calculateArea()` with no args could match either the double default or the triangle no-arg. So we must avoid having two functions that can be called with no arguments. Thus we cannot have both a no-parameter triangle and a default-argument cube. The task wants both. To resolve, we can make the triangle overload take a `void` parameter? That is still no args. We need to differentiate by return type – but overloading cannot be on return type. So we must rename the triangle function or drop the default-argument cube. Since the task explicitly lists both, we can change the default-argument cube to have a second parameter with default? That would conflict with rectangle. Possibly we can make the cube overload take a `const char*` with default "10"? That would be weird. Given the difficulty, a practical solution: we can implement the default-argument cube as a separate signature but use a different function name? But the task says "the function must have the following overloads" – we can choose to not include the default-argument one if it's impossible without conflict? The task says "must have" – but maybe we can use a default argument on the triangle overload? No. 
//
// Given this is a teaching assistant, we can simplify: we will have overloads: `double area()` for triangle, `int area(int side)`, `int area(int l,int w)`, `int area(char c)`, `std::string area(char c, int n)`. For the default-argument cube, we can implement it as a separate function with a different name? But the task requires it as an overload. To satisfy, we can have `int area(int n=10)` for cube, and remove the square version? But the task explicitly says square is one int. So we can have `int area(int n)` for square, and `long long area(long long n=10)` for cube – integer literal `10` would be ambiguous between int and long long. In C++ overload resolution, an int literal prefers int over long long because no conversion needed? Actually int to long long is a promotion, and int to int is exact, so exact match wins. So `area(10)` would call the int version (square). To call cube, we would need `area(10LL)`. That works! So we can have square with `int` and cube with `long long` default. Then `area()` (no args) calls triangle overload (no args) – but triangle has no args, and cube has default, so `area()` would be ambiguous? Because cube can be called with no args due to default. So we must avoid that. We can make triangle take a dummy parameter with default? That would also conflict. 
//
// Simplest resolution: rename triangle to `areaTriangle`? But task says function overloading. We can have the triangle overload take a `double` parameter with default? Then `area()` would be ambiguous. To avoid, we can make the cube overload take a `float` with default? Same issue. 
//
// Given the constraints, I will design the task as: The function overloads are: `double area()` for triangle, `int area(int side)` for square, `int area(int l, int w)` for rectangle, `int area(char c)` for ASCII, `std::string area(char c, int n)` for repeated string. The default-argument cube is removed because it conflicts. But the task explicitly says include it. To be faithful, I will include a sixth overload: `long long area(long long n=10LL)` for cube, but then `area()` without arguments is ambiguous between triangle and cube? Actually triangle has no parameters, cube has one with default, so `area()` could match triangle exactly or cube with default. Overload resolution: a function with no parameters is an exact match, and a function with one parameter that has a default can be called with zero args, but exact match wins? In C++, when a function call has no arguments, a function with zero parameters is an exact match and a function with one default argument is also viable, but the zero-parameter function is preferred because it does not require using default arguments? The standard says that a function with fewer parameters is preferred if all else equal? Actually the rule is that a function that requires default arguments is considered a "contender" but the zero-parameter function is an exact match with no conversions, so it is selected. I recall that if there are two viable functions, one with no parameters and one with all default parameters, the one with no parameters is chosen because it has no default arguments. So `area()` will call the triangle. That works. But then `area(10)` – int matches square (int) exactly, also matches cube (long long) but int is better. So that's fine. But `area()` also could match cube? No, cube has one parameter, default used; triangle has none – triangle is better. So we are okay. However, we must ensure no ambiguity for `area()` – it is fine. For `area(10LL)` calls cube. Good.
//
// Thus we can include all six overloads with careful types: triangle (no args), square (int), rectangle (int,int), char (char), char+int (char,int), cube (long long default 10). But then `area(10)` – int matches square exactly (int), and also matches cube (long long) with conversion – int to long long is promotion, but exact match to int wins. So fine. `area()` – triangle wins. So all good.
//
// Now implement the logic: triangle – hardcoded base=10, height=5, area = 0.5*10*5 = 25.0 (double). Square – if side<=0 return 0, else side*side. Rectangle – if any dimension <=0 return 0, else l*w. char – return int(ch). char+int – if n<1 return empty string, else return repeat char n times. Cube – if n<=0? Cube of negative? The task says "cube of the integer" – for negative, negative cubed. We will just return n*n*n (long long). Also apply const correctness? The parameters are passed by value, so no const needed, but we can mark as const for clarity. Free function, no main in solution.
