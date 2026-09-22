// Write a C++ function named `swapStacks` that takes two `std::stack<int>` objects by reference and swaps their contents. The function should not return anything. After the call, the first stack should contain all elements that were originally in the second stack (and vice versa), preserving their internal order (i.e., the top of each stack after swap corresponds to the old top of the other stack). The function must work correctly for stacks of any size, including empty stacks. You may not use the built-in `swap` member or global `std::swap` for stacks; you must implement the swap manually (e.g., using a temporary third stack). Use only standard library headers `<stack>` and necessary others.
// The task requires manually swapping the contents of two stacks without using `std::swap` or `stack::swap`. The straightforward approach is to create a temporary stack. Move all elements from the first stack to the temporary stack (by popping from `a` and pushing onto `temp`), then move all elements from the second stack to the first stack (pop from `b`, push to `a`), and finally move all elements from the temporary stack back to the second stack (pop from `temp`, push to `b`). This preserves the relative order because when you pop from a stack and push to another, the order reverses; but moving from `a` to `temp` reverses `a`, then moving from `temp` to `b` reverses again, effectively restoring original order of `a` into `b`. Similarly for `b` moving to `a` directly preserves order (since we pop from `b` and push to `a`, but we process all of `b` before touching `a` again). However, careful: after we move `a` to temp (a becomes empty), we move `b` into `a` (b becomes empty, a gets b's elements reversed? Actually let's trace). Suppose a = [1,2,3] (top 3). b = [4,5] (top 5). Step1: move a to temp: temp = [1,2,3]? Let's simulate: pop top 3 push temp => temp=[3]; pop 2 => temp=[3,2]; pop 1 => temp=[3,2,1]. a is empty. Step2: move b to a: pop 5 push a => a=[5]; pop 4 => a=[5,4]; b empty. Step3: move temp to b: pop 1 push b => b=[1]; pop 2 => b=[1,2]; pop 3 => b=[1,2,3]. Now a has [5,4] (top 4) which is original b in reversed order? Original b top was 5, now a top is 4 – that's reversed! That's not correct. So the order is reversed. To preserve order, we need to move to a temporary then to another? Actually a correct manual swap preserving order can be done using three stacks: temp1, temp2. Move a to temp1 (reverses), move b to temp2 (reverses), then move temp1 to b (reverses back to original a order), move temp2 to a (reverses back to original b order). That works. But that uses O(n+m) extra space. Alternatively, we can use a single temporary stack but with a clever sequence: move a to temp, then move b to a, then move temp to b? That gave wrong order. Let's test: we want after swap, a should contain original b elements in the same order as they were in b (top = old top). Using single temp: move a to temp (reverses a), move b to a (this pushes b's top first, then next... so a gets b's elements reversed), then move temp to b (temp is reversed a, so moving to b gives original a order? Actually temp has reversed a: [3,2,1] (top 1). Pop 1 push b => b=[1]; pop2 => b=[1,2]; pop3 => b=[1,2,3] – that gives original a order in b, correct. But a has b's reversed order: a = [5,4] but original b was [4,5] top 5; we have a top 4, not 5. So that's wrong. Thus single temp doesn't work for preserving order for both. So we need two temps. That's acceptable. Complexity: time O(n+m) where n and m are sizes, space O(n+m) for the two temps. Edge cases: empty stacks – loops simply do nothing, works fine. Also handle different sizes. Use `std::stack<int>` and references.
#include <stack>

// Swap the contents of two stacks 'a' and 'b' manually, preserving order.
// Uses two temporary stacks to correctly reverse and restore the order.
void swapStacks(std::stack<int>& a, std::stack<int>& b) {
    std::stack<int> temp1, temp2;

    // Move all elements from a to temp1 (reverses order)
    while (!a.empty()) {
        temp1.push(a.top());
        a.pop();
    }

    // Move all elements from b to temp2 (reverses order)
    while (!b.empty()) {
        temp2.push(b.top());
        b.pop();
    }

    // Move temp1's elements to b (reverses again, restoring original a order)
    while (!temp1.empty()) {
        b.push(temp1.top());
        temp1.pop();
    }

    // Move temp2's elements to a (reverses again, restoring original b order)
    while (!temp2.empty()) {
        a.push(temp2.top());
        temp2.pop();
    }
}
#include <cassert>
#include <stack>

// Declaration of the function under test
void swapStacks(std::stack<int>& a, std::stack<int>& b);

int main() {
    // Test 1: Both non-empty, different sizes
    std::stack<int> s1, s2;
    s1.push(10); s1.push(20); s1.push(30); // top 30
    s2.push(101); s2.push(202);            // top 202

    swapStacks(s1, s2);
    // s1 should now contain {202, 101} with top 202
    assert(s1.size() == 2);
    assert(s1.top() == 202); s1.pop();
    assert(s1.top() == 101); s1.pop();
    // s2 should now contain {30, 20, 10} with top 30
    assert(s2.size() == 3);
    assert(s2.top() == 30); s2.pop();
    assert(s2.top() == 20); s2.pop();
    assert(s2.top() == 10); s2.pop();

    // Test 2: One empty, one non-empty
    std::stack<int> empty, has;
    has.push(1); has.push(2);
    swapStacks(empty, has);
    assert(empty.size() == 2);
    assert(empty.top() == 2); empty.pop();
    assert(empty.top() == 1); empty.pop();
    assert(has.empty());

    // Test 3: Both empty
    std::stack<int> e1, e2;
    swapStacks(e1, e2);
    assert(e1.empty() && e2.empty());

    // Test 4: Equal sizes
    std::stack<int> a,b;
    a.push(1); a.push(2);
    b.push(3); b.push(4);
    swapStacks(a,b);
    assert(a.top() == 4); a.pop(); assert(a.top() == 3); a.pop();
    assert(b.top() == 2); b.pop(); assert(b.top() == 1); b.pop();

    // Test 5: Single element each
    std::stack<int> x,y;
    x.push(7); y.push(8);
    swapStacks(x,y);
    assert(x.top() == 8); assert(y.top() == 7);

    return 0;
}
