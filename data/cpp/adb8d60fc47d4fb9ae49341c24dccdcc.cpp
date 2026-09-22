/*
Write a standalone C++ function that processes a singly linked list of integers (provided through its head pointer) and returns a new singly linked list containing only the prime numbers from the original list, preserving their relative order. The function must take a `const` pointer to the head node, not modify the input list, and return the head of a newly allocated list. You may define your own node structure inside your solution; do not rely on any external linked list library. The function should handle empty lists, lists with no primes, and lists with duplicate primes. Memory for the new list must be properly allocated; you do not need to handle deallocation in this exercise.
*/

#include <cmath>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Checks if a given integer is prime. Returns true for primes, false otherwise.
bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i <= std::sqrt(num); i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

// Given the head of a singly linked list, returns the head of a new list
// containing only the prime numbers, preserving order. Does not modify the input.
Node* filterPrimes(const Node* head) {
    Node* resultHead = nullptr;
    Node* resultTail = nullptr;
    
    for (const Node* current = head; current != nullptr; current = current->next) {
        if (isPrime(current->data)) {
            Node* newNode = new Node(current->data);
            if (resultHead == nullptr) {
                resultHead = resultTail = newNode;
            } else {
                resultTail->next = newNode;
                resultTail = newNode;
            }
        }
    }
    return resultHead;
}

#include <cassert>

int main() {
    // Helper to build a list from an array
    Node* buildList(const int* arr, int size) {
        Node* head = nullptr;
        Node* tail = nullptr;
        for (int i = 0; i < size; ++i) {
            Node* n = new Node(arr[i]);
            if (head == nullptr) head = tail = n;
            else { tail->next = n; tail = n; }
        }
        return head;
    }

    // Helper to convert list to vector for easy comparison
    std::vector<int> toVector(const Node* head) {
        std::vector<int> vec;
        for (const Node* p = head; p != nullptr; p = p->next) vec.push_back(p->data);
        return vec;
    }

    // Test 1: Mixed list
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Node* l1 = buildList(arr1, 10);
    Node* res1 = filterPrimes(l1);
    std::vector<int> v1 = toVector(res1);
    assert(v1 == std::vector<int>({2, 3, 5, 7}));

    // Test 2: No primes
    int arr2[] = {0, 1, 4, 6, 8, 9};
    Node* l2 = buildList(arr2, 6);
    Node* res2 = filterPrimes(l2);
    assert(res2 == nullptr);

    // Test 3: Empty list
    Node* res3 = filterPrimes(nullptr);
    assert(res3 == nullptr);

    // Test 4: Duplicate primes and negatives
    int arr4[] = {-3, 11, -7, 11, 2, 13, 13, 0};
    Node* l4 = buildList(arr4, 8);
    Node* res4 = filterPrimes(l4);
    std::vector<int> v4 = toVector(res4);
    assert(v4 == std::vector<int>({11, 11, 2, 13, 13}));

    // Test 5: All primes
    int arr5[] = {2, 3, 5, 7};
    Node* l5 = buildList(arr5, 4);
    Node* res5 = filterPrimes(l5);
    std::vector<int> v5 = toVector(res5);
    assert(v5 == std::vector<int>({2, 3, 5, 7}));

    // Test 6: Only largest prime
    int arr6[] = {97};
    Node* l6 = buildList(arr6, 1);
    Node* res6 = filterPrimes(l6);
    assert(toVector(res6) == std::vector<int>({97}));
}

// The core approach is to traverse the input list once, and for each node, test whether its integer value is prime. A number is prime if it is greater than 1 and has no divisors other than 1 and itself. To test primality efficiently, check divisibility up to the square root of the number. For each prime found, create a new node and append it to the result list. Since we preserve relative order, it is easiest to append to the tail of the result list, keeping track of both head and tail pointers. Edge cases include: an empty input list (return `nullptr`), a list with no primes (return `nullptr`), negative numbers and 0/1 (all non-prime), and duplicate primes (each should be added separately). The algorithm runs in O(n * sqrt(m)) time, where n is the number of nodes and m is the maximum integer value in the list; auxiliary space is O(k), where k is the number of primes in the result list, due to newly allocated nodes.
