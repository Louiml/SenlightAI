// Write a standalone C++ function named `evaluateLagrangeBasisAtPoint` that computes the value of the \(j\)-th Lagrange basis polynomial at a given evaluation point \(x\), using a fixed set of \(n\) interpolation nodes passed as a `const std::vector<double>&`. The function must take the evaluation point `xval`, the node index `j`, the vector of nodes `nodes`, and return the double result. It must assert that `j` is a valid index (i.e., `0 <= j < nodes.size()`) and that the nodes vector is non-empty. The formula to use is \(L_j(x) = \prod_{i=0, i \neq j}^{n-1} \frac{x - x_i}{x_j - x_i}\). The function must handle the case where `x` equals the value of node `j` correctly (it returns 1.0 because all factors become 1), and must handle the case where the evaluation point coincides with another node (then the product includes a factor 0, so the result is 0). Do not use any external quadrature library; implement the formula directly. The solution should be a free function with appropriate `const` correctness and include the `<cassert>` and `<vector>` headers.
// The solution directly implements the Lagrange basis formula. For each index `i` from 0 to `n-1`, skip the case where `i == j` because that term would be 0/0. For all other `i`, multiply the running product by `(xval - nodes[i]) / (nodes[j] - nodes[i])`. Important edge cases: (1) If `nodes` is empty, assert fails. (2) If `j` is out of range, assert fails. (3) If `xval` is exactly equal to `nodes[j]`, then every numerator equals the corresponding denominator, so the product is exactly 1.0. (4) If `xval` equals any other node `nodes[k]` where `k != j`, then the factor for `i = k` is 0, making the entire product 0, which is mathematically correct. (5) If there is only one node (`n=1`), then the product is empty (since we skip `i=j`), and the loop does not execute, returning the initial value 1.0—this is correct because with one point, the basis polynomial is constant 1. Time complexity is \(O(n)\) because we loop over all `n` nodes once, and space complexity is \(O(1)\) auxiliary, as we only use a single double accumulator.
#include <cassert>
#include <vector>

// Evaluate the j-th Lagrange basis polynomial at xval given nodes.
// Assumes nodes is non-empty and j is a valid index (0 <= j < nodes.size()).
double evaluateLagrangeBasisAtPoint(const double xval, const int j,
                                    const std::vector<double>& nodes) {
    // Validate input.
    assert(!nodes.empty());
    assert(j >= 0 && j < static_cast<int>(nodes.size()));

    const int npoints = static_cast<int>(nodes.size());

    // Start product at 1.0 (identity for multiplication).
    double Lvalue = 1.0;

    for (int i = 0; i < npoints; ++i) {
        if (i == j) {
            continue; // Skip the j-th term to avoid division by zero.
        }
        // Multiply by the ratio for term i.
        Lvalue *= (xval - nodes[i]) / (nodes[j] - nodes[i]);
    }

    return Lvalue;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Two nodes, evaluate at node 0 -> L0 = 1, L1 = 0
    std::vector<double> nodes1 = {0.0, 1.0};
    assert(evaluateLagrangeBasisAtPoint(0.0, 0, nodes1) == 1.0);
    assert(evaluateLagrangeBasisAtPoint(0.0, 1, nodes1) == 0.0);

    // Test 2: Three nodes, evaluate at a point not equal to any node.
    std::vector<double> nodes2 = {-1.0, 0.0, 1.0};
    double x = 0.5;
    double L0 = evaluateLagrangeBasisAtPoint(x, 0, nodes2);
    double L1 = evaluateLagrangeBasisAtPoint(x, 1, nodes2);
    double L2 = evaluateLagrangeBasisAtPoint(x, 2, nodes2);
    // Sum of basis polynomials at any x must be 1.
    assert(L0 + L1 + L2 == 1.0);

    // Test 3: Single node -> constant 1.
    std::vector<double> nodes3 = {3.14};
    assert(evaluateLagrangeBasisAtPoint(10.0, 0, nodes3) == 1.0);
    assert(evaluateLagrangeBasisAtPoint(3.14, 0, nodes3) == 1.0);

    // Test 4: Evaluate at another node (not j) gives 0.
    std::vector<double> nodes4 = {0.0, 2.0, 5.0};
    assert(evaluateLagrangeBasisAtPoint(2.0, 0, nodes4) == 0.0);
    assert(evaluateLagrangeBasisAtPoint(5.0, 1, nodes4) == 0.0);

    // Test 5: Evaluate at j itself gives 1 for any j.
    assert(evaluateLagrangeBasisAtPoint(2.0, 1, nodes4) == 1.0);
    assert(evaluateLagrangeBasisAtPoint(5.0, 2, nodes4) == 1.0);

    // Test 6: Hand-computed value with nodes {-2, 0, 3} at x=1, j=1 (node=0)
    // L1(1) = (1-(-2))/(0-(-2)) * (1-3)/(0-3) = (3/2) * (-2/-3) = (1.5)*(2/3)=1.0
    std::vector<double> nodes5 = {-2.0, 0.0, 3.0};
    assert(evaluateLagrangeBasisAtPoint(1.0, 1, nodes5) == 1.0);

    // Test 7: Negative nodes and x, correctness via interpolation identity.
    std::vector<double> nodes6 = {-5.0, -1.0, 2.0, 4.0};
    double sum = 0.0;
    double testX = -3.0;
    for (int j = 0; j < 4; ++j) {
        sum += evaluateLagrangeBasisAtPoint(testX, j, nodes6);
    }
    assert(sum == 1.0);

    return 0;
}
