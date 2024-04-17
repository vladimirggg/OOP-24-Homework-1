#include "MultiSet.h"
#include <fstream>
#include <exception>

uint8_t MultiSet::MAX_VALUES[8] = {
    0b11111111,
    0b01111111,
    0b00111111,
    0b00011111,
    0b00001111,
    0b00000111,
    0b00000011,
    0b00000001
};
size_t MultiSet::NUMBER_OF_BITS = 8;

MultiSet::MultiSet(): numberSet(nullptr), bucketSize(0), maxNumber(0){}

MultiSet::MultiSet(unsigned int n, unsigned int k) : maxNumber(n) {
    if(k < 1 || k > 8) throw std::out_of_range("K out of bounds!");
    else bucketSize = k;
    const size_t arraySize = getArraySize(); // 8 bit rounded up
    numberSet = new uint8_t[arraySize]; // round up
}

MultiSet::MultiSet(const MultiSet& other) {
    copyFrom(other);
}

MultiSet& MultiSet::operator=(const MultiSet& other) {
    if(this != &other){
        free();
        copyFrom(other);
    }
    return *this;
}

void MultiSet::copyFrom(const MultiSet &other) {
    bucketSize = other.bucketSize;
    maxNumber = other.maxNumber;
    const size_t arraySize = getArraySize();
    numberSet = new uint8_t[arraySize];
    for (int i = 0; i <= arraySize; ++i)
        numberSet[i] = other.numberSet[i];
}

void MultiSet::free() {
    maxNumber = 0;
    bucketSize = 0;
    delete[] numberSet;
}

size_t MultiSet::getArraySize() const {
    return ((maxNumber * bucketSize) / 8) + 1;
}

void MultiSet::add(unsigned int number) {
    if(number > maxNumber) throw std::out_of_range("Out of bounds!");
    if(occurrenceCount(number) == MultiSet::MAX_VALUES[MultiSet::NUMBER_OF_BITS - bucketSize]) return;

    size_t clusterIndex = ((number - 1) * bucketSize) / MultiSet::NUMBER_OF_BITS;
    size_t bitIndex = ((number - 1) * bucketSize) % MultiSet::NUMBER_OF_BITS;

    if((numberSet[clusterIndex] >> bitIndex) == MultiSet::MAX_VALUES[MultiSet::NUMBER_OF_BITS - bitIndex]){
        numberSet[clusterIndex] &= ~(MultiSet::MAX_VALUES[MultiSet::NUMBER_OF_BITS - bitIndex] << bitIndex);
        //clear the last bits
        numberSet[clusterIndex + 1] += 1;

    }
    else{
        uint8_t mask = 1 << bitIndex;
        numberSet[clusterIndex] += mask;
    }
}

uint8_t MultiSet::occurrenceCount(unsigned int number) const {
    size_t clusterIndex = ((number - 1) * bucketSize) / MultiSet::NUMBER_OF_BITS;
    size_t bitIndex = ((number - 1) * bucketSize) % MultiSet::NUMBER_OF_BITS;
    uint8_t occurrenceMask = numberSet[clusterIndex] >> bitIndex;

    if (bitIndex + bucketSize > MultiSet::NUMBER_OF_BITS) {
        uint8_t remainingBits = bucketSize - (MultiSet::NUMBER_OF_BITS - bitIndex);

        occurrenceMask |= (numberSet[clusterIndex + 1] & ((1 << remainingBits) - 1)) << (MultiSet::NUMBER_OF_BITS - bitIndex);
    }

    occurrenceMask &= MAX_VALUES[MultiSet::NUMBER_OF_BITS - bucketSize];

    return occurrenceMask;
}

void MultiSet::printNumbers() const {
    for (size_t i = 1; i <= maxNumber; i++) {
        uint8_t occurrences = occurrenceCount(i);
        if (occurrences)
            std::cout << i << " occurs "<< (int)occurrences << " times." <<std::endl;
    }
}

void MultiSet::printMultiSetInMemory() const {
    const size_t arraySize = getArraySize();
    for (size_t i = 0; i < arraySize; ++i) {
        for (int j = 7; j >= 0; --j) {
            std::cout << ((numberSet[i] >> j) & 1);
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

void MultiSet::serialize(const char* fileName) const{
    std::ofstream file(fileName, std::ios::binary);
    if(!file){
        std::cout<<"Error opening output file!\n";
        return;
    }
    const size_t arraySize = getArraySize();
    file.write((const char*) &maxNumber, sizeof(unsigned int));
    file.write((const char*) &bucketSize, sizeof(size_t));
    file.write((const char*) numberSet, arraySize * sizeof(uint8_t));
    file.close();
}

void MultiSet::deserialize(const char* fileName){
    std::ifstream file(fileName);
    if(!file){
        std::cout<<"Error opening input file!\n";
        return;
    }
    free();
    file.read((char*) &maxNumber, sizeof(unsigned int));
    file.read((char*) &bucketSize, sizeof(size_t));
    const size_t arraySize = getArraySize();
    numberSet = new uint8_t[arraySize];
    file.read((char*) numberSet, arraySize * sizeof(uint8_t));
}

MultiSet MultiSet::intersection(const MultiSet& other) const {
    if(bucketSize != other.bucketSize)
        throw std::invalid_argument("Incompatible Set!");

    unsigned int minMaxNumber = maxNumber > other.maxNumber ? other.maxNumber : maxNumber;
    MultiSet intersection(minMaxNumber, bucketSize);

    for (unsigned int num = 1; num <= minMaxNumber; ++num) {
        unsigned int occurrences = occurrenceCount(num) < other.occurrenceCount(num) ?
                            occurrenceCount(num) : other.occurrenceCount(num);
        for (unsigned int i = 0; i < occurrences; ++i) {
            intersection.add(num);
        }
    }

    return intersection;
}

MultiSet MultiSet::difference(const MultiSet& other) const {
    if(bucketSize != other.bucketSize)
        throw std::invalid_argument("Incompatible Set!");

    unsigned int maxMaxNumber = maxNumber < other.maxNumber ? other.maxNumber : maxNumber;
    MultiSet difference(maxMaxNumber, bucketSize);

    for (unsigned int num = 1; num <= maxMaxNumber; ++num) {
        unsigned int occ1 = occurrenceCount(num), occ2 = other.occurrenceCount(num);
        unsigned int diff = occ1 > occ2 ? occ1 - occ2 : occ2 - occ1;
        for (unsigned int i = 0; i < diff; ++i) {
            difference.add(num);
        }
    }

    return difference;
}

MultiSet MultiSet::fillInMultiSet() const {
    MultiSet filledIn(*this);
    const size_t arraySize = getArraySize();
    for (size_t i = 0; i < arraySize - 1; i++){
        filledIn.numberSet[i] = ~filledIn.numberSet[i];
    }

    size_t bitIndex = ((maxNumber - 1) * bucketSize) % MultiSet::NUMBER_OF_BITS;
    uint8_t mask = ~MultiSet::MAX_VALUES[MultiSet::NUMBER_OF_BITS - bitIndex];
    filledIn.numberSet[maxNumber - 1] &= mask;

    return filledIn;
}

MultiSet::~MultiSet() {
    free();
}
