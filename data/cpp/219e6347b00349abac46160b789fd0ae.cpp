/*
Write a C++ function `decomposeSingleAddmm` that takes a simplified representation of a computation graph node and returns an equivalent graph expressed as two operations: a matrix multiplication (`mm`) followed by an addition (`add`), but only when the original node represents an `addmm` operation with exactly three matrix inputs and both scalar coefficients `alpha` and `beta` are equal to 1.0. The function should accept a struct representing the node with fields: an operation kind string (`"addmm"` for eligible), an array of three dense double-precision matrices (each a 2D `std::vector<std::vector<double>>`), and two scalar coefficients. If the node is not an `addmm` or if either scalar is not exactly 1.0, return the original node unchanged. For eligible nodes, return a new struct representing two sequential operations: first `mm` on the second and third input matrices, then `add` of the first input matrix to that product (element-wise addition, with broadcasting only if the first matrix has shape 1×N or M×1 matching one dimension, otherwise require exact same shape). Assume all matrix dimensions are non-zero and valid for multiplication (inner dimensions match).
*/

#include <vector>
#include <string>
#include <stdexcept>
#include <cstddef>

// Simplified node in a computation graph.
struct GraphNode {
    std::string kind;                     // "addmm", "mm", "add", etc.
    std::vector<std::vector<double>> inputs; // For addmm: [bias, mat1, mat2]; for others: variable.
    double alpha = 0.0;
    double beta = 0.0;
    std::vector<GraphNode> children;      // sub-operations after decomposition
};

// Matrix multiplication: A (m x k) * B (k x n) => C (m x n)
static std::vector<std::vector<double>> matrixMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    std::size_t m = A.size();
    std::size_t k = A[0].size();
    std::size_t n = B[0].size();
    std::vector<std::vector<double>> C(m, std::vector<double>(n, 0.0));
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (std::size_t p = 0; p < k; ++p) {
                sum += A[i][p] * B[p][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

// Element-wise addition with broadcasting support (1xN or Mx1 bias).
static std::vector<std::vector<double>> matrixAdd(
    const std::vector<std::vector<double>>& bias,
    const std::vector<std::vector<double>>& product) {
    std::size_t m = product.size();
    std::size_t n = product[0].size();
    std::vector<std::vector<double>> result = product;
    bool biasIsRow = (bias.size() == 1 && bias[0].size() == n);
    bool biasIsCol = (bias.size() == m && bias[0].size() == 1);
    bool biasSame = (bias.size() == m && bias[0].size() == n);
    if (!biasIsRow && !biasIsCol && !biasSame) {
        throw std::invalid_argument("Bias shape not broadcastable to product");
    }
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            double b = 0.0;
            if (biasSame) {
                b = bias[i][j];
            } else if (biasIsRow) {
                b = bias[0][j];
            } else if (biasIsCol) {
                b = bias[i][0];
            }
            result[i][j] += b;
        }
    }
    return result;
}

// Decompose addmm into mm followed by add if alpha=1.0 and beta=1.0.
GraphNode decomposeSingleAddmm(const GraphNode& node) {
    if (node.kind != "addmm" || node.inputs.size() != 3) {
        return node; // Not an addmm or malformed, return unchanged.
    }
    if (node.alpha != 1.0 || node.beta != 1.0) {
        return node; // Coefficients not both 1.0, return unchanged.
    }
    const auto& bias = node.inputs[0];
    const auto& mat1 = node.inputs[1];
    const auto& mat2 = node.inputs[2];

    // Check inner dimensions match for multiplication.
    if (mat1.empty() || mat2.empty() || mat1[0].size() != mat2.size()) {
        throw std::invalid_argument("Matrix dimensions do not align for multiplication");
    }

    auto product = matrixMultiply(mat1, mat2);
    auto finalResult = matrixAdd(bias, product);

    // Build the decomposed node: first mm with inputs [mat1, mat2], then add with [bias, mm_result].
    GraphNode mm_node;
    mm_node.kind = "mm";
    mm_node.inputs = {mat1, mat2};

    GraphNode add_node;
    add_node.kind = "add";
    add_node.inputs = {bias, product}; // product is the result of mm

    GraphNode result;
    result.kind = "decomposed";
    result.inputs = {finalResult}; // The final combined result for convenience
    result.children = {mm_node, add_node};
    return result;
}

#include <cassert>
#include <iostream>

int main() {
    // Test 1: Valid addmm with alpha=beta=1.0, bias same shape.
    GraphNode node1;
    node1.kind = "addmm";
    node1.alpha = 1.0;
    node1.beta = 1.0;
    node1.inputs = {
        {{1.0, 0.0}, {0.0, 1.0}},  // bias 2x2
        {{1.0, 2.0}, {3.0, 4.0}},  // mat1 2x2
        {{5.0, 6.0}, {7.0, 8.0}}   // mat2 2x2
    };
    auto res1 = decomposeSingleAddmm(node1);
    assert(res1.kind == "decomposed");
    assert(res1.children.size() == 2);
    assert(res1.children[0].kind == "mm");
    assert(res1.children[1].kind == "add");
    // Expected mm: [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]] = [[19,22], [43,50]]
    // Then add bias: [[20,22], [43,51]]
    assert(res1.inputs[0] == std::vector<std::vector<double>>({{20.0,22.0},{43.0,51.0}}));

    // Test 2: alpha not 1.0, should return unchanged.
    GraphNode node2;
    node2.kind = "addmm";
    node2.alpha = 2.0;
    node2.beta = 1.0;
    node2.inputs = {
        {{0.0}}, {{1.0}}, {{2.0}}
    };
    auto res2 = decomposeSingleAddmm(node2);
    assert(res2.kind == "addmm");
    assert(res2.inputs.size() == 3);
    assert(res2.children.empty());

    // Test 3: beta not 1.0.
    GraphNode node3 = node2;
    node3.alpha = 1.0;
    node3.beta = 0.5;
    auto res3 = decomposeSingleAddmm(node3);
    assert(res3.kind == "addmm");

    // Test 4: Not an addmm.
    GraphNode node4;
    node4.kind = "mm";
    node4.inputs = {{ {1.0}, {2.0} }};
    auto res4 = decomposeSingleAddmm(node4);
    assert(res4.kind == "mm");

    // Test 5: Broadcasting bias row (1xN).
    GraphNode node5;
    node5.kind = "addmm";
    node5.alpha = 1.0;
    node5.beta = 1.0;
    node5.inputs = {
        {{10.0, 20.0}},                  // bias 1x2
        {{1.0, 2.0}, {3.0, 4.0}},        // mat1 2x2
        {{5.0, 6.0}, {7.0, 8.0}}         // mat2 2x2
    };
    auto res5 = decomposeSingleAddmm(node5);
    // mm = [[19,22], [43,50]] + bias row -> [[29,42], [53,70]]
    assert(res5.inputs[0] == std::vector<std::vector<double>>({{29.0,42.0},{53.0,70.0}}));

    // Test 6: Broadcasting bias column (Mx1).
    GraphNode node6;
    node6.kind = "addmm";
    node6.alpha = 1.0;
    node6.beta = 1.0;
    node6.inputs = {
        {{100.0}, {200.0}},              // bias 2x1
        {{1.0, 2.0}, {3.0, 4.0}},
        {{5.0, 6.0}, {7.0, 8.0}}
    };
    auto res6 = decomposeSingleAddmm(node6);
    // mm = [[19,22], [43,50]] + bias column -> [[119,122], [243,250]]
    assert(res6.inputs[0] == std::vector<std::vector<double>>({{119.0,122.0},{243.0,250.0}}));

    // Test 7: Invalid broadcast shape throws.
    GraphNode node7 = node1;
    node7.inputs[0] = {{1.0, 2.0, 3.0}}; // 1x3 cannot broadcast to 2x2
    bool threw = false;
    try {
        decomposeSingleAddmm(node7);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 8: Inner dimension mismatch throws.
    GraphNode node8 = node1;
    node8.inputs[2] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}; // 2x3, mat1 is 2x2 -> inner 2 != 3
    threw = false;
    try {
        decomposeSingleAddmm(node8);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    std::cout << "All tests passed.\n";
    return 0;
}

// The solution must first validate that the node is indeed an `addmm` with exactly three inputs and that both `alpha` and `beta` are exactly 1.0. If either condition fails, return the input unchanged. Otherwise, perform the matrix multiplication of the second input (mat1) with the third input (mat2) using standard triple-nested loop O(M*K*N), where mat1 is M×K and mat2 is K×N. Then add the first input matrix (bias) to the product. For addition, handle broadcasting: if bias is 1×N, add to each row; if bias is M×1, add to each column; if bias has same shape as product, add element-wise; otherwise, throw an exception (though the problem assumes valid input). The returned struct should have operation kind `"mm_add"` and store the result of the mm and the bias? Actually, to preserve the sequence, the natural representation is to return a struct with a list of two sub-operations: first `mm` with inputs [mat1, mat2], then `add` with inputs [bias, mm_result]. So the struct could have a field `sub_ops` vector of two nodes. For simplicity, define a `GraphNode` struct that has a kind string, a vector of matrices (inputs), and optionally a vector of child nodes. The solution returns that. Edge cases: alpha/beta might be stored as `double` with floating-point comparison — use exact equality since 1.0 is exactly representable. Also handle the degenerate case where bias is zero-sized? Not needed as per non-zero assumption. Time complexity: O(M*K*N) for mm, plus O(M*N) for add if shapes match, else O(M*N) with broadcasting still O(M*N). Space: O(M*N) for the result matrix of mm, and the final result same size.
