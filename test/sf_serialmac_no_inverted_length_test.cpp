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
 * @brief      Serial MAC unit tests class with disabled inverted length field
 */

#include "sf_serialmac_no_inverted_length_test.h"
#include "sf_crc.h"

const uint SerialMacNoInvertedLengthTest::macHeaderFieldLength = SF_SERIALMAC_PROTOCOL_HEADER_LEN;
const uint SerialMacNoInvertedLengthTest::macLengthFieldLength = SF_SERIALMAC_PROTOCOL_HEADER_LEN - macSyncWordFieldLength;

SerialMacNoInvertedLengthTest::SerialMacNoInvertedLengthTest() {
    invertedLengthField = false;
    headerBuffer = (uint8_t*)std::malloc(macHeaderFieldLength);
}

SerialMacNoInvertedLengthTest::~SerialMacNoInvertedLengthTest() {
    std::free(headerBuffer);
}

void SerialMacNoInvertedLengthTest::SetupFrameHeader(uint16_t payloadLength) {
    headerBuffer[0] = SF_SERIALMAC_PROTOCOL_SYNC_WORD;
    headerBuffer[1] = (uint8_t)(payloadLength >> 8);
    headerBuffer[2] = (uint8_t)payloadLength;
}
