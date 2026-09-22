Write a C++ function named `classifyParity` that takes a non-negative integer `n` as input and returns a `std::string` containing `"odd"` if the number is odd, or `"even"` if the number is even. The function must handle the edge case where `n` equals 0 (which is even) and must not rely on any global state. The task is to implement only the function; no `main` or I/O should be included in the solution. The function should be correctly marked as `const` where appropriate (though the parameter is passed by value, the function itself has no side effects). The solution must compile with a C++17 compiler and include only necessary standard headers.
// The core algorithm is trivial: parity is determined by the remainder when dividing by 2. In C++, the modulo operator `%` works with integers, and for non-negative inputs, `n % 2` yields `0` for even numbers and `1` for odd numbers. The edge case `n = 0` naturally falls into the even category because `0 % 2 == 0`. The function returns a string literal directly, which is safe as it points to static storage. The function has no side effects, so we can declare it `const` (though for a free function, `const` is not meaningful; however, the parameter can be passed by value, and we can emphasize that we do not modify it. For correctness, we can simply use `int n` by value). Time complexity is O(1) constant time, and space complexity is O(1) since no auxiliary data structures are used. The only edge case is negative numbers—the task specifies non-negative, so we avoid handling negative modulo behavior in C++ (where `-1 % 2` is `-1`), which is why the constraint is important.
#include <string>

// Returns "odd" if n is odd, "even" otherwise (including n == 0).
std::string classifyParity(int n) {
    return (n % 2) ? "odd" : "even";
}
#include <cassert>
#include <string>

// Function under test (declared here for the test; typically included from header)
std::string classifyParity(int n) {
    return (n % 2) ? "odd" : "even";
}

int main() {
    assert(classifyParity(0) == "even");
    assert(classifyParity(1) == "odd");
    assert(classifyParity(2) == "even");
    assert(classifyParity(3) == "odd");
    assert(classifyParity(100) == "even");
    assert(classifyParity(101) == "odd");
    assert(classifyParity(999999) == "odd");
    assert(classifyParity(1000000) == "even");
    // Note: negative inputs are outside the task specification.
    return 0;
}
