Write a C++ function `countAcceptedCombinations` that takes a vector of workflow definitions (each as a string in the format `name{rule1,rule2,...,final}` where each rule is `attribute<comparison>value:target` and the final rule has no colon) and returns the total number of distinct combinations of integer values for attributes `x`, `m`, `a`, `s` (each in the inclusive range 1..4000) that are accepted when starting at the workflow named `"in"`. Use a recursive range‑splitting approach similar to the provided snippet: each workflow either accepts or rejects a whole 4‑dimensional box, or splits it along one dimension when a rule threshold cuts through the box. You may assume all attribute names are exactly `'x'`, `'m'`, `'a'`, `'s'`, comparisons are `<` or `>`, workflow names are lowercase letters, and the special workflows `"A"` and `"R"` always exist and are never defined as rules. The function should be self‑contained, not rely on global state, and handle arbitrary numbers of workflows and rules.
The problem is a 4‑dimensional hyper‑rectangle counting problem. Each workflow is a sequence of conditional rules; if a part (a point in the 4‑D space) satisfies a rule’s condition, it is sent to the rule’s target workflow; otherwise it proceeds to the next rule, and if none match, it goes to the workflow’s final target. To count accepted combinations without enumerating up to 4000⁴ points, we recursively process boxes defined by inclusive lower and upper bounds for each attribute. For a given box and workflow:
- If the workflow is `"A"`, the entire box is accepted, so add the product of the lengths of each dimension (max−min+1) to the total.
- If the workflow is `"R"`, the entire box is rejected.
- Otherwise, iterate through the rules. For a rule `c op v` (where `op` is `<` or `>`):
  - If the entire box satisfies the rule (e.g., for `>`: min of attribute `c` > v), then recurse on the whole box with the rule’s target workflow and stop processing further rules (because every point in the box would match this rule).
  - If the entire box violates the rule (e.g., for `>`: max of attribute `c` ≤ v), then skip this rule and continue to the next rule.
  - Otherwise, the box is split: the part that satisfies the rule (call it `passBox`) and the part that does not (call it `failBox`). Recurse on `passBox` with the rule’s target workflow, then set the current box to `failBox` and continue with the next rule. The split is done along the single attribute `c`: for `>`, the passBox has `cmin = oldValue+1` (clip to min) and the failBox has `cmax = oldValue`; for `<`, the passBox has `cmax = oldValue-1` and the failBox has `cmin = oldValue`. If the split produces an empty interval (min > max) for either part, that part is simply not recursed.
After processing all rules, recurse with the workflow’s final target on the remaining (unmatched) box.
Edge cases: empty boxes must be ignored (return immediately). The initial box is [1,4000] for all attributes. Workflows may reference targets that are not yet defined; one can create placeholder mappings on the fly. Time complexity is O(R * W) where R is the number of rules and W is the number of times a box is split; in the worst case, the number of distinct intervals per attribute is O(R*W), but in practice each workflow splits each attribute at most once per bound value, and the recursion terminates because each split strictly reduces the size of the box along one dimension. Space complexity is O(depth * number of workflows) for the recursion stack, which is bounded by the number of distinct split points (at most 4000 per attribute) times the workflow chain length.
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdint>

// Internal representation of a workflow rule.
struct Rule {
    char attr;          // 'x', 'm', 'a', or 's'
    bool greater;       // true if '>', false if '<'
    int value;          // comparison threshold
    std::string target; // workflow name to go to if rule matches
};

// Internal representation of a workflow.
struct Workflow {
    std::string name;
    std::vector<Rule> rules;
    std::string finalTarget;
};

// A 4‑D hyper‑rectangle defined by inclusive bounds.
struct Range {
    int xmin, xmax;
    int mmin, mmax;
    int amin, amax;
    int smin, smax;
};

// Helper to get min/max of a specific attribute in a range.
int getMin(const Range& r, char c) {
    switch(c) {
        case 'x': return r.xmin;
        case 'm': return r.mmin;
        case 'a': return r.amin;
        case 's': return r.smin;
    }
    return 0;
}

int getMax(const Range& r, char c) {
    switch(c) {
        case 'x': return r.xmax;
        case 'm': return r.mmax;
        case 'a': return r.amax;
        case 's': return r.smax;
    }
    return 0;
}

// Set min of a specific attribute to the given value.
void setMin(Range& r, char c, int v) {
    switch(c) {
        case 'x': r.xmin = v; break;
        case 'm': r.mmin = v; break;
        case 'a': r.amin = v; break;
        case 's': r.smin = v; break;
    }
}

// Set max of a specific attribute to the given value.
void setMax(Range& r, char c, int v) {
    switch(c) {
        case 'x': r.xmax = v; break;
        case 'm': r.mmax = v; break;
        case 'a': r.amax = v; break;
        case 's': r.smax = v; break;
    }
}

// Check if a range is empty (any dimension has min > max).
bool isEmpty(const Range& r) {
    return r.xmin > r.xmax || r.mmin > r.mmax || r.amin > r.amax || r.smin > r.smax;
}

// Recursively count accepted combinations in the given range for the workflow.
void countRange(
    const std::string& wfName,
    const Range& range,
    const std::unordered_map<std::string, Workflow>& workflows,
    std::int64_t& total
) {
    if (wfName == "A") {
        // All combinations in the box are accepted.
        std::int64_t combos = 1;
        combos *= static_cast<std::int64_t>(range.xmax - range.xmin + 1);
        combos *= static_cast<std::int64_t>(range.mmax - range.mmin + 1);
        combos *= static_cast<std::int64_t>(range.amax - range.amin + 1);
        combos *= static_cast<std::int64_t>(range.smax - range.smin + 1);
        total += combos;
        return;
    }
    if (wfName == "R") {
        return;
    }

    auto it = workflows.find(wfName);
    if (it == workflows.end()) return; // should not happen
    const Workflow& wf = it->second;

    Range current = range; // the unmatched part of the box

    for (const Rule& rule : wf.rules) {
        int curMin = getMin(current, rule.attr);
        int curMax = getMax(current, rule.attr);
        bool wholeRulePass, wholeRuleFail;

        if (rule.greater) {
            wholeRulePass = (curMin > rule.value);
            wholeRuleFail = (curMax <= rule.value);
        } else { // less
            wholeRulePass = (curMax < rule.value);
            wholeRuleFail = (curMin >= rule.value);
        }

        if (wholeRuleFail) {
            // No part of the current box passes, so skip this rule.
            continue;
        }

        // At least part of the box passes.
        if (wholeRulePass) {
            // Entire box passes -> go to target and we are done with this workflow.
            countRange(rule.target, current, workflows, total);
            return;
        }

        // Partial: split the box.
        Range passBox = current;
        Range failBox = current;

        if (rule.greater) {
            // Pass: attr > value → min = value+1
            setMin(passBox, rule.attr, rule.value + 1);
            // Fail: attr <= value → max = value
            setMax(failBox, rule.attr, rule.value);
        } else { // less
            // Pass: attr < value → max = value-1
            setMax(passBox, rule.attr, rule.value - 1);
            // Fail: attr >= value → min = value
            setMin(failBox, rule.attr, rule.value);
        }

        if (!isEmpty(passBox)) {
            countRange(rule.target, passBox, workflows, total);
        }
        // Continue with the failing part for the next rule.
        current = failBox;
        if (isEmpty(current)) {
            return; // nothing left to process
        }
    }

    // All rules processed, go to final target.
    countRange(wf.finalTarget, current, workflows, total);
}

// Public function: parses workflow definitions and returns total accepted combinations.
std::int64_t countAcceptedCombinations(const std::vector<std::string>& definitions) {
    std::unordered_map<std::string, Workflow> workflows;
    // Always include A and R.
    workflows["A"] = Workflow{"A", {}, "A"};
    workflows["R"] = Workflow{"R", {}, "R"};

    // Parse each definition line: name{rule1,rule2,...,final}
    for (const std::string& line : definitions) {
        size_t brace = line.find('{');
        std::string name = line.substr(0, brace);
        std::string inner = line.substr(brace + 1, line.size() - brace - 2); // strip trailing '}'

        Workflow wf;
        wf.name = name;

        std::stringstream ss(inner);
        std::string token;
        while (std::getline(ss, token, ',')) {
            // Check if token is a rule (contains ':') or the final target.
            size_t colon = token.find(':');
            if (colon != std::string::npos) {
                Rule r;
                r.attr = token[0];
                r.greater = (token[1] == '>');
                r.value = std::stoi(token.substr(2, colon - 2));
                r.target = token.substr(colon + 1);
                wf.rules.push_back(r);
            } else {
                // final target
                wf.finalTarget = token;
            }
        }
        workflows[name] = wf;
    }

    Range initial;
    initial.xmin = initial.mmin = initial.amin = initial.smin = 1;
    initial.xmax = initial.mmax = initial.amax = initial.smax = 4000;

    std::int64_t total = 0;
    countRange("in", initial, workflows, total);
    return total;
}
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

// The solution function is declared elsewhere; include here for testing.

int main() {
    // Test 1: Single workflow that accepts everything.
    {
        std::vector<std::string> defs = {"in{x<4001:A}"};
        assert(countAcceptedCombinations(defs) == 4000LL * 4000LL * 4000LL * 4000LL);
    }

    // Test 2: Reject everything.
    {
        std::vector<std::string> defs = {"in{x>0:R}"};
        assert(countAcceptedCombinations(defs) == 0);
    }

    // Test 3: Accept only x=1, others any.
    {
        std::vector<std::string> defs = {"in{x<1:A,R}"}; // x<1 passes, else reject
        assert(countAcceptedCombinations(defs) == 1LL * 4000LL * 4000LL * 4000LL);
    }

    // Test 4: Accept x>2000 (i.e., 2001..4000) and m<1000.
    {
        std::vector<std::string> defs = {
            "in{x>2000:check,R}",
            "check{m<1000:A,R}"
        };
        // x: 2000, m: 1..999, a,s any → 2000 * 999 * 4000 * 4000
        assert(countAcceptedCombinations(defs) == 2000LL * 999LL * 4000LL * 4000LL);
    }

    // Test 5: Chained rules splitting into two accepted regions.
    {
        std::vector<std::string> defs = {
            "in{x>2000:high,low}",
            "high{A}",
            "low{m<100:A,R}"
        };
        // high: x 2001..4000 any m → 2000*4000*4000*4000
        // low fails m>=100, so only m 1..99 and x 1..2000 → 2000*99*4000*4000
        std::int64_t expected = 2000LL * 4000LL * 4000LL * 4000LL + 2000LL * 99LL * 4000LL * 4000LL;
        assert(countAcceptedCombinations(defs) == expected);
    }

    // Test 6: Nested workflow with no final (only rules), and a final target.
    {
        std::vector<std::string> defs = {
            "in{a>1000:t1,t2}",
            "t1{s<500:A,R}",
            "t2{x>3000:A,R}"
        };
        // t1: a>1000 (3000) and s<500 (499) → 3000*499*4000*4000
        // t2: a≤1000 (1000) and x>3000 (1000) → 1000*1000*4000*4000
        std::int64_t expected = 3000LL * 499LL * 4000LL * 4000LL + 1000LL * 1000LL * 4000LL * 4000LL;
        assert(countAcceptedCombinations(defs) == expected);
    }

    return 0;
}
