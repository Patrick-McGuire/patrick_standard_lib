#ifndef PMCGUIRESTANDARD_PMCGUIRE_H
#define PMCGUIRESTANDARD_PMCGUIRE_H

#include "iostream"

namespace psl {
    typedef int t_size;
    typedef int t_index;

    inline void println(const char *str) {
        std::cout << str << "\n";
    }

    template<unsigned...>
    struct compile_time_sum;

    template<unsigned size>
    struct compile_time_sum<size> {
        static constexpr int value = size;
    };

    template<unsigned size, unsigned... sizes>
    struct compile_time_sum<size, sizes...> {
        static constexpr int value = size + compile_time_sum<sizes...>::value;
    };


    template <int N, typename... T>
    struct variadic_nth_type;

    template <typename T0, typename... T>
    struct variadic_nth_type<0, T0, T...> {
        typedef T0 type;
    };
    template <int N, typename T0, typename... T>
    struct variadic_nth_type<N, T0, T...> {
        typedef typename variadic_nth_type<N - 1, T...>::type type;
    };
}

#endif //PMCGUIRESTANDARD_PMCGUIRE_H
