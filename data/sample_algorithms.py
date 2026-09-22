"""Common data structures and algorithms for Senlight Coder AI training."""

from typing import Any, Dict, List, Optional


class TreeNode:
    """A binary search tree node."""

    def __init__(self, value: int) -> None:
        self.value = value
        self.left: Optional["TreeNode"] = None
        self.right: Optional["TreeNode"] = None


class BST:
    """A simple binary search tree with insertion, search, and traversal."""

    def __init__(self) -> None:
        self.root: Optional[TreeNode] = None

    def insert(self, value: int) -> None:
        if self.root is None:
            self.root = TreeNode(value)
            return
        node = self.root
        while True:
            if value < node.value:
                if node.left is None:
                    node.left = TreeNode(value)
                    return
                node = node.left
            else:
                if node.right is None:
                    node.right = TreeNode(value)
                    return
                node = node.right

    def search(self, value: int) -> bool:
        node = self.root
        while node is not None:
            if node.value == value:
                return True
            node = node.left if value < node.value else node.right
        return False

    def inorder(self) -> List[int]:
        result: List[int] = []
        stack: List[TreeNode] = []
        node = self.root
        while stack or node is not None:
            while node is not None:
                stack.append(node)
                node = node.left
            node = stack.pop()
            result.append(node.value)
            node = node.right
        return result


def quicksort(arr: List[int], lo: int = 0, hi: Optional[int] = None) -> List[int]:
    """In-place quicksort using the Lomuto partition scheme."""
    if hi is None:
        hi = len(arr) - 1
    if lo < hi:
        pivot = arr[hi]
        i = lo - 1
        for j in range(lo, hi):
            if arr[j] <= pivot:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]
        arr[i + 1], arr[hi] = arr[hi], arr[i + 1]
        p = i + 1
        quicksort(arr, lo, p - 1)
        quicksort(arr, p + 1, hi)
    return arr


def merge_sort(arr: List[int]) -> List[int]:
    """Classic merge sort implementation."""
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    merged: List[int] = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            merged.append(left[i])
            i += 1
        else:
            merged.append(right[j])
            j += 1
    merged.extend(left[i:])
    merged.extend(right[j:])
    return merged


class LRUCache:
    """Least-Recently-Used cache using an ordered dict."""

    def __init__(self, capacity: int = 128) -> None:
        self.capacity = capacity
        self._store: "Dict[int, int]" = {}

    def get(self, key: int) -> int:
        if key not in self._store:
            return -1
        value = self._store.pop(key)
        self._store[key] = value
        return value

    def put(self, key: int, value: int) -> None:
        if key in self._store:
            self._store.pop(key)
        elif len(self._store) >= self.capacity:
            oldest = next(iter(self._store))
            self._store.pop(oldest)
        self._store[key] = value