Write a C++ function `std::string compareComplexNumbers(const std::string& line1, const std::string& line2)` that takes two strings, each containing two integers separated by whitespace (representing the real and imaginary parts of a complex number), parses them, and returns a string describing the result of adding the two complex numbers. The output must be formatted as `"R+iI"` where `R` is the sum of the real parts and `I` is the sum of the imaginary parts, with no spaces, and if the imaginary part is negative (or zero), it still appears as `+i-3` or `+i0` per the original snippet's style. The input is guaranteed to have exactly two integers per line, possibly with extra whitespace, but the integers may be negative, zero, or positive. The function must be robust and use `const` references.

#include <cassert>
#include <string>

// Function declaration from the solution (for completeness).
std::string compareComplexNumbers(const std::string& line1, const std::string& line2);

int main() {
    // Basic positive numbers
    assert(compareComplexNumbers("1 2", "3 4") == "4+i6");
    // Negative real parts
    assert(compareComplexNumbers("-5 10", "2 -3") == "-3+i7");
    // Zero values
    assert(compareComplexNumbers("0 0", "0 0") == "0+i0");
    // Negative imaginary sum
    assert(compareComplexNumbers("1 1", "-2 -3") == "-1+i-2");
    // Extra whitespace
    assert(compareComplexNumbers("  5   6  ", "  -1   -2  ") == "4+i4");
    // Mixed signs
    assert(compareComplexNumbers("-3 -4", "10 20") == "7+i16");
    // Large integers (within int range)
    assert(compareComplexNumbers("100000 200000", "-50000 30000") == "50000+i230000");
    // Repeated minus signs (valid integers)
    assert(compareComplexNumbers("-0 5", "0 -5") == "0+i0");
    // Leading/trailing whitespace with tabs
    assert(compareComplexNumbers("\t1\t2\t", " 3 4 ") == "4+i6");
    // Single-digit and multi-digit combinations
    assert(compareComplexNumbers("9 1", "1 9") == "10+i10");
    return 0;
}

#include <string>
#include <sstream>

// Parse two integers from a whitespace-separated string and return their sum as a formatted complex number.
std::string compareComplexNumbers(const std::string& line1, const std::string& line2) {
    int real1, imag1, real2, imag2;
    std::istringstream stream1(line1);
    std::istringstream stream2(line2);
    
    stream1 >> real1 >> imag1;
    stream2 >> real2 >> imag2;
    
    int sumReal = real1 + real2;
    int sumImag = imag1 + imag2;
    
    return std::to_string(sumReal) + "+i" + std::to_string(sumImag);
}

// The task is straightforward: parse two integers from each input string using `std::istringstream`, compute the sums of the real and imaginary parts, and return a formatted string. Edge cases include negative numbers (they parse fine with `operator>>`), extra whitespace (handled by the stream skipping whitespace), and zero values (still formatted with a plus sign before `i`). The main algorithm: for each line, read two integers into local variables, then add them. Time complexity is O(1) as only two fixed-size lines are processed; space complexity is O(1) for the integers plus O(1) for the returned string, which is constant relative to input size since input size is fixed at two integers per line.
