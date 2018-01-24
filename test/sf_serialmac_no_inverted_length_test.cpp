/**
 * @code
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions.==============
 * @endcode
 *
 * @file
 * @copyright  STACKFORCE GmbH, Heitersheim, Germany, http://www.stackforce.de
 * @author     Adrian Antonana <adrian.antonana@stackforce.de>
 * @brief      Serial MAC unit tests class with disabled inverted length field
 */

#include "sf_serialmac_no_inverted_length_test.h"
#include "sf_crc.h"

SerialMacNoInvertedLengthTest::SerialMacNoInvertedLengthTest() {
    invertedLengthField = false;
}

SerialMacNoInvertedLengthTest::~SerialMacNoInvertedLengthTest() {}

void SerialMacNoInvertedLengthTest::SetupHalBuffer(const std::vector<uint8_t> payload) {
    uint16_t payloadSize = payload.size();
    uint8_t payloadBuff[payloadSize];
    uint16_t crc;

    int i;
    for(i=0; i<payloadSize; i++) {
        payloadBuff[i] = payload[i];
    }

    crc = crc_calc_finalize(payloadBuff, payloadSize);
    halBuffer.clear();
    halBuffer.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
    halBuffer.push_back((uint8_t)(payloadSize >> 8));
    halBuffer.push_back((uint8_t)payloadSize);
    halBuffer.insert(halBuffer.end(), payload.begin(), payload.end());
    halBuffer.push_back((uint8_t)(crc >> 8));
    halBuffer.push_back((uint8_t)crc);
    itHalBuffer = halBuffer.begin();
}
