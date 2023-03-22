#include "cosmos-crypto.h"
#include <string.h>

namespace Artemis
{
    namespace Teensy
    {
        Crypto::Crypto(const char *key)
        {
            gcm.setKey((uint8_t *)key, strlen(key));
        }

        Crypto::Crypto() {}

        Crypto::~Crypto()
        {
            gcm.clear();
        }

        int32_t Crypto::setKey(const char *key)
        {
            gcm.setKey((uint8_t *)key, strlen(key));
            return 0;
        }

        int32_t Crypto::setIV(vector<uint8_t> iv)
        {
            this->iv = iv;
            return 0;
        }

        int32_t Crypto::randomizeIV(uint8_t size)
        {
            RNG.rand(iv.data(), size);
            return 0;
        }

        int32_t Crypto::encrypt(vector<uint8_t> plaintext_str, vector<uint8_t> &ciphertext_str)
        {
            vector<uint8_t> plaintext;

            ciphertext_str.resize(plaintext_str.size() + iv.size());
            gcm.encrypt(ciphertext_str.data(), plaintext_str.data(), plaintext_str.size());
            ciphertext_str.insert(ciphertext_str.end(), iv.begin(), iv.end());

            return 0;
        }

        int32_t Crypto::decrypt(vector<uint8_t> ciphertext_str, vector<uint8_t> &plaintext_str)
        {
            plaintext_str.resize(ciphertext_str.size() - iv.size());
            gcm.decrypt(plaintext_str.data(), ciphertext_str.data(), ciphertext_str.size() - iv.size());

            return 0;
        }

    }
}