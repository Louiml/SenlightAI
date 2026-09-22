Given a positive integer n (1 ≤ n ≤ 1000), write a C++ function `largestAlternatingSum(int n)` that returns a `std::string` representing the largest integer whose digits sum exactly to n, contain no digit 0, and have no two adjacent equal digits. The returned string should be the decimal representation of that maximum integer (without leading zeros). For example, for n=3, the possible integers are 3, 12, 21, 111, etc., but only 21 and 12 have no adjacent equal digits; the largest is "21".
The key insight is that the number of digits matters more than the digit values: any number with more digits is automatically larger than one with fewer digits (since there are no leading zeros). To maximize the digit count, we should use the smallest allowed digits (1 and 2) because using a larger digit like 3 would reduce the total number of digits for the same sum. Since digits must alternate (no equal adjacent), the only possible repeating pattern is "12" or "21". The sum of each "12" or "21" pair is 3. Therefore, for a given n, the maximum length is achieved by using as many full pairs of sum 3 as possible, plus possibly one extra digit (1 or 2) when n % 3 != 0. The choice between starting with "12" or "21" affects whether the leftover digit (if any) can be placed without violating adjacency. When n % 3 == 0, we can output either pattern, but "21" repeated gives a larger first digit, so it's larger. When n % 3 == 1, we must start with "1" to append the extra 1, giving pattern "1" + "21" repeated. When n % 3 == 2, we start with "2" to append the extra 2, giving "2" + "12" repeated. This yields the maximum possible value. Time complexity is O(n/3) for string construction, space O(n) for the result. Edge cases: n=1 gives "1", n=2 gives "2", and large n just extends the pattern.
#include <string>

// Returns the largest integer (as a string) whose digits sum to n,
// contain no 0, and have no two adjacent equal digits.
std::string largestAlternatingSum(int n) {
    int fullPairs = n / 3;
    int remainder = n % 3;

    std::string result;

    if (remainder == 0) {
        for (int i = 0; i < fullPairs; ++i) {
            result += "21";
        }
    } else if (remainder == 1) {
        result += '1';
        for (int i = 0; i < fullPairs; ++i) {
            result += "21";
        }
    } else { // remainder == 2
        result += '2';
        for (int i = 0; i < fullPairs; ++i) {
            result += "12";
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Assume the solution function is declared above.
int main() {
    assert(largestAlternatingSum(1) == "1");
    assert(largestAlternatingSum(2) == "2");
    assert(largestAlternatingSum(3) == "21");
    assert(largestAlternatingSum(4) == "121");
    assert(largestAlternatingSum(5) == "212");
    assert(largestAlternatingSum(6) == "2121");
    assert(largestAlternatingSum(7) == "12121");
    assert(largestAlternatingSum(8) == "21212");
    assert(largestAlternatingSum(9) == "212121");
    assert(largestAlternatingSum(10) == "1212121");
}
