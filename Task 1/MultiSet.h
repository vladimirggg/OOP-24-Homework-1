#pragma once
#include <iostream>

class MultiSet {
    static uint8_t MAX_VALUES[8];
    static size_t NUMBER_OF_BITS;
    uint8_t* numberSet;
    size_t bucketSize;
    unsigned int maxNumber;

    void copyFrom(const MultiSet& other);
    void free();
    size_t getArraySize() const;
public:
    MultiSet();
    MultiSet(unsigned int n, unsigned int k);
    MultiSet(const MultiSet& other);
    MultiSet& operator= (const MultiSet& other);

    void add(unsigned int number);
    uint8_t occurrenceCount(unsigned int number) const;

    void printNumbers() const;
    void printMultiSetInMemory() const;

    void serialize(const char* fileName) const; // out
    void deserialize(const char* fileName); // in

    MultiSet intersection(const MultiSet& other) const;
    MultiSet difference(const MultiSet& other) const;
    MultiSet fillInMultiSet() const;
    
    ~MultiSet();
};