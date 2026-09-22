// Write a C++ function named `optimizeMatrixVectorMul` that takes a vector of arithmetic expressions represented as strings in a simple custom format: each expression is either a matrix-vector multiplication like `M * v`, a matrix transpose multiplication like `M^T * v`, a standalone matrix name (`M`), or a standalone vector name (`v`). The function should identify expressions that multiply a matrix by a vector and, if the matrix is one of two whitelisted names (`A` or `B`), rewrite the expression to use the transpose of that matrix in a vector-matrix-transpose form: specifically, swap the operands so the expression becomes `v * M^T`. For other matrices (e.g., `C`, `D`), leave the expression unchanged. The input is a vector of strings, each in the format `"LHS * RHS"` or simply a name (no spaces). The function returns a new vector of strings with the transformations applied. Assume all input strings are valid and no spaces except around the `*`. The transposed matrix name uses a suffix `_T` (e.g., `A_T` for `A`). Only matrix-vector multiplications where the left operand is exactly `A` or `B` and the right operand is a valid vector name (single uppercase letter, e.g., `V`, `W`) should be transformed. Also update the expression to use the vector first and the transposed matrix second, separated by ` * ` (with spaces). If the expression is a standalone name (e.g., `"A"` or `"V"`), keep it as is. Preserve order.

The goal is to scan each input string and detect a multiplication pattern. The key algorithm: for each string, if it contains `" * "` (since only multiplications have that format), split into left and right parts. Check if the left part is exactly `"A"` or `"B"` and the right part is a single uppercase letter (a vector name). If so, transform to `right + " * " + left + "_T"`. Otherwise, leave the string unchanged. Edge cases: standalone names have no `" * "`, so they pass through unchanged. Strings like `"C * V"` or `"A * 5"` (right not a vector) are left unchanged. Strings like `"A * B"` (right is a matrix?) but since right is uppercase letter but could be matrix, but specification says vector name is uppercase letter; however to be safe, we check if right is exactly one character and is uppercase alpha. Also ensure no duplicate transformations: if an expression already uses `_T`, it won't match because left operand is not exactly `A` or `B`. For each string, we parse using `std::string::find` to locate `" * "`, then substring left and right. Time complexity is O(n * m) where n is number of strings and m is average string length due to string operations; auxiliary space O(n) for the result vector.

#include <string>
#include <vector>
#include <cctype>

// Transforms specific matrix-vector multiplications to use transposed matrices.
// Given a vector of expressions, returns a new vector where any expression
// exactly of the form "A * V" or "B * V" (where V is a single uppercase letter)
// is rewritten as "V * A_T" or "V * B_T". All other expressions are unchanged.
std::vector<std::string> optimizeMatrixVectorMul(const std::vector<std::string>& expressions) {
    std::vector<std::string> result;
    result.reserve(expressions.size());

    const std::string mult_sep = " * ";

    for (const auto& expr : expressions) {
        // Find the multiplication separator. If not present, it's a standalone name.
        const size_t sep_pos = expr.find(mult_sep);
        if (sep_pos == std::string::npos) {
            result.push_back(expr);
            continue;
        }

        // Extract left and right sides.
        const std::string left = expr.substr(0, sep_pos);
        const std::string right = expr.substr(sep_pos + mult_sep.length());

        // Check if left is exactly "A" or "B" and right is a single uppercase letter.
        const bool is_valid_matrix = (left == "A" || left == "B");
        const bool is_valid_vector = (right.length() == 1) && std::isupper(static_cast<unsigned char>(right[0]));

        if (is_valid_matrix && is_valid_vector) {
            // Rewrite as "vector * matrix_T"
            result.push_back(right + " * " + left + "_T");
        } else {
            // Keep original expression.
            result.push_back(expr);
        }
    }

    return result;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Basic transformation
    std::vector<std::string> input1 = {"A * V"};
    auto out1 = optimizeMatrixVectorMul(input1);
    assert(out1.size() == 1 && out1[0] == "V * A_T");

    // Mixed expressions
    std::vector<std::string> input2 = {"B * W", "C * V", "A * X", "D"};
    auto out2 = optimizeMatrixVectorMul(input2);
    assert(out2.size() == 4);
    assert(out2[0] == "W * B_T");
    assert(out2[1] == "C * V");  // unchanged, matrix not whitelisted
    assert(out2[2] == "X * A_T");
    assert(out2[3] == "D");      // standalone

    // No multiplication
    std::vector<std::string> input3 = {"A", "V"};
    auto out3 = optimizeMatrixVectorMul(input3);
    assert(out3 == input3);

    // Invalid right operand (not a vector)
    std::vector<std::string> input4 = {"A * 5", "B * VV"};
    auto out4 = optimizeMatrixVectorMul(input4);
    assert(out4[0] == "A * 5");
    assert(out4[1] == "B * VV");

    // Already transposed form shouldn't change
    std::vector<std::string> input5 = {"V * A_T"};
    auto out5 = optimizeMatrixVectorMul(input5);
    assert(out5[0] == "V * A_T");

    // Empty input
    std::vector<std::string> input6;
    auto out6 = optimizeMatrixVectorMul(input6);
    assert(out6.empty());

    // Multiple identical transformations
    std::vector<std::string> input7 = {"A * V", "A * V"};
    auto out7 = optimizeMatrixVectorMul(input7);
    assert(out7[0] == "V * A_T" && out7[1] == "V * A_T");
}
