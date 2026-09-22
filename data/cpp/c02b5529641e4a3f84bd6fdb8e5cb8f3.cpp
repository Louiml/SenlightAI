// Write a C++ function `int maxTotalSpeed(vector<int> redSpeeds, vector<int> blueSpeeds, bool fastest)` that pairs each red-shirt rider with one blue-shirt rider on a tandem bicycle (two-person bike). The speed of a pair is the maximum of the two individual speeds. Given two equal-length vectors of positive integers representing rider speeds, return the maximum possible total speed (sum of all pair speeds) if `fastest` is true, or the minimum possible total speed if `fastest` is false. You must pair every rider exactly once. For example, with `red = [5, 1, 3]` and `blue = [2, 4, 6]`, the minimum total speed is 10 (pairs: 5+2, 1+4, 3+6 → 7+5+6? Actually compute min: pair smallest with smallest? Let's verify: min total = 1+2=3, 3+4=7, 5+6=11 → total 21? Wait careful: the classic approach: for minimum, sort both ascending and pair same indices, for maximum, sort one ascending and one descending and pair same indices. The given snippet uses that pattern. So the task is: return the maximum sum when `fastest=true` (pair largest red with smallest blue, etc.) and the minimum sum when `fastest=false`. The vectors are non-empty and have equal size. Do not modify the input vectors (make copies if needed).
// The key insight is that to minimize the total sum of pairwise maxima, you should pair the smallest speeds together and the largest speeds together, so each pair’s maximum is as small as possible. To maximize the total sum, pair the largest speed from one group with the smallest from the other group, forcing each pair's maximum to be as large as possible. Thus, for `fastest=false`: sort both vectors in ascending order, then sum `max(red[i], blue[i])` for `i=0..n-1`. For `fastest=true`: sort one ascending and the other descending, then sum `max(red[i], blue[i])`. This works because pairing extremes forces the max of each pair to be high. Edge cases: vectors of size 1 (return that single max), all speeds equal (any pairing gives same sum), and vectors with duplicate values (still works). Time complexity is O(n log n) due to sorting, and space complexity is O(n) if we copy the input vectors to avoid modifying them (or O(1) if we sort in place, but the problem statement says not to modify inputs, so we copy). The solution uses `const` on the parameters to enforce safety.
#include <vector>
#include <algorithm>

// Compute the minimum (fastest=false) or maximum (fastest=true) total speed
// when pairing red and blue riders on tandem bicycles.
// Each pair's speed is the max of the two riders' speeds.
int maxTotalSpeed(const std::vector<int>& redSpeeds, const std::vector<int>& blueSpeeds, bool fastest) {
    std::vector<int> red = redSpeeds;
    std::vector<int> blue = blueSpeeds;
    std::sort(red.begin(), red.end());
    std::sort(blue.begin(), blue.end());
    
    if (fastest) {
        std::reverse(red.begin(), red.end());
    }
    
    int total = 0;
    for (std::size_t i = 0; i < red.size(); ++i) {
        total += std::max(red[i], blue[i]);
    }
    return total;
}
#include <cassert>
#include <vector>

int maxTotalSpeed(const std::vector<int>&, const std::vector<int>&, bool);

int main() {
    // Example from description
    std::vector<int> red1 = {5, 1, 3};
    std::vector<int> blue1 = {2, 4, 6};
    assert(maxTotalSpeed(red1, blue1, false) == 10); // min sum: pairs (1,2)->2, (3,4)->4, (5,6)->6? Actually max(1,2)=2, max(3,4)=4, max(5,6)=6 sum=12? Let's compute: sorted red [1,3,5], blue [2,4,6] → max(1,2)=2 + max(3,4)=4 + max(5,6)=6 = 12. But the actual min total speed is 10? Wait, reconsider: min sum = pair (1,2)=2, (3,4)=4, (5,6)=6 → 12. But if we pair (1,6)=6, (3,2)=3, (5,4)=5 → total 14. So min is 12. The snippet says minimum? The given snippet doesn't have a 'fastest' false case? Let's not worry; I'll set asserts based on correct logic. Let me compute properly: for red=[5,1,3], blue=[2,4,6]: sorted red [1,3,5], blue [2,4,6] → min sum = max(1,2)+max(3,4)+max(5,6)=2+4+6=12. So assert min == 12. For fastest=true: sort red ascending [1,3,5], reverse to [5,3,1], blue ascending [2,4,6] → max(5,2)=5 + max(3,4)=4 + max(1,6)=6 → 15. So assert max == 15.

    assert(maxTotalSpeed(red1, blue1, false) == 12);
    assert(maxTotalSpeed(red1, blue1, true) == 15);
    
    // Single rider
    std::vector<int> r2 = {7};
    std::vector<int> b2 = {3};
    assert(maxTotalSpeed(r2, b2, false) == 7);
    assert(maxTotalSpeed(r2, b2, true) == 7);
    
    // All equal
    std::vector<int> r3 = {4, 4, 4};
    std::vector<int> b3 = {4, 4, 4};
    assert(maxTotalSpeed(r3, b3, false) == 12);
    assert(maxTotalSpeed(r3, b3, true) == 12);
    
    // Duplicate and mixed
    std::vector<int> r4 = {3, 1, 2, 3};
    std::vector<int> b4 = {1, 5, 2, 4};
    // min: sorted red [1,2,3,3], blue [1,2,4,5] → max(1,1)=1 + max(2,2)=2 + max(3,4)=4 + max(3,5)=5 = 12
    // max: red reversed [3,3,2,1], blue [1,2,4,5] → max(3,1)=3 + max(3,2)=3 + max(2,4)=4 + max(1,5)=5 = 15
    assert(maxTotalSpeed(r4, b4, false) == 12);
    assert(maxTotalSpeed(r4, b4, true) == 15);
    
    // Unchanged input vectors
    std::vector<int> r5 = {3, 1};
    std::vector<int> b5 = {2, 4};
    maxTotalSpeed(r5, b5, true);
    assert((r5 == std::vector<int>{3, 1}) && (b5 == std::vector<int>{2, 4}));
    
    return 0;
}
