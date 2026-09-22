// Implement a self-contained C++ function named `dequeOperations` that simulates the behavior of a double-ended queue (deque) with a fixed capacity of 6 elements using only integer arrays and index variables. The function must accept a string command sequence (commands separated by spaces) and return a string of outputs for each command, separated by spaces. Supported commands are: `pushFront <value>`, `pushRear <value>`, `popFront`, `popRear`, `getFront`, `getRear`, `isEmpty`, and `isFull`. The deque initially starts empty, and its capacity is always 6. The function should handle edge cases correctly: pushing to a full deque returns `"false"` (printed as `"false"`), popping from an empty deque returns `-1`, and when popping, the removed element should be returned. For `isEmpty` and `isFull`, return `"true"` or `"false"` as strings. The output string contains the result of each command in order, separated by a single space. The input string is guaranteed to be well-formed (no extra spaces, valid command names and integer values).

#include <cassert>
#include <string>

std::string dequeOperations(const std::string& commands); // forward declaration

int main() {
    // Basic sequence from the original snippet (capacity 6)
    assert(dequeOperations("pushFront 10 pushFront 20 pushFront 30 pushRear 40 pushRear 60 pushRear 50 popFront getFront getRear isEmpty isFull popRear") ==
           "true true true true true true 30 20 50 false true 50");

    // Push to full deque
    assert(dequeOperations("pushFront 1 pushRear 2 pushFront 3 pushRear 4 pushFront 5 pushRear 6 pushFront 7") ==
           "true true true true true true false");

    // Pop from empty deque
    assert(dequeOperations("popFront popRear getFront getRear isEmpty isFull") ==
           "-1 -1 -1 -1 true false");

    // Wraparound behavior: push to fill, pop front, push rear wraps
    assert(dequeOperations("pushRear 1 pushRear 2 pushRear 3 pushRear 4 pushRear 5 pushRear 6 popFront pushRear 7 getRear getFront") ==
           "true true true true true true 1 true 7 2");

    // Single element then pop both sides
    assert(dequeOperations("pushFront 42 popFront popFront isEmpty") ==
           "true 42 -1 true");

    // Mixed operations and isFull after partial pop
    assert(dequeOperations("pushRear 5 pushRear 10 popFront pushRear 15 popRear getFront getRear isFull") ==
           "true true 5 10 10 15 false");

    // Test exact capacity boundary
    assert(dequeOperations("pushFront 1 pushRear 2 pushFront 3 pushRear 4 pushFront 5 pushRear 6 isFull popRear popFront isFull") ==
           "true true true true true true true 6 5 true");

    return 0;
}

#include <string>
#include <sstream>
#include <vector>
#include <cstddef>

// Simulate a fixed-capacity deque (capacity = 6) and return the outputs of all commands.
std::string dequeOperations(const std::string& commands) {
    const int CAPACITY = 6;
    int arr[CAPACITY] = {0};
    int front = -1, rear = -1;

    auto isEmpty = [&]() { return front == -1; };
    auto isFull = [&]() { return ((rear + 1) % CAPACITY) == front; };

    std::istringstream stream(commands);
    std::string token;
    std::vector<std::string> outputs;

    while (stream >> token) {
        if (token == "pushFront" || token == "pushRear") {
            int value;
            stream >> value;
            if (isFull()) {
                outputs.push_back("false");
            } else {
                if (isEmpty()) {
                    front = rear = 0;
                } else if (token == "pushFront") {
                    front = (front == 0) ? CAPACITY - 1 : front - 1;
                } else { // pushRear
                    rear = (rear == CAPACITY - 1) ? 0 : rear + 1;
                }
                arr[front] = value; // for pushFront, front is new; for pushRear, we need to set at rear
                if (token == "pushRear") {
                    arr[rear] = value;
                }
                outputs.push_back("true");
            }
        } else if (token == "popFront" || token == "popRear") {
            if (isEmpty()) {
                outputs.push_back("-1");
            } else {
                int ans;
                if (token == "popFront") {
                    ans = arr[front];
                    if (front == rear) {
                        front = rear = -1;
                    } else {
                        front = (front == CAPACITY - 1) ? 0 : front + 1;
                    }
                } else { // popRear
                    ans = arr[rear];
                    if (front == rear) {
                        front = rear = -1;
                    } else {
                        rear = (rear == 0) ? CAPACITY - 1 : rear - 1;
                    }
                }
                outputs.push_back(std::to_string(ans));
            }
        } else if (token == "getFront" || token == "getRear") {
            if (isEmpty()) {
                outputs.push_back("-1");
            } else if (token == "getFront") {
                outputs.push_back(std::to_string(arr[front]));
            } else {
                outputs.push_back(std::to_string(arr[rear]));
            }
        } else if (token == "isEmpty") {
            outputs.push_back(isEmpty() ? "true" : "false");
        } else if (token == "isFull") {
            outputs.push_back(isFull() ? "true" : "false");
        }
    }

    std::string result;
    for (std::size_t i = 0; i < outputs.size(); ++i) {
        if (i > 0) result += " ";
        result += outputs[i];
    }
    return result;
}

// The solution uses a classic circular array implementation of a deque with `front` and `rear` indices initialized to `-1` (empty). For each command parsed from the input string via `std::istringstream`, we process it in order. The key operations:  
// - `pushFront`: If full, output `"false"`. If empty, set both `front` and `rear` to 0. Else if `front == 0` and `rear != size-1`, wrap `front` to `size-1`. Otherwise decrement `front`. Place value at `arr[front]` and output `"true"`.  
// - `pushRear`: If full, output `"false"`. If empty, set both to 0. Else if `rear == size-1` and `front != 0`, wrap `rear` to 0. Otherwise increment `rear`. Place value and output `"true"`.  
// - `popFront`: If empty, output `-1`. Else save `arr[front]`, set `arr[front]` to `-1` (sentinel). If `front == rear`, set both to `-1`. Else if `front == size-1`, set `front = 0`. Else increment `front`. Output saved value.  
// - `popRear`: Similar, but with `rear` decrement and wrap.  
// - `getFront`/`getRear`: Output `-1` if empty, else return `arr[front]`/`arr[rear]`.  
// - `isEmpty`/`isFull`: Output `"true"`/`"false"` based on conditions.  
//
// Important edge cases: The circular condition `isFull` must check both `(front == 0 && rear == size-1)` and `(rear == (front-1) % (size-1))` — note the original snippet uses a flawed `% (size-1)` which fails when `size=6` and `front=0` gives `rear == -5 % 5` = `-0`? Actually `(front-1) % (size-1)` = `-1 % 5` = `-1` (in C++), so correct condition should be `(rear == (front-1 + size) % size)` or `(rear == (front-1 + size) % size)`. For correctness, we use: `(front == 0 && rear == size-1) || (rear == (front-1 + size) % size)`. But since capacity is fixed at 6, we can simply check `((rear + 1) % size == front)`. Our implementation will use a robust circular check. Time complexity is O(C) where C is the number of commands, each operation O(1). Space is O(1) extra (only the fixed array of size 6).
