#ifndef PATRICK_STANDARD_LIB_STRING_H
#define PATRICK_STANDARD_LIB_STRING_H

#include <psl.h>
#include "iostream"


//int Mat = rand()%7;
//
//if (Mat == 6 || Mat == 5){
//std::cout << "\nMat got away  :)";
//runaway++;
//} else{
//std::cout << "\nMat could not face the Gazebo alone  :(";
//}

namespace psl {
    class SubString;

    class String {
    protected:
        char *m_buff = nullptr;
        t_size &m_length;
        t_size m_maxLength = 0;

        void term() {
            m_buff[m_length] = '\0';
        }

    public:
        String(char *buff, t_size maxLength, t_size &lengthRef) : m_length(lengthRef) {
            m_buff = buff;
            m_maxLength = maxLength;
        }

        t_size &lengthRef() {
            return m_length;
        }

        const char *c_str() const {
            return m_buff;
        }

        t_size length() const {
            return m_length;
        }

        t_size maxLength() const {
            return m_maxLength;
        }

        bool full() const {
            return length() >= maxLength();
        }

        bool empty() const {
            return length() <= 0;
        }

        void clear() {
            m_buff[0] = '\0';
            m_length = 0;
        }

        virtual const char &operator[](psl::t_index i) const {
            return m_buff[i];
        }

        virtual char &operator[](psl::t_index i) {
            return m_buff[i];
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
                m_buff[m_length] = c;
                m_length++;
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
            char converter[32];
            snprintf(converter, 32, "%.2f", num);
            return append(converter);
        }

        bool remove(int i, int len = 1) {
            memmove(&m_buff[i], &m_buff[i + len], sizeof(char) * (m_length - i - len));
            m_length -= len;
            term();
            return true;
        }

        String& operator=(const String &other) {
            if(this == &other) {
                return *this;
            }
            clear();
            append(other);
            return *this;
        }

        SubString subString(int start, int len);

        void subString(String &output, int start, int len);

        int toInt() const {
            return atoi(c_str());
        }

        double toDouble() const {
            return atof(c_str());
        }
    };


    class SubString final : public String {
    private:
        char storedChar;
        int replaceLength;
    public:
        SubString(String &str, int start, int len) : String(&str[start], str.maxLength() - start, str.lengthRef()) {
            replaceLength = len;
            storedChar = (*this)[replaceLength];
            (*this)[replaceLength] = '\0';
        }

        ~SubString() {
            (*this)[replaceLength] = storedChar;
        }
    };

    template<unsigned maxLengthN>
    class StringStorage final : public String {
    private:
        char m_storage[maxLengthN + 1] = "";
        t_size m_length = 0;
    public:
        StringStorage() : String(m_storage, maxLengthN, m_length) {}
    };


    SubString String::subString(int start, int len) {
        return {*this, start, len};
    }

    void String::subString(String &output, int start, int len) {
        output = subString(start, len);
    }
}

#endif //PATRICK_STANDARD_LIB_STRING_H
