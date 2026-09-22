/*
Write a C++ function named `computeStats` that takes three integers as parameters: two input values `a` and `b`, and a third integer `op` representing an operation code (1 = sum, 2 = maximum, 3 = minimum). The function must return a `std::pair<int, std::string>` where the first element is the computed result and the second is a descriptive message such as "sum", "maximum", or "minimum". If `op` is not 1, 2, or 3, return `{0, "invalid"}`. The function must use pass-by-reference for outputs internally (as in the original snippet) but the public interface returns a pair. Handle edge cases like duplicate values and negative numbers correctly, and ensure the function is `const`-correct by taking inputs by value (since they are not modified) and returning the result by value.
*/

#include <utility>
#include <string>

namespace {
    // Internal helper: computes sum a1 + b1 and stores in add (pass by reference)
    void sumHelper(int a1, int b1, int &add) {
        add = a1 + b1;
    }
    // Internal helper: computes maximum and stores in high (pass by reference)
    void maxHelper(int a1, int b1, int &high) {
        if (a1 > b1) high = a1;
        else high = b1;
    }
    // Internal helper: computes minimum and stores in low (pass by reference)
    void minHelper(int a1, int b1, int &low) {
        if (a1 < b1) low = a1;
        else low = b1;
    }
}

// Public function: returns {result, description} based on op code
std::pair<int, std::string> computeStats(int a, int b, int op) {
    int result = 0;
    switch (op) {
        case 1:
            sumHelper(a, b, result);
            return {result, "sum"};
        case 2:
            maxHelper(a, b, result);
            return {result, "maximum"};
        case 3:
            minHelper(a, b, result);
            return {result, "minimum"};
        default:
            return {0, "invalid"};
    }
}

#include <cassert>
#include <string>
#include <utility>

int main() {
    auto res = computeStats(10, 5, 1);
    assert(res.first == 15 && res.second == "sum");

    res = computeStats(10, 5, 2);
    assert(res.first == 10 && res.second == "maximum");

    res = computeStats(10, 5, 3);
    assert(res.first == 5 && res.second == "minimum");

    res = computeStats(-3, -7, 1);
    assert(res.first == -10 && res.second == "sum");

    res = computeStats(-3, -7, 2);
    assert(res.first == -3 && res.second == "maximum");

    res = computeStats(-3, -7, 3);
    assert(res.first == -7 && res.second == "minimum");

    // duplicate values
    res = computeStats(4, 4, 2);
    assert(res.first == 4 && res.second == "maximum");
    res = computeStats(4, 4, 3);
    assert(res.first == 4 && res.second == "minimum");

    // invalid op
    res = computeStats(2, 3, 99);
    assert(res.first == 0 && res.second == "invalid");
}

// The solution approach mirrors the original snippet's logic but refactors it into a single function that internally uses three helper functions (each taking the two inputs and a reference to an output variable) to compute sum, max, and min. The main algorithm checks the `op` code in a switch-like structure: if `op == 1`, call the internal sum helper; if `op == 2`, call max helper; if `op == 3`, call min helper; otherwise, return the invalid pair. Each helper performs a simple comparison or addition, which is O(1) time. Edge cases include: negative numbers are handled naturally by comparisons, duplicates are handled by using `>=` or `<=` appropriately (but since we only assign `high = a1` if `a1 > b1` and else `high = b1`, duplicates of equal values correctly assign `b1`; for minimum, if `a1 < b1` assign `a1` else `b1` – also correct for duplicates). The overall time complexity is O(1) for any operation, and space complexity is O(1) aside from the returned pair's string.
