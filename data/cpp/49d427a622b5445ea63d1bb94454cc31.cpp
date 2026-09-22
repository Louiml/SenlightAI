// Write a C++ function `streamMedian(int arr[], int n)` that processes an array of integers one by one, maintaining a sorted linked list of all values seen so far. For each element added, the function must output the current median of the stream: if the number of elements seen so far is odd, output the middle value (integer); if even, output the floor of the average of the two middle values. The function should handle arbitrary integers (positive, negative, duplicates) and arrays of any length ≥ 1; it returns nothing but prints each median on a new line. Do not use vectors, arrays, or STL containers for the sorted structure — implement it as a singly linked list with dynamic allocation. Zero-indexing: after processing the i-th element (i starts at 0), the median corresponds to a stream of size i+1.

#include <cassert>
#include <iostream>
#include <sstream>

// Redirect cout to a string stream to capture output for testing
std::string capture(int arr[], int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    streamMedian(arr, n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    {
        int arr[] = {5, 2, 8, 1, 9};
        std::string out = capture(arr, 5);
        // Stream: 5 -> median 5; 5,2 -> sorted [2,5] median (2+5)/2=3.5 floor=3; 5,2,8 -> [2,5,8] median 5; 5,2,8,1 -> [1,2,5,8] median (2+5)/2=3.5 floor=3; 5,2,8,1,9 -> [1,2,5,8,9] median 5
        assert(out == "5\n3\n5\n3\n5\n");
    }
    {
        int arr[] = {1, 2, 3, 4};
        std::string out = capture(arr, 4);
        // 1 -> 1; 1,2 -> (1+2)/2=1.5 floor=1; 1,2,3 -> 2; 1,2,3,4 -> (2+3)/2=2.5 floor=2
        assert(out == "1\n1\n2\n2\n");
    }
    {
        int arr[] = {-1, -3, 5, 2};
        std::string out = capture(arr, 4);
        // -1 -> -1; -1,-3 -> sorted [-3,-1] median (-3-1)/2=-2 floor=-2; -1,-3,5 -> [-3,-1,5] median -1; -1,-3,5,2 -> [-3,-1,2,5] median (-1+2)/2=0.5 floor=0
        assert(out == "-1\n-2\n-1\n0\n");
    }
    {
        int arr[] = {7};
        std::string out = capture(arr, 1);
        assert(out == "7\n");
    }
    {
        int arr[] = {3, 3, 3};
        std::string out = capture(arr, 3);
        // 3 -> 3; 3,3 -> (3+3)/2=3; 3,3,3 -> 3
        assert(out == "3\n3\n3\n");
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>
#include <cmath>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// Print the median of a streaming sequence of integers using a sorted singly linked list.
// The input array is processed element by element, and after each insertion the current
// median is printed on a new line. For even-sized streams, the floor of the average of
// the two middle values is printed.
void streamMedian(int arr[], int n) {
    if (n <= 0) return;

    ListNode* head = nullptr; // sorted linked list
    int size = 0;

    for (int i = 0; i < n; ++i) {
        int value = arr[i];
        // Insert into sorted position
        ListNode* newNode = new ListNode(value);
        if (!head || head->val >= value) {
            newNode->next = head;
            head = newNode;
        } else {
            ListNode* curr = head;
            while (curr->next && curr->next->val < value) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
        ++size;

        // Compute median
        if (size % 2 == 1) {
            // Odd size: middle element
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                slow = slow->next;
                fast = fast->next->next;
            }
            std::cout << slow->val << std::endl;
        } else {
            // Even size: floor of average of two middle elements
            ListNode* slow = head;
            ListNode* fast = head;
            ListNode* prev = nullptr;
            while (fast && fast->next) {
                prev = slow;
                slow = slow->next;
                fast = fast->next->next;
            }
            // prev is the left middle, slow is the right middle
            double avg = (prev->val + slow->val) / 2.0;
            std::cout << static_cast<int>(std::floor(avg)) << std::endl;
        }
    }

    // Clean up memory
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// The core challenge is maintaining a dynamically sorted singly linked list and efficiently locating the median. Since the linked list is sorted at all times, we insert each new element in the correct position via linear traversal — worst case O(k) per insertion, where k is the current size. For median computation, we traverse the list with the two-pointer (slow/fast) technique: for an odd-sized list (size = i+1, where i is even), the slow pointer ends at the middle node; for an even-sized list (i is odd), slow and a `prev` pointer give the two middle nodes. The floor of the average is computed using integer arithmetic; note the original code has a bug that truncates incorrectly for negative halves, but we will use `floor((a+b)/2.0)` to ensure correct floor behavior (e.g., (2+3)/2=2.5→2, (-1+0)/2=-0.5→-1). Edge cases: empty array (should return without output), single element (median is that element), duplicates (list insertion must handle equal values, typically insert after equal elements or before — consistency doesn't affect median), and negative numbers (averaging must handle signs correctly). Time complexity: each insertion is O(n) worst case, and each median query is O(n) due to traversal, so overall O(n²) for n elements. Space complexity: O(n) for the linked list nodes. Memory management: we must delete all nodes after processing to avoid leaks, but the task only requests a free function, so we internally clean up after printing.
