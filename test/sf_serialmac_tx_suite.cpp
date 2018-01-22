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
 * @brief      Serial MAC unit tests class
 */

#include "sf_serialmac_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

TEST_F(SerialMacTest, SendFrameWrongMacCtxt) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[3] = {0xAA, 0xBB, 0xCC};

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(NULL, 3, txBuffer, 3);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

TEST_F(SerialMacTest, SendFrameWrongBuffer) {

    sf_serialmac_return macRet;

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(serialMacCtxt, 3, NULL, 3);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

TEST_F(SerialMacTest, SendFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[MAX_TEST_PAYLOAD_LEN];
    std::vector<uint8_t> expectedTxFullBufferVector;
    uint16_t i;
    uint16_t expectedCrc;

    InitSerialMac();

    for(i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        txBuffer[i] = (uint8_t)i;
        expectedCrc = crc_calc_finalize(txBuffer, i+1);

        SerialMacTest::fullSentTestBuffer.clear();
        expectedTxFullBufferVector.clear();
        expectedTxFullBufferVector.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
        expectedTxFullBufferVector.push_back((i+1) >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(i+1));
        expectedTxFullBufferVector.push_back(0xff - ((i+1) >> 8));
        expectedTxFullBufferVector.push_back(0xff - ((uint8_t)(i+1)));
        expectedTxFullBufferVector.insert(expectedTxFullBufferVector.end(), txBuffer, txBuffer+i+1);
        expectedTxFullBufferVector.push_back(expectedCrc >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(expectedCrc));

        macRet = sf_serialmac_tx_frame(serialMacCtxt, i+1, txBuffer, i+1);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx failed";

        macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx callback failed";

        EXPECT_EQ(SerialMacTest::fullSentTestBuffer, expectedTxFullBufferVector)
        << "Unexpected frame sent";
    }
}
