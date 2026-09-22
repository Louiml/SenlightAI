Write a C++ function `std::string binaryRepresentation(int n)` that, given a non-negative integer `n`, returns its binary representation as a string without leading zeros. The function must handle `n == 0` by returning the string `"0"`. For positive integers, compute the binary digits by repeatedly dividing by 2 and collecting remainders (least significant bit first), then reverse them to form the result. The input is guaranteed to be non-negative and fits in a standard `int`. Do not use any built‑in binary conversion functions (e.g., `std::bitset`, `itoa`, etc.). The function should be efficient and avoid undefined behavior.

// The solution uses the classic repeated‑division algorithm: while the number is greater than 0, take the remainder when divided by 2 (which gives the least significant bit), append it to a string (or store in a container), then divide the number by 2. This process continues until the number becomes 0. For the special case `n == 0`, the result is just `"0"` (the loop would produce an empty string). After collecting the remainders, they are in reverse order (most significant bit last), so we reverse them to get the correct binary string. Alternatively, we can build the string by prepending each bit. Edge cases: `n = 0` returns `"0"`; `n = 1` returns `"1"`; large values like `INT_MAX` produce a 31‑bit string correctly. Time complexity is \(O(\log_2 n)\) because we process one bit per iteration; space complexity is also \(O(\log_2 n)\) for storing the result string. The algorithm is deterministic and does not require any special libraries beyond `<string>`.

#include <string>
#include <algorithm>

// Return the binary representation of a non-negative integer as a string.
std::string binaryRepresentation(int n) {
    if (n == 0) {
        return "0";
    }
    std::string result;
    while (n > 0) {
        result.push_back((n % 2) ? '1' : '0');
        n /= 2;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <string>

// The function declaration is provided here to make the test self-contained.
std::string binaryRepresentation(int n);

int main() {
    assert(binaryRepresentation(0) == "0");
    assert(binaryRepresentation(1) == "1");
    assert(binaryRepresentation(2) == "10");
    assert(binaryRepresentation(3) == "11");
    assert(binaryRepresentation(5) == "101");
    assert(binaryRepresentation(8) == "1000");
    assert(binaryRepresentation(16) == "10000");
    assert(binaryRepresentation(255) == "11111111");
    assert(binaryRepresentation(1024) == "10000000000");
    assert(binaryRepresentation(2147483647) == "1111111111111111111111111111111");
    return 0;
}
