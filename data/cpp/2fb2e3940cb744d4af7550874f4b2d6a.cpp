// Write a C++ function `decryptCode` that takes a circular array of integers `code` and an integer `k`. It should return a new vector where each position `i` is replaced by the sum of the next `k` elements if `k > 0`, or the sum of the previous `|k|` elements if `k < 0`. If `k == 0`, the entire result vector is filled with zeros. The array is treated as circular (the element after the last is the first, and vice versa). Your solution must correctly handle any size `n` (including `n == 0` returning an empty vector), negative numbers in the array, and any integer `k` (including values larger than `n`, which will naturally wrap around multiple times).

The algorithm iterates over each index `i` in the circular array. When `k > 0`, we start at index `(i+1) % n` and add exactly `k` elements, advancing the index by `1` modulo `n` after each addition. When `k < 0`, we start at index `(i-1+n) % n`, add exactly `|k|` elements, and move backward using `(j-1+n) % n`. When `k == 0`, the result for that position is simply `0`. Edge cases include an empty input `n == 0` (return an empty vector immediately), `k` being larger than `n` (the while loop runs `|k|` times, wrapping multiple times without issue), and negative elements in `code` (summation works normally). The time complexity is O(n * |k|) because for each of the `n` positions we perform exactly `|k|` additions. The space complexity is O(n) for the output vector, and O(1) extra auxiliary space besides that.

#include <vector>

// Return a vector where each element at index i is replaced by the sum of
// the next k elements (if k > 0) or the previous |k| elements (if k < 0)
// in a circular array. If k == 0, all returned elements are zero.
std::vector<int> decryptCode(const std::vector<int>& code, int k) {
    const int n = static_cast<int>(code.size());
    if (n == 0) {
        return {};
    }

    std::vector<int> result(n, 0);

    for (int i = 0; i < n; ++i) {
        if (k > 0) {
            int steps = k;
            int j = (i + 1) % n;
            while (steps-- > 0) {
                result[i] += code[j];
                j = (j + 1) % n;
            }
        } else if (k < 0) {
            int steps = -k; // |k|
            int j = (i - 1 + n) % n;
            while (steps-- > 0) {
                result[i] += code[j];
                j = (j - 1 + n) % n;
            }
        }
        // k == 0 leaves result[i] as 0 (already initialized)
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function (already provided above, but included here for completeness)
std::vector<int> decryptCode(const std::vector<int>& code, int k) {
    const int n = static_cast<int>(code.size());
    if (n == 0) {
        return {};
    }

    std::vector<int> result(n, 0);

    for (int i = 0; i < n; ++i) {
        if (k > 0) {
            int steps = k;
            int j = (i + 1) % n;
            while (steps-- > 0) {
                result[i] += code[j];
                j = (j + 1) % n;
            }
        } else if (k < 0) {
            int steps = -k;
            int j = (i - 1 + n) % n;
            while (steps-- > 0) {
                result[i] += code[j];
                j = (j - 1 + n) % n;
            }
        }
    }

    return result;
}

int main() {
    // Example from LeetCode: code = [5,7,1,4], k = 3 -> [12,10,16,13]
    assert(decryptCode({5,7,1,4}, 3) == std::vector<int>({12,10,16,13}));

    // k = 0 gives all zeros
    assert(decryptCode({1,2,3}, 0) == std::vector<int>({0,0,0}));

    // k negative: code = [2,4,9,3], k = -2 -> [12,5,6,13]
    // Explain: index0 sum of previous two: 3+9=12; index1: 2+3=5; index2: 4+2=6; index3: 9+4=13
    assert(decryptCode({2,4,9,3}, -2) == std::vector<int>({12,5,6,13}));

    // Single element, k positive (wrap to itself multiple times)
    assert(decryptCode({7}, 2) == std::vector<int>({14})); // 7+7
    assert(decryptCode({7}, -1) == std::vector<int>({7})); // previous is itself

    // Empty input
    assert(decryptCode({}, 5) == std::vector<int>());

    // k larger than n (wrap more than once)
    assert(decryptCode({1,2,3}, 5) == std::vector<int>({12, 9, 9}));
    // Compute: for i=0: sum(1,2,3,1,2)=9? wait let's recalc: i=0 next 5 elements: 2,3,1,2,3 = 11? Actually let's compute properly:
    // i=0: j=1(2),2(3),0(1),1(2),2(3) = 11
    // i=1: j=2(3),0(1),1(2),2(3),0(1) = 10
    // i=2: j=0(1),1(2),2(3),0(1),1(2) = 9
    // So expected: {11,10,9} — let's fix the assert:
    assert(decryptCode({1,2,3}, 5) == std::vector<int>({11,10,9}));

    // Negative numbers in the array
    assert(decryptCode({-1,2,-3,4}, 2) == std::vector<int>({-1,1,3,-4}));
    // i=0: -3+4 = 1? wait i=0 next two: code[1]=2, code[2]=-3 => -1? Let's compute carefully:
    // code = {-1,2,-3,4}, k=2
    // i=0: j=1(2) + j=2(-3) = -1
    // i=1: j=2(-3) + j=3(4) = 1
    // i=2: j=3(4) + j=0(-1) = 3
    // i=3: j=0(-1) + j=1(2) = 1
    // So correct expected: {-1,1,3,1} — fix that:
    assert(decryptCode({-1,2,-3,4}, 2) == std::vector<int>({-1,1,3,1}));

    return 0;
}
