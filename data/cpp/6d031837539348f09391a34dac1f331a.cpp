// Write a function `int shortestFlips(const std::vector<int>& arr)` that returns 1 if there exists any index i (0 ≤ i < arr.size()-1) such that arr[i] != arr[i+1]; otherwise, returns arr.size(). Assume arr is non-empty. This function models the scenario where you need to flip exactly one element to make all adjacent pairs equal, but if the array is already uniform, you must flip the entire array? No, just follow the logic.
//
// I'll define it as: "You are given a row of colored tiles. You are allowed to repaint a single tile to any color. You want to achieve a state where every pair of adjacent tiles has the same color? Actually if you have [1,2,1], adjacent pairs are (1,2) different and (2,1) different. Changing one tile can't fix both if the array is length 3? Change middle to 1 gives [1,1,1] all equal, so yes. Change middle to 2 gives [1,2,2] still one adjacent different. So it's possible. For [1,2,3], changing any one can't make all adjacent equal because you need at most two distinct values and one appears once. So impossible. But the snippet returns 1 for [1,2,3] because there is an adjacent different pair. So the problem isn't about making all adjacent equal; it's simply checking if there is any adjacent different. Let's just make the task be exactly that.
//
// I'll write:

// The function simply iterates through the vector from index 0 to n-2 and checks if a[i] != a[i+1]. If found, return 1 immediately. If the loop completes without finding any unequal adjacent pair, all adjacent pairs are equal, which implies all elements are equal (by transitivity), so return n. The time complexity is O(n) in the worst case (when all elements are equal), and O(1) average-case if a difference is found early. Space complexity is O(1). Edge cases: n=1 returns 1; large n with all equal returns n; n with first two different returns 1.

#include <vector>

// Returns 1 if there exists an adjacent pair with differing values,
// otherwise returns the size of the vector.
int checkDiversity(const std::vector<int>& a) {
    for (size_t i = 0; i + 1 < a.size(); ++i) {
        if (a[i] != a[i + 1]) {
            return 1;
        }
    }
    return static_cast<int>(a.size());
}

#include <cassert>
#include <vector>
int main() {
    assert(checkDiversity({1, 2, 1}) == 1);
    assert(checkDiversity({5, 5, 5}) == 3);
    assert(checkDiversity({7}) == 1);
    assert(checkDiversity({-1, -1, -1, -1}) == 4);
    assert(checkDiversity({0, 0, 1}) == 1);
    assert(checkDiversity({3, 2}) == 1);
    assert(checkDiversity({10}) == 1);
    assert(checkDiversity({1, 1, 2, 2}) == 1);
    assert(checkDiversity({4, 4, 4, 4, 4}) == 5);
    assert(checkDiversity({1, 2, 3, 4}) == 1);
}
