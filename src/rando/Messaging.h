#ifndef PATRICK_STANDARD_LIB_MESSAGING_H
#define PATRICK_STANDARD_LIB_MESSAGING_H

#include "../psl.h"
#include "iostream"
#include "cstdint"

// Message characteristics
#ifndef PSL_MESSAGE_ID_SIZE
#define PSL_MESSAGE_ID_SIZE 1
#endif
#ifndef PSL_MESSAGE_LEN_SIZE
#define PSL_MESSAGE_LEN_SIZE 1
#endif
#ifndef PSL_MESSAGE_CHECKSUM_SIZE
#define PSL_MESSAGE_CHECKSUM_SIZE 2
#endif

namespace psl {
    struct SerializedMessage {
        const uint8_t *buff;
        const t_size length;

        SerializedMessage(const uint8_t *buffy, t_size lengthy) : buff(buffy), length(lengthy) {}
    };

    template<class Format, typename... Args>
    class CustomMessage : public SerializedMessage, public Format {
    public:
        typedef typename Format::CHECKSUM ChecksumType;
        static constexpr t_size ID_SIZE = sizeof(typename Format::ID_TYPE);
        static constexpr t_size LENGTH_SIZE = sizeof(typename Format::LENGTH);
        static constexpr t_size PREAMBLE_SIZE = ID_SIZE + LENGTH_SIZE;
        static constexpr t_size CHECKSUM_SIZE = sizeof(typename Format::CHECKSUM);
        static constexpr t_size LENGTH = compile_time_sum<PREAMBLE_SIZE, CHECKSUM_SIZE, sizeof(Args)...>::value;
        static constexpr t_size ARGUMENT_NUM = sizeof...(Args);

        uint8_t elements[LENGTH] = {0};
        void *elementPointers[ARGUMENT_NUM] = {nullptr};

        template<typename First>
        int createMessage(uint8_t *pos, First first) {
            memcpy(pos, &first, sizeof(First));
            return sizeof(First);
        }

        template<typename First, typename Second, typename... Args2>
        int createMessage(uint8_t *pos, First first, Second second, Args2...args) {
            return createMessage<Second, Args2...>(pos + createMessage(pos, first), second, args ...);
        }

        template<typename First>
        void generateElementPointers(uint8_t **pos, int i) {
            elementPointers[i] = *pos;
            *pos += sizeof(First);
        }

        template<typename First, typename Second, typename... Args2>
        void generateElementPointers(uint8_t **pos, int i) {
            generateElementPointers<First>(pos, i);
            return generateElementPointers<Second, Args2...>(pos, i + 1);
        }

        void generateElementPointers() {
            uint8_t *pos = &elements[0] + PREAMBLE_SIZE;
            generateElementPointers<Args...>(&pos, 0);
        }

        void init() {
            generateElementPointers();
            *((typename Format::ID_TYPE *) (&elements[0])) = Format::ID;
            *((typename Format::ID_TYPE *) (&elements[0] + ID_SIZE)) = LENGTH;
        }

        void calculateCRC() {
            *((ChecksumType *) (&elements[0] + LENGTH - CHECKSUM_SIZE)) = crc(&elements[0], LENGTH - CHECKSUM_SIZE);
        }

    public:
        static ChecksumType crc(const uint8_t *buffer, t_size len) {
            ChecksumType crc = 0;
            for (int i = 0; i < len; i++) {
                crc ^= buffer[i];
            }
            return crc;
        }

        CustomMessage() : SerializedMessage(elements, LENGTH) {
            init();
        }

        explicit CustomMessage(Args...args) : SerializedMessage(elements, LENGTH) {
            init();
            set(args...);
        }

        CustomMessage(const uint8_t *otherBuff, t_size otherLength) : SerializedMessage(elements, LENGTH) {
            init();
            parse(otherBuff, otherLength);
        }

        static bool verifyCRC(const uint8_t *otherBuff) {
            return crc(otherBuff, LENGTH - CHECKSUM_SIZE) == *((ChecksumType *) (&otherBuff[0] + LENGTH - CHECKSUM_SIZE));
        }

        static bool verifyType(const uint8_t *otherBuff) {
            return *((typename Format::ID_TYPE *) otherBuff) == Format::ID;
        }

        bool parse(const uint8_t *otherBuff, t_size otherLength) {
            if (otherLength >= LENGTH && verifyType(otherBuff) && verifyCRC(otherBuff)) {
                memcpy(&elements[0], otherBuff, LENGTH);
                return true;
            }
            std::cout << "Message invalid" << "\n";
            return false;
        }

        void set(Args...args) {
            createMessage<Args...>(&elements[0] + PREAMBLE_SIZE, args...);
            calculateCRC();
        }

        template<unsigned N>
        void set(typename variadic_nth_type<N, Args...>::type val) {
            *((typename variadic_nth_type<N, Args...>::type *) (elementPointers[N])) = val;
            calculateCRC();
        }

        template<unsigned N>
        typename variadic_nth_type<N, Args...>::type get() {
            return *((typename variadic_nth_type<N, Args...>::type *) (elementPointers[N]));
        }
    };

    struct StandardMessageFormat {
        using ID_TYPE = uint8_t;
        using LENGTH = uint8_t;
        using CHECKSUM = uint16_t;
    };

    struct GpsPosFormat : StandardMessageFormat {
        static constexpr uint8_t ID = 13;
        enum {
            TIME = 0,
            LAT = 1,
            LON = 2,
            ALT = 3,
            SATS = 4,
        };
    };

    typedef CustomMessage<GpsPosFormat, uint64_t, double, double, float, uint8_t, uint8_t> GpsPosMsg;

    struct NineAxisFormat : StandardMessageFormat {
        static constexpr uint8_t ID = 13;
        enum {
            MAG_X = 0,
            MAG_Y = 1,
            MAG_Z = 2,
            IMU_X = 3,
            IMU_Y = 4,
            IMU_Z = 5,
            ACCEL_X = 6,
            ACCEL_Y = 7,
            ACCEL_Z = 8,
        };
    };

    typedef CustomMessage<NineAxisFormat, float, float, float, float, float, float, float, float, float> NineAxisMsg;
}

#endif //PATRICK_STANDARD_LIB_MESSAGING_H
