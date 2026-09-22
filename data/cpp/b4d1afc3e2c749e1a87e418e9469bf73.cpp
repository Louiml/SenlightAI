Write a C++ function `serveQueue(int n, const std::string& commands)` that simulates a queue with three operation types: `'+'` followed by an integer (append to the back), `'*'` followed by an integer (insert at the front), and `'-'` (pop and return the front element, output it). The input is given as a single string where each command is separated by whitespace, and the first token is the number of operations `n` (though you may ignore it if the commands are parsed directly). The function must return a `std::vector<int>` containing the results of all `'-'` operations in order. The queue must always maintain the invariant that the front half of elements are in one deque and the back half in another, rebalancing after every operation so that the front deque has size equal to or one greater than the back deque, ensuring that popping the front is always O(1). Note that operations are performed sequentially, and the total number of commands is exactly `n` as given.

#include <cassert>
#include <vector>

int main() {
    // Simple operations
    std::vector<int> r1 = serveQueue(6, "6 + 1 + 2 + 3 - - -");
    assert((r1 == std::vector<int>{1, 2, 3}));

    // '*' inserts at front effectively
    std::vector<int> r2 = serveQueue(8, "8 + 1 * 2 + 3 - - - -");
    assert((r2 == std::vector<int>{2, 1, 3}));

    // Alternating operations with rebalancing
    std::vector<int> r3 = serveQueue(7, "7 + 5 * 3 + 7 - - + 9 - -");
    assert((r3 == std::vector<int>{3, 5, 7, 9}));

    // Empty queue? Not tested, but after adding many then removing all
    std::vector<int> r4 = serveQueue(4, "4 + 10 + 20 - -");
    assert((r4 == std::vector<int>{10, 20}));

    // Large sequence to stress rebalancing
    std::vector<int> r5 = serveQueue(9, "9 + 1 + 2 + 3 + 4 * 0 * 5 - - - - - - -");
    assert((r5 == std::vector<int>{0, 5, 1, 2, 3, 4}));

    // Only one operation
    std::vector<int> r6 = serveQueue(2, "2 + 42 -");
    assert((r6 == std::vector<int>{42}));

    // More complex interleaving
    std::vector<int> r7 = serveQueue(10, "10 + 1 * 2 - + 3 - * 4 - - + 5 -");
    assert((r7 == std::vector<int>{2, 1, 3, 4, 5}));

    return 0;
}

#include <deque>
#include <vector>
#include <sstream>
#include <string>

// Simulates a queue with push-back, push-front, and pop-front operations.
// Returns the results of all pop-front operations in order.
std::vector<int> serveQueue(int n, const std::string& commands) {
    std::deque<int> frontPart;  // front half (size >= backPart size)
    std::deque<int> backPart;   // back half
    std::vector<int> results;

    std::istringstream input(commands);
    int tokenCount;
    input >> tokenCount;  // ignore the given n; we use the actual count
    for (int i = 0; i < n; ++i) {
        char op;
        input >> op;
        if (op == '+') {
            int value;
            input >> value;
            backPart.push_back(value);
        } else if (op == '*') {
            int value;
            input >> value;
            backPart.push_front(value);
        } else { // op == '-'
            results.push_back(frontPart.front());
            frontPart.pop_front();
        }

        // Rebalance: frontPart should have size == backPart.size() or size == backPart.size()+1
        if (frontPart.size() < backPart.size()) {
            frontPart.push_back(backPart.front());
            backPart.pop_front();
        } else if (frontPart.size() > backPart.size() + 1) {
            backPart.push_front(frontPart.back());
            frontPart.pop_back();
        }
    }
    return results;
}

// The core idea is to use two deques: `front` for the first ceil(k/2) elements and `back` for the remaining floor(k/2) elements, where `k` is the current total size. When adding `'+'`, we push to the back deque. When adding `'*'`, we push to the front of the back deque (since it becomes the new element right after the front half). When removing `'-'`, we pop from the front of the front deque and output it. After every operation, we rebalance: if the front deque is smaller than the back deque, move the first element of the back deque to the back of the front deque; if the front deque is larger than the back deque by more than one, move the last element of the front deque to the front of the back deque. This keeps sizes balanced (front size = back size or front size = back size + 1). Edge cases: when the queue is empty, `'-'` should not be called (input guaranteed valid). The `'*'` operation when the queue is empty works fine since both deques are empty. Time complexity: each operation is O(1) amortized because each element is moved at most once per rebalance, and rebalancing moves at most one element per operation. Space complexity: O(n) for storing all elements in the two deques.
