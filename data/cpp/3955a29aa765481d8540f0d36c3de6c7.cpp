Write a C++ function `std::vector<std::string> optimizeSteps(std::vector<std::string> steps)` that takes a vector of strings representing XPath location path steps (in order from left to right), where each step is one of these exact tokens: `"descendant-or-self::node()"`, `"child::*"`, `"child::foo"`, or `"descendant::*"`. The function must detect the specific pattern where a first step is exactly `"descendant-or-self::node()"` and the second step is exactly `"child::*"` or `"child::foo"`. In that case, the two steps should be merged into a single step: if the second step is `"child::*"`, the result is `"descendant::*"`; if the second step is `"child::foo"`, the result is `"descendant::foo"`. The merged step replaces both, and then the remaining vector is returned (preserving order and any other steps). If no such pattern exists, return the input unchanged. This mirrors the `optimizeStepPair` logic for the common case of `//` (descendant-or-self::node()/child::NodeTest) being simplified to `descendant::NodeTest`. You may assume steps are always valid and contain no extra whitespace.
The solution scans the vector left-to-right. Whenever we find two consecutive elements matching the exact pattern — first is `"descendant-or-self::node()"` and second is either `"child::*"` or `"child::foo"` — we replace them with the corresponding merged step (`"descendant::*"` or `"descendant::foo"`). After merging, we continue scanning from the position where the merged step was placed (so that a subsequent pair could also match if the merged step were followed by another child step, though in this simple task that won't happen because merged steps are descendant, not descendant-or-self). The key edge cases: if the vector has fewer than two elements, return unchanged. If the first step matches but the second does not, leave both as-is and move past the first step. The algorithm runs in O(m) time where m is the number of steps, and uses O(1) extra space besides the output vector. It is important to compare strings exactly (no partial matching, no case-insensitivity).
#include <string>
#include <vector>

// Given a sequence of XPath steps as strings, apply the optimizeStepPair pattern:
// If a step "descendant-or-self::node()" is immediately followed by "child::*" or "child::foo",
// merge them into "descendant::*" or "descendant::foo" respectively, replacing both steps with one.
std::vector<std::string> optimizeSteps(const std::vector<std::string>& steps) {
    std::vector<std::string> result;
    result.reserve(steps.size());

    for (size_t i = 0; i < steps.size(); ++i) {
        // Check if current step is the descendant-or-self pattern and there is a next step.
        if (steps[i] == "descendant-or-self::node()" && i + 1 < steps.size()) {
            const std::string& next = steps[i + 1];
            if (next == "child::*") {
                result.push_back("descendant::*");
                ++i; // Skip the second step
                continue;
            }
            if (next == "child::foo") {
                result.push_back("descendant::foo");
                ++i; // Skip the second step
                continue;
            }
        }
        // Otherwise, just copy the current step.
        result.push_back(steps[i]);
    }

    return result;
}
#include <cassert>
#include <string>
#include <vector>

// (Solution function from above would be placed here or in a separate header.)

int main() {
    using V = std::vector<std::string>;

    // Basic merge cases
    assert(optimizeSteps({"descendant-or-self::node()", "child::*"}) == V({"descendant::*"}));
    assert(optimizeSteps({"descendant-or-self::node()", "child::foo"}) == V({"descendant::foo"}));

    // No pattern: unchanged
    assert(optimizeSteps({"child::*", "child::foo"}) == V({"child::*", "child::foo"}));
    assert(optimizeSteps({"descendant-or-self::node()", "descendant::*"}) == V({"descendant-or-self::node()", "descendant::*"}));

    // Single step
    assert(optimizeSteps({"descendant-or-self::node()"}) == V({"descendant-or-self::node()"}));
    assert(optimizeSteps({}) == V({}));

    // Pattern in middle, with extra steps before and after
    assert(optimizeSteps({"child::bar", "descendant-or-self::node()", "child::*", "child::baz"})
           == V({"child::bar", "descendant::*", "child::baz"}));
    assert(optimizeSteps({"child::bar", "descendant-or-self::node()", "child::foo", "child::baz"})
           == V({"child::bar", "descendant::foo", "child::baz"}));

    // Pattern at the front, with following step
    assert(optimizeSteps({"descendant-or-self::node()", "child::*", "child::baz"})
           == V({"descendant::*", "child::baz"}));

    // Pattern at the end
    assert(optimizeSteps({"child::bar", "descendant-or-self::node()", "child::*"})
           == V({"child::bar", "descendant::*"}));

    // Two consecutive patterns: only the first merges, then the second is left alone because merged step is not descendant-or-self
    assert(optimizeSteps({"descendant-or-self::node()", "child::*", "descendant-or-self::node()", "child::foo"})
           == V({"descendant::*", "descendant::foo"})); // Wait: after merging first pair, result is {"descendant::*", "descendant-or-self::node()", "child::foo"} — but our algorithm merges both pairs in one pass? Let's re-check: 
    // Actually the above is not valid because the second pattern is still present and the algorithm merges it too. 
    // The correct expected result is {"descendant::*", "descendant::foo"}. So keep this assertion as is.
    return 0;
}
