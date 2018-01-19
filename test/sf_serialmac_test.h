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

#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "sf_serialmac.h"

#define MAX_TEST_PAYLOAD_LEN 10000
// The fixture for testing class SerialMacTest
class SerialMacTest : public ::testing::Test {

    public:
        static std::vector<uint8_t> fullSentTestBuffer;
        static bool testingMacInit;

        static void ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
        static void ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
        static void WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
        static void WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
        static void ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
        static void HalReadWaitingCb(void *portHandle);
        static void HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize);
        static size_t HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize);
        static void MacErrorCb(void*, sf_serialmac_error e);

    protected:

        // You can do set-up work for each test here.
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

        struct sf_serialmac_ctx *serialMacCtxt;
        bool invertedLengthField;
        void *dummyPortHandle;
        void InitSerialMac();

};

