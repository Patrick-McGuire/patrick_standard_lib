#include <iostream>
//#include <Array.h>
#include <String.h>

#include <vector>

using namespace psl;

void doThing(String & str) {
    std::cout << str.c_str() << "\n";
}

//void stringSplit()

int main() {
    StringStorage<100> str;
    StringStorage<100> subStr;
    str.append("123456789");
    str.subString(subStr, 1, 3);
    std::cout << str.c_str() << "\n";
    std::cout << subStr.c_str() << "\n";


//    str.append("123456789");
//    {
//        SubString a = str.subString(1, 3);
//                auto b = a.subString(0, 2);
//                        auto c = b.subString(1, 1);
//        doThing(c);
////        SubString b = a.subString(0, 2);
////        doThing(b);
//    }
//    std::cout << str.c_str() << "\n";

//    std::cout << str.c_str() << "\n";

//    {
//        SubString str2 = str.subString(1, 3);
////        str.clear();
////        str.append(str2);
//        std::cout << str2.c_str() << "\n";
//        std::cout << str.c_str() << "\n";
//    }
//    std::cout << str.c_str() << "\n";

//    str.remove(1, 8);
//    std::cout << str.c_str();
}

//void printArray(const Array<double> &array) {
//    for (int i = 0; i < array.length(); i++) {
//        std::cout << array[i] << ", ";
//    }
//    std::cout << "\n";
//}
//
//int main() {
//    auto array = ArrayStorage<double, 10>();
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
//
//
//    return 0;
//}
