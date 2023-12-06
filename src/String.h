#ifndef PATRICK_STANDARD_LIB_String22_H
#define PATRICK_STANDARD_LIB_String22_H

#include "psl.h"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "Converter.h"


namespace psl {
    class Substr;

    class Substr2;

    class StringBase {
    protected:
        t_size m_maxLength;
        t_size &m_length;
        char *m_buff = nullptr;

        StringBase &reCreate(char *buff, t_size &length, t_size maxLength); // Allow for construction after the constructor

    public:
        // Hold constants
        enum : t_size {
            NOT_FOUND = -1,
        };


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
        bool operator==(const StringBase &other);
        bool operator==(const char *str);
        bool operator!=(const StringBase &other);
        bool operator!=(const char *str);
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
        SubstrData substr(int start, int len);
        Substr2 substr2(int start, int len);

        // Conversion
        bool toBool() const;
        int64_t toInt() const;
        double toDouble() const;

        char *rawBuffer() const {
            return m_buff;
        }

        t_size &rawLength() const {
            return m_length;
        }
    };

    template<unsigned N>
    class String : public StringBase {
    private:
        using StringBase::rawBuffer;
        using StringBase::rawLength;
        t_size m_lengthMemory = 0;
        char m_storage[N + 1] = "";
    public:
        String() : StringBase(m_storage, m_lengthMemory, N) {}

        template<typename T>
        String(const T &input) : StringBase(m_storage, m_lengthMemory, N) {
            append(input);
        }
    };

    class StringRef : public StringBase {
    private:
        using StringBase::rawBuffer;
        using StringBase::rawLength;
    public:
        template<unsigned N>
        StringRef(String<N> other) : StringBase(other.rawBuffer(), other.rawLength(), other.maxLength()) {}

        StringRef(const StringRef &other) = default;
    };

    class Substr : public StringBase {
    private:
        using StringBase::rawBuffer;
        using StringBase::rawLength;
        using StringBase::operator=;
        using StringBase::append;
        using StringBase::insert;
        using StringBase::remove;
        using StringBase::pop;
        using StringBase::clear;

        t_size m_lengthMemory = 0;
        char swapChar = '\0';
        t_size swapIndex = NOT_FOUND;
    public:
        inline void destroy() {
            if (swapIndex != NOT_FOUND) {
                (*this)[swapIndex] = swapChar;
            }
        }

    public:
        Substr(const Substr &) : StringBase(&swapChar, m_lengthMemory, 0) {}

        Substr() : StringBase(&swapChar, m_lengthMemory, 0) {}

        Substr &operator=(const Substr &) = delete;

        Substr(SubstrData &&data) : m_lengthMemory(data.len), StringBase(data.str.rawBuffer() + data.start, m_lengthMemory, data.len) {
            // Insert null term
            swapIndex = data.len;
            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';
        }

        Substr &operator=(SubstrData &&data) {
            // Recreate the object
            destroy();
            m_lengthMemory = data.len;
            reCreate(data.str.rawBuffer() + data.start, m_lengthMemory, data.len);
            // Insert null term
            swapIndex = data.len;
            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';

            // Return
            return *this;
        }

        ~Substr() {
            destroy();
        }
    };


    class Substr2 : public StringBase {
    private:
        using StringBase::rawBuffer;
        using StringBase::rawLength;
        using StringBase::operator=;
        using StringBase::append;
        using StringBase::insert;
        using StringBase::remove;
        using StringBase::pop;
        using StringBase::clear;
        using StringBase::substr2;

        t_size m_lengthMemory = 0;
        char swapChar = '\0';
        t_size swapIndex = NOT_FOUND;

    public:
        Substr2() : StringBase(&swapChar, m_lengthMemory, 0) {}

        Substr2(Substr2 &&other) noexcept: StringBase(other.rawBuffer(), m_lengthMemory, other.maxLength()) {
            std::cout << "Moving" << "\n";
            m_lengthMemory = other.m_lengthMemory;
            swapIndex = other.swapIndex;
            swapChar = other.swapChar;
            other.transfer();
        }

        Substr2(StringBase &str, t_size start, t_size len) : m_lengthMemory(len), StringBase(str.rawBuffer() + start, m_lengthMemory, len) {
            // Insert null term
            swapIndex = len;
            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';
        }

        void transfer() {
            swapIndex = NOT_FOUND;
        }

        void destroy() {
            if (swapIndex != NOT_FOUND) {
                (*this)[swapIndex] = swapChar;
                transfer();
            }
        }

        Substr2 &operator=(Substr2 &&other) noexcept {
            // Recreate the object
            destroy();
            m_lengthMemory = other.length();
            reCreate(other.rawBuffer(), m_lengthMemory, other.length());
            // Insert null term
            swapIndex = m_lengthMemory;
            swapChar = (*this)[swapIndex];
            (*this)[swapIndex] = '\0';
            other.transfer();
            // Return
            return *this;
        }

        Substr2 &operator=(const Substr2 &) = delete;
        Substr2(const Substr2 &) = delete;

        ~Substr2() {
            destroy();
        }
    };

}

#include "StringBaseDef.h"


#endif //PATRICK_STANDARD_LIB_String22_H
