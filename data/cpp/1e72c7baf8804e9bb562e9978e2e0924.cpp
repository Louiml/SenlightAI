Write a C++ function named `incrementAndGetReference` that takes a single integer parameter `value`, creates a local integer variable initialized to `value`, and then returns a reference to that local variable. However, because returning a reference to a local variable is dangerous (the local variable is destroyed when the function returns), your function must instead detect this issue by returning a reference to a static integer that is first set to `value + 1`. The function should then be used to demonstrate the difference between passing by value and passing by reference in the main program. Specifically, write a function `modifyByValue` that takes an integer by value and increments it, and a function `modifyByReference` that takes an integer by reference and increments it. The task is to implement these three functions and, in the test harness, verify that `modifyByValue` does not change the original variable, `modifyByReference` does change it, and that calling `incrementAndGetReference` returns a reference that reflects the last value set (since static storage persists).

// The task focuses on understanding C++ reference semantics and the lifetime of variables. For `incrementAndGetReference`, using a `static int` local variable ensures its storage duration is the entire program run, so the returned reference remains valid. The function should assign `value + 1` to this static variable and then return the static variable by reference. For `modifyByValue`, a copy of the argument is made, so the original caller's variable is unchanged. For `modifyByReference`, a reference alias is created, so increments affect the original variable. Edge cases: the input integer can be any int, including negative and zero; the static variable's initial value doesn't matter because it is overwritten each call. Time complexity is O(1) for all functions; space complexity is O(1) auxiliary. The test should confirm that the reference returned by `incrementAndGetReference` points to the same static memory across calls, so the value changes appropriately.

#include <cassert>
#include <iostream>

// Returns a reference to a static integer that is set to value + 1.
// Safe because static storage lasts for the program's lifetime.
int& incrementAndGetReference(int value) {
    static int stored = 0;
    stored = value + 1;
    return stored;
}

// Takes parameter by value; original argument is not modified.
void modifyByValue(int n) {
    n++;
}

// Takes parameter by reference; original argument is modified.
void modifyByReference(int& n) {
    n++;
}

int main() {
    // Test modifyByValue does not change original
    int a = 5;
    modifyByValue(a);
    assert(a == 5);

    // Test modifyByReference changes original
    int b = 10;
    modifyByReference(b);
    assert(b == 11);

    // Test incrementAndGetReference returns reference to static variable
    int& ref1 = incrementAndGetReference(4);
    assert(ref1 == 5);
    
    int& ref2 = incrementAndGetReference(9);
    assert(ref2 == 10);
    // Both ref1 and ref2 refer to the same static storage
    assert(&ref1 == &ref2);
    
    // Verify the static variable persists and is overwritten
    assert(ref1 == 10); // same as ref2 because they share memory
    
    // Edge cases: negative and zero
    int& ref3 = incrementAndGetReference(-3);
    assert(ref3 == -2);
    int& ref4 = incrementAndGetReference(0);
    assert(ref4 == 1);
    
    return 0;
}
