/*
Write a C++ function `bool isPerfectSquare(const BigInt& n)` that determines whether a given non‑negative integer, represented by a custom `BigInt` class storing its digits in base 10 (least significant digit first), is a perfect square. The `BigInt` class provides the basic arithmetic operations `add`, `mul`, `sub`, `compare` (for equality and ordering), and a zero check. Your function must use only these operations (no floating‑point or built‑in integer types) to decide if `n` is the square of an integer. For example, `BigInt("0")`, `BigInt("1")`, `BigInt("4")`, `BigInt("81")` are perfect squares, while `BigInt("2")`, `BigInt("3")`, `BigInt("10")`, `BigInt("999")` are not. The input is guaranteed to be non‑negative (i.e., its sign is positive or zero). Your solution must handle numbers with up to 10^5 digits efficiently.
*/

#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

// ---------- BigInt class (simplified for the task) ----------
class BigInt {
public:
    std::vector<uint32_t> digits; // least significant first, base 10
    bool positive = true; // true for non‑negative, false for negative (not used here)

    BigInt() = default;
    BigInt(uint64_t val) { if (val==0) digits.push_back(0); else while(val){digits.push_back(val%10); val/=10;} }
    BigInt(const std::string& s) {
        if (s.empty() || s=="0") { digits.push_back(0); return; }
        for (int i=s.size()-1; i>=0; --i) digits.push_back(s[i]-'0');
        trim();
    }
    BigInt(const std::vector<uint32_t>& d) : digits(d) { trim(); }

    void trim() { while (digits.size()>1 && digits.back()==0) digits.pop_back(); }
    bool isZero() const { return digits.size()==1 && digits[0]==0; }

    // Compare absolute values: returns -1 if abs(a)<abs(b), 0 if equal, 1 if greater
    int compareAbs(const BigInt& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size() ? -1 : 1;
        for (int i=digits.size()-1; i>=0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i] ? -1 : 1;
        }
        return 0;
    }

    bool operator==(const BigInt& other) const { return compareAbs(other)==0 && positive==other.positive; }
    bool operator!=(const BigInt& other) const { return !(*this==other); }
    bool operator<(const BigInt& other) const {
        if (positive != other.positive) return !positive;
        int c = compareAbs(other);
        return positive ? (c<0) : (c>0);
    }
    bool operator>=(const BigInt& other) const { return !(*this < other); }

    // Addition (absolute values, ignoring sign; both non‑negative in this task)
    BigInt add(const BigInt& other) const {
        std::vector<uint32_t> res;
        size_t n = std::max(digits.size(), other.digits.size());
        res.reserve(n+1);
        uint32_t carry = 0;
        for (size_t i=0; i<n; ++i) {
            uint32_t da = i<digits.size() ? digits[i] : 0;
            uint32_t db = i<other.digits.size() ? other.digits[i] : 0;
            uint32_t sum = da + db + carry;
            if (sum >= 10) { sum -= 10; carry = 1; } else carry = 0;
            res.push_back(sum);
        }
        if (carry) res.push_back(1);
        return BigInt(res);
    }

    // Subtraction (absolute values, assumes *this >= other)
    BigInt sub(const BigInt& other) const {
        std::vector<uint32_t> res;
        res.reserve(digits.size());
        int32_t borrow = 0;
        for (size_t i=0; i<digits.size(); ++i) {
            int32_t da = digits[i];
            int32_t db = i<other.digits.size() ? other.digits[i] : 0;
            int32_t diff = da - db - borrow;
            if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
            res.push_back(diff);
        }
        return BigInt(res);
    }

    // Multiplication (absolute values)
    BigInt mul(const BigInt& other) const {
        if (isZero() || other.isZero()) return BigInt(0);
        std::vector<uint32_t> res(digits.size() + other.digits.size(), 0);
        for (size_t i=0; i<digits.size(); ++i) {
            uint32_t carry = 0;
            for (size_t j=0; j<other.digits.size(); ++j) {
                uint32_t cur = res[i+j] + digits[i]*other.digits[j] + carry;
                res[i+j] = cur % 10;
                carry = cur / 10;
            }
            res[i+other.digits.size()] += carry;
        }
        BigInt result(res);
        result.trim();
        return result;
    }

    // Division by a single‑digit (returns quotient, sets remainder)
    BigInt divSingle(uint32_t divisor, uint32_t& remainder) const {
        std::vector<uint32_t> quotient(digits.size());
        uint32_t rem = 0;
        for (int i=digits.size()-1; i>=0; --i) {
            uint32_t cur = rem*10 + digits[i];
            quotient[i] = cur / divisor;
            rem = cur % divisor;
        }
        remainder = rem;
        return BigInt(quotient);
    }

    // Division by BigInt (integer division, assumes divisor > 0)
    BigInt div(const BigInt& divisor) const {
        if (divisor.isZero()) throw std::runtime_error("Division by zero");
        if (*this < divisor) return BigInt(0);
        // Use long division on digit sequences
        BigInt quotient;
        BigInt remainder;
        for (int i=digits.size()-1; i>=0; --i) {
            // shift remainder left one digit and append current digit
            remainder = remainder.mul(BigInt(10));
            remainder = remainder.add(BigInt(digits[i]));
            // find digit d such that divisor*d <= remainder
            uint32_t lo = 0, hi = 10;
            while (lo+1 < hi) {
                uint32_t mid = (lo+hi)/2;
                if (divisor.mul(BigInt(mid)) <= remainder) lo = mid; else hi = mid;
            }
            quotient.digits.push_back(lo); // will reverse later
            remainder = remainder.sub(divisor.mul(BigInt(lo)));
        }
        std::reverse(quotient.digits.begin(), quotient.digits.end());
        quotient.trim();
        return quotient;
    }
};

// Helper: return 10^exp as BigInt
BigInt pow10(int exp) {
    std::vector<uint32_t> d(exp+1,0);
    d[exp]=1;
    return BigInt(d);
}

// Function: check if n is a perfect square
bool isPerfectSquare(const BigInt& n) {
    if (n.isZero()) return true;
    if (n.digits.size() == 1) {
        uint32_t v = n.digits[0];
        for (uint32_t i=0; i*i<=v; ++i) if (i*i==v) return true;
        return false; // for small numbers could do better, but fine
    }

    // Initial guess: 10^(ceil(digits/2)) where digits = number of base‑10 digits
    int numDigits = n.digits.size();
    int halfDigits = (numDigits + 1) / 2;
    BigInt x = pow10(halfDigits);
    BigInt two = BigInt(2);

    while (true) {
        BigInt x_plus_n_div = x.add(n.div(x));
        BigInt next = x_plus_n_div.div(two);
        if (next >= x) break;
        x = next;
    }
    // Now x is the integer square root (largest integer with x*x <= n)
    BigInt square = x.mul(x);
    return square == n;
}

#include <cassert>
#include <iostream>

// Include the BigInt class and isPerfectSquare here (omitted for brevity in this answer, but in practice paste the above code)

int main() {
    // Basic small numbers
    assert(isPerfectSquare(BigInt(0)) == true);
    assert(isPerfectSquare(BigInt(1)) == true);
    assert(isPerfectSquare(BigInt(2)) == false);
    assert(isPerfectSquare(BigInt(3)) == false);
    assert(isPerfectSquare(BigInt(4)) == true);
    assert(isPerfectSquare(BigInt(9)) == true);
    assert(isPerfectSquare(BigInt(10)) == false);
    assert(isPerfectSquare(BigInt(16)) == true);

    // Larger perfect squares
    assert(isPerfectSquare(BigInt("81")) == true);
    assert(isPerfectSquare(BigInt("100")) == true);
    assert(isPerfectSquare(BigInt("121")) == true);
    assert(isPerfectSquare(BigInt("144")) == true);
    assert(isPerfectSquare(BigInt("999")) == false);
    assert(isPerfectSquare(BigInt("1000")) == false);

    // Very large perfect square: (10^10)^2 = 10^20
    assert(isPerfectSquare(BigInt("100000000000000000000")) == true);
    // Non‑square near it
    assert(isPerfectSquare(BigInt("100000000000000000001")) == false);

    // Perfect square with many digits (10^5 digits)
    // Construct (10^50000)^2 = 10^100000 (1 followed by 100000 zeros)
    std::string s(100001, '0');
    s[0] = '1';
    assert(isPerfectSquare(BigInt(s)) == true);

    // Slightly off: add 1 to the 10^100000
    std::string s2(100001, '0');
    s2[0] = '1';
    s2[100000] = '1'; // this becomes 10^100000 + 1
    assert(isPerfectSquare(BigInt(s2)) == false);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// A standard approach is to use integer square root via Newton's method (also called the Babylonian method), adapted for big integers. Given a non‑negative integer `n`, we want to find the largest integer `r` such that `r*r <= n`. We start with an initial guess `x0` that is a power of two close to the square root of `n`. The number of digits of `n` in base 10 is `d`. The square root of `n` has about `ceil(d/2)` digits. We can set the initial guess to `10^(ceil(d/2))` (i.e., a number with `ceil(d/2)` zeros and a leading 1). Then we iterate using Newton's update: `x_{k+1} = (x_k + n / x_k) / 2`, using integer division. We stop when `x_{k+1} >= x_k` (the sequence converges to the integer square root from above). After convergence, `x` is the integer square root (the largest integer whose square is ≤ n). Finally, we check if `x*x == n`. If yes, `n` is a perfect square; otherwise it is not. Edge cases: `n=0` gives `x=0` and `0*0 == 0`, true. `n=1` gives `x=1` and true. For very large numbers, the initial guess ensures fast convergence—typically logarithmic number of iterations. Each iteration uses big‑integer multiplication, division (which can be implemented as repeated subtraction or long division), and addition, so the overall time complexity is roughly `O(d * log d)` per iteration (using efficient multiplication), and the number of iterations is `O(log d)`. Space complexity is `O(d)` for storing operands.
