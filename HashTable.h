#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "MovieNode.h"
#include <string>
#include <iostream>
using namespace std;

// Hash table entry node for chaining
struct HashEntry {
    string key;
    MovieNode** movies;      // Array of movie pointers
    int movieCount;          // Number of movies in this entry
    int capacity;            // Capacity of the movies array
    HashEntry* next;         // Pointer to next entry in chain
    
    HashEntry(const string& k) : key(k), movieCount(0), capacity(10), next(nullptr) {
        movies = new MovieNode*[capacity];
    }
    
    ~HashEntry() {
        delete[] movies;
    }
    
    void addMovie(MovieNode* movie) {
        if (movieCount >= capacity) {
            resize();
        }
        movies[movieCount++] = movie;
    }
    
private:
    void resize() {
        capacity *= 2;
        MovieNode** newMovies = new MovieNode*[capacity];
        for (int i = 0; i < movieCount; i++) {
            newMovies[i] = movies[i];
        }
        delete[] movies;
        movies = newMovies;
    }
};

class HashTable {
private:
    HashEntry** table;       // Array of hash table entries
    int tableSize;           // Size of the hash table
    int totalEntries;        // Total number of entries
    
    // Hash function: djb2 algorithm
    int hashFunction(const string& key) const {
        unsigned long hash = 5381;
        int c;
        const char* str = key.c_str();
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % tableSize;
    }
    
    // Helper function to convert to lowercase for case-insensitive search
    string toLower(const string& str) const {
        string result = str;
        for (int i = 0; i < result.length(); i++) {
            if (result[i] >= 'A' && result[i] <= 'Z') {
                result[i] = result[i] - 'A' + 'a';
            }
        }
        return result;
    }
    
public:
    // Constructor
    HashTable(int size = 1000) : tableSize(size), totalEntries(0) {
        table = new HashEntry*[tableSize];
        for (int i = 0; i < tableSize; i++) {
            table[i] = nullptr;
        }
    }
    
    // Destructor
    ~HashTable() {
        for (int i = 0; i < tableSize; i++) {
            HashEntry* current = table[i];
            while (current != nullptr) {
                HashEntry* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }
    
    // Insert a movie with a key (e.g., actor name, title, genre, director)
    void insert(const string& key, MovieNode* movie) {
        if (key.empty() || movie == nullptr) return;
        
        string lowerKey = toLower(key);
        int index = hashFunction(lowerKey);
        
        // Check if entry exists
        HashEntry* current = table[index];
        while (current != nullptr) {
            if (current->key == lowerKey) {
                // Entry exists, add movie to it
                current->addMovie(movie);
                return;
            }
            current = current->next;
        }
        
        // Create new entry
        HashEntry* newEntry = new HashEntry(lowerKey);
        newEntry->addMovie(movie);
        newEntry->next = table[index];
        table[index] = newEntry;
        totalEntries++;
    }
    
    // Search for movies by key (exact match)
    int search(const string& key, MovieNode** results, int maxResults) const {
        if (key.empty()) return 0;
        
        string lowerKey = toLower(key);
        int index = hashFunction(lowerKey);
        int resultCount = 0;
        
        HashEntry* current = table[index];
        while (current != nullptr) {
            if (current->key == lowerKey) {
                // Found the entry, copy movies to results
                int count = (current->movieCount < maxResults) ? current->movieCount : maxResults;
                for (int i = 0; i < count; i++) {
                    results[resultCount++] = current->movies[i];
                }
                return resultCount;
            }
            current = current->next;
        }
        
        return 0; // Key not found
    }
    
    // Search for movies by partial key match (substring search)
    int searchPartial(const string& key, MovieNode** results, int maxResults) const {
        if (key.empty()) return 0;
        
        string lowerKey = toLower(key);
        int resultCount = 0;
        
        // Search through all entries
        for (int i = 0; i < tableSize && resultCount < maxResults; i++) {
            HashEntry* current = table[i];
            while (current != nullptr && resultCount < maxResults) {
                // Check if key contains the search term
                if (current->key.find(lowerKey) != string::npos) {
                    // Add all movies from this entry
                    for (int j = 0; j < current->movieCount && resultCount < maxResults; j++) {
                        results[resultCount++] = current->movies[j];
                    }
                }
                current = current->next;
            }
        }
        
        return resultCount;
    }
    
    // Get all movies for a key (returns count and fills results array)
    int getMovies(const string& key, MovieNode** results, int maxResults) const {
        return search(key, results, maxResults);
    }
    
    // Check if a key exists
    bool contains(const string& key) const {
        if (key.empty()) return false;
        
        string lowerKey = toLower(key);
        int index = hashFunction(lowerKey);
        
        HashEntry* current = table[index];
        while (current != nullptr) {
            if (current->key == lowerKey) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get statistics
    int getTotalEntries() const {
        return totalEntries;
    }
    
    int getTableSize() const {
        return tableSize;
    }
    
    // Display hash table statistics
    void displayStats() const {
        int maxChainLength = 0;
        int emptyBuckets = 0;
        int totalChainLength = 0;
        
        for (int i = 0; i < tableSize; i++) {
            int chainLength = 0;
            HashEntry* current = table[i];
            
            if (current == nullptr) {
                emptyBuckets++;
            } else {
                while (current != nullptr) {
                    chainLength++;
                    current = current->next;
                }
                if (chainLength > maxChainLength) {
                    maxChainLength = chainLength;
                }
                totalChainLength += chainLength;
            }
        }
        
        cout << "\n=== Hash Table Statistics ===" << endl;
        cout << "Table Size: " << tableSize << endl;
        cout << "Total Entries: " << totalEntries << endl;
        cout << "Empty Buckets: " << emptyBuckets << endl;
        cout << "Max Chain Length: " << maxChainLength << endl;
        if (totalEntries > 0) {
            cout << "Average Chain Length: " << (double)totalChainLength / (tableSize - emptyBuckets) << endl;
        }
        cout << "Load Factor: " << (double)totalEntries / tableSize << endl;
    }
};

#endif // HASHTABLE_H

