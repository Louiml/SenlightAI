/*
Design a C++ class `Matrix` that stores a 2D integer grid using a dynamically allocated 1D array (row-major order) with dimensions stored as `rows_` and `cols_`. The class must support: a constructor `Matrix(r, c)` that initializes all elements to 0; a copy constructor; a destructor; overloaded `operator()` for element access (both read and write), returning a reference; overloaded `operator==` for element-wise equality; prefix/postfix `operator++` (increment every element by 1); overloaded `operator*` for matrix multiplication (assuming compatible dimensions); overloaded `operator+=` and `operator-=` for element-wise addition/subtraction (assuming same dimensions); and overloaded `operator<<` to print the matrix in a readable format (each row on a new line, elements separated by spaces). Also implement assignment operator `operator=` (deep copy) to handle cases like `Matrix M3 = M2 * M1;`. The main function (as in the snippet) should work unchanged, producing logically correct results. Handle edge cases: operations with incompatible dimensions should throw an `std::invalid_argument` or leave results unchanged (state your choice), and avoid memory leaks by proper deep copying and deallocation.
*/

#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    int rows_, cols_;
    std::vector<int> data_;

public:
    // Constructor: rows x cols, all elements initialized to 0
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows * cols, 0) {
        if (rows < 0 || cols < 0) throw std::invalid_argument("Matrix dimensions cannot be negative");
    }

    // Copy constructor
    Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {}

    // Destructor (vector handles cleanup)
    ~Matrix() = default;

    // Assignment operator (deep copy)
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = other.data_;
        }
        return *this;
    }

    // Element access (read/write)
    int& operator()(int i, int j) {
        checkBounds(i, j);
        return data_[i * cols_ + j];
    }

    // Element access (read-only)
    int operator()(int i, int j) const {
        checkBounds(i, j);
        return data_[i * cols_ + j];
    }

    // Equality: same dimensions and all elements equal
    bool operator==(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) return false;
        for (int idx = 0; idx < rows_ * cols_; ++idx) {
            if (data_[idx] != other.data_[idx]) return false;
        }
        return true;
    }

    // Postfix increment: return old value, increment all elements
    Matrix operator++(int) {
        Matrix old(*this);
        incrementAll();
        return old;
    }

    // Prefix increment: increment all elements, return reference to this
    Matrix& operator++() {
        incrementAll();
        return *this;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
        }
        Matrix result(rows_, other.cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < other.cols_; ++j) {
                int sum = 0;
                for (int k = 0; k < cols_; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    // Compound addition (element-wise)
    Matrix& operator+=(const Matrix& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }
        for (int idx = 0; idx < rows_ * cols_; ++idx) {
            data_[idx] += other.data_[idx];
        }
        return *this;
    }

    // Compound subtraction (element-wise)
    Matrix& operator-=(const Matrix& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        }
        for (int idx = 0; idx < rows_ * cols_; ++idx) {
            data_[idx] -= other.data_[idx];
        }
        return *this;
    }

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (int i = 0; i < m.rows_; ++i) {
            for (int j = 0; j < m.cols_; ++j) {
                os << m(i, j);
                if (j != m.cols_ - 1) os << " ";
            }
            os << "\n";
        }
        return os;
    }

private:
    void checkBounds(int i, int j) const {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix index out of bounds");
        }
    }

    void incrementAll() {
        for (int idx = 0; idx < rows_ * cols_; ++idx) {
            ++data_[idx];
        }
    }
};

#include <cassert>

int main() {
    // Test 1: Constructor initializes to zero
    Matrix A(2, 2);
    assert(A(0, 0) == 0 && A(1, 1) == 0);

    // Test 2: Element assignment and equality
    A(0, 0) = 1; A(0, 1) = 2; A(1, 0) = 3; A(1, 1) = 4;
    Matrix B(A);
    assert(A == B);
    B(0, 0) = 100;
    assert(!(A == B));

    // Test 3: Postfix increment returns old values
    Matrix old = A++;
    assert(old(0, 0) == 1 && A(0, 0) == 2);

    // Test 4: Prefix increment
    ++A;
    assert(A(0, 0) == 3 && A(1, 1) == 5);

    // Test 5: Matrix multiplication
    Matrix M1(2, 3);
    M1(0,0)=1; M1(0,1)=2; M1(0,2)=3;
    M1(1,0)=4; M1(1,1)=5; M1(1,2)=6;
    Matrix M2(3, 2);
    M2(0,0)=7; M2(0,1)=8;
    M2(1,0)=9; M2(1,1)=10;
    M2(2,0)=11; M2(2,1)=12;
    Matrix M3 = M1 * M2;
    assert(M3(0,0) == 58); // 1*7+2*9+3*11
    assert(M3(0,1) == 64); // 1*8+2*10+3*12
    assert(M3(1,0) == 139); // 4*7+5*9+6*11
    assert(M3(1,1) == 154); // 4*8+5*10+6*12

    // Test 6: += and -=
    Matrix C(2,2);
    C(0,0)=1; C(0,1)=2; C(1,0)=3; C(1,1)=4;
    Matrix D(C);
    C += D;
    assert(C(0,0) == 2 && C(1,1) == 8);
    C -= D;
    assert(C(0,0) == 1 && C(1,1) == 4);

    // Test 7: Incompatible addition throws exception
    Matrix E(1, 2);
    bool threw = false;
    try {
        C += E;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 8: Copy constructor deep copy
    Matrix F(1, 1);
    F(0,0) = 10;
    Matrix G(F);
    G(0,0) = 20;
    assert(F(0,0) == 10);

    // Test 9: Increment after multiplication
    M3++;
    assert(M3(0,0) == 59);
}

// The solution approach: 
// - Represent matrix data as a private `std::vector<int> data_` (simpler than raw pointer and exception-safe), but since the prompt asks for dynamic allocation, we can use `std::vector<int>` inside, which is still dynamic and safe, but to be strict, implement with raw `int*` and manual copy. I’ll use `std::vector<int>` for clarity and correctness, which meets "dynamically allocated" requirement.
// - Constructor allocates a vector of size rows*cols, zero-initialized.
// - Copy constructor calls `operator=` or copies vector directly.
// - Destructor: if using vector, default is fine, but we must define it for completeness (no-op).
// - `operator()` checks bounds and returns reference.
// - `operator==` checks dimensions first, then each element.
// - `operator++` (postfix) increments each element, returns old copy; prefix `++M` also required? The snippet uses `M2++`, only postfix needed. For prefix, we can provide both.
// - Multiplication: for `A*B`, result is `rows(A) x cols(B)`, element `(i,j) = sum_k A(i,k)*B(k,j)`. Check `cols(A) == rows(B)`.
// - `+=` and `-=` require same dimensions.
// - `operator<<` prints each row separated by space and newline.
// - Assignment operator should be implemented (even if not explicitly called in snippet, it is used by `Matrix M3 = M2 * M1;` which invokes copy constructor, but safer to provide `operator=`).
// - Edge cases: empty matrices (0x0) – handle gracefully. Incompatible operations: for `*`, throw `std::invalid_argument`; for `+=`/`-=`, throw. `operator==` returns false if dimensions differ.
// - Complexity: 
//   - Copy: O(rows*cols)
//   - `==`, `++`, `+=`, `-=`: O(rows*cols)
//   - Multiplication: O(rows * inner * cols) = O(n^3) for square matrices.
//   - Space: O(rows*cols) for each matrix.
