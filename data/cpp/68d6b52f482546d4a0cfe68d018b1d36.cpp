// Write a C++ function named `reverseDigitsOfEach` that takes an integer `T` and a vector of integers `inputs`, and returns a vector of integers where each element is the reverse of the corresponding input's decimal digits. For example, if an input is `1234`, the reversed value is `4321`; if the input is `1200`, the reversed value is `21` (leading zeros are dropped because the result is stored as an integer). The function must handle positive integers only, and if `T` is less than or equal to 0 or greater than the size of `inputs`, it should return an empty vector. The reversal must be performed using arithmetic operations without converting to a string.
// The algorithm processes each number independently. For each number `N`, we repeatedly extract the last digit using `N % 10`, append it to an accumulator `rev` using `rev = rev * 10 + digit`, and then remove that digit from `N` using `N /= 10`. This continues until `N` becomes 0. This correctly reverses digits, but note that any leading zeros in the reversed number are automatically discarded because the accumulator starts at 0 and multiplication by 10 places the first extracted digit in the most significant position. Edge cases: if an input is 0, the while loop is skipped and `rev` remains 0, which is correct. If `T` is negative, zero, or larger than the vector size, we return an empty vector. The time complexity is O(total number of digits in all inputs) because each digit is processed once. The space complexity is O(T) for the output vector, plus O(1) extra space per number for the reversal.
#include <vector>

// Reverse the decimal digits of each integer in inputs.
// Returns a vector of reversed values, or an empty vector if T is invalid.
std::vector<int> reverseDigitsOfEach(int T, const std::vector<int>& inputs) {
    if (T <= 0 || static_cast<size_t>(T) > inputs.size()) {
        return {};
    }

    std::vector<int> result;
    result.reserve(T);

    for (int i = 0; i < T; ++i) {
        int N = inputs[i];
        int reversed = 0;
        while (N != 0) {
            int digit = N % 10;
            reversed = reversed * 10 + digit;
            N /= 10;
        }
        result.push_back(reversed);
    }

    return result;
}
#include <cassert>
#include <vector>

// Function signature from the solution
std::vector<int> reverseDigitsOfEach(int T, const std::vector<int>& inputs);

int main() {
    // Standard case
    std::vector<int> inputs1 = {1234, 56, 7, 890};
    std::vector<int> rev1 = reverseDigitsOfEach(4, inputs1);
    std::vector<int> expected1 = {4321, 65, 7, 98};
    assert(rev1 == expected1);

    // Single number
    std::vector<int> inputs2 = {1000};
    std::vector<int> rev2 = reverseDigitsOfEach(1, inputs2);
    std::vector<int> expected2 = {1};
    assert(rev2 == expected2);

    // Zero input
    std::vector<int> inputs3 = {0};
    std::vector<int> rev3 = reverseDigitsOfEach(1, inputs3);
    std::vector<int> expected3 = {0};
    assert(rev3 == expected3);

    // T is larger than inputs size -> empty
    std::vector<int> inputs4 = {12, 34};
    std::vector<int> rev4 = reverseDigitsOfEach(5, inputs4);
    assert(rev4.empty());

    // T is zero or negative -> empty
    std::vector<int> inputs5 = {1, 2, 3};
    assert(reverseDigitsOfEach(0, inputs5).empty());
    assert(reverseDigitsOfEach(-1, inputs5).empty());

    // All numbers with trailing zeros
    std::vector<int> inputs6 = {120, 300, 4500};
    std::vector<int> rev6 = reverseDigitsOfEach(3, inputs6);
    std::vector<int> expected6 = {21, 3, 54};
    assert(rev6 == expected6);

    return 0;
}
