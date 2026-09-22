Write a C++ function `parseAndAddComplex` that accepts two strings, each representing a complex number in the format `a+ib` (where `a` and `b` are non-negative integers, and there may be spaces around the `+` and `i` symbols, e.g., `"3 + i4"`). The function must return a string representing the sum of the two complex numbers in the exact format `a+ib` (with no spaces, and where `a` and `b` are the sums of the real and imaginary parts respectively). The input strings are guaranteed to be valid in the given format, but may contain leading/trailing spaces or multiple spaces between tokens. The function must not use `iostream` extraction for parsing; instead, manually parse the digits from the strings. Your implementation must be standalone (no `main` function), and include any necessary headers. The function should handle edge cases such as numbers with multiple digits and ensure correct summation even when parts are zero.

#include <cassert>
#include <string>

// The solution function is declared in the global scope (include the solution code above this).
std::string parseAndAddComplex(const std::string&, const std::string&);

int main() {
    // Basic addition
    assert(parseAndAddComplex("1+2i", "3+4i") == "4+6i");
    // With spaces
    assert(parseAndAddComplex("1 + i2", "3 + i4") == "4+6i");
    // Multiple digits and zero imaginary parts
    assert(parseAndAddComplex("10+0i", "20+0i") == "30+0i");
    // Leading/trailing spaces and multiple spaces between tokens
    assert(parseAndAddComplex("  5  +  i 7 ", " 1 + i 2 ") == "6+9i");
    // Imaginary parts sum to zero
    assert(parseAndAddComplex("1+3i", "2+3i") == "3+6i"); // not zero, just checking
    assert(parseAndAddComplex("1+2i", "2+2i") == "3+4i");
    // Real parts sum to zero
    assert(parseAndAddComplex("0+1i", "0+2i") == "0+3i");
    // Large digits
    assert(parseAndAddComplex("123+456i", "877+544i") == "1000+1000i");
    // All zeros
    assert(parseAndAddComplex("0+0i", "0+0i") == "0+0i");
    // More spaces around plus and i
    assert(parseAndAddComplex("7+ i8", "2+i3") == "9+11i");
    // Same input twice
    assert(parseAndAddComplex("4+5i", "4+5i") == "8+10i");
}

#include <string>

// Helper to parse a complex string of form "a+ib" (with optional spaces) into a pair (real, imag).
static std::pair<int, int> parseComplex(const std::string& s) {
    int real = 0;
    size_t i = 0;
    // Read real part until '+'
    while (i < s.length() && s[i] != '+') {
        real = real * 10 + (s[i] - '0');
        i++;
    }
    // Skip spaces, the '+' and 'i' (the snippet skips until after 'i')
    while (i < s.length() && (s[i] == ' ' || s[i] == '+' || s[i] == 'i')) {
        i++;
    }
    int imag = 0;
    while (i < s.length()) {
        imag = imag * 10 + (s[i] - '0');
        i++;
    }
    return {real, imag};
}

// Parses two complex strings, sums them, and returns the result as a string "sumReal+iSumImag".
std::string parseAndAddComplex(const std::string& s1, const std::string& s2) {
    auto c1 = parseComplex(s1);
    auto c2 = parseComplex(s2);
    int sumReal = c1.first + c2.first;
    int sumImag = c1.second + c2.second;
    return std::to_string(sumReal) + "+i" + std::to_string(sumImag);
}

// The main algorithm involves manually parsing each string to extract the real part (`a`) and imaginary part (`b`). The `input` logic from the snippet uses a character-by-character scan: first, it reads digits until it encounters a `'+'` character to compute the real part; then it skips any spaces, the `'+'`, and the `'i'` character; finally, it reads the remaining digits to compute the imaginary part. This parsing works because the format is strictly `digits+ spaces? + spaces? i digits` (though the snippet’s skipping also handles spaces after the `i`). For the solution function, we can replicate this parsing logic twice (once for each input string), or better, create a helper that parses a single string into a pair of integers. After obtaining `(a1,b1)` and `(a2,b2)`, compute `sumReal = a1 + a2` and `sumImag = b1 + b2`. Then format the result as `to_string(sumReal) + "+i" + to_string(sumImag)`. Edge cases: numbers may have multiple digits (the parsing loop handles that), the imaginary part may be `0` (still output `+i0`), and there may be extra spaces that are skipped correctly. The parser must correctly identify the first `'+'` as the separator (since there is no sign before the real part). The algorithm runs in O(len(s1)+len(s2)) time and uses O(1) auxiliary space (excluding the returned string and the string-conversion temporary). The solution must be `const`-correct: pass strings by `const std::string&` and only read from them.
