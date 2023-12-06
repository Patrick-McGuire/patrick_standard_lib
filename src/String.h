#ifndef PATRICK_STANDARD_LIB_String22_H
#define PATRICK_STANDARD_LIB_String22_H

#include "psl.h"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "Converter.h"
#include <utility>

#define PSL_STRING_USE_NON_CONST_METHODS \
using StringBase::operator=;\
using StringBase::append;\
using StringBase::insert;\
using StringBase::remove;\
using StringBase::pop;\
using StringBase::clear;\
using StringBase::substr;
#define PSL_STRING_USE_SUBSCRIPT_OPERATOR using StringBase::operator[];
#define PSL_STRING_USE_CONST_METHODS \
using StringBase::length;\
using StringBase::maxLength;\
using StringBase::availableLength;\
using StringBase::validIndex;\
using StringBase::full;\
using StringBase::empty;\
using StringBase::c_str;\
using StringBase::operator==;\
using StringBase::operator!=;\
using StringBase::endsWith;\
using StringBase::startsWith;\
using StringBase::indexOf;\
using StringBase::count;\
using StringBase::toBool;\
using StringBase::toInt;\
using StringBase::toDouble;
#define PSL_STRING_USE_ALL_METHODS \
PSL_STRING_USE_SUBSCRIPT_OPERATOR  \
PSL_STRING_USE_CONST_METHODS       \
PSL_STRING_USE_NON_CONST_METHODS

namespace psl {
    class Substr;

    class StringBase {
    protected:
        friend class Substr;

        friend class StringRef;

        friend class ConstStringRef;

        template<unsigned N> friend
        class String;

        t_size m_maxLength;
        t_size &m_length;
        char *m_buff = nullptr;

        StringBase &reCreate(char *buff, t_size &length, t_size maxLength); // Allow for construction after the constructor

        // Constructors
        StringBase(const StringBase &other) = default;
        StringBase(char *buff, t_size &length, t_size maxLength);

        // Capacity
        t_size length() const;
        t_size maxLength() const;
        t_size availableLength() const;
        bool validIndex(t_size i) const;
        bool full() const;
        bool empty() const;

        // Access
        char &operator[](t_size i);
        const char &operator[](t_size i) const;
        const char *c_str() const;

        // Checking
        bool operator==(const StringBase &other) const;
        bool operator==(const char *str) const;
        bool operator!=(const StringBase &other) const;
        bool operator!=(const char *str) const;
        bool endsWith(const StringBase &other) const;
        bool endsWith(const char *str) const;
        bool endsWith(char c) const;
        bool startsWith(const StringBase &other) const;
        bool startsWith(const char *str) const;
        bool startsWith(char c) const;

        // Modifiers
        bool append(const StringBase &other);
        bool append(const char *str);
        bool append(const char *str, int n);
        bool append(char c);
        bool append(int num);
        bool append(double num, int digits = 4);
        bool insert(t_size i, const StringBase &other);
        bool insert(t_size i, const char *str);
        bool insert(t_size i, char c);
        bool remove(int i, int len = 1);
        bool pop();
        void clear();

        // Assigment
        StringBase &operator=(const StringBase &other);     // Must define the copy assigment operator explicitly
        template<typename T>
        StringBase &operator=(const T &other);

        // Location
        t_size indexOf(const StringBase &other, int i = 0) const;
        t_size indexOf(const char *str, int i = 0) const;
        t_size indexOf(char c, int i = 0) const;
        t_size count(const StringBase &other) const;
        t_size count(const char *str) const;
        t_size count(char c) const;

        // Splitting and substring
        void substr(StringBase &destination, int start, int len) const;
        Substr substr(int start, int len);

        // Conversion
        bool toBool() const;
        int64_t toInt() const;
        double toDouble() const;

    public:
        // Hold constants
        enum : t_size {
            NOT_FOUND = -1,
        };
    };

    template<unsigned N>
    class String : public StringBase {
    private:
        t_size m_lengthMemory = 0;
        char m_storage[N + 1] = "";

    public:
        PSL_STRING_USE_ALL_METHODS

        String() : StringBase(m_storage, m_lengthMemory, N) {}

        template<typename T>
        String(const T &input) : StringBase(m_storage, m_lengthMemory, N) {
            append(input);
        }
    };


    class Substr : public StringBase {
    private:
        t_size m_lengthMemory = 0;
        char m_swapChar = '\0';
        t_size m_swapIndex = NOT_FOUND;
        void transfer();

    public:
        PSL_STRING_USE_CONST_METHODS
        PSL_STRING_USE_SUBSCRIPT_OPERATOR

        Substr();
        Substr(Substr &&other) noexcept;
        Substr(StringBase &str, t_size start, t_size len);
        Substr &operator=(Substr &&other) noexcept;
        ~Substr();
        Substr &operator=(const Substr &) = delete;
        Substr(const Substr &) = delete;
        void release();
    };

    class StringRef : public StringBase {
    public:
        PSL_STRING_USE_ALL_METHODS

        template<unsigned N>
        StringRef(String<N> &other) : StringBase(other.m_buff, other.m_length, other.m_maxLength) {}

        StringRef(const StringRef &other) = default;
    };

    class ConstStringRef : public StringBase {
    public:
        PSL_STRING_USE_CONST_METHODS

        template<unsigned N>
        ConstStringRef(String<N> &other) : StringBase(other.m_buff, other.m_length, other.m_maxLength) {}

        ConstStringRef(StringBase &other) : StringBase(other.m_buff, other.m_length, other.m_maxLength) {}

        ConstStringRef(const ConstStringRef &other) = default;
    };
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        StringBase Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {
    StringBase::StringBase(char *buff, t_size &length, t_size maxLength) : m_buff(buff), m_length(length), m_maxLength(maxLength) {}

    t_size StringBase::length() const {
        return m_length;
    }

    t_size StringBase::maxLength() const {
        return m_maxLength;
    }

    t_size StringBase::availableLength() const {
        return m_maxLength - m_length;
    }

    bool StringBase::validIndex(t_size i) const {
        return i >= 0 && i < m_length;
    }

    bool StringBase::full() const {
        return m_length == m_maxLength;
    }

    bool StringBase::empty() const {
        return m_length == 0;
    }

    char &StringBase::operator[](t_size i) {
        // Handle negative incidences
        if (i < 0) i = m_length + i;
        // Check for valid index
        return validIndex(i) ? m_buff[i] : m_buff[m_length];
    }

    const char &StringBase::operator[](t_size i) const {
        // Handle negative incidences
        if (i < 0) i = m_length + i;
        // Check for valid index
        return validIndex(i) ? m_buff[i] : m_buff[m_length];
    }

    const char *StringBase::c_str() const {
        return m_buff;
    }

    bool StringBase::operator==(const StringBase &other) const {
        if (m_length == other.m_length) {
            for (int i = 0; i < m_length; i++)
                if (m_buff[i] != other.m_buff[i])
                    return false;
            return true;
        }
        return false;
    }

    bool StringBase::operator==(const char *str) const {
        if (m_length == strlen(str)) {
            for (int i = 0; i < m_length; i++)
                if (m_buff[i] != str[i])
                    return false;
            return true;
        }
        return false;
    }

    bool StringBase::operator!=(const StringBase &other) const {
        return !operator==(other);
    }

    bool StringBase::operator!=(const char *str) const {
        return !operator==(str);
    }

    bool StringBase::endsWith(const StringBase &other) const {
        // @todo check
        if (m_length < other.m_length || !m_buff || !other.m_buff) return false;
        return strcmp(&m_buff[m_length - other.m_length], other.m_buff) == 0;
    }

    bool StringBase::endsWith(const char *str) const {
        // @todo check
        t_size otherLength = strlen(str);
        if (m_length < otherLength || !m_buff || !str) return false;
        return strcmp(&m_buff[m_length - otherLength], str) == 0;
    }

    bool StringBase::endsWith(char c) const {
        return !empty() && m_buff[m_length - 1] == c;
    }

    bool StringBase::startsWith(const StringBase &other) const {
        if (m_length >= other.m_length) {
            for (int i = 0; i < other.m_length; i++)
                if (m_buff[i] != other.m_buff[i])
                    return false;
            return true;
        }
        return false;
    }

    bool StringBase::startsWith(const char *str) const {
        for (int i = 0; i < m_length; i++) {
            if (str[i] == '\0' || m_buff[i] != str[i])
                return false;
        }
        return true;
    }

    bool StringBase::startsWith(char c) const {
        return m_buff[0] == c;
    }

    bool StringBase::append(const StringBase &other) {
        const char *str = other.m_buff;
        for (const char *end = str + other.m_length; !full() && str != end; str++)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    bool StringBase::append(const char *str) {
        for (const char *end = str + strlen(str); !full() && str != end; str++)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    bool StringBase::append(const char *str, int n) {
        for (const char *end = str + strlen(str); !full() && str != end && n > 0; str++, n--)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    bool StringBase::append(char c) {
        if (!full()) {
            m_buff[m_length++] = c;
            m_buff[m_length] = '\0';
            return true;
        }
        return false;
    }

    bool StringBase::append(int num) {
        char converter[16];
        itoa(num, converter, 10);
        return append(converter);
        // @todo check
    }

    bool StringBase::append(double num, int digits) {
        if (availableLength() > 0) {
            doubleToString(num, &m_buff[length()], digits, availableLength());
            return true;
        }
        return false;
        // @todo check
    }

    bool StringBase::insert(t_size i, const StringBase &other) {
        // @todo make work better if &other == this
        t_size length = other.m_length;
        const char *str = other.m_buff;
        if (availableLength() >= length) {
            // Check for overlap
            // If there's an overlap, perform the move in the reverse direction
            memmove(&m_buff[i + length], &m_buff[i], sizeof(char) * (m_length - i));
            for (int j = length - 1; j >= 0; --j) {
                m_buff[i + j] = str[j];
            }

            m_length += length;
            m_buff[m_length] = '\0';
            return true;
        }

        return false;
    }

    bool StringBase::insert(t_size i, const char *str) {
        t_size length = strlen(str);

        if (availableLength() >= length) {
            // Check for overlap
            // If there's an overlap, perform the move in the reverse direction
            memmove(&m_buff[i + length], &m_buff[i], sizeof(char) * (m_length - i));
            for (int j = length - 1; j >= 0; --j) {
                m_buff[i + j] = str[j];
            }

            m_length += length;
            m_buff[m_length] = '\0';
            return true;
        }

        return false;
    }

    bool StringBase::insert(t_size i, char c) {
        if (!full()) {
            memmove(&m_buff[i + 1], &m_buff[i], sizeof(char) * (m_length - i));
            m_buff[i] = c;
            m_buff[++m_length] = '\0';
            return true;
        }
        return false;
    }

    bool StringBase::remove(int i, int len) {
        if (m_length - i >= len) {
            memmove(&m_buff[i], &m_buff[i + len], sizeof(char) * (m_length - i - len));
            m_length -= len;
            m_buff[m_length] = '\0';
            return true;
        }
        return false;
    }

    bool StringBase::pop() {
        if (!empty()) {
            m_buff[--m_length] = '\0';
            return true;
        }
        return false;
    }

    void StringBase::clear() {
        m_length = 0;
        m_buff[0] = '\0';
    }

    StringBase &StringBase::operator=(const StringBase &other) {
        if (&other != this) {
            clear();
            append(other);
        }
        return *this;
    }

    template<typename T>
    StringBase &StringBase::operator=(const T &other) {
        clear();
        append(other);
        return *this;
    }

    t_size StringBase::indexOf(const StringBase &other, int i) const {
        return indexOf(other.m_buff, i);
    }

    t_size StringBase::indexOf(const char *str, int i) const {
        char *ptr = m_buff;
        while (i-- > 0 && (ptr = strstr(ptr, str)) != nullptr);
        return ptr != nullptr ? (t_size) (ptr - m_buff) : NOT_FOUND;
    }

    t_size StringBase::indexOf(char c, int i) const {
        if (i >= 0) {
            for (t_size j = 0; j < m_length; ++j) {
                if (m_buff[j] == c)
                    if (i-- == 0)
                        return j;
            }
        }
        return NOT_FOUND;
    }

    t_size StringBase::count(const StringBase &other) const {
        t_size count = 0;
        // Iterate through the string
        for (t_size i = 0; i <= m_length - other.m_length; ++i) {
            // Check if the substring matches
            bool match = true;
            for (t_size j = 0; j < other.m_length; ++j) {
                if (m_buff[i + j] != other.m_buff[j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                count++;
            }
        }
        return count;
    }

    t_size StringBase::count(const char *str) const {
        t_size count = 0;
        auto strLen = (t_size) strlen(str);

        // Iterate through the string
        for (t_size i = 0; i <= m_length - strLen; ++i) {
            // Check if the substring matches
            bool match = true;
            for (t_size j = 0; j < strLen; ++j) {
                if (m_buff[i + j] != str[j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                count++;
            }
        }
        return count;
    }

    t_size StringBase::count(char c) const {
        t_size count = 0;
        for (char *str = m_buff; *str != '\0'; str++) {
            if (*str == c) {
                count++;
            }
        }
        return count;
    }

    bool StringBase::toBool() const {
        return toInt() != 0;
    }

    int64_t StringBase::toInt() const {
        return atoi(m_buff);
    }

    double StringBase::toDouble() const {
        return stringToDouble(m_buff);
    }

    StringBase &StringBase::reCreate(char *buff, t_size &length, t_size maxLength) {
        m_buff = buff;
        m_length = length;
        m_maxLength = maxLength;
        return *this;
    }

    void StringBase::substr(StringBase &destination, int start, int len) const {
        destination.clear();
        destination.append(m_buff + start, len);
    }

    Substr StringBase::substr(int start, int len) {
        return {*this, start, len};
    }


}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        Substr Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {

    Substr::Substr() : StringBase(&m_swapChar, m_lengthMemory, 0) {

    }

    Substr::Substr(StringBase &str, t_size start, t_size len) : m_lengthMemory(len), StringBase(str.m_buff + start, m_lengthMemory, len) {
        // Insert null term
        m_swapIndex = len;
        m_swapChar = (*this)[m_swapIndex];
        (*this)[m_swapIndex] = '\0';
    }

    Substr::Substr(Substr &&other) noexcept: StringBase(other.m_buff, m_lengthMemory, other.maxLength()) {
        m_lengthMemory = other.m_lengthMemory;
        m_swapIndex = other.m_swapIndex;
        m_swapChar = other.m_swapChar;
        other.transfer();
    }

    Substr &Substr::operator=(Substr &&other) noexcept {
        // Recreate the object
        release();
        reCreate(other.m_buff, m_lengthMemory, other.maxLength());
        // Copy over
        m_lengthMemory = other.m_lengthMemory;
        m_swapIndex = other.m_swapIndex;
        m_swapChar = other.m_swapChar;
        other.transfer();
        // Return
        return *this;
    }

    Substr::~Substr() {
        release();
    }

    void Substr::transfer() {
        m_swapIndex = NOT_FOUND;
    }

    void Substr::release() {
        if (m_swapIndex != NOT_FOUND) {
            (*this)[m_swapIndex] = m_swapChar;
            transfer();
        }
    }

}

#endif //PATRICK_STANDARD_LIB_String22_H
