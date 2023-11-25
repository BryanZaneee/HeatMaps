#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <utility>

template <typename K, typename V>
class HashMap {
public:
    HashMap() {
        // Initialize your hash map here, e.g., set an initial size for the table
        table.resize(100); // Example: initializing the table with 100 buckets
    }

    ~HashMap() {
        // Destructor
        // Clean up your hash map here if necessary
    }

    void insert(const K& key, const V& value) {
        // Insert a key-value pair into the hash map
        unsigned long index = hashFunction(key);
        table[index].push_back(std::make_pair(key, value));
    }

    void remove(const K& key) {
        // Remove a key-value pair from the hash map
        unsigned long index = hashFunction(key);
        auto& list = table[index];
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->first == key) {
                list.erase(it);
                return;
            }
        }
    }

    V get(const K& key) const {
        // Retrieve a value by key
        unsigned long index = hashFunction(key);
        const auto& list = table[index];
        for (const auto& pair : list) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    bool contains(const K& key) const {
        // Check if a key exists in the hash map
        unsigned long index = hashFunction(key);
        const auto& list = table[index];
        for (const auto& pair : list) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<std::list<std::pair<K, V>>> table;
    // The hash table, a vector of lists. Each list is a chain of key-value pairs (std::pair).
    // This structure is chosen to handle collisions using chaining.

    unsigned long hashFunction(const K& key) const {
        // Simple hash function to convert a key to an index in the hash table
        // Note: This is just a placeholder. You'll need a better hash function for real-world use.
        return std::hash<K>{}(key) % table.size();
    }

    void resize() {
        // Resize the hash table
        // This function would typically be used to maintain efficient operations as the table grows.
        // It's a bit complex to implement, so it's left as a placeholder here.
    }
};