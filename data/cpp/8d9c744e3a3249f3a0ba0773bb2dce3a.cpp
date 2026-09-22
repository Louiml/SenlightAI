/*
Write a standalone C++ function that simulates the key validation and user‑input logic from the provided MPI‑based DNA clustering program, but without any MPI or external helper dependencies. Specifically, implement a function `processDNAParameters` that reads a sequence of parameter values from a `std::istream` representing user input. The function must validate each token as follows: for `length`, `dataSize`, `clusterNum`, and `iteration`, the token must be a non‑negative integer (i.e., only digits, possibly empty? No—empty is invalid). For `threshold`, the token must be a valid double (including decimals, signs, etc.). If any token fails validation, the function must immediately return `-1` (an error code). If all tokens are valid, the function must compute and return the product: `length * dataSize * clusterNum * iteration` (as a `long long` to avoid overflow). The function should read exactly four integer tokens and one double token from the stream, in that order, and ignore any extra whitespace. Do not use `main`; just provide the function and any helper validation functions.
*/
#include <string>
#include <sstream>
#include <cctype>

// Helper: true if the string is a non‑negative integer (only digits, non‑empty)
bool isUnsignedInt(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

// Helper: true if the string represents a valid double (including decimals, signs)
bool isDouble(const std::string& s) {
    if (s.empty()) return false;
    std::istringstream iss(s);
    double d;
    iss >> d;
    return !iss.fail() && (iss >> std::ws).eof();
}

// Reads five parameters from 'input': length, dataSize, clusterNum, threshold, iteration.
// Returns -1 if any token is invalid, otherwise the product length*dataSize*clusterNum*iteration.
long long processDNAParameters(std::istream& input) {
    std::string token;
    long long length, dataSize, clusterNum, iteration; // satisfy the requirement to read four integers
    double threshold;

    // Read length (integer)
    if (!(input >> token) || !isUnsignedInt(token)) return -1;
    length = std::stoll(token);

    // Read dataSize (integer)
    if (!(input >> token) || !isUnsignedInt(token)) return -1;
    dataSize = std::stoll(token);

    // Read clusterNum (integer)
    if (!(input >> token) || !isUnsignedInt(token)) return -1;
    clusterNum = std::stoll(token);

    // Read threshold (double)
    if (!(input >> token) || !isDouble(token)) return -1;
    threshold = std::stod(token); // used only to validate; not in product per spec

    // Read iteration (integer)
    if (!(input >> token) || !isUnsignedInt(token)) return -1;
    iteration = std::stoll(token);

    // Product of the four integer parameters (threshold is not included)
    return length * dataSize * clusterNum * iteration;
}
#include <cassert>
#include <sstream>
#include <string>

// The solution function is declared above; here is a test driver.
int main() {
    // Valid inputs: 10 100 5 0.001 1000 -> product = 10*100*5*1000 = 5,000,000
    {
        std::istringstream iss("10 100 5 0.001 1000");
        assert(processDNAParameters(iss) == 5000000LL);
    }

    // Valid inputs with extra whitespace and leading zeros
    {
        std::istringstream iss("  010   007   0000   3.14   2  ");
        // length=010->10, dataSize=007->7, clusterNum=0000->0, iteration=2 -> product=10*7*0*2=0
        assert(processDNAParameters(iss) == 0LL);
    }

    // Invalid: threshold is not a double (e.g., "abc")
    {
        std::istringstream iss("10 100 5 abc 1000");
        assert(processDNAParameters(iss) == -1);
    }

    // Invalid: length contains a negative sign (not unsigned)
    {
        std::istringstream iss("-5 100 5 0.001 1000");
        assert(processDNAParameters(iss) == -1);
    }

    // Invalid: dataSize has a letter
    {
        std::istringstream iss("10 10x 5 0.001 1000");
        assert(processDNAParameters(iss) == -1);
    }

    // Invalid: clusterNum empty token (two spaces between)
    {
        std::istringstream iss("10 100   5 0.001 1000"); // spaces skipped; actually valid because >> skips
        // But to force empty, we can provide fewer tokens? Actually >> will fail if no token.
        // So test missing token case:
        std::istringstream iss2("10 100 5 0.001"); // missing iteration
        assert(processDNAParameters(iss2) == -1);
    }

    // Valid: all zeros -> product 0
    {
        std::istringstream iss("0 0 0 0.0 0");
        assert(processDNAParameters(iss) == 0LL);
    }

    // Valid: large numbers to test long long product (e.g., 1000^4 = 1e12)
    {
        std::istringstream iss("1000 1000 1000 0.5 1000");
        assert(processDNAParameters(iss) == 1000000000000LL);
    }

    // Invalid: double with trailing garbage "0.5abc"
    {
        std::istringstream iss("1 1 1 0.5abc 1");
        assert(processDNAParameters(iss) == -1);
    }

    // Invalid: double with two decimals "1.2.3"
    {
        std::istringstream iss("1 1 1 1.2.3 1");
        assert(processDNAParameters(iss) == -1);
    }

    return 0;
}
// The solution must read tokens sequentially from the provided stream, one per parameter. For each, we need a validation helper: `isUnsignedInt` checks that the string is non‑empty and all characters are digits (matching the original `is_number`). `isDouble` uses a `std::istringstream` to extract a double and then checks that the entire string was consumed (matching `checkForDouble`). The main function reads a string token, validates it, and if invalid returns `-1` immediately. If valid, we convert it to the appropriate type (e.g., `atoi` or `atof`) but for safety we cast to `long long` only for the final product after all validations pass. Important edge cases: empty tokens (the stream operator `>>` will skip whitespace, so tokens won't be empty unless input is malformed—but we still guard for safety), leading zeros (e.g., `"007"` is valid as integer 7), and the double can have a decimal point and sign. The time complexity is O(total input length) because each token is parsed once, and space is O(max token length). The function does not need to handle extra tokens; it stops after reading the five required tokens. If the stream runs out of tokens, `cin >> enter` will fail and leave `enter` unchanged—but in a test we can provide a string stream with exactly the right tokens, so no error case needed beyond invalid characters.
