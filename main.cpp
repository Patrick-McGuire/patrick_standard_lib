#include <iostream>
#include <iomanip>
#include "psl.h"
#include "String.h"
#include "Array.h"
#include "Converter.h"

using namespace psl;

void printArray(const Array<String *> &array) {
    for (int i = 0; i < array.length(); i++) {
        std::cout << array[i]->c_str() << ", ";
    }
    std::cout << "\n";
}

int main() {
    String::size<100> str("-3241.342,54,213.234354,.1243254,12343567.43212345");
    for (int i = 0; i <= str.count(','); i++) {
        String::substr split = str.split(',', i);
        std::cout << split.toDouble() << "\n";
    }
    std::cout << str.c_str() << "\n";


//    std::cout << std::setprecision(30) << str.toDouble() << "\n";

//    String::size<100> str1("abcdefg");
//    {
//        String::substr a = str1.subString(1, 4).subString(1,2).subString(0,1);
//        std::cout << a.c_str() << "\n";
//        std::cout << str1.c_str() << "\n";
//    }
//    std::cout << str1.c_str() << "\n";


//    Array<double>::size<10> array;
//    Array<double> ar = array;
//
//    array.length();
//    array.maxLength();
//    array.full();
//    array.empty();
//
//    array.append(1.4);
//    array.append(2.5);
//    array.append(3.6);
//    array.append(4.7);
//    array.append(5.8);
//    array.append(6.9);
//    array.insert(6, 99);
//    printArray(array);
//    array.remove(5);
//    printArray(array);


    return 0;
}