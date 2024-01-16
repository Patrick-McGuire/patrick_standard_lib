#ifndef PATRICK_STANDARD_LIB_CSVPARSER_H
#define PATRICK_STANDARD_LIB_CSVPARSER_H

#include "psl.h"
#include "Array.h"
#include "String.h"

namespace psl {
    //    template<unsigned N>
    class CsvParser : StringRef {
        struct CsvNode {
            enum Types {
                T_NONE,
                T_STR,
                T_BOOL,
                T_DOUBLE,
                T_INT,
            };

            Types type = T_NONE;
            Substr str;

            union {
                bool boolVal;
                double doubleVal;
                int64_t intVal;
            };

            CsvNode() {
            }

            CsvNode &operator=(const CsvNode &other) {
                return *this;
            }
        };

        Array<CsvNode, 10> nodes;

        t_size endIndex() {
            t_size newLine = indexOf('\n');
            t_size carRtn = indexOf('\r');
            if (newLine == NOT_FOUND) newLine = m_length - 1;
            if (carRtn == NOT_FOUND) carRtn = m_length - 1;
            return min(newLine, carRtn);
        }

        void parseOne(Substr str) {
            std::cout << "b\n";
            nodes.append({});
            std::cout << "d\n";
            nodes[m_length - 1].type = CsvNode::T_STR;
            std::cout << "c\n";
            nodes[m_length - 1].str = std::move(str);
            std::cout << "dc\n";
        }

    public:
        template<unsigned N2>
        CsvParser(String<N2> &other) : StringRef(other) {
        }

        CsvParser(const StringRef &other) : StringRef(other) {
        }

        bool paresLine() {
            // Reset our array of nodes
            nodes.clear();
            // Find the maximum index to go to
            t_size end = endIndex();


            t_size lastCommaIndex = -1;
            t_size currentCommaIndex;
            for (; true; lastCommaIndex = currentCommaIndex) {
                // Get the index of the next comma
                currentCommaIndex = indexOf(',', lastCommaIndex + 1);
                if (currentCommaIndex == NOT_FOUND) {
                    currentCommaIndex = end;
                    break;
                }
                // Parse the thing
                parseOne(substr(lastCommaIndex + 1, currentCommaIndex - lastCommaIndex - 1));
                std::cout << "a\n";
            }
            return true;
        }

        void printNodes() {
            for (int i = 0; i < nodes.length(); i++) {
                std::cout << (const char *) nodes[i].str << "\n";
            }
        }
    };
}


#endif //PATRICK_STANDARD_LIB_CSVPARSER_H
