Write a C++ function `bool isBalancedBrackets(const std::string& expression)` that, given a string containing only the characters `(`, `)`, `{`, `}`, `[`, and `]`, determines whether the brackets are properly balanced and nested. The function must return `true` if every opening bracket has a matching closing bracket of the same type in the correct order, and `false` otherwise. An empty string is considered balanced. You must implement your own stack data structure (do not use `std::stack`) using a singly linked list with smart pointers or raw pointers. The solution must be self-contained, efficient, and handle edge cases such as unmatched closing brackets, interleaved bracket types, and empty input.
The solution uses a custom stack of characters implemented as a singly linked list. Traverse the input string character by character: if the character is an opening bracket (`(`, `{`, or `[`), push it onto the stack. If it is a closing bracket, first check whether the stack is empty — if empty, return `false` immediately because there is no matching opener. Otherwise, pop the top element and verify that it matches the expected opening bracket for the current closing bracket. A mismatch in type (e.g., `(` closed by `]`) also returns `false`. After processing all characters, the stack must be empty for the expression to be balanced; any leftover openers mean unmatched brackets. Edge cases include empty input (valid), only closing brackets (invalid), and nested/adjacent valid pairs (e.g., `()[]{}`). Time complexity is `O(n)` for one pass over the string, and space complexity is `O(n)` in the worst case for the stack storing all opening brackets (e.g., `((((...))))`). The custom stack must correctly handle push, pop, empty, and top operations, with proper memory management (e.g., using `std::shared_ptr` nodes).
#include <memory>
#include <string>

// Node for the linked-list-based stack.
template <typename T>
struct StackNode {
    T data;
    std::shared_ptr<StackNode<T>> next;
    explicit StackNode(const T& value) : data(value), next(nullptr) {}
};

// Simple stack using a singly linked list with shared_ptr nodes.
template <typename T>
class LinkedListStack {
public:
    LinkedListStack() : top_(nullptr), count_(0) {}

    void push(const T& value) {
        auto newNode = std::make_shared<StackNode<T>>(value);
        newNode->next = top_;
        top_ = newNode;
        ++count_;
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("pop from empty stack");
        }
        T value = top_->data;
        top_ = top_->next;
        --count_;
        return value;
    }

    T& peek() const {
        if (empty()) {
            throw std::runtime_error("peek from empty stack");
        }
        return top_->data;
    }

    bool empty() const {
        return top_ == nullptr;
    }

    size_t size() const {
        return count_;
    }

private:
    std::shared_ptr<StackNode<T>> top_;
    size_t count_;
};

// Helper to check if two brackets match.
bool matches(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

// Returns true if all brackets in the expression are properly balanced.
bool isBalancedBrackets(const std::string& expression) {
    LinkedListStack<char> stack;
    for (char ch : expression) {
        if (ch == '(' || ch == '{' || ch == '[') {
            stack.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (stack.empty()) {
                return false; // Closing without any opener.
            }
            char open = stack.pop();
            if (!matches(open, ch)) {
                return false; // Wrong bracket type.
            }
        }
        // Ignore any non-bracket characters (though spec says only brackets).
    }
    return stack.empty(); // All openers must be matched.
}
#include <cassert>
#include <string>

// The solution function is declared above; this is the test harness.
int main() {
    assert(isBalancedBrackets("") == true);
    assert(isBalancedBrackets("()") == true);
    assert(isBalancedBrackets("()[]{}") == true);
    assert(isBalancedBrackets("({[]})") == true);
    assert(isBalancedBrackets("(]") == false);
    assert(isBalancedBrackets("([)]") == false);
    assert(isBalancedBrackets("((()))") == true);
    assert(isBalancedBrackets(")") == false);
    assert(isBalancedBrackets("(") == false);
    assert(isBalancedBrackets("{[()()][]}") == true);
    return 0;
}
