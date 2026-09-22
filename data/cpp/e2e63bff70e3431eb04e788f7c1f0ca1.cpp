/*
Given a fixed-capacity stack implemented as in the provided snippet (using `kMaxStack = 30`, `count`, and a fixed array), write a C++ function named `removeMinimum` that takes a reference to a `Stack<int>` and removes **all** occurrences of the smallest value currently in the stack. The function should preserve the relative order of the remaining elements (i.e., elements that are not the minimum) in the stack after removal. For example, if the stack contains (bottom to top) `[5, 2, 3, 2, 1]`, the minimum is `1`, so the stack becomes `[5, 2, 3, 2]` (bottom to top). If the stack contains only copies of the minimum, the stack becomes empty. The function must not use any other container besides the provided `Stack` (you may use an auxiliary `Stack` for temporary storage). Assume the stack is non-empty when called. The function should modify the passed stack in-place and return nothing (void). Use the exact `Stack` class from the snippet, but you must write the function as a free function (not a member) that works with that class. Note: the `Stack` class has `push`, `pop`, `top`, and `empty` methods; `pop` removes and returns the top element. You may also use `top` to peek.
*/

#include <cstdio>
#include <cstdlib>

const int kMaxStack = 30;

template <class T>
class Stack {
  int count;
  T entrys[kMaxStack];
public:
  Stack() : count(0) {}
  bool empty() const { return count <= 0; }
  T pop() {
    if (count == 0) { printf("Underflow\n"); exit(0); }
    count--;
    return entrys[count];
  }
  T top() const {
    if (count == 0) { printf("Underflow\n"); exit(0); }
    return entrys[count-1];
  }
  void push(T entry) {
    if (count >= kMaxStack) { printf("Overflow\n"); exit(0); }
    entrys[count] = entry;
    count++;
  }
};

// Remove all occurrences of the smallest value from the stack,
// preserving the relative order of the remaining elements.
void removeMinimum(Stack<int>& s) {
    if (s.empty()) return; // safety, though spec says non-empty

    Stack<int> aux1, aux2;

    // Pass 1: pop all elements, find the minimum
    int minVal = s.top();
    while (!s.empty()) {
        int v = s.pop();
        if (v < minVal) minVal = v;
        aux1.push(v);
    }

    // Pass 2: transfer non-minimum elements to aux2 (preserves original order)
    while (!aux1.empty()) {
        int v = aux1.pop();
        if (v != minVal) {
            aux2.push(v);
        }
    }

    // Pass 3: transfer back to aux1 (reverses), then to s (reverses again -> original order)
    while (!aux2.empty()) {
        aux1.push(aux2.pop());
    }
    while (!aux1.empty()) {
        s.push(aux1.pop());
    }
}

#include <cassert>

int main() {
    // Test 1: typical case
    Stack<int> s1;
    s1.push(5); s1.push(2); s1.push(3); s1.push(2); s1.push(1);
    removeMinimum(s1);
    // Expected bottom-to-top: 5,2,3,2
    assert(s1.pop() == 2); // top
    assert(s1.pop() == 3);
    assert(s1.pop() == 2);
    assert(s1.pop() == 5);
    assert(s1.empty());

    // Test 2: all elements are the minimum
    Stack<int> s2;
    s2.push(4); s2.push(4); s2.push(4);
    removeMinimum(s2);
    assert(s2.empty());

    // Test 3: single element
    Stack<int> s3;
    s3.push(7);
    removeMinimum(s3);
    assert(s3.empty());

    // Test 4: minimum appears multiple times in middle
    Stack<int> s4;
    s4.push(10); s4.push(1); s4.push(8); s4.push(1); s4.push(20);
    removeMinimum(s4);
    // Expected bottom-to-top: 10,8,20
    assert(s4.pop() == 20);
    assert(s4.pop() == 8);
    assert(s4.pop() == 10);
    assert(s4.empty());

    // Test 5: minimum at bottom
    Stack<int> s5;
    s5.push(0); s5.push(5); s5.push(6);
    removeMinimum(s5);
    // Expected bottom-to-top: 5,6
    assert(s5.pop() == 6);
    assert(s5.pop() == 5);
    assert(s5.empty());

    // Test 6: negative numbers and duplicates
    Stack<int> s6;
    s6.push(-3); s6.push(-1); s6.push(-3); s6.push(2);
    removeMinimum(s6);
    // Expected bottom-to-top: -1,2
    assert(s6.pop() == 2);
    assert(s6.pop() == -1);
    assert(s6.empty());

    return 0;
}

// The main idea is to first scan the stack to find the minimum value. Since the stack only allows access from the top, we need to temporarily move all elements out to an auxiliary stack while tracking the minimum. Then we push elements back in reverse order, skipping any that equal the minimum. To preserve the original order (bottom-to-top) after removal, we must be careful: popping from the original stack yields elements in reverse order (top-to-bottom). So we first pop all elements into `auxStack`, which after the loop will contain the original elements in reverse order (because we pop from top and push to aux, so aux's top is the original bottom). While popping, we track the minimum. Then we pop from `auxStack` (which gives elements in original bottom-to-top order when we pop from aux's top) and push non-minimum elements back into the original stack. However, pushing them back one by one will reverse their order again. To fix this, we can instead pop from aux into a second temporary buffer? But we are limited to using only `Stack` containers. The cleanest is: after finding the minimum, we pop all from original into `auxStack` (now aux has original reversed). Then we pop from `auxStack` and push non-minimum elements into another stack `tempStack` (so tempStack will have them in original order but then pushed onto tempStack makes them reversed again). Actually better: we can first pop all from original into aux (aux = reversed). Then we pop all from aux into another stack `resultStack` but skip minimums. When we pop from aux (which has original reversed), we get original bottom first, then push into resultStack, so resultStack will have them in original order? Let's simulate: original bottom-to-top: [a,b,c] (a bottom). Pop original: c (top) popped, push to aux -> aux: [c]; pop b -> aux: [c,b]; pop a -> aux: [c,b,a]. Now aux top is a (original bottom). Now pop aux: a, push to resultStack -> resultStack: [a]; pop b -> [a,b]; pop c -> [a,b,c]. But resultStack's top is c, so bottom-to-top is [a,b,c], which is correct. However, if we skip some elements, the order of the remaining is preserved. Then we need to move the contents of resultStack back to the original stack, but that would reverse again. To avoid that, we can instead pop aux and push non-minimum directly into the original stack? That would reverse order. So the correct approach: Use two auxiliary stacks. First, pop original into `aux1` (reversed) while tracking min. Then pop all from `aux1` into `aux2` but skip min. Now `aux2` has the remaining elements in original order (bottom-to-top as we verified). But we need to put them back into the original stack in that same order. If we pop from `aux2` and push into original, that reverses again. So we need to transfer back by popping `aux2` into another stack `aux3` and then from `aux3` into original. But that's three auxiliaries. Alternatively, we can do two passes with a single auxiliary: First pass: pop all from original into aux, track min. Second pass: pop all from aux back into original but skip min? That would reverse order again. Actually, the standard trick: Since we know the minimum, we can do a single pass: pop from original, push non-min into a temporary stack, then after original is empty, pop temporary and push back into original. But that reverses order. To preserve order, we need to reverse twice. So: Step 1: Pop all from original into `temp` (original becomes empty). While popping, find min. Step 2: Now pop all from `temp` into `original` but skip elements equal to min. This results in `original` containing the remaining elements, but in reversed order compared to original? Let's check: original bottom-to-top [5,2,3,2,1]. Pop all into temp: temp top is 5? Actually pop 1 first -> temp [1]; pop 2 -> [1,2]; pop 3 -> [1,2,3]; pop 2 -> [1,2,3,2]; pop 5 -> [1,2,3,2,5]. So temp has original reversed (top is 5). Now to put back preserving order, we need to pop temp (which gives 5,2,3,2,1) and push into original, but that would make original bottom-to-top [1,2,3,2,5] (reversed). To fix, we need to do an extra reversal. So the correct algorithm uses two temporary stacks: 
// 1. Pop all from original into `aux1`, tracking min.
// 2. Pop all from `aux1` into `aux2` but skip the min. Now `aux2` holds the remaining elements in original order (since aux1 was reversed, popping aux1 gives original order, pushing to aux2 reverses that again? Wait: aux1 has original reversed. Pop aux1 (which is original bottom first) push to aux2 -> aux2 will have bottom-to-top in reverse of that? Let's do: original [a,b,c] (a bottom). aux1 after step1: [c,b,a] (top a). Step2: pop a (original bottom) -> push to aux2 -> aux2 [a]; pop b -> [a,b]; pop c -> [a,b,c]. So aux2 top is c, bottom a, so it's same as original order. Now we need to transfer aux2 to original. If we pop aux2 and push to original, we get original with top being a (bottom-to-top [c,b,a]) which is reversed. So we need to reverse again. Thus use a third stack `aux3`: pop aux2 into aux3 (reverses), then pop aux3 into original (reverses back). So total two auxiliary stacks are enough because we can reuse the original after it's empty? Actually after step1 original is empty. We can use original as the final destination after reversing. So: 
// - Use `aux1` and `aux2`.
// 1. Pop all from original to `aux1`, find min.
// 2. Pop from `aux1`; if value != min, push to `aux2`.
// 3. Now `aux2` has remaining elements in original order. Pop from `aux2` into `aux1` (now aux1 has reversed order), then pop from `aux1` into original (now original has original order). 
// But actually step2 and step3 can be combined: after step1, we have aux1 reversed. Then pop aux1, skip min, push to aux2. Then pop aux2, push back to aux1, then pop aux1, push to original. That’s fine. Time complexity O(n) where n is the number of elements (we do three passes). Space complexity O(n) auxiliary stack space (two stacks each up to n). Edge cases: all elements are the minimum, then the stack becomes empty. The function assumes non-empty input, so we don't need to check for empty. Also note the `Stack` class uses `exit` on underflow/overflow, but we won't trigger those. Also `kMaxStack` is 30, but we can ignore that in analysis.
