Write a C++ function that counts the number of balls in each box, where each ball is labeled with a number from `lowLimit` to `highLimit` inclusive, and is placed into a box whose index equals the sum of the digits of the ball's label. Return the maximum number of balls that end up in any single box. The function should accept two integers `lowLimit` and `highLimit` (both non-negative, with `lowLimit <= highLimit` and `highLimit <= 100000`). The result is an integer representing the largest frequency of balls in any box.

// The solution iterates over every integer from `lowLimit` to `highLimit`, computes the sum of its digits using a helper function, and increments a counter for that box number. Since the maximum label is 100,000, the digit sum cannot exceed 45 (for 99,999), so an array or map is sufficient. Using an array indexed by digit sum avoids the overhead of a map and is faster. After counting, we scan through the array (or map) to find the maximum frequency. Edge cases: single ball range (returns 1), all balls having the same digit sum (e.g., 10 to 19 all sum to 1 through 10, but numbers like 1 and 10 both sum to 1), and zero for lowLimit (digit sum of 0 is 0). Time complexity is O((highLimit - lowLimit + 1) * number_of_digits) which is O(N * d) where d is at most 6, effectively O(N). Space complexity is O(1) if using a fixed-size array of 46 elements, or O(k) for a map where k is number of distinct digit sums (at most 46).

#include <vector>
#include <algorithm>

// Compute the sum of the digits of a non-negative integer
int digitSum(int number) {
    int sum = 0;
    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

// Return the maximum number of balls in any box, where box index = digit sum of ball label
int maxBallsInBox(int lowLimit, int highLimit) {
    // Max digit sum for numbers <= 100000 is 45 (for 99999), so allocate 46 slots
    const int MAX_DIGIT_SUM = 45;
    std::vector<int> boxCount(MAX_DIGIT_SUM + 1, 0);

    for (int ball = lowLimit; ball <= highLimit; ++ball) {
        int box = digitSum(ball);
        ++boxCount[box];
    }

    return *std::max_element(boxCount.begin(), boxCount.end());
}

#include <cassert>

int main() {
    // Basic case: 1 to 9, each digit sum unique, each box has 1 ball
    assert(maxBallsInBox(1, 9) == 1);

    // 1 to 10: digit sums are 1,2,...,9,1 -> box 1 has 2 balls
    assert(maxBallsInBox(1, 10) == 2);

    // Single ball
    assert(maxBallsInBox(5, 5) == 1);

    // 0 to 0: digit sum 0
    assert(maxBallsInBox(0, 0) == 1);

    // 19 to 22: 19->10, 20->2, 21->3, 22->4 -> each unique, max 1
    assert(maxBallsInBox(19, 22) == 1);

    // 1 to 100: many sums, but 1 appears 10 times? Check: 1,10,19,28,37,46,55,64,73,82,91,100 (1+0+0=1) so 12 times
    assert(maxBallsInBox(1, 100) == 12);

    // 99990 to 100000: sums: 99990->45, 99991->46? wait 99991 sum = 46? No, 9+9+9+9+1 = 37? Actually 9+9+9+9+1 = 37, 100000 sum=1, so check 99990 sum=45, 99999 sum=45 -> 2 balls in box 45
    assert(maxBallsInBox(99990, 100000) == 2);

    // All same sum: 10 to 18: sums 1,2,3,...,9 each once -> max 1
    assert(maxBallsInBox(10, 18) == 1);

    // 11 to 20: sums 2,3,4,5,6,7,8,9,10,2 -> box 2 has 2 balls
    assert(maxBallsInBox(11, 20) == 2);
}
