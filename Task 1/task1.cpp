#include "MultiSet.h"

int main(){
    MultiSet set(10,5);
    set.add(1);
    set.add(2);
    set.add(3);
    set.add(2);
    set.add(3);
    set.add(1);
    set.printNumbers();
    std::cout<<"Occurrence of 1: "<<(int)set.occurrenceCount(1);
    std::cout<<"\nOccurrence of 2: "<<(int)set.occurrenceCount(2);
    std::cout<<"\nOccurrence of 3: "<<(int)set.occurrenceCount(3)<<std::endl;



    MultiSet set1(10, 5);
    set1.add(1);
    set1.add(2);
    set1.add(3);

    MultiSet set2(10, 5);
    set2.add(2);
    set2.add(3);
    set2.add(4);

    std::cout<<"=====intersection=====\n";
    MultiSet intersection = set1.intersection(set2);
    intersection.printNumbers(); // Expected output: 2, 3

    std::cout<<"=====difference=====\n";
    MultiSet difference = set1.difference(set2);
    difference.printNumbers(); // Expected output: 1, 4


}