// Write a C++ function named `RotateRight3` that takes three integer parameters by reference and cyclically shifts their values one position to the right: the original value of the third parameter moves to the first, the original first moves to the second, and the original second moves to the third. The function must modify the original variables passed by the caller and return `void`. The task is to implement this function so that when the provided `main` reads three integers from standard input and calls `RotateRight3(n1, n2, n3)`, the output prints the three integers in their rotated order separated by spaces.
// The rotation is a simple cyclic permutation of three integers. The algorithm must preserve the original values before overwriting them, because a naive assignment like `n1 = n3; n2 = n1; n3 = n2;` would lose data (after `n1 = n3`, `n1` no longer holds the original first value). Instead, store the original first value in a temporary variable, then assign `n1 = n3; n2 = temp; n3 = n2_original` — but note `n2` gets overwritten, so also save it. More simply, use three temporaries or a `std::tuple`. The cleanest approach: `int old1 = n1, old2 = n2, old3 = n3; n1 = old3; n2 = old1; n3 = old2;`. Edge cases: no special cases; duplicates are fine. Time complexity: O(1) constant. Space complexity: O(1) auxiliary.
#include <utility>

// Rotate three integers to the right: (a,b,c) becomes (c,a,b).
void RotateRight3(int& a, int& b, int& c) {
    int original_a = a;
    int original_b = b;
    int original_c = c;
    
    a = original_c;
    b = original_a;
    c = original_b;
}
#include <cassert>

int main() {
    // Test 1: Normal rotation
    int a = 1, b = 2, c = 3;
    RotateRight3(a, b, c);
    assert(a == 3 && b == 1 && c == 2);

    // Test 2: All equal values
    int d = 5, e = 5, f = 5;
    RotateRight3(d, e, f);
    assert(d == 5 && e == 5 && f == 5);

    // Test 3: Negative numbers
    int g = -1, h = -2, i = -3;
    RotateRight3(g, h, i);
    assert(g == -3 && h == -1 && i == -2);

    // Test 4: Zero and mixed signs
    int j = 0, k = 10, l = -10;
    RotateRight3(j, k, l);
    assert(j == -10 && k == 0 && l == 10);

    // Test 5: Rotate twice returns one step further right
    int m = 1, n = 2, o = 3;
    RotateRight3(m, n, o);
    RotateRight3(m, n, o);
    assert(m == 2 && n == 3 && o == 1);

    // Test 6: Rotate three times returns original order
    int p = 7, q = 8, r = 9;
    RotateRight3(p, q, r);
    RotateRight3(p, q, r);
    RotateRight3(p, q, r);
    assert(p == 7 && q == 8 && r == 9);

    return 0;
}
