#ifndef PMCGUIRESTANDARD_PMCGUIRE_H
#define PMCGUIRESTANDARD_PMCGUIRE_H

#include "iostream"

namespace psl {
    using std::max;
    using std::min;

    typedef int t_size;
    typedef int t_index;

    inline void println(const char *str) {
        std::cout << str << "\n";
    }

    template<typename T>
    struct is_signed_type {
        static const bool value = T(-1) < T(0);
    };

    template<typename T>
    T integer_max_value() {
        return ((T)1 << (sizeof(T) * 8 - (is_signed_type<T>::value ? 2 : 1))) + (((T)1 << (sizeof(T) * 8 - (is_signed_type<T>::value ? 2 : 1))) - 1);
    }

    template<typename T>
    T integer_min_value() {
        return is_signed_type<T>::value ? -integer_max_value<T>() - 1 : 0;
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


    template<int N, typename... T>
    struct variadic_nth_type;

    template<typename T0, typename... T>
    struct variadic_nth_type<0, T0, T...> {
        typedef T0 type;
    };
    template<int N, typename T0, typename... T>
    struct variadic_nth_type<N, T0, T...> {
        typedef typename variadic_nth_type<N - 1, T...>::type type;
    };

    enum : t_size {
        INDEX_ERROR = -1,
        OVERFLOW_ERROR = -2,
    };
}

#endif //PMCGUIRESTANDARD_PMCGUIRE_H
