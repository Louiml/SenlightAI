// Write a C++ function `std::string MatricaUString(const Matrica<double>& m)` that takes a `Matrica<double>` object and returns its string representation in the format `{{a11,a12,...,a1n},{a21,a22,...,a2n},...,{am1,am2,...,amn}}` with no spaces. Each element must be formatted as a plain decimal number without trailing zeros, e.g., `2.5`, `-3`, `0`, `1.25`. For example, for a 2x2 matrix with elements `1.0`, `2.5`, `-3.0`, and `0.0`, the output should be `{{1,2.5},{-3,0}}`. Ensure the function is `const`-correct and does not modify the input matrix. The function must work for any `Matrica<double>` instance that has been properly initialized with dimensions.

The solution requires traversing the matrix row by row and column by column, formatting each element as a string without trailing zeros and unnecessary decimal points. The core algorithm is straightforward: for each element, convert it to a string using `std::to_string`, then remove trailing zeros from the fractional part. Starting from the end of the string, repeatedly remove characters while they are `'0'`; after removing trailing zeros, if the last remaining character is `'.'`, remove it as well. The formatted elements are then concatenated following the required brace and comma structure. Important edge cases include integer values (e.g., `3.0` becomes `"3"`), negative numbers, and zeros. The time complexity is \(O(m \cdot n \cdot L)\), where \(L\) is the average length of the formatted number strings, since each element is processed constant times. Space complexity is \(O(m \cdot n \cdot L)\) for the output string, plus the internal temporary strings per element. The implementation must handle matrices of any valid dimensions, including 1x1, and must not alter the matrix.

#include <string>
#include <cstddef>

// Assume Matrica<double> is provided externally as in the snippet.
// This function returns the matrix as a string with no spaces and no trailing zeros in numbers.
std::string MatricaUString(const Matrica<double>& m) {
    std::string result = "{";
    for (int i = 0; i < m.br_redova; ++i) {  // Use public accessors if available; here using public operator[] and dimensions
        result += "{";
        for (int j = 0; j < m.br_kolona; ++j) {
            // Access element using const operator[] (returns pointer to row)
            double value = m[i][j];
            // Convert to string with std::to_string (produces e.g. "2.500000")
            std::string temp = std::to_string(value);
            // Remove trailing zeros
            while (!temp.empty() && temp.back() == '0') {
                temp.pop_back();
            }
            // If last char is '.', remove it as well (e.g., "2." -> "2")
            if (!temp.empty() && temp.back() == '.') {
                temp.pop_back();
            }
            result += temp;
            if (j != m.br_kolona - 1) {
                result += ",";
            }
        }
        result += "}";
        if (i != m.br_redova - 1) {
            result += ",";
        }
    }
    result += "}";
    return result;
}

// Assume the full Matrica class and operators from the snippet are available.
// This test file compiles and runs with the provided Matrica<double> implementation.
#include <cassert>

int main() {
    // Test 1: 2x2 matrix with mixed values
    Matrica<double> a(2, 2, 'A');
    a[0][0] = 1.0; a[0][1] = 2.5;
    a[1][0] = -3.0; a[1][1] = 0.0;
    assert(MatricaUString(a) == "{{1,2.5},{-3,0}}");

    // Test 2: 1x1 matrix
    Matrica<double> b(1, 1, 'B');
    b[0][0] = 7.0;
    assert(MatricaUString(b) == "{{7}}");

    // Test 3: 1x3 matrix
    Matrica<double> c(1, 3, 'C');
    c[0][0] = 0.0; c[0][1] = -0.0; c[0][2] = 3.25;
    assert(MatricaUString(c) == "{{0,0,3.25}}");

    // Test 4: 3x1 matrix
    Matrica<double> d(3, 1, 'D');
    d[0][0] = -1.5; d[1][0] = 2.0; d[2][0] = 4.75;
    assert(MatricaUString(d) == "{{-1.5},{2},{4.75}}");

    // Test 5: All integers
    Matrica<double> e(2, 2, 'E');
    e[0][0] = 10.0; e[0][1] = -20.0;
    e[1][0] = 30.0; e[1][1] = 40.0;
    assert(MatricaUString(e) == "{{10,-20},{30,40}}");

    // Test 6: Large numbers
    Matrica<double> f(1, 2, 'F');
    f[0][0] = 123456.789; f[0][1] = -0.001;
    // std::to_string of -0.001 gives "-0.001000", after trimming becomes "-0.001"
    assert(MatricaUString(f) == "{{123456.789,-0.001}}");

    // Test 7: values after decimal points that end in zero
    Matrica<double> g(1, 1, 'G');
    g[0][0] = 2.10;
    // std::to_string(2.1) gives "2.100000" -> trim to "2.1"
    assert(MatricaUString(g) == "{{2.1}}");

    // Test 8: negative zero
    Matrica<double> h(1, 1, 'H');
    h[0][0] = -0.0;
    // std::to_string(-0.0) gives "-0.000000" -> trim to "-0"
    assert(MatricaUString(h) == "{{-0}}");

    // Test 9: multiple trailing zeros
    Matrica<double> i(2, 1, 'I');
    i[0][0] = 3.14000;
    i[1][0] = 2.000;
    assert(MatricaUString(i) == "{{3.14},{2}}");

    // Test 10: empty matrix is not allowed by constructor, but test 0x0 not needed.
    return 0;
}
