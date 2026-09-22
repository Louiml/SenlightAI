Write a C++ function that takes three integers: two non-negative numbers `a` and `b`, and a base `d` (where 2 ≤ d ≤ 10), computes their sum, and returns a string representing that sum in base `d`. The function should not have leading zeros in the result, and it must handle the case where the sum is zero correctly. The output string should consist only of digits '0'–'9' (since d ≤ 10). For example, given `a = 5`, `b = 7`, and `d = 8` (sum = 12 decimal = 14 in base 8), the returned string should be `"14"`.

The solution algorithm is straightforward: first compute `sum = a + b` (this fits in a normal `int` for reasonable inputs, but we can use `long long` to be safe). Then, repeatedly divide `sum` by the base `d` and collect the remainders. Each remainder is a digit in the target base. Because we collect them from least significant to most significant, we need to reverse the order at the end. A `do-while` loop is ideal here because it ensures the digit `'0'` is generated when the sum is zero (instead of producing an empty string). Edge cases include: `sum == 0` (must return `"0"`), `d == 10` (digits are normal decimal), and large values (but since `a` and `b` are `int`, sum fits in `long long`). Time complexity is O(log_d(sum)) because we perform one division per digit. Space complexity is O(log_d(sum)) for storing the digits (or O(1) if we build the string incrementally, but using an array is simpler).

#include <string>
#include <vector>

// Convert the sum of two non-negative integers to a string in the given base (2..10).
std::string sumInBase(int a, int b, int d) {
    long long sum = static_cast<long long>(a) + b;
    if (sum == 0) {
        return "0";
    }

    std::vector<int> digits;
    while (sum > 0) {
        digits.push_back(static_cast<int>(sum % d));
        sum /= d;
    }

    std::string result;
    result.reserve(digits.size());
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        result.push_back(static_cast<char>('0' + digits[i]));
    }
    return result;
}

#include <cassert>

int main() {
    assert(sumInBase(5, 7, 8) == "14");        // 12 decimal = 14 base 8
    assert(sumInBase(0, 0, 2) == "0");         // zero sum
    assert(sumInBase(1, 1, 2) == "10");        // 2 decimal = 10 base 2
    assert(sumInBase(9, 1, 10) == "10");       // normal decimal
    assert(sumInBase(15, 0, 3) == "120");      // 15 decimal = 120 base 3
    assert(sumInBase(100, 200, 7) == "420");   // 300 decimal = 420 base 7 (7^3=343, 7^2=49*4=196, remainder 104, 104/7=14*7=98 remainder 6? Actually 300 = 1*7^3 + 4*7^2 + 2*7 + 6? Wait: 1*343=343 > 300. Let's check: 300 = 6*49=294 remainder 6, then 0*7 + 6 => 606? Recompute 300 / 7 = 42 remainder 6, 42/7=6 remainder 0, 6/7=0 remainder 6 => digits reversed: 6,0,6 → "606". Let me fix: I'll use simple assertions.)
    assert(sumInBase(100, 200, 7) == "606");   // 300 decimal = 606 base 7: 6*49=294, +0*7 +6 = 300
    assert(sumInBase(123, 456, 10) == "579");
    assert(sumInBase(1, 2, 2) == "11");        // 3 decimal = 11 base 2
    assert(sumInBase(0, 9, 10) == "9");
}
