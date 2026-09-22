// Write a C++ function that returns a vector of all perfect squares (integers that are the square of another integer) between 1 and 1000 inclusive, sorted in ascending order. The function must be named `perfectSquaresUpTo1000` and should return `std::vector<int>`. Additionally, the function must not use any loops that continue past the upper bound (i.e., it should stop checking once `i*i > 1000`), and it must ensure that no value outside [1, 1000] is included even if the loop condition is slightly off. Demonstrate the function’s correctness by testing that it returns exactly 31 values (since 31²=961 and 32²=1024 > 1000), that the first value is 1, the last is 961, and that every element is positive, strictly increasing, and a perfect square.

// The straightforward algorithm iterates over candidate integers `i` starting at 1 and computes `square = i * i`. Since we want squares ≤ 1000, the largest possible `i` is 31 because 31²=961 ≤ 1000 and 32²=1024 > 1000. We can loop `i` from 1 upward, compute `square` each iteration, and break immediately when `square > 1000` to avoid unnecessary work. For each valid square, we append it to a result vector. Edge cases: the loop must not include 0 (since 0 is not in the range 1–1000) and must not include 1024 or any value >1000. Because we break early, we never exceed 1000. We do not need to check for duplicates since each `i` gives a unique square. Time complexity is O(√1000) = O(1) effectively, as we perform at most 31 iterations. Space complexity is O(31) = O(1) for storing the output vector.

#include <vector>

// Returns all perfect squares between 1 and 1000 inclusive, in ascending order.
std::vector<int> perfectSquaresUpTo1000() {
    std::vector<int> result;
    for (int i = 1; ; ++i) {
        const int square = i * i;
        if (square > 1000) {
            break;
        }
        result.push_back(square);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test that the function returns exactly 31 values.
    std::vector<int> squares = perfectSquaresUpTo1000();
    assert(squares.size() == 31);

    // Test first and last values.
    assert(squares.front() == 1);
    assert(squares.back() == 961);

    // Test that all values are positive, strictly increasing, and perfect squares.
    for (size_t i = 0; i < squares.size(); ++i) {
        assert(squares[i] > 0);
        if (i > 0) {
            assert(squares[i] > squares[i - 1]);
        }
        int root = static_cast<int>(std::sqrt(squares[i]));
        assert(root * root == squares[i]);
    }

    // Test specific known values.
    assert(squares[0] == 1);
    assert(squares[10] == 121); // 11^2
    assert(squares[30] == 961); // 31^2

    // Test that no value exceeds 1000.
    assert(squares.back() <= 1000);
}
