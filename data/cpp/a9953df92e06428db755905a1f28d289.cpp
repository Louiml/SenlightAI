/*
Write a C++ function that simulates a dynamic queue of student records. Each record contains an admission number (integer), a name (character array or string), and marks (floating-point). The function should take a single string command as input, where commands are separated by semicolons and formatted as `INSERT <adno> <name> <mks>`, `DELETE`, `DISPLAY`, and `EXIT`. The function must process commands sequentially, maintaining a queue implemented with a linked list, and return a string containing the output of all `DELETE` and `DISPLAY` operations, with each output line separated by newline characters. For `DELETE`, output the deleted record in the format "Deleted: adno name mks". For `DISPLAY`, output all current records in the format "adno name mks" one per line. For `INSERT`, read the three fields and append to the queue. Handle underflow (attempting `DELETE` or `DISPLAY` on an empty queue) by outputting "Underflow" and overflow (when allocation fails) by outputting "Overflow". The function must not print to console; it must return the accumulated output string. The queue should be properly cleaned up (no memory leaks) before the function returns.
*/

#include <string>
#include <sstream>
#include <vector>

struct Node {
    int adno;
    std::string name;
    float mks;
    Node* link;
    Node(int a, const std::string& n, float m) : adno(a), name(n), mks(m), link(nullptr) {}
};

// Simulate a dynamic queue of student records based on a semicolon-separated command string.
std::string processQueueCommands(const std::string& commands) {
    std::string output;
    Node* front = nullptr;
    Node* rear = nullptr;

    std::vector<std::string> commandList;
    std::string temp;
    std::istringstream commandStream(commands);
    while (std::getline(commandStream, temp, ';')) {
        if (!temp.empty()) {
            commandList.push_back(temp);
        }
    }

    for (const auto& cmd : commandList) {
        std::istringstream iss(cmd);
        std::string op;
        iss >> op;

        if (op == "INSERT") {
            int adno;
            std::string name;
            float mks;
            iss >> adno >> name >> mks;
            Node* newNode = new (std::nothrow) Node(adno, name, mks);
            if (!newNode) {
                output += "Overflow\n";
                // Clean up and stop processing further commands
                for (Node* cur = front; cur; ) {
                    Node* toDelete = cur;
                    cur = cur->link;
                    delete toDelete;
                }
                return output;
            }
            if (rear == nullptr) {
                front = rear = newNode;
            } else {
                rear->link = newNode;
                rear = newNode;
            }
        } else if (op == "DELETE") {
            if (front == nullptr) {
                output += "Underflow\n";
            } else {
                Node* toDelete = front;
                output += "Deleted: " + std::to_string(toDelete->adno) + " " + toDelete->name + " " + std::to_string(toDelete->mks) + "\n";
                front = front->link;
                if (front == nullptr) {
                    rear = nullptr;
                }
                delete toDelete;
            }
        } else if (op == "DISPLAY") {
            if (front == nullptr) {
                output += "Underflow\n";
            } else {
                Node* cur = front;
                while (cur != nullptr) {
                    output += std::to_string(cur->adno) + " " + cur->name + " " + std::to_string(cur->mks) + "\n";
                    cur = cur->link;
                }
            }
        }
        // Ignore EXIT or unknown commands for simplicity, but EXIT stops processing? Here we just ignore.
    }

    // Clean up remaining nodes
    for (Node* cur = front; cur; ) {
        Node* toDelete = cur;
        cur = cur->link;
        delete toDelete;
    }

    return output;
}

#include <cassert>
#include <string>

// The solution function is declared above; this test file includes it via compilation linking.

int main() {
    // Test basic insert and delete
    assert(processQueueCommands("INSERT 1 Alice 85.5; DELETE;") == "Deleted: 1 Alice 85.500000\n");
    
    // Test display with multiple records
    assert(processQueueCommands("INSERT 1 Alice 85.5; INSERT 2 Bob 90.0; DISPLAY;") == "1 Alice 85.500000\n2 Bob 90.000000\n");
    
    // Test underflow on empty queue
    assert(processQueueCommands("DELETE; DISPLAY;") == "Underflow\nUnderflow\n");
    
    // Test FIFO order: delete should remove earliest inserted
    assert(processQueueCommands("INSERT 1 A 10.0; INSERT 2 B 20.0; DELETE; DELETE;") == "Deleted: 1 A 10.000000\nDeleted: 2 B 20.000000\n");
    
    // Test after delete, display shows remaining
    assert(processQueueCommands("INSERT 1 A 10.0; INSERT 2 B 20.0; DELETE; DISPLAY;") == "Deleted: 1 A 10.000000\n2 B 20.000000\n");
    
    // Test multiple commands in one call, including EXIT (ignored)
    assert(processQueueCommands("INSERT 5 E 50.0; INSERT 6 F 60.0; DELETE; DISPLAY; EXIT;") == "Deleted: 5 E 50.000000\n6 F 60.000000\n");
    
    // Test empty command string
    assert(processQueueCommands("") == "");
    
    // Test commands with extra semicolons
    assert(processQueueCommands(";;INSERT 1 X 1.0;DELETE;;") == "Deleted: 1 X 1.000000\n");
    
    // Test name with spaces (assuming name is a single token in command)
    assert(processQueueCommands("INSERT 1 JohnDoe 70.0; DISPLAY;") == "1 JohnDoe 70.000000\n");
    
    // Test underflow after all deletes
    assert(processQueueCommands("INSERT 1 A 1.0; DELETE; DELETE;") == "Deleted: 1 A 1.000000\nUnderflow\n");
    
    return 0;
}

// The solution uses a singly linked list where each node stores the student record and a pointer to the next node. The queue maintains two pointers, `front` and `rear`. To process the command string, split it by semicolons into individual commands. For each command, parse it by extracting the operation keyword and, in the case of `INSERT`, the additional data. Use a string stream to parse the fixed-format command. For `INSERT`, dynamically allocate a new node; if allocation fails (though in practice this may not happen in a test environment, it is handled for correctness), append to the rear; if the queue is empty, set both `front` and `rear` to the new node; otherwise link the new node after the current rear and update rear. For `DELETE`, if the queue is empty, append "Underflow" to the output; otherwise, remove the front node, capture its data into the output string formatted as "Deleted: adno name mks", delete the node, and update the front pointer (if the queue becomes empty, set rear to null as well). For `DISPLAY`, if empty, append "Underflow"; otherwise traverse from front to rear, formatting each record as "adno name mks" and appending with a newline. The returned string accumulates all outputs from DELETE and DISPLAY, with each output line terminated by a newline. After processing all commands, delete any remaining nodes to avoid memory leaks. Time complexity for each operation is O(1) for INSERT and DELETE (except deletion of entire queue at the end, which is O(n) total), and O(k) for DISPLAY where k is the current queue size. Space complexity is O(n) for the queue plus O(n) for the output string and input parsing overhead, but the auxiliary space excluding storage is O(1) per operation.
