#include "heatmap.h"

template <typename K, typename V>
HashMap<K, V>::HashMap() {
    // Constructor
    // Initialize your hash map here, e.g., set an initial size for the table
    table.resize(100); // Example: initializing the table with 100 buckets
}

template <typename K, typename V>
HashMap<K, V>::~HashMap() {
    // Destructor
    // Clean up your hash map here if necessary
}

template <typename K, typename V>
void HashMap<K, V>::insert(const K& key, const V& value) {
    // Insert a key-value pair into the hash map
    unsigned long index = hashFunction(key);
    table[index].push_back(std::make_pair(key, value));
}

template <typename K, typename V>
void HashMap<K, V>::remove(const K& key) {
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

template <typename K, typename V>
V HashMap<K, V>::get(const K& key) const {
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

template <typename K, typename V>
bool HashMap<K, V>::contains(const K& key) const {
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

template <typename K, typename V>
unsigned long HashMap<K, V>::hashFunction(const K& key) const {
    // Simple hash function to convert a key to an index in the hash table
    // Note: This is just a placeholder. You'll need a better hash function for real-world use.
    return std::hash<K>{}(key) % table.size();
}

template <typename K, typename V>
void HashMap<K, V>::resize() {
    // Resize the hash table
    // This function would typically be used to maintain efficient operations as the table grows.
    // It's a bit complex to implement, so it's left as a placeholder here.
}
