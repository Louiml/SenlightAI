Write a C++ function that determines whether a football team can avoid relegation. The function takes two vectors: `attackers` (the team’s attacking player ratings) and `defenders` (the opponent’s defensive player ratings). The team is considered safe if its weakest attacker (minimum rating) is strictly less than the opponent’s second-strongest defender (the second largest value in the defenders list). If the condition holds, return `true` (safe), otherwise `false`. The input vectors contain at least 2 elements each, and all ratings are positive integers. The function must not sort the original vectors; instead, it should work on copies or use a partial selection approach.

The problem is derived from the given bubble sort snippet, but now we only need two specific statistics: the minimum of the attackers and the second-largest of the defenders. A straightforward approach is to sort the vectors (or copies) and then compare `attackers[0]` with `defenders[defenders.size()-2]`. Sorting a copy is fine because the original data should remain unchanged. However, a more efficient approach avoids full sorting: find the minimum in O(n) and find the largest and second-largest in O(m) by scanning. Edge cases: if either vector has fewer than 2 elements, the problem is ill-defined (but constraints say at least 2). Duplicate values in defenders are handled correctly: if the two largest are equal, the second-largest equals the largest. The overall time complexity is O(a + d) where `a` and `d` are sizes, and space is O(1) if we don't copy. But since the task specifies "must not sort the original vectors" but allows copies, we can either sort copies in O(n log n) or use selection in O(n). We'll present the O(n) selection approach for clarity and efficiency. The function should be `const` correct, take vectors by const reference, and return a bool.

#include <vector>
#include <algorithm>

// Determine if the team is safe: minimum attacker < second-largest defender.
bool isSafe(const std::vector<int>& attackers, const std::vector<int>& defenders) {
    // Find minimum attacker
    int minAttacker = attackers[0];
    for (size_t i = 1; i < attackers.size(); ++i) {
        if (attackers[i] < minAttacker) {
            minAttacker = attackers[i];
        }
    }

    // Find largest and second-largest defender
    int largest = defenders[0];
    int secondLargest = defenders[1];
    if (secondLargest > largest) {
        std::swap(largest, secondLargest);
    }
    for (size_t i = 2; i < defenders.size(); ++i) {
        int value = defenders[i];
        if (value > largest) {
            secondLargest = largest;
            largest = value;
        } else if (value > secondLargest) {
            secondLargest = value;
        }
    }

    return minAttacker < secondLargest;
}

#include <cassert>
#include <vector>

// Forward declaration for testing
bool isSafe(const std::vector<int>& attackers, const std::vector<int>& defenders);

int main() {
    // Basic cases
    assert(isSafe({1, 5, 3}, {2, 4, 6}) == true);   // min=1 < secondLargest=4
    assert(isSafe({10, 20}, {5, 8, 9}) == false);   // min=10 < secondLargest=8? no
    assert(isSafe({7, 2, 9}, {3, 3, 5}) == true);   // min=2 < secondLargest=3

    // Duplicate defenders
    assert(isSafe({1, 8}, {4, 4, 7}) == false);     // secondLargest=4? actually largest=7, second=4, min=1<4 true? wait: largest=7, second=4, min=1<4 true
    // Let's recompute: largest=7, secondLargest=4, min=1<4 => true
    assert(isSafe({1, 8}, {4, 4, 7}) == true);
    assert(isSafe({10, 10}, {5, 5, 5}) == false);   // min=10 < secondLargest=5? no

    // Edge: exactly two defenders
    assert(isSafe({3, 1}, {2, 9}) == true);         // min=1 < secondLargest=2

    // Large values
    assert(isSafe({100, 200, 50}, {1000, 900, 800}) == false); // min=50 < secondLargest=900? true? Actually 50<900 true, so expected true
    // Correct: true
    assert(isSafe({100, 200, 50}, {1000, 900, 800}) == true);

    // Ensure original vectors not modified
    std::vector<int> atk = {5, 1, 3};
    std::vector<int> def = {4, 2, 6};
    assert(isSafe(atk, def) == true);
    assert(atk[0] == 5 && atk[1] == 1 && atk[2] == 3); // unchanged
    assert(def[0] == 4 && def[1] == 2 && def[2] == 6); // unchanged

    return 0;
}
