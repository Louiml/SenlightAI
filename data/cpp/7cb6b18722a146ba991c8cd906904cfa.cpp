/*
Write a C++ function `int simulatePL0Program(int initialC)` that simulates the execution of the given PL/0-style program, treating the program’s final `write(b)` output as the return value. The program reads one initial integer into `c` (the only input), then executes the nested procedures and loops exactly as described by the Pascal-like semantics: variables `b`, `c`, `d`, `f`, `g` are dynamically scoped across procedure calls, constants are fixed (`a=10`, `e=5`), and all arithmetic is integer (division not used). The simulation must faithfully reproduce the write order and final value of `b` after the last `call p` at the end of the main block. You may assume the input `c` is a positive integer, and the loop `while c>0` continues until `c` becomes non-positive after the final `read(c)`. The function should return the integer value that would be printed by the last `write(b)` statement in the main program body (i.e., the value of `b` after the final call to `p`).
*/

#include <cassert>

// Simulate the given PL/0 program with a single positive input c.
// The program loops once (subsequent read returns 0), and the last
// write(b) outputs (initialC - 10). Returns that value.
int lastWrittenB(int initialC) {
    assert(initialC > 0);
    // After one loop iteration: b = -10 + initialC
    return initialC - 10;
}

int main() {
    // For any positive input, the last write(b) prints initialC - 10.
    assert(lastWrittenB(1) == -9);
    assert(lastWrittenB(2) == -8);
    assert(lastWrittenB(5) == -5);
    assert(lastWrittenB(10) == 0);
    assert(lastWrittenB(100) == 90);
    assert(lastWrittenB(1234) == 1224);
}

// The PL/0 program has nested procedures with dynamic scoping: `main` sets `b=8`, reads `c`, then loops while `c>0`. In each loop iteration it writes `b`, calls `s` (which calls `p`, which updates `b` to `-10` after calling `q` and `r`; then writes `b`), then after returning from `s`, main updates `b=b+c` and writes it, then reads a new `c`. The loop ends when `c≤0`. After the loop, main calls `p` once more, which increments `d=a+b` (dynamic scoping: `d` is local to `p`), writes `d`, then calls `q`. Inside `q`, `f` is local, `r` computes `g=a+e` and writes it (always `15`), then sets `f=g`, writes `f`, then sets `f=a+e*f` (using old `f` = 15, so `f=10+5*15=85`), writes `f` and `d` (which from `p` is `10+b`). After `q` returns, `p` returns. Since `p` does not modify `b` itself, the final `b` after the last call to `p` is unchanged from the value it had after the loop ended. Thus the return value is the `b` value after the last `read(c)` and loop termination.
//
// We simulate manually. Start: `b=8`, `c=initialC`. While `c>0`:
// - Write `b` (not relevant to return).
// - Call `s`: calls `p`. In `p`, `d=a+b` (dynamic `b` from main). Writes `d` (not relevant). Then calls `q`. In `q`, `f` local (uninitialized but overwritten). `r` writes `g=15`, sets `f=15`, writes `f=15`, then sets `f=a+e*f=10+5*15=85`, writes `f` and `d` (which is `10+b`). Returns. `p` returns. Back in `s`, `b=-10`, writes `b` (not relevant). Returns to main.
// - Now `b=-10` (because `s` changed it). Then main does `b=b+c` (dynamic scope: `c` is main’s current `c`), so `b = -10 + c`. Writes `b`. Reads new `c`.
// Loop continues while new `c>0`.
//
// After loop ends (when `c≤0`), main calls `p` again. In this last `p`, `d=a+b` with current `b` (which is the value after the last `b=b+c` update before the loop exited, because after that update, `read(c)` set `c` to a non-positive value and loop ended). `p` does not modify `b`. So final `b` is that value.
//
// We need the return value as a function of initial `c`. Let’s simulate pattern. Let `c0` be initial. Loop iterations: each iteration, after calling `s`, `b` becomes `-10`, then `b = -10 + c_current`. Then read next `c`. So after each iteration, `b` updates to `c_current - 10`. The loop continues as long as the newly read `c` > 0. The final `b` after the last iteration (the one where after updating, the next `read` gives `c≤0`) is `c_last - 10`, where `c_last` is the `c` value used in that last iteration (the one that was read before that iteration). But careful: The loop condition is checked at the top. Suppose initial `c0=1`. Iteration 1: write b=8, call s → b=-10, write b=-10, then b=b+c0 = -10+1=-9, write -9, read c1. If c1>0, loop continues; else stop. Suppose we only have one iteration, then after reading c1≤0, loop ends, and final b = -9. Then call p, no change, return -9. For c0=2: iteration1: b=8 → s → b=-10 → b=-10+2=-8, read c1. If c1=0, stop, return -8. If c1>0, iteration2: write b=-8, call s → b=-10, then b=-10+c1, read c2, etc. So the process depends on the sequence of inputs. But the problem says "the only input is initialC" – but the program also has `read(c)` inside the loop, which reads additional numbers. The task statement says "reads one initial integer into c (the only input)" – that implies we assume the loop reads the same value? Or maybe we assume the program reads from a stream that provides only one number, and subsequent reads get EOF or 0? The snippet has `read(c)` inside loop, but if only one input is given, subsequent reads might get 0 (or undefined). For a deterministic solution, we must interpret: The task says "simulate the execution ... reading an initial integer into c (the only input)" – that suggests that after the first read, any subsequent `read(c)` inside the loop should read a value that terminates the loop (e.g., 0) because there is no more input. To make it well-defined, we’ll assume that subsequent reads return 0 (which is typical for EOF in PL/0). So the loop runs exactly once, because after the first iteration, the second `read(c)` reads 0, which is not >0, so loop exits.
//
// Thus for any positive initial `c`, the loop executes once:
// - Initial b=8, c=c0.
// - While c>0 (true):
//   - write b (ignored)
//   - call s: inside s, call p: p writes d=10+8=18, then q: writes g=15, f=15, then f=85, writes f=85, d=10+8=18, then returns. Then s sets b=-10, writes b. Back in main.
//   - b = b + c0 = -10 + c0.
//   - write b (ignored)
//   - read c → gets 0 (EOF), loop ends.
// - call p: inside p, d=10+b, writes d, calls q, which writes g=15, f=15, f=85, writes f and d, returns. b unchanged.
// - Program ends with final write(b) (though not shown in snippet, the main block ends after `call p`; actually the snippet ends with `begin ... call p; end.` and does not have a write at the end. Wait, looking at snippet: main block starts `begin b:=8; read(c); while ... do ...; call p; end.` There is no final `write(b)`. The last statement is `call p;`. So there is no write of `b` after the loop. The problem asks "return the integer value that would be printed by the last `write(b)` statement in the main program body" – but the main program has `write(b)` only inside the loop (the first statement of the loop body) and after `call s` there is `write(b)`. Actually the loop body: `begin write(b); call s; b:=b+c; write(b); read(c); end;` So there are two `write(b)` per iteration. The last `write(b)` executed is the second one in the loop (after b:=b+c), because after the loop, there is only `call p`, no write. So the last `write(b)` in the entire program is the one after `b:=b+c` in the only loop iteration (since loop runs once). That value is `-10 + c0`. So the return value should be that.
//
// Therefore the function simply returns `initialC - 10` for any positive `initialC`. But we must be careful: if initialC is 0 or negative, the loop never runs, then `call p` is executed, and there is no `write(b)` ever? The problem states "the last `write(b)` statement in the main program body" – if the loop runs zero times, no write happens, so undefined. The task says "You may assume the input `c` is a positive integer", so initialC > 0. So return `initialC - 10`.
//
// Thus the algorithm is O(1) time, O(1) space.
//
// Edge case: initialC=0 would cause no loop, but assumption says positive.
