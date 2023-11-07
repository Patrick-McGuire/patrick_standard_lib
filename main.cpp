#include <iostream>
#include <Array.h>

#include <vector>

using namespace pmcguire;


void printArray(const Array<double> &array) {
    for (int i = 0; i < array.length(); i++) {
        std::cout << array[i] << ", ";
    }
    std::cout << "\n";
}

int main() {
    auto array = ArrayStorage<double, 10>();
    array.length();
    array.maxLength();
    array.full();
    array.empty();

    array.append(1.4);
    array.append(2.5);
    array.append(3.6);
    array.append(4.7);
    array.append(5.8);
    array.append(6.9);
    array.insert(7, 99);
    printArray(array);
    array.remove(5);
    printArray(array);


    return 0;
}
