// You are given a sequence of N (1 ≤ N ≤ 100,000) integers where each integer is either 0 or 1. The sequence is scanned from left to right, but only up to and including the rightmost occurrence of the value 1 (if there is no 1, the answer is 0). For each position visited in this scan, if the current value is 1, you increment a counter. If the current value is 0, you increment the counter only if the zero is "close enough" to the last seen 1: specifically, you increment if the distance to the last seen 1 is either 1 step (i.e., the 0 is immediately to the right of that 1) OR the 0 is immediately followed by a 1 (i.e., the next element is 1). Zeros that are at least 2 steps away from the last 1 and those that are immediately followed by a 0 are not counted. Write a C++ function `int countSpecialZeros(const std::vector<int>& a)` that returns this total counter value. The function must handle the case where no 1 exists (return 0), and the case where the scan stops early at the last 1. Do not modify the input vector. The function should be efficient for the given constraints.
The problem is a straightforward simulation of the described loop. First, we find the index `lastOne` of the last occurrence of a 1 in the array (searching from the right). If there is no 1, return 0. Then, we iterate from index 0 to `lastOne` inclusive. For each element, we track `lastSeen` which is the index of the most recent 1 encountered so far (initialize to -1). For each position i:
- If `a[i] == 1`: increment answer; update `lastSeen = i`.
- Else (a[i] == 0): only if `lastSeen != -1`, check if `i - lastSeen == 1` (adjacent to last one) OR (i+1 < n and a[i+1] == 1) — the zero is followed by a one. If either condition holds, increment answer. (Note: the condition `i - lastSeen >= 2` and the neighbor check combined replicate the "not counted" case: actually the given code counts if NOT(i-lastSeen >= 2 OR (i<n-1 && a[i+1]==0)), which simplifies to counting if (i-lastSeen == 1) OR (i+1 < n && a[i+1]==1). Because if i-lastSeen == 1, the first OR is false but second is true when a[i+1]==1? Wait carefully. The original condition is: if `a[i]==0` then `if (!(i-lastSeen >= 2 || (i<n-1 && a[i+1]==0)))` → the negation of a disjunction is `(i-lastSeen < 2) && !(i<n-1 && a[i+1]==0)`. That simplifies to: `(i-lastSeen == 1)` (since lastSeen != i because a[i]==0) and `(i==n-1 || a[i+1]==1)`. So indeed the counted zeros are exactly those where the distance from the last 1 is exactly 1 AND (the zero is at the end OR the next element is 1). This is equivalent to: the zero is immediately to the right of a 1 and that zero is either the last element of the whole array or is followed by a 1. So we implement exactly that simplified logic, because the original loop stops at `lastOne`, but the `i+1` check uses the full array length n. However, since we stop at `lastOne`, the zero at `lastOne` cannot exist because `lastOne` is a 1. For zeros before `lastOne`, the condition `i+1<n` and `a[i+1]==1` is safe. The time complexity is O(n) because we scan the array twice (once to find lastOne, once to compute). Space is O(1) extra, not counting input.

Edge cases: All zeros → lastOne = -1 → return 0. Single 1 alone → answer 1 (count the 1). Sequence "1 0 1" – at index 0 count 1, index 1 zero: lastSeen=0, i-lastSeen=1, and a[2]==1 so count, index 2 count 1 → total 3. Sequence "1 0 0 1" – at index 0 count 1, index1 zero: distance 1, a[2]==0 → not counted, index2 zero: distance 2 → not counted, index3 count 1 → total 2. Sequence "0 1 0" – lastOne=1, iterate i=0 (zero but lastSeen=-1 so skip), i=1 count 1 → total 1.

Now produce the reference solution as a free function without main.
#include <vector>

// Count ones and "special" zeros in the prefix ending at the last 1.
// A zero is special if it is immediately after a 1 and is either
// at the end of the array or is followed by a 1.
int countSpecialZeros(const std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    int lastOne = -1;
    for (int i = n - 1; i >= 0; --i) {
        if (a[i] == 1) {
            lastOne = i;
            break;
        }
    }
    if (lastOne == -1) {
        return 0;
    }
    int ans = 0;
    int lastSeen = -1;
    for (int i = 0; i <= lastOne; ++i) {
        if (a[i] == 1) {
            lastSeen = i;
            ++ans;
        } else { // a[i] == 0 and lastSeen != -1 (since i <= lastOne and we have seen a 1 before i)
            if (i - lastSeen == 1 && (i == n - 1 || a[i + 1] == 1)) {
                ++ans;
            }
        }
    }
    return ans;
}
#include <vector>
#include <cassert>

int countSpecialZeros(const std::vector<int>& a);

int main() {
    // No ones -> 0
    assert(countSpecialZeros({0,0,0}) == 0);
    // Single 1 alone
    assert(countSpecialZeros({1}) == 1);
    // 1 0 1 -> count 1, then zero (immediately after 1 and followed by 1) count, then 1 -> total 3
    assert(countSpecialZeros({1,0,1}) == 3);
    // 1 0 0 1 -> count 1 at idx0, zero at idx1 (followed by 0, not counted), zero at idx2 (distance 2, not counted), 1 at idx3 -> total 2
    assert(countSpecialZeros({1,0,0,1}) == 2);
    // 0 1 0 -> lastOne=1, iterate i=0 (zero before any 1, skip), i=1 count 1 -> total 1
    assert(countSpecialZeros({0,1,0}) == 1);
    // 1 0 1 0 -> lastOne=2, iterate: idx0 1 count=1, idx1 zero (distance1 and next=1) count=2, idx2 1 count=3 -> stop. total 3
    assert(countSpecialZeros({1,0,1,0}) == 3);
    // 1 0 1 1 0 -> lastOne=3, idx0 1 count1, idx1 zero (distance1 next=1) count2, idx2 1 count3, idx3 1 count4, stop. total 4
    assert(countSpecialZeros({1,0,1,1,0}) == 4);
    // All ones -> count all
    assert(countSpecialZeros({1,1,1}) == 3);
    // Edge: 0 1 0 1 -> lastOne=3, idx0 zero skip, idx1 1 count1, idx2 zero (distance1 but next=1) count2, idx3 1 count3 -> total 3
    assert(countSpecialZeros({0,1,0,1}) == 3);
    return 0;
}
