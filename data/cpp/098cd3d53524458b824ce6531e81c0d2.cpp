/*
Given an array of integers \( a_1, a_2, \dots, a_n \) (with \( 1 \le n \le 5000 \) and each \( |a_i| \le 100000 \)), write a C++ function `int countSpecialIndexes(const std::vector<int>& arr)` that returns the number of indices \( i \) (1-indexed) such that there exist two indices \( j, k \) with \( 1 \le j < i \) and \( 1 \le k \le i \) satisfying \( a_i - a_j = a_i + a_k \). Essentially, count how many positions \( i \) can be expressed as a difference between an earlier element and a sum of an earlier-or-equal element. The result is guaranteed to fit in a 32-bit signed integer.
*/

#include <vector>
#include <array>
#include <algorithm>

// Count indices i (0-based) such that either:
//   - there exists j < i with arr[j] == -arr[i], or
//   - there exist j, k < i with arr[j] + arr[k] == 0.
// Value range of arr elements is in [-100000, 100000].
int countSpecialIndexes(const std::vector<int>& arr) {
    constexpr int OFFSET = 100000;
    constexpr int RANGE = 200000 + 2; // to accommodate -100000..100000
    std::array<bool, RANGE> seen{};
    bool prefixHasZeroSum = false; // whether any pair among seen elements sums to 0

    int count = 0;
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        int value = arr[i];

        // Check condition for index i
        bool good = prefixHasZeroSum; // already had a zero-sum pair before i
        if (!good) {
            // Check if there is an earlier element equal to -value
            if (seen[-value + OFFSET]) {
                good = true;
            }
        }
        if (good) {
            ++count;
        }

        // Now add value to the seen set and update prefixHasZeroSum
        // A new zero-sum pair can be formed if -value is already seen, or if value == 0
        if (seen[-value + OFFSET] || value == 0) {
            prefixHasZeroSum = true;
        }
        seen[value + OFFSET] = true;
    }
    return count;
}

#include <cassert>
#include <vector>

int countSpecialIndexes(const std::vector<int>& arr); // declaration

int main() {
    // Single element: no earlier index, count = 0
    assert(countSpecialIndexes({5}) == 0);

    // No zero-sum possible at all
    assert(countSpecialIndexes({1, 2, 3}) == 0);

    // Pair with itself: a[i] = 0, then for i=1 (0-based) it has j=0 with arr[0]=0? Actually 0+0=0, so index 0? No condition for i=0 because no earlier. For i=1, arr[1]=0, earlier arr[0]=0, so condition true (0+0=0). So count=1.
    assert(countSpecialIndexes({0, 0}) == 1);

    // Pair (i with earlier -i)
    assert(countSpecialIndexes({5, -5, 7}) == 1); // index 1 (value -5) pairs with index 0 (5)

    // Prefix zero-sum pair: {3, -3} at indices 0 and 1, then any later index qualifies
    assert(countSpecialIndexes({3, -3, 100, 200}) == 2); // indices 2 and 3

    // Mixed: earlier zero-sum and direct negation
    assert(countSpecialIndexes({2, -2, 5, -5, 0}) == 3); // indices 1,3,4? Let's compute:
    // i=0: none
    // i=1 (-2): has earlier 2 -> yes
    // i=2 (5): prefix has zero-sum (2,-2) -> yes
    // i=3 (-5): prefix has zero-sum -> yes, also direct -5 with 5? but 5 is earlier, yes
    // i=4 (0): prefix has zero-sum -> yes
    // total 4? Actually indices 1,2,3,4 all qualify? Check i=2: is there any (j,k) with j<2,k≤2 such that sum=0? (0,1) gives 2+(-2)=0, yes. So i=2 qualifies. i=3: (0,1) qualifies. i=4: (0,1) qualifies. So count = 4.
    // Let's adjust.
    assert(countSpecialIndexes({2, -2, 5, -5, 0}) == 4);

    // Large values within range
    assert(countSpecialIndexes({100000, -100000}) == 1);

    // Duplicate zeros
    assert(countSpecialIndexes({0, 0, 0}) == 2); // i=1 and i=2

    // Random check with known property
    assert(countSpecialIndexes({1, -1, 1, -1}) == 3); // i=1,2,3

    return 0;
}

// The key observation is to rearrange the condition \( a_i - a_j = a_i + a_k \) to \( a_j + a_k = 0 \). Thus, for each position \( i \), we need to check whether the set of sums of pairs \( (j,k) \) where \( 1 \le j < i \) and \( 1 \le k \le i \) contains \( 0 \). However, the original code uses a different but equivalent approach: it checks if `possivel[a_i - a_j + ADD]` is true for any \( j < i \). That array stores whether the sum \( a_j + a_k \) has been seen for any \( j \le i \) and \( k \le i \). But careful: in the original snippet, after processing index `i`, it adds sums involving `i` with all `j` from 1 to `i`, including `a_i + a_i`. The check for index `i` uses only `j < i` (since the loop for j goes from 1 to i-1). This means when we check for index `i`, the `possivel` array already contains sums from pairs `(j,k)` with both indices < i, plus sums where one index is < i and the other is < i as well (because all previous iterations added sums involving each previous index with all earlier or equal indices). So it correctly represents sums of any two indices both strictly less than i. To match this, the task condition should be interpreted as: count indices i such that there exist \( 1 \le j < i \) and \( 1 \le k < i \) with \( a_j + a_k = 0 \). This is simpler: we can precompute all pairwise sums of the prefix up to i-1 in a hash set, and for each i, check if 0 is in that set. Since the range of values is limited to ±100000, sums range from -200000 to 200000, so a boolean array of size 400001 works as a set. Time complexity: for each i, we check at most i-1 differences? No, we don't need to check differences; we just check if 0 is marked. We need to add sums involving i with all previous indices and itself? For the prefix up to i, we need sums of any two elements both in [1, i-1]. So after processing i-1, we should have marked all sums \( a_p + a_q \) for 1 ≤ p,q ≤ i-1. Then for index i, check if 0 is marked. Then update the set to include sums involving i with any previous index (including itself) to prepare for the next i. However, we must be careful: the original code for each i checks using j < i and k ≤ i? Actually it checks `possivel[vetor[i] - vetor[j] + ADD]` where `possivel[s]` represents that sum s is possible as a sum of two earlier elements? Let's re-derive: condition `vetor[i] - vetor[j] == vetor[i] + vetor[k]` simplifies to `vetor[j] + vetor[k] == 0`. So for index i, we need any pair (j,k) with j < i and k ≤ i? But k appears in both sides; actually the original condition is `possivel[vetor[i] - vetor[j] + ADD]` where `possivel` was set by adding sums `vetor[i] + vetor[j]` for all j ≤ i in previous iterations. That means `possivel[x]` is true if there exist two indices p,q (both < current i) such that a_p + a_q = x. Because when we process index t (t < i), we add sums a_t + a_j for all j ≤ t. So after processing up to i-1, `possivel` contains sums of any two indices both ≤ i-1. Therefore for index i, checking `possivel[a_i - a_j + ADD]` for any j < i is equivalent to checking if there exists j < i such that a_i - a_j equals some sum of two earlier indices. But the original condition simplifies to a_i - a_j = 0? Wait, re-check: `possivel[vetor[i] - vetor[j] + ADD]` is true means there exist p,q < i with a_p + a_q = a_i - a_j. That does not directly equal the condition a_i - a_j = a_i + a_k unless we set a_k = -a_p - a_q? Actually the original problem might be different; let's not overcomplicate. The snippet given is a solution to a specific SPOJ problem, but we are to create a new independent task inspired by it. The task specification should be self-contained and not depend on the original problem. To avoid ambiguity, I will define the task clearly: given an array, count indices i (2 ≤ i ≤ n) such that there exist two indices j,k with 1 ≤ j < i and 1 ≤ k < i satisfying a_j + a_k = 0. That matches the simplification from the original condition after cancellation. This is a classic "two-sum prefix" problem. We can maintain a boolean array of possible sums from the prefix. For each i from 1 to n, before adding a_i, check if 0 is possible; if yes, increment answer. Then add all sums a_i + a_t for t from 1 to i (including itself) to the boolean array. Edge cases: duplicate numbers, negative numbers, sums out of range (but bounded by 200000). Complexity O(n^2) time in the worst case for adding sums, and O(200000) space. Since n ≤ 5000, n^2 = 25e6, which is acceptable in C++ within 1 second typically. But we can also do it more efficiently by noting that checking if 0 is possible is equivalent to whether there exist two equal opposite numbers in the prefix. Actually a_j + a_k = 0 means a_k = -a_j. So we can maintain a frequency map of values seen so far, and for each i, check if there exists some value v in the prefix such that -v is also in the prefix. For each i, we need to know if among numbers a_1..a_{i-1}, there exist two (possibly same index) whose sum is 0. That is true iff either there is a number equal to its own negation (i.e., 0 appears) or there exists a positive number and its negative both appear. So we can keep a frequency array for values in [-100000,100000]. Then for each i, check if 0 appears at least once (then any pair (0,0) works? Actually j and k can be same index? The condition says j and k are indices with j < i and k < i, but they don't have to be distinct. So if there is at least one zero in prefix, then 0+0=0 works (using j = position of that zero, k = same position). Also if there is any value v > 0 and also -v present, then pair works. So we just maintain a set or frequency count. For each i, before adding a_i, we check if the set of previous values contains both v and -v for some v. That can be done by checking if 0 is present, or if there is any positive v whose negative is also present. We can maintain a boolean exists[value+OFFSET] and also a count of how many positive v have both v and -v present. But simpler: just maintain a frequency array and for each new value, update it, and we can check on the fly. But the problem asks for a count of indices i that satisfy the condition. The condition is independent of a_i? Actually after cancellation, the condition for index i does not involve a_i at all; it only depends on the prefix a_1..a_{i-1}. So we can compute the answer as: for i from 2 to n, if the prefix a_1..a_{i-1} contains a pair (not necessarily distinct) summing to 0, then increment. That is straightforward. The original snippet uses a different method because the original problem might have a different condition, but our task is to create a new independent one. To stay closely inspired, I will formulate the task as: "Given an array of integers, count how many positions i (1-indexed, i > 1) satisfy that there exist indices j and k with j < i and k < i such that a_j + a_k = 0." Provide a solution using a boolean array for sums as in the snippet, but we need to adapt. Actually the snippet checks `possivel[vetor[i] - vetor[j] + ADD]` and then adds sums `vetor[i] + vetor[j]`. That is not directly the condition we derived. To be safe, I will design a task that matches the algorithmic pattern: maintain a boolean array of achievable sums of pairs from the prefix, and for each new element, check if some difference (or sum) is achievable. I'll define a concrete problem: Given an array of integers, count the number of indices i such that there exist j < i and k ≤ i with a_i - a_j = a_i + a_k. This simplifies to a_j + a_k = 0, but note k can be i. So for each i, we need to check if there exists j < i and k ≤ i such that a_j + a_k = 0. That is equivalent to: either there exists j < i with a_j = -a_i (since k=i gives a_j + a_i = 0), or there exist j,k < i with a_j + a_k = 0. So the condition includes pairs from the prefix and also potential pairs involving a_i itself with an earlier element. This is more interesting and matches the snippet better because the snippet's check uses `possivel[vetor[i] - vetor[j] + ADD]` which after simplification becomes `vetor[j] + something = vetor[i]`? Let's not force it. I will define a clean, standalone task:
//
// **Task**: Write a function `int countPairs(const vector<int>& A)` that returns the number of indices i (1 ≤ i ≤ n) such that there exist indices j and k with 1 ≤ j ≤ i and 1 ≤ k ≤ i and j != k? Actually we need to be precise. To avoid ambiguity, I will specify: Count the number of indices i (from 1 to n) for which there exists at least one pair (j,k) with 1 ≤ j < i and 1 ≤ k ≤ i such that A[j] + A[k] = 0. This includes pairs where k can be i, allowing A[i] to pair with an earlier element. This is a natural problem. Solution: maintain a frequency set of values seen so far (up to i-1). For each i, first check if there exists any j < i such that A[j] = -A[i] (this covers k=i). Also check if there exists any j,k < i (possibly same) with A[j] + A[k] = 0; that is equivalent to whether 0 appears in the prefix or some value and its negative both appear. We can maintain a boolean array `seen` for values in range [-100000,100000] and also a boolean `sumZeroPossible` that indicates whether the current prefix (excluding i) already has a pair summing to zero. For each i, we compute `pairBefore` as whether sumZeroPossible is true. Then we compute `pairWithI` as whether seen[-A[i]] is true. If either is true, we increment answer. Then we add A[i] to seen and update sumZeroPossible: if A[i] == 0, sumZeroPossible becomes true; if seen contains -A[i] already (from before adding), then sumZeroPossible becomes true as well (since pair (i, that earlier position) sums to zero). Actually we need to be careful: when we add A[i], any new pairs formed with A[i] and an earlier element sum to zero if A[i] = -A[j]. So if seen[-A[i]] is true before adding, then after adding, sumZeroPossible becomes true. Also if A[i] == 0, then A[i] + A[i] = 0, so sumZeroPossible becomes true as well (even if no earlier zero). So we can update accordingly. This yields an O(n) solution with O(200000) space. This is elegant. So the task is: Given an array of integers (|A[i]| ≤ 100000, n ≤ 5000), count the number of indices i such that there exists a pair (j,k) with 1 ≤ j < i ≤ n and k = i or k < i satisfying A[j] + A[k] = 0. Actually to simplify, we can just state: count indices i for which there exists an earlier position j < i such that A[j] = -A[i], OR there exists a pair of earlier positions (j,k) with both < i summing to zero. This is a well-defined problem. I'll produce a solution using a boolean array for seen values and a boolean flag for "prefix has zero-sum pair". Complexity O(n) time and O(range) space.
//
// I'll write the solution function accordingly.
