// Ring buffer + generational arena for Senlight Coder AI training.

use std::collections::VecDeque;

/// A fixed-capacity circular buffer that overwrites the oldest element.
pub struct RingBuffer<T> {
    deque: VecDeque<T>,
    capacity: usize,
}

impl<T> RingBuffer<T> {
    pub fn new(capacity: usize) -> Self {
        RingBuffer { deque: VecDeque::with_capacity(capacity), capacity }
    }

    pub fn push(&mut self, value: T) -> Option<T> {
        self.deque.push_back(value);
        if self.deque.len() > self.capacity {
            self.deque.pop_front()
        } else {
            None
        }
    }

    pub fn len(&self) -> usize {
        self.deque.len()
    }

    pub fn is_empty(&self) -> bool {
        self.deque.is_empty()
    }

    pub fn iter(&self) -> std::collections::vec_deque::Iter<'_, T> {
        self.deque.iter()
    }
}

/// A simple generational arena meant to demonstrate stable handles into a
/// vector of objects managed by a free-list.
pub struct Arena<T> {
    data: Vec<T>,
    free: Vec<u32>,
    alive: Vec<bool>,
    generation: Vec<u32>,
}

pub struct Handle {
    index: u32,
    generation: u32,
}

impl Handle {
    pub fn index(&self) -> u32 {
        self.index
    }
}

impl<T> Arena<T> {
    pub fn new() -> Self {
        Arena { data: Vec::new(), free: Vec::new(), alive: Vec::new(), generation: Vec::new() }
    }

    /// Allocate a new slot, reusing a freed index if available.
    pub fn alloc(&mut self, value: T) -> Handle {
        if let Some(idx) = self.free.pop() {
            self.data[idx as usize] = value;
            self.alive[idx as usize] = true;
            return Handle { index: idx, generation: self.generation[idx as usize] };
        }
        self.data.push(value);
        self.alive.push(true);
        self.generation.push(0);
        let index = (self.data.len() - 1) as u32;
        Handle { index, generation: 0 }
    }

    /// Free a handle, optionally bumping its generation to invalidate stale
    /// handles that point at this slot.
    pub fn free(&mut self, handle: Handle) {
        let idx = handle.index as usize;
        if idx < self.alive.len() && self.alive[idx] {
            self.alive[idx] = false;
            self.generation[idx] = self.generation[idx].wrapping_add(1);
            self.free.push(handle.index);
        }
    }

    /// Access a live value, validating the handle's generation.
    pub fn get(&self, handle: Handle) -> Option<&T> {
        let idx = handle.index as usize;
        if idx < self.data.len()
            && self.alive[idx]
            && self.generation[idx] == handle.generation
        {
            Some(&self.data[idx])
        } else {
            None
        }
    }

    pub fn len(&self) -> usize {
        self.alive.iter().filter(|&&a| a).count()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ring_buffer_evicts_oldest() {
        let mut buf = RingBuffer::new(2);
        assert_eq!(buf.push(1), None);
        assert_eq!(buf.push(2), None);
        assert_eq!(buf.push(3), Some(1));
        assert_eq!(buf.len(), 2);
    }

    #[test]
    fn arena_reuses_freed_slots() {
        let mut arena = Arena::new();
        let a = arena.alloc(10);
        arena.free(a);
        let b = arena.alloc(20);
        assert_eq!(b.index(), a.index());
        assert_eq!(arena.len(), 1);
    }
}