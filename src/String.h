#ifndef PATRICK_STANDARD_LIB_String22_H
#define PATRICK_STANDARD_LIB_String22_H

#include "psl.h"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "Converter.h"


namespace psl {
    class StringBase {
    protected:
        const t_size m_maxLength;
        t_size &m_length;
        char *m_buff = nullptr;

    public:
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
        char m_storage[N + 1] = "";
        t_size m_lengthMemory = 0;
    public:
        String() : StringBase(m_storage, m_lengthMemory, N) {}

        template<typename T>
        String(const T &input) : StringBase(m_storage, m_lengthMemory, N) {
            append(input);
        }
    };

    class StringRef : public StringBase {
    public:
        template<unsigned N>
        StringRef(String<N> other) : StringBase(other.rawBuffer(), other.rawLength(), other.maxLength()) {}

        StringRef(const StringRef &other) = default;
    };

}


#include "StringDef.h"


#endif //PATRICK_STANDARD_LIB_String22_H
