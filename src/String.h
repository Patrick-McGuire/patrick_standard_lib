#ifndef PATRICK_STANDARD_LIB_String_H
#define PATRICK_STANDARD_LIB_String_H

#include "psl.h"
#include <cstring>
#include <cstdint>
#include "Converter.h"

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
using StringBase::operator const char *;\
using StringBase::operator==;\
using StringBase::operator!=;\
using StringBase::endsWith;\
using StringBase::startsWith;\
using StringBase::nthIndexOf;\
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

        friend class ConstStringRef;

        template<unsigned N>
        friend
        class String;

        t_size m_maxLength;
        t_size &m_length;
        char *m_buff = nullptr;

        StringBase &reCreate(char *buff, const t_size &length, t_size maxLength);

        // Allow for construction after the constructor

        // Constructors
        StringBase(const StringBase &other) = default;

        StringBase(char *buff, t_size &length, t_size maxLength);

        // Capacity
        inline t_size length() const;

        inline t_size maxLength() const;

        inline t_size availableLength() const;

        inline bool validIndex(t_size i) const;

        inline bool full() const;

        inline bool empty() const;

        // Access
        char &operator[](t_size i);

        const char &operator[](t_size i) const;

        const char *c_str() const;

        operator const char *() const;

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

        bool append(int num, uint8_t base = 10);

        bool append(int64_t num, uint8_t base = 10);

        bool append(uint64_t num, uint8_t base = 10);

        bool append(double num, int digits = 4, uint8_t base = 10);

        bool insert(t_size i, const StringBase &other);

        bool insert(t_size i, const char *str);

        bool insert(t_size i, char c);

        bool remove(int i, int len = 1);

        bool pop();

        void clear();

        // Assigment
        StringBase &operator=(const StringBase &other); // Must define the copy assigment operator explicitly
        template<typename T>
        StringBase &operator=(const T &other);

        // Location
        t_size nthIndexOf(const StringBase &other, int i = 0) const;

        t_size nthIndexOf(const char *str, int i = 0) const;

        t_size nthIndexOf(char c, int i = 0) const;

        t_size indexOf(char c, int i = 0) const;

        t_size count(const StringBase &other) const;

        t_size count(const char *str) const;

        t_size count(char c) const;

        // Splitting and substring
        void substr(StringBase &destination, int start, int len) const;

        Substr substr(int start, int len);

        // Conversion
        bool toBool(bool *success = nullptr) const;

        int toInt(bool *success = nullptr, uint8_t base = 10) const;

        int64_t toInt64(bool *success = nullptr, uint8_t base = 10) const;

        uint64_t toUInt64(bool *success = nullptr, uint8_t base = 10) const;

        double toDouble(bool *success = nullptr, uint8_t base = 10) const;

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

        String() : StringBase(m_storage, m_lengthMemory, N) {
        }

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

        Substr(const StringBase &str, t_size start, t_size len);

        Substr &operator=(Substr &&other) noexcept;

        ~Substr();

        Substr &operator=(const Substr &) = delete;

        Substr(const Substr &) = delete;

        void release();
    };

    class ConstStringRef : public StringBase {
    public:
        PSL_STRING_USE_CONST_METHODS

        ConstStringRef(const StringBase &other) : StringBase(other.m_buff, other.m_length, other.m_maxLength) {
        }

        ConstStringRef(const ConstStringRef &other) = default;
    };

    class StringRef : public ConstStringRef {
    public:
        PSL_STRING_USE_ALL_METHODS

        template<unsigned N>
        StringRef(String<N> &other) : ConstStringRef(other) {
        }

        StringRef(const StringRef &other) = default;
    };
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        StringBase Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {
    inline StringBase::StringBase(char *buff, t_size &length, const t_size maxLength) : m_maxLength(maxLength), m_length(length), m_buff(buff) {
    }

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

    inline char &StringBase::operator[](t_size i) {
        // Handle negative incidences
        if (i < 0) i = m_length + i;
        // Check for valid index
        return validIndex(i) ? m_buff[i] : m_buff[m_length];
    }

    inline const char &StringBase::operator[](t_size i) const {
        // Handle negative incidences
        if (i < 0) i = m_length + i;
        // Check for valid index
        return validIndex(i) ? m_buff[i] : m_buff[m_length];
    }

    inline const char *StringBase::c_str() const {
        return m_buff;
    }

    inline StringBase::operator const char *() const {
        return m_buff;
    }

    inline bool StringBase::operator==(const StringBase &other) const {
        if (m_length == other.m_length) {
            for (int i = 0; i < m_length; i++)
                if (m_buff[i] != other.m_buff[i])
                    return false;
            return true;
        }
        return false;
    }

    inline bool StringBase::operator==(const char *str) const {
        if (m_length == strlen(str)) {
            for (int i = 0; i < m_length; i++)
                if (m_buff[i] != str[i])
                    return false;
            return true;
        }
        return false;
    }

    inline bool StringBase::operator!=(const StringBase &other) const {
        return !operator==(other);
    }

    inline bool StringBase::operator!=(const char *str) const {
        return !operator==(str);
    }

    inline bool StringBase::endsWith(const StringBase &other) const {
        // @todo check
        if (m_length < other.m_length || !m_buff || !other.m_buff) return false;
        return strcmp(&m_buff[m_length - other.m_length], other.m_buff) == 0;
    }

    inline bool StringBase::endsWith(const char *str) const {
        // @todo check
        const auto otherLength = t_size(strlen(str));
        if (m_length < otherLength || !m_buff || !str) return false;
        return strcmp(&m_buff[m_length - otherLength], str) == 0;
    }

    inline bool StringBase::endsWith(char c) const {
        return !empty() && m_buff[m_length - 1] == c;
    }

    inline bool StringBase::startsWith(const StringBase &other) const {
        if (m_length >= other.m_length) {
            for (int i = 0; i < other.m_length; i++)
                if (m_buff[i] != other.m_buff[i])
                    return false;
            return true;
        }
        return false;
    }

    inline bool StringBase::startsWith(const char *str) const {
        for (int i = 0; i < m_length; i++) {
            if (str[i] == '\0' || m_buff[i] != str[i])
                return false;
        }
        return true;
    }

    inline bool StringBase::startsWith(const char c) const {
        return m_buff[0] == c;
    }

    inline bool StringBase::append(const StringBase &other) {
        const char *str = other.m_buff;
        for (const char *end = str + other.m_length; !full() && str != end; str++)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    inline bool StringBase::append(const char *str) {
        for (const char *end = str + strlen(str); !full() && str != end; str++)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    inline bool StringBase::append(const char *str, int n) {
        for (const char *end = str + strlen(str); !full() && str != end && n > 0; str++, n--)
            m_buff[m_length++] = *str;
        m_buff[m_length] = '\0';
        return *str == '\0';
    }

    inline bool StringBase::append(char c) {
        if (!full()) {
            m_buff[m_length++] = c;
            m_buff[m_length] = '\0';
            return true;
        }
        return false;
    }

    inline bool StringBase::append(const int num, const uint8_t base) {
        if (full()) return false;
        const t_size status = integerToString(num, m_buff + m_length, availableLength(), base);
        if (status < 0) {
            m_buff[m_length] = '\0';
            return false;
        }
        m_length += status;
        return true;
    }

    inline bool StringBase::append(const int64_t num, const uint8_t base) {
        if (full()) return false;
        const t_size status = integerToString(num, m_buff + m_length, availableLength(), base);
        if (status < 0) {
            m_buff[m_length] = '\0';
            return false;
        }
        m_length += status;
        return true;
    }

    inline bool StringBase::append(const uint64_t num, const uint8_t base) {
        if (full()) return false;
        const t_size status = integerToString(num, m_buff + m_length, availableLength(), base);
        if (status < 0) {
            m_buff[m_length] = '\0';
            return false;
        }
        m_length += status;
        return true;
    }

    inline bool StringBase::append(const double num, const int digits, const uint8_t base) {
        if (full()) return false;
        const t_size status = doubleToString(num, m_buff + m_length, digits, availableLength(), base);
        if (status < 0) {
            m_buff[m_length] = '\0';
            return false;
        }
        m_length += status;
        return true;
    }

    inline bool StringBase::insert(const t_size i, const StringBase &other) {
        // @todo make work better if &other == this
        const t_size length = other.m_length;
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

    inline bool StringBase::insert(const t_size i, const char *str) {
        const auto length = t_size(strlen(str));

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

    inline bool StringBase::insert(t_size i, char c) {
        if (!full()) {
            memmove(&m_buff[i + 1], &m_buff[i], sizeof(char) * (m_length - i));
            m_buff[i] = c;
            m_buff[++m_length] = '\0';
            return true;
        }
        return false;
    }

    inline bool StringBase::remove(int i, int len) {
        if (m_length - i >= len) {
            memmove(&m_buff[i], &m_buff[i + len], sizeof(char) * (m_length - i - len));
            m_length -= len;
            m_buff[m_length] = '\0';
            return true;
        }
        return false;
    }

    inline bool StringBase::pop() {
        if (!empty()) {
            m_buff[--m_length] = '\0';
            return true;
        }
        return false;
    }

    inline void StringBase::clear() {
        m_length = 0;
        m_buff[0] = '\0';
    }

    inline StringBase &StringBase::operator=(const StringBase &other) {
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

    inline t_size StringBase::nthIndexOf(const StringBase &other, int i) const {
        return nthIndexOf(other.m_buff, i);
    }

    inline t_size StringBase::nthIndexOf(const char *str, int i) const {
        const char *ptr = m_buff;
        while ((ptr = strstr(ptr, str)) != nullptr)
            return ptr != nullptr ? (t_size) (ptr - m_buff) : NOT_FOUND;
        return NOT_FOUND;
        // @todo check
    }

    inline t_size StringBase::nthIndexOf(char c, int i) const {
        if (i >= 0) {
            for (t_size j = 0; j < m_length; ++j) {
                if (m_buff[j] == c)
                    if (i-- == 0)
                        return j;
            }
        }
        return NOT_FOUND;
    }

    inline t_size StringBase::indexOf(char c, int i) const {
        for (; validIndex(i); i++) {
            if (m_buff[i] == c) {
                return i;
            }
        }
        return NOT_FOUND;
    }

    inline t_size StringBase::count(const StringBase &other) const {
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

    inline t_size StringBase::count(const char *str) const {
        t_size count = 0;
        const auto strLen = (t_size) strlen(str);

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

    inline t_size StringBase::count(char c) const {
        t_size count = 0;
        for (const char *str = m_buff; *str != '\0'; str++) {
            if (*str == c) {
                count++;
            }
        }
        return count;
    }

    inline bool StringBase::toBool(bool *success) const {
        // Check for an integer
        bool successInternal;
        const int val = int(toInt(&successInternal));
        if (successInternal) {
            if (success != nullptr) *success = true;
            return val != 0;
        }
        // Check for true/false string
        if (*this == "true") {
            if (success != nullptr) *success = true;
            return true;
        }
        if (*this == "false") {
            if (success != nullptr) *success = true;
            return false;
        }
        if (success != nullptr) *success = false;
        return false;
    }

    inline int StringBase::toInt(bool *success, uint8_t base) const {
        return stringToInteger<int>(m_buff, success, base);
    }

    inline int64_t StringBase::toInt64(bool *success, uint8_t base) const {
        return stringToInteger<int64_t>(m_buff, success, base);
    }

    inline uint64_t StringBase::toUInt64(bool *success, uint8_t base) const {
        return stringToInteger<uint64_t>(m_buff, success, base);
    }

    //Julia was here
    inline double StringBase::toDouble(bool *success, uint8_t base) const {
        return stringToDouble(m_buff, success, base);
    }

    inline StringBase &StringBase::reCreate(char *buff, const t_size &length, const t_size maxLength) {
        m_buff = buff;
        m_length = length;
        m_maxLength = maxLength;
        return *this;
    }

    inline void StringBase::substr(StringBase &destination, int start, int len) const {
        destination.clear();
        destination.append(m_buff + start, len);
    }

    inline Substr StringBase::substr(int start, int len) {
        return {*this, start, len};
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        Substr Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {
    inline Substr::Substr() : StringBase(&m_swapChar, m_lengthMemory, 0) {
    }

    inline Substr::Substr(const StringBase &str, const t_size start, const t_size len) : StringBase(str.m_buff + start, m_lengthMemory, len), m_lengthMemory(len) {
        // Insert null term
        m_swapIndex = len;
        m_swapChar = (*this)[m_swapIndex];
        (*this)[m_swapIndex] = '\0';
    }

    inline Substr::Substr(Substr &&other) noexcept: StringBase(other.m_buff, m_lengthMemory, other.maxLength()) {
        m_lengthMemory = other.m_lengthMemory;
        m_swapIndex = other.m_swapIndex;
        m_swapChar = other.m_swapChar;
        other.transfer();
    }

    inline Substr &Substr::operator=(Substr &&other) noexcept {
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

    inline Substr::~Substr() {
        release();
    }

    inline void Substr::transfer() {
        if (m_buff != &m_swapChar) {
            m_lengthMemory = 0;
            m_swapChar = '\0';
            m_swapIndex = NOT_FOUND;
            reCreate(&m_swapChar, m_lengthMemory, 0);
        }
    }

    inline void Substr::release() {
        if (m_buff != &m_swapChar) {
            if (m_swapIndex != NOT_FOUND) {
                (*this)[m_swapIndex] = m_swapChar;
                transfer();
            }
        }
    }


    inline void string_test() {
    }
}


#endif //PATRICK_STANDARD_LIB_String_H
