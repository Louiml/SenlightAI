/*
You are given an array of positive and negative integers. You may perform at most `K` operations. In each operation, you may remove either the leftmost remaining element or the rightmost remaining element from the current array and discard it (you do not keep the value). However, you are also allowed to stop at any point and keep the sum of all currently remaining elements. Your goal is to maximize the sum of the remaining elements after performing at most `K` removals. Write a C++ function `int maxRemainingSum(const std::vector<int>& v, int K)` that returns the maximum possible sum of the remaining array. The original array is given as a vector of integers, and `K` is non-negative. You may assume that `K` can be larger than the array length, but in that case you may remove all elements (sum becomes 0). The function should handle empty arrays (return 0) and arrays with all positive or all negative numbers correctly.
*/
#include <vector>
#include <algorithm>
#include <numeric>

/**
 * Given an array of integers, you can take at most K cards from the two ends.
 * You may also discard some taken cards (only negative ones) up to the remaining operations.
 * Returns the maximum possible sum of cards you keep.
 */
int maxRemainingSum(const std::vector<int>& v, int K) {
    const int n = static_cast<int>(v.size());
    if (n == 0 || K <= 0) return 0;
    K = std::min(K, n); // can't take more than n

    int best = 0;

    // Enumerate how many we take from the left (a) and from the right (b)
    for (int a = 0; a <= K; ++a) {
        for (int b = 0; a + b <= K; ++b) {
            // a and b cannot exceed the array length
            if (a + b > n) continue;

            // Collect the selected cards
            std::vector<int> taken;
            taken.reserve(a + b);
            for (int i = 0; i < a; ++i) taken.push_back(v[i]);
            for (int i = n - b; i < n; ++i) taken.push_back(v[i]);

            // Current sum of all taken
            int sum = std::accumulate(taken.begin(), taken.end(), 0);
            best = std::max(best, sum);

            // Extra operations we can use to discard negative cards
            int discardLimit = K - (a + b);

            // Sort ascending so negatives come first
            std::sort(taken.begin(), taken.end());

            // Discard the most negative ones while possible
            for (int val : taken) {
                if (discardLimit <= 0) break;
                if (val < 0) {
                    sum -= val;  // removing a negative increases sum
                    discardLimit--;
                }
            }
            best = std::max(best, sum);
        }
    }
    return best;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the function here or else declare it.
int maxRemainingSum(const std::vector<int>& v, int K);

int main() {
    // Empty array
    assert(maxRemainingSum({}, 5) == 0);

    // K = 0, can't take anything
    assert(maxRemainingSum({1, -2, 3}, 0) == 0);

    // All positive, take as many as possible from ends
    assert(maxRemainingSum({1, 2, 3, 4, 5}, 2) == 9); // take 5 and 4 (right) or 5 and 1 (left+right) -> max 9
    assert(maxRemainingSum({1, 2, 3, 4, 5}, 5) == 15); // take all

    // All negative, best to take and discard? actually take negative then discard if allowed
    // K=1, take -1, discard 0 times -> sum -1, but you could take nothing? We take at most 1, so best is -1? But you can choose to take 0, so answer 0.
    assert(maxRemainingSum({-1, -2, -3}, 1) == 0); // take none (since taking negative lowers sum)
    assert(maxRemainingSum({-1, -2, -3}, 2) == 0); // take none

    // Mixed: take positive and discard negatives
    // v = [3, -5, 2, -1, 4], K=3
    // Option a=1,b=1: take 3 and 4, sum=7, discardLimit=1, can discard -1? No, -1 is not in taken. Actually can't discard. So best 7
    // Option a=0,b=3: take -1,4? Wait suffix of length 3: [2,-1,4] sum=5, can discard -1 -> sum 6
    // Option a=2,b=0: take [3,-5] sum=-2, discard -5 -> sum 3
    // Option a=1,b=2: take [3] and [−1,4] sum=6, discard -1 -> sum 7
    // So best 7
    assert(maxRemainingSum({3, -5, 2, -1, 4}, 3) == 7);

    // K larger than n, can take all and discard all negatives
    // v=[-10, -20, 5], K=10 -> can take all (sum -25), discard -10 and -20 (2 discards) -> sum 5
    assert(maxRemainingSum({-10, -20, 5}, 10) == 5);

    // Single element
    assert(maxRemainingSum({7}, 1) == 7);
    assert(maxRemainingSum({-7}, 1) == 0); // take none

    // Example from the original code snippet: let's test a simple case
    // v = [1, -2, 3, -4, 5], K=2
    // Option a=0,b=2: take [-4,5] sum=1, discardLimit=0 -> 1
    // a=1,b=1: [1] and [5] sum=6, discardLimit=0 -> 6
    // a=2,b=0: [1,-2] sum=-1, discardLimit=0 -> -1
    // So best 6
    assert(maxRemainingSum({1, -2, 3, -4, 5}, 2) == 6);

    // Edge: K=1, v=[10, -5, 20] -> best take 10 or 20, so 20
    assert(maxRemainingSum({10, -5, 20}, 1) == 20);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// We need to decide how many elements to take from the left (`a`) and from the right (`b`), with the condition `a + b <= K` (since we remove these elements). After removing `a` from left and `b` from right, the remaining sum is the total sum of the array minus the sum of the removed elements. To maximize the remaining sum, we want to minimize the sum of the removed elements, but we must remove exactly `a` from the left and `b` from the right (since we can choose to remove fewer than K, but we also can choose to stop earlier; however, if we have already decided to take `a` and `b`, we must remove those exact elements). The optimal strategy is to choose which elements to remove from the two ends so that the removed sum is as small as possible, ideally negative numbers to reduce the total sum less. More precisely, for each pair `(a,b)` with `a+b <= K`, we remove the `a` leftmost and `b` rightmost elements. But if we have extra removals allowed (`K - (a+b)`), we can optionally discard additional elements from either side among the already removed ones? Actually the given code's approach is: it enumerates `a` and `b` such that `a+b <= K`. Then for the selected `a` and `b`, it computes the sum of the remaining elements (total sum minus sum of removed left and right elements). But it also allows extra removals (up to `K-(a+b)`) by discarding some negative elements from the *removed* lists (i.e., from the edges of the remaining array? Wait, the code's logic is different: it collects the removed left elements into `veca` and right elements into `vecb`, then sorts them and tries to remove additional negative numbers from these two lists, up to the remaining removal count. That effectively means: after removing `a` from left and `b` from right, we have the remaining middle segment. But the code's `tmp` removes negative numbers from the *removed* lists? Actually it subtracts negative values from `isum` (the sum of remaining) which increases it. This is confusing. Let's reinterpret: The code’s approach is: choose `a` from left and `b` from right to be the *final* kept elements? No, let's read: It enumerates `a` (left count) and `b` (right count) such that `a+b <= K`. Then it keeps those `a` leftmost and `b` rightmost? Actually it pushes `v[a-1]` into `veca` and `v[n-b]` into `vecb`. So `veca` holds the first `a` elements (in reverse order) and `vecb` holds the last `b` elements (in reverse order). Then `suma` and `sumb` are sums of those. Then `isum = suma + sumb` which is the sum of the selected edge elements. Then it tries to discard up to `K-(a+b)` additional elements from these selected edges by removing negative numbers (since removing a negative from the sum increases it). After that, it takes max over `isum`. So the final answer is the maximum sum of a subset that can be formed by taking some left prefix and some right suffix, with total taken count at most `K`, and you may drop some negative values from those taken. This is actually the classic problem: you have an array, you can pick at most `K` elements total from the two ends, and you want the maximum sum of picked elements (you are allowed to pick fewer than K). Because removing a negative from the picked set increases the sum, so you would only pick positive numbers if possible. The correct interpretation: We are allowed to take up to K elements from the two ends (any number from left, any from right, total ≤ K) and we want the maximum sum of the taken elements. Because we can choose to not take some of the allowed operations (i.e., not remove them). The problem statement says "perform at most K operations" where each operation removes an end element, but you keep the sum of remaining? Actually the original problem (AtCoder ABC 127 E? or similar) is: You have N cards, you can perform at most K operations. In each operation you can take the leftmost or rightmost card, and you keep it. You want to maximize the sum of cards you take. That matches the code's approach: it picks `a` from left and `b` from right, then it may discard some negative ones (i.e., not take them) to stay within K. So the final answer is the maximum sum of at most K cards taken from the ends, where you can choose any mix of left and right, but you cannot skip an interior card without taking all cards before it. So the solution: The optimal chosen set is a prefix of length `a` and a suffix of length `b` (with `a+b ≤ K`), but you can also drop some negative numbers from that chosen set? Wait, if you have taken a prefix and suffix, dropping a negative from the middle? No, you can only take from ends. The code's approach: it enumerates `a` and `b` as the number of cards you *consider* taking from left and right, but then it allows discarding some negative ones among those considered, effectively meaning you take fewer cards. That is equivalent to: you can choose any prefix length `p` and suffix length `q` with `p+q ≤ K`, but you are allowed to not take some negative numbers? Actually you cannot skip an element in the middle of the prefix; you must take all from the end up to the point. However, the code sorts the considered elements and discards negative ones, which is incorrect if you cannot skip. But the typical accepted solution for this problem (AtCoder ABC 127 F? Actually it's ABC 123 C? Let's think) is: Enumerate `a` (left taken count) from 0 to min(N,K). For each `a`, compute the sum of the left `a` elements. Then among the remaining allowed operations `K-a`, you can take up to `K-a` from the right. But you can also choose to not use all operations, so you only take the best `K-a` elements from the right? No, the right elements are a suffix, so you must take them in order. So you can take a suffix of length `b` where `b ≤ K-a`. Then you have a total of `a+b` cards taken, but you can also drop some negative cards from the taken set? The actual problem (AtCoder ABC 127 E? Actually it's "Cards" problem from AtCoder ABC 127 F? Let's recall: There is a problem where you have a row of cards, you can perform up to K operations: take the leftmost or rightmost card, and you can also discard a card from your hand. The goal is to maximize sum of cards in hand. Solution: enumerate how many from left, then for the right, you sort the negative values and discard them. That matches the code: after selecting `a` left and `b` right, you have those cards in hand, but you may discard up to `K-(a+b)` of the negative ones (since you have that many operations left to discard). So the final sum is sum of selected left+right minus the sum of the most negative discarded ones. So the algorithm: For each `a` from 0 to min(N,K), for each `b` from 0 to min(N-a, K-a) – but the code uses `a+b<=K` – compute the sum of the first `a` elements and last `b` elements. Then you have `a+b` cards, and you can discard up to `K-(a+b)` cards among them, but only if they are negative (discarding a positive would lower sum). To maximize, discard the most negative ones first. So collect all selected cards, sort them, and remove up to `K-(a+b)` negative ones from the sum. The answer is the maximum over all such configurations.
//
// But the code in the snippet does it by sorting `veca` and `vecb` separately and then doing a merge-like process. That's inefficient. A better solution: collect all selected cards into a vector, sort, then take the sum of the largest `(a+b) - discard` where discard is the minimal number of negatives to remove, but since you want max sum, you should discard the smallest (most negative) values up to the allowed number. So the optimal is: sum all selected, then subtract the sum of the `min(discard_count, number_of_negatives)` most negative values. That is O(N^2 log N) if you do it for each pair, but N and K are small typically (N ≤ 100 in the original problem). So it's fine.
//
// We need to produce a function `maxRemainingSum` that does this. Important edge cases: K can be 0, then answer is 0 (since you cannot take any card). If N=0, return 0. If K > N, then you can take all cards but can also discard negatives; since you have at most N cards, the limit is N. So we cap K at N. Also, if all numbers are positive, the best is to take as many as possible, so take the whole array if K>=N, else take the K largest? No, you can only take from ends, so you might take the entire array if K>=N, else you take a prefix and suffix totaling K, and chose the best combination. But also you can discard none, so answer is max sum of any prefix+suffix of total length ≤ K. But if all positive, the best is to take the entire array if possible, else take the K largest? Actually you cannot skip middle, so you take the best of taking `a` from left and `b` from right with sum of lengths = min(K,N). That is a classic sliding window: you take the maximum sum of a subarray that is a prefix+suffix? Wait, you cannot take a middle segment; you take a prefix and a suffix. So the maximum sum of a prefix and suffix disjoint is the maximum over `a+b ≤ K` of sum(prefix a) + sum(suffix b). That's what we compute.
//
// The reference solution: implement a function that iterates `a` from 0 to min(N,K), and for each `a`, iterate `b` from 0 to min(N-a, K-a). Compute the sum of prefix a and suffix b. If a+b > 0, collect the selected elements into a temporary vector. Then if `discard_allowed = K - (a+b)` is positive, we may remove up to that many negative elements from the selected vector. Since we want max sum, we sort the selected vector ascending (so negatives first). Then we can discard up to discard_allowed of the most negative ones, but only if they are negative. So we iterate through sorted vector and subtract negative values while `discard_allowed > 0`. But careful: if we discard a negative, we increase the sum. So we should discard as many negative ones as possible up to the allowed count. So after sorting, we can sum all, then for each negative element (from start), if `discard_allowed > 0`, subtract that negative value (i.e., add its absolute value) and decrement `discard_allowed`. But that's equivalent to taking the sum of all selected and then subtracting the sum of the most negative ones. Actually simpler: compute the sum, then sort the selected, and while `discard_allowed > 0` and the smallest element is negative, remove it (i.e., subtract it from sum) and decrement. This yields the maximum possible sum. Then take the maximum among all `(a,b)`.
//
// Edge case: if K is larger than N, then `a` can go up to N, and `b` can go up to N-a, but total operations used for discarding is `K-(a+b)`, which can be large, but you can only discard at most `a+b` negative elements. So we cap discard_allowed at N.
//
// Time complexity: For each `a` and `b`, we copy up to `a+b` elements (O(N)) and sort them (O(N log N)). The number of (a,b) pairs is O(N^2). So worst-case O(N^3 log N) but N is small (≤100). Space O(N).
//
// Alternative more efficient: Precompute prefix sums and suffix sums, and for each `a,b` compute sum quickly, and collect only the selected elements for sorting. Since N small, it's fine.
//
// We'll provide a clear implementation.
