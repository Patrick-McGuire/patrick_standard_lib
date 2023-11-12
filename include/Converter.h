#ifndef PATRICK_STANDARD_LIB_CONVERTER_H
#define PATRICK_STANDARD_LIB_CONVERTER_H

#include <intrin.h>
#include <cstdint>
#include "psl.h"
#include "cstdio"

namespace psl {
    inline bool charIsDigit(char c) {
        return c >= '0' && c <= '9';
    }

    inline bool charIsWhitespace(char c) {
        return c == '\r' || c == '\n' || c == ' ' || c == '\t' || c == '\v' || c == '\f';
    }

    inline char *doubleToString(double val, char *buff, uint8_t precision, uint8_t maxLength = 255) {
        // Get sign
        if (val < 0) {
            maxLength--;
            buff[0] = '-';
            buff++;
            val = -val;
        }
        // Handle rounding
        double rounding = 0.5;
        for (int i = 0; i < precision; ++i) {
            rounding /= 10.0;
        }
        val += rounding;

        // Separate the decimal and integer parts of the value
        unsigned long int_part = (unsigned long) val;
        double remainder = val - (double) int_part;

        if (precision > 0) {
            // Extract digits from the remainder
            unsigned long dec_part = 0;
            double decade = 1.0;
            for (int i = 0; i < precision; i++) {
                decade *= 10.0;
            }
            remainder *= decade;
            dec_part = (int) remainder;

            while (dec_part > 0 && dec_part % 10 == 0) {
                dec_part /= 10;
            }

            snprintf(buff, maxLength, "%ld.%ld", int_part, dec_part);
        } else {
            snprintf(buff, maxLength, "%ld", int_part);
        }
        return buff;
    }

    inline char *int64ToString(int64_t val, char *buff, uint8_t maxLength = 255) {
        snprintf(buff, maxLength, "%lld", val);
        return buff;
    }

    inline double stringToDouble(const char *str) {
        bool positive = true;
        // Remove all leading whitespace
        for (; charIsWhitespace(*str); str++);
        // Detect positive
        if (*str == '+' || *str == '-') {
            if (*(str++) == '-') {
                positive = false;
            }
        }
        // Get the integer portion of the number
        double output = 0;
        for (; charIsDigit(*str); str++) {
            output = output * 10.0 + (*str - '0');
        }
        // Get the decimal portion if it'str there
        if (*str == '.') {
            str++;
            double decOutput = 0;
            int exponent = 0;
            for (; charIsDigit(*str); str++) {
                decOutput = decOutput * 10.0 + (*str - '0');
                exponent++;
            }
            for (; exponent > 0; exponent--) {
                decOutput /= 10;
            }
            output += decOutput;
        }
        if (!positive) output *= -1;
        return output;
    }

}

#endif //PATRICK_STANDARD_LIB_CONVERTER_H
