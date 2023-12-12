#ifndef PATRICK_STANDARD_LIB_CONVERTER_H
#define PATRICK_STANDARD_LIB_CONVERTER_H

#include "psl.h"
#include <cstdio>
#include <cfloat>

namespace psl {
    constexpr const char *DIGIT_TO_CHAR = "0123456789ABCDEF";

    inline bool charIsDigit(char c, const uint8_t base = 10) {
        return (c >= '0' && c <= '0' + base - 1) || ((base > 10) && ((c >= 'A' && c <= 'A' + base - 11) || (c >= 'a' && c <= 'a' + base - 11)));
    }

    inline uint8_t charToDigit(char c) {
        if (charIsDigit(c, 10)) return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0;
    }

    inline bool charIsWhitespace(char c) {
        return c == '\r' || c == '\n' || c == ' ' || c == '\t' || c == '\v' || c == '\f';
    }

    template<typename T>
    t_size integerToString(T num, char *buff, t_size maxLength = -1, const uint8_t base = 10) {
        // Check for errors
        if (base < 2 || base > 16) {
            return INDEX_ERROR;
        }
        // Handle the 0 case
        if (num == 0) {
            *buff++ = '0';
            *buff = '\0';
            return 2;
        }
        // Save the start for later
        char *start = buff;
        // Handle negative numbers
        bool negative = num < 0;
        num *= negative ? -1 : 1;
        // Add numbers to string backwards
        for (; num > 0; maxLength--, buff++, num /= base) {
            // Don't allow the string to be overfilled
            if (maxLength == 2) {
                *start = '\0';
                return OVERFLOW_ERROR;
            }
            // Add the number
            *buff = DIGIT_TO_CHAR[num % base];
        }
        // Add negative sign to end
        if (negative) *buff++ = '-';
        // Add null term, subtract one from buff so that buff points to the end char (needed for next for loop)
        *buff = '\0';
        // Reverse the string
        for (char *front = start, *back = buff - 1; front < back; front++, back--) {
            char swap = *front;
            *front = *back;
            *back = swap;
        }
        // Return length, +1 for null term
        return (t_size) (buff - start + 1);
    }

    template<typename T>
    T stringToInteger(const char *str, bool *success = nullptr, const uint8_t base = 10) {
        bool containsNumber;
        // Remove leading whitespace
        for (; charIsWhitespace(*str); str++);
        if (*str == '-') {
            if (!is_signed_type<T>::value) {
                if (success != nullptr) *success = false;
                return 0;
            }
            str++;
            // Collect the number
            T output = 0;
            T integerMax = integer_min_value<T>();
            containsNumber = charIsDigit(*str, base);
            for (; charIsDigit(*str, base); str++) {
                // Check for overflow
                if ((output < integerMax / (T) base) || (output < (integerMax + charToDigit(*str)) / (T) base)) {
                    if (success != nullptr) *success = false;
                    return 0;
                }
                // Add the next char to the number
                output = output * base - charToDigit(*str);
            }
            if (success != nullptr) *success = containsNumber;
            return output;
        } else {
            // Ignore positive signs
            if (*str == '+') str++;
            // Collect the number
            T output = 0;
            T integerMax = integer_max_value<T>();
            containsNumber = charIsDigit(*str, base);
            for (; charIsDigit(*str, base); str++) {
                // Check for overflow
                if ((output > integerMax / (T) base) || (output > (integerMax - charToDigit(*str)) / (T) base)) {
                    if (success != nullptr) *success = false;
                    return 0;
                }
                // Add the next char to the number
                output = output * base + charToDigit(*str);
            }
            if (success != nullptr) *success = containsNumber;
            return output;
        }
    }

    template<typename T>
    inline T stringToInteger(const char *str, T defaultValue, const uint8_t base = 10) {
        bool success;
        T result = stringToInteger<T>(str, &success);
        return success ? result : defaultValue;
    }

    t_size doubleToString(double num, char *buff, uint8_t precision = 2, t_size maxLength = -1, const uint8_t base = 10) {
        // Check for errors
        if (base < 2 || base > 16) {
            return INDEX_ERROR;
        }
        // Save the start for later
        char *start = buff;
        if (num < 0) {
            *buff++ = '-';
            maxLength--;
            num *= -1;
        }
        // Output the integer part of the number
        auto integerPart = (uint64_t) (num);
        t_size status = integerToString(integerPart, buff, maxLength, base);
        if (status < 0) {
            *start = '\0';
            return status;
        }
        buff += --status;
        maxLength -= status;
        // Output the decimal part of the number
        double decimalPart = num - double(integerPart);
        if (precision > 0 && decimalPart > 0) {
            *buff++ = '.';
            maxLength--;
            for (uint8_t i = 0; i < precision; ++i) {
                decimalPart *= base;
                auto digit = uint8_t(decimalPart);
                *buff++ = DIGIT_TO_CHAR[digit];
                decimalPart -= digit;
            }
        }
        return t_size(buff - start + 1);
    }

    double stringToDouble(const char *str, bool *success = nullptr, const uint8_t base = 10) {
        bool containsNumber, successInternal;
        // Ensure we have a tracker for success
        if (success == nullptr) success = &successInternal;
        // Remove leading whitespace
        for (; charIsWhitespace(*str); str++);
        // Handle signs
        bool isNegative = false;
        if (*str == '-') {
            isNegative = true;
            str++;
        } else if (*str == '+') str++;
        // Yoinkio that integer portion
        double output = 0;
        auto integerPortion = stringToInteger<uint64_t>(str, success, base);
        // Ensure a valid output
        if (!success || integerPortion > uint64_t(DBL_MAX)) {
            *success = false;
            return 0;
        }
        output = double(integerPortion);
        std::cout << integerPortion << ", " << output << " func\n";
        // Advance past the parsed number
        for (; charIsDigit(*str, base); str++);
        // Get the decimal portion if it's there
        if (*str == '.') {
            str++;
            double decOutput = 0;
            int exponent = 0;
            for (; charIsDigit(*str, base); str++) {
                decOutput = decOutput * base + charToDigit(*str);
                exponent++;
            }
            for (; exponent > 0; exponent--) {
                decOutput /= base;
            }
            output += decOutput;
        }
        // Return value
        *success = true;
        return output * (isNegative ? -1 : 1);
    }

    inline double stringToDouble(const char *str, double defaultValue, const uint8_t base = 10) {
        bool success;
        double result = stringToDouble(str, &success, base);
        return success ? result : defaultValue;
    }

}

#endif //PATRICK_STANDARD_LIB_CONVERTER_H
