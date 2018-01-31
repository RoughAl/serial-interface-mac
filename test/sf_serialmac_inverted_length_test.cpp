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

#include "sf_serialmac_inverted_length_test.h"
#include "sf_crc.h"

const uint SerialMacInvertedLengthTest::macHeaderFieldLength = SF_SERIALMAC_PROTOCOL_HEADER_LEN_EXTENDED;
const uint SerialMacInvertedLengthTest::macLengthFieldLength = SF_SERIALMAC_PROTOCOL_HEADER_LEN_EXTENDED - macSyncWordFieldLength;

SerialMacInvertedLengthTest::SerialMacInvertedLengthTest() {
    invertedLengthField = true;
    headerBuffer = (uint8_t*)std::malloc(macHeaderFieldLength);
}

SerialMacInvertedLengthTest::~SerialMacInvertedLengthTest() {
    std::free(headerBuffer);
}

void SerialMacInvertedLengthTest::SetupFrameHeader(uint16_t payloadLength) {
    headerBuffer[0] = SF_SERIALMAC_PROTOCOL_SYNC_WORD;
    headerBuffer[1] = (uint8_t)(payloadLength >> 8);
    headerBuffer[2] = (uint8_t)payloadLength;
    headerBuffer[3] = (uint8_t)(~payloadLength >> 8);
    headerBuffer[4] = (uint8_t)~payloadLength;
}
