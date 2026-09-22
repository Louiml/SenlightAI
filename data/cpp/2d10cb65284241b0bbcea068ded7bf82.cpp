/*
Write a C++ function named `countSpecialRemainders` that reads a sequence of non-zero integers from standard input until the value `0` is entered, and for each entered number `num` (excluding the terminating `0`), computes `resto = num % 11`. The function must count two independent groups: (1) the number of inputs where `resto` itself is a multiple of 3 (i.e., `resto % 3 == 0`), and (2) the number of inputs where both `resto` is a multiple of 5 AND the original `num` is also a multiple of 5 (i.e., `resto % 5 == 0` and `num % 5 == 0`). Return a `std::pair<int,int>` where the first value is the count for group 1 and the second is the count for group 2. The function must handle any integer values (positive, negative, or zero—though zero terminates), treat the terminating `0` as not part of any group, and ignore any input that is zero (since zero stops the loop). Assume standard input is valid and contains at least the terminating zero.
*/
#include <iostream>
#include <utility>

// Reads integers until 0 is entered, counts two groups:
// 1) count of numbers where (num % 11) % 3 == 0
// 2) count of numbers where both (num % 11) % 5 == 0 AND num % 5 == 0
// Returns a pair: {count1, count2}. The terminating 0 is not counted.
std::pair<int, int> countSpecialRemainders() {
    int countMultiplesOf3 = 0;
    int countJointMultiplesOf5 = 0;
    int num = 0;

    while (true) {
        std::cin >> num;
        if (num == 0) {
            break;
        }
        const int resto = num % 11;

        if (resto % 3 == 0) {
            ++countMultiplesOf3;
        }
        if (resto % 5 == 0 && num % 5 == 0) {
            ++countJointMultiplesOf5;
        }
    }

    return {countMultiplesOf3, countJointMultiplesOf5};
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <utility>

// Include the solution function here (or declare it) — for standalone test, we assume it's above.
// To make the test runnable, we'll copy the function here (but in a real separate file, just include it).

std::pair<int, int> countSpecialRemaindersFromStream(std::istream& input) {
    int countMultiplesOf3 = 0;
    int countJointMultiplesOf5 = 0;
    int num = 0;

    while (input >> num) {
        if (num == 0) break;
        const int resto = num % 11;
        if (resto % 3 == 0) ++countMultiplesOf3;
        if (resto % 5 == 0 && num % 5 == 0) ++countJointMultiplesOf5;
    }
    return {countMultiplesOf3, countJointMultiplesOf5};
}

int main() {
    // Test 1: 11 (resto=0, %3==0 => count1++, %5? resto=0%5==0 but num=11%5!=0 => no count2), 0
    std::istringstream in1("11 0");
    auto res1 = countSpecialRemaindersFromStream(in1);
    assert(res1.first == 1 && res1.second == 0);

    // Test 2: 5 (resto=5, 5%3!=0, 5%5==0 and 5%5==0 => count2++), 0
    std::istringstream in2("5 0");
    auto res2 = countSpecialRemaindersFromStream(in2);
    assert(res2.first == 0 && res2.second == 1);

    // Test 3: 16 (resto=5, 5%3!=0, 5%5==0 but 16%5!=0 => no count2), 22 (resto=0, %3==0, %5==0 but 22%5!=0 => no count2), 0
    std::istringstream in3("16 22 0");
    auto res3 = countSpecialRemaindersFromStream(in3);
    assert(res3.first == 1 && res3.second == 0);

    // Test 4: 55 (resto=0, %3==0 count1++, %5==0 and 55%5==0 => count2++), 0
    std::istringstream in4("55 0");
    auto res4 = countSpecialRemaindersFromStream(in4);
    assert(res4.first == 1 && res4.second == 1);

    // Test 5: -33 (resto=-33%11=0? -33/11=-3, remainder 0 => %3==0 count1++, %5? resto=0, num=-33%5= -3 !=0 => no count2), 0
    std::istringstream in5("-33 0");
    auto res5 = countSpecialRemaindersFromStream(in5);
    assert(res5.first == 1 && res5.second == 0);

    // Test 6: Just 0 → both counts 0
    std::istringstream in6("0");
    auto res6 = countSpecialRemaindersFromStream(in6);
    assert(res6.first == 0 && res6.second == 0);

    // Test 7: Numbers 3 (resto=3, %3==0), 10 (resto=10, %3!=0, %5==0 but 10%5==0 => count2++), 15 (resto=4, no), 0
    std::istringstream in7("3 10 15 0");
    auto res7 = countSpecialRemaindersFromStream(in7);
    assert(res7.first == 1 && res7.second == 1);

    // Test 8: 33 (resto=0, count1++, count2? 33%5!=0 => no), 66 (resto=0, count1++, 66%5!=0), 0
    std::istringstream in8("33 66 0");
    auto res8 = countSpecialRemaindersFromStream(in8);
    assert(res8.first == 2 && res8.second == 0);

    // Test 9: -10 (resto=-10%11=-10, -10%3=-1 (not 0), -10%5==0 and -10%5==0 => count2++), 0
    std::istringstream in9("-10 0");
    auto res9 = countSpecialRemaindersFromStream(in9);
    assert(res9.first == 0 && res9.second == 1);

    // Test 10: 121 (resto=0, count1++, 121%5!=0), 0
    std::istringstream in10("121 0");
    auto res10 = countSpecialRemaindersFromStream(in10);
    assert(res10.first == 1 && res10.second == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution approach is to use a `do-while` loop that reads integers from `std::cin` until a zero is encountered. For each non-zero integer read, compute `resto = num % 11` (note that in C++, `%` with negative numbers yields a negative remainder with the sign of the dividend, but the divisibility checks `resto % 3 == 0` and `resto % 5 == 0` work correctly regardless of sign because a negative remainder divisible by 3 or 5 is still divisible). For group 1, increment the first counter if `resto % 3 == 0`. For group 2, increment the second counter only if both `resto % 5 == 0` and `num % 5 == 0`. The terminating zero is excluded by checking `num != 0` before counting. Edge cases include negative numbers: e.g., `-11 % 11 == 0`, `-13 % 11 == -2`, and `-25 % 11 == -3`, but the modulo checks still work. If the input contains only `0`, both counters remain zero. Time complexity is O(n) where n is the number of integers read until zero, and space complexity is O(1) auxiliary (only a few integer variables and the pair). The function uses `const` where appropriate (e.g., the loop variable is not modified after reading, but we use plain ints for simplicity; `const` is applied to local constants if any).
