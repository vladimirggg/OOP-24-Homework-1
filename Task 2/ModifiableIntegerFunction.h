#pragma once
#include <iostream>

class ModifiableIntegerFunction
{
    static const size_t NUMBER_OF_ELEMENTS;
    int16_t (*function)(int16_t);
    int16_t* resultsOfFunction;
    uint8_t* disabledNumbers;

    void copyFrom(const ModifiableIntegerFunction& other);
    void free();
    void initArray();
public:
    // Constructors
    ModifiableIntegerFunction();
    ModifiableIntegerFunction(int16_t (*_function)(int16_t));
    ModifiableIntegerFunction(const ModifiableIntegerFunction& other);
    ModifiableIntegerFunction& operator=(const ModifiableIntegerFunction& other);
    
    // Functions
    ModifiableIntegerFunction inverse() const;
    void setCustomResult(size_t number, int16_t result) const;
    void disable(int16_t number);
    int16_t invoke(int16_t number) const;
    bool isDisabled(int16_t number) const;
    bool isInjective() const;
    bool isSurjective() const;
    bool isBijective() const;
    bool areParallel(const ModifiableIntegerFunction& other) const;

    //Operators
    ModifiableIntegerFunction operator+(const ModifiableIntegerFunction& other) const;
    ModifiableIntegerFunction operator-(const ModifiableIntegerFunction& other) const;
    ModifiableIntegerFunction operator*(const ModifiableIntegerFunction& other) const;
    bool operator==(const ModifiableIntegerFunction& other) const;
    bool operator!=(const ModifiableIntegerFunction& other) const;
    bool operator<(const ModifiableIntegerFunction& other) const;
    bool operator<=(const ModifiableIntegerFunction& other) const;
    bool operator>(const ModifiableIntegerFunction& other) const;
    bool operator>=(const ModifiableIntegerFunction& other) const;
    ModifiableIntegerFunction operator^(size_t power) const;

    //Serializing/Deserializing
    void serialize(const char* filename) const;
    void deserialize(const char* filename);
    
    //Destructor
    ~ModifiableIntegerFunction();
};