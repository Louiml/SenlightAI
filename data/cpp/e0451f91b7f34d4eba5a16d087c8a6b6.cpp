// Given a string of whitespace-separated floating-point numbers (which may include negatives and duplicates) that represents a prediction model, write a C++ function `std::vector<double> parsePredictionLC(const std::string& prediction)` that parses the string into a vector of doubles and validates that each parsed value is in the inclusive range `[0.0, 1.0]`. If the input string has fewer than two numbers, or any number is outside the valid range, return an empty vector. Otherwise, return the parsed vector in the same order as the input. The function must handle leading/trailing whitespace, multiple spaces between numbers, and gracefully ignore extra whitespace. Do not rely on any external libraries beyond the standard C++ library (e.g., `<sstream>`, `<string>`, `<vector>`).
// The solution uses `std::istringstream` to tokenize the input string by whitespace. We read one `double` at a time; if extraction fails (e.g., the string is empty or contains non-numeric tokens), return an empty vector immediately. We track the count of successfully parsed values; if after reading all tokens the count is less than 2, return empty. For each parsed value, check that it is not less than 0.0 and not greater than 1.0; if any value violates this, return an empty vector. The vector is built by pushing each valid value. Time complexity is \(O(n)\) where \(n\) is the number of tokens, and space complexity is \(O(n)\) for the output vector plus the string stream overhead. Edge cases: empty input, single value, values exactly at 0.0 or 1.0 (valid), values slightly outside (e.g., 1.0001), and non-numeric tokens (handled by extraction failure).
#include <sstream>
#include <string>
#include <vector>

/**
 * Parse a whitespace-separated string of floating-point values, each in [0.0, 1.0].
 * The input must contain at least two values; otherwise, returns an empty vector.
 * Returns an empty vector if any token is not a valid number or is out of range.
 */
std::vector<double> parsePredictionLC(const std::string& prediction) {
    std::istringstream input(prediction);
    std::vector<double> values;
    double value;
    
    while (input >> value) {
        if (value < 0.0 || value > 1.0) {
            return std::vector<double>();
        }
        values.push_back(value);
    }
    
    // Check that the stream extraction succeeded for at least two values.
    // If the stream failed before reading any value (e.g., empty string),
    // values.size() will be zero, which is < 2.
    return (values.size() >= 2) ? values : std::vector<double>();
}
#include <cassert>
#include <vector>

int main() {
    // Basic valid case with multiple spaces and leading/trailing whitespace.
    std::vector<double> r1 = parsePredictionLC("  0.1   0.2  0.3  ");
    assert(r1.size() == 3);
    assert((r1[0] == 0.1 && r1[1] == 0.2 && r1[2] == 0.3));

    // Exactly two values including boundaries.
    std::vector<double> r2 = parsePredictionLC("0.0 1.0");
    assert(r2.size() == 2);
    assert(r2[0] == 0.0 && r2[1] == 1.0);

    // Negative value is invalid.
    assert(parsePredictionLC("0.5 -0.1").empty());

    // Value greater than 1 is invalid.
    assert(parsePredictionLC("1.1 0.5").empty());

    // Fewer than two values.
    assert(parsePredictionLC("0.5").empty());

    // Empty string.
    assert(parsePredictionLC("").empty());

    // Non-numeric token causes extraction to stop early; if fewer than two valid tokens.
    assert(parsePredictionLC("0.1 abc 0.2").empty());

    // Duplicate values and many tokens.
    std::vector<double> r7 = parsePredictionLC("0.5 0.5 0.25 1.0 0.0");
    assert(r7.size() == 5);
    assert(r7[3] == 1.0 && r7[4] == 0.0);

    return 0;
}
