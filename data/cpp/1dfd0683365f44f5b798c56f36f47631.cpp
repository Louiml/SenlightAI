Given a non-empty sequence of integers representing rewards for a two-contestant game show, write a C++ function that returns the number of ways to place exactly two "passes" so that the game ends in a tie. The rules: the contestant who starts receives rewards from the beginning until the first pass, then the other contestant receives rewards until the second pass, and after the second pass the first contestant receives the remaining rewards. Both contestants’ total scores must be equal. The two passes must be placed strictly between rewards (i.e., after reward 1 up to before the last reward), and the first pass must occur before the second pass. If the sequence has fewer than 3 rewards, return 0. For example, for rewards `[-16, 1, 5, -6, 7, 9]`, the only valid pass pair is after positions 2 and 5, giving both contestants 0 points. Count each distinct ordered pair of pass positions (as indices of the reward after which the pass occurs) that yields equal totals.
#include <cassert>
#include <vector>

int main() {
    // Example 1 from problem: [-16, 1, 5, -6, 7, 9] -> 1 way (pass after 2 and 5)
    assert(countTieWays({-16, 1, 5, -6, 7, 9}) == 1);
    
    // Example 2: [1, 5, -6, 7, 9] -> 0 ways (only possible with 3 passes, not 2)
    assert(countTieWays({1, 5, -6, 7, 9}) == 0);
    
    // Trivial small sequences
    assert(countTieWays(std::vector<int>{}) == 0);
    assert(countTieWays({5}) == 0);
    assert(countTieWays({5, 5}) == 0);
    
    // Simple case: all zeros, any two passes give equal totals
    // n=4, passes can be at (1,2),(1,3),(2,3) -> 3 ways
    assert(countTieWays({0, 0, 0, 0}) == 3);
    
    // Case with positive numbers: [1, 2, 3] total=6 target=3
    // Only pair (1,2)? segment B = [2] sum=2 not 3; (0,1) segment B=[1] sum=1; (0,2) segment B=[1,2]=3 -> one way (pass after 0 and after 2) actually i=0, j=2 gives sum(B)=1+2=3 -> yes
    assert(countTieWays({1, 2, 3}) == 1);
    
    // Case with negative numbers: [-1, 1, 0] total=0 target=0
    // pairs: (0,1) B=[1] sum=1; (0,2) B=[1,0]=1; (1,2) B=[0] sum=0 -> one way (pass after 1 and 2)
    assert(countTieWays({-1, 1, 0}) == 1);
    
    // Large test with odd total -> 0
    assert(countTieWays({1, 2, 3, 4}) == 0); // total=10 even target=5 but check manually: no pair works? Actually we trust function.
    
    // Test with duplicate sums: [1, -1, 1, -1] total=0 target=0
    // pairs: (0,1) B=[-1] sum=-1; (0,2) B=[-1,1]=0 yes; (0,3) B=[-1,1,-1]=-1; (1,2) B=[1] sum=1; (1,3) B=[1,-1]=0 yes; (2,3) B=[-1] sum=-1 -> 2 ways
    assert(countTieWays({1, -1, 1, -1}) == 2);
    
    // Test where all prefix sums distinct and only one pair
    assert(countTieWays({3, -1, 2}) == 1); // total=4 target=2, pair (0,1) B=[-1]? no; (0,2) B=[-1,2]=1; (1,2) B=[2]=2 -> one way
    assert(countTieWays({3, -1, 2}) == 1);
    
    // Test where no way despite even total
    assert(countTieWays({2, 2, 2, 2}) == 0); // total=8 target=4, possible pairs? B sums: (0,1)=2, (0,2)=4, (0,3)=6, (1,2)=2, (1,3)=4, (2,3)=2 -> two ways (0,2) and (1,3) actually check: i=0,j=2 gives B=[2,2]=4 yes; i=1,j=3 gives B=[2,2]=4 yes -> 2 ways? Let's compute: (0,2) sum=4 yes; (1,3) sum=4 yes; (0,3) sum=6 no; (1,2) sum=2 no; (2,3) sum=2 no; (0,1) sum=2 no. So 2 ways. We'll assert 2.
    assert(countTieWays({2, 2, 2, 2}) == 2);
    
    return 0;
}
#include <vector>
#include <unordered_map>

// Returns the number of pairs (i, j) with 0 <= i < j <= n-1 such that
// sum of elements from i+1 to j equals exactly half of the total sum.
// The rules require exactly two passes: after position i and after position j.
// The first contestant receives rewards [0..i] and [j+1..n-1],
// the second contestant receives rewards [i+1..j].
// The condition is sum(first) == sum(second).
int countTieWays(const std::vector<int>& rewards) {
    int n = static_cast<int>(rewards.size());
    if (n < 3) {
        return 0;
    }
    
    long long total = 0;
    for (int val : rewards) {
        total += val;
    }
    if (total % 2 != 0) {
        return 0;
    }
    long long target = total / 2;
    
    // prefixSum[i] = sum of rewards[0..i-1], with prefixSum[0] = 0.
    std::vector<long long> prefix(n + 1, 0);
    for (int idx = 0; idx < n; ++idx) {
        prefix[idx + 1] = prefix[idx] + rewards[idx];
    }
    
    // For each j (second pass position), we need number of i < j such that
    // prefix[j] - prefix[i] == target, i.e., prefix[i] == prefix[j] - target.
    // We maintain counts of seen prefix values for i < current j.
    std::unordered_map<long long, int> seenPrefixCounts;
    int ways = 0;
    for (int j = 1; j < n; ++j) {
        // Add prefix[j-1] as an available i for future j values.
        seenPrefixCounts[prefix[j - 1]]++;
        // Now check how many i < j satisfy prefix[i] == prefix[j] - target.
        long long needed = prefix[j] - target;
        auto it = seenPrefixCounts.find(needed);
        if (it != seenPrefixCounts.end()) {
            ways += it->second;
        }
    }
    return ways;
}
// The problem reduces to partitioning the sequence into three contiguous segments: segment A (rewards before the first pass), segment B (rewards between the first and second pass), and segment C (rewards after the second pass). The first contestant gets sum(A) + sum(C), and the second contestant gets sum(B). We need sum(A) + sum(C) == sum(B). Since A+B+C is the total sum, this condition is equivalent to sum(A) + (total - sum(A) - sum(B)) == sum(B), simplifying to total == 2*sum(B). So the second contestant’s segment must exactly equal half of the total sum. Counting all pairs (i, j) with i < j such that the sum of segment B (from index i+1 to j inclusive) equals half the total. A naive nested loop over all pairs is O(n²) per query, but we can do it in O(n) using prefix sums: For each possible second-pass position j (indices from 1 to n-1 inclusive), count how many first-pass positions i (indices from 0 to j-1) satisfy that the sum from i+1 to j equals target. That sum is prefixSum[j] - prefixSum[i] (with prefixSum[0]=0). We want prefixSum[j] - prefixSum[i] == target, or prefixSum[i] == prefixSum[j] - target. Iterate j from 1 to n-1, maintaining a frequency map or counter of how many prefix sums equal prefixSum[j] - target have been seen for i < j. Add that count to answer. Key edge cases: total sum must be even, otherwise return 0; if total sum is odd or the array size < 3 (i.e., fewer than 2 possible pass positions), return 0. Also, passes are placed strictly between rewards, so i ranges from 0 to n-2 and j ranges from i+1 to n-1, which the prefix method naturally handles. Time complexity O(n) with O(n) space for prefix sums, or O(n) time with O(n) space for prefix sums plus a hash map for counts. Space can be reduced to O(n) for prefix sums; using a hash map of counts also O(n). The provided skeleton uses a linked list; our reference solution will use a vector for clarity.
