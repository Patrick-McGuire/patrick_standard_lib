#include <iostream>
#include "pfunction.h"
#include "cstring"
#include "cstdlib"
#include "MethodBinder.h"

using namespace pmcguire;

//volatile void doThing(StackFunction<double(int)> &functor, int i) {
//    auto a = [i](int j) {
//        std::cout << i << ", " << j << "\n";
//        return i * j + .01;
//    };
//    functor = &a;
//}
//
//volatile void thing2(StackFunction<double(int)> &functor, int i) {
//    volatile double a[100];
//    memset((void *) &a, 0, sizeof(a));
//    functor(i * 2);
////    volatile double aa = 0;
////    volatile double ba = 0;
////    volatile double ca = 0;
//}

struct AA {
    int a = 0;

    virtual double calla(int b) {
        a -= b;
        std::cout << a << " aa\n";
        return 9.87;
    }

    double callaC(int b) const {
        std::cout << a << " aa\n";
        return 1.1;
    }
};

struct BB : AA {
    double calla(int b) override {
        a += b;
        std::cout << a << " bb\n";
        return 6.54;
    }
};

void aFunc() {
    std::cout << "adf\n";
}

int main() {
    AA aa;

//    AA &ab = aa;
//    AA aaa;

//
    Method<double(int)> method(aa, &AA::calla);

//    Method<double(int)> method(aa, &AA::callaC);


//    Function<void(void)> stackFunction(aFunc);
//    Method<void(void)> weird(stackFunction);
//    weird();
//
//    Method<void(void)> lambdaMethod([]() { aFunc(); });
//    lambdaMethod();

//    aa.a = 10;

    std::cout << method(5) << "\n";
//    for (int i = 0; i < 5; i++) {
//        StackFunction<double(int)> functor;
//        doThing(functor, i);
//        thing2(functor, i);
////        volatile double a[100];
////        memset((void *) &a, 0, sizeof(a));
////        std::cout << functor(i * 2) << "\n";
////        volatile double aa[100];
////        memset((void *) &aa, 0, sizeof(aa));
//
//    }
    return 0;
}