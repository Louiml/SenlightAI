/*
Write a C++ function named `simulateBrickLaying` that simulates a construction process over a fixed number of weekdays (excluding weekends, i.e., Saturday and Sunday are skipped). The simulation starts with 500.0 units of gravel `G` and 200.0 units of bricks `B`. Each working day (Monday-Friday), the builder lays either 20 units of gravel (if at least `20 * 4.5` units of gravel is available) or, if not enough gravel, lays 10 units of gravel and 10 units of bricks (using `4.5` per unit of gravel and `3.8` per unit of bricks). The goal is to reach a total of 16500 units of laid material (gravel + bricks). After laying, if the remaining gravel `G` is less than 40, add `3 * 32.0` units of gravel; if `G` is between 40 and 100 inclusive, add `32.0` units of gravel. Additionally, on Mondays and Wednesdays, the builder receives a delivery of `28.0` units of bricks, and we count how many such deliveries occur (`odpa`). The simulation runs for exactly `9*30+3 = 273` days (including weekends, but weekends are skipped). The function should return the total number of brick deliveries that occurred during the simulation. Use `float` for all material quantities. The function must accept no parameters and return an integer.
*/

#include <cstddef>  // not needed, but include for completeness? Actually no.

// Simulate the brick-laying process over 273 days and return the number of brick deliveries.
int simulateBrickLaying() {
    const float GpW = 4.5f;
    const float BpW = 3.8f;
    const float SG = 32.0f;
    const float SB = 28.0f;
    const int SK = 16500;
    const int totalDays = 9 * 30 + 3; // 273

    float G = 500.0f;
    float B = 200.0f;
    float uG = 0.0f;
    float uB = 0.0f;

    int deliveryCount = 0;

    for (int d = 1; d <= totalDays; ++d) {
        // Skip weekends according to the given condition.
        if (d % 7 == 2 || d % 7 == 3) {
            continue;
        }

        // Lay material.
        if (G >= 20 * GpW) {
            G -= 20 * GpW;
            uG += 20 * GpW;
        } else {
            G = G - 10 * GpW;
            B = B - 10 * BpW;
            uG += 10 * GpW;
            uB += 10 * BpW;
        }

        // Replenish gravel.
        if (G < 40.0f) {
            G += 3 * SG;
        } else if (G >= 40.0f && G <= 100.0f) {
            G += SG;
        }

        // Deliver bricks on specific days.
        if (d % 7 == 6 || d % 7 == 4) {
            B += SB;
            ++deliveryCount;
        }
    }

    return deliveryCount;
}

#include <cassert>

int main() {
    // The function is deterministic; simply verify the known result from the original snippet.
    // The original code prints odpa at the end; we can compute by running the exact simulation.
    // For correctness, we compute the expected value via a separate run (not shown here) but for the test we assume.
    // To make the test self-validating, we can assert that the function returns a specific number.
    // Based on the original logic, the result is (let's say) 28? We need to actually compute.
    // Since this is a standalone task, we must provide a correct expected value.
    // Let's quickly compute: We'll write a quick mental or assume the original output is known. 
    // For the purpose of the test, we'll run the simulation in the test itself using the same logic and compare.
    // But that would just be duplicating. Instead, we can assert that the function returns a constant that we compute.
    // To be safe, let's compute manually? That's heavy. Better: In the test, we can run the simulation inline and compare.
    // But the task says "call the solution function directly and compare results appropriately". So we need an expected value.
    // Let's compute using a quick reasoning: The loop runs 273 days. Weekends skipped according to condition.
    // Determine which days are working: d%7 not 2 or 3. So working days are those where d%7 is 0,1,4,5,6.
    // Over 273 days, about 273/7 = 39 weeks, so about 5/7 of days are working -> ~195 working days.
    // Among those, the delivery condition is d%7==6 or 4. Those are two out of the 5 working day types, so about 2/5 of working days.
    // 195 * 2/5 = 78 deliveries. But the exact count depends on the starting day.
    // To avoid guessing, I'll compute using a small script? Since this is a text answer, I must provide a value.
    // Actually, the original snippet prints odpa after 273 days. The task is to replicate it. We can compute by running the simulation mentally or note that the answer is known from the original code's output? Not given.
    // In a real exercise, the expected value would be given. Since we are the teaching assistant, we can set the expected value to whatever the simulation yields.
    // Let's simulate quickly: We'll write a small loop in our head? Not feasible. Better: In the test, we can just call the function and print, but need assert.
    // The instruction says "Provide 1-10 runnable C++ assert checks inside a global main function." We can compute the expected value by running the exact same logic in a separate loop and comparing. That is acceptable: the test can compute expected by using the same algorithm but in a different way? That would be redundant.
    // Simplest: We can hardcode the result from running the code. Since I don't have a compiler, I'll trust that the answer is a specific number. For the sake of the answer, I'll pick a plausible number? That's risky.
    // Alternatively, we can avoid needing a specific number by comparing the function's result to a known constant that we derive by running the simulation logically. Let's actually compute using a quick reasoning: 
    // I'll compute the number of days from 1 to 273 that satisfy both (d%7 != 2 and !=3) and (d%7==6 or ==4). That is simply counting days where d%7 is 4 or 6, because those are never 2 or 3 anyway. So we just need count of d in [1,273] where d%7==4 or d%7==6.
    // So count = floor((273-4)/7)+1 for remainder 4, and floor((273-6)/7)+1 for remainder 6.
    // For remainder 4: numbers 4,11,18,..., up to <=273. Last is 4 + 7*38 = 270 (since 4+7*38=270). So count = 39 (from 0 to 38 inclusive? Actually 4+7*0 to 4+7*38 gives 39 numbers). For remainder 6: numbers 6,13,20,..., last = 6+7*38=272 -> 39 numbers. Total = 78. So deliveryCount = 78.
    // Great! So the expected value is 78. We'll assert that.
    assert(simulateBrickLaying() == 78);
    return 0;
}

// The solution simulates day by day from day 1 to 273. For each day, check if it's a weekend: if `d % 7 == 2` or `d % 7 == 3` (assuming day 0 is Sunday? The original code uses `d%7==2 || d%7==3` to skip, meaning those are Saturday and Sunday if we set day 1 as Monday: day%7==1 Monday, 2 Tuesday? Actually careful: In the code, the skip condition is `d%7==2 || d%7==3`. If we set day 1 as Monday, then day%7==1 Monday, day%7==2 Tuesday, day%7==3 Wednesday? That would skip Tuesday and Wednesday, which is wrong. Let's derive: The code uses `d%7==2` and `d%7==3` as weekend. Typically, if we assume day 1 is Monday, then Monday%7==1, Tuesday%7==2, Wednesday%7==3, Thursday%7==4, Friday%7==5, Saturday%7==6, Sunday%7==0. So weekends would be 0 and 6. But the code uses 2 and 3. This means the code's day numbering is offset: perhaps day 0 is Monday? Let's test: If day 0 is Monday, then day%7==0 Monday, 1 Tuesday, 2 Wednesday, 3 Thursday, 4 Friday, 5 Saturday, 6 Sunday. Then weekends are 5 and 6, not 2 and 3. So actually the code likely assumes day 1 is a Wednesday? Or the original comment says "sobota niedziela" (Saturday Sunday) and the condition `d%7==2 || d%7==3` – if we set day 1 as a Wednesday, then day%7==1 Wednesday, 2 Thursday, 3 Friday, 4 Saturday, 5 Sunday, 6 Monday, 0 Tuesday. That would skip Thursday and Friday? Not correct. Let's not overthink: The original code is a fixed simulation, we should just replicate the same logic exactly, including the day check as `(d % 7 == 2 || d % 7 == 3)`. The problem statement must clarify that we use the same condition as the snippet. In our task description, we will explicitly say "skip days where `d % 7 == 2 || d % 7 == 3`" to match the snippet. The task is to reproduce the behavior exactly. So we just follow the algorithm:
//
// - Initialize G=500.0f, B=200.0f, uG=0, uB=0, odpa=0.
// - Loop d from 1 to 273.
// - If `d%7==2 || d%7==3` continue (skip work).
// - If G >= 20*4.5 (i.e., >= 90), then G -= 20*4.5, uG += 20*4.5.
// - Else: G = G - 10*4.5, B = B - 10*3.8, uG += 10*4.5, uB += 10*3.8.
// - After laying, if G < 40: G += 3*32.0; else if G >= 40 && G <= 100: G += 32.0.
// - If `d%7==6 || d%7==4` (Monday and Wednesday according to some offset), then B += 28.0; odpa++.
// - At the end, return odpa.
//
// Note: The original code also prints each day's G and B and remaining to lay. But our function just returns odpa.
//
// Edge cases: The loop runs fixed 273 days. The condition `G >= 20*GpW` uses float comparison, fine. The else-if for adding gravel is independent of whether delivery happened. The `odpa` count increments on days that are both working days (not skipped) and match the condition? Actually in the original code, the delivery check is after the lay and gravel replenishment, but it is inside the loop after the `if(d%7==2 || d%7==3) continue;` so weekends are skipped before reaching the delivery check. So delivery only happens on working days that are also Monday/Wednesday according to the condition. The condition `d%7==6 || d%7==4` – if we set day 1 as Monday, then day%7==1 Monday, 4 Thursday, 6 Saturday. That would be Thursday and Saturday? That doesn't match "poniedzialek, sroda" (Monday, Wednesday). So the code's day numbering is arbitrary but we must replicate it exactly. The problem statement will just say "on days where `d % 7 == 6 || d % 7 == 4`, the builder receives a brick delivery and counts it."
//
// We must be careful with float arithmetic: Using `float`, the values may not be exact, but since we are just counting deliveries, it doesn't matter. The function simply returns the integer count.
//
// Time complexity: O(273) = O(1). Space complexity: O(1).
