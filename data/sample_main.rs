// Sample Rust source for Senlight Coder AI tokenizer training.
// Exercises tabs, nested indentation, and code token boundaries.

use std::collections::HashMap;

/// A tiny symbol table used to demo tokenization over Rust code.
pub struct SymbolTable {
    entries: HashMap<String, u64>,
}

impl SymbolTable {
    pub fn new() -> Self {
        SymbolTable { entries: HashMap::new() }
    }

    /// Insert a symbol and return its (possibly new) unique id.
    pub fn intern(&mut self, name: &str) -> u64 {
        let syms = &mut self.entries;
        match syms.get(name) {
            Some(id) => *id,
            None => {
                let next = syms.len() as u64;
                syms.insert(name.to_string(), next);
                next
            }
        }
    }

    pub fn resolve(&self, name: &str) -> Option<u64> {
        self.entries.get(name).copied()
    }
}

fn fibonacci(n: u64) -> u64 {
    if n < 2 {
        return n;
    }
    let (mut a, mut b) = (0u64, 1u64);
    for _ in 0..n {
        let temp = a + b;
        a = b;
        b = temp;
    }
    a
}

fn main() {
    let mut table = SymbolTable::new();
    table.intern("fib");
    table.intern("main");
    table.intern("SymbolTable");

    for i in 0..10 {
        let value = fibonacci(i);
        println!("fib({}) = {}", i, value);
    }

    match table.resolve("fib") {
        Some(id) => println!("'fib' resolves to id {}", id),
        None => println!("symbol not found"),
    }
}