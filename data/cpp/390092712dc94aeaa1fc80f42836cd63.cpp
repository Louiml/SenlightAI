Given \(n\) pairs of positive integers \((a_i, b_i)\) with distinct indices from 1 to \(n\), write a C++ function that returns a vector of exactly \(\lfloor n/2 \rfloor + 1\) indices such that the chosen pairs satisfy both of the following conditions: (1) the sum of their \(a\)-values is at least half the total sum of all \(a\)-values (i.e., \(\sum_{\text{chosen}} a_i \ge \frac{1}{2}\sum_{i=1}^n a_i\)); (2) the sum of their \(b\)-values is at least half the total sum of all \(b\)-values. The function must work for any \(n \ge 1\), with at least one valid selection always existing. You may sort the pairs, but you must preserve original indices. Return the selected indices in any order.
The key observation is to sort all pairs in decreasing order of \(a\). After sorting, take the pair with the largest \(a\) unconditionally. Then, for the remaining even-sized groups of two consecutive pairs (starting from the second pair if \(n\) is even, or from the first pair after the initial one if \(n\) is odd), select the pair with the larger \(b\) from each adjacent group of two. This guarantees that at least half the total \(a\)-sum is chosen because the first pair alone already contributes at least as much as any other single pair, and you pick at least one from every two subsequent pairs, so you are taking at least half the remaining pairs. For the \(b\)-sum, by always choosing the larger \(b\) in each adjacent pair from the sorted order, the chosen \(b\)-sum is at least half of the total \(b\)-sum. Let \(m = \lfloor n/2 \rfloor + 1\) be the number chosen. Sorting takes \(O(n \log n)\) time, and the selection loop takes \(O(n)\), so total time is \(O(n \log n)\). Space is \(O(n)\) for the sorted array and the result vector. Edge cases: when \(n=1\), only that single element is chosen, trivially satisfying both conditions. When \(n=2\), the first element (largest \(a\)) and the second are both chosen. When \(n\) is odd, after taking the first, the rest are processed in pairs from index 1,3,5,...; when \(n\) is even, after taking the first, we must also take the second (since pairs must be disjoint), then process from index 3. The algorithm is a known competitive programming technique.
#include <vector>
#include <algorithm>

struct Pair {
    int a, b, id;
};

// Returns a vector of indices (1-based) of size n/2+1 satisfying both half-sum conditions.
std::vector<int> chooseHalfPairs(std::vector<int> a, std::vector<int> b) {
    int n = static_cast<int>(a.size());
    std::vector<Pair> pairs(n);
    for (int i = 0; i < n; ++i) {
        pairs[i] = {a[i], b[i], i + 1};
    }
    std::sort(pairs.begin(), pairs.end(), [](const Pair& x, const Pair& y) {
        return x.a > y.a;
    });

    std::vector<int> chosen;
    chosen.reserve(n / 2 + 1);
    chosen.push_back(pairs[0].id);

    int i = (n % 2 == 0) ? 1 : 2;
    if (n % 2 == 0) {
        chosen.push_back(pairs[1].id);
    }

    for (; i < n; i += 2) {
        if (pairs[i].b > pairs[i + 1].b) {
            chosen.push_back(pairs[i].id);
        } else {
            chosen.push_back(pairs[i + 1].id);
        }
    }
    return chosen;
}
#include <cassert>
#include <vector>
#include <algorithm>

// (Test harness includes the solution function above)
std::vector<int> chooseHalfPairs(std::vector<int> a, std::vector<int> b);

int main() {
    // Basic test
    {
        std::vector<int> a = {10, 5, 8, 2};
        std::vector<int> b = {1, 10, 3, 4};
        auto ans = chooseHalfPairs(a, b);
        assert(ans.size() == 3);
        long long sumA = 0, sumB = 0, totalA = 0, totalB = 0;
        for (int x : a) totalA += x;
        for (int x : b) totalB += x;
        for (int idx : ans) {
            sumA += a[idx - 1];
            sumB += b[idx - 1];
        }
        assert(sumA * 2 >= totalA);
        assert(sumB * 2 >= totalB);
    }

    // Single element
    {
        auto ans = chooseHalfPairs({7}, {3});
        assert(ans.size() == 1);
        assert(ans[0] == 1);
    }

    // Two elements
    {
        auto ans = chooseHalfPairs({1, 100}, {100, 1});
        assert(ans.size() == 2);
        assert((ans[0] == 2 && ans[1] == 1) || (ans[0] == 1 && ans[1] == 2));
    }

    // Odd length
    {
        std::vector<int> a = {5, 4, 3, 2, 1};
        std::vector<int> b = {1, 2, 3, 4, 5};
        auto ans = chooseHalfPairs(a, b);
        assert(ans.size() == 3);
        long long sumA = 0, sumB = 0, totalA = 0, totalB = 0;
        for (int x : a) totalA += x;
        for (int x : b) totalB += x;
        for (int idx : ans) {
            sumA += a[idx - 1];
            sumB += b[idx - 1];
        }
        assert(sumA * 2 >= totalA);
        assert(sumB * 2 >= totalB);
    }

    // Even length, all equal a
    {
        std::vector<int> a = {10, 10, 10, 10};
        std::vector<int> b = {1, 100, 50, 60};
        auto ans = chooseHalfPairs(a, b);
        assert(ans.size() == 3);
        long long sumB = 0, totalB = 0;
        for (int x : b) totalB += x;
        for (int idx : ans) sumB += b[idx - 1];
        assert(sumB * 2 >= totalB);
    }

    // Random test with many elements
    {
        std::vector<int> a = {12, 3, 45, 6, 78, 9, 11, 22, 33, 44};
        std::vector<int> b = {55, 66, 77, 88, 99, 11, 22, 33, 44, 55};
        auto ans = chooseHalfPairs(a, b);
        assert(ans.size() == a.size() / 2 + 1);
        long long sumA = 0, sumB = 0, totalA = 0, totalB = 0;
        for (int x : a) totalA += x;
        for (int x : b) totalB += x;
        for (int idx : ans) {
            sumA += a[idx - 1];
            sumB += b[idx - 1];
        }
        assert(sumA * 2 >= totalA);
        assert(sumB * 2 >= totalB);
    }

    return 0;
}
