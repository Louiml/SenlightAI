// Write a C++ function `int minimumPizzas(int puebla, int chihuahua, int guanajuato)` that takes three non-negative integers representing the number of people from three cities: Puebla, Chihuahua, and Guanajuato. The function must return the minimum number of pizzas needed to feed everyone, following these rules: each pizza is cut into 4 slices. A Puebla person eats exactly 1 slice and can be paired with one Guanajuato person where one pizza feeds one from each (using 2 slices total, but the pairing is treated as consuming one pizza regardless of leftover slices). However, the optimal strategy is: first pair Puebla people with Guanajuato people one-to-one, each such pair consumes one pizza. Then pair remaining Puebla people in groups of two with one Chihuahua person: each such triple (2 Puebla + 1 Chihuahua) consumes one pizza. After that, if any Puebla people remain, each group of up to 4 of them consumes one pizza (ceil division by 4). If any Chihuahua remain, each pair consumes one pizza (ceil division by 2). Any remaining Guanajuato each consumes one full pizza (since they need 4 slices but the pairing rule only works with Puebla). The function must return the total number of pizzas. Assume inputs are non-negative and fit in an `int`. The function should use `const` where appropriate and be self-contained with necessary headers.
The solution follows a greedy algorithm that matches the original code exactly: perform the Puebla–Guanajuato pairing first, consuming one pizza per pair and decrementing both counts until one is zero. Next, pair Puebla with Chihuahua at a 2:1 ratio, consuming one pizza per triple, decrementing Puebla by 2 and Chihuahua by 1 until either is exhausted. Then, for any leftover Puebla, compute the ceiling of divide by 4 (using integer arithmetic `(p + 3) / 4`). For leftover Chihuahua, compute ceiling of divide by 2 (using `(c + 1) / 2`). For leftover Guanajuato, add one per person. This order is optimal because it uses the most efficient pairings first. Edge cases: all zeros (returns 0), only one city has people (just compute the relevant ceiling or sum), large values (no overflow since `int` is fine and sums are linear). The loop-based approach runs in O(n) worst-case if counts are large, but since each iteration reduces at least one count by 1, it is O(P + G + C) time, but typically the number of iterations is bounded by the maximum input. However, we can make it constant time by using arithmetic directly after the first pairing, but the original uses loops and we can keep that for clarity. Space complexity is O(1).
#include <algorithm>

// Returns the minimum number of pizzas needed to feed the given people.
// Pairing strategy: Puebla+Guanajuato (1:1) first, then 2 Puebla+1 Chihuahua,
// then leftover Puebla in groups of 4, leftover Chihuahua in pairs,
// and each leftover Guanajuato alone.
int minimumPizzas(int puebla, int chihuahua, int guanajuato) {
    int pizzas = 0;
    
    // Pair Puebla with Guanajuato one-to-one
    while (puebla > 0 && guanajuato > 0) {
        ++pizzas;
        --puebla;
        --guanajuato;
    }
    
    // Pair two Puebla with one Chihuahua
    while (puebla > 0 && chihuahua > 0) {
        ++pizzas;
        puebla -= 2;
        --chihuahua;
    }
    
    // Leftover Puebla: ceil(puebla / 4)
    if (puebla > 0) {
        pizzas += (puebla + 3) / 4;
    }
    
    // Leftover Chihuahua: ceil(chihuahua / 2)
    if (chihuahua > 0) {
        pizzas += (chihuahua + 1) / 2;
    }
    
    // Leftover Guanajuato: one per person
    pizzas += guanajuato;
    
    return pizzas;
}
#include <cassert>

int main() {
    // Pairing with both Puebla and Guanajuato
    assert(minimumPizzas(1, 0, 1) == 1);
    assert(minimumPizzas(5, 0, 5) == 5);
    
    // Pairing with Puebla and Chihuahua
    assert(minimumPizzas(2, 1, 0) == 1);
    assert(minimumPizzas(4, 2, 0) == 2); // 2 triples
    
    // Leftover Puebla only
    assert(minimumPizzas(3, 0, 0) == 1); // ceil(3/4)=1
    assert(minimumPizzas(5, 0, 0) == 2); // ceil(5/4)=2
    
    // Leftover Chihuahua only
    assert(minimumPizzas(0, 1, 0) == 1); // ceil(1/2)=1
    assert(minimumPizzas(0, 3, 0) == 2); // ceil(3/2)=2
    
    // Leftover Guanajuato only
    assert(minimumPizzas(0, 0, 2) == 2);
    
    // Mixed scenario (example from snippet: input 1 1 1 => first pair uses 1 pizza, then 0 left)
    assert(minimumPizzas(1, 1, 1) == 1);
    
    // Complex: 2 Puebla, 1 Chihuahua, 1 Guanajuato => pair P+G (1 pizza), then 1 P left and 1 C left => no triple, 1P needs 1 pizza, 1C needs 1 pizza, total 3
    assert(minimumPizzas(2, 1, 1) == 3);
    
    // All zeros
    assert(minimumPizzas(0, 0, 0) == 0);
}
