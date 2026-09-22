// Write a C++ function `multiply2x2` that takes two constant references to 2x2 matrices (represented as `std::array<std::array<int,2>,2>` or, for simplicity and direct compatibility with the given C-style code, as `const int A[2][2]` and `const int B[2][2]`) and returns a `std::array<std::array<int,2>,2>` containing the product computed using Strassen’s algorithm. The function must handle all integer inputs (positive, negative, zero) and must produce exactly the same result as conventional matrix multiplication. Do not modify the input matrices. The function should be self-contained, include all necessary headers, and be named `strassenMultiply`. Avoid using a `main` function in the solution.

Strassen’s algorithm reduces the number of multiplications for 2x2 matrices from 8 to 7 at the cost of extra additions/subtractions. The algorithm defines seven products `M1` through `M7` as linear combinations of the entries of `A` and `B`. Then each entry of the result `C` is a combination of these `M` values. The order of operations is critical: all intermediate values must be computed using integer arithmetic, and overflow is possible if inputs are large; for typical test cases within the `int` range, no overflow occurs under standard constraints. Edge cases include zero matrices, identical matrices, and matrices with negative numbers—Strassen’s formulas handle all uniformly. Time complexity is O(1) since the size is fixed (2x2), but conceptually it reduces multiplication count from 8 to 7 while increasing additions to 18 (vs 4 for naive). Space complexity is O(1) if we use a few local variables.

#include <array>

// Compute the product of two 2x2 matrices using Strassen's algorithm.
// A and B are input matrices, returns the product matrix C = A * B.
std::array<std::array<int, 2>, 2> strassenMultiply(const int A[2][2], const int B[2][2]) {
    int M1, M2, M3, M4, M5, M6, M7;
    
    M1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    M2 = (A[1][0] + A[1][1]) * B[0][0];
    M3 = A[0][0] * (B[0][1] - B[1][1]);
    M4 = A[1][1] * (B[1][0] - B[0][0]);
    M5 = (A[0][0] + A[0][1]) * B[1][1];
    M6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    M7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

    std::array<std::array<int, 2>, 2> C;
    C[0][0] = M1 + M4 - M5 + M7;
    C[0][1] = M3 + M5;
    C[1][0] = M2 + M4;
    C[1][1] = M1 - M2 + M3 + M6;
    return C;
}

#include <cassert>
#include <array>

// Declaration of the function under test (matching the solution above).
std::array<std::array<int, 2>, 2> strassenMultiply(const int A[2][2], const int B[2][2]);

int main() {
    // Test 1: identity matrices
    int A1[2][2] = {{1,0},{0,1}};
    int B1[2][2] = {{5,6},{7,8}};
    auto C1 = strassenMultiply(A1, B1);
    assert(C1[0][0] == 5 && C1[0][1] == 6 && C1[1][0] == 7 && C1[1][1] == 8);

    // Test 2: zero matrix
    int A2[2][2] = {{0,0},{0,0}};
    int B2[2][2] = {{1,2},{3,4}};
    auto C2 = strassenMultiply(A2, B2);
    assert(C2[0][0] == 0 && C2[0][1] == 0 && C2[1][0] == 0 && C2[1][1] == 0);

    // Test 3: negative numbers
    int A3[2][2] = {{1,-2},{3,4}};
    int B3[2][2] = {{-1,2},{0,5}};
    auto C3 = strassenMultiply(A3, B3);
    // Naive: C[0][0]=1*(-1)+(-2)*0=-1; C[0][1]=1*2+(-2)*5=2-10=-8; C[1][0]=3*(-1)+4*0=-3; C[1][1]=3*2+4*5=6+20=26
    assert(C3[0][0] == -1 && C3[0][1] == -8 && C3[1][0] == -3 && C3[1][1] == 26);

    // Test 4: both matrices with all non-zero entries
    int A4[2][2] = {{2,3},{4,5}};
    int B4[2][2] = {{6,7},{8,9}};
    auto C4 = strassenMultiply(A4, B4);
    // Naive: C[0][0]=2*6+3*8=12+24=36; C[0][1]=2*7+3*9=14+27=41; C[1][0]=4*6+5*8=24+40=64; C[1][1]=4*7+5*9=28+45=73
    assert(C4[0][0] == 36 && C4[0][1] == 41 && C4[1][0] == 64 && C4[1][1] == 73);

    // Test 5: swapped compared to test 1 (commutativity holds for any matrices, but check product)
    int A5[2][2] = {{5,6},{7,8}};
    int B5[2][2] = {{1,0},{0,1}};
    auto C5 = strassenMultiply(A5, B5);
    assert(C5[0][0] == 5 && C5[0][1] == 6 && C5[1][0] == 7 && C5[1][1] == 8);

    // Test 6: all ones
    int A6[2][2] = {{1,1},{1,1}};
    int B6[2][2] = {{1,1},{1,1}};
    auto C6 = strassenMultiply(A6, B6);
    assert(C6[0][0] == 2 && C6[0][1] == 2 && C6[1][0] == 2 && C6[1][1] == 2);

    // Test 7: large values to check integer arithmetic correctness (within int range)
    int A7[2][2] = {{100,200},{300,400}};
    int B7[2][2] = {{500,600},{700,800}};
    auto C7 = strassenMultiply(A7, B7);
    // C[0][0]=100*500+200*700=50000+140000=190000; C[0][1]=100*600+200*800=60000+160000=220000
    // C[1][0]=300*500+400*700=150000+280000=430000; C[1][1]=300*600+400*800=180000+320000=500000
    assert(C7[0][0] == 190000 && C7[0][1] == 220000 && C7[1][0] == 430000 && C7[1][1] == 500000);

    return 0;
}
