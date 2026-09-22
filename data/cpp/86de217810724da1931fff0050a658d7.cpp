Write a C++ function `std::string sorobanRepresentation(int n)` that, given a non-negative integer `n`, returns a string containing a line-by-line representation of `n` using a Japanese soroban (abacus) digit display. Each digit of `n` (from the least significant to the most significant) is rendered on its own line using the following format: two characters for the "heaven" (upper) bead part (either `-O` if the digit has 5 or more, otherwise `O-`), followed by `|`, then a sequence of `O` characters for the "earth" (lower) beads (the remainder of the digit modulo 5) on the left side of a dash, then a dash `-`, then enough `O` characters to make the total of earth beads plus the right-side `O`s equal exactly 4 (i.e., if remainder is `r`, output `r` `O`s before the dash and `4-r` `O`s after). If `n` is 0, output `O-|-OOOO` on a single line. Each line must end with a newline character. The function should not read from standard input; it receives the integer as a parameter and returns the complete multi-line string.

The task is to convert each decimal digit of `n` from least significant to most significant into a soroban digit pattern. For each digit `d`, we first determine if it is at least 5; if so, the upper bead is "on" (represented as `-O`), and we subtract 5 to get the lower bead count `r`; otherwise, the upper bead is "off" (`O-`) and `r = d`. Then we build the line as: upper bead part, `|`, `r` times `O`, `-`, `(4-r)` times `O`, and a newline. We process digits by repeatedly taking `n % 10` and dividing `n` by 10 until `n` becomes 0. Edge cases: `n = 0` must output exactly `O-|-OOOO\n` (since digit 0 has upper bead off, remainder 0, so 0 `O`s before dash and 4 `O`s after). Negative integers are not allowed per the specification. Time complexity is `O(d * (r + (4-r))) = O(d)` where `d` is the number of digits, because the inner loops each iterate at most 4 times (since `r` ranges 0-4). Space complexity is `O(d)` for the returned string, plus constant extra.

#include <string>

// Returns a soroban representation of a non-negative integer n.
std::string sorobanRepresentation(int n) {
    std::string result;
    if (n == 0) {
        return "O-|-OOOO\n";
    }
    while (n > 0) {
        int digit = n % 10;
        n /= 10;
        int lowerBeads;
        if (digit >= 5) {
            result += "-O";
            lowerBeads = digit - 5;
        } else {
            result += "O-";
            lowerBeads = digit;
        }
        result += '|';
        for (int i = 0; i < lowerBeads; ++i) {
            result += 'O';
        }
        result += '-';
        for (int i = 0; i < 4 - lowerBeads; ++i) {
            result += 'O';
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>
// Assume sorobanRepresentation is defined above.
int main() {
    assert(sorobanRepresentation(0) == "O-|-OOOO\n");
    assert(sorobanRepresentation(1) == "O-|O---OOO\n");
    assert(sorobanRepresentation(5) == "-O|-OOOO\n");
    assert(sorobanRepresentation(9) == "-O|O---OOO\n");
    assert(sorobanRepresentation(10) == "O-|-OOOO\nO-|O---OOO\n");
    assert(sorobanRepresentation(49) == "-O|O---OOO\nO-|OOO--O\n");
    assert(sorobanRepresentation(123) == "O-|OOO--O\nO-|OO---OO\nO-|O---OOO\n");
    assert(sorobanRepresentation(9999) == "-O|O---OOO\n-O|O---OOO\n-O|O---OOO\n-O|O---OOO\n");
    assert(sorobanRepresentation(100000) == "O-|-OOOO\nO-|-OOOO\nO-|-OOOO\nO-|-OOOO\nO-|O---OOO\n");
    assert(sorobanRepresentation(50000) == "O-|-OOOO\nO-|-OOOO\nO-|-OOOO\nO-|-OOOO\n-O|-OOOO\n");
}
