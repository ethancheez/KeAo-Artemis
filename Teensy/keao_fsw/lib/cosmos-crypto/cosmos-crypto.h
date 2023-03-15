#ifndef _CRYPTO_H
#define _CRYPTO_H

#include <GCM.h>
#include <AES.h>
#include <RNG.h>
#include <support/configCosmosKernel.h>
#include <support/cosmos-errno.h>

namespace Artemis
{
    namespace Teensy
    {
        class Crypto
        {
        public:
            static std::atomic<uint32_t> prev_sequence;

            Crypto(const char *key);
            Crypto();
            ~Crypto();
            int32_t setKey(const char *key);
            int32_t setIV(vector<uint8_t> iv);
            int32_t randomizeIV(uint8_t size);
            int32_t encrypt(vector<uint8_t> plaintext_str, vector<uint8_t> &ciphertext_str);
            int32_t decrypt(vector<uint8_t> ciphertext_str, vector<uint8_t> &plaintext_str);

        private:
            GCM<AES256> gcm;
            vector<uint8_t> iv;
        };
    }
}

#endif // _CRYPTO_H
