#ifndef PATRICK_STANDARD_LIB_SUBSTRINGDEF_H
#define PATRICK_STANDARD_LIB_SUBSTRINGDEF_H

#include "psl.h"
#include "String.h"

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
#endif //PATRICK_STANDARD_LIB_SUBSTRINGDEF_H
