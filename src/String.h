#ifndef PATRICK_STANDARD_LIB_String22_H
#define PATRICK_STANDARD_LIB_String22_H

#include "psl.h"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "Converter.h"


#define PSL_STRING_HIDE_DANGEROUS_METHODS using StringBase::rawBuffer; using StringBase::rawLength;
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

        t_size &rawLength() const {
            return m_length;
        }

        char *rawBuffer() const {
            return m_buff;
        }
    };

    template<unsigned N>
    class String : public StringBase {
    private:
        PSL_STRING_HIDE_DANGEROUS_METHODS
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

    class StringRef : public StringBase {
    private:
        PSL_STRING_HIDE_DANGEROUS_METHODS

    public:
        PSL_STRING_USE_ALL_METHODS

        template<unsigned N>
        StringRef(String<N> other) : StringBase(other.rawBuffer(), other.rawLength(), other.maxLength()) {}

        StringRef(const StringRef &other) = default;
    };


    class Substr : public StringBase {
    private:
        PSL_STRING_HIDE_DANGEROUS_METHODS
        t_size m_lengthMemory = 0;
        char swapChar = '\0';
        t_size swapIndex = NOT_FOUND;
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
}

#include "StringBaseDef.h"
#include "SubStringDef.h"


#endif //PATRICK_STANDARD_LIB_String22_H
