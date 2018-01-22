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
 * @brief      Configurable Serial MAC Tx test suite with disabled inverted length field.
 */

#include "sf_serialmac_no_inverted_length_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

/**
 * @brief Test empty frame payload edge case.
 */
TEST_F(SerialMacNoInvertedLengthTest, SendEmptyFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[1];
    std::vector<uint8_t> expectedTxFullBufferVector;
    uint16_t expectedCrc;

    expectedCrc = crc_calc_finalize(txBuffer, 0);

    SerialMacNoInvertedLengthTest::fullSentTestBuffer.clear();
    expectedTxFullBufferVector.clear();
    expectedTxFullBufferVector.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
    expectedTxFullBufferVector.push_back(0x00);
    expectedTxFullBufferVector.push_back(0x00);
    expectedTxFullBufferVector.push_back(expectedCrc >> 8);
    expectedTxFullBufferVector.push_back((uint8_t)(expectedCrc));

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(serialMacCtxt, 0, txBuffer, 0);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx failed";

    macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx callback failed";

    EXPECT_EQ(SerialMacNoInvertedLengthTest::fullSentTestBuffer, expectedTxFullBufferVector)
    << "Unexpected frame sent";
}

/**
 * @brief Test max possible frame payload edge case.
 */
TEST_F(SerialMacNoInvertedLengthTest, SendMaxSizeFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[UINT16_MAX];
    std::vector<uint8_t> expectedTxFullBufferVector;
    uint16_t expectedCrc;
    uint16_t i;

    for(i=0; i<UINT16_MAX; i++) {
        txBuffer[i] = (uint8_t)i;
    }

    expectedCrc = crc_calc_finalize(txBuffer, UINT16_MAX);

    SerialMacNoInvertedLengthTest::fullSentTestBuffer.clear();
    expectedTxFullBufferVector.clear();
    expectedTxFullBufferVector.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
    expectedTxFullBufferVector.push_back(0xff);
    expectedTxFullBufferVector.push_back(0xff);
    expectedTxFullBufferVector.insert(expectedTxFullBufferVector.end(), txBuffer, txBuffer+UINT16_MAX);
    expectedTxFullBufferVector.push_back(expectedCrc >> 8);
    expectedTxFullBufferVector.push_back((uint8_t)(expectedCrc));

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(serialMacCtxt, UINT16_MAX, txBuffer, UINT16_MAX);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx failed";

    macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx callback failed";

    EXPECT_EQ(SerialMacNoInvertedLengthTest::fullSentTestBuffer, expectedTxFullBufferVector)
    << "Unexpected frame sent";
}

/**
 * @brief Test correct mac frames are handled to the HAL callback.
 *        The test produces frame payloads with size from 1 to MAX_TEST_PAYLOAD_LEN
 *        and determines that for each generated payload the mac produces the correct
 *        frame header, payload and CRC.
 */
TEST_F(SerialMacNoInvertedLengthTest, SendFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[MAX_TEST_PAYLOAD_LEN];
    std::vector<uint8_t> expectedTxFullBufferVector;
    uint16_t i;
    uint16_t expectedCrc;

    InitSerialMac();

    for(i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        txBuffer[i] = (uint8_t)i;
        expectedCrc = crc_calc_finalize(txBuffer, i+1);

        SerialMacNoInvertedLengthTest::fullSentTestBuffer.clear();
        expectedTxFullBufferVector.clear();
        expectedTxFullBufferVector.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
        expectedTxFullBufferVector.push_back((i+1) >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(i+1));
        expectedTxFullBufferVector.insert(expectedTxFullBufferVector.end(), txBuffer, txBuffer+i+1);
        expectedTxFullBufferVector.push_back(expectedCrc >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(expectedCrc));

        macRet = sf_serialmac_tx_frame(serialMacCtxt, i+1, txBuffer, i+1);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx failed";

        macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx callback failed";

        EXPECT_EQ(SerialMacNoInvertedLengthTest::fullSentTestBuffer, expectedTxFullBufferVector)
        << "Unexpected frame sent";
    }
}
