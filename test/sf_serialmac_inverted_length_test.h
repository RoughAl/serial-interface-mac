/**
 * @code
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions.==============
 * @endcode
 *
 * @copyright  STACKFORCE GmbH, Heitersheim, Germany, http://www.stackforce.de
 * @author     Adrian Antonana <adrian.antonana@stackforce.de>
 * @brief      Serial MAC unit tests class with enabled inverted length field
 */

#ifndef _SF_SERIALMAC_INV_LENGTH_TEST_H_
#define _SF_SERIALMAC_INV_LENGTH_TEST_H_

#include "sf_serialmac_test.h"

/**
 * This class is specialized for the inverted-length-field-enabled tests. The
 * class constructor sets the invertedLengthField attribute to true. In addition
 * the SetupHalBuffer() creates the full HAL buffer with the extra inverted
 * length field.
 *
 * The frames tested with this class follow this format:
 *
 *       +-------+--------+------------+------------------+--------+
 * FIELD | SYNC  | LENGTH | INV_LENGTH |      PAYLOAD     |   CRC  |
 *       +-------+--------+------------+------------------+--------+
 * SIZE  | 1Byte | 2Bytes |   2Bytes   | 65535Bytes (MAX) | 2Bytes |
 *       +-------+--------+------------+------------------+--------+
 */

class SerialMacInvertedLengthTest : public SerialMacTest {

    public:

        static const uint macHeaderFieldLength;
        static const uint macLengthFieldLength;

        SerialMacInvertedLengthTest();
        virtual ~SerialMacInvertedLengthTest();

        /**
         * Sets up the parent class headerBuffer with enabled inverted
         * length field for the given payload length.
         */
        void SetupFrameHeader(uint16_t payloadLength) override;
};

#endif
