#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <utility> // for std::pair for key-value pairs

// Template class declaration with key K and value V.
template <typename K, typename V>
class HashMap {
public:
    HashMap();
    ~HashMap();

    void insert(const K& key, const V& value);
    void remove(const K& key);
    V get(const K& key) const;
    bool contains(const K& key) const;

private:
    // Define the underlying data structure for the hash map
    // For simplicity, we can use a vector of lists (chaining for collision resolution)
    std::vector<std::list<std::pair<K, V>>> table;

    // Hash function
    unsigned long hashFunction(const K& key) const;

    // Resize table if necessary
    void resize();
};
