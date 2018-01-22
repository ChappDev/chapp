//
// Created by Александр on 19.12.17.
//

#ifndef CLIENT_COMMON_AESENCODER_H
#define CLIENT_COMMON_AESENCODER_H


#include "DiffieHellmanWrapper.h"
class DiffieHellmanWrapper;
class AesEncoder {
public:
    static std::string& encrypt(DiffieHellmanWrapper* wrapper, std::string message);
    static std::string& decrypt(DiffieHellmanWrapper* wrapper, std::string message);
};


#endif //CLIENT_COMMON_AESENCODER_H
