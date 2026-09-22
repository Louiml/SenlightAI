/*
Given a string `s` containing only uppercase 'A' and 'B' characters, write a C++ function `chooseCoins` that returns the maximum possible total sum of selected values derived from contiguous segments of 'A's, with the constraint that a segment may be omitted if and only if there exists at least one 'B' immediately preceding it (i.e., the 'B' directly to its left) and at least one 'B' immediately following it (i.e., the 'B' directly to its right). More precisely, define each maximal contiguous block of 'A's as having a value equal to its length. You may choose to include all blocks, or you may exclude at most one block, but only if that block is strictly between two 'B's (i.e., not at the very beginning or very end of the string, and having at least one 'B' on both sides). Determine the maximum total sum of included block lengths under this rule. For example, for `"AABAA"`, there is one block of length 2 at the start and one block of length 2 at the end, and neither can be excluded because each is at an edge; the result is 4. For `"AAABAA"`, blocks are length 3 and 2; the first is at the beginning, the second at the end, so no block can be excluded; result is 5. For `"AABAAA"`, blocks are length 2 and 3, and again neither is between two B's; result is 5. For `"BAAB"`, the only block of length 2 is between two B's, so you may exclude it, giving 0; but including it gives 2, so answer is 2. For `"BAABAA"`, blocks are length 2 (between B's) and length 2 (at end); you can exclude the first block (since it's between B's) giving 2, or include all giving 4; answer is 4. If the string has no 'A's, the answer is 0. If there are no 'B's at all, then no block is between two B's, so all blocks must be included; answer is total length of all 'A' runs. Provide the function that takes a `std::string` and returns a `long long` (to handle large inputs) representing this maximum sum.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

// Given a string of 'A' and 'B', return the maximum sum of lengths of included
// contiguous 'A' segments, where at most one interior segment (with B on both sides)
// may be excluded.
long long chooseCoins(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n == 0) return 0;

    std::vector<long long> runLengths;
    std::vector<bool> excludable;

    int i = 0;
    while (i < n) {
        if (s[i] == 'A') {
            int start = i;
            while (i < n && s[i] == 'A') ++i;
            int end = i - 1;
            long long len = end - start + 1;
            runLengths.push_back(len);
            bool leftB = (start > 0) && (s[start-1] == 'B');
            bool rightB = (end < n-1) && (s[end+1] == 'B');
            excludable.push_back(leftB && rightB);
        } else {
            ++i;
        }
    }

    if (runLengths.empty()) return 0;

    long long total = 0;
    for (long long len : runLengths) total += len;

    bool hasExcludable = false;
    long long minExcludable = 0;
    for (size_t idx = 0; idx < runLengths.size(); ++idx) {
        if (excludable[idx]) {
            if (!hasExcludable || runLengths[idx] < minExcludable) {
                minExcludable = runLengths[idx];
                hasExcludable = true;
            }
        }
    }

    if (!hasExcludable) return total;

    return std::max(total, total - minExcludable);
}

#include <cassert>
#include <string>

long long maxRemaining(const std::string& s);

int main() {
    assert(maxRemaining("") == 0);
    assert(maxRemaining("B") == 0);
    assert(maxRemaining("A") == 1);
    assert(maxRemaining("AA") == 2);
    assert(maxRemaining("BB") == 0);
    // No deletable block because block is at edge
    assert(maxRemaining("AABAA") == 4);
    // Block at start and end, no deletable
    assert(maxRemaining("AAABAA") == 5);
    // Single block between B's, deletable, we delete it -> 0
    assert(maxRemaining("BAB") == 0);
    // Two blocks: one deletable length1, one at edge length2 -> delete the 1
    assert(maxRemaining("BAABAA") == 3); // total=4, delete min deletable=1? Actually runs: "AA" between B's (len2) deletable, "AA" at end (len2) not, total=4, minDeletable=2, answer=2? Wait "BAABAA": B A A B A A -> first run len2 (between B's) deletable, second run len2 at end not, total=4, delete first len2 -> answer 2. Correct answer 2.
    assert(maxRemaining("BAABAA") == 2);
    // Example with multiple deletable blocks, pick smallest
    assert(maxRemaining("BABAB") == 1); // runs len1 at pos1 and len1 at pos3, both deletable, total=2, min=1, answer=1
    // Example with a large interior block and small edge block: must delete interior block
    assert(maxRemaining("BAAAAAB") == 0); // only block len5 between B's, delete it -> 0
    assert(maxRemaining("BAAAAABAA") == 4); // runs: len5 (deletable) and len2 (not), total=7, delete 5 -> 2? Actually total=7, delete len5 -> answer 2. But wait "BAAAAABAA": B AAAAA B AA -> len5 and len2, answer = 7-5=2.
    assert(maxRemaining("BAAAAABAA") == 2);
    // When no deletion possible, keep all
    assert(maxRemaining("AAAABBBBAAAA") == 8); // runs at both ends, no interior B on both sides? Actually first run starts at index0, left none, not deletable; second run ends at end, right none, not deletable. So answer 8.
    // Edge case: string all B's
    assert(maxRemaining("BBBB") == 0);
    // Edge case with single A not between B's
    assert(maxRemaining("AB") == 1);
    assert(maxRemaining("BA") == 1);
    // Single A between B's
    assert(maxRemaining("BAB") == 0);
    return 0;
}

Note: The test file includes a main function with assert checks. The solution function is declared without a definition in the test file (since it's provided separately). In a real combined file, the solution definition would be included before main. The test includes declaration.

I will now output the required sections with the corrected solution and tests.

// The key is to partition the string into maximal contiguous runs of 'A's. Each run has a length (its value). We can include all runs. Optionally, we may exclude exactly one run, but only if that run is "interior," meaning it has at least one 'B' immediately before it and at least one 'B' immediately after it. This condition is equivalent to: the run is neither the first run (if the string starts with 'A') nor the last run (if the string ends with 'A'), and there is at least one 'B' on both sides (which is automatically true for interior runs because runs are separated by B's). However, we must be careful: a run at the very beginning or very end cannot be excluded. Also, if the string begins with 'B', the first run (if any) is still interior? Actually, if the string starts with 'B', then the first run of 'A's has a 'B' to its left (the starting character) and a 'B' to its right (the next B after the run), so it is allowed to be excluded. Similarly, if the string ends with 'B', the last run is allowed. So the valid excludable runs are those that are not at the boundary of the entire string *and* have a 'B' on both sides; but actually being at the boundary means there's no character on one side, so it's not between two B's. So the valid set is all runs that are not at the very start (if string[0]=='A') and not at the very end (if string.back()=='A'). More simply: a run is excludable if and only if it has at least one 'B' immediately to its left and at least one 'B' immediately to its right. That means the run is not adjacent to the string boundaries. For example, in `"BAABAA"`, the first run (position 1-2) has B on left and B on right (since after it there's an A? Wait, after that run there is an A? Actually the string is B A A B A A: first run is indices 1-2, left is B at index0, right is B at index3 → excludable. Second run indices 4-5, left is B at index3, right is end of string → not excludable. So answer is sum of all runs (4) minus smallest excludable run's length (2) = 2? But the example says answer is 4 because we choose to not exclude. So we simply take max of (total sum) and (total sum - length of the smallest excludable run). If no excludable run exists, answer is total sum. The original code snippet processes the string by scanning and tracking `t` which counts consecutive B's up to 2, and uses `cnt` to count something like number of B's adjacent to runs; but the snippet seems to have a bug or different logic. The correct approach: collect lengths of all runs of 'A's into a vector. Also track for each run whether it is excludable: it is excludable if there is at least one 'B' immediately to its left and at least one 'B' immediately to its right. That is, during scanning, we can note if the run is preceded by a 'B' and followed by a 'B'. Simpler: iterate over the string, find runs, record (length, left_has_B, right_has_B). For each run, left_has_B is true if (start_index > 0 && s[start_index-1]=='B'), right_has_B is true if (end_index < n-1 && s[end_index+1]=='B'). Then if both true, it's excludable. Compute total sum. If there is at least one excludable run, the best is max(total_sum, total_sum - min_length_among_excludable_runs). If no excludable runs, answer is total_sum. Edge cases: empty string? Problem likely guarantees non-empty? We'll handle empty by returning 0. String with no 'A's: total_sum=0. String with no 'B's: then no run has a B on both sides (since there are no B's), so no excludable runs, answer total_sum. Complexity: O(n) time and O(number_of_runs) space, which is O(n) worst-case. We can even do it in O(1) extra space by tracking total sum, minimum excludable length, and flag. But using a vector is fine.
