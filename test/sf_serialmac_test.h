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

#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "mocked_mac_callbacks.h"
#include "sf_serialmac.h"

#define MAX_TEST_PAYLOAD_LEN 1000

/**
 * The base test class is an interface class, therefore it may not be used directly
 * as a test fixture.
 * Child classes inheriting from this one have to implement the SetupHalBuffer() method.
 */
class SerialMacTest : public ::testing::Test {

    public:
        static std::vector<uint8_t> fullSentTestBuffer;
        static size_t halRxBytesWaiting;
        static std::vector<uint8_t> halBuffer;
        static std::vector<uint8_t>::iterator itHalBuffer;
        static uint8_t *payloadBuffer;
        static std::vector<uint8_t> rxPayload;
        static enum sf_serialmac_error macError;

    protected:
        MockMacCallbacks macCallbacksMock;

        // You can do set-up work for each test here.
        uint8_t *headerBuffer;
        uint8_t *crcBuffer;
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

        virtual void SetupFrameHeader(uint16_t payloadLength) = 0;

        // The serial MAC context.
        struct sf_serialmac_ctx *serialMacCtxt;

        // Enable/Disable the inverted length field.
        bool invertedLengthField;

        // Dummy HAL port handle.
        int dummyPortHandle;

        // Initialize the serial mac.
        void InitSerialMac();

        /**
         * Setup a HAL buffer for the given payload. This method has to be implemented by child
         * tests classes depending on the inverted length field having to be activated or not.
         */
        void SetupFrameCrc(uint8_t *payload, uint16_t payloadLength);
        virtual void SetupHalBuffer(const std::vector<uint8_t> payload) = 0;
};

