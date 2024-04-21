#include "ModifiableIntegerFunction.h"
#include <fstream>
#include <exception>

const size_t ModifiableIntegerFunction::NUMBER_OF_ELEMENTS = INT16_MAX - INT16_MIN + 1;

void ModifiableIntegerFunction::copyFrom(const ModifiableIntegerFunction& other){
    function = other.function;
    resultsOfFunction = new int16_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS];
    disabledNumbers = new uint8_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8];
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        resultsOfFunction[i] = other.resultsOfFunction[i];
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            disabledNumbers[i] = other.disabledNumbers[i];
    }
}

void ModifiableIntegerFunction::free(){
    delete[] resultsOfFunction;
    delete[] disabledNumbers;
}

void ModifiableIntegerFunction::initArray(){
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        resultsOfFunction[i] = function(i + INT16_MIN);
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            disabledNumbers[i] = 0;
    }
}

ModifiableIntegerFunction::ModifiableIntegerFunction() : function(nullptr), resultsOfFunction(nullptr), disabledNumbers(nullptr) {}

ModifiableIntegerFunction::ModifiableIntegerFunction(int16_t (*_function)(int16_t)) : function(_function){
    resultsOfFunction = new int16_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS];
    disabledNumbers = new uint8_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8];
    if(function) initArray();
}

ModifiableIntegerFunction::ModifiableIntegerFunction(const ModifiableIntegerFunction& other){
    copyFrom(other);
}

ModifiableIntegerFunction& ModifiableIntegerFunction::operator=(const ModifiableIntegerFunction& other){
    if(this != &other){
        free();
        copyFrom(other);
    }
    return *this;
}

ModifiableIntegerFunction ModifiableIntegerFunction::inverse() const
{
    if(!isBijective()) throw std::invalid_argument("Function is not reversible!");
    ModifiableIntegerFunction result(nullptr);
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; ++i) {
        result.resultsOfFunction[resultsOfFunction[i]] = i + INT16_MIN;
    }
    return result;
}

void ModifiableIntegerFunction::setCustomResult(size_t number, int16_t result) const{
    resultsOfFunction[number - INT16_MIN] = result;
}

void ModifiableIntegerFunction::disable(int16_t number){
    if(isDisabled(number)) return;
    size_t index = (number - INT16_MIN) / 8;
    uint8_t mask = 1 << ((number - INT16_MIN) % 8);
    disabledNumbers[index] |= mask;
    resultsOfFunction[number - INT16_MIN] = 0;
}

int16_t ModifiableIntegerFunction::invoke(int16_t number) const {
    if(isDisabled(number))
        throw std::invalid_argument("The number is disabled!");

    return resultsOfFunction[number - INT16_MIN];
}

bool ModifiableIntegerFunction::isDisabled(int16_t number) const {
    size_t index = (number - INT16_MIN) / 8;
    uint8_t mask = 1 << ((number - INT16_MIN) % 8);
    return disabledNumbers[index] & mask;
}

bool ModifiableIntegerFunction::isInjective() const{
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8; i++)
        if(disabledNumbers[i]) return false;

    return true;
}

bool ModifiableIntegerFunction::isSurjective() const {
    bool *covered = new bool[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS];
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; ++i) {
        if (!isDisabled(i + INT16_MIN)) {
            int16_t output = resultsOfFunction[i];
            if (output >= INT16_MIN && output <= INT16_MAX) {
                covered[output - INT16_MIN] = true;
            }
        }
    }

    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; ++i) 
        if (!covered[i])
            return false;

    return true;
}

bool ModifiableIntegerFunction::isBijective() const {
    return isSurjective() && isInjective();
}

bool ModifiableIntegerFunction::areParallel(const ModifiableIntegerFunction& other) const {
    for (size_t i = 0; i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; ++i) {
        if (!isDisabled(i + INT16_MIN) && !other.isDisabled(i + INT16_MIN)) {
            int16_t diff1 = invoke(i + INT16_MIN);
            int16_t diff2 = other.invoke(i + INT16_MIN);
            if (diff1 != diff2)
                return false;
        }
    }
    return true;
}

ModifiableIntegerFunction ModifiableIntegerFunction::operator+(const ModifiableIntegerFunction& other) const{
    ModifiableIntegerFunction result(nullptr);
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        result.resultsOfFunction[i] = resultsOfFunction[i] + other.resultsOfFunction[i];
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            result.disabledNumbers[i] = disabledNumbers[i] & other.disabledNumbers[i];
    }
    return result;
}

ModifiableIntegerFunction ModifiableIntegerFunction::operator-(const ModifiableIntegerFunction& other) const{
    ModifiableIntegerFunction result(nullptr);
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        result.resultsOfFunction[i] = resultsOfFunction[i] - other.resultsOfFunction[i];
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            result.disabledNumbers[i] = disabledNumbers[i] & other.disabledNumbers[i];
    }
    return result;
}

ModifiableIntegerFunction ModifiableIntegerFunction::operator*(const ModifiableIntegerFunction& other) const{
    ModifiableIntegerFunction result(nullptr);
    for (size_t i = 0; i < NUMBER_OF_ELEMENTS; i++){
        result.resultsOfFunction[i] = function(other.resultsOfFunction[i]);
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            result.disabledNumbers[i] = disabledNumbers[i] | other.disabledNumbers[i];
    }
    return result;
}

bool ModifiableIntegerFunction::operator==(const ModifiableIntegerFunction& other) const {
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        if(resultsOfFunction[i] != other.resultsOfFunction[i])
            return false;
        if(i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            if(disabledNumbers[i] != other.disabledNumbers[i])
                return false;
    }
    return true;
}

bool ModifiableIntegerFunction::operator!=(const ModifiableIntegerFunction& other) const
{
    return !(*this == other);
}

bool ModifiableIntegerFunction::operator<(const ModifiableIntegerFunction& other) const{
    for (size_t i = 0; i < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; i++){
        if(resultsOfFunction[i] >= other.resultsOfFunction[i])
            return false;
        if(i <= ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8)
            if(disabledNumbers[i] != other.disabledNumbers[i])
                return false;
    }
    return true;
}

bool ModifiableIntegerFunction::operator<=(const ModifiableIntegerFunction& other) const
{
    return (*this < other) || (*this == other);
}

bool ModifiableIntegerFunction::operator>(const ModifiableIntegerFunction& other) const
{
    return !(*this <= other);
}

bool ModifiableIntegerFunction::operator>=(const ModifiableIntegerFunction& other) const
{
    return !(*this < other);
}

ModifiableIntegerFunction ModifiableIntegerFunction::operator^(size_t power) const{
    ModifiableIntegerFunction result(*this);

    for (size_t i = 0; i < power; ++i)
        for (size_t j = 0; j < ModifiableIntegerFunction::NUMBER_OF_ELEMENTS; ++j)
            result.resultsOfFunction[i] = function(resultsOfFunction[i]);

    return result;
}

void ModifiableIntegerFunction::serialize(const char* filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error opening output file! " << std::endl;
        return;
    }

    file.write((const char*)resultsOfFunction, NUMBER_OF_ELEMENTS * sizeof(int16_t));
    file.write((const char*)disabledNumbers, (NUMBER_OF_ELEMENTS / 8) * sizeof(uint8_t));

    file.close();
}

void ModifiableIntegerFunction::deserialize(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error opening input file! " << std::endl;
        return;
    }
    free(); // clear the memory 
    resultsOfFunction = new int16_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS];
    disabledNumbers = new uint8_t[ModifiableIntegerFunction::NUMBER_OF_ELEMENTS / 8];

    file.read((char*) resultsOfFunction, NUMBER_OF_ELEMENTS * sizeof(int16_t));
    file.read((char*) disabledNumbers, (NUMBER_OF_ELEMENTS / 8) * sizeof(uint8_t));

    file.close();
}

ModifiableIntegerFunction::~ModifiableIntegerFunction(){
    free();
}