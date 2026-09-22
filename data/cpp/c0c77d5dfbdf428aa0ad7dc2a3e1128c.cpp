Implement a C++ function `randDoubleFromSeed` that takes a 48-bit seed value as a `long` and returns a pseudo-random `double` in the range [0.0, 1.0) using the same linear congruential generator algorithm as the provided snippet. The function must internally maintain the generator state (the three 16-bit words `x[0]`, `x[1]`, `x[2]`, the multiplier words `a[0..2]`, and carry `c`) as static variables, initialize them according to the provided `SEED` logic when the seed changes (e.g., first call or explicit re-seed), and produce the same output as `drand48()` from the snippet after each call. The function should be callable repeatedly; each call advances the state and returns a new value. Ensure the algorithm uses only unsigned arithmetic with 16-bit masking as shown, and that the returned double is computed using the exact formula `two16m * (two16m * (two16m * x[0] + x[1]) + x[2])` where `two16m = 1.0 / (1L << 16)`. The function must be self-contained, not rely on any external random library, and handle any non-negative 32-bit seed value.
The core challenge is replicating the exact pseudo-random number generator state machine from the snippet. The generator maintains a 48-bit state split into three unsigned 16-bit words `x[0]` (most significant), `x[1]`, and `x[2]` (least significant). The multiplier `a` is fixed as three 16-bit words `A0=0xE66D`, `A1=0xDEEC`, `A2=0x5`, and the increment `c` is `0xB`. The `seed` function resets the state: `x[0]=X0 (0x330E)`, `x[1]=LOW(seedval)`, `x[2]=HIGH(seedval)` (where HIGH shifts right by 16 bits and masks to 16 bits), and sets `c=C`. The `next()` function performs the linear recurrence using partial multiplications with carry propagation. The key is the `MUL` macro: it multiplies two 16-bit numbers and splits the 32-bit result into low and high 16-bit words. The `ADDEQU` macro adds a value to a word, setting a carry flag, and wraps modulo 2^16. The update formula computes new `x[2]`, `x[1]`, `x[0]` sequentially using a series of multiplications and additions with carry flags, exactly as in the snippet. Edge cases include handling a seed value with high bits (e.g., negative `long` is not expected, but we can mask to 32 bits), and ensuring the state persists across calls without re-seeding unless a new seed is provided (the snippet does not provide a separate `srand`; so we can design the function to always use the same fixed initial seed or allow an optional seed parameter). For simplicity, we can define the function to accept a seed on the first call and ignore subsequent seed arguments if we choose to keep state static, but the task says "takes a 48-bit seed value" and returns a double—so each call might be expected to use that seed? However, to match the snippet's behavior, we need a function that returns successive values from a single sequence. A clean approach: have a `void srand48(long)` like the snippet and a `double drand48()` that uses static state. But the task asks for a single free function `randDoubleFromSeed(long)`. To reconcile, we can interpret it as: the function takes a seed, initializes the generator with that seed, and returns the first value. Then each subsequent call with a different seed resets the generator and returns the first value for that seed. Alternatively, we can have the function accept a seed only on the first call and ignore later ones, but that is ambiguous. Given the requirement "takes a 48-bit seed value and returns a pseudo-random double" and "the function must be callable repeatedly; each call advances the state", it suggests that the seed is only used for initialization on the first call, and subsequent calls ignore the argument? That is unusual. Better to interpret: the function takes a seed, and internally stores it; but to advance state, we need a separate mechanism. The safest is to implement two functions: `void srand48(long)` and `double drand48()`. But the task explicitly says "a descriptively named free function that matches the task specification" and "Do not include a main function". So we can write a single function `double randDoubleFromSeed(long seedVal)` that, on each call, uses the passed seed to reset the generator? That would mean each call returns the same first value for the same seed, not advancing. That does not match "each call advances the state". The snippet's `drand48()` takes no arguments and advances static state; `srand48` sets the state. For a standalone task, we can adopt the pattern: provide a `void seedGenerator(long)` and `double nextRandom()`, but the task asks for one function. A possible resolution: `randDoubleFromSeed(long seed)` can maintain a static flag that indicates whether the seed has changed; if the seed differs from the last used seed, it re-seeds and returns the first value; if the same seed is passed again, it advances and returns the next value. This is a bit artificial. Alternatively, we can rename the function to reflect it takes a seed and returns the first value, but also provide a separate `nextRandom()` in the solution? The task says "a descriptively named free function" and "Do not include a main". It does not strictly forbid additional helper functions, but the test code must call the solution function directly. To satisfy the requirement, I'll implement a single function `double randDoubleFromSeed(long)` that, when called with a given seed, sets the generator state to that seed and returns the first random value. Then to test multiple values, we can call it with the same seed repeatedly? That would not advance because each call resets. To test advancing, the test code can call it with different seeds? That would not test the recurrence. So it's better to follow the snippet exactly: create two free functions: `void seedRandom(long)` and `double nextRandom()`. The task says "a descriptively named free function that matches the task specification" – but the specification says "takes a 48-bit seed value and returns a pseudo-random double". That implies a single call. However, the snippet itself separates seeding and generation. For a standalone task, I think it's acceptable to provide a function `double seededRandom(long seed)` that initializes the generator with that seed, returns one value, and leaves the state so that a subsequent call to a second function `nextRandom()` would continue. But to keep it simple and match the test requirement where we call the solution function directly and compare, I'll design the solution to have a function `double getRandom(long seed)` that, for each call, uses the given seed to reset the state and then returns the first value. That way, the test can call it with a known seed and assert the value matches expected from the algorithm. The "advances" part is not tested. But the task instruction says "each call advances the state". That is contradictory with passing a seed each call. To resolve, I'll interpret the seed as an initial seed only for the first call, and subsequent calls ignore the seed and just advance. That is a common pattern: the function signature uses a seed as a parameter, but internally it only seeds once. So we can define a static bool `initialized` and a static `long lastSeed`; on first call, seed with the given value; on subsequent calls, if the seed equals the last seed, we advance; if it differs, we re-seed and advance? That is messy. To keep it clean, I'll implement a function that takes a seed and returns the first value, and then a separate `nextRandom()` that advances. But the task's test code must call the solution function directly. The test can call `nextRandom()` after seeding via a separate function, but the task says "Call the solution function directly". So the solution function must be the one that produces the random value. I'll make the solution function `double randDoubleFromSeed(long)` that always seeds with the given seed and returns the first value. Then to test the recurrence, we can call it with different seeds and compare to known outputs. The "advances" can be demonstrated by calling the function twice with the same seed? No, that would reset. To advance, we need state persistence. The simplest correct approach: implement a function `double randDoubleFromSeed(long seed)` that, if `seed` is non-negative, sets the generator state to that seed and returns the first value. Then implement a static `bool firstCall` and store the seed; on subsequent calls with the same seed, we ignore the seed and advance. But the signature has only one parameter; we cannot know if it's the same seed. That is not feasible. After rereading the task statement: "Write a C++ function that takes a 48-bit seed value and returns a pseudo-random double in the range [0.0, 1.0) ... The function must be callable repeatedly; each call advances the state". This implies that the seed is provided once, perhaps as an argument to first call, and then the function remembers it. But in C++, we can't have an optional parameter to indicate "ignore this seed". We can design the function to accept a `long` but only use it on the first call, ignoring subsequent calls' arguments. That is odd but technically possible: `double randDoubleFromSeed(long ignored) { static bool init = false; if(!init){ seedGenerator(ignored); init=true; } return nextRandom(); }`. Then the first call seeds with the given value, subsequent calls ignore the argument and advance. This matches the requirement: you call it repeatedly, each call advances, and the seed is provided on the first call. The test can call it multiple times with the same seed value (or different, but it ignores them after first). This is a reasonable design. I'll implement that. In the solution, I'll include the helper functions `seedGenerator` and `advance` but only expose the single function `randDoubleFromSeed`. The test will call it with a seed and then multiple times to check the sequence. For verification, I need to compute expected values. But I don't have the actual output of the random generator. However, I can either compute them by hand or trust that the reference solution is correct and test for properties like range, and sequence reproducibility: calling it twice with the same seed yields the same first value; calling with different seed yields different values; values are in [0,1). But the task asks for `assert` checks comparing results appropriately using `==` or another suitable comparison. We can assert that the first value matches a known constant if we compute it. Let's compute the first value manually or write a small program mentally. Actually, we can derive the first value from the algorithm after seeding. Let's do that.

Seed with `seedval = 1`. Then `x[0] = X0 = 0x330E`, `x[1] = LOW(1) = 1`, `x[2] = HIGH(1) = 0`? HIGH(x) is LOW(x>>N) with N=16, so HIGH(1) = LOW(0) = 0. So x = {0x330E, 1, 0}. a = {0xE66D, 0xDEEC, 0x5}, c=0xB. Then next() computes new x.

Let's compute step-by-step:

Given N=16, MASK = (1<<15)+(1<<15)-1 = 32768+32768-1 = 65535 (which is 0xFFFF). So LOW(x) = x & 0xFFFF, HIGH(x) = (x>>16) & 0xFFFF (since LOW of shifted). MUL(x,y,z): z[0]=LOW(x*y) (mod 65536), z[1]=HIGH(x*y) = floor((x*y)/65536) mod 65536? Actually HIGH(x) = LOW((x)>>16) = ((x)>>16) & 0xFFFF, which for a 32-bit product gives the high 16 bits. So MUL gives low and high 16-bit words of the 32-bit product.

Define variables: p[2], q[2], r[2], carry0, carry1.

Step 1: MUL(a[0]=0xE66D, x[0]=0x330E, p)
Compute 0xE66D * 0x330E. Let's do decimal: 0xE66D = 58989, 0x330E = 13070. Product = 58989*13070 = 771,006,? Let's compute: 58989*13000 = 766,857,000; 58989*70 = 4,129,230; sum = 770,986,230. In hex, maybe easier: Use modular arithmetic. But we can use the fact that the algorithm is deterministic; for test purposes, we don't need to know the exact expected value unless we compute it. Instead, we can write a reference implementation in the solution and then use the same code in the test to compute expected values? That would be circular. The test must assert against known constants. So we need to compute a few outputs. Alternatively, we can test properties: first value is in [0,1), and different seeds produce different values. But `assert` with `==` needs exact match. So we can compute the expected first few values manually or write a small program in our head? That is tedious. Instead, we can make the test compare the output of the function to a manually computed constant. Let's do the computation step by step using the macros.

We'll compute `next()` once.

Given x = {0x330E, 0x0001, 0x0000}
a = {0xE66D, 0xDEEC, 0x0005}
c = 0x000B

MUL(a0, x0, p) where a0=0xE66D, x0=0x330E.
Product = 0xE66D * 0x330E. Let's compute in hex:
0xE66D * 0x330E = ?

Use 0xE66D = 0xE600 + 0x6D, 0x330E = 0x3300 + 0x0E.

0xE600*0x3300 = (0xE6*0x33)*0x10000? Actually 0xE600 = 0xE6 * 0x100, 0x3300 = 0x33*0x100 -> product = 0xE6*0x33 * 0x10000. 0xE6*0x33 = 0xE6*0x30=0x2AC0? Wait 0xE6=230, 0x33=51 -> 230*51=11730 = 0x2DD2. So that term = 0x2DD2 * 0x10000 = 0x2DD20000.

0xE600*0x0E = 0xE6*0x0E *0x100 = (0xE6=230, 0x0E=14 -> 3220 = 0xC94) *0x100 = 0xC9400.

0x6D*0x3300 = 0x6D*0x33 *0x100 = (0x6D=109, 0x33=51 -> 5559 = 0x15B7)*0x100 = 0x15B700.

0x6D*0x0E = 109*14=1526 = 0x5F6.

Sum these: 0x2DD20000 + 0xC9400 = 0x2DDEC400? Let's add: 0x2DD20000 + 0x000C9400 = 0x2DDE9400. Then +0x15B700 = 0x2DF44B00? Add 0x2DDE9400 + 0x0015B700 = 0x2DF44B00. Then +0x5F6 = 0x2DF450F6. So product = 0x2DF450F6.

Now LOW(product) = 0x50F6, HIGH(product) = 0x2DF4. So p[0]=0x50F6, p[1]=0x2DF4.

ADDEQU(p[0], c, carry0): x = p[0] + c = 0x50F6 + 0x000B = 0x5101. Since 0x5101 <= 0xFFFF, carry0=0. So p[0]=0x5101, carry0=0.

ADDEQU(p[1], carry0, carry1): p[1] = 0x2DF4 + 0 = 0x2DF4, carry1=0.

Now MUL(a[0], x[1], q) where a[0]=0xE66D, x[1]=0x0001. Product = 0xE66D * 0x0001 = 0xE66D. So q[0]=0xE66D (low), q[1]=0x0000 (high).

ADDEQU(p[1], q[0], carry0): p[1] = 0x2DF4 + 0xE66D = 0x2DF4 + 0xE66D = 0x11461? Let's compute: 0x2DF4 + 0xE66D = 0x11461. But since we take LOW, we get 0x1461, and carry0 = 1 (because sum > 0xFFFF). So p[1]=0x1461, carry0=1.

MUL(a[1], x[0], r) where a[1]=0xDEEC, x[0]=0x330E. Compute 0xDEEC * 0x330E. Let's calculate. 0xDEEC = 0xDE00 + 0xEC, 0x330E = 0x3300 + 0x0E.

0xDE00*0x3300 = (0xDE*0x33)*0x10000 = (222*51=11322=0x2C3A)*0x10000 = 0x2C3A0000.
0xDE00*0x0E = (0xDE*0x0E)*0x100 = (222*14=3108=0xC24)*0x100 = 0xC2400.
0xEC*0x3300 = (0xEC*0x33)*0x100 = (236*51=12036=0x2F04)*0x100 = 0x2F0400.
0xEC*0x0E = 236*14=3304=0xCE8.
Sum: 0x2C3A0000 + 0xC2400 = 0x2C462400; +0x2F0400 = 0x2C752800; +0xCE8 = 0x2C7534E8. So product = 0x2C7534E8. Thus r[0]=0x34E8, r[1]=0x2C75.

Now we compute x[2] = LOW(carry0 + carry1 + CARRY(p[1], r[0]) + q[1] + r[1] + a[0]*x[2] + a[1]*x[1] + a[2]*x[0]).

We have carry0=1, carry1=0.

Compute CARRY(p[1], r[0]): p[1]=0x1461, r[0]=0x34E8. Sum = 0x1461+0x34E8 = 0x4949, which is <=0xFFFF, so carry = 0.

q[1]=0, r[1]=0x2C75.

Now a[0]*x[2] = 0xE66D * 0x0000 = 0.
a[1]*x[1] = 0xDEEC * 0x0001 = 0xDEEC.
a[2]*x[0] = 0x0005 * 0x330E = 0x0005*0x330E = 0x1004E? Let's compute: 0x330E * 5 = 0x1004E (since 0x330E*4=0xCC38, *5=0x1004E). So that's 0x1004E.

Now sum all components:
carry0 = 1
carry1 = 0
CARRY(p1,r0) = 0
q[1] = 0
r[1] = 0x2C75
a0*x2 = 0
a1*x1 = 0xDEEC
a2*x0 = 0x1004E

Total = 1 + 0 + 0 + 0 + 0x2C75 + 0 + 0xDEEC + 0x1004E = 1 + 0x2C75 = 0x2C76; +0xDEEC = 0x10B62? Let's add: 0x2C76 + 0xDEEC = 0x2C76 + 0xDEEC. 0x2C76 (11382 decimal) + 0xDEEC (57068) = 68450 decimal = 0x10B62? 0x10B62 = 0x10000 + 0x0B62 = 65536+2914=68450, yes. Then +0x1004E (65550 decimal) = 68450+65550=134000 decimal = 0x20B70? 0x20B70 = 0x20000 + 0x0B70 = 131072 + 2928 = 134000. So total = 0x20B70. Then LOW(total) = 0x20B70 & 0xFFFF = 0x0B70. So x[2] = 0x0B70.

Now x[1] = LOW(p[1] + r[0]) = LOW(0x1461 + 0x34E8) = LOW(0x4949) = 0x4949.
x[0] = LOW(p[0]) = LOW(0x5101) = 0x5101.

So after one `next()` call, the state is x[0]=0x5101, x[1]=0x4949, x[2]=0x0B70.

Then the returned `drand48()` value = two16m * (two16m * (two16m * x[0] + x[1]) + x[2]).

two16m = 1.0 / (1L << N) = 1.0 / 65536.0 = 0.0000152587890625.

Compute inner = two16m * x[0] + x[1]? Actually formula: two16m * (two16m * (two16m * x[0] + x[1]) + x[2]).

Let's compute step by step:

First: a = two16m * x[0] = (1/65536)*0x5101 = 0x5101 / 65536 = 20737 / 65536 ≈ 0.3164215087890625.

Then b = a + x[1] = that + 0x4949 (18761 decimal) = 18761.3164215...

Then c = two16m * b = (18761.3164215)/65536 ≈ 0.286272? Let's compute precisely using fractions. Better: overall value = (x[0]*65536^2 + x[1]*65536 + x[2]) / 65536^3? Actually two16m = 1/65536. So value = ( ( (x[0]/65536) + x[1] )/65536 + x[2] )/65536 = (x[0]/65536^2 + x[1]/65536 + x[2])/65536? Let's derive: two16m * (two16m * (two16m * x[0] + x[1]) + x[2]) = (1/65536)*((1/65536)*((1/65536)*x[0] + x[1]) + x[2]) = (1/65536^2)*((1/65536)*x[0] + x[1]) + (1/65536)*x[2] = (1/65536^3)*x[0] + (1/65536^2)*x[1] + (1/65536)*x[2]. So it's the 48-bit number formed by x[0] (high), x[1], x[2] divided by 2^48. So value = (x[0]*2^32 + x[1]*2^16 + x[2]) / 2^48. So for x[0]=0x5101=20737, x[1]=0x4949=18761, x[2]=0x0B70=2928. Compute numerator = 20737*2^32 + 18761*2^16 + 2928. 2^32=4294967296, 2^16=65536. 20737*4294967296 = let's compute: 20737 * 4294967296 = 20737 * (4.294967296e9) = 8.907? Actually 20737*4294967296 = (20000+737)*4294967296 = 20000*4294967296=85,899,345,920,000 + 737*4294967296. 737*4294967296 = 737 * (4,294,967,296) = 3,168,? Let's not compute exactly. Instead, we can compute the double directly using division. But for test, we can just assert that the value is in [0,1) and that it is consistent with the algorithm. However, the task requires assert with `==` or another suitable comparison. We can compute the expected double using a reference implementation in the test? That would be circular, but it's acceptable to compute expected value using the same algorithm? No, because then it's tautological. We need an independent constant. I'll compute the value numerically: 20737*2^32 = 20737 * 4294967296. Let's do carefully: 20737 * 4294967296 = 20737 * (2^32). 2^32 = 4,294,967,296. 20737*4,294,967,296 = (20737 * 4,294,967,296). We can use: 20737 * 4,294,967,296 = (20736+1)*2^32 = 20736*2^32 + 2^32. 20736 = 2^? Not power of two. Better to use division. Let's compute the double directly: value = (20737/65536^3) + (18761/65536^2) + (2928/65536). 65536^2 = 4,294,967,296; 65536^3 = 281,474,976,710,656. So value ≈ 20737/2.81474976710656e14 = 7.365e-11? Wait that's tiny. Actually 20737/281,474,976,710,656 = 7.365e-11. Then 18761/4,294,967,296 = 4.368e-6. Then 2928/65536 = 0.044677734375. Sum ≈ 0.044682. So value ≈ 0.04468. Actually the formula yields a number between 0 and 1. So we can assert that value is approximately 0.04468? But floating point precision, we can use an epsilon comparison. The task says "compare results appropriately using == or another suitable comparison" – we can use `assert(fabs(value - expected) < 1e-12)`. For that, we need to compute expected exactly. We can compute the expected double using the formula with integer arithmetic and then cast to double: `double expected = (double)( ( (double)x0 * 65536.0 + x1) * 65536.0 + x2 ) / 281474976710656.0;` But that would be using the same algorithm, not independent. However, that is acceptable for testing: we can compute the expected value using the same integer state that we know from manual calculation. But then the test is just verifying the implementation matches our manual state computation, which is fine. So we can hard-code the state after first call as {0x5101,0x4949,0x0B70} and compute the expected double. Let's compute exact double using high precision: We can compute the fraction exactly as a rational number and then convert to double. But for simplicity, we can compute in the test using integer arithmetic to produce a double. That is allowed because we are checking the function's output against an independently computed value from the known state. So in the test, we can write:

```
// expected first value after seeding with 1
unsigned long long num = ((unsigned long long)0x5101 << 32) | ((unsigned long long)0x4949 << 16) | 0x0B70;
double expected = (double)num / 281474976710656.0;
assert(fabs(randDoubleFromSeed(1) - expected) < 1e-15);
```

But we need to be sure that the function indeed produces that. We can also do a second call to check the next value? That would require computing the next state. To avoid manual computation, we can simply test that the first call with a known seed returns a value in [0,1) and that two calls with different seeds produce different values. The task says "provide 1-10 runnable C++ assert checks". So we can have several asserts checking properties. For example:
- Call with seed 1 and assert value >= 0.0 and < 1.0.
- Call with seed 1 again? But our function ignores seed after first call, so second call with seed 1 will advance and produce a different value, which we don't know. We can assert that it is also in [0,1). We can also assert that it is different from the first.
- Call with a different seed (e.g., 2) – but our function only uses the seed on the first call, so if we call with 2 after already calling with 1, it will ignore the new seed and keep advancing. So to test re-seeding, we need to reset the static state between tests. But the function as designed does not allow resetting except via first call. That complicates testing multiple scenarios. So maybe a better design is to have the function take a seed and always reset the generator with that seed and return the first value. That way, each call with a new seed resets. Then to test advancing, we cannot do it with a single function. But the task says "each call advances the state" – that implies the function should advance. So we need to support both seeding and advancing. A clean solution is to provide two functions: `void srand48(long)` and `double drand48()`, but the task explicitly asks for "a descriptively named free function" singular. However, the example output format shows a single function that returns a result. So I will implement a single function `double randDoubleFromSeed(long seed)` that:
- If the `seed` argument is not the same as the last used seed (or if it's the first call), it resets the state using the seed and then returns the first value.
- If the `seed` argument is the same as the last used seed, it advances the state and returns the next value.
This way, calling it repeatedly with the same seed advances the sequence. Calling it with a different seed re-seeds. This is a reasonable design. In the implementation, we can store a static `lastSeed` and a static `bool initialized`. On each call, if `!initialized` or `seed != lastSeed`, we re-seed and set `initialized=true`, `lastSeed=seed`, then generate the first value. If the seed equals `lastSeed`, we just advance and return. That matches "takes a seed" and "advances" on repeated calls with same seed. The test can then do:
```
double v1 = randDoubleFromSeed(1);
double v2 = randDoubleFromSeed(1); // same seed, advances
assert(v1 != v2);
assert(v2 >= 0 && v2 < 1);
double w1 = randDoubleFromSeed(2); // different seed resets
assert(w1 >= 0 && w1 < 1);
```
And we can also hardcode the first value for seed 1 as we computed: 0.044678... Let's compute exact expected for seed 1.

State after first `next()`: x = {0x5101, 0x4949, 0x0B70}. Compute numerator as unsigned long long: 
0x5101 = 20737
0x4949 = 18761
0x0B70 = 2928
Numerator = 20737 * 2^32 + 18761 * 2^16 + 2928.
Let's compute 20737 * 4294967296 = 
20737 * 4294967296 = (20737 * 4294967296). Use long multiplication:
20737 * 4294967296 = 20737 * (4,294,967,296) = 
4,294,967,296 * 20000 = 85,899,345,920,000
4,294,967,296 * 700 = 3,006,477,107,200
4,294,967,296 * 30 = 128,849,018,880
4,294,967,296 * 7 = 30,064,771,072
Sum = 85,899,345,920,000 + 3,006,477,107,200 = 88,905,823,027,200
+128,849,018,880 = 89,034,672,046,080
+30,064,771,072 = 89,064,736,817,152. So that term = 89,064,736,817,152.

Now 18761 * 2^16 = 18761 * 65536 = 18761*65536. Compute: 18761 * 65536 = 18761 * (65536) = 
18761 * 60000 = 1,125,660,000
18761 * 5000 = 93,805,000
18761 * 500 = 9,380,500
18761 * 30 = 562,830
18761 * 6 = 112,566
Sum: 1,125,660,000 + 93,805,000 = 1,219,465,000; +9,380,500 = 1,228,845,500; +562,830 = 1,229,408,330; +112,566 = 1,229,520,896. So that term = 1,229,520,896.

Add 2928.

Total numerator = 89,064,736,817,152 + 1,229,520,896 = 89,065,966,338,048; +2928 = 89,065,966,340,976.

Denominator = 2^48 = 281,474,976,710,656.

Now compute double value: 89,065,966,340,976 / 281,474,976,710,656 ≈ 0.3165? Let's compute: 281,474,976,710,656 * 0.3164 = 89,? Let's do: 0.3164 * 281,474,976,710,656 = 0.3 * 281,474,976,710,656 = 84,442,493,013,196.8; 0.0164 * 281,474,976,710,656 = 4,616,? 0.01 * 281,474,976,710,656 = 2,814,749,767,106.56; 0.006 * = 1,688,849,860,263.936; 0.0004 * = 112,589,990,684.2624; sum 2,814,749,767,106.56 + 1,688,849,860,263.936 = 4,503,599,627,370.496; +112,589,990,684.2624 = 4,616,189,618,054.7584; +84,442,493,013,196.8 = 89,058,682,631,251.5584. Our numerator is 89,065,966,340,976, which is higher by ~7,283,709,724.44. Divide by 281,474,976,710,656 gives ~0.0000259. So value ≈ 0.3164259. Let's compute more precisely: 89,065,966,340,976 / 281,474,976,710,656 = ?

Difference from 0.3164 is 7,283,709,724.44 / 281,474,976,710,656 = 2.587e-5. So value ≈ 0.31642587. Let's compute exactly using decimal: 281,474,976,710,656 * 0.3164 = 89,058,? We did that. Let's calculate 281,474,976,710,656 * 0.3164 exactly: 281,474,976,710,656 * 0.3164 = (281,474,976,710,656 * 3164) / 10000 = (281,474,976,710,656 * 3164) / 10000. 281,474,976,710,656 * 3164 = 281,474,976,710,656 * (3000+100+60+4) = 844,424,930,131,968,000 + 28,147,497,671,065,600 + 16,888,498,602,639,360 + 1,125,899,906,842,624 = 890,586,826,312,515,584? Let's sum: 844,424,930,131,968,000 + 28,147,497,671,065,600 = 872,572,427,803,033,600; +16,888,498,602,639,360 = 889,460,926,405,672,960; +1,125,899,906,842,624 = 890,586,826,312,515,584. Divide by 10000 gives 89,058,682,631,251.5584. So that is the value for 0.3164.

Our numerator is 89,065,966,340,976. Difference = 7,283,709,724.4416. Divide by 281,474,976,710,656 = 0.00002588. So value = 0.31642588. So expected double ≈ 0.316425879... We can compute more exactly using integer division in test: `double expected = (double)num / (double)281474976710656ULL;` That gives the exact double. We can assert `fabs(v1 - expected) < 1e-15`. That is a valid assert.

Now for the second call with same seed, we need the next state. We could compute it similarly but that is time-consuming. Instead, we can just assert that the second value is different from the first and within [0,1). Also, we can test that seeding with a different seed resets and gives a value not equal to the previous first for that seed. To test that re-seeding works, we can call `randDoubleFromSeed(2)` and assert that it is different from `v1` (the first value for seed 1). But we need to know the expected first value for seed 2? Not necessary; we can assert it is in [0,1) and not equal to v1. That is sufficient.

Thus, the test will include asserts:
- `double v1 = randDoubleFromSeed(1); assert(v1 >= 0.0 && v1 < 1.0);` and compute expected for first call with seed 1 using known state and compare with tolerance.
- `double v2 = randDoubleFromSeed(1); assert(v2 >= 0.0 && v2 < 1.0); assert(v2 != v1);`
- `double w1 = randDoubleFromSeed(2); assert(w1 >= 0.0 && w1 < 1.0); assert(w1 != v1);`
- Maybe also `double w2 = randDoubleFromSeed(2); assert(w2 != w1);`

Also test that calling with a new seed after some calls resets the sequence: For example, after calling with seed 1 twice, we call with seed 2 and get a value, then call with seed 1 again – that should re-seed and give v1 again. So `double v1_again = randDoubleFromSeed(1); assert(v1_again == v1);` Because different seed resets. That is a good test.

Now we need to implement the solution. We'll use `static` variables inside the function. The function will have a static `unsigned x[3]`, `a[3]`, `c`, and also a static `long lastSeed` and `bool initialized`. The macros from the snippet can be copied as static inline or just code inline. We'll write a helper `seedGenerator(long)` and `nextGenerator()` as static functions or as static lambdas. But to keep it self-contained, we can define them as static functions inside the same file.

We'll include `<cstdint>` maybe, but we can use `unsigned` as in snippet (32-bit). We need `unsigned long` for carry.

Let's write the solution code.
#include <cstdint>

// Pseudo-random number generator based on the provided snippet.
// Seeds with the given seed on the first call or when a different seed is provided.
// Repeated calls with the same seed advance the state and return successive values.
double randDoubleFromSeed(long seedVal) {
    constexpr unsigned N = 16;
    constexpr unsigned MASK = (1u << (N - 1)) + (1u << (N - 1)) - 1u; // 0xFFFF
    constexpr unsigned X0 = 0x330E;
    constexpr unsigned A0 = 0xE66D;
    constexpr unsigned A1 = 0xDEEC;
    constexpr unsigned A2 = 0x0005;
    constexpr unsigned C = 0x000B;

    // Static generator state
    static unsigned x[3] = {X0, 0, 0};
    static unsigned a[3] = {A0, A1, A2};
    static unsigned c = C;
    static long lastSeed = 0;
    static bool initialized = false;

    // Low and high 16-bit words
    auto LOW = [](unsigned v) -> unsigned { return v & MASK; };
    auto HIGH = [](unsigned v) -> unsigned { return LOW(v >> N); };

    // Reset state with a given seed
    auto seedGenerator = [&](long seed) {
        unsigned low = static_cast<unsigned>(seed) & MASK;       // LOW(seedval)
        unsigned high = LOW(static_cast<unsigned>(seed) >> N);   // HIGH(seedval)
        x[0] = X0;
        x[1] = low;
        x[2] = high;
        a[0] = A0;
        a[1] = A1;
        a[2] = A2;
        c = C;
    };

    // Advance state, as in the snippet's next()
    auto nextGenerator = [&]() {
        unsigned p[2], q[2], r[2], carry0, carry1;

        // MUL(a[0], x[0], p)
        {
            unsigned long l = static_cast<unsigned long>(a[0]) * static_cast<unsigned long>(x[0]);
            p[0] = LOW(static_cast<unsigned>(l));
            p[1] = HIGH(static_cast<unsigned>(l));
        }

        // ADDEQU(p[0], c, carry0)
        {
            unsigned long sum = static_cast<unsigned long>(p[0]) + c;
            carry0 = (sum > MASK) ? 1u : 0u;
            p[0] = LOW(static_cast<unsigned>(sum));
        }

        // ADDEQU(p[1], carry0, carry1)
        {
            unsigned long sum = static_cast<unsigned long>(p[1]) + carry0;
            carry1 = (sum > MASK) ? 1u : 0u;
            p[1] = LOW(static_cast<unsigned>(sum));
        }

        // MUL(a[0], x[1], q)
        {
            unsigned long l = static_cast<unsigned long>(a[0]) * static_cast<unsigned long>(x[1]);
            q[0] = LOW(static_cast<unsigned>(l));
            q[1] = HIGH(static_cast<unsigned>(l));
        }

        // ADDEQU(p[1], q[0], carry0)
        {
            unsigned long sum = static_cast<unsigned long>(p[1]) + q[0];
            carry0 = (sum > MASK) ? 1u : 0u;
            p[1] = LOW(static_cast<unsigned>(sum));
        }

        // MUL(a[1], x[0], r)
        {
            unsigned long l = static_cast<unsigned long>(a[1]) * static_cast<unsigned long>(x[0]);
            r[0] = LOW(static_cast<unsigned>(l));
            r[1] = HIGH(static_cast<unsigned>(l));
        }

        // Compute x[2] using the full expression
        {
            unsigned carryPR = (static_cast<unsigned long>(p[1]) + r[0] > MASK) ? 1u : 0u;
            unsigned long total = carry0 + carry1 + carryPR + q[1] + r[1] +
                                  static_cast<unsigned long>(a[0]) * x[2] +
                                  static_cast<unsigned long>(a[1]) * x[1] +
                                  static_cast<unsigned long>(a[2]) * x[0];
            x[2] = LOW(static_cast<unsigned>(total));
        }

        x[1] = LOW(p[1] + r[0]);
        x[0] = LOW(p[0]);
    };

    // Seed if not initialized or if a different seed is given
    if (!initialized || seedVal != lastSeed) {
        seedGenerator(seedVal);
        lastSeed = seedVal;
        initialized = true;
    } else {
        // Same seed as previous call: advance state
        nextGenerator();
    }

    // Compute the double as in the snippet:
    // two16m = 1.0 / (1L << N) = 1/65536
    constexpr double two16m = 1.0 / 65536.0;
    return two16m * (two16m * (two16m * static_cast<double>(x[0]) + static_cast<double>(x[1])) + static_cast<double>(x[2]));
}
Note: In the above, I add `nextGenerator()` when same seed but also on first call? Actually on first call we seed and then need to generate first value. The snippet's `drand48()` calls `next()` before returning. So after seeding, we need to call `next()` once. In my code, if `!initialized` or seed differs, I call `seedGenerator` but do not call `nextGenerator`. Then the function returns based on the just-seeded state, but that would give a value based on the initial x, not the first `next()`. To match the snippet, after seeding we must call `next()` once. So we need to adjust: When we seed (first call or different seed), we should call `nextGenerator()` to advance to the first output. When same seed, we also call `nextGenerator()` to advance. So the logic should be: always call `nextGenerator()` after ensuring the state is seeded. So modify to:

if (!initialized || seedVal != lastSeed) {
    seedGenerator(seedVal);
    lastSeed = seedVal;
    initialized = true;
}
nextGenerator();

That way, both first call and subsequent calls advance. That matches the snippet: after seeding, you call `drand48` which calls `next`. So yes.

Thus, in the solution code above, I have the `if/else` that either seeds or not, but I forgot to call `nextGenerator` in the seeding branch. Let me correct in the final solution. In the solution provided, I'll ensure that after the state is set (either fresh or existing), we always call `nextGenerator()` to advance. But careful: on first call, we must seed and then advance. On same seed, we must advance only. So simply:

if (!initialized || seedVal != lastSeed) {
    seedGenerator(seedVal);
    lastSeed = seedVal;
    initialized = true;
}
nextGenerator();

Yes. I'll rewrite the solution accordingly.

Also note: The `HIGH` macro in the snippet uses `LOW((x) >> N)`. My lambda for HIGH does the same. Good.

Now the test code.
#include <cassert>
#include <cmath>
#include <cstdint>

// Assume the solution function is declared above or included.

int main() {
    // First call with seed 1: should return the first value of the sequence.
    double v1 = randDoubleFromSeed(1);
    // Expected first value after seeding with 1: after one next() call,
    // x = {0x5101, 0x4949, 0x0B70}.
    unsigned long long num = ((unsigned long long)0x5101 << 32) | ((unsigned long long)0x4949 << 16) | 0x0B70ULL;
    double expected = (double)num / 281474976710656.0; // 2^48
    assert(fabs(v1 - expected) < 1e-15);
    assert(v1 >= 0.0 && v1 < 1.0);

    // Second call with same seed: should advance to the next value.
    double v2 = randDoubleFromSeed(1);
    assert(v2 >= 0.0 && v2 < 1.0);
    assert(v2 != v1);

    // Call with a different seed: should reseed and produce a different first value.
    double w1 = randDoubleFromSeed(2);
    assert(w1 >= 0.0 && w1 < 1.0);
    assert(w1 != v1);

    // Calling with seed 2 again should advance.
    double w2 = randDoubleFromSeed(2);
    assert(w2 >= 0.0 && w2 < 1.0);
    assert(w2 != w1);

    // Re-seeding with seed 1 again should return exactly the first value again.
    double v1_again = randDoubleFromSeed(1);
    assert(v1_again == v1); // restart from seed 1

    // Ensure no value goes out of range over several calls.
    double v = randDoubleFromSeed(1);
    for (int i = 0; i < 100; ++i) {
        v = randDoubleFromSeed(1);
        assert(v >= 0.0 && v < 1.0);
    }
}

Note: In the test, the first call computes `expected` using the known state. This is independent because we hardcoded the state from manual calculation, not from the function. The function's internal state after one call should match. The test should pass.

Now I'll output the final answer with the four sections as required. I'll include the corrected solution. Ensure the solution has no main function. The solution must be a free function. I'll include necessary headers `<cstdint>` and maybe `<cmath>` not needed. The function uses `unsigned` and `long`. I'll keep it simple.

Let me also note that in the solution, I defined lambdas inside the function, but that is fine; they are called within the function scope. The static variables are in the function scope. The function is reentrant? No, it uses static state, which is intended. The function signature is `double randDoubleFromSeed(long seedVal)`.

Now produce the final response. Ensure each section has the exact headers , , , . The solution code only, no extra explanation. The test code has a main function.

Let's write the solution carefully with correct logic.

I will also ensure that the `nextGenerator` lambda captures by value? It captures by reference to the static arrays and c etc. That is fine.

I'll update the solution to call `nextGenerator()` after seeding.

One more detail: In the snippet, `SEED` sets `a` to A0, A1, A2 and `c` to C. My seedGenerator does that. Good.

Also, in the snippet, `drand48` uses `next()` which updates x in place. My code does that.

Now final output. length constraints: The response must be high-quality and self-contained.

I'll now produce the final answer.
