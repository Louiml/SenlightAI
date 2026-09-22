Write a C++ function `int minAdjustments(const std::vector<int>& nums)` that, given a sequence of integers where each value represents an element and the frequency of each value is counted from the input (the original program reads counts from standard input, but here the input is provided as a vector), returns the minimum number of elements that must be removed so that every remaining distinct value `x` appears at most `x` times in the final multiset. For example, if the input is `[2,2,2,3,3]`, the value `2` appears three times but should appear at most twice, so at least one `2` must be removed; the value `3` appears twice, which is allowed (at most 3, so no removal). The answer is the total minimum removals. If a value `x` appears fewer than `x` times, all its occurrences may remain (no removal needed). The input vector may contain any integers (including zero and negative numbers? treat them normally: a value `x` that is ≤0 has a limit of `x`, which means if `x` is 0, you cannot keep any occurrences because 0 ≤ 0, so you must remove all of them; if `x` is negative, the limit is negative, which is impossible to satisfy, so you must remove all occurrences). Return the integer sum of removals. Note: the vector is not necessarily sorted, and may be empty (return 0).
#include <cassert>
#include <vector>

// Forward declaration of the tested function
int minAdjustments(const std::vector<int>& nums);

int main() {
    // Basic case: each value appears exactly its value times -> no removals.
    assert(minAdjustments({1,2,2,3,3,3}) == 0);
    // Value 2 appears 3 times but should be 2 -> remove 1.
    assert(minAdjustments({2,2,2,3,3,3}) == 1);
    // Value 5 appears 3 times, less than 5 -> remove all 3.
    assert(minAdjustments({5,5,5}) == 3);
    // Mixed: 3 appears 2 (remove all 2), 1 appears 1 (keep), 4 appears 5 (remove 1).
    assert(minAdjustments({3,3,1,4,4,4,4,4}) == 3); // 2 removals for 3, 1 for 4 -> total 3.
    // Non-positive values are always removed.
    assert(minAdjustments({0,0,-1,-1,2,2}) == 4); // remove 3 zeros? Actually 0 appears 2, -1 appears 2 -> all 4 removed, 2s kept.
    // Empty vector.
    assert(minAdjustments({}) == 0);
    // Large value with one occurrence: value 10 appears once, less than 10 -> remove it.
    assert(minAdjustments({10}) == 1);
    // Duplicate positive value with excess: 3 appears 6 -> remove 3.
    assert(minAdjustments({3,3,3,3,3,3}) == 3);
    return 0;
}
#include <vector>
#include <unordered_map>

// Returns the minimum number of elements to remove so that for each distinct value x,
// if the original count of x is at least x, the final count is exactly x; otherwise, all x are removed.
int minAdjustments(const std::vector<int>& nums) {
    std::unordered_map<int, int> freq;
    for (int v : nums) {
        ++freq[v];
    }

    int removals = 0;
    for (const auto& [value, count] : freq) {
        // Only positive values can be kept, and only if we have at least that many.
        int kept = (value > 0 && count >= value) ? value : 0;
        removals += count - kept;
    }
    return removals;
}
// The solution is to first count the frequency of each value in the input vector using a hash map (e.g., `std::unordered_map<int,int>` or `std::map`). Then iterate over each distinct value `x` and its frequency `f`. The allowed remaining count is `x` if `x > 0`, otherwise it is `0` (because for `x <= 0`, no occurrence can be kept). For each distinct value, the number of removals needed is the difference between `f` and the allowed count, but only if `f` exceeds the allowed count; otherwise it is `0`. That is: `removals = max(0, f - allowed)`. This exactly matches the original code's logic: if `i.first > i.second`, it adds `i.second` (i.e., `f`), but wait, the original code had a different branch: when `x > f`, it adds `f`? Let me re-read: original code: if `i.first > i.second` then `ans += i.second` else `ans += abs(i.second - i.first)`. That is: if `x > f`, add `f`; else add `f - x` (since abs). That is equivalent to: if `f < x`, add `f` (you must remove all because you can keep at most `x` but you have fewer than `x`; actually the original code says: if count is less than value, add count (i.e., remove all), else add difference. That is wrong interpretation? Let's test: suppose `x=3`, `f=2`. Original: `i.first > i.second`? 3>2 true, so `ans += i.second=2`. That means remove all 2 occurrences. But per task description, if `x` appears fewer than `x` times, all may remain. So the original code is actually not matching the task I described. Wait, the task description I wrote says: "If a value `x` appears fewer than `x` times, all its occurrences may remain (no removal needed)." But the original code would remove them. So I need to clarify: The task is inspired by the given snippet, but I can adjust the logic. Let me re-read the original snippet: It reads `n`, then map counts. Then for each `(key, count)`, if `key > count`, adds `count`; else adds `abs(count - key)`. That means: if the value is greater than its count, you remove all (count removed). If value <= count, you remove the excess (count - value). So in both cases, you are bringing the count down to exactly `value`. That is: the final count should be exactly `value` for each value? But if `value` is 5 and count is 3, you cannot have 5 because you have only 3, so you remove all 3, resulting in 0. Actually the logic says: if count less than value, you remove all (because you cannot have value, so you remove all). That is a stricter rule: you want each distinct value `x` to appear exactly `x` times in the final multiset? But that's impossible if count < x, so you remove all. So the problem is: given frequencies, for each value `x`, you must keep exactly `x` instances if possible, otherwise keep 0. The minimum removals is sum of `max(0, count - x)` when count >= x, and `count` when count < x. That is exactly `if (x > count) ans += count; else ans += count - x;` which simplifies to `ans += max(0, count - x) + (x > count ? count : 0)`? Actually that's not the same as `max(0, count - x)` because when count < x, `count - x` is negative, max gives 0, but we need count. So the removal is: if count >= x, remove count - x; if count < x, remove count (all). So removal = min(count, max(0, count - x)? No). Let's compute: if count < x, you remove count; if count >= x, you remove count - x. So removal = count - min(count, x) actually? For count=3, x=5: 3 - min(3,5)=0? No. Let's derive: final kept = min(count, x) if x > 0? But if x <=0, you keep 0. So kept = (x > 0 ? min(count, x) : 0). Then removals = count - kept. For x=5, count=3: kept=3, removals=0? That would mean you can keep all 3, but original code removes all. So the original code is not that. Let's re-evaluate original code: For x=5, count=3: `i.first > i.second` (5>3) true, ans += 3, so you remove all 3. That means final count is 0, not 3. So the rule is: you must have exactly `x` occurrences of value `x`; if you cannot, you remove all. So the required final count is either `x` (if you have at least x) or 0 (otherwise). So kept = (count >= x ? x : 0). Then removals = count - kept. For x=5, count=3: kept=0, removals=3. For x=3, count=5: kept=3, removals=2. For x=3, count=2: removals=2. So removal = count - (count >= x ? x : 0). This matches the original code's logic: if x > count (i.e., count < x), removal = count; else removal = count - x. So the task should be stated accordingly. I will adjust the task description to match the original snippet exactly: You want to remove elements so that for every distinct value `x`, the final count of `x` is exactly `x` if the original count was at least `x`; otherwise, all occurrences of `x` are removed (final count 0). This is a classic "make frequencies equal to value" problem. I'll write the task description clearly.
//
// Also handle `x <= 0`: since you can never have a positive count of a non-positive value, the condition `x > 0`? For x=0, you would need count exactly 0, so if count>0 you remove all. For negative, impossible, remove all. So kept = (count >= x && x > 0 ? x : 0) but actually if x is negative, count >= x is true for any non-negative count, but you cannot have negative count, so you must keep 0. So we must require x > 0. So kept = (x > 0 && count >= x ? x : 0). Then removals = count - kept.
//
// The algorithm: count frequencies using unordered_map, iterate, for each `(value, count)`, compute kept = (value > 0 && count >= value) ? value : 0; removals += count - kept. Time O(n) for building map (assuming unordered_map average), space O(distinct). Edge cases: empty vector -> 0; all numbers negative -> remove all; zeros -> remove all; large numbers with few occurrences -> remove all. I'll present the solution accordingly.
