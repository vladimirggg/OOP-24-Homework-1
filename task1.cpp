#include "MultiSet.h"

int main(){
    MultiSet set1(10,2);
    set1.add(1);
    set1.add(2);
    set1.add(3);
    set1.add(2);
    set1.add(2);
    set1.add(2);
    set1.add(2);
    set1.add(3);
    set1.add(1);
    set1.printNumbers();
    std::cout<<"Occurrence of 1: "<<(int)set1.occurrenceCount(1);
    std::cout<<"\nOccurrence of 2: "<<(int)set1.occurrenceCount(2);
    std::cout<<"\nOccurrence of 3: "<<(int)set1.occurrenceCount(3);
}