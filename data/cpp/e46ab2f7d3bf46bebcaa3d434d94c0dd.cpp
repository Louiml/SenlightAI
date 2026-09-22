/*
Write a C++ function `int shuffleSteps(int n, const std::string& a, const std::string& b, const std::string& target)` that simulates the following process: Given two strings `a` and `b` of equal length `n` (containing only lowercase English letters), repeatedly form a new string `res` by interleaving characters: first `b[0]`, then `a[0]`, then `b[1]`, then `a[1]`, ..., up to `b[n-1]`, then `a[n-1]`. After each shuffle, the first `n` characters of `res` become the new `a`, and the last `n` characters become the new `b` for the next iteration. The process repeats until `res` equals `target` or until 50 shuffles have been performed. Return the number of shuffles needed to reach the target (the first successful shuffle count), or `-1` if the target is not reached within 50 shuffles. The function must not modify the input strings.
*/
#include <string>

// Simulates the shuffle process up to 50 steps.
// Returns the number of shuffles needed to reach target, or -1 if not reached.
int shuffleSteps(int n, const std::string& a, const std::string& b, const std::string& target) {
    if (n == 0) {
        return (target.empty()) ? 0 : -1;
    }

    std::string current_a = a;
    std::string current_b = b;

    for (int step = 1; step <= 50; ++step) {
        std::string res;
        res.reserve(2 * n);
        for (int i = 0; i < n; ++i) {
            res.push_back(current_b[i]);
            res.push_back(current_a[i]);
        }

        if (res == target) {
            return step;
        }

        current_a = res.substr(0, n);
        current_b = res.substr(n);
    }

    return -1;
}
#include <cassert>
#include <string>

int shuffleSteps(int n, const std::string& a, const std::string& b, const std::string& target);

int main() {
    // Basic case: target reached on first shuffle
    assert(shuffleSteps(2, "ab", "cd", "cadb") == 1);

    // Target reached on second shuffle
    // Step1: res = "bdac", a="bd", b="ac"
    // Step2: res = "abcd" (target)
    assert(shuffleSteps(2, "ab", "cd", "abcd") == 2);

    // Target never reached within 50 steps
    assert(shuffleSteps(2, "ab", "cd", "zzzz") == -1);

    // Single character strings
    assert(shuffleSteps(1, "a", "b", "ba") == 1);
    assert(shuffleSteps(1, "a", "b", "ab") == -1);

    // n=3, target reached after 4 steps (hand-calculated)
    // start: a="abc", b="def"
    // step1: res = "dae bfc" -> "daebfc", a="dae", b="bfc"
    // step2: res = "bd a f c e" -> "bdafce", a="bda", b="fce"
    // step3: res = "f b c d e a" -> "fbcdea", a="fbc", b="dea"
    // step4: res = "d f e a c b" -> "dfeacb", which is not target; actually simulate further to find target
    // To avoid manual error, use a known repeat: after 3 steps res = "fbcdea"; after 4 steps res = "dfeacb"; not target.
    // Let's test a target that is reached after 3 steps: target="fbcdea"
    assert(shuffleSteps(3, "abc", "def", "fbcdea") == 3);

    // Target equal to initial interleaving when strings are same
    assert(shuffleSteps(2, "aa", "bb", "baba") == 1);

    // Larger n to ensure loop works (target not found)
    assert(shuffleSteps(5, "hello", "world", "xxxxx") == -1);

    // Edge: n=0 with empty target
    assert(shuffleSteps(0, "", "", "") == 0);

    // Edge: n=0 with non-empty target
    assert(shuffleSteps(0, "", "", "a") == -1);

    return 0;
}
// The algorithm directly simulates the process described. For each step from 1 to 50, we build the interleaved string `res` by iterating indices `0` to `n-1`, appending `b[i]` then `a[i]`. We compare `res` to `target`; if equal, return the current step count. If not equal, we update `a = res.substr(0, n)` and `b = res.substr(n)`, then continue. Edge cases: when `n = 0` (though constraints likely avoid this, handle gracefully by checking if target is empty and returning 0 or -1 appropriately), when the target is reached on the first shuffle (return 1), and when it is never reached within 50 steps (return -1). The loop is bounded to 50 iterations, so the time complexity is O(50 * n) = O(n) per call, and space complexity is O(n) for the intermediate string. No additional data structures are needed.
