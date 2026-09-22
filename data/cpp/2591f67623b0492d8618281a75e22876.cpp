// Write a C++ function `bool isBalanced(const std::string& expression)` that takes a string containing only characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'`, and returns `true` if the brackets are properly nested and balanced, and `false` otherwise. The function must use a stack implemented manually via a singly linked list (as inspired by the provided code), not the STL `std::stack`. The input may be empty (which is considered balanced). Handle edge cases like closing brackets without matching openers, mismatched bracket types, and leftover open brackets at the end.

#include <cassert>

int main() {
    assert(isBalanced("") == true);
    assert(isBalanced("()") == true);
    assert(isBalanced("[]") == true);
    assert(isBalanced("{}") == true);
    assert(isBalanced("({[]})") == true);
    assert(isBalanced("()[]{}") == true);
    assert(isBalanced("([{}])") == true);

    assert(isBalanced("(") == false);
    assert(isBalanced(")") == false);
    assert(isBalanced("(]") == false);
    assert(isBalanced("([)]") == false);
    assert(isBalanced("{()}") == false);
    assert(isBalanced("((()))") == true);
    assert(isBalanced("((())") == false);
    assert(isBalanced(")(") == false);
    assert(isBalanced("{[]}(") == false);
    assert(isBalanced("}") == false);
}

#include <string>

// Node for linked-list-based stack
struct CharNode {
    char data;
    CharNode* next;
};

// Simple stack using linked list
struct CharStack {
    CharNode* top = nullptr;
    int size = 0;

    bool isEmpty() {
        return top == nullptr;
    }

    void push(char c) {
        CharNode* newNode = new CharNode{c, top};
        top = newNode;
        size++;
    }

    char pop() {
        if (isEmpty()) {
            // Return a dummy char; caller should check empty first
            return '\0';
        }
        CharNode* old = top;
        char value = old->data;
        top = old->next;
        delete old;
        size--;
        return value;
    }

    // Destructor to free remaining nodes (optional but good practice)
    ~CharStack() {
        while (top != nullptr) {
            CharNode* old = top;
            top = top->next;
            delete old;
        }
    }
};

// Check if brackets are properly balanced
bool isBalanced(const std::string& expression) {
    CharStack stack;

    for (char c : expression) {
        if (c == '(' || c == '{' || c == '[') {
            stack.push(c);
        } else {
            if (stack.isEmpty()) {
                return false; // closing bracket without opener
            }
            char open = stack.pop();
            // Check matching pairs
            if ((c == ')' && open != '(') ||
                (c == '}' && open != '{') ||
                (c == ']' && open != '[')) {
                return false; // mismatched bracket type
            }
        }
    }

    return stack.isEmpty(); // true if all openers matched
}

// The solution uses a manual stack of characters built from linked list nodes. For each character in the input: if it is an opening bracket (`'('`, `'{'`, `'['`), push it onto the stack. If it is a closing bracket, first check if the stack is empty — if so, return `false` immediately (no matching opener). Otherwise, pop the top character and verify it matches the corresponding opening bracket for the current closing bracket; if not, return `false`. After processing all characters, return `true` if the stack is empty (all openers matched); otherwise `false`. Edge cases include empty string (balanced), a closing bracket with empty stack, and mismatched pair types. Time complexity is O(n) where n is input length, and space complexity is O(n) in the worst case for the linked list stack.
