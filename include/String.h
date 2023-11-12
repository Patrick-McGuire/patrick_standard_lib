#ifndef PATRICK_STANDARD_LIB_STRING_H
#define PATRICK_STANDARD_LIB_STRING_H

#include "psl.h"
#include "Converter.h"

namespace psl {
    class String {
    protected:
        struct StringData {
            char *buff;
            t_size length;
            t_size maxLength;
        };
        StringData &stringData;

        inline void term() const {
            stringData.buff[length()] = '\0';
        }

        inline void autoLength() {
            for (int i = 0; i < maxLength(); i++) {
                if (stringData.buff[i] == '\0') {
                    stringData.length = i + 1;
                    break;
                }
            }
            term();
        }

    public:
        // Class that has storage, will extend this
        template<unsigned N>
        class size;

        class substr;

        explicit String(StringData &stringData1) : stringData(stringData1) {}

        String(const String &other) = default;

        template<unsigned N>
        String(const size<N> &other);

        String(const substr &other);

        t_size length() const {
            return stringData.length;
        }

        t_size maxLength() const {
            return stringData.maxLength;
        }

        t_size availableLength() {
            return maxLength() - length();
        }

        bool full() const {
            return length() >= maxLength();
        }

        bool empty() const {
            return length() <= 0;
        }

        const char *c_str() const {
            return stringData.buff;
        }

        void clear() {
            stringData.buff[0] = '\0';
            stringData.length = 0;
        }

        virtual const char &operator[](psl::t_index i) const {
            return stringData.buff[i];
        }

        virtual char &operator[](psl::t_index i) {
            return stringData.buff[i];
        }

        bool equals(const String &other) const {
            if (length() == other.length()) {
                for (int i = 0; i < length(); i++) {
                    if ((*this)[i] != other[i]) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool append(char c) {
            if (!full()) {
                stringData.buff[length()] = c;
                stringData.length++;
                term();
                return true;
            }
            return false;
        }

        bool append(const char *str) {
            while (*str != '\0' && append(*str)) {
                str++;
            }
            return *str == '\0';
        }

        bool append(const String &other) {
            return append(other.c_str());
        }

        bool append(int num) {
            char converter[16];
            itoa(num, converter, 10);
            return append(converter);
        }

        bool append(double num, int digits = 4) {
            if (availableLength() > 0) {
                doubleToString(num, &stringData.buff[length()], digits, availableLength());
                return true;
            }
            return false;
        }

        bool remove(int i, int len = 1) {
            memmove(&stringData.buff[i], &stringData.buff[i + len], sizeof(char) * (stringData.length - i - len));
            stringData.length -= len;
            term();
            return true;
        }

        String &operator=(const String &other) {
            if (this == &other) {
                return *this;
            }
            clear();
            append(other);
            return *this;
        }

        String &operator=(const substr &other);

        template<unsigned N>
        String &operator=(const size<N> &other);

        substr subString(int start, int len);

        void subString(String &output, int start, int len);

        int toInt() const {
            return atoi(c_str());
        }

        double toDouble() const {
            return stringToDouble(c_str());
        }

        t_size count(char c) const {
            t_size count = 0;
            for (const char *str = stringData.buff; *str != '\0'; str++) {
                if (*str == c) count++;
            }
            return count;
        }

        t_size indexOf(char c, t_size n = 1) const {
            for (const char *str = stringData.buff; *str != '\0'; str++) {
                if (*str == c) {
                    n--;
                }

                if (n <= 0) {
                    return str - stringData.buff;
                }
            }
            return -1;
        }

        substr split(char c, t_size n = 0);
    };

    class String::substr final : public String {
    private:
        char swapChar = '\0';
        t_size swapIndex = -1;
        StringData stringDataNonRef{};

    public:
        substr(const String &str, int start, int len) : String(stringDataNonRef) {
            stringDataNonRef = {str.stringData.buff + start, len, len};
            swapIndex = len;
            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';
        }

        substr(const String::substr &other) : String(stringDataNonRef) {
            swapChar = other.swapChar;
            swapIndex = other.swapIndex;
            stringDataNonRef = other.stringDataNonRef;

            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';
        }

        ~substr() {
            (*this)[swapIndex] = swapChar;
        }
    };

    String::substr String::subString(int start, int len) {
        return {*this, start, len};
    }

    void String::subString(String &output, int start, int len) {
        output = subString(start, len);
    }

    /**
     * Array storage class
     * @brief Version of array that has it's own storage
     *
     * @tparam T Type of data to store in array
     * @tparam N Maximum number of elements
     */
    template<unsigned N>
    class String::size final : public String {
    private:
        char buff[N + 1] = "";
        StringData stringDataNonRef = {buff, 0, N};
    public:
        size() : String(stringDataNonRef) {}

        explicit size(const char *str) : String(stringDataNonRef) {
            append(str);
        }

        explicit size(double num, int precision = 4) : String(stringDataNonRef) {
            append(num, precision);
        }

        explicit size(int num) : String(stringDataNonRef) {
            append(num);
        }
    };

    template<unsigned int N>
    String::String(const String::size<N> &other) : stringData(other.stringData) {}

    String::String(const String::substr &other) : stringData(other.stringData) {}

    String &String::operator=(const String::substr &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        append(other);
        return *this;
    }

    String::substr String::split(char c, t_size n) {
        t_size start;
        if (n == 0) {
            start = 0;
        } else {
            start = indexOf(c, n) + 1;
        }


        t_size end = indexOf(c, n + 1);
        if (end < 0)
            end = length();
        return {
                *this, start, end - start
        };
    }

    template<unsigned int N>
    String &String::operator=(const String::size<N> &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        append(other);
        return *this;
    }

}

#endif //PATRICK_STANDARD_LIB_STRING_H
