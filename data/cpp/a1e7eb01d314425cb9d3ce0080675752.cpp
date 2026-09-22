// Design a C++ function that reads a sequence of tokens from a string, where tokens are either non-negative integer literals (e.g., "0", "42", "123") or single-character operators from the set `+ - * / ^ > < =`. The function must emulate the behavior of the provided code snippet: maintain two "long number" values `a` (initially 0) and `b` (initially 0). When an integer token is read, shift the previous `b` into `a` and set `b` to that integer. When an operator token is read, perform the corresponding operation on the current `a` and `b` (with `^` meaning exponentiation, and comparison operators returning "true"/"false"), and output the result as a string. All arithmetic operations must use arbitrary-precision integer arithmetic (i.e., standard 64-bit integers are insufficient; you must implement or use a big-integer representation). For subtraction, if `a < b`, output "Error". For division, if `b == 0`, output "Error". For exponentiation, if both `a == 0` and `b == 0`, output "Error". For any unknown operator, output "Error". The resulting function takes a single `std::string` input (the token stream) and returns a `std::string` containing each result on its own line, in the order they are produced. Ignore whitespace between tokens. The input may contain leading/trailing whitespace and any number of tokens. You must implement a big-integer class or use a vector-based representation to handle arbitrarily large integers (e.g., numbers with up to 1000 digits). The solution must be self-contained (no external libraries beyond standard headers).
// The main challenge is implementing arbitrary-precision arithmetic for non-negative integers (since input is non-negative and operations like subtraction are only performed when `a >= b`). We can represent a big integer as a `std::string` of decimal digits (most significant first) and implement comparison, addition, subtraction (non-negative result), multiplication, division (integer division, non-negative), and exponentiation. Exponentiatiation can be done via fast exponentiation (binary exponentiation) using multiplication, but since the exponent `b` itself is a big integer, we need to handle it as a decimal string: iterate over its digits, squaring the base each time and multiplying when the current digit is odd, processing from the most significant digit (like manual exponentiation). All operations must handle leading zeros and produce canonical output (no leading zeros except "0"). For division, use long division one digit at a time. The overall algorithm processes tokens sequentially: maintain `a` and `b` as big integers. For each integer token, shift `a = b`, `b = token`. For each operator, compute result and append to output string. Edge cases: empty input (return empty string), operators at the very beginning (operate on default 0 and 0), multiple operators in a row (each uses the same `a` and `b`). Time complexity: each arithmetic operation on numbers with up to `d` digits takes `O(d^2)` for multiplication/division (using naive algorithms) or `O(d * log exponent)` for exponentiation via repeated squaring. Overall, for `n` tokens and digits `d`, the worst-case is `O(n * d^2)` which is acceptable for `d ≤ 1000` and `n` small (like ≤ 100). Space complexity is `O(d)` per number.
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

// Big integer class for non-negative integers using decimal string representation.
class BigInt {
public:
    std::string digits; // most significant digit first, no leading zeros except "0"

    BigInt() : digits("0") {}
    BigInt(const std::string& s) {
        // Remove leading zeros
        size_t pos = s.find_first_not_of('0');
        if (pos == std::string::npos) {
            digits = "0";
        } else {
            digits = s.substr(pos);
        }
    }
    BigInt(long long v) : digits(std::to_string(v)) {}

    bool isZero() const { return digits == "0"; }

    // Compare two BigInts: returns -1, 0, 1
    int compare(const BigInt& other) const {
        if (digits.length() != other.digits.length()) {
            return digits.length() < other.digits.length() ? -1 : 1;
        }
        if (digits == other.digits) return 0;
        return digits < other.digits ? -1 : 1;
    }

    bool operator<(const BigInt& other) const { return compare(other) < 0; }
    bool operator>(const BigInt& other) const { return compare(other) > 0; }
    bool operator==(const BigInt& other) const { return compare(other) == 0; }

    // Addition (both non-negative)
    BigInt operator+(const BigInt& other) const {
        std::string result;
        int i = digits.length() - 1;
        int j = other.digits.length() - 1;
        int carry = 0;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += digits[i--] - '0';
            if (j >= 0) sum += other.digits[j--] - '0';
            result.push_back('0' + (sum % 10));
            carry = sum / 10;
        }
        std::reverse(result.begin(), result.end());
        return BigInt(result);
    }

    // Subtraction (requires this >= other, result non-negative)
    BigInt operator-(const BigInt& other) const {
        assert(compare(other) >= 0);
        std::string result;
        int i = digits.length() - 1;
        int j = other.digits.length() - 1;
        int borrow = 0;
        while (i >= 0) {
            int diff = (digits[i--] - '0') - borrow;
            if (j >= 0) diff -= (other.digits[j--] - '0');
            if (diff < 0) { diff += 10; borrow = 1; } else { borrow = 0; }
            result.push_back('0' + diff);
        }
        std::reverse(result.begin(), result.end());
        return BigInt(result);
    }

    // Multiplication (naive O(n*m))
    BigInt operator*(const BigInt& other) const {
        if (isZero() || other.isZero()) return BigInt("0");
        std::vector<int> res(digits.length() + other.digits.length(), 0);
        for (int i = digits.length() - 1; i >= 0; --i) {
            for (int j = other.digits.length() - 1; j >= 0; --j) {
                int mul = (digits[i] - '0') * (other.digits[j] - '0');
                int sum = mul + res[i + j + 1];
                res[i + j + 1] = sum % 10;
                res[i + j] += sum / 10;
            }
        }
        std::string result;
        bool started = false;
        for (int v : res) {
            if (v != 0 || started) { result.push_back('0' + v); started = true; }
        }
        if (!started) result = "0";
        return BigInt(result);
    }

    // Division (integer division, requires other != 0)
    BigInt operator/(const BigInt& other) const {
        assert(!other.isZero());
        BigInt dividend = *this;
        BigInt divisor = other;
        if (dividend < divisor) return BigInt("0");
        std::string quotient;
        BigInt remainder("0");
        for (char c : dividend.digits) {
            remainder = remainder * BigInt("10") + BigInt(std::string(1, c));
            int digit = 0;
            while (remainder >= divisor) {
                remainder = remainder - divisor;
                digit++;
            }
            quotient.push_back('0' + digit);
        }
        // Remove leading zeros
        size_t pos = quotient.find_first_not_of('0');
        if (pos == std::string::npos) return BigInt("0");
        return BigInt(quotient.substr(pos));
    }

    // Exponentiation: this^exp, exp is non-negative BigInt
    BigInt pow(const BigInt& exp) const {
        if (exp.isZero()) return BigInt("1");
        BigInt base = *this;
        BigInt result("1");
        // Process exponent as decimal string, most significant first
        for (char c : exp.digits) {
            int digit = c - '0';
            // result = result^10 * base^digit
            BigInt temp = result;
            for (int i = 1; i < 10; ++i) result = result * temp;
            BigInt multiplier("1");
            for (int i = 0; i < digit; ++i) multiplier = multiplier * base;
            result = result * multiplier;
        }
        return result;
    }
};

// Implementation of the token-processing function
std::string processTokens(const std::string& input) {
    std::string output;
    BigInt a("0"), b("0");
    size_t i = 0;
    while (i < input.length()) {
        // Skip whitespace
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\r') {
            ++i;
            continue;
        }
        // If digit, parse integer token
        if (isdigit(input[i])) {
            size_t start = i;
            while (i < input.length() && isdigit(input[i])) ++i;
            std::string numStr = input.substr(start, i - start);
            // Shift: a = b; b = new number
            a = b;
            b = BigInt(numStr);
        } else {
            // Operator
            char op = input[i];
            ++i;
            std::string result;
            switch (op) {
                case '+': result = (a + b).digits; break;
                case '-': 
                    if (a < b) result = "Error";
                    else result = (a - b).digits;
                    break;
                case '*': result = (a * b).digits; break;
                case '/':
                    if (b.isZero()) result = "Error";
                    else result = (a / b).digits;
                    break;
                case '^':
                    if (a.isZero() && b.isZero()) result = "Error";
                    else result = a.pow(b).digits;
                    break;
                case '>': result = (a > b) ? "true" : "false"; break;
                case '<': result = (a < b) ? "true" : "false"; break;
                case '=': result = (a == b) ? "true" : "false"; break;
                default: result = "Error"; break;
            }
            if (!output.empty()) output += "\n";
            output += result;
        }
    }
    return output;
}
#include <string>
#include <cassert>

// forward declaration of the function under test
std::string processTokens(const std::string& input);

int main() {
    // Basic operations with small numbers
    assert(processTokens("3 4 +") == "7");
    assert(processTokens("5 2 -") == "3");
    assert(processTokens("5 2 *") == "10");
    assert(processTokens("7 2 /") == "3");
    assert(processTokens("2 3 ^") == "8");
    assert(processTokens("3 4 >") == "false");
    assert(processTokens("4 4 =") == "true");
    assert(processTokens("1 2 <") == "true");

    // Edge cases: division by zero, negative subtraction
    assert(processTokens("5 0 /") == "Error");
    assert(processTokens("2 5 -") == "Error");
    assert(processTokens("0 0 ^") == "Error");

    // Sequential tokens: multiple integers then operations
    // Sequence: a=0,b=5; then a=5,b=3; then op '*'
    assert(processTokens("5 3 *") == "15");
    // Sequence: a=0,b=2; then a=2,b=3; then a=3,b=4; then op '+'
    assert(processTokens("2 3 4 +") == "7");  // a=3, b=4 => 3+4
    // After one operation, the values remain, so next integer shifts
    assert(processTokens("2 3 + 4") == "5\n"); // but our function only outputs operations, not integers

    // Test with whitespace
    assert(processTokens("  10   5   -  ") == "5");

    // Larger numbers and exponentiation
    assert(processTokens("123456789 987654321 *") == "121932631112635269");
    assert(processTokens("2 10 ^") == "1024");
    assert(processTokens("0 5 ^") == "0");
    assert(processTokens("5 0 ^") == "1");

    // Multiple operations
    assert(processTokens("10 5 - 2 *") == "10"); // 10-5=5, then 5*2=10

    // Unknown operator
    assert(processTokens("3 4 %") == "Error");

    return 0;
}
