//#include ""
#include <iostream>
#include "src/psl.h"
#include "src/String.h"

using namespace psl;

void manipulateString(StringRef str) {
    str.append("123");
    std::cout << str.c_str() << " output\n";
}

int main() {
    String<100> str = "test1234";

    String<150> str2 = str;

    manipulateString(str);
    std::cout << str2.c_str() << " output\n";



}


//#include <iostream>
//#include <iomanip>
//#include "psl.h"
//#include "String2.h"
//#include "Array.h"
//#include "Converter.h"
//#include "pub_sub/Manager.h"
//#include "pub_sub/Publisher.h"
//#include "pub_sub/Subscriber.h"
//
//
//using namespace psl;
//
//struct Test {
//    double a;
//    double b;
//};
//
//void event(Test &test) {
//    std::cout << test.a << ", " << test.b << " FROM EVENT\n";
//}
//
//int main() {
//    Manager manager;
//
//    Publisher<Test> anglePub(manager, "Angle");
//
//    Subscriber<Test, 5> angleSub(manager, "Angle");
//
//    angleSub.setCallback(event);
//
//    anglePub.publish({123, 3}, 4);
//    anglePub.publish({432.32423, 9.1});
//
//    while (angleSub.available()) {
//        std::cout << angleSub.peek().a << ", " << angleSub.peek().b << ", " << angleSub.peekTime() << "\n";
//        angleSub.pop();
//    }
//
//}
//
////void printArray(const Array<String2 *> &array) {
////    for (int i = 0; i < array.m_lengthMemory(); i++) {
////        std::cout << array[i]->c_str() << ", ";
////    }
////    std::cout << "\n";
////}
//
////int main() {
////    String2::size<100> str("-3241.342,54,213.234354,.1243254,12343567.43212345");
////    for (int i = 0; i <= str.count(','); i++) {
////        String2::substr split = str.split(',', i);
////        std::cout << split.toDouble() << "\n";
////    }
////    std::cout << str.c_str() << "\n";
////
////
//////    std::cout << std::setprecision(30) << str.toDouble() << "\n";
////
//////    String2::size<100> str1("abcdefg");
//////    {
//////        String2::substr a = str1.subString(1, 4).subString(1,2).subString(0,1);
//////        std::cout << a.c_str() << "\n";
//////        std::cout << str1.c_str() << "\n";
//////    }
//////    std::cout << str1.c_str() << "\n";
////
////
//////    Array<double>::size<10> array;
//////    Array<double> ar = array;
//////
//////    array.m_lengthMemory();
//////    array.maxLength();
//////    array.full();
//////    array.empty();
//////
//////    array.append(1.4);
//////    array.append(2.5);
//////    array.append(3.6);
//////    array.append(4.7);
//////    array.append(5.8);
//////    array.append(6.9);
//////    array.insert(6, 99);
//////    printArray(array);
//////    array.remove(5);
//////    printArray(array);
////
////
////    return 0;
////}