Implement a C++ function `bool circularQueueSimulation(int k, const std::vector<int>& operations, std::vector<int>& outputs)` that simulates a circular queue of fixed capacity `k` using a **doubly linked list** (not an array). The `operations` vector encodes commands as integers: `1` = enQueue (with a following value in the next element), `2` = deQueue, `3` = Front, `4` = Rear, `5` = isEmpty, `6` = isFull. For each operation, append the result to `outputs` as follows: enQueue and deQueue produce `1` (true) or `0` (false), Front and Rear produce the integer value or `-1` if empty, and isEmpty/isFull produce `1` or `0`. The function must maintain FIFO order, wrap around logically (though using a linked list, no physical wrapping exists), and return `true` if all operations are valid (i.e., no enQueue on full queue, no deQueue on empty queue — such operations should be considered invalid and the function should return `false` and stop, but you must still append the appropriate result to `outputs` for that operation before returning). Use a custom `DoubleListNode` structure with `prev` and `next` pointers, and a sentinel head and tail node for simplicity. The function should be standalone, with no `main` function inside the solution code.
// The core idea is to implement a circular queue using a doubly linked list with sentinel nodes (dummy head and tail) to simplify insertion and deletion at the ends. Maintain three private variables: `size` (capacity), `len` (current number of elements), and pointers `head` and `tail` to sentinel nodes. Initially, `head->next = tail` and `tail->prev = head`, forming an empty list. For `enQueue`, check if `isFull()` (i.e., `len == size`). If full, operation is invalid and we append `0` to `outputs` and return `false` from the simulation. Otherwise, create a new node, insert it before the sentinel tail (standard doubly linked list insertion), increment `len`, append `1`. For `deQueue`, check if `isEmpty()` (`len == 0`). If empty, append `0` and return `false`. Otherwise, remove the node after the sentinel head, decrement `len`, append `1`. `Front` returns `head->next->val` if not empty, else `-1`. `Rear` returns `tail->prev->val` if not empty, else `-1`. `isEmpty` and `isFull` just return `len == 0` and `len == size` respectively. Edge cases: capacity zero (all enQueue operations are invalid immediately), operations that attempt to enQueue when full or deQueue when empty must be flagged. After processing all operations, return `true` if no invalid operation occurred. Time complexity is O(1) per operation (each node insertion/removal is constant time), space complexity O(k) for the linked list nodes, plus O(m) for the output vector where m is the number of operations.
#include <vector>

struct DoubleListNode {
    int val;
    DoubleListNode* prev;
    DoubleListNode* next;
    DoubleListNode(int x) : val(x), prev(nullptr), next(nullptr) {}
};

bool circularQueueSimulation(int k, const std::vector<int>& operations, std::vector<int>& outputs) {
    outputs.clear();
    int len = 0;
    DoubleListNode* head = new DoubleListNode(-1);
    DoubleListNode* tail = new DoubleListNode(-1);
    head->next = tail;
    tail->prev = head;
    bool valid = true;

    auto isFull = [&]() { return len == k; };
    auto isEmpty = [&]() { return len == 0; };

    for (size_t i = 0; i < operations.size(); i += 1) {
        int op = operations[i];
        if (op == 1) {
            // enQueue, need a value at operations[i+1]
            if (i + 1 >= operations.size()) {
                valid = false;
                outputs.push_back(0);
                break;
            }
            int value = operations[i + 1];
            i += 1;
            if (isFull()) {
                valid = false;
                outputs.push_back(0);
                break;
            }
            DoubleListNode* node = new DoubleListNode(value);
            node->next = tail;
            node->prev = tail->prev;
            tail->prev->next = node;
            tail->prev = node;
            len++;
            outputs.push_back(1);
        } else if (op == 2) {
            if (isEmpty()) {
                valid = false;
                outputs.push_back(0);
                break;
            }
            DoubleListNode* node = head->next;
            head->next = node->next;
            node->next->prev = head;
            delete node;
            len--;
            outputs.push_back(1);
        } else if (op == 3) {
            outputs.push_back(isEmpty() ? -1 : head->next->val);
        } else if (op == 4) {
            outputs.push_back(isEmpty() ? -1 : tail->prev->val);
        } else if (op == 5) {
            outputs.push_back(isEmpty() ? 1 : 0);
        } else if (op == 6) {
            outputs.push_back(isFull() ? 1 : 0);
        } else {
            valid = false;
            break;
        }
    }

    // Clean up remaining nodes
    DoubleListNode* curr = head->next;
    while (curr != tail) {
        DoubleListNode* next = curr->next;
        delete curr;
        curr = next;
    }
    delete head;
    delete tail;

    return valid;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> outputs;

    // Example from LeetCode: k=3, enqueue 1,2,3, enqueue 4 (fail), Rear=3, isFull, deQueue, enQueue 4, Rear=4
    std::vector<int> ops1 = {1,1, 1,2, 1,3, 1,4, 4, 6, 2, 1,4, 4};
    assert(circularQueueSimulation(3, ops1, outputs) == false); // enQueue(4) fails, returns false
    std::vector<int> expected1 = {1,1,1,0,3,1,1,1,4};
    assert(outputs == expected1);

    // Empty queue: Front and Rear return -1, deQueue fails, isEmpty true
    std::vector<int> ops2 = {3, 4, 2, 5};
    assert(circularQueueSimulation(2, ops2, outputs) == false); // deQueue on empty fails
    std::vector<int> expected2 = {-1,-1,0,1};
    assert(outputs == expected2);

    // Successful full cycle with capacity 1
    std::vector<int> ops3 = {1,10, 6, 2, 1,20, 6};
    assert(circularQueueSimulation(1, ops3, outputs) == true);
    std::vector<int> expected3 = {1,1,1,1,1};
    assert(outputs == expected3);

    // Capacity zero: any enQueue fails, isFull always true
    std::vector<int> ops4 = {1,5, 6, 5, 2};
    assert(circularQueueSimulation(0, ops4, outputs) == false);
    std::vector<int> expected4 = {0,1,1,0}; // enQueue fail, isFull true, isEmpty true (len=0), deQueue fail
    assert(outputs == expected4);

    // Mixed operations with valid sequence
    std::vector<int> ops5 = {1,1, 1,2, 3, 4, 2, 3, 4, 5, 6};
    assert(circularQueueSimulation(3, ops5, outputs) == true);
    std::vector<int> expected5 = {1,1,1,2,1,2,-1,1,0}; // front=1, rear=2, dequeue, front=2, rear=2, not empty, not full
    assert(outputs == expected5);

    return 0;
}
