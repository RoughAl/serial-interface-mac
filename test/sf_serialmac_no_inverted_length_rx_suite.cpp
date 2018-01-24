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
 * @brief      Iverted length Serial MAC Rx test suite.
 */

#include "sf_serialmac_no_inverted_length_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

/**
 * @brief Test max possible frame payload edge case.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveMaxSizeFrame) {

    sf_serialmac_return macRet;
    std::vector<uint8_t> rxExpectedPayload;
    uint16_t i;

    for(i=0; i<UINT16_MAX; i++) {
        rxExpectedPayload.push_back((uint8_t)i);
    }

    SetupHalBuffer(rxExpectedPayload);
    InitSerialMac();

    macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "HAL Rx callback failed";

    EXPECT_EQ(rxExpectedPayload, SerialMacNoInvertedLengthTest::rxPayload)
    << "Received unexpected payload";
}

/**
 * @brief Test correct frame payloads are received for frames sizes
 *        from 1 to MAX_TEST_PAYLOAD_LEN.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveFrames) {

    sf_serialmac_return macRet;
    std::vector<uint8_t> rxExpectedPayload;
    int i;

    InitSerialMac();

    for(i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        rxExpectedPayload.push_back((uint8_t)i);
        SetupHalBuffer(rxExpectedPayload);

        macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "HAL Rx callback failed";

        EXPECT_EQ(rxExpectedPayload, SerialMacNoInvertedLengthTest::rxPayload)
        << "Received unexpected payload";
    }
}

/**
 * @brief Test receiving frames with wrong crc.
 */
TEST_F(SerialMacNoInvertedLengthTest, ReceiveFramesWrongCrc) {

    sf_serialmac_return macRet;
    std::vector<uint8_t> rxPayload;
    uint16_t i;

    for(i=0; i<UINT16_MAX; i++) {
        rxPayload.push_back((uint8_t)i);
    }

    SetupHalBuffer(rxPayload);
    halBuffer.at(halBuffer.back())--;
    InitSerialMac();

    macRet = sf_serialmac_hal_rx_callback(serialMacCtxt);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "HAL Rx callback failed";

    EXPECT_EQ(macError, SF_SERIALMAC_ERROR_INVALID_CRC)
    << "Crc field verification should have failed";
}
