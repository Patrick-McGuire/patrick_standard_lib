#include <iostream>
//#include <iomanip>
#include "src/psl.h"
#include "src/String.h"
//#include "src/Array2.h"
#include "src/Converter.h"
#include "src/pub_sub/Manager.h"
#include "src/pub_sub/Publisher.h"
#include "src/pub_sub/Subscriber.h"
#include "src/Array.h"
using namespace psl;

//
//
//int main() {
//    String<100> str = "123456789";
//    String<120> str2 = str;
//    StringRef aa(str);
//    ConstStringRef bb(str);
//
//    {
//        Substr sub = str.substr(3, 3);
//        Substr sub2 = std::move(sub);
//        sub.release();
//        std::cout << sub2.c_str() << "\n";
//        sub2.release();
//        std::cout << sub.c_str() << "\n";
//
//        sub = str.substr(4, 3);
//        std::cout << sub.c_str() << ", " << "" << "\n";
//    }
//    std::cout << str.c_str() << "\n";
//    std::cout << str2.c_str() << "\n";
//
//}

//struct Test {
//    double a;
//    double b;
//};
//
//void event(StringRef test, double time) {
//    std::cout << test.c_str() << ", :" << time << " FROM EVENT\n";
////    std::cout << test.a << ", " << test.b << ", time: " << time <<" FROM EVENT\n";
//}
//
//int main() {
//    Manager manager;
//    Publisher<String<100>> anglePub(manager, "Angle");
//    Subscriber<String<100>, 5> angleSub(manager, "Angle");
//
//
//    angleSub.setCallback(event);
//
//    String<100> str = "asdf";
//    anglePub.publish(&str, 4);
//    anglePub.publish("{432.32423, 9.1}", 3424.4);
//
////    anglePub.publish({123, 3}, 4);
////    anglePub.publish({432.32423, 9.1}, 3424.4);
//
//    while (angleSub.available()) {
//        std::cout << angleSub.peek().c_str() << ", :" << angleSub.peekTime()  << " FROM QUEUE\n";
////        std::cout << angleSub.peek().a << ", " << angleSub.peek().b << ", " << angleSub.peekTime() << "\n";
//        angleSub.pop();
//    }
//
//}
//
void printArray(const ArrayRef<double> &array) {
    for (int i = 0; i < array.length(); i++) {
        std::cout << array[i] << ", ";
    }
    psl::println("");
}

int main() {
    Array<double, 10> array;

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
    array.insert(3, 99);
    printArray(array);
    array.remove(5);
    printArray(array);


    return 0;
}