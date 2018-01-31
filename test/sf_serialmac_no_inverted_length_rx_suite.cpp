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
 * @brief      Iverted length Serial MAC Rx test suite.
 */

#include "sf_serialmac_no_inverted_length_test.h"
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
 * @brief Test max possible frame payload edge case.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveMaxSizeFrame) {

    sf_serialmac_return macRet;
    uint8_t payloadBuff[UINT16_MAX];
    uint16_t payloadLength = UINT16_MAX;


    for(uint16_t i=0; i<payloadLength; i++) {
        payloadBuff[i] = (uint8_t)i;
    }

    SetupFrameHeader(payloadLength);
    SetupFrameCrc(payloadBuff, payloadLength);

    {
        InSequence seq;
        EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
            .WillOnce(
                Return(payloadLength +
                        macHeaderFieldLength +
                        macCrcFieldLength
                )
            );

        EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macSyncWordFieldLength))
            .WillOnce(
                DoAll(AssignBuffer(headerBuffer),
                      Return(macSyncWordFieldLength)
                )
            );

        EXPECT_CALL(macCallbacksMock, ReadSyncByteCb(_, _, _))
            .Times(1);

        EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
            .WillOnce(
                Return(payloadLength +
                        macLengthFieldLength +
                        macCrcFieldLength
                )
            );

        EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macLengthFieldLength))
            .WillOnce(
                DoAll(
                    AssignBuffer(headerBuffer+macSyncWordFieldLength),
                    Return(macLengthFieldLength)
                )
            );
        EXPECT_CALL(macCallbacksMock, ReadBufferCb(_, _, payloadLength))
            .WillOnce(
                AllocateAndReceiveBuffer(&payloadBuffer)
            );

        EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
            .WillOnce(
                Return(payloadLength + macCrcFieldLength)
            );

        EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, payloadLength))
            .WillOnce(
                DoAll(
                    AssignBuffer(payloadBuff),
                    Return(payloadLength)
                )
            );

        EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
            .WillOnce(
                Return(macCrcFieldLength)
            );

        EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macCrcFieldLength))
            .WillOnce(
                DoAll(
                    AssignBuffer(crcBuffer),
                    Return(macCrcFieldLength)
                )
            );

        EXPECT_CALL(macCallbacksMock, ReadFrameCb(_, _, payloadLength))
            .WillOnce(
                FreeBuffer(&payloadBuffer)
            );
    }

    InitSerialMac();

    macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "HAL Rx callback failed";
}

/**
 * @brief Test correct frame payloads are received for frames sizes
 *        from 1 to MAX_TEST_PAYLOAD_LEN.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveFrames) {

    sf_serialmac_return macRet;
    uint8_t payloadBuff[MAX_TEST_PAYLOAD_LEN];
    uint16_t payloadLength;

    InitSerialMac();

    for(uint16_t i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        payloadBuff[i] = (uint8_t)i;
        payloadLength = i+1;
        SetupFrameHeader(payloadLength);
        SetupFrameCrc(payloadBuff, payloadLength);

        {
            InSequence seq;
            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength +
                            macHeaderFieldLength +
                            macCrcFieldLength
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macSyncWordFieldLength))
                .WillOnce(
                    DoAll(AssignBuffer(headerBuffer),
                        Return(macSyncWordFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, ReadSyncByteCb(_, _, _))
                .Times(1);

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength +
                            macLengthFieldLength +
                            macCrcFieldLength
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macLengthFieldLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(headerBuffer + macSyncWordFieldLength),
                        Return(macLengthFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, ReadBufferCb(_, _, payloadLength))
                .WillOnce(
                    AllocateAndReceiveBuffer(&payloadBuffer)
                );

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength + macCrcFieldLength)
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, payloadLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(payloadBuff),
                        Return(payloadLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(macCrcFieldLength)
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macCrcFieldLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(crcBuffer),
                        Return(macCrcFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, ReadFrameCb(_, _, payloadLength))
                .WillOnce(
                    FreeBuffer(&payloadBuffer)
                );
        }

        macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "HAL Rx callback failed";
    }
}

/**
 * @brief Test receiving frames with wrong crc.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveFramesWrongCrc) {

    sf_serialmac_return macRet;
    uint8_t payloadBuff[MAX_TEST_PAYLOAD_LEN];
    uint16_t payloadLength;

    InitSerialMac();

    for(uint16_t i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        payloadBuff[i] = (uint8_t)i;
        payloadLength = i+1;
        SetupFrameHeader(payloadLength);
        SetupFrameCrc(payloadBuff, payloadLength);
        crcBuffer[0]--;

        {
            InSequence seq;
            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength +
                            macHeaderFieldLength +
                            macCrcFieldLength
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macSyncWordFieldLength))
                .WillOnce(
                    DoAll(AssignBuffer(headerBuffer),
                        Return(macSyncWordFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, ReadSyncByteCb(_, _, _))
                .Times(1);

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength +
                            macLengthFieldLength +
                            macCrcFieldLength
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macLengthFieldLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(headerBuffer + macSyncWordFieldLength),
                        Return(macLengthFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, ReadBufferCb(_, _, payloadLength))
                .WillOnce(
                    AllocateAndReceiveBuffer(&payloadBuffer)
                );

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(payloadLength + macCrcFieldLength)
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, payloadLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(payloadBuff),
                        Return(payloadLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, HalReadWaitingCb(_))
                .WillOnce(
                    Return(macCrcFieldLength)
                );

            EXPECT_CALL(macCallbacksMock, HalReadCb(_, _, macCrcFieldLength))
                .WillOnce(
                    DoAll(
                        AssignBuffer(crcBuffer),
                        Return(macCrcFieldLength)
                    )
                );

            EXPECT_CALL(macCallbacksMock, MacErrorCb(_, SF_SERIALMAC_ERROR_INVALID_CRC))
                .WillOnce(
                    FreeBuffer(&payloadBuffer)
                );
        }

        macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "HAL Rx callback failed";
    }
}
