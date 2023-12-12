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
#include <string>

using namespace psl;


#include "iomanip"

int main() {
//    char buff[100];
//    memset(buff, 0, 100);
//    int a = doubleToString(255.101, buff, 50, 100, 16);
//    std::cout << &buff[0] << ", " << a << "\n";

//    double aa = 123456789.123456789123456789;
//    aa += ;
//    std::cout << std::setprecision(100) << aa <<  "\n";

    std::cout << std::setprecision(100);
    bool success;
    double result = stringToDouble("3794F7.AF83BDA", &success, 16);
    std::cout << std::setprecision(100) << result << ", " << double(0.) << ", " << success << "\n";


//    std::cout << integer_min_value<unsigned long long>() << "\n";
//    9223372036854775807
//    18446744073709551615
//    std::cout << willOverflow<uint64_t>((uint64_t)1844674407370955161, '6') << "\n";
//    typedef int64_t T;
//    bool success;
//    T result = stringToInteger<T>("3794F7", &success, 16);
//    std::cout << result << ", " << (int) success << "\n";


}


//void constFunc(ConstStringRef ref) {
//
//}
//
//void func(StringRef ref) {
//
//}
//
//int main() {
//    String<100> str = "123456789";
//    const String<120> str2 = str;
//
//    StringRef ref(str);
////    StringRef ref2(str2);
//    ConstStringRef ref3(str2);
//
//    ConstStringRef ref22 = ref;
////    StringRef ref33 = ref3;
//    {
//        Substr sub = str.substr(3, 3);
////        func(str.substr(3, 3));
////        constFunc(str.substr(3, 3));
////        StringRef aRef = sub;
//        ConstStringRef aaRef = sub;
//
//        Substr sub2 = std::move(sub);
//
//        std::cout << sub << "\n";
//        std::cout << sub2 << "\n";
//        sub2.release();
//        std::cout << sub2 << "\n";
//
//        sub = str.substr(4, 3);
//        std::cout << sub << "\n";
//    }
//    std::cout << str << "\n";
//    std::cout << str2 << "\n";
//
//    return 0;
//}
//
////struct Test {
////    double a;
////    double b;
////};
////
////void event(StringRef test, double time) {
////    std::cout << test << ", :" << time << " FROM EVENT\n";
//////    std::cout << test.a << ", " << test.b << ", time: " << time <<" FROM EVENT\n";
////}
////
////int main() {
////    Manager manager;
////    Publisher<String<100>> anglePub(manager, "Angle");
////    Subscriber<String<100>, 5> angleSub(manager, "Angle");
////
////
////    angleSub.setCallback(event);
////
////    String<100> str = "asdf";
////    anglePub.publish(&str, 4);
////    anglePub.publish("{432.32423, 9.1}", 3424.4);
////
//////    anglePub.publish({123, 3}, 4);
//////    anglePub.publish({432.32423, 9.1}, 3424.4);
////
////    while (angleSub.available()) {
////        std::cout << angleSub.peek() << ", :" << angleSub.peekTime()  << " FROM QUEUE\n";
//////        std::cout << angleSub.peek().a << ", " << angleSub.peek().b << ", " << angleSub.peekTime() << "\n";
////        angleSub.pop();
////    }
////
////}
////
////void printArray(ArrayRef<double> array) {
////    for (int i = 0; i < array.length(); i++) {
////        std::cout << array[i] << ", ";
////    }
////    psl::println("");
////}
////
////int main() {
////    Array<double, 10> array;
////    array.append({123, 32});
////    array = {123, 32};
////    printArray(array);
////    array.clear();
////
////    array.length();
////    array.maxLength();
////    array.full();
////    array.empty();
////
////    array.append(1.4);
////    array.append(2.5);
////    array.append(3.6);
////    array.append(4.7);
////    array.append(5.8);
////    array.append(6.9);
////    array.insert(3, 99);
////    printArray(array);
////    array.remove(5);
////    printArray(array);
////
////
////    return 0;
////}