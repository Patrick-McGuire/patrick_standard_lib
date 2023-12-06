#ifndef PATRICK_STANDARD_LIB_SUBSTRINGDEF_H
#define PATRICK_STANDARD_LIB_SUBSTRINGDEF_H

#include "psl.h"
#include "String.h"

namespace psl {

    Substr::Substr() : StringBase(&swapChar, m_lengthMemory, 0) {

    }

    Substr::Substr(StringBase &str, t_size start, t_size len) : m_lengthMemory(len), StringBase(str.rawBuffer() + start, m_lengthMemory, len) {
        // Insert null term
        swapIndex = len;
        swapChar = (*this)[swapIndex];
        (*this)[swapIndex] = '\0';
    }

    Substr::Substr(Substr &&other) noexcept: StringBase(other.rawBuffer(), m_lengthMemory, other.maxLength()) {
        m_lengthMemory = other.m_lengthMemory;
        swapIndex = other.swapIndex;
        swapChar = other.swapChar;
        other.transfer();
    }

    Substr &Substr::operator=(Substr &&other) noexcept {
        // Recreate the object
        release();
        reCreate(other.rawBuffer(), m_lengthMemory, other.maxLength());
        // Copy over
        m_lengthMemory = other.m_lengthMemory;
        swapIndex = other.swapIndex;
        swapChar = other.swapChar;
        other.transfer();
        // Return
        return *this;
    }

    Substr::~Substr() {
        release();
    }

    void Substr::transfer() {
        swapIndex = NOT_FOUND;
    }

    void Substr::release() {
        if (swapIndex != NOT_FOUND) {
            (*this)[swapIndex] = swapChar;
            transfer();
        }
    }

}
#endif //PATRICK_STANDARD_LIB_SUBSTRINGDEF_H
