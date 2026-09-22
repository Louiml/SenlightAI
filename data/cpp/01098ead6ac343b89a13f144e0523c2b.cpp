// Write a C++ function `transformBinarySequence` that takes a vector of integers containing only 0s and 1s, where the last element is guaranteed to be 0. The function must return a vector of integers representing "operation indices" such that: starting from the rightmost 0, repeatedly find the nearest run of consecutive 1s immediately to its left, record the count of 1s in that run (or 0 if there are no consecutive 1s directly before the current position), then move left past that entire run and the separating 0(s). Specifically, scan from right to left: for each 0 encountered (including the last one), output the number of consecutive 1s immediately before that 0. If no 1s are immediately before, output 0. Continue this process until reaching the beginning of the array, ensuring the number of outputs equals the number of 0s in the array. The output vector must contain exactly one entry per 0 in the original array, in order from rightmost 0 to leftmost 0. For example, given `[0,1,1,0,1,0]`, the rightmost 0 has one 1 before it → output 1, then skip that 1 and the next 0 (which is also a 0), now at position of the third element (1), but the next 0 to the left is the one at index 2, which has two 1s before it → output 2, then skip those two 1s and the 0 at index 0 → output 0. Final vector: `[1,2,0]`. The function should not modify the input and should handle edge cases like the array starting with a 0 or having consecutive zeros.

#include <cassert>
#include <vector>

// Function declaration (from solution)
std::vector<int> transformBinarySequence(const std::vector<int>& a);

int main() {
    // Example from description: [0,1,1,0,1,0] -> [1,2,0]
    assert(transformBinarySequence({0,1,1,0,1,0}) == std::vector<int>({1,2,0}));

    // Single zero
    assert(transformBinarySequence({0}) == std::vector<int>({0}));

    // One 1 before a zero
    assert(transformBinarySequence({1,0}) == std::vector<int>({1}));

    // Consecutive zeros
    assert(transformBinarySequence({0,0,0}) == std::vector<int>({0,0,0}));

    // Leading zero then a run of 1s then zero
    assert(transformBinarySequence({0,1,1,0}) == std::vector<int>({2,0}));

    // All ones then a final zero
    assert(transformBinarySequence({1,1,1,0}) == std::vector<int>({3}));

    // Alternating pattern
    assert(transformBinarySequence({1,0,1,0}) == std::vector<int>({1,1}));

    // Multiple runs with zeros between
    assert(transformBinarySequence({1,0,1,1,0,0,1,0}) == std::vector<int>({1,0,2,1}));

    // Large run
    std::vector<int> big(1001, 1);
    big.push_back(0); // last is zero, size 1001 ones then 0
    auto res = transformBinarySequence(big);
    assert(res.size() == 1 && res[0] == 1001);

    // Mixed with many zeros
    std::vector<int> mixed = {1,0,0,1,0,0,0};
    assert(transformBinarySequence(mixed) == std::vector<int>({0,0,1,0,0}));
    // Explanation: rightmost zero (index6) -> 0 ones before it? Actually a[5]=0, so 0. Index5 zero -> 0. Index4 zero -> has a[3]=1, so 1. Index2 zero -> a[1]=0, so 0. Index1 zero -> a[0]=1, so 1? Wait: Let's check: mixed = [1,0,0,1,0,0,0]. From rightmost: pos=6 (0), prior=5 (0) not 1 -> count 0 -> push 0, last=5. pos=5 (0), prior=4 (0) not 1 -> count 0 -> push 0, last=4. pos=4 (0), prior=3 (1) -> skip while, prior=3? Actually while (prior>=0 && a[prior]==1): a[3]=1 so prior-- -> prior=2. Then count = last - prior -1 = 4-2-1=1, push 1, last=2. pos=2 (0), prior=1 (0) not 1 -> count = 2-1-1=0 push 0, last=1. pos=1 (0), prior=0 (1) -> while: prior-- to -1 -> count = 1-(-1)-1=1 push 1, last=-1. So result [0,0,1,0,1] but my assertion says [0,0,1,0,0]. That's wrong. So my expected is incorrect. Let's fix: mixed = [1,0,0,1,0,0,0] output should be [0,0,1,0,1]. I'll correct the test.

    assert(transformBinarySequence(mixed) == std::vector<int>({0,0,1,0,1}));

    return 0;
}

#include <vector>

// Given a vector of 0s and 1s with last element guaranteed 0,
// return for each zero (from right to left) the number of consecutive 1s
// immediately to its left. The output has exactly one entry per zero.
std::vector<int> transformBinarySequence(const std::vector<int>& a) {
    std::vector<int> indexes;
    int last = static_cast<int>(a.size()) - 1;
    while (last >= 0) {
        int prior = last - 1;
        while (prior >= 0 && a[prior] == 1) {
            --prior;
        }
        indexes.push_back(last - prior - 1);
        last = prior;
    }
    return indexes;
}

// The problem reduces to a right-to-left scan that groups consecutive 1s that are immediately followed by a 0 (to the right). For each zero in the input (from right to left), we count how many consecutive 1s appear immediately to its left. Then we jump left past those 1s and also past the zero itself, and repeat. Since the last element is always 0, we always start with a valid zero. The number of outputs exactly equals the number of zeros, because each zero we process produces one output and then we skip it. The main algorithm uses a single pass from right to left with two pointers: `pos` tracks the current position (starting at `size-1`), and `prior` moves left to find the start of the run of 1s. For each zero, we push `prior` back until we hit a 0 or the beginning, and the count of 1s is `last - prior`. Then set `last = prior` to continue. Edge cases: if the array begins with a 0, the last iteration will push a single 0 (since `prior` becomes -1, we push 0). Consecutive zeros produce an output of 0 for each zero except the rightmost of a consecutive block? Actually, let's check: if we have `[0,0,1,0]`, starting at last index 3 (0), `prior=2` is 1, so we count 1, push 1, set last=2. Now last=2 is 1, not a zero, but our loop condition is `last >= 0`; we treat it as: find the nearest zero to the right? The original code's logic: it always starts with `last` being the index of a zero? Actually after processing a run, `last` becomes `prior` which is the index of the first 1 in the run (if any) or the zero before the run. The code then continues with the same while loop assuming `last` points to a zero? Not exactly—let's re-read the snippet: `last = prior;` where `prior` is the index of the first 0 encountered when moving left from `last`? Wait, in the snippet, `prior` starts at `last-1` and while `a[prior]==1`, it pushes 0 and decrements. After the while, `prior` now points to either a 0 or -1. Then it pushes `last - prior - 1` which is the count of 1s in that run. Then `last = prior`. So `last` now points to a 0 (or -1). Good. So the algorithm correctly handles consecutive zeros because when `last` points to a zero, `prior = last-1`; if `a[prior]` is not 1 (i.e., 0 or out of bounds), the while doesn't run, and we push `last - prior - 1`. If `a[prior]` is 0, then `prior` stays as `last-1`, and we push `last - (last-1) - 1 = 0`. So consecutive zeros produce a 0 for the left zero. Time complexity is O(n) because each index is visited at most once (or twice for the push of 0s inside the while, but those are part of the output). Space complexity O(k) where k is number of zeros, which is O(n) in worst case. The output must have exactly one entry per zero.
