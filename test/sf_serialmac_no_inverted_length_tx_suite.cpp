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

    std::vector<uint8_t> txExpectedPayload;
    SetupHalBuffer(txExpectedPayload);
    InitSerialMac();

    macRet = sf_serialmac_tx_frame(serialMacCtxt, 0, txBuffer, 0);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx failed";

    macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx callback failed";

    EXPECT_EQ(fullSentTestBuffer, halBuffer)
    << "Unexpected frame sent";
}

/**
 * @brief Test max possible frame payload edge case.
 */
TEST_F(SerialMacNoInvertedLengthTest, SendMaxSizeFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[UINT16_MAX];

    for(uint16_t i=0; i<UINT16_MAX; i++) {
        txBuffer[i] = (uint8_t)i;
    }

    SetupFrameHeader(UINT16_MAX);
    SetupFrameCrc(txBuffer, UINT16_MAX);
    InitSerialMac();

    {
        InSequence seq;
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(headerBuffer, SF_SERIALMAC_PROTOCOL_HEADER_LEN), SF_SERIALMAC_PROTOCOL_HEADER_LEN))
            .WillOnce(Return(SF_SERIALMAC_PROTOCOL_HEADER_LEN));
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(txBuffer, UINT16_MAX), UINT16_MAX))
            .WillOnce(Return(UINT16_MAX));
        EXPECT_CALL(macCallbacksMock, WriteBufferCb(_, BufferIsEq(txBuffer, UINT16_MAX), UINT16_MAX))
            .Times(1);
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(crcBuffer, SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN), SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN))
            .WillOnce(Return(SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN));
        EXPECT_CALL(macCallbacksMock, WriteFrameCb(_, _, UINT16_MAX))
            .Times(1);
    }

    macRet = sf_serialmac_tx_frame(serialMacCtxt, UINT16_MAX, txBuffer, UINT16_MAX);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx failed";

    macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx callback failed";
}

/**
 * @brief Test correct mac frames are handled to the HAL callback.
 *        The test produces frame payloads with size from 1 to MAX_TEST_PAYLOAD_LEN
 *        and determines that for each generated payload the mac produces the correct
 *        frame header, payload and CRC.
 */
TEST_F(SerialMacNoInvertedLengthTest, SendFrames) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[MAX_TEST_PAYLOAD_LEN];

    InitSerialMac();

    for(uint16_t i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        txBuffer[i] = (uint8_t)i;

        SetupFrameHeader(i+1);
        SetupFrameCrc(txBuffer, i+1);

        {
            InSequence seq;
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(headerBuffer, SF_SERIALMAC_PROTOCOL_HEADER_LEN), SF_SERIALMAC_PROTOCOL_HEADER_LEN))
                .WillOnce(Return(SF_SERIALMAC_PROTOCOL_HEADER_LEN));
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(txBuffer, i+1), i+1))
                .WillOnce(Return(i+1));
            EXPECT_CALL(macCallbacksMock, WriteBufferCb(_, BufferIsEq(txBuffer, i+1), i+1))
                .Times(1);
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(crcBuffer, SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN), SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN))
                .WillOnce(Return(SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN));
            EXPECT_CALL(macCallbacksMock, WriteFrameCb(_,_,i+1))
                .Times(1);
        }

        macRet = sf_serialmac_tx_frame(serialMacCtxt, i+1, txBuffer, i+1);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx failed";

        macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx callback failed";
    }
}
