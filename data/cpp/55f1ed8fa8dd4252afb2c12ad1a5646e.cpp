Write a C++ function named `performCustomShuffle` that takes four integer reference parameters and randomly reorders their values so that after the function returns, the four variables contain a uniformly random permutation of the original four values (i.e., each of the 24 possible orderings is equally likely). The function must not return anything, must not use any standard library shuffling utilities (like `std::shuffle` or `std::random_shuffle`), and must not modify the values in any way other than reassigning them to a permutation. The solution must guarantee that the final assignment is correct even if any of the original values are equal, and it must avoid infinite loops. The function can use `rand()` for simplicity, but you must ensure all indices are valid and that no duplicate index is ever produced.

#include <cassert>
#include <cstdlib>
#include <algorithm>

int main() {
    // Seed for reproducibility in tests (though not strictly necessary).
    srand(0);

    // Test 1: Basic permutation with distinct values.
    int a = 10, b = 20, c = 30, d = 40;
    int sum_before = a + b + c + d;
    performCustomShuffle(a, b, c, d);
    assert(a + b + c + d == sum_before);  // set of values unchanged
    int values[] = {a, b, c, d};
    std::sort(values, values + 4);
    assert(values[0] == 10 && values[1] == 20 && values[2] == 30 && values[3] == 40);

    // Test 2: Values with duplicates must still be a permutation.
    int x = 5, y = 5, z = 7, w = 7;
    performCustomShuffle(x, y, z, w);
    int vals[] = {x, y, z, w};
    std::sort(vals, vals + 4);
    assert(vals[0] == 5 && vals[1] == 5 && vals[2] == 7 && vals[3] == 7);

    // Test 3: All same values – result must still sum correctly.
    int p = 3, q = 3, r = 3, s = 3;
    performCustomShuffle(p, q, r, s);
    assert(p == 3 && q == 3 && r == 3 && s == 3);

    // Test 4: Negative and zero values.
    int n1 = -1, n2 = 0, n3 = -5, n4 = 100;
    int neg_sum = n1 + n2 + n3 + n4;
    performCustomShuffle(n1, n2, n3, n4);
    assert(n1 + n2 + n3 + n4 == neg_sum);
    int neg_vals[] = {n1, n2, n3, n4};
    std::sort(neg_vals, neg_vals + 4);
    assert(neg_vals[0] == -5 && neg_vals[1] == -1 && neg_vals[2] == 0 && neg_vals[3] == 100);

    // Test 5: Many calls to verify no crash and consistent set of values.
    int v1=1, v2=2, v3=3, v4=4;
    for (int i = 0; i < 1000; ++i) {
        performCustomShuffle(v1, v2, v3, v4);
        int arr[] = {v1, v2, v3, v4};
        std::sort(arr, arr+4);
        assert(arr[0]==1 && arr[1]==2 && arr[2]==3 && arr[3]==4);
    }

    return 0;
}

#include <cstdlib>

// Randomly permutes the values of a, b, c, d in place.
// Each of the 24 possible orderings is equally likely.
// Uses manual Fisher–Yates shuffle on an index array, then maps original values.
void performCustomShuffle(int &a, int &b, int &c, int &d) {
    // Step 1: Create an index array initialized to {0,1,2,3}.
    int order[4] = {0, 1, 2, 3};

    // Step 2: Fisher–Yates shuffle on the indices.
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);  // random index in [0, i]
        // Swap order[i] and order[j]
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    // Step 3: Save original values before overwriting.
    int original[4] = {a, b, c, d};

    // Step 4: Assign back according to the shuffled order.
    a = original[order[0]];
    b = original[order[1]];
    c = original[order[2]];
    d = original[order[3]];
}

// The core algorithm follows the classic Fisher–Yates (or partial‑shuffle) idea but implemented manually. We need to produce a permutation of four distinct indices (0,1,2,3) and then map the original values to those indices. A common but flawed approach is to repeatedly generate random indices until we find one not already used; this can be inefficient and, in the worst case, lead to long loops if `rand()` is biased or if we are unlucky. A better approach is to perform a deterministic shuffling of an index array using a loop: start with an array `order = {0,1,2,3}`. For each position `i` from 3 down to 1, pick a random index `j` between 0 and `i` (inclusive) using `rand() % (i+1)`, then swap `order[i]` and `order[j]`. This produces a uniformly random permutation with only O(4) operations and no rejection sampling. After that, create a temporary array holding the original values (`orig = {a,b,c,d}`), then assign `a = orig[order[0]]`, `b = orig[order[1]]`, `c = orig[order[2]]`, `d = orig[order[3]]`. Note that we must store the original values before overwriting them, because assignments happen sequentially. The function uses only constant extra space (the `order` and `orig` arrays), so space complexity is O(1). Time complexity is O(1) (constant number of operations). Edge cases: values may be equal; that is fine because the mapping is by position, not by value. The loop runs exactly three iterations (for i=3,2,1) guaranteeing finite execution. We also ensure that `rand()` is not called with a modulus that could cause bias, but for the purposes of this task, using `rand() % (i+1)` is acceptable. Since the function only rearranges values, there are no other edge cases.
