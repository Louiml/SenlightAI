Write a C++ function named `divideWithQuotientAndRemainder` that accepts two integer parameters, a dividend and a divisor (which must be non-zero), and returns a `std::pair<int, int>` containing the quotient first and the remainder second, using truncating integer division (C++ `/` and `%` operators). The function must guard against division by zero by throwing a `std::invalid_argument` exception with an appropriate message when the divisor is zero. The function must be `const`-correct (it does not modify its inputs) and must handle both positive and negative operands correctly: the quotient is truncated toward zero, and the remainder satisfies `(a / b) * b + (a % b) == a`. The function should be self-contained with necessary headers and include a concise comment describing its behavior. Do not include a `main` function.
#include <cassert>
#include <utility>
#include <stdexcept>

// Assume the solution function is declared above.

int main() {
    // Positive numbers
    auto result1 = divideWithQuotientAndRemainder(10, 3);
    assert(result1.first == 3);
    assert(result1.second == 1);

    // Dividend smaller than divisor
    auto result2 = divideWithQuotientAndRemainder(2, 5);
    assert(result2.first == 0);
    assert(result2.second == 2);

    // Negative dividend
    auto result3 = divideWithQuotientAndRemainder(-10, 3);
    assert(result3.first == -3); // truncated toward zero
    assert(result3.second == -1); // remainder has sign of dividend

    // Negative divisor
    auto result4 = divideWithQuotientAndRemainder(10, -3);
    assert(result4.first == -3);
    assert(result4.second == 1);

    // Both negative
    auto result5 = divideWithQuotientAndRemainder(-10, -3);
    assert(result5.first == 3);
    assert(result5.second == -1);

    // Exact division
    auto result6 = divideWithQuotientAndRemainder(15, 5);
    assert(result6.first == 3);
    assert(result6.second == 0);

    // Divisor of 1
    auto result7 = divideWithQuotientAndRemainder(7, 1);
    assert(result7.first == 7);
    assert(result7.second == 0);

    // Divisor of -1
    auto result8 = divideWithQuotientAndRemainder(7, -1);
    assert(result8.first == -7);
    assert(result8.second == 0);

    // Zero dividend (divisor non-zero)
    auto result9 = divideWithQuotientAndRemainder(0, 4);
    assert(result9.first == 0);
    assert(result9.second == 0);

    // Division by zero should throw
    bool threw = false;
    try {
        divideWithQuotientAndRemainder(5, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
#include <utility>   // for std::pair
#include <stdexcept> // for std::invalid_argument

// Returns the quotient and remainder of integer division of a by b.
// Throws std::invalid_argument if b is zero.
// Quotient is truncated toward zero; remainder satisfies (a/b)*b + (a%b) == a.
std::pair<int, int> divideWithQuotientAndRemainder(const int dividend, const int divisor) {
    if (divisor == 0) {
        throw std::invalid_argument("Division by zero is not allowed");
    }
    const int quotient = dividend / divisor;
    const int remainder = dividend % divisor;
    return std::make_pair(quotient, remainder);
}
// The core solution is straightforward: compute `quotient = a / b` and `remainder = a % b` directly using C++ integer arithmetic, which already implements truncation toward zero. The main algorithm is constant time: two arithmetic operations and one check. The primary edge case is `b == 0`, which would cause undefined behavior (usually a runtime error or crash); to handle it safely, we check the divisor first and throw `std::invalid_argument` if it is zero. For negative operands, C++ guarantees that `(a / b) * b + (a % b) == a` and that the quotient is truncated toward zero; the remainder will have the same sign as the dividend (e.g., `-7 / 2` gives quotient `-3`, remainder `-1`). This matches typical mathematical expectations for truncating division. Time complexity is \(O(1)\) and space complexity is \(O(1)\) since we only store two integers and return a pair (which may require a small constant allocation depending on the implementation, but effectively constant). The function is `const`-qualifiable because it does not mutate any state; we mark parameters as `const int` for clarity, but they are passed by value anyway.
