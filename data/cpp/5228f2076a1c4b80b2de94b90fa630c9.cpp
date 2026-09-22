/*
Write a C++ function named `buildArrayFromStream` that takes a strictly increasing vector of integers `target` (each between 1 and n) and an integer `n`, and simulates building the `target` array using a stream of integers from 1 to n. At each step, you may either "Push" the current stream integer onto the result or "Push" and then immediately "Pop" it. Return the sequence of operations (as strings "Push" and "Pop") that results in the final stack/array exactly equal to `target`. You must stop as soon as the target is fully matched (do not process remaining stream numbers). The returned vector must be in the exact order of operations.
*/
#include <vector>
#include <string>

// Simulate building the target array from a stream of integers 1..n.
// Return the sequence of "Push"/"Pop" operations to produce exactly target.
std::vector<std::string> buildArrayFromStream(const std::vector<int>& target, int n) {
    std::vector<std::string> result;
    int targetIndex = 0;
    int streamValue = 1;
    
    while (targetIndex < static_cast<int>(target.size()) && streamValue <= n) {
        result.push_back("Push");
        if (target[targetIndex] == streamValue) {
            ++targetIndex;
        } else {
            result.push_back("Pop");
        }
        ++streamValue;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (as above)
std::vector<std::string> buildArrayFromStream(const std::vector<int>& target, int n);

int main() {
    // Basic case from the prompt
    assert(buildArrayFromStream({1, 3}, 3) == std::vector<std::string>({"Push", "Push", "Pop", "Push"}));
    
    // Target equals entire stream
    assert(buildArrayFromStream({1, 2, 3}, 3) == std::vector<std::string>({"Push", "Push", "Push"}));
    
    // Target only has a single high value
    assert(buildArrayFromStream({4}, 4) == std::vector<std::string>({"Push", "Pop", "Push", "Pop", "Push", "Pop", "Push"}));
    
    // Empty target
    assert(buildArrayFromStream({}, 5).empty());
    
    // Target that is a suffix of the stream
    assert(buildArrayFromStream({2, 3}, 3) == std::vector<std::string>({"Push", "Pop", "Push", "Push"}));
    
    // Target equal to 1
    assert(buildArrayFromStream({1}, 1) == std::vector<std::string>({"Push"}));
    
    // All elements need pushing and popping except the last
    assert(buildArrayFromStream({3}, 3) == std::vector<std::string>({"Push", "Pop", "Push", "Pop", "Push"}));
    
    // Large n, target is only first element
    auto resultLarge = buildArrayFromStream({1}, 100);
    assert(resultLarge.size() == 1 && resultLarge[0] == "Push");
    
    // Target with gaps in the middle
    assert(buildArrayFromStream({1, 4}, 4) == std::vector<std::string>({"Push", "Push", "Pop", "Push", "Pop", "Push", "Pop", "Push"}));
    
    return 0;
}
// The core idea is a greedy one-pass simulation. We maintain a pointer `i` into the `target` array and iterate through stream numbers from 1 to n. For each stream value, we always perform a "Push" operation. If the current stream value equals `target[i]`, we advance `i` (the matched element is kept). Otherwise, since the stream value is not needed for the target, we immediately "Pop" it. This continues until the entire target is matched (`i == target.size()`) or we exhaust the stream (`stream > n`). Because `target` is strictly increasing, the order of matches is guaranteed, and this greedy approach always produces a valid minimal (and unique) sequence of operations. Edge cases: if `target` is empty, the result is empty; if the last element of `target` equals `n`, the loop stops exactly when `i` reaches the end; we never push after the target is fully built. Time complexity is O(n + target.size()) in the worst case (since we may push/pop elements not in target), but more precisely it’s O(n) because we iterate at most n stream numbers, and each iteration does constant work. Space complexity is O(target.size()) for the output vector (which in the worst case is O(n)).
