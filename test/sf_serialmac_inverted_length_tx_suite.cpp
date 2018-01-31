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
 * @brief      Serial MAC Tx test suite.
 */

#include <gmock/gmock.h>

#include "sf_serialmac_test.h"
#include "sf_serialmac_inverted_length_test.h"
#include "mocked_mac_callbacks.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::DefaultValue;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SaveArgPointee;
using ::testing::Matcher;
using ::testing::ContainerEq;

/**
 * @brief Test attempt to send with a NULL mac context.
 */
TEST_F(SerialMacInvertedLengthTest, SendFrameWrongMacCtxt) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[3] = {0xaa, 0xbb, 0xcc};

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(NULL, 3, txBuffer, 3);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

/**
 * @brief Test attempt to send with a NULL buffer.
 */
TEST_F(SerialMacInvertedLengthTest, SendFrameWrongBuffer) {

    sf_serialmac_return macRet;

    InitSerialMac();

    macRet = sf_serialmac_tx_frame(serialMacCtxt, 0, NULL, 0);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

/**
 * @brief Test empty frame payload edge case.
 */
TEST_F(SerialMacInvertedLengthTest, SendEmptyFrame) {

    sf_serialmac_return macRet;
    uint8_t txPayloadBuffer[0];
    uint16_t txPayloadLength = 0;
    std::vector<uint8_t> txPayload;

    SetupFrameCrc(txPayloadBuffer, txPayloadLength);
    SetupFrameHeader(txPayloadLength);
    InitSerialMac();

    {
        InSequence seq;
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, _, macHeaderFieldLength))
            .WillOnce(Return(macHeaderFieldLength));
        EXPECT_CALL(macCallbacksMock, WriteBufferCb(_, _, txPayloadLength))
            .Times(1);
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, _, macCrcFieldLength))
            .WillOnce(Return(macCrcFieldLength));
        EXPECT_CALL(macCallbacksMock, WriteFrameCb(_, _, txPayloadLength))
            .Times(1);
    }

    macRet = sf_serialmac_tx_frame(serialMacCtxt, txPayloadLength, txPayloadBuffer, txPayloadLength);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx failed";

    macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Frame tx callback failed";
}

/**
 * @brief Test max possible frame payload edge case.
 */
TEST_F(SerialMacInvertedLengthTest, SendMaxSizeFrame) {

    sf_serialmac_return macRet;
    uint16_t txPayloadLength = UINT16_MAX;
    uint8_t txPayloadBuffer[txPayloadLength];

    for(uint16_t i=0; i<txPayloadLength; i++) {
        txPayloadBuffer[i] = (uint8_t)i;
    }

    SetupFrameHeader(txPayloadLength);
    SetupFrameCrc(txPayloadBuffer, txPayloadLength);
    InitSerialMac();

    {
        InSequence seq;
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(headerBuffer, macHeaderFieldLength), macHeaderFieldLength))
            .WillOnce(Return(macHeaderFieldLength));
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(txPayloadBuffer, txPayloadLength), txPayloadLength))
            .WillOnce(Return(txPayloadLength));
        EXPECT_CALL(macCallbacksMock, WriteBufferCb(_, BufferIsEq(txPayloadBuffer, txPayloadLength), txPayloadLength))
            .Times(1);
        EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(crcBuffer, macCrcFieldLength), macCrcFieldLength))
            .WillOnce(Return(macCrcFieldLength));
        EXPECT_CALL(macCallbacksMock, WriteFrameCb(_, _, txPayloadLength))
            .Times(1);
    }

    macRet = sf_serialmac_tx_frame(serialMacCtxt, txPayloadLength, txPayloadBuffer, txPayloadLength);
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
TEST_F(SerialMacInvertedLengthTest, SendFrames) {

    sf_serialmac_return macRet;
    uint16_t txPayloadLength = MAX_TEST_PAYLOAD_LEN;
    uint8_t txPayloadBuffer[txPayloadLength];

    InitSerialMac();

    for(uint16_t i=0; i<txPayloadLength; i++) {
        txPayloadBuffer[i] = (uint8_t)i;

        SetupFrameHeader(i+1);
        SetupFrameCrc(txPayloadBuffer, i+1);

        {
            InSequence seq;
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(headerBuffer, macHeaderFieldLength), macHeaderFieldLength))
                .WillOnce(Return(macHeaderFieldLength));
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(txPayloadBuffer, i+1), i+1))
                .WillOnce(Return(i+1));
            EXPECT_CALL(macCallbacksMock, WriteBufferCb(_, BufferIsEq(txPayloadBuffer, i+1), i+1))
                .Times(1);
            EXPECT_CALL(macCallbacksMock, HalWriteCb(_, BufferIsEq(crcBuffer, macCrcFieldLength), macCrcFieldLength))
                .WillOnce(Return(macCrcFieldLength));
            EXPECT_CALL(macCallbacksMock, WriteFrameCb(_,_,i+1))
                .Times(1);
        }

        macRet = sf_serialmac_tx_frame(serialMacCtxt, i+1, txPayloadBuffer, i+1);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx failed";

        macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx callback failed";
    }
}
