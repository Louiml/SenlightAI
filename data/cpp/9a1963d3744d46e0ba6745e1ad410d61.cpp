Given an array of positive integers, write a C++ function `bool canBecomeEqual(const std::vector<long long>& a)` that returns `true` if, after repeatedly applying the operation (if the current number is odd, add its last digit to itself; repeat this addition as long as the number’s last digit is neither `4` nor `0`), all numbers in the array can be made equal by further applying the same operation (each number independently) any number of times. The operation is: replace `x` by `x + (x % 10)`. The transformation is applied to each element independently and can be repeated arbitrarily many times. The function should return `false` if it is impossible to make all elements equal under these rules. Note that once a number becomes even, its last digit cycles predictably: the last digit sequence for even numbers eventually reaches `4` or `0` and then `0` remains `0`, while `4` cycles `4 → 8 → 6 → 2 → 4` (mod 20 pattern). You may assume the input is a non‑empty vector of positive integers.
#include <cassert>
#include <vector>

// Declaration of the function under test.
bool canBecomeEqual(const std::vector<long long>& input);

int main() {
    // All already equal.
    assert(canBecomeEqual({5, 5, 5}) == true);
    // Odd numbers: 1 -> 2, then 2->4->8->6->2... cycles; both become 4? 1+1=2, 2+2=4, 4→8→6→2→4; eventually all end in 4. Check modulo 20: 1→2→4→8→6→2→4... eventually 4 (or 14). Both 1 and 3 become even: 1→2, 3→6; 2→4, 6→2→4; both end at 4? Actually 2→4, 6→2→4; both become 4 (mod 20 = 4). So true.
    assert(canBecomeEqual({1, 3}) == true);
    // 2 and 8: 2→4, 8→6→2→4; both end at 4 (mod 20 = 4). True.
    assert(canBecomeEqual({2, 8}) == true);
    // 2 and 12: 2→4, 12→4? 12%10=2, 12+2=14→4+? Actually 14%10=4, so 12→14→8→6→2→4; ends at 4 (mod 20 = 4). 2→4 (mod 20 = 4). True.
    assert(canBecomeEqual({2, 12}) == true);
    // 4 and 14: both already end in 4, mod 20: 4 and 14 -> not equal, impossible.
    assert(canBecomeEqual({4, 14}) == false);
    // Multiple of 10 fixed: 10 and 20 cannot become equal.
    assert(canBecomeEqual({10, 20}) == false);
    // 10 and 10 are equal.
    assert(canBecomeEqual({10, 10}) == true);
    // 5 and 15: 5→10, 15→20; both end in 0 but different values, impossible.
    assert(canBecomeEqual({5, 15}) == false);
    // 6 and 26: 6→2→4 (mod 20 = 4), 26→2→4 (mod 20 = 4), true.
    assert(canBecomeEqual({6, 26}) == true);
    // 8 and 18: 8→6→2→4 (mod 20 = 4), 18→6→2→4 (mod 20 = 4), true.
    assert(canBecomeEqual({8, 18}) == true);
    // 4 and 24: both end in 4, mod 20: 4 and 4 -> true.
    assert(canBecomeEqual({4, 24}) == true);
    // Edge: single element always true.
    assert(canBecomeEqual({12345}) == true);
    
    return 0;
}
#include <vector>
#include <cstdint>

// Returns true if all elements can be made equal via the operation x -> x + (x % 10).
bool canBecomeEqual(const std::vector<long long>& input) {
    std::vector<long long> a = input;  // work on a copy
    int n = static_cast<int>(a.size());
    
    // First, if any element is odd, add its last digit once to make it even.
    for (int i = 0; i < n; ++i) {
        if (a[i] % 2 == 1) {
            a[i] += a[i] % 10;
        }
    }
    
    // Now all elements are even. Drive each to have last digit 0 or 4.
    for (int i = 0; i < n; ++i) {
        while (a[i] % 10 != 4 && a[i] % 10 != 0) {
            a[i] += a[i] % 10;
        }
    }
    
    // Check if any element ends in 0.
    bool hasZero = false;
    for (int i = 0; i < n; ++i) {
        if (a[i] % 10 == 0) {
            hasZero = true;
            break;
        }
    }
    
    if (hasZero) {
        // If any ends in 0, all must be exactly equal (since 0 is a fixed point).
        for (int i = 1; i < n; ++i) {
            if (a[i] != a[0]) return false;
        }
        return true;
    }
    
    // No element ends in 0; all end in 4. They must have the same value modulo 20.
    int baseMod = static_cast<int>(a[0] % 20);
    for (int i = 1; i < n; ++i) {
        if (static_cast<int>(a[i] % 20) != baseMod) return false;
    }
    return true;
}
// The key insight is that after the first operation on an odd number, it becomes even (since odd + odd%10 = odd + odd = even). Once all numbers are even (or after we force odd numbers to become even by adding their last digit), the last‑digit behavior depends only on the number modulo 20. Specifically, starting from any even number, repeatedly adding its last digit will eventually reach a number ending in `0` (which stays `0` forever) or enter a cycle among numbers whose last digit is in {2,4,6,8} with a period of 20 in terms of the actual value modulo 20. For numbers ending in `0`, they are stuck (`x + 0 = x`), so if any number can reach a multiple of 10, all must be able to reach the same multiple of 10 to be equal. For numbers that do not end in `0`, after enough steps they all enter the same cycle modulo 20 (specifically, the residues 2,4,6,8,12,14,16,18). If any number ends in `0` after the initial transformation, then all numbers must eventually end in `0` and be the same multiple of 10; otherwise impossible. If no number ends in `0`, then by repeatedly applying the operation until the last digit is `4`, each number’s value modulo 20 becomes one of {4,8,12,16}? Actually careful: starting from an even number, the last digit evolves: e.g., 2→4→8→6→2→4... So if we stop when last digit is 4, the number modulo 20 is either 4 or 14 (since 14%10=4). Similarly, after forcing all numbers to have last digit 4, we reduce each number modulo 20 and check if all residues are equal. If they are, we can continue the cycle to make them exactly equal (since from a residue r, we can add a fixed sequence to reach any later residue in the cycle, and the cycle length is 20, so we can align them). If any number ends in 0 after the initial odd‑to‑even step, then we must check whether all numbers can become the same multiple of 10; if yes, they can all become that value (since adding 0 keeps them fixed), otherwise no. The algorithm: first, for each element, if it is odd, add its last digit once (making it even). Then for each element, while its last digit is not 0 and not 4, add its last digit repeatedly. After that, if any element has last digit 0, then we must check that all elements are equal (since 0 is a fixed point, and no other number can become a multiple of 10 unless it already is one, because numbers ending in 4 cycle and never produce a 0). If any element has last digit 0 and any other element does not, return false. If all are the same multiple of 10, return true. Otherwise, if no element ends in 0, then all end in 4; compute each element modulo 20; all such residues must be equal (either all 4 or all 14) for equality to be possible; if equal, return true, else false. Time complexity is O(n * k) where k is the maximum number of steps per element (bounded by a constant, at most 5 steps to reach 4 or 0), so effectively O(n). Space complexity O(1) extra.
