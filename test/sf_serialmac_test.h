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
 * @brief      Serial MAC unit tests base class
 */

#ifndef _SF_SERIALMAC_TEST_H_
#define _SF_SERIALMAC_TEST_H_

#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "mocked_mac_callbacks.h"
#include "sf_serialmac.h"

#define MAX_TEST_PAYLOAD_LEN 1000

/**
 * Custom Matchers
 */
bool BufferMatcher(uint8_t *buff, uint8_t *matchBuff, size_t matchBuffLen);

MATCHER_P2(BufferIsEq, matchBuff, matchBuffLen, "") {
    return BufferMatcher(arg, matchBuff, matchBuffLen);
}

/**
 * Custom Actions
 */
ACTION_P(AssignBuffer, srcBuffer) {
    for(int i=0; i<arg2; i++) {
        arg1[i] = srcBuffer[i];
    }
}

ACTION_P(AllocateAndReceiveBuffer, payloadBuffer) {
    *payloadBuffer = (uint8_t*)std::malloc(sizeof(uint8_t)*arg2);
    sf_serialmac_rx_frame(arg0, *payloadBuffer, arg2);
}

ACTION_P(FreeBuffer, buffer) {
    if(*buffer) {
        std::free(*buffer);
    }
}

 * The base test class is an interface class, therefore it may not be used directly
 * as a test fixture.
 * Child classes inheriting from this one have to implement the pure virtual methods.
 */
class SerialMacTest : public ::testing::Test {

    public:
        static const uint macSyncWordFieldLength;
        static const uint macCrcFieldLength;
        static std::vector<uint8_t> fullSentTestBuffer;
        static size_t halRxBytesWaiting;
        static std::vector<uint8_t> halBuffer;
        static std::vector<uint8_t>::iterator itHalBuffer;
        static uint8_t *payloadBuffer;
        static std::vector<uint8_t> rxPayload;
        static enum sf_serialmac_error macError;

    protected:
        MockMacCallbacks macCallbacksMock;
        uint8_t *headerBuffer;
        uint8_t *crcBuffer;

        /** Base MAC test calss constructor.
         *
         * Allocates a serial MAC context and frame crc buffer, enables the inverted
         * frame header length field.
         * Sets a reference to the mocked mac callbacks class MockMacCallbacks.
         */
        SerialMacTest();

        // You can do clean-up work that doesn't throw exceptions here.
        virtual ~SerialMacTest();

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        // Code here will be called immediately after the constructor (right
        // before each test).
        virtual void SetUp();

        // Code here will be called immediately after each test (right
        // before the destructor).
        virtual void TearDown();

        /**
         * Set up the headerBuffer. Child classes have to implement this method with the
         * according inverted length field.
         */
        virtual void SetupFrameHeader(uint16_t payloadLength) = 0;

        // The serial MAC context.
        struct sf_serialmac_ctx *serialMacCtxt;

        // Enable/Disable the inverted length field.
        bool invertedLengthField;

        // Dummy HAL port handle.
        int dummyPortHandle;

        /**
         * Initialize the serial mac callback functions.
         */
        void InitSerialMac();

        /**
         */
        void SetupFrameCrc(uint8_t *payload, uint16_t payloadLength);
};

#endif
