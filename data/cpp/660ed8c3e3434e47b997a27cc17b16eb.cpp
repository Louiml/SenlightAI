// Write a C++ function named `digitalRoot` that takes a single non-negative integer `num` as input and repeatedly sums its digits until the result is a single digit (i.e., a number from 0 to 9). The function should return that single digit. For example, given `38`, the function should return `2` because `3 + 8 = 11`, then `1 + 1 = 2`. The function must handle the edge case `num = 0` correctly, returning `0`. You may not use any external libraries beyond the standard C++ headers, and the function should be `const`-correct (i.e., it should not modify the input parameter, which should be passed by value). The function signature should be `int digitalRoot(int num);`.

// The problem is a classic "digital root" problem. The direct iterative approach is to keep summing the digits of `num` until it is less than 10. This is straightforward: while `num >= 10`, compute the sum of its digits using modulo 10 and integer division, then set `num` to that sum. For `num = 0`, the loop is skipped and `0` is returned. The algorithm is efficient because each iteration reduces the number of digits, and the number of iterations is at most the number of digits in the original number (which is proportional to `log10(num)`). The time complexity is `O(d)` per iteration, where `d` is the number of digits, and the total time is `O(d * iterations)` — but since `d` shrinks each time, the worst-case for a 32-bit integer is about 10 iterations, so effectively `O(1)` for practical inputs. Space complexity is `O(1)` since only a few integer variables are used.

// Compute the digital root of a non-negative integer.
// Repeatedly sums the digits of num until a single digit remains.
int digitalRoot(int num) {
    while (num >= 10) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        num = sum;
    }
    return num;
}

int main() {
    assert(digitalRoot(0) == 0);
    assert(digitalRoot(9) == 9);
    assert(digitalRoot(10) == 1);
    assert(digitalRoot(38) == 2);
    assert(digitalRoot(12345) == 6); // 1+2+3+4+5=15 -> 1+5=6
    assert(digitalRoot(999) == 9);   // 9+9+9=27 -> 2+7=9
    assert(digitalRoot(1000000000) == 1);
    return 0;
}
